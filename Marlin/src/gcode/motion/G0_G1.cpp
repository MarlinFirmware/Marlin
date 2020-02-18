/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../gcode.h"
#include "../../module/motion.h"

#include "../../MarlinCore.h"

#if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)
  #include "../../feature/fwretract.h"
#endif

#include "../../sd/cardreader.h"

#if ENABLED(NANODLP_Z_SYNC)
  #include "../../module/stepper.h"
#endif

extern xyze_pos_t destination;

#if ENABLED(VARIABLE_G0_FEEDRATE)
  feedRate_t fast_move_feedrate = MMM_TO_MMS(G0_FEEDRATE);
#endif

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
void GcodeSuite::G0_G1(
  #if IS_SCARA || defined(G0_FEEDRATE)
    const bool fast_move/*=false*/
  #endif
) {

  if (IsRunning()
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      && !axis_unhomed_error(
          (parser.seen('X') ? _BV(X_AXIS) : 0)
        | (parser.seen('Y') ? _BV(Y_AXIS) : 0)
        | (parser.seen('Z') ? _BV(Z_AXIS) : 0) )
    #endif
  ) {

    #ifdef G0_FEEDRATE
      feedRate_t old_feedrate;
      #if ENABLED(VARIABLE_G0_FEEDRATE)
        if (fast_move) {
          old_feedrate = feedrate_mm_s;             // Back up the (old) motion mode feedrate
          feedrate_mm_s = fast_move_feedrate;       // Get G0 feedrate from last usage
        }
      #endif
    #endif

    get_destination_from_command();                 // Process X Y Z E F parameters

    #ifdef G0_FEEDRATE
      if (fast_move) {
        #if ENABLED(VARIABLE_G0_FEEDRATE)
          fast_move_feedrate = feedrate_mm_s;       // Save feedrate for the next G0
        #else
          old_feedrate = feedrate_mm_s;             // Back up the (new) motion mode feedrate
          feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);  // Get the fixed G0 feedrate
        #endif
      }
    #endif

    #if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)

      if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
        // When M209 Autoretract is enabled, convert E-only moves to firmware retract/recover moves
        if (fwretract.autoretract_enabled && parser.seen('E') && !(parser.seen('X') || parser.seen('Y') || parser.seen('Z'))) {
          const float echange = destination.e - current_position.e;
          // Is this a retract or recover move?
          if (WITHIN(ABS(echange), MIN_AUTORETRACT, MAX_AUTORETRACT) && fwretract.retracted[active_extruder] == (echange > 0.0)) {
            current_position.e = destination.e;       // Hide a G1-based retract/recover from calculations
            sync_plan_position_e();                   // AND from the planner
            return fwretract.retract(echange < 0.0);  // Firmware-based retract/recover (double-retract ignored)
          }
        }
      }

    #endif // FWRETRACT

    #if IS_SCARA
      fast_move ? prepare_fast_move_to_destination() : prepare_move_to_destination();
    #else
      prepare_move_to_destination();
    #endif

    #ifdef G0_FEEDRATE
      // Restore the motion mode feedrate
      if (fast_move) feedrate_mm_s = old_feedrate;
    #endif

    #if ENABLED(NANODLP_Z_SYNC)
      #if ENABLED(NANODLP_ALL_AXIS)
        #define _MOVE_SYNC parser.seenval('X') || parser.seenval('Y') || parser.seenval('Z')  // For any move wait and output sync message
      #else
        #define _MOVE_SYNC parser.seenval('Z')  // Only for Z move
      #endif
      if (_MOVE_SYNC) {
        planner.synchronize();
        SERIAL_ECHOLNPGM(MSG_Z_MOVE_COMP);
      }
    #endif
  }
}
