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
#include "stepper.h"

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

#if ENABLED(SWITCHING_EXTRUDER)

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
    stepper.synchronize();
    #if EXTRUDERS & 1
      if (e < EXTRUDERS - 1)
    #endif
    {
      MOVE_SERVO(_SERVO_NR, angles[e]);
      safe_delay(500);
    }
  }

#endif // SWITCHING_EXTRUDER

#if ENABLED(SWITCHING_NOZZLE)

  void move_nozzle_servo(const uint8_t e) {
    const int16_t angles[2] = SWITCHING_NOZZLE_SERVO_ANGLES;
    stepper.synchronize();
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

#endif // PARKING_EXTRUDER


inline void invalid_extruder_error(const uint8_t e) {
  SERIAL_ECHO_START();
  SERIAL_CHAR('T');
  SERIAL_ECHO_F(e, DEC);
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(MSG_INVALID_EXTRUDER);
}

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, const float fr_mm_s/*=0.0*/, bool no_move/*=false*/) {
  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1

    if (tmp_extruder >= MIXING_VIRTUAL_TOOLS)
      return invalid_extruder_error(tmp_extruder);

    // T0-Tnnn: Switch virtual tool by changing the mix
    for (uint8_t j = 0; j < MIXING_STEPPERS; j++)
      mixing_factor[j] = mixing_virtual_tool_mix[tmp_extruder][j];

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

        #if ENABLED(DUAL_X_CARRIAGE)

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
            stepper.synchronize();
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

              if (active_extruder_parked)
                current_position[X_AXIS] = inactive_extruder_x_pos;
              else
                current_position[X_AXIS] = destination[X_AXIS] + duplicate_extruder_x_offset;
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

        #else // !DUAL_X_CARRIAGE

          #if ENABLED(PARKING_EXTRUDER) // Dual Parking extruder
            float z_raise = PARKING_EXTRUDER_SECURITY_RAISE;
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
              current_position[Z_AXIS] += z_raise;
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(1) Raise Z-Axis ");
                if (DEBUGGING(LEVELING)) DEBUG_POS("Moving to Raised Z-Position", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
              stepper.synchronize();

              // STEP 2
              current_position[X_AXIS] = parkingposx[active_extruder] + hotend_offset[X_AXIS][active_extruder];
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPAIR("(2) Park extruder ", active_extruder);
                if (DEBUGGING(LEVELING)) DEBUG_POS("Moving ParkPos", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              stepper.synchronize();

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
              stepper.synchronize();

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
              stepper.synchronize();

              // Step 7
              current_position[X_AXIS] = midpos - hotend_offset[X_AXIS][tmp_extruder];
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(7) Move midway between hotends");
                if (DEBUGGING(LEVELING)) DEBUG_POS("Move midway to new extruder", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              stepper.synchronize();
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
            current_position[Z_AXIS] -= hotend_offset[Z_AXIS][tmp_extruder] - hotend_offset[Z_AXIS][active_extruder]; // Apply Zoffset

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) DEBUG_POS("Applying Z-offset", current_position);
            #endif

          #endif // dualParking extruder

          #if ENABLED(SWITCHING_NOZZLE)
            #define DONT_SWITCH (SWITCHING_EXTRUDER_SERVO_NR == SWITCHING_NOZZLE_SERVO_NR)
            // <0 if the new nozzle is higher, >0 if lower. A bigger raise when lower.
            const float z_diff = hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder],
                        z_raise = 0.3 + (z_diff > 0.0 ? z_diff : 0.0);

            // Always raise by some amount
            current_position[Z_AXIS] += z_raise;
            planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
            move_nozzle_servo(tmp_extruder);
          #endif

          /**
           * Set current_position to the position of the new nozzle.
           * Offsets are based on linear distance, so we need to get
           * the resulting position in coordinate space.
           *
           * - With grid or 3-point leveling, offset XYZ by a tilted vector
           * - With mesh leveling, update Z for the new position
           * - Otherwise, just use the raw linear distance
           *
           * Software endstops are altered here too. Consider a case where:
           *   E0 at X=0 ... E1 at X=10
           * When we switch to E1 now X=10, but E1 can't move left.
           * To express this we apply the change in XY to the software endstops.
           * E1 can move farther right than E0, so the right limit is extended.
           *
           * Note that we don't adjust the Z software endstops. Why not?
           * Consider a case where Z=0 (here) and switching to E1 makes Z=1
           * because the bed is 1mm lower at the new position. As long as
           * the first nozzle is out of the way, the carriage should be
           * allowed to move 1mm lower. This technically "breaks" the
           * Z software endstop. But this is technically correct (and
           * there is no viable alternative).
           */
          #if ABL_PLANAR
            // Offset extruder, make sure to apply the bed level rotation matrix
            vector_3 tmp_offset_vec = vector_3(hotend_offset[X_AXIS][tmp_extruder],
                                               hotend_offset[Y_AXIS][tmp_extruder],
                                               0),
                     act_offset_vec = vector_3(hotend_offset[X_AXIS][active_extruder],
                                               hotend_offset[Y_AXIS][active_extruder],
                                               0),
                     offset_vec = tmp_offset_vec - act_offset_vec;

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                tmp_offset_vec.debug(PSTR("tmp_offset_vec"));
                act_offset_vec.debug(PSTR("act_offset_vec"));
                offset_vec.debug(PSTR("offset_vec (BEFORE)"));
              }
            #endif

            offset_vec.apply_rotation(planner.bed_level_matrix.transpose(planner.bed_level_matrix));

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) offset_vec.debug(PSTR("offset_vec (AFTER)"));
            #endif

            // Adjustments to the current position
            const float xydiff[2] = { offset_vec.x, offset_vec.y };
            current_position[Z_AXIS] += offset_vec.z;

          #else // !ABL_PLANAR

            const float xydiff[2] = {
              hotend_offset[X_AXIS][tmp_extruder] - hotend_offset[X_AXIS][active_extruder],
              hotend_offset[Y_AXIS][tmp_extruder] - hotend_offset[Y_AXIS][active_extruder]
            };

            #if ENABLED(MESH_BED_LEVELING)

              if (planner.leveling_active) {
                #if ENABLED(DEBUG_LEVELING_FEATURE)
                  if (DEBUGGING(LEVELING)) SERIAL_ECHOPAIR("Z before MBL: ", current_position[Z_AXIS]);
                #endif
                float x2 = current_position[X_AXIS] + xydiff[X_AXIS],
                      y2 = current_position[Y_AXIS] + xydiff[Y_AXIS],
                      z1 = current_position[Z_AXIS], z2 = z1;
                planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], z1);
                planner.apply_leveling(x2, y2, z2);
                current_position[Z_AXIS] += z2 - z1;
                #if ENABLED(DEBUG_LEVELING_FEATURE)
                  if (DEBUGGING(LEVELING))
                    SERIAL_ECHOLNPAIR(" after: ", current_position[Z_AXIS]);
                #endif
              }

            #endif // MESH_BED_LEVELING

          #endif // !HAS_ABL

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPAIR("Offset Tool XY by { ", xydiff[X_AXIS]);
              SERIAL_ECHOPAIR(", ", xydiff[Y_AXIS]);
              SERIAL_ECHOLNPGM(" }");
            }
          #endif

          // The newly-selected extruder XY is actually at...
          current_position[X_AXIS] += xydiff[X_AXIS];
          current_position[Y_AXIS] += xydiff[Y_AXIS];

          // Set the new active extruder
          active_extruder = tmp_extruder;

        #endif // !DUAL_X_CARRIAGE

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("Sync After Toolchange", current_position);
        #endif

        // Tell the planner the new "current position"
        SYNC_PLAN_POSITION_KINEMATIC();

        #if ENABLED(DELTA)
          //LOOP_XYZ(i) update_software_endstops(i); // or modify the constrain function
          // Do a small lift to avoid the workpiece in the move back (below)
          const bool safe_to_move = current_position[Z_AXIS] < delta_clip_start_height - 1;
          if (safe_to_move && !no_move && IsRunning()) {
            ++current_position[Z_AXIS];
            planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
          }
        #else
          constexpr bool safe_to_move = true;
        #endif

        // Move to the "old position" (move the extruder into place)
        #if ENABLED(SWITCHING_NOZZLE)
          destination[Z_AXIS] += z_diff;  // Include the Z restore with the "move back"
        #endif

        if (safe_to_move && !no_move && IsRunning()) {
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("Move back", destination);
          #endif
          // Move back to the original (or tweaked) position
          do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS]);
        }
        #if ENABLED(SWITCHING_NOZZLE)
          else {
            // Move back down. (Including when the new tool is higher.)
            do_blocking_move_to_z(destination[Z_AXIS], planner.max_feedrate_mm_s[Z_AXIS]);
          }
        #endif
      } // (tmp_extruder != active_extruder)

      stepper.synchronize();

      #if ENABLED(EXT_SOLENOID) && !ENABLED(PARKING_EXTRUDER)
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif // EXT_SOLENOID

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

    #if ENABLED(SWITCHING_EXTRUDER) && !DONT_SWITCH
      stepper.synchronize();
      move_extruder_servo(active_extruder);
    #endif

    #if HAS_FANMUX
      fanmux_switch(active_extruder);
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ACTIVE_EXTRUDER, (int)active_extruder);

  #endif // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1
}
