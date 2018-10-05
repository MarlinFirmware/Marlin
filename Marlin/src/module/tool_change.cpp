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

#include "probe.h"
#include "motion.h"
#include "planner.h"

#include "../Marlin.h"

#if ENABLED(PARKING_EXTRUDER) && PARKING_EXTRUDER_SOLENOIDS_DELAY > 0
  #include "../gcode/gcode.h" // for dwell()
#endif

#if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE) || ENABLED(SWITCHING_TOOLHEAD)
  #include "../module/servo.h"
#endif

#if ENABLED(EXT_SOLENOID) && DISABLED(PARKING_EXTRUDER)
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

#if ENABLED(ULTIPANEL)
  #include "../lcd/ultralcd.h"
#endif

#if DO_SWITCH_EXTRUDER

  #if EXTRUDERS > 3
    #define _SERVO_NR(E) ((E) < 2 ? SWITCHING_EXTRUDER_SERVO_NR : SWITCHING_EXTRUDER_E23_SERVO_NR)
  #else
    #define _SERVO_NR(E) SWITCHING_EXTRUDER_SERVO_NR
  #endif

  void move_extruder_servo(const uint8_t e) {
    planner.synchronize();
    #if EXTRUDERS & 1
      if (e < EXTRUDERS - 1)
    #endif
    {
      MOVE_SERVO(_SERVO_NR(e), servo_angles[_SERVO_NR(e)][e]);
      safe_delay(500);
    }
  }

#endif // DO_SWITCH_EXTRUDER

#if ENABLED(SWITCHING_NOZZLE)

  void move_nozzle_servo(const uint8_t e) {
    planner.synchronize();
    MOVE_SERVO(SWITCHING_NOZZLE_SERVO_NR, servo_angles[SWITCHING_NOZZLE_SERVO_NR][e]);
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

  inline void parking_extruder_tool_change(const uint8_t tmp_extruder, bool no_move) {
    if (!no_move) {

      const float parkingposx[] = PARKING_EXTRUDER_PARKING_X,
                  midpos = (parkingposx[0] + parkingposx[1]) * 0.5 + hotend_offset[X_AXIS][active_extruder],
                  grabpos = parkingposx[tmp_extruder] + hotend_offset[X_AXIS][active_extruder]
                            + (tmp_extruder == 0 ? -(PARKING_EXTRUDER_GRAB_DISTANCE) : PARKING_EXTRUDER_GRAB_DISTANCE);
      /**
       * 1. Raise Z-Axis to give enough clearance
       * 2. Move to park position of old extruder
       * 3. Disengage magnetic field, wait for delay
       * 4. Move near new extruder
       * 5. Engage magnetic field for new extruder
       * 6. Move to parking incl. offset of new extruder
       * 7. Lower Z-Axis
       */

      // STEP 1
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("Start Autopark", current_position);
      #endif
      current_position[Z_AXIS] += PARKING_EXTRUDER_SECURITY_RAISE;
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("(1) Raise Z-Axis", current_position);
      #endif
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
      planner.synchronize();

      // STEP 2
      current_position[X_AXIS] = parkingposx[active_extruder] + hotend_offset[X_AXIS][active_extruder];
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOLNPAIR("(2) Park extruder ", int(active_extruder));
          DEBUG_POS("Moving ParkPos", current_position);
        }
      #endif
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();

      // STEP 3
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(3) Disengage magnet ");
      #endif
      pe_deactivate_magnet(active_extruder);

      // STEP 4
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(4) Move to position near new extruder");
      #endif
      current_position[X_AXIS] += active_extruder ? -10 : 10; // move 10mm away from parked extruder

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("Move away from parked extruder", current_position);
      #endif
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();

      // STEP 5
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(5) Engage magnetic field");
      #endif

      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_magnet(active_extruder); //just save power for inverted magnets
      #endif
      pe_activate_magnet(tmp_extruder);

      // STEP 6
      current_position[X_AXIS] = grabpos + (tmp_extruder ? -10 : 10);
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      current_position[X_AXIS] = grabpos;
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("(6) Unpark extruder", current_position);
      #endif
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS]/2, active_extruder);
      planner.synchronize();

      // Step 7
      current_position[X_AXIS] = midpos - hotend_offset[X_AXIS][tmp_extruder];
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("(7) Move midway between hotends", current_position);
      #endif
      planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
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

#if ENABLED(SWITCHING_TOOLHEAD)

  inline void switching_toolhead_tool_change(const uint8_t tmp_extruder, const float fr_mm_s/*=0.0*/, bool no_move/*=false*/) {
    if (no_move) return;

    constexpr uint16_t angles[2] = SWITCHING_TOOLHEAD_SERVO_ANGLES;

    const float toolheadposx[] = SWITCHING_TOOLHEAD_X_POS,
                placexpos = toolheadposx[active_extruder],
                grabxpos = toolheadposx[tmp_extruder];

    /**
     * 1. Raise Z to give enough clearance
     * 2. Move to switch position of current toolhead
     * 3. Unlock tool and drop it in the dock
     * 4. Move to the new toolhead
     * 5. Grab and lock the new toolhead
     * 6. Apply the z-offset of the new toolhead
     */

    // STEP 1
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Starting Toolhead change", current_position);
    #endif
    current_position[Z_AXIS] += SWITCHING_TOOLHEAD_SECURITY_RAISE;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("(1) Raise Z-Axis", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
    planner.synchronize();

    // STEP 2
    current_position[X_AXIS] = placexpos;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPAIR("(2) Place old tool ", int(active_extruder));
        DEBUG_POS("Move X SwitchPos", current_position);
      }
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
    planner.synchronize();

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS - SWITCHING_TOOLHEAD_Y_SECURITY;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Security", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Y_AXIS], active_extruder);
    planner.synchronize();

    // STEP 3
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(3) Unlock and Place Toolhead");
    #endif
    MOVE_SERVO(SWITCHING_TOOLHEAD_SERVO_NR, angles[1]);
    safe_delay(500);

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);
    #endif
    planner.buffer_line(current_position,(planner.max_feedrate_mm_s[Y_AXIS] * 0.5), active_extruder);
    planner.synchronize();
    safe_delay(200);
    current_position[Y_AXIS] -= SWITCHING_TOOLHEAD_Y_CLEAR;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move back Y clear", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Y_AXIS], active_extruder); // move away from docked toolhead
    planner.synchronize();

    // STEP 4
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(4) Move to new toolhead position");
    #endif
    current_position[X_AXIS] = grabxpos;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move to new toolhead X", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
    planner.synchronize();
    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS - SWITCHING_TOOLHEAD_Y_SECURITY;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Security", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Y_AXIS], active_extruder);
    planner.synchronize();

    // STEP 5
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(5) Grab and lock new toolhead ");
    #endif
    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Y_AXIS] * 0.5, active_extruder);
    planner.synchronize();

    safe_delay(200);
    MOVE_SERVO(SWITCHING_TOOLHEAD_SERVO_NR, angles[0]);
    safe_delay(500);

    current_position[Y_AXIS] -= SWITCHING_TOOLHEAD_Y_CLEAR;
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("Move back Y clear", current_position);
    #endif
    planner.buffer_line(current_position, planner.max_feedrate_mm_s[Y_AXIS], active_extruder); // move away from docked toolhead
    planner.synchronize();

    // STEP 6
    current_position[Z_AXIS] += hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("(6) Apply Z offset", current_position);
    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Toolhead change done.");
    #endif
  }

#endif // SWITCHING_TOOLHEAD

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
          case DXC_SCALED_DUPLICATION_MODE: SERIAL_ECHOLNPGM("DXC_SCALED_DUPLICATION_MODE"); break;
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

      #define CUR_X current_position[X_AXIS]
      #define CUR_Y current_position[Y_AXIS]
      #define CUR_Z current_position[Z_AXIS]
      #define CUR_E current_position[E_AXIS]

      planner.buffer_line(CUR_X, CUR_Y, raised_z, CUR_E, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
      planner.buffer_line(xhome, CUR_Y, raised_z, CUR_E, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.buffer_line(xhome, CUR_Y, CUR_Z,    CUR_E, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);

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

  #if ENABLED(DUAL_X_CARRIAGE)  // Only T0 allowed if the Printer is in DXC_DUPLICATION_MODE or DXC_SCALED_DUPLICATION_MODE
    if (tmp_extruder != 0 && dxc_is_duplicating())
       return invalid_extruder_error(tmp_extruder);
  #endif

  #if HAS_LEVELING
    // Set current position to the physical position
    const bool leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

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

        #if ENABLED(DUAL_X_CARRIAGE)

          #if HAS_SOFTWARE_ENDSTOPS
            // Update the X software endstops early
            active_extruder = tmp_extruder;
            update_software_endstops(X_AXIS);
            active_extruder = !tmp_extruder;

            // Don't move the new extruder out of bounds
            if (!WITHIN(current_position[X_AXIS], soft_endstop_min[X_AXIS], soft_endstop_max[X_AXIS]))
              no_move = true;

          #else
              // No software endstops? Use the configured limits
              if (active_extruder == 0) {
                if (!WITHIN(current_position[X_AXIS], X2_MIN_POS, X2_MAX_POS))
                  no_move = true;
              }
              else if (!WITHIN(current_position[X_AXIS], X1_MIN_POS, X1_MAX_POS))
                no_move = true;
          #endif

          #if ENABLED(ULTIPANEL)
            lcd_return_to_status();
          #endif

          if (!no_move) set_destination_from_current();
          dualx_tool_change(tmp_extruder, no_move); // Can modify no_move

        #else // !DUAL_X_CARRIAGE

          set_destination_from_current();

          const float xdiff = hotend_offset[X_AXIS][tmp_extruder] - hotend_offset[X_AXIS][active_extruder],
                      ydiff = hotend_offset[Y_AXIS][tmp_extruder] - hotend_offset[Y_AXIS][active_extruder];

          #if ENABLED(PARKING_EXTRUDER) // Dual Parking extruder
            constexpr float zdiff = 0;
            parking_extruder_tool_change(tmp_extruder, no_move);
          #elif ENABLED(SWITCHING_TOOLHEAD) // Switching Toolhead
            constexpr float zdiff = 0;
            switching_toolhead_tool_change(tmp_extruder, fr_mm_s, no_move);
          #else
            const float zdiff = hotend_offset[Z_AXIS][tmp_extruder] - hotend_offset[Z_AXIS][active_extruder];
            #if ENABLED(SWITCHING_NOZZLE)
              // Always raise by at least 1 to avoid workpiece
              current_position[Z_AXIS] += MAX(-zdiff, 0.0) + 1;
              planner.buffer_line(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
              move_nozzle_servo(tmp_extruder);
            #endif
          #endif

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPAIR("Offset Tool XY by { ", xdiff);
              SERIAL_ECHOPAIR(", ", ydiff);
              SERIAL_ECHOPAIR(", ", zdiff);
              SERIAL_ECHOLNPGM(" }");
            }
          #endif

          // The newly-selected extruder XY is actually at...
          current_position[X_AXIS] += xdiff;
          current_position[Y_AXIS] += ydiff;
          current_position[Z_AXIS] += zdiff;

          // Set the new active extruder
          active_extruder = tmp_extruder;

        #endif // !DUAL_X_CARRIAGE

        // Tell the planner the new "current position"
        sync_plan_position();

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
            planner.buffer_line(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
          #endif
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("Move back", destination);
          #endif
          #if ENABLED(DUAL_X_CARRIAGE)
            // Dual x carriage does not properly apply these to current position due to command ordering
            // So we apply the offsets for y and z to the destination here. X cannot have an offset in this mode
            // as it is utilized for X2 home position.
            destination[Y_AXIS] -= hotend_offset[Y_AXIS][active_extruder] - hotend_offset[Y_AXIS][tmp_extruder];
            destination[Z_AXIS] -= hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];
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

      #if ENABLED(EXT_SOLENOID) && DISABLED(PARKING_EXTRUDER)
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif

      feedrate_mm_s = old_feedrate_mm_s;

      #if HAS_SOFTWARE_ENDSTOPS && ENABLED(DUAL_X_CARRIAGE)
        update_software_endstops(X_AXIS);
      #endif

    #else // HOTENDS <= 1

      UNUSED(fr_mm_s);
      UNUSED(no_move);

      #if ENABLED(MK2_MULTIPLEXER)
        if (tmp_extruder >= E_STEPPERS)
          return invalid_extruder_error(tmp_extruder);

        select_multiplexed_stepper(tmp_extruder);
      #endif

      #if EXTRUDERS > 1
        // Set the new active extruder
        active_extruder = tmp_extruder;
      #endif

    #endif // HOTENDS <= 1

    #if DO_SWITCH_EXTRUDER
      planner.synchronize();
      move_extruder_servo(active_extruder);
    #endif

    #if HAS_FANMUX
      fanmux_switch(active_extruder);
    #endif

    #if HAS_LEVELING
      // Restore leveling to re-establish the logical position
      set_bed_leveling_enabled(leveling_was_active);
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ACTIVE_EXTRUDER, int(active_extruder));

  #endif // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1
}
