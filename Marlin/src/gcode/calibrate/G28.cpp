/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/stepper.h"
#include "../../module/endstops.h"

#if HAS_MULTI_HOTEND
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

#include "../../lcd/marlinui.h"
#if ENABLED(DWIN_CREALITY_LCD)
  #include "../../lcd/dwin/e3v2/dwin.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

#if HAS_L64XX                         // set L6470 absolute position registers to counts
  #include "../../libs/L64XX/L64XX_Marlin.h"
#endif

#if ENABLED(LASER_MOVE_G28_OFF)
  #include "../../feature/spindle_laser.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

#if ENABLED(QUICK_HOME)

  static void quick_home_xy() {

    // Pretend the current position is 0,0
    current_position.set(0.0, 0.0);
    sync_plan_position();

    const int x_axis_home_dir = TOOL_X_HOME_DIR(active_extruder);

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

    do_blocking_move_to_xy(1.5 * mlx * x_axis_home_dir, 1.5 * mly * Y_HOME_DIR, fr_mm_s);

    endstops.validate_homing_move();

    current_position.set(0.0, 0.0);

    #if ENABLED(SENSORLESS_HOMING) && DISABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
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
    DEBUG_SECTION(log_G28, "home_z_safely", DEBUGGING(LEVELING));

    // Disallow Z homing if X or Y homing is needed
    if (homing_needed_error(_BV(X_AXIS) | _BV(Y_AXIS))) return;

    sync_plan_position();

    /**
     * Move the Z probe (or just the nozzle) to the safe homing point
     * (Z is already at the right height)
     */
    constexpr xy_float_t safe_homing_xy = { Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT };
    #if HAS_HOME_OFFSET
      xy_float_t okay_homing_xy = safe_homing_xy;
      okay_homing_xy -= home_offset;
    #else
      constexpr xy_float_t okay_homing_xy = safe_homing_xy;
    #endif

    destination.set(okay_homing_xy, current_position.z);

    TERN_(HOMING_Z_WITH_PROBE, destination -= probe.offset_xy);

    if (position_is_reachable(destination)) {

      if (DEBUGGING(LEVELING)) DEBUG_POS("home_z_safely", destination);

      // Free the active extruder for movement
      TERN_(DUAL_X_CARRIAGE, idex_set_parked(false));

      TERN_(SENSORLESS_HOMING, safe_delay(500)); // Short delay needed to settle

      do_blocking_move_to_xy(destination);
      homeaxis(Z_AXIS);
    }
    else {
      LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
      SERIAL_ECHO_MSG(STR_ZPROBE_OUT_SER);
    }
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
    TERN_(HAS_CLASSIC_JERK, planner.max_jerk = slow_homing.jerk_xy);
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
 *  L<bool>   Force leveling state ON (if possible) or OFF after homing (Requires RESTORE_LEVELING_AFTER_G28 or ENABLE_LEVELING_AFTER_G28)
 *  O         Home only if the position is not known and trusted
 *  R<linear> Raise by n mm/inches before homing
 *
 * Cartesian/SCARA parameters
 *
 *  X   Home to the X endstop
 *  Y   Home to the Y endstop
 *  Z   Home to the Z endstop
 */
void GcodeSuite::G28() {
  DEBUG_SECTION(log_G28, "G28", DEBUGGING(LEVELING));
  if (DEBUGGING(LEVELING)) log_machine_info();

  TERN_(LASER_MOVE_G28_OFF, cutter.set_inline_enabled(false));  // turn off laser

  TERN_(FULL_REPORT_TO_HOST_FEATURE, set_and_report_grblstate(M_HOMING));

  #if ENABLED(DUAL_X_CARRIAGE)
    bool IDEX_saved_duplication_state = extruder_duplication_enabled;
    DualXMode IDEX_saved_mode = dual_x_carriage_mode;
  #endif

  #if ENABLED(MARLIN_DEV_MODE)
    if (parser.seen_test('S')) {
      LOOP_LINEAR_AXES(a) set_axis_is_at_home((AxisEnum)a);
      sync_plan_position();
      SERIAL_ECHOLNPGM("Simulated Homing");
      report_current_position();
      return;
    }
  #endif

  // Home (O)nly if position is unknown
  if (!axes_should_home() && parser.seen_test('O')) {
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> homing not needed, skip");
    return;
  }

  TERN_(DWIN_CREALITY_LCD, DWIN_StartHoming());
  TERN_(EXTENSIBLE_UI, ExtUI::onHomingStart());

  planner.synchronize();          // Wait for planner moves to finish!

  SET_SOFT_ENDSTOP_LOOSE(false);  // Reset a leftover 'loose' motion state

  // Disable the leveling matrix before homing
  #if CAN_SET_LEVELING_AFTER_G28
    const bool leveling_restore_state = parser.boolval('L', TERN1(RESTORE_LEVELING_AFTER_G28, planner.leveling_active));
  #endif

  // Cancel any prior G29 session
  TERN_(PROBE_MANUALLY, g29_in_progress = false);

  // Disable leveling before homing
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));

  // Reset to the XY plane
  TERN_(CNC_WORKSPACE_PLANES, workspace_plane = PLANE_XY);

  // Count this command as movement / activity
  reset_stepper_timeout();

  #define HAS_CURRENT_HOME(N) (defined(N##_CURRENT_HOME) && N##_CURRENT_HOME != N##_CURRENT)
  #if HAS_CURRENT_HOME(X) || HAS_CURRENT_HOME(X2) || HAS_CURRENT_HOME(Y) || HAS_CURRENT_HOME(Y2)
    #define HAS_HOMING_CURRENT 1
  #endif

  #if HAS_HOMING_CURRENT
    auto debug_current = [](PGM_P const s, const int16_t a, const int16_t b){
      DEBUG_ECHOPGM_P(s); DEBUG_ECHOLNPAIR(" current: ", a, " -> ", b);
    };
    #if HAS_CURRENT_HOME(X)
      const int16_t tmc_save_current_X = stepperX.getMilliamps();
      stepperX.rms_current(X_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current(PSTR("X"), tmc_save_current_X, X_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(X2)
      const int16_t tmc_save_current_X2 = stepperX2.getMilliamps();
      stepperX2.rms_current(X2_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current(PSTR("X2"), tmc_save_current_X2, X2_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(Y)
      const int16_t tmc_save_current_Y = stepperY.getMilliamps();
      stepperY.rms_current(Y_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current(PSTR("Y"), tmc_save_current_Y, Y_CURRENT_HOME);
    #endif
    #if HAS_CURRENT_HOME(Y2)
      const int16_t tmc_save_current_Y2 = stepperY2.getMilliamps();
      stepperY2.rms_current(Y2_CURRENT_HOME);
      if (DEBUGGING(LEVELING)) debug_current(PSTR("Y2"), tmc_save_current_Y2, Y2_CURRENT_HOME);
    #endif
  #endif

  TERN_(IMPROVE_HOMING_RELIABILITY, slow_homing_t slow_homing = begin_slow_homing());

  // Always home with tool 0 active
  #if HAS_MULTI_HOTEND
    #if DISABLED(DELTA) || ENABLED(DELTA_HOME_TO_SAFE_ZONE)
      const uint8_t old_tool_index = active_extruder;
    #endif
    // PARKING_EXTRUDER homing requires different handling of movement / solenoid activation, depending on the side of homing
    #if ENABLED(PARKING_EXTRUDER)
      const bool pe_final_change_must_unpark = parking_extruder_unpark_after_homing(old_tool_index, X_HOME_DIR + 1 == old_tool_index * 2);
    #endif
    tool_change(0, true);
  #endif

  TERN_(HAS_DUPLICATION_MODE, set_duplication_enabled(false));

  remember_feedrate_scaling_off();

  endstops.enable(true); // Enable endstops for next homing move

  #if ENABLED(DELTA)

    constexpr bool doZ = true; // for NANODLP_Z_SYNC if your DLP is on a DELTA

    home_delta();

    TERN_(IMPROVE_HOMING_RELIABILITY, end_slow_homing(slow_homing));

  #elif ENABLED(AXEL_TPARA)

    constexpr bool doZ = true; // for NANODLP_Z_SYNC if your DLP is on a TPARA

    home_TPARA();

  #else

    #define _UNSAFE(A) (homeZ && TERN0(Z_SAFE_HOMING, axes_should_home(_BV(A##_AXIS))))

    const bool homeZ = TERN0(HAS_Z_AXIS, parser.seen_test('Z')),
               LINEAR_AXIS_LIST(              // Other axes should be homed before Z safe-homing
                 needX = _UNSAFE(X), needY = _UNSAFE(Y), needZ = false, // UNUSED
                 needI = _UNSAFE(I), needJ = _UNSAFE(J), needK = _UNSAFE(K)
               ),
               LINEAR_AXIS_LIST(              // Home each axis if needed or flagged
                 homeX = needX || parser.seen_test('X'),
                 homeY = needY || parser.seen_test('Y'),
                 homeZZ = homeZ,
                 homeI = needI || parser.seen_test(AXIS4_NAME), homeJ = needJ || parser.seen_test(AXIS5_NAME), homeK = needK || parser.seen_test(AXIS6_NAME),
               ),
               home_all = LINEAR_AXIS_GANG(   // Home-all if all or none are flagged
                    homeX == homeX, && homeY == homeX, && homeZ == homeX,
                 && homeI == homeX, && homeJ == homeX, && homeK == homeX
               ),
               LINEAR_AXIS_LIST(
                 doX = home_all || homeX, doY = home_all || homeY, doZ = home_all || homeZ,
                 doI = home_all || homeI, doJ = home_all || homeJ, doK = home_all || homeK
               );

    #if HAS_Z_AXIS
      UNUSED(needZ); UNUSED(homeZZ);
    #else
      constexpr bool doZ = false;
    #endif

    TERN_(HOME_Z_FIRST, if (doZ) homeaxis(Z_AXIS));

    const float z_homing_height = parser.seenval('R') ? parser.value_linear_units() : Z_HOMING_HEIGHT;

    if (z_homing_height && (LINEAR_AXIS_GANG(doX, || doY, || TERN0(Z_SAFE_HOMING, doZ), || doI, || doJ, || doK))) {
      // Raise Z before homing any other axes and z is not already high enough (never lower z)
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("Raise Z (before homing) by ", z_homing_height);
      do_z_clearance(z_homing_height);
      TERN_(BLTOUCH, bltouch.init());
    }

    // Diagonal move first if both are homing
    TERN_(QUICK_HOME, if (doX && doY) quick_home_xy());

    // Home Y (before X)
    if (ENABLED(HOME_Y_BEFORE_X) && (doY || TERN0(CODEPENDENT_XY_HOMING, doX)))
      homeaxis(Y_AXIS);

    // Home X
    if (doX || (doY && ENABLED(CODEPENDENT_XY_HOMING) && DISABLED(HOME_Y_BEFORE_X))) {

      #if ENABLED(DUAL_X_CARRIAGE)

        // Always home the 2nd (right) extruder first
        active_extruder = 1;
        homeaxis(X_AXIS);

        // Remember this extruder's position for later tool change
        inactive_extruder_x = current_position.x;

        // Home the 1st (left) extruder
        active_extruder = 0;
        homeaxis(X_AXIS);

        // Consider the active extruder to be in its "parked" position
        idex_set_parked();

      #else

        homeaxis(X_AXIS);

      #endif
    }

    // Home Y (after X)
    if (DISABLED(HOME_Y_BEFORE_X) && doY)
      homeaxis(Y_AXIS);

    TERN_(IMPROVE_HOMING_RELIABILITY, end_slow_homing(slow_homing));

    // Home Z last if homing towards the bed
    #if HAS_Z_AXIS && DISABLED(HOME_Z_FIRST)
      if (doZ) {
        #if EITHER(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
          stepper.set_all_z_lock(false);
          stepper.set_separate_multi_axis(false);
        #endif

        TERN(Z_SAFE_HOMING, home_z_safely(), homeaxis(Z_AXIS));
        probe.move_z_after_homing();
      }
    #endif

    #if LINEAR_AXES >= 4
      if (doI) homeaxis(I_AXIS);
    #endif
    #if LINEAR_AXES >= 5
      if (doJ) homeaxis(J_AXIS);
    #endif
    #if LINEAR_AXES >= 6
      if (doK) homeaxis(K_AXIS);
    #endif

    sync_plan_position();

  #endif

  /**
   * Preserve DXC mode across a G28 for IDEX printers in DXC_DUPLICATION_MODE.
   * This is important because it lets a user use the LCD Panel to set an IDEX Duplication mode, and
   * then print a standard GCode file that contains a single print that does a G28 and has no other
   * IDEX specific commands in it.
   */
  #if ENABLED(DUAL_X_CARRIAGE)

    if (idex_is_duplicating()) {

      TERN_(IMPROVE_HOMING_RELIABILITY, slow_homing = begin_slow_homing());

      // Always home the 2nd (right) extruder first
      active_extruder = 1;
      homeaxis(X_AXIS);

      // Remember this extruder's position for later tool change
      inactive_extruder_x = current_position.x;

      // Home the 1st (left) extruder
      active_extruder = 0;
      homeaxis(X_AXIS);

      // Consider the active extruder to be parked
      idex_set_parked();

      dual_x_carriage_mode = IDEX_saved_mode;
      set_duplication_enabled(IDEX_saved_duplication_state);

      TERN_(IMPROVE_HOMING_RELIABILITY, end_slow_homing(slow_homing));
    }

  #endif // DUAL_X_CARRIAGE

  endstops.not_homing();

  // Clear endstop state for polled stallGuard endstops
  TERN_(SPI_ENDSTOPS, endstops.clear_endstop_state());

  // Move to a height where we can use the full xy-area
  TERN_(DELTA_HOME_TO_SAFE_ZONE, do_blocking_move_to_z(delta_clip_start_height));

  TERN_(CAN_SET_LEVELING_AFTER_G28, if (leveling_restore_state) set_bed_leveling_enabled());

  restore_feedrate_and_scaling();

  // Restore the active tool after homing
  #if HAS_MULTI_HOTEND && (DISABLED(DELTA) || ENABLED(DELTA_HOME_TO_SAFE_ZONE))
    tool_change(old_tool_index, TERN(PARKING_EXTRUDER, !pe_final_change_must_unpark, DISABLED(DUAL_X_CARRIAGE)));   // Do move if one of these
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
    #if HAS_CURRENT_HOME(I)
      stepperI.rms_current(tmc_save_current_I);
    #endif
    #if HAS_CURRENT_HOME(J)
      stepperJ.rms_current(tmc_save_current_J);
    #endif
    #if HAS_CURRENT_HOME(K)
      stepperK.rms_current(tmc_save_current_K);
    #endif
  #endif // HAS_HOMING_CURRENT

  ui.refresh();

  TERN_(DWIN_CREALITY_LCD, DWIN_CompletedHoming());
  TERN_(EXTENSIBLE_UI, ExtUI::onHomingComplete());

  report_current_position();

  if (ENABLED(NANODLP_Z_SYNC) && (doZ || ENABLED(NANODLP_ALL_AXIS)))
    SERIAL_ECHOLNPGM(STR_Z_MOVE_COMP);

  TERN_(FULL_REPORT_TO_HOST_FEATURE, set_and_report_grblstate(M_IDLE));

  #if HAS_L64XX
    // Set L6470 absolute position registers to counts
    // constexpr *might* move this to PROGMEM.
    // If not, this will need a PROGMEM directive and an accessor.
    #define _EN_ITEM(N) , E_AXIS
    static constexpr AxisEnum L64XX_axis_xref[MAX_L64XX] = {
      LINEAR_AXIS_LIST(X_AXIS, Y_AXIS, Z_AXIS, I_AXIS, J_AXIS, K_AXIS),
      X_AXIS, Y_AXIS, Z_AXIS, Z_AXIS, Z_AXIS
      REPEAT(E_STEPPERS, _EN_ITEM)
    };
    #undef _EN_ITEM
    for (uint8_t j = 1; j <= L64XX::chain[0]; j++) {
      const uint8_t cv = L64XX::chain[j];
      L64xxManager.set_param((L64XX_axis_t)cv, L6470_ABS_POS, stepper.position(L64XX_axis_xref[cv]));
    }
  #endif
}
