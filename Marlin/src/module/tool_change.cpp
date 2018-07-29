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

#include "tool_change.h"

#include "motion.h"
#include "planner.h"

#include "../Marlin.h"

#include "../inc/MarlinConfig.h"

#if ENABLED(PARKING_EXTRUDER) && PARKING_EXTRUDER_SOLENOIDS_DELAY > 0
  #include "../gcode/gcode.h" // for dwell()
#endif

#if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE)
  #include "../module/servo.h"
#endif

#if ENABLED(EXT_SOLENOID) && !ENABLED(PARKING_EXTRUDER)
  #include "../feature/solenoid.h"
#endif

#if ENABLED(MK2_MULTIPLEXER)
  #include "../feature/snmm.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "../feature/mixing.h"
#endif

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if HAS_FANMUX
  #include "../feature/fanmux.h"
#endif

#if DO_SWITCH_EXTRUDER

  #if EXTRUDERS > 3
    #define REQ_ANGLES 4
    #define _SERVO_NR (e < 2 ? SWITCHING_EXTRUDER_SERVO_NR : SWITCHING_EXTRUDER_E23_SERVO_NR)
  #else
    #define REQ_ANGLES 2
    #define _SERVO_NR SWITCHING_EXTRUDER_SERVO_NR
  #endif

  void move_extruder_servo(const uint8_t e) {
    constexpr int16_t angles[] = SWITCHING_EXTRUDER_SERVO_ANGLES;
    static_assert(COUNT(angles) == REQ_ANGLES, "SWITCHING_EXTRUDER_SERVO_ANGLES needs " STRINGIFY(REQ_ANGLES) " angles.");
    planner.synchronize();
    #if EXTRUDERS & 1
      if (e < EXTRUDERS - 1)
    #endif
    {
      MOVE_SERVO(_SERVO_NR, angles[e]);
      safe_delay(500);
    }
  }

#endif // DO_SWITCH_EXTRUDER

#if ENABLED(SWITCHING_NOZZLE)

  void move_nozzle_servo(const uint8_t e) {
    const int16_t angles[2] = SWITCHING_NOZZLE_SERVO_ANGLES;
    planner.synchronize();
    MOVE_SERVO(SWITCHING_NOZZLE_SERVO_NR, angles[e]);
    safe_delay(500);
  }

#endif // SWITCHING_NOZZLE

#if ENABLED(PARKING_EXTRUDER)

  void pe_magnet_init() {
    for (uint8_t n = 0; n <= 1; ++n)
      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_magnet(n);
      #else
        pe_deactivate_magnet(n);
      #endif
  }

  void pe_set_magnet(const uint8_t extruder_num, const uint8_t state) {
    switch (extruder_num) {
      case 1: OUT_WRITE(SOL1_PIN, state); break;
      default: OUT_WRITE(SOL0_PIN, state); break;
    }
    #if PARKING_EXTRUDER_SOLENOIDS_DELAY > 0
      gcode.dwell(PARKING_EXTRUDER_SOLENOIDS_DELAY);
    #endif
  }

  inline void parking_extruder_tool_change(const uint8_t tmp_extruder, const float fr_mm_s/*=0.0*/, bool no_move/*=false*/) {
    if (!no_move) {

      const float parkingposx[] = PARKING_EXTRUDER_PARKING_X,
                  midpos = (parkingposx[0] + parkingposx[1]) * 0.5 + hotend_offset[X_AXIS][active_extruder],
                  grabpos = parkingposx[tmp_extruder] + hotend_offset[X_AXIS][active_extruder]
                            + (tmp_extruder == 0 ? -(PARKING_EXTRUDER_GRAB_DISTANCE) : PARKING_EXTRUDER_GRAB_DISTANCE);
      /**
       *  Steps:
       *    1. Raise Z-Axis to give enough clearance
       *    2. Move to park position of old extruder
       *    3. Disengage magnetic field, wait for delay
       *    4. Move near new extruder
       *    5. Engage magnetic field for new extruder
       *    6. Move to parking incl. offset of new extruder
       *    7. Lower Z-Axis
       */

      // STEP 1
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("Starting Autopark");
        if (DEBUGGING(LEVELING)) DEBUG_POS("current position:", current_position);
      #endif
      current_position[Z_AXIS] += PARKING_EXTRUDER_SECURITY_RAISE;
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("(1) Raise Z-Axis ");
        if (DEBUGGING(LEVELING)) DEBUG_POS("Moving to Raised Z-Position", current_position);
      #endif
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
      planner.synchronize();

      // STEP 2
      current_position[X_AXIS] = parkingposx[active_extruder] + hotend_offset[X_AXIS][active_extruder];
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPAIR("(2) Park extruder ", active_extruder);
        if (DEBUGGING(LEVELING)) DEBUG_POS("Moving ParkPos", current_position);
      #endif
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();

      // STEP 3
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("(3) Disengage magnet ");
      #endif
      pe_deactivate_magnet(active_extruder);

      // STEP 4
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("(4) Move to position near new extruder");
      #endif
      current_position[X_AXIS] += (active_extruder == 0 ? 10 : -10); // move 10mm away from parked extruder

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("Moving away from parked extruder", current_position);
      #endif
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();

      // STEP 5
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("(5) Engage magnetic field");
      #endif

      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_magnet(active_extruder); //just save power for inverted magnets
      #endif
      pe_activate_magnet(tmp_extruder);

      // STEP 6
      current_position[X_AXIS] = grabpos + (tmp_extruder == 0 ? (+10) : (-10));
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      current_position[X_AXIS] = grabpos;
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPAIR("(6) Unpark extruder ", tmp_extruder);
        if (DEBUGGING(LEVELING)) DEBUG_POS("Move UnparkPos", current_position);
      #endif
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS]/2, active_extruder);
      planner.synchronize();

      // Step 7
      current_position[X_AXIS] = midpos - hotend_offset[X_AXIS][tmp_extruder];
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("(7) Move midway between hotends");
        if (DEBUGGING(LEVELING)) DEBUG_POS("Move midway to new extruder", current_position);
      #endif
      planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        SERIAL_ECHOLNPGM("Autopark done.");
      #endif
    }
    else { // nomove == true
      // Only engage magnetic field for new extruder
      pe_activate_magnet(tmp_extruder);
      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_magnet(active_extruder); // Just save power for inverted magnets
      #endif
    }
    current_position[Z_AXIS] += hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Applying Z-offset", current_position);
    #endif
  }

#endif // PARKING_EXTRUDER

inline void invalid_extruder_error(const uint8_t e) {
  SERIAL_ECHO_START();
  SERIAL_CHAR('T');
  SERIAL_ECHO_F(e, DEC);
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(MSG_INVALID_EXTRUDER);
}

#if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1

  inline void mixing_tool_change(const uint8_t tmp_extruder) {
    if (tmp_extruder >= MIXING_VIRTUAL_TOOLS)
      return invalid_extruder_error(tmp_extruder);

    // T0-Tnnn: Switch virtual tool by changing the mix
    for (uint8_t j = 0; j < MIXING_STEPPERS; j++)
      mixing_factor[j] = mixing_virtual_tool_mix[tmp_extruder][j];
  }

#endif // MIXING_EXTRUDER && MIXING_VIRTUAL_TOOLS > 1

#if ENABLED(DUAL_X_CARRIAGE)

  inline void dualx_tool_change(const uint8_t tmp_extruder, bool &no_move) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPGM("Dual X Carriage Mode ");
        switch (dual_x_carriage_mode) {
          case DXC_FULL_CONTROL_MODE: SERIAL_ECHOLNPGM("DXC_FULL_CONTROL_MODE"); break;
          case DXC_AUTO_PARK_MODE: SERIAL_ECHOLNPGM("DXC_AUTO_PARK_MODE"); break;
          case DXC_DUPLICATION_MODE: SERIAL_ECHOLNPGM("DXC_DUPLICATION_MODE"); break;
        }
      }
    #endif

    const float xhome = x_home_pos(active_extruder);
    if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE
        && IsRunning()
        && (delayed_move_time || current_position[X_AXIS] != xhome)
    ) {
      float raised_z = current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT;
      #if ENABLED(MAX_SOFTWARE_ENDSTOPS)
        NOMORE(raised_z, soft_endstop_max[Z_AXIS]);
      #endif
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOLNPAIR("Raise to ", raised_z);
          SERIAL_ECHOLNPAIR("MoveX to ", xhome);
          SERIAL_ECHOLNPAIR("Lower to ", current_position[Z_AXIS]);
        }
      #endif
      // Park old head: 1) raise 2) move to park position 3) lower
      for (uint8_t i = 0; i < 3; i++)
        planner.buffer_line(
          i == 0 ? current_position[X_AXIS] : xhome,
          current_position[Y_AXIS],
          i == 2 ? current_position[Z_AXIS] : raised_z,
          current_position[E_AXIS],
          planner.max_feedrate_mm_s[i == 1 ? X_AXIS : Z_AXIS],
          active_extruder
        );
      planner.synchronize();
    }

    // Apply Y & Z extruder offset (X offset is used as home pos with Dual X)
    current_position[Y_AXIS] -= hotend_offset[Y_AXIS][active_extruder] - hotend_offset[Y_AXIS][tmp_extruder];
    current_position[Z_AXIS] -= hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];

    // Activate the new extruder ahead of calling set_axis_is_at_home!
    active_extruder = tmp_extruder;

    // This function resets the max/min values - the current position may be overwritten below.
    set_axis_is_at_home(X_AXIS);

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("New Extruder", current_position);
    #endif

    // Only when auto-parking are carriages safe to move
    if (dual_x_carriage_mode != DXC_AUTO_PARK_MODE) no_move = true;

    switch (dual_x_carriage_mode) {
      case DXC_FULL_CONTROL_MODE:
        // New current position is the position of the activated extruder
        current_position[X_AXIS] = inactive_extruder_x_pos;
        // Save the inactive extruder's position (from the old current_position)
        inactive_extruder_x_pos = destination[X_AXIS];
        break;
      case DXC_AUTO_PARK_MODE:
        // record raised toolhead position for use by unpark
        COPY(raised_parked_position, current_position);
        raised_parked_position[Z_AXIS] += TOOLCHANGE_UNPARK_ZLIFT;
        #if ENABLED(MAX_SOFTWARE_ENDSTOPS)
          NOMORE(raised_parked_position[Z_AXIS], soft_endstop_max[Z_AXIS]);
        #endif
        active_extruder_parked = true;
        delayed_move_time = 0;
        break;
      case DXC_DUPLICATION_MODE:
        // If the new extruder is the left one, set it "parked"
        // This triggers the second extruder to move into the duplication position
        active_extruder_parked = (active_extruder == 0);
        current_position[X_AXIS] = active_extruder_parked ? inactive_extruder_x_pos : destination[X_AXIS] + duplicate_extruder_x_offset;
        inactive_extruder_x_pos = destination[X_AXIS];
        extruder_duplication_enabled = false;
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPAIR("Set inactive_extruder_x_pos=", inactive_extruder_x_pos);
            SERIAL_ECHOLNPGM("Clear extruder_duplication_enabled");
          }
        #endif
        break;
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPAIR("Active extruder parked: ", active_extruder_parked ? "yes" : "no");
        DEBUG_POS("New extruder (parked)", current_position);
      }
    #endif

    // No extra case for HAS_ABL in DUAL_X_CARRIAGE. Does that mean they don't work together?
  }

#endif // DUAL_X_CARRIAGE

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, const float fr_mm_s/*=0.0*/, bool no_move/*=false*/) {
  planner.synchronize();

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1

    mixing_tool_change(tmp_extruder);

  #else // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1

    if (tmp_extruder >= EXTRUDERS)
      return invalid_extruder_error(tmp_extruder);

    #if HOTENDS > 1

      const float old_feedrate_mm_s = fr_mm_s > 0.0 ? fr_mm_s : feedrate_mm_s;

      feedrate_mm_s = fr_mm_s > 0.0 ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;

      if (tmp_extruder != active_extruder) {
        if (!no_move && axis_unhomed_error()) {
          no_move = true;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("No move on toolchange");
          #endif
        }

        // Save current position to destination, for use later
        set_destination_from_current();

        #if HAS_LEVELING
          // Set current position to the physical position
          const bool leveling_was_active = planner.leveling_active;
          set_bed_leveling_enabled(false);
        #endif

        #if ENABLED(DUAL_X_CARRIAGE)

          dualx_tool_change(tmp_extruder, no_move); // Can modify no_move

        #else // !DUAL_X_CARRIAGE

          #if ENABLED(PARKING_EXTRUDER) // Dual Parking extruder
            parking_extruder_tool_change(tmp_extruder, no_move);
          #endif

          #if ENABLED(SWITCHING_NOZZLE)
            // Always raise by at least 1 to avoid workpiece
            const float zdiff = hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];
            current_position[Z_AXIS] += (zdiff > 0.0 ? zdiff : 0.0) + 1;
            planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
            move_nozzle_servo(tmp_extruder);
          #endif

          const float xdiff = hotend_offset[X_AXIS][tmp_extruder] - hotend_offset[X_AXIS][active_extruder],
                      ydiff = hotend_offset[Y_AXIS][tmp_extruder] - hotend_offset[Y_AXIS][active_extruder];

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPAIR("Offset Tool XY by { ", xdiff);
              SERIAL_ECHOPAIR(", ", ydiff);
              SERIAL_ECHOLNPGM(" }");
            }
          #endif

          // The newly-selected extruder XY is actually at...
          current_position[X_AXIS] += xdiff;
          current_position[Y_AXIS] += ydiff;

          // Set the new active extruder
          active_extruder = tmp_extruder;

        #endif // !DUAL_X_CARRIAGE

        #if HAS_LEVELING
          // Restore leveling to re-establish the logical position
          set_bed_leveling_enabled(leveling_was_active);
        #endif

        #if ENABLED(SWITCHING_NOZZLE)
          // The newly-selected extruder Z is actually at...
          current_position[Z_AXIS] -= zdiff;
        #endif

        // Tell the planner the new "current position"
        SYNC_PLAN_POSITION_KINEMATIC();

        #if ENABLED(DELTA)
          //LOOP_XYZ(i) update_software_endstops(i); // or modify the constrain function
          const bool safe_to_move = current_position[Z_AXIS] < delta_clip_start_height - 1;
        #else
          constexpr bool safe_to_move = true;
        #endif

        // Raise, move, and lower again
        if (safe_to_move && !no_move && IsRunning()) {
          #if DISABLED(SWITCHING_NOZZLE)
            // Do a small lift to avoid the workpiece in the move back (below)
            current_position[Z_AXIS] += 1.0;
            planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
          #endif
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("Move back", destination);
          #endif
          // Move back to the original (or tweaked) position
          do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS]);
          #if ENABLED(DUAL_X_CARRIAGE)
            active_extruder_parked = false;
          #endif
        }
        #if ENABLED(SWITCHING_NOZZLE)
          else {
            // Move back down. (Including when the new tool is higher.)
            do_blocking_move_to_z(destination[Z_AXIS], planner.max_feedrate_mm_s[Z_AXIS]);
          }
        #endif
      } // (tmp_extruder != active_extruder)

      planner.synchronize();

      #if ENABLED(EXT_SOLENOID) && !ENABLED(PARKING_EXTRUDER)
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif

      feedrate_mm_s = old_feedrate_mm_s;

    #else // HOTENDS <= 1

      UNUSED(fr_mm_s);
      UNUSED(no_move);

      #if ENABLED(MK2_MULTIPLEXER)
        if (tmp_extruder >= E_STEPPERS)
          return invalid_extruder_error(tmp_extruder);

        select_multiplexed_stepper(tmp_extruder);
      #endif

      // Set the new active extruder
      active_extruder = tmp_extruder;

    #endif // HOTENDS <= 1

    #if DO_SWITCH_EXTRUDER
      planner.synchronize();
      move_extruder_servo(active_extruder);
    #endif

    #if HAS_FANMUX
      fanmux_switch(active_extruder);
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ACTIVE_EXTRUDER, (int)active_extruder);

  #endif // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1
}
