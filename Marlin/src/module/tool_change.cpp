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

#include "../inc/MarlinConfigPre.h"

#include "tool_change.h"

#include "probe.h"
#include "motion.h"
#include "planner.h"
#include "temperature.h"

#include "../Marlin.h"

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

#if EXTRUDERS > 1
  toolchange_settings_t toolchange_settings;  // Initialized by settings.load()
#endif

#if ENABLED(SINGLENOZZLE)
  uint16_t singlenozzle_temp[EXTRUDERS];
  #if FAN_COUNT > 0
    uint8_t singlenozzle_fan_speed[EXTRUDERS];
  #endif
#endif

#if ENABLED(MAGNETIC_PARKING_EXTRUDER) || (ENABLED(PARKING_EXTRUDER) && PARKING_EXTRUDER_SOLENOIDS_DELAY > 0)
  #include "../gcode/gcode.h" // for dwell()
#endif

#if ANY(SWITCHING_EXTRUDER, SWITCHING_NOZZLE, SWITCHING_TOOLHEAD)
  #include "servo.h"
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

#if ENABLED(PRUSA_MMU2)
  #include "../feature/prusa_MMU2/mmu2.h"
#endif

#if HAS_LCD_MENU
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

  #if SWITCHING_NOZZLE_TWO_SERVOS

    inline void _move_nozzle_servo(const uint8_t e, const uint8_t angle_index) {
      constexpr int8_t  sns_index[2] = { SWITCHING_NOZZLE_SERVO_NR, SWITCHING_NOZZLE_E1_SERVO_NR };
      constexpr int16_t sns_angles[2] = SWITCHING_NOZZLE_SERVO_ANGLES;
      planner.synchronize();
      MOVE_SERVO(sns_index[e], sns_angles[angle_index]);
      safe_delay(500);
    }

    void lower_nozzle(const uint8_t e) { _move_nozzle_servo(e, 0); }
    void raise_nozzle(const uint8_t e) { _move_nozzle_servo(e, 1); }

  #else

    void move_nozzle_servo(const uint8_t angle_index) {
      planner.synchronize();
      MOVE_SERVO(SWITCHING_NOZZLE_SERVO_NR, servo_angles[SWITCHING_NOZZLE_SERVO_NR][angle_index]);
      safe_delay(500);
    }

  #endif

#endif // SWITCHING_NOZZLE

inline void fast_line_to_current(const AxisEnum fr_axis) {
  planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[fr_axis], active_extruder);
}

#if ENABLED(MAGNETIC_PARKING_EXTRUDER)

  float parkingposx[2] ,           // M951 R L
        parkinggrabdistance ,      // M951 I
        parkingslowspeed,          // M951 J
        parkinghighspeed ,         // M951 H
        parkingtraveldistance,     // M951 D
        compensationmultiplier;

  inline void magnetic_parking_extruder_tool_change(const uint8_t tmp_extruder) {

    const float oldx = current_position[X_AXIS],
                grabpos = mpe_settings.parking_xpos[tmp_extruder] + (tmp_extruder ? mpe_settings.grab_distance : -mpe_settings.grab_distance),
                offsetcompensation =
                  #if HAS_HOTEND_OFFSET
                    hotend_offset[X_AXIS][active_extruder] * mpe_settings.compensation_factor
                  #else
                    0
                  #endif
              ;

    if (axis_unhomed_error(true, false, false)) return;

    /**
     * Z Lift and Nozzle Offset shift ar defined in caller method to work equal with any Multi Hotend realization
     *
     * Steps:
     *   1. Move high speed to park position of new extruder
     *   2. Move to couple position of new extruder (this also discouple the old extruder)
     *   3. Move to park position of new extruder
     *   4. Move high speed to approach park position of old extruder
     *   5. Move to park position of old extruder
     *   6. Move to starting position
     */

    // STEP 1

    current_position[X_AXIS] = mpe_settings.parking_xpos[tmp_extruder] + offsetcompensation;

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(1) Move extruder ", int(tmp_extruder));
      DEBUG_POS(" to new extruder ParkPos", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.fast_feedrate, tmp_extruder);
    planner.synchronize();

    // STEP 2

    current_position[X_AXIS] = grabpos + offsetcompensation;

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(2) Couple extruder ", int(tmp_extruder));
      DEBUG_POS(" to new extruder GrabPos", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.slow_feedrate, tmp_extruder);
    planner.synchronize();

    // Delay before moving tool, to allow magnetic coupling
    gcode.dwell(150);

    // STEP 3

    current_position[X_AXIS] = mpe_settings.parking_xpos[tmp_extruder] + offsetcompensation;
    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(3) Move extruder ", int(tmp_extruder));
      DEBUG_POS(" back to new extruder ParkPos", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.slow_feedrate, tmp_extruder);
    planner.synchronize();

    // STEP 4

    current_position[X_AXIS] = mpe_settings.parking_xpos[active_extruder] + (active_extruder == 0 ? MPE_TRAVEL_DISTANCE : -MPE_TRAVEL_DISTANCE) + offsetcompensation;
    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(4) Move extruder ", int(tmp_extruder));
      DEBUG_POS(" close to old extruder ParkPos", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.fast_feedrate, tmp_extruder);
    planner.synchronize();

    // STEP 5

    current_position[X_AXIS] = mpe_settings.parking_xpos[active_extruder] + offsetcompensation;

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(5) Park extruder ", int(tmp_extruder));
      DEBUG_POS(" at old extruder ParkPos", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.slow_feedrate, tmp_extruder);
    planner.synchronize();

    // STEP 6

    current_position[X_AXIS] = oldx;

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("(6) Move extruder ", int(tmp_extruder));
      DEBUG_POS(" to starting position", current_position);
    }

    planner.buffer_line(current_position, mpe_settings.fast_feedrate, tmp_extruder);
    planner.synchronize();

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Autopark done.");
  }

#elif ENABLED(PARKING_EXTRUDER)

  void pe_solenoid_init() {
    for (uint8_t n = 0; n <= 1; ++n)
      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_solenoid(n);
      #else
        pe_deactivate_solenoid(n);
      #endif
  }

  void pe_set_solenoid(const uint8_t extruder_num, const uint8_t state) {
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

      constexpr float parkingposx[] = PARKING_EXTRUDER_PARKING_X;

      #if HAS_HOTEND_OFFSET
        const float x_offset = hotend_offset[X_AXIS][active_extruder];
      #else
        constexpr float x_offset = 0;
      #endif

      const float midpos = (parkingposx[0] + parkingposx[1]) * 0.5 + x_offset,
                  grabpos = parkingposx[tmp_extruder] + (tmp_extruder ? PARKING_EXTRUDER_GRAB_DISTANCE : -(PARKING_EXTRUDER_GRAB_DISTANCE)) + x_offset;

      /**
       * 1. Move to park position of old extruder
       * 2. Disengage magnetic field, wait for delay
       * 3. Move near new extruder
       * 4. Engage magnetic field for new extruder
       * 5. Move to parking incl. offset of new extruder
       * 6. Lower Z-Axis
       */

      // STEP 1

      if (DEBUGGING(LEVELING)) DEBUG_POS("Start Autopark", current_position);

      current_position[X_AXIS] = parkingposx[active_extruder] + x_offset;

      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLNPAIR("(1) Park extruder ", int(active_extruder));
        DEBUG_POS("Moving ParkPos", current_position);
      }

      fast_line_to_current(X_AXIS);
      planner.synchronize();

      // STEP 2

      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(2) Disengage magnet ");

      pe_deactivate_solenoid(active_extruder);

      // STEP 3

      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(3) Move to position near new extruder");

      current_position[X_AXIS] += active_extruder ? -10 : 10; // move 10mm away from parked extruder

      if (DEBUGGING(LEVELING)) DEBUG_POS("Move away from parked extruder", current_position);

      fast_line_to_current(X_AXIS);
      planner.synchronize();

      // STEP 4
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(4) Engage magnetic field");

      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_solenoid(active_extruder); //just save power for inverted magnets
      #endif

      pe_activate_solenoid(tmp_extruder);

      // STEP 5

      current_position[X_AXIS] = grabpos + (tmp_extruder ? -10 : 10);
      fast_line_to_current(X_AXIS);
      current_position[X_AXIS] = grabpos;
      if (DEBUGGING(LEVELING)) DEBUG_POS("(5) Unpark extruder", current_position);
      planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[X_AXIS] * 0.5, active_extruder);
      planner.synchronize();

      // STEP 6

      current_position[X_AXIS] = midpos
        #if HAS_HOTEND_OFFSET
          - hotend_offset[X_AXIS][tmp_extruder]
        #endif
      ;

      if (DEBUGGING(LEVELING)) DEBUG_POS("(6) Move midway between hotends", current_position);

      fast_line_to_current(X_AXIS);
      planner.synchronize();

      DEBUG_ECHOLNPGM("Autopark done.");
    }
    else { // nomove == true
      // Only engage magnetic field for new extruder
      pe_activate_solenoid(tmp_extruder);
      #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
        pe_activate_solenoid(active_extruder); // Just save power for inverted magnets
      #endif
    }
  }

#endif // PARKING_EXTRUDER

#if ENABLED(SWITCHING_TOOLHEAD)

  inline void switching_toolhead_tool_change(const uint8_t tmp_extruder, bool no_move/*=false*/) {
    if (no_move) return;

    constexpr uint16_t angles[2] = SWITCHING_TOOLHEAD_SERVO_ANGLES;

    const float toolheadposx[] = SWITCHING_TOOLHEAD_X_POS,
                placexpos = toolheadposx[active_extruder],
                grabxpos = toolheadposx[tmp_extruder];

    /**
     * 1. Move to switch position of current toolhead
     * 2. Unlock tool and drop it in the dock
     * 3. Move to the new toolhead
     * 4. Grab and lock the new toolhead
     */

    // 1. Move to switch position of current toolhead

    if (DEBUGGING(LEVELING)) DEBUG_POS("Starting Toolhead change", current_position);

    current_position[X_AXIS] = placexpos;

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOLNPAIR("(1) Place old tool ", int(active_extruder));
      DEBUG_POS("Move X SwitchPos", current_position);
    }

    fast_line_to_current(X_AXIS);
    planner.synchronize();

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS - SWITCHING_TOOLHEAD_Y_SECURITY;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Security", current_position);

    fast_line_to_current(Y_AXIS);
    planner.synchronize();

    // 2. Unlock tool and drop it in the dock

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(2) Unlock and Place Toolhead");

    MOVE_SERVO(SWITCHING_TOOLHEAD_SERVO_NR, angles[1]);
    safe_delay(500);

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);

    planner.buffer_line(current_position,(planner.settings.max_feedrate_mm_s[Y_AXIS] * 0.5), active_extruder);
    planner.synchronize();
    safe_delay(200);
    current_position[Y_AXIS] -= SWITCHING_TOOLHEAD_Y_CLEAR;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move back Y clear", current_position);

    fast_line_to_current(Y_AXIS); // move away from docked toolhead
    planner.synchronize();

    // 3. Move to the new toolhead

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(3) Move to new toolhead position");

    current_position[X_AXIS] = grabxpos;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move to new toolhead X", current_position);

    fast_line_to_current(X_AXIS);
    planner.synchronize();
    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS - SWITCHING_TOOLHEAD_Y_SECURITY;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Security", current_position);

    fast_line_to_current(Y_AXIS);
    planner.synchronize();

    // 4. Grab and lock the new toolhead

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("(4) Grab and lock new toolhead ");

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Y_AXIS] * 0.5, active_extruder);
    planner.synchronize();

    safe_delay(200);
    MOVE_SERVO(SWITCHING_TOOLHEAD_SERVO_NR, angles[0]);
    safe_delay(500);

    current_position[Y_AXIS] -= SWITCHING_TOOLHEAD_Y_CLEAR;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move back Y clear", current_position);

    fast_line_to_current(Y_AXIS); // move away from docked toolhead
    planner.synchronize();

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Toolhead change done.");
  }

#endif // SWITCHING_TOOLHEAD

#if ENABLED(MAGNETIC_SWITCHING_TOOLHEAD)

  inline void magnetic_switching_toolhead_tool_change(const uint8_t tmp_extruder, bool no_move/*=false*/) {
    if (no_move) return;

    const float toolheadposx[] = SWITCHING_TOOLHEAD_X_POS,
                placexpos = toolheadposx[active_extruder],
                grabxpos = toolheadposx[tmp_extruder];

    /**
     * 1. Move to switch position of current toolhead
     * 2. Release and place toolhead in the dock
     * 3. Move to the new toolhead
     * 4. Grab the new toolhead and move to security position
     */

    if (DEBUGGING(LEVELING)) DEBUG_POS("Starting Toolhead change", current_position);

    // 1. Move to switch position current toolhead

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS + SWITCHING_TOOLHEAD_Y_CLEAR;

    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOLNPAIR("(1) Place old tool ", int(active_extruder));
      DEBUG_POS("Move Y SwitchPos + Security", current_position);
    }

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Y_AXIS], active_extruder);
    planner.synchronize();

    current_position[X_AXIS] = placexpos + SWITCHING_TOOLHEAD_X_SECURITY;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move X SwitchPos + Security", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[X_AXIS], active_extruder);
    planner.synchronize();

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Y_AXIS], active_extruder);
    planner.synchronize();

    current_position[X_AXIS] = placexpos;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move X SwitchPos", current_position);

    planner.buffer_line(current_position, (planner.settings.max_feedrate_mm_s[X_AXIS] * 0.25), active_extruder);
    planner.synchronize();

    // 2. Release and place toolhead in the dock

    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(2) Release and Place Toolhead");

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS + SWITCHING_TOOLHEAD_Y_RELEASE;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Release", current_position);

    planner.buffer_line(current_position, (planner.settings.max_feedrate_mm_s[Y_AXIS] * 0.1), active_extruder);
    planner.synchronize();

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS + SWITCHING_TOOLHEAD_Y_SECURITY;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Security", current_position);

    planner.buffer_line(current_position, (planner.settings.max_feedrate_mm_s[Y_AXIS]), active_extruder);
    planner.synchronize();

    // 3. Move to new toolhead position

    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(3) Move to new toolhead position");

    current_position[X_AXIS] = grabxpos;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move to new toolhead X", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[X_AXIS], active_extruder);
    planner.synchronize();

    // 4. Grab the new toolhead and move to security position

    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("(4) Grab new toolhead and move to security position");

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS + SWITCHING_TOOLHEAD_Y_RELEASE;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos + Release", current_position);

    planner.buffer_line(current_position, (planner.settings.max_feedrate_mm_s[Y_AXIS]), active_extruder);
    planner.synchronize();

    current_position[Y_AXIS] = SWITCHING_TOOLHEAD_Y_POS;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move Y SwitchPos", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Y_AXIS] * 0.2, active_extruder);
    planner.synchronize();
    safe_delay(100);

    current_position[X_AXIS] = grabxpos + SWITCHING_TOOLHEAD_X_SECURITY;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move to new toolhead X + Security", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[X_AXIS] * 0.1, active_extruder);
    planner.synchronize();
    safe_delay(100);

    current_position[Y_AXIS] += SWITCHING_TOOLHEAD_Y_CLEAR;

    if (DEBUGGING(LEVELING)) DEBUG_POS("Move back Y clear", current_position);

    planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Y_AXIS], active_extruder); // move away from docked toolhead
    planner.synchronize();
  }

#endif // MAGNETIC_SWITCHING_TOOLHEAD

inline void invalid_extruder_error(const uint8_t e) {
  SERIAL_ECHO_START();
  SERIAL_CHAR('T'); SERIAL_ECHO(int(e));
  SERIAL_CHAR(' '); SERIAL_ECHOLNPGM(MSG_INVALID_EXTRUDER);
}

#if ENABLED(DUAL_X_CARRIAGE)

  inline void dualx_tool_change(const uint8_t tmp_extruder, bool &no_move) {
    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPGM("Dual X Carriage Mode ");
      switch (dual_x_carriage_mode) {
        case DXC_FULL_CONTROL_MODE: DEBUG_ECHOLNPGM("FULL_CONTROL"); break;
        case DXC_AUTO_PARK_MODE:    DEBUG_ECHOLNPGM("AUTO_PARK");    break;
        case DXC_DUPLICATION_MODE:  DEBUG_ECHOLNPGM("DUPLICATION");  break;
        case DXC_MIRRORED_MODE:     DEBUG_ECHOLNPGM("MIRRORED");     break;
      }
    }

    const float xhome = x_home_pos(active_extruder);
    if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE
        && IsRunning()
        && (delayed_move_time || current_position[X_AXIS] != xhome) && ! no_move
    ) {

      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("MoveX to ", xhome);

      // Park old head
      planner.buffer_line(xhome, current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], planner.settings.max_feedrate_mm_s[X_AXIS], active_extruder);
      planner.synchronize();
    }

    // Activate the new extruder ahead of calling set_axis_is_at_home!
    active_extruder = tmp_extruder;

    // This function resets the max/min values - the current position may be overwritten below.
    set_axis_is_at_home(X_AXIS);

    if (DEBUGGING(LEVELING)) DEBUG_POS("New Extruder", current_position);

    switch (dual_x_carriage_mode) {
      case DXC_FULL_CONTROL_MODE:
        // New current position is the position of the activated extruder
        current_position[X_AXIS] = inactive_extruder_x_pos;
        // Save the inactive extruder's position (from the old current_position)
        inactive_extruder_x_pos = destination[X_AXIS];
        break;
      case DXC_AUTO_PARK_MODE:
        // record current raised toolhead position for use by unpark
        COPY(raised_parked_position, current_position);
        active_extruder_parked = true;
        delayed_move_time = 0;
        break;
      default:
        break;
    }

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOLNPAIR("Active extruder parked: ", active_extruder_parked ? "yes" : "no");
      DEBUG_POS("New extruder (parked)", current_position);
    }
  }

#endif // DUAL_X_CARRIAGE

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, bool no_move/*=false*/) {

  #if ENABLED(MAGNETIC_SWITCHING_TOOLHEAD)
    if (tmp_extruder == active_extruder) return;
  #endif

  #if ENABLED(MIXING_EXTRUDER)

    UNUSED(no_move);

    if (tmp_extruder >= MIXING_VIRTUAL_TOOLS)
      return invalid_extruder_error(tmp_extruder);

    #if MIXING_VIRTUAL_TOOLS > 1
      // T0-Tnnn: Switch virtual tool by changing the index to the mix
      mixer.T(tmp_extruder);
    #endif

  #elif ENABLED(PRUSA_MMU2)

    UNUSED(no_move);

    mmu2.tool_change(tmp_extruder);

  #elif EXTRUDERS < 2

    UNUSED(no_move);

    if (tmp_extruder) invalid_extruder_error(tmp_extruder);
    return;

  #else // EXTRUDERS > 1

    planner.synchronize();

    #if ENABLED(DUAL_X_CARRIAGE)  // Only T0 allowed if the Printer is in DXC_DUPLICATION_MODE or DXC_MIRRORED_MODE
      if (tmp_extruder != 0 && dxc_is_duplicating())
         return invalid_extruder_error(tmp_extruder);
    #endif

    #if HAS_LEVELING
      // Set current position to the physical position
      const bool leveling_was_active = planner.leveling_active;
      set_bed_leveling_enabled(false);
    #endif

    if (tmp_extruder >= EXTRUDERS)
      return invalid_extruder_error(tmp_extruder);

    if (!no_move && !all_axes_homed()) {
      no_move = true;
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("No move (not homed)");
    }

    #if HAS_LCD_MENU
      ui.return_to_status();
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      const bool idex_full_control = dual_x_carriage_mode == DXC_FULL_CONTROL_MODE;
    #else
      constexpr bool idex_full_control = false;
    #endif

    const bool can_move_away = !no_move && !idex_full_control;

    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      const bool should_swap = can_move_away && toolchange_settings.swap_length;
      #if ENABLED(PREVENT_COLD_EXTRUSION)
        const bool too_cold = !DEBUGGING(DRYRUN) && (thermalManager.targetTooColdToExtrude(active_extruder) || thermalManager.targetTooColdToExtrude(tmp_extruder));
      #else
        constexpr bool too_cold = false;
      #endif
      if (should_swap) {
        if (too_cold) {
          SERIAL_ECHO_MSG(MSG_ERR_HOTEND_TOO_COLD);
          #if ENABLED(SINGLENOZZLE)
            active_extruder = tmp_extruder;
            return;
          #endif
        }
        else {
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            do_pause_e_move(-toolchange_settings.swap_length, MMM_TO_MMS(toolchange_settings.retract_speed));
          #else
            current_position[E_AXIS] -= toolchange_settings.swap_length / planner.e_factor[active_extruder];
            planner.buffer_line(current_position, MMM_TO_MMS(toolchange_settings.retract_speed), active_extruder);
            planner.synchronize();
          #endif
        }
      }
    #endif // TOOLCHANGE_FILAMENT_SWAP

    if (tmp_extruder != active_extruder) {

      #if SWITCHING_NOZZLE_TWO_SERVOS
        raise_nozzle(active_extruder);
      #endif

      REMEMBER(fr, feedrate_mm_s, XY_PROBE_FEEDRATE_MM_S);

      #if HAS_SOFTWARE_ENDSTOPS
        #if HAS_HOTEND_OFFSET
          #define _EXT_ARGS , active_extruder, tmp_extruder
        #else
          #define _EXT_ARGS
        #endif
        update_software_endstops(X_AXIS _EXT_ARGS);
        #if DISABLED(DUAL_X_CARRIAGE)
          update_software_endstops(Y_AXIS _EXT_ARGS);
          update_software_endstops(Z_AXIS _EXT_ARGS);
        #endif
      #endif

      set_destination_from_current();

      #if DISABLED(SWITCHING_NOZZLE)
        if (can_move_away) {
          // Do a small lift to avoid the workpiece in the move back (below)
          current_position[Z_AXIS] += toolchange_settings.z_raise;
          #if HAS_SOFTWARE_ENDSTOPS
            NOMORE(current_position[Z_AXIS], soft_endstop[Z_AXIS].max);
          #endif
          fast_line_to_current(Z_AXIS);
          #if ENABLED(TOOLCHANGE_PARK)
            current_position[X_AXIS] = toolchange_settings.change_point.x;
            current_position[Y_AXIS] = toolchange_settings.change_point.y;
          #endif
          planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
          planner.synchronize();
        }
      #endif

      #if HAS_HOTEND_OFFSET
        #if ENABLED(DUAL_X_CARRIAGE)
          constexpr float xdiff = 0;
        #else
          const float xdiff = hotend_offset[X_AXIS][tmp_extruder] - hotend_offset[X_AXIS][active_extruder];
        #endif
        const float ydiff = hotend_offset[Y_AXIS][tmp_extruder] - hotend_offset[Y_AXIS][active_extruder],
                    zdiff = hotend_offset[Z_AXIS][tmp_extruder] - hotend_offset[Z_AXIS][active_extruder];
      #else
        constexpr float xdiff = 0, ydiff = 0, zdiff = 0;
      #endif

      #if ENABLED(DUAL_X_CARRIAGE)
        dualx_tool_change(tmp_extruder, no_move);
      #elif ENABLED(PARKING_EXTRUDER) // Dual Parking extruder
        parking_extruder_tool_change(tmp_extruder, no_move);
      #elif ENABLED(MAGNETIC_PARKING_EXTRUDER) // Magnetic Parking extruder
        magnetic_parking_extruder_tool_change(tmp_extruder);
      #elif ENABLED(SWITCHING_TOOLHEAD) // Switching Toolhead
        switching_toolhead_tool_change(tmp_extruder, no_move);
      #elif ENABLED(MAGNETIC_SWITCHING_TOOLHEAD) // Magnetic Switching Toolhead
        magnetic_switching_toolhead_tool_change(tmp_extruder, no_move);
      #elif ENABLED(SWITCHING_NOZZLE) && !SWITCHING_NOZZLE_TWO_SERVOS
        // Raise by a configured distance to avoid workpiece, except with
        // SWITCHING_NOZZLE_TWO_SERVOS, as both nozzles will lift instead.
        current_position[Z_AXIS] += MAX(-zdiff, 0.0) + toolchange_settings.z_raise;
        #if HAS_SOFTWARE_ENDSTOPS
          NOMORE(current_position[Z_AXIS], soft_endstop[Z_AXIS].max);
        #endif
        if (!no_move) fast_line_to_current(Z_AXIS);
        move_nozzle_servo(tmp_extruder);
      #endif

      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("Offset Tool XY by { ", xdiff, ", ", ydiff, ", ", zdiff, " }");

      // The newly-selected extruder XY is actually at...
      current_position[X_AXIS] += xdiff;
      current_position[Y_AXIS] += ydiff;
      current_position[Z_AXIS] += zdiff;

      // Set the new active extruder if not already done in tool specific function above
      active_extruder = tmp_extruder;

      // Tell the planner the new "current position"
      sync_plan_position();

      #if ENABLED(DELTA)
        //LOOP_XYZ(i) update_software_endstops(i); // or modify the constrain function
        const bool safe_to_move = current_position[Z_AXIS] < delta_clip_start_height - 1;
      #else
        constexpr bool safe_to_move = true;
      #endif

      // Return to position and lower again
      if (safe_to_move && !no_move && IsRunning()) {

        #if ENABLED(SINGLENOZZLE)
          #if FAN_COUNT > 0
            singlenozzle_fan_speed[active_extruder] = thermalManager.fan_speed[0];
            thermalManager.fan_speed[0] = singlenozzle_fan_speed[tmp_extruder];
          #endif

          singlenozzle_temp[active_extruder] = thermalManager.temp_hotend[0].target;
          if (singlenozzle_temp[tmp_extruder] && singlenozzle_temp[tmp_extruder] != singlenozzle_temp[active_extruder]) {
            thermalManager.setTargetHotend(singlenozzle_temp[tmp_extruder], 0);
            #if EITHER(ULTRA_LCD, EXTENSIBLE_UI)
              thermalManager.set_heating_message(0);
            #endif
            (void)thermalManager.wait_for_hotend(0, false);  // Wait for heating or cooling
          }
          active_extruder = tmp_extruder;
        #endif

        #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
          if (should_swap && !too_cold) {
            #if ENABLED(ADVANCED_PAUSE_FEATURE)
              do_pause_e_move(toolchange_settings.swap_length + TOOLCHANGE_FIL_EXTRA_PRIME, MMM_TO_MMS(toolchange_settings.prime_speed));
            #else
              current_position[E_AXIS] += (toolchange_settings.swap_length + TOOLCHANGE_FIL_EXTRA_PRIME) / planner.e_factor[tmp_extruder];
              planner.buffer_line(current_position, MMM_TO_MMS(toolchange_settings.prime_speed), tmp_extruder);
            #endif
            planner.synchronize();

            #if TOOLCHANGE_FIL_EXTRA_PRIME
              planner.set_e_position_mm((destination[E_AXIS] = current_position[E_AXIS] = current_position[E_AXIS] - (TOOLCHANGE_FIL_EXTRA_PRIME)));
            #endif
          }
        #endif

        // Prevent a move outside physical bounds
        #if ENABLED(MAGNETIC_SWITCHING_TOOLHEAD)
          // If the original position is within tool store area, go to X origin at once
          if (destination[Y_AXIS] < SWITCHING_TOOLHEAD_Y_POS + SWITCHING_TOOLHEAD_Y_CLEAR) {
            current_position[X_AXIS] = 0;
            planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[X_AXIS], active_extruder);
            planner.synchronize();
          }
        #else
          apply_motion_limits(destination);
        #endif

        // Should the nozzle move back to the old position?
        if (can_move_away) {
          #if ENABLED(TOOLCHANGE_NO_RETURN)
            // Just move back down
            if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Move back Z only");
            do_blocking_move_to_z(destination[Z_AXIS], planner.settings.max_feedrate_mm_s[Z_AXIS]);
          #else
            // Move back to the original (or adjusted) position
            if (DEBUGGING(LEVELING)) DEBUG_POS("Move back", destination);
            do_blocking_move_to(destination);
          #endif
        }
        else if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Move back skipped");

        #if ENABLED(DUAL_X_CARRIAGE)
          active_extruder_parked = false;
        #endif
      }
      #if ENABLED(SWITCHING_NOZZLE)
        else {
          // Move back down. (Including when the new tool is higher.)
          do_blocking_move_to_z(destination[Z_AXIS], planner.settings.max_feedrate_mm_s[Z_AXIS]);
        }
      #endif

      #if ENABLED(PRUSA_MMU2)
        mmu2.tool_change(tmp_extruder);
      #endif

      #if SWITCHING_NOZZLE_TWO_SERVOS
        lower_nozzle(active_extruder);
      #endif

    } // (tmp_extruder != active_extruder)

    planner.synchronize();

    #if ENABLED(EXT_SOLENOID) && DISABLED(PARKING_EXTRUDER)
      disable_all_solenoids();
      enable_solenoid_on_active_extruder();
    #endif

    #if ENABLED(MK2_MULTIPLEXER)
      if (tmp_extruder >= E_STEPPERS) return invalid_extruder_error(tmp_extruder);
      select_multiplexed_stepper(tmp_extruder);
    #endif

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

  #endif // EXTRUDERS > 1
}
