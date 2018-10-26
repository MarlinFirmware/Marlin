/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * fwretract.cpp - Implement firmware-based retraction
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(FWRETRACT)

#include "fwretract.h"

FWRetract fwretract; // Single instance - this calls the constructor

#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/stepper.h"

#if ENABLED(RETRACT_SYNC_MIXING)
  #include "../feature/mixing.h"
#endif

// private:

#if EXTRUDERS > 1
  bool FWRetract::retracted_swap[EXTRUDERS];          // Which extruders are swap-retracted
#endif

// public:

fwretract_settings_t FWRetract::settings;             // M207 S F Z W, M208 S F W R

#if ENABLED(FWRETRACT_AUTORETRACT)
  bool FWRetract::autoretract_enabled;                // M209 S - Autoretract switch
#endif

bool FWRetract::retracted[EXTRUDERS];                 // Which extruders are currently retracted

float FWRetract::current_retract[EXTRUDERS],          // Retract value used by planner
      FWRetract::current_hop;

void FWRetract::reset() {
  #if ENABLED(FWRETRACT_AUTORETRACT)
    autoretract_enabled = false;
  #endif
  settings.retract_length = RETRACT_LENGTH;
  settings.retract_feedrate_mm_s = RETRACT_FEEDRATE;
  settings.retract_zraise = RETRACT_ZRAISE;
  settings.retract_recover_length = RETRACT_RECOVER_LENGTH;
  settings.retract_recover_feedrate_mm_s = RETRACT_RECOVER_FEEDRATE;
  settings.swap_retract_length = RETRACT_LENGTH_SWAP;
  settings.swap_retract_recover_length = RETRACT_RECOVER_LENGTH_SWAP;
  settings.swap_retract_recover_feedrate_mm_s = RETRACT_RECOVER_FEEDRATE_SWAP;
  current_hop = 0.0;

  for (uint8_t i = 0; i < EXTRUDERS; ++i) {
    retracted[i] = false;
    #if EXTRUDERS > 1
      retracted_swap[i] = false;
    #endif
    current_retract[i] = 0.0;
  }
}

/**
 * Retract or recover according to firmware settings
 *
 * This function handles retract/recover moves for G10 and G11,
 * plus auto-retract moves sent from G0/G1 when E-only moves are done.
 *
 * To simplify the logic, doubled retract/recover moves are ignored.
 *
 * Note: Auto-retract will apply the set Z hop in addition to any Z hop
 *       included in the G-code. Use M207 Z0 to to prevent double hop.
 */
void FWRetract::retract(const bool retracting
  #if EXTRUDERS > 1
    , bool swapping /* =false */
  #endif
) {
  // Prevent two retracts or recovers in a row
  if (retracted[active_extruder] == retracting) return;

  // Prevent two swap-retract or recovers in a row
  #if EXTRUDERS > 1
    // Allow G10 S1 only after G10
    if (swapping && retracted_swap[active_extruder] == retracting) return;
    // G11 priority to recover the long retract if activated
    if (!retracting) swapping = retracted_swap[active_extruder];
  #else
    constexpr bool swapping = false;
  #endif

  /* // debugging
    SERIAL_ECHOLNPAIR("retracting ", retracting);
    SERIAL_ECHOLNPAIR("swapping ", swapping);
    SERIAL_ECHOLNPAIR("active extruder ", active_extruder);
    for (uint8_t i = 0; i < EXTRUDERS; ++i) {
      SERIAL_ECHOPAIR("retracted[", i);
      SERIAL_ECHOLNPAIR("] ", retracted[i]);
      #if EXTRUDERS > 1
        SERIAL_ECHOPAIR("retracted_swap[", i);
        SERIAL_ECHOLNPAIR("] ", retracted_swap[i]);
      #endif
    }
    SERIAL_ECHOLNPAIR("current_position[z] ", current_position[Z_AXIS]);
    SERIAL_ECHOLNPAIR("current_position[e] ", current_position[E_AXIS]);
    SERIAL_ECHOLNPAIR("current_hop ", current_hop);
  //*/

  const float old_feedrate_mm_s = feedrate_mm_s,
              unscale_e = RECIPROCAL(planner.e_factor[active_extruder]),
              unscale_fr = 100.0 / feedrate_percentage, // Disable feedrate scaling for retract moves
              base_retract = (
                (swapping ? settings.swap_retract_length : settings.retract_length)
                #if ENABLED(RETRACT_SYNC_MIXING)
                  * (MIXING_STEPPERS)
                #endif
              );

  // The current position will be the destination for E and Z moves
  set_destination_from_current();

  #if ENABLED(RETRACT_SYNC_MIXING)
    uint8_t old_mixing_tool = mixer.get_current_v_tool();
    mixer.T(MIXER_AUTORETRACT_TOOL);
  #endif

  if (retracting) {
    // Retract by moving from a faux E position back to the current E position
    feedrate_mm_s = (
      settings.retract_feedrate_mm_s * unscale_fr
      #if ENABLED(RETRACT_SYNC_MIXING)
        * (MIXING_STEPPERS)
      #endif
    );
    current_retract[active_extruder] = base_retract * unscale_e;
    prepare_move_to_destination();                        // set_current_to_destination
    planner.synchronize();                                // Wait for move to complete

    // Is a Z hop set, and has the hop not yet been done?
    if (settings.retract_zraise > 0.01 && !current_hop) {           // Apply hop only once
      current_hop += settings.retract_zraise;                       // Add to the hop total (again, only once)
      feedrate_mm_s = planner.settings.max_feedrate_mm_s[Z_AXIS] * unscale_fr;  // Maximum Z feedrate
      prepare_move_to_destination();                      // Raise up, set_current_to_destination
      planner.synchronize();                              // Wait for move to complete
    }
  }
  else {
    // If a hop was done and Z hasn't changed, undo the Z hop
    if (current_hop) {
      current_hop = 0.0;
      feedrate_mm_s = planner.settings.max_feedrate_mm_s[Z_AXIS] * unscale_fr;  // Z feedrate to max
      prepare_move_to_destination();                      // Lower Z, set_current_to_destination
      planner.synchronize();                              // Wait for move to complete
    }

    const float extra_recover = swapping ? settings.swap_retract_recover_length : settings.retract_recover_length;
    if (extra_recover != 0.0) {
      current_position[E_AXIS] -= extra_recover;          // Adjust the current E position by the extra amount to recover
      sync_plan_position_e();                             // Sync the planner position so the extra amount is recovered
    }

    current_retract[active_extruder] = 0.0;
    feedrate_mm_s = (
      (swapping ? settings.swap_retract_recover_feedrate_mm_s : settings.retract_recover_feedrate_mm_s) * unscale_fr
      #if ENABLED(RETRACT_SYNC_MIXING)
        * (MIXING_STEPPERS)
      #endif
    );
    prepare_move_to_destination();                        // Recover E, set_current_to_destination
    planner.synchronize();                                // Wait for move to complete
  }

  #if ENABLED(RETRACT_SYNC_MIXING)
    mixer.T(old_mixing_tool);                             // Restore original mixing tool
  #endif

  feedrate_mm_s = old_feedrate_mm_s;                      // Restore original feedrate
  retracted[active_extruder] = retracting;                // Active extruder now retracted / recovered

  // If swap retract/recover update the retracted_swap flag too
  #if EXTRUDERS > 1
    if (swapping) retracted_swap[active_extruder] = retracting;
  #endif

  /* // debugging
    SERIAL_ECHOLNPAIR("retracting ", retracting);
    SERIAL_ECHOLNPAIR("swapping ", swapping);
    SERIAL_ECHOLNPAIR("active_extruder ", active_extruder);
    for (uint8_t i = 0; i < EXTRUDERS; ++i) {
      SERIAL_ECHOPAIR("retracted[", i);
      SERIAL_ECHOLNPAIR("] ", retracted[i]);
      #if EXTRUDERS > 1
        SERIAL_ECHOPAIR("retracted_swap[", i);
        SERIAL_ECHOLNPAIR("] ", retracted_swap[i]);
      #endif
    }
    SERIAL_ECHOLNPAIR("current_position[z] ", current_position[Z_AXIS]);
    SERIAL_ECHOLNPAIR("current_position[e] ", current_position[E_AXIS]);
    SERIAL_ECHOLNPAIR("current_hop ", current_hop);
  //*/
}

#endif // FWRETRACT
