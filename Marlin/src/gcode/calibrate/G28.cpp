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

#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/stepper.h"
#include "../../module/endstops.h"

#if HOTENDS > 1
  #include "../../module/tool_change.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(SENSORLESS_HOMING)
  #include "../../feature/tmc_util.h"
#endif

#include "../../module/probe.h"

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#include "../../lcd/ultralcd.h"

#if HAS_L64XX                         // set L6470 absolute position registers to counts
  #include "../../libs/L64XX/L64XX_Marlin.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

#if ENABLED(QUICK_HOME)

  static void quick_home_xy() {

    // Pretend the current position is 0,0
    current_position.set(0.0, 0.0);
    sync_plan_position();

    const int x_axis_home_dir =
      #if ENABLED(DUAL_X_CARRIAGE)
        x_home_dir(active_extruder)
      #else
        home_dir(X_AXIS)
      #endif
    ;

    const float mlx = max_length(X_AXIS),
                mly = max_length(Y_AXIS),
                mlratio = mlx > mly ? mly / mlx : mlx / mly,
                fr_mm_s = _MIN(homing_feedrate(X_AXIS), homing_feedrate(Y_AXIS)) * SQRT(sq(mlratio) + 1.0);

    #if ENABLED(SENSORLESS_HOMING)
      sensorless_t stealth_states {
          tmc_enable_stallguard(stepperX)
        , tmc_enable_stallguard(stepperY)
        , false
        , false
          #if AXIS_HAS_STALLGUARD(X2)
            || tmc_enable_stallguard(stepperX2)
          #endif
        , false
          #if AXIS_HAS_STALLGUARD(Y2)
            || tmc_enable_stallguard(stepperY2)
          #endif
      };
    #endif

    do_blocking_move_to_xy(1.5 * mlx * x_axis_home_dir, 1.5 * mly * home_dir(Y_AXIS), fr_mm_s);

    endstops.validate_homing_move();

    current_position.set(0.0, 0.0);

    #if ENABLED(SENSORLESS_HOMING)
      tmc_disable_stallguard(stepperX, stealth_states.x);
      tmc_disable_stallguard(stepperY, stealth_states.y);
      #if AXIS_HAS_STALLGUARD(X2)
        tmc_disable_stallguard(stepperX2, stealth_states.x2);
      #endif
      #if AXIS_HAS_STALLGUARD(Y2)
        tmc_disable_stallguard(stepperY2, stealth_states.y2);
      #endif
    #endif
  }

#endif // QUICK_HOME

#if ENABLED(Z_SAFE_HOMING)

  inline void home_z_safely() {

    // Disallow Z homing if X or Y are unknown
    if (!TEST(axis_known_position, X_AXIS) || !TEST(axis_known_position, Y_AXIS)) {
      LCD_MESSAGEPGM(MSG_ERR_Z_HOMING);
      SERIAL_ECHO_MSG(MSG_ERR_Z_HOMING_SER);
      return;
    }

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("home_z_safely >>>");

    sync_plan_position();

    /**
     * Move the Z probe (or just the nozzle) to the safe homing point
     * (Z is already at the right height)
     */
    destination.set(safe_homing_xy, current_position.z);

    #if HOMING_Z_WITH_PROBE
      destination -= probe.offset_xy;
    #endif

    if (position_is_reachable(destination)) {

      if (DEBUGGING(LEVELING)) DEBUG_POS("home_z_safely", destination);

      // This causes the carriage on Dual X to unpark
      #if ENABLED(DUAL_X_CARRIAGE)
        active_extruder_parked = false;
      #endif

      #if ENABLED(SENSORLESS_HOMING)
        safe_delay(500); // Short delay needed to settle
      #endif

      do_blocking_move_to_xy(destination);
      homeaxis(Z_AXIS);
    }
    else {
      LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
      SERIAL_ECHO_MSG(MSG_ZPROBE_OUT_SER);
    }

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< home_z_safely");
  }

#endif // Z_SAFE_HOMING

#if ENABLED(IMPROVE_HOMING_RELIABILITY)

  slow_homing_t begin_slow_homing() {
    slow_homing_t slow_homing{0};
    slow_homing.acceleration.set(planner.settings.max_acceleration_mm_per_s2[X_AXIS],
                                 planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    planner.settings.max_acceleration_mm_per_s2[X_AXIS] = 100;
    planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = 100;
    #if HAS_CLASSIC_JERK
      slow_homing.jerk_xy = planner.max_jerk;
      planner.max_jerk.set(0, 0);
    #endif
    planner.reset_acceleration_rates();
    return slow_homing;
  }

  void end_slow_homing(const slow_homing_t &slow_homing) {
    planner.settings.max_acceleration_mm_per_s2[X_AXIS] = slow_homing.acceleration.x;
    planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = slow_homing.acceleration.y;
    #if HAS_CLASSIC_JERK
      planner.max_jerk = slow_homing.jerk_xy;
    #endif
    planner.reset_acceleration_rates();
  }

#endif // IMPROVE_HOMING_RELIABILITY

/**
 * G28: Home all axes according to settings
 *
 * Parameters
 *
 *  None  Home to all axes with no parameters.
 *        With QUICK_HOME enabled XY will home together, then Z.
 *
 *  O   Home only if position is unknown
 *
 *  Rn  Raise by n mm/inches before homing
 *
 * Cartesian/SCARA parameters
 *
 *  X   Home to the X endstop
 *  Y   Home to the Y endstop
 *  Z   Home to the Z endstop
 *
 */
void GcodeSuite::G28(const bool always_home_all) {
  if (DEBUGGING(LEVELING)) {
    DEBUG_ECHOLNPGM(">>> G28");
    log_machine_info();
  }

  #if ENABLED(DUAL_X_CARRIAGE)
    bool IDEX_saved_duplication_state = extruder_duplication_enabled;
    DualXMode IDEX_saved_mode = dual_x_carriage_mode;
  #endif

  #if ENABLED(MARLIN_DEV_MODE)
    if (parser.seen('S')) {
      LOOP_XYZ(a) set_axis_is_at_home((AxisEnum)a);
      sync_plan_position();
      SERIAL_ECHOLNPGM("Simulated Homing");
      report_current_position();
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G28");
      return;
    }
  #endif

  // Home (O)nly if position is unknown
  if (!homing_needed() && parser.boolval('O')) {
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> homing not needed, skip\n<<< G28");
    return;
  }

  // Wait for planner moves to finish!
  planner.synchronize();

  // Disable the leveling matrix before homing
  #if HAS_LEVELING

    // Cancel the active G29 session
    #if ENABLED(PROBE_MANUALLY)
      g29_in_progress = false;
    #endif

    #if ENABLED(RESTORE_LEVELING_AFTER_G28)
      const bool leveling_was_active = planner.leveling_active;
    #endif
    set_bed_leveling_enabled(false);
  #endif

  #if ENABLED(CNC_WORKSPACE_PLANES)
    workspace_plane = PLANE_XY;
  #endif

  #define HAS_CURRENT_HOME(N) (defined(N##_CURRENT_HOME) && N##_CURRENT_HOME != N##_CURRENT)
  #define HAS_HOMING_CURRENT (HAS_CURRENT_HOME(X) || HAS_CURRENT_HOME(X2) || HAS_CURRENT_HOME(Y) || HAS_CURRENT_HOME(Y2))

  #if HAS_HOMING_CURRENT
    auto debug_current = [](const char * const s, const int16_t a, const int16_t b){
      DEBUG_ECHO(s); DEBUG_ECHOLNPAIR(" current: ", a, " -> ", b);
    };
    #if HAS_CURRENT_HOME(X)
      const int16_t tmc_save_current_X = stepperX.getMilliamps();
      stepperX.rms_current(X_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current("X", tmc_save_current_X, X_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(X2)
      const int16_t tmc_save_current_X2 = stepperX2.getMilliamps();
      stepperX2.rms_current(X2_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current("X2", tmc_save_current_X2, X2_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(Y)
      const int16_t tmc_save_current_Y = stepperY.getMilliamps();
      stepperY.rms_current(Y_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current("Y", tmc_save_current_Y, Y_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(Y2)
      const int16_t tmc_save_current_Y2 = stepperY2.getMilliamps();
      stepperY2.rms_current(Y2_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current("Y2", tmc_save_current_Y2, Y2_CURRENT_HOME);
    #endif
  #endif

  #if ENABLED(IMPROVE_HOMING_RELIABILITY)
    slow_homing_t slow_homing = begin_slow_homing();
  #endif

  // Always home with tool 0 active
  #if HOTENDS > 1
    #if DISABLED(DELTA) || ENABLED(DELTA_HOME_TO_SAFE_ZONE)
      const uint8_t old_tool_index = active_extruder;
    #endif
    tool_change(0, true);
  #endif

  #if HAS_DUPLICATION_MODE
    extruder_duplication_enabled = false;
  #endif

  remember_feedrate_scaling_off();

  endstops.enable(true); // Enable endstops for next homing move

  #if ENABLED(DELTA)

    home_delta();
    UNUSED(always_home_all);

    #if ENABLED(IMPROVE_HOMING_RELIABILITY)
      end_slow_homing(slow_homing);
    #endif

  #else // NOT DELTA

    const bool homeX = parser.seen('X'), homeY = parser.seen('Y'), homeZ = parser.seen('Z'),
               home_all = always_home_all || (homeX == homeY && homeX == homeZ),
               doX = home_all || homeX, doY = home_all || homeY, doZ = home_all || homeZ;

    destination = current_position;

    #if Z_HOME_DIR > 0  // If homing away from BED do Z first

      if (doZ) homeaxis(Z_AXIS);

    #endif

    const float z_homing_height = (
      #if ENABLED(UNKNOWN_Z_NO_RAISE)
        !TEST(axis_known_position, Z_AXIS) ? 0 :
      #endif
          (parser.seenval('R') ? parser.value_linear_units() : Z_HOMING_HEIGHT)
    );

    if (z_homing_height && (doX || doY)) {
      // Raise Z before homing any other axes and z is not already high enough (never lower z)
      destination.z = z_homing_height + (TEST(axis_known_position, Z_AXIS) ? 0.0f : current_position.z);
      if (destination.z > current_position.z) {
        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("Raise Z (before homing) to ", destination.z);
        do_blocking_move_to_z(destination.z);
      }
    }

    #if ENABLED(QUICK_HOME)

      if (doX && doY) quick_home_xy();

    #endif

    // Home Y (before X)
    #if ENABLED(HOME_Y_BEFORE_X)

      if (doY
        #if ENABLED(CODEPENDENT_XY_HOMING)
          || doX
        #endif
      ) homeaxis(Y_AXIS);

    #endif

    // Home X
    if (doX
      #if ENABLED(CODEPENDENT_XY_HOMING) && DISABLED(HOME_Y_BEFORE_X)
        || doY
      #endif
    ) {

      #if ENABLED(DUAL_X_CARRIAGE)

        // Always home the 2nd (right) extruder first
        active_extruder = 1;
        homeaxis(X_AXIS);

        // Remember this extruder's position for later tool change
        inactive_extruder_x_pos = current_position.x;

        // Home the 1st (left) extruder
        active_extruder = 0;
        homeaxis(X_AXIS);

        // Consider the active extruder to be parked
        raised_parked_position = current_position;
        delayed_move_time = 0;
        active_extruder_parked = true;

      #else

        homeaxis(X_AXIS);

      #endif
    }

    // Home Y (after X)
    #if DISABLED(HOME_Y_BEFORE_X)
      if (doY) homeaxis(Y_AXIS);
    #endif

    #if ENABLED(IMPROVE_HOMING_RELIABILITY)
      end_slow_homing(slow_homing);
    #endif

    // Home Z last if homing towards the bed
    #if Z_HOME_DIR < 0

      if (doZ) {
        #if ENABLED(BLTOUCH)
          bltouch.init();
        #endif
        #if ENABLED(Z_SAFE_HOMING)
          home_z_safely();
        #else
          homeaxis(Z_AXIS);
        #endif

        #if HOMING_Z_WITH_PROBE && defined(Z_AFTER_PROBING)
          #if Z_AFTER_HOMING > Z_AFTER_PROBING
            do_blocking_move_to_z(Z_AFTER_HOMING);
          #else
            probe.move_z_after_probing();
          #endif
        #elif defined(Z_AFTER_HOMING)
          do_blocking_move_to_z(Z_AFTER_HOMING);
        #endif

      } // doZ

    #endif // Z_HOME_DIR < 0

    sync_plan_position();

  #endif // !DELTA (G28)

  /**
   * Preserve DXC mode across a G28 for IDEX printers in DXC_DUPLICATION_MODE.
   * This is important because it lets a user use the LCD Panel to set an IDEX Duplication mode, and
   * then print a standard GCode file that contains a single print that does a G28 and has no other
   * IDEX specific commands in it.
   */
  #if ENABLED(DUAL_X_CARRIAGE)

    if (dxc_is_duplicating()) {

      #if ENABLED(IMPROVE_HOMING_RELIABILITY)
        slow_homing = begin_slow_homing();
      #endif

      // Always home the 2nd (right) extruder first
      active_extruder = 1;
      homeaxis(X_AXIS);

      // Remember this extruder's position for later tool change
      inactive_extruder_x_pos = current_position.x;

      // Home the 1st (left) extruder
      active_extruder = 0;
      homeaxis(X_AXIS);

      // Consider the active extruder to be parked
      raised_parked_position = current_position;
      delayed_move_time = 0;
      active_extruder_parked = true;
      extruder_duplication_enabled = IDEX_saved_duplication_state;
      dual_x_carriage_mode         = IDEX_saved_mode;
      stepper.set_directions();

      #if ENABLED(IMPROVE_HOMING_RELIABILITY)
        end_slow_homing(slow_homing);
      #endif
    }

  #endif // DUAL_X_CARRIAGE

  endstops.not_homing();

  // Clear endstop state for polled stallGuard endstops
  #if ENABLED(SPI_ENDSTOPS)
    endstops.clear_endstop_state();
  #endif

  #if BOTH(DELTA, DELTA_HOME_TO_SAFE_ZONE)
    // move to a height where we can use the full xy-area
    do_blocking_move_to_z(delta_clip_start_height);
  #endif

  #if HAS_LEVELING && ENABLED(RESTORE_LEVELING_AFTER_G28)
    set_bed_leveling_enabled(leveling_was_active);
  #endif

  restore_feedrate_and_scaling();

  // Restore the active tool after homing
  #if HOTENDS > 1 && (DISABLED(DELTA) || ENABLED(DELTA_HOME_TO_SAFE_ZONE))
    #if EITHER(PARKING_EXTRUDER, DUAL_X_CARRIAGE)
      #define NO_FETCH false // fetch the previous toolhead
    #else
      #define NO_FETCH true
    #endif
    tool_change(old_tool_index, NO_FETCH);
  #endif

  #if HAS_HOMING_CURRENT
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Restore driver current...");
    #if HAS_CURRENT_HOME(X)
      stepperX.rms_current(tmc_save_current_X);
    #endif
    #if HAS_CURRENT_HOME(X2)
      stepperX2.rms_current(tmc_save_current_X2);
    #endif
    #if HAS_CURRENT_HOME(Y)
      stepperY.rms_current(tmc_save_current_Y);
    #endif
    #if HAS_CURRENT_HOME(Y2)
      stepperY2.rms_current(tmc_save_current_Y2);
    #endif
  #endif

  ui.refresh();

  report_current_position();

  #if ENABLED(NANODLP_Z_SYNC)
    #if ENABLED(NANODLP_ALL_AXIS)
      #define _HOME_SYNC true       // For any axis, output sync text.
    #else
      #define _HOME_SYNC doZ        // Only for Z-axis
    #endif
    if (_HOME_SYNC)
      SERIAL_ECHOLNPGM(MSG_Z_MOVE_COMP);
  #endif

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G28");

  #if HAS_L64XX
    // Set L6470 absolute position registers to counts
    // constexpr *might* move this to PROGMEM.
    // If not, this will need a PROGMEM directive and an accessor.
    static constexpr AxisEnum L64XX_axis_xref[MAX_L64XX] = {
      X_AXIS, Y_AXIS, Z_AXIS,
      X_AXIS, Y_AXIS, Z_AXIS, Z_AXIS,
      E_AXIS, E_AXIS, E_AXIS, E_AXIS, E_AXIS, E_AXIS
    };
    for (uint8_t j = 1; j <= L64XX::chain[0]; j++) {
      const uint8_t cv = L64XX::chain[j];
      L64xxManager.set_param((L64XX_axis_t)cv, L6470_ABS_POS, stepper.position(L64XX_axis_xref[cv]));
    }
  #endif
}
