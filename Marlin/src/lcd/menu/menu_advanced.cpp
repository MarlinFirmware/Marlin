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

//
// Advanced Settings Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_MENU

#include "menu_item.h"
#include "../../module/planner.h"

#if DISABLED(NO_VOLUMETRICS)
  #include "../../gcode/parser.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#if ANY(PIDTEMP, PIDTEMPBED, PIDTEMPCHAMBER)
  #include "../../module/temperature.h"
#endif

#if HAS_FILAMENT_RUNOUT_DISTANCE
  #include "../../feature/runout.h"
#endif

#if ENABLED(SD_FIRMWARE_UPDATE)
  #include "../../module/settings.h"
#endif

#if ENABLED(PASSWORD_FEATURE)
  #include "../../feature/password/password.h"
#endif

void menu_tmc();
void menu_backlash();

#if HAS_MOTOR_CURRENT_DAC

  #include "../../feature/dac/stepper_dac.h"

  void menu_dac() {
    static xyze_uint8_t driverPercent;
    LOOP_LOGICAL_AXES(i) driverPercent[i] = stepper_dac.get_current_percent((AxisEnum)i);
    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);
    #define EDIT_DAC_PERCENT(A) EDIT_ITEM(uint8, MSG_DAC_PERCENT_##A, &driverPercent[_AXIS(A)], 0, 100, []{ stepper_dac.set_current_percents(driverPercent); })
    LOGICAL_AXIS_CODE(EDIT_DAC_PERCENT(E), EDIT_DAC_PERCENT(A), EDIT_DAC_PERCENT(B), EDIT_DAC_PERCENT(C), EDIT_DAC_PERCENT(I), EDIT_DAC_PERCENT(J), EDIT_DAC_PERCENT(K));
    ACTION_ITEM(MSG_DAC_EEPROM_WRITE, stepper_dac.commit_eeprom);
    END_MENU();
  }

#endif

#if HAS_MOTOR_CURRENT_PWM

  #include "../../module/stepper.h"

  void menu_pwm() {
    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);
    #define EDIT_CURRENT_PWM(LABEL,I) EDIT_ITEM_P(long5, PSTR(LABEL), &stepper.motor_current_setting[I], 100, 2000, stepper.refresh_motor_power)
    #if ANY_PIN(MOTOR_CURRENT_PWM_XY, MOTOR_CURRENT_PWM_X, MOTOR_CURRENT_PWM_Y)
      EDIT_CURRENT_PWM(STR_A STR_B, 0);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
      EDIT_CURRENT_PWM(STR_C, 1);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
      EDIT_CURRENT_PWM(STR_E, 2);
    #endif
    END_MENU();
  }

#endif

#if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
  //
  // Advanced Settings > Filament
  //
  void menu_advanced_filament() {
    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);

    #if ENABLED(LIN_ADVANCE)
      #if EXTRUDERS == 1
        EDIT_ITEM(float42_52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 10);
      #elif HAS_MULTI_EXTRUDER
        EXTRUDER_LOOP()
          EDIT_ITEM_N(float42_52, e, MSG_ADVANCE_K_E, &planner.extruder_advance_K[e], 0, 10);
      #endif
    #endif

    #if DISABLED(NO_VOLUMETRICS)
      EDIT_ITEM(bool, MSG_VOLUMETRIC_ENABLED, &parser.volumetric_enabled, planner.calculate_volumetric_multipliers);

      #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
        EDIT_ITEM_FAST(float42_52, MSG_VOLUMETRIC_LIMIT, &planner.volumetric_extruder_limit[active_extruder], 0.0f, 20.0f, planner.calculate_volumetric_extruder_limits);
        #if HAS_MULTI_EXTRUDER
          EXTRUDER_LOOP()
            EDIT_ITEM_FAST_N(float42_52, e, MSG_VOLUMETRIC_LIMIT_E, &planner.volumetric_extruder_limit[e], 0.0f, 20.00f, planner.calculate_volumetric_extruder_limits);
        #endif
      #endif

      if (parser.volumetric_enabled) {
        EDIT_ITEM_FAST(float43, MSG_FILAMENT_DIAM, &planner.filament_size[active_extruder], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
        #if HAS_MULTI_EXTRUDER
          EXTRUDER_LOOP()
            EDIT_ITEM_FAST_N(float43, e, MSG_FILAMENT_DIAM_E, &planner.filament_size[e], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
        #endif
      }
    #endif

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      constexpr float extrude_maxlength = TERN(PREVENT_LENGTHY_EXTRUDE, EXTRUDE_MAXLENGTH, 999);

      EDIT_ITEM_FAST(float4, MSG_FILAMENT_UNLOAD, &fc_settings[active_extruder].unload_length, 0, extrude_maxlength);
      #if HAS_MULTI_EXTRUDER
        EXTRUDER_LOOP()
          EDIT_ITEM_FAST_N(float4, e, MSG_FILAMENTUNLOAD_E, &fc_settings[e].unload_length, 0, extrude_maxlength);
      #endif

      EDIT_ITEM_FAST(float4, MSG_FILAMENT_LOAD, &fc_settings[active_extruder].load_length, 0, extrude_maxlength);
      #if HAS_MULTI_EXTRUDER
        EXTRUDER_LOOP()
          EDIT_ITEM_FAST_N(float4, e, MSG_FILAMENTLOAD_E, &fc_settings[e].load_length, 0, extrude_maxlength);
      #endif
    #endif

    #if HAS_FILAMENT_RUNOUT_DISTANCE
      editable.decimal = runout.runout_distance();
      EDIT_ITEM_FAST(float3, MSG_RUNOUT_DISTANCE_MM, &editable.decimal, 1, 999,
        []{ runout.set_runout_distance(editable.decimal); }, true
      );
    #endif

    END_MENU();
  }

#endif // !NO_VOLUMETRICS || ADVANCED_PAUSE_FEATURE

//
// Advanced Settings > Temperature helpers
//

#if ENABLED(PID_AUTOTUNE_MENU)

  #if ENABLED(PIDTEMP)
    int16_t autotune_temp[HOTENDS] = ARRAY_BY_HOTENDS1(PREHEAT_1_TEMP_HOTEND);
  #endif
  #if ENABLED(PIDTEMPBED)
    int16_t autotune_temp_bed = PREHEAT_1_TEMP_BED;
  #endif
  #if ENABLED(PIDTEMPCHAMBER)
    int16_t autotune_temp_chamber = PREHEAT_1_TEMP_CHAMBER;
  #endif

  #include "../../gcode/queue.h"

  void _lcd_autotune(const heater_id_t hid) {
    char cmd[30];
    int16_t tune_temp;
    switch (hid) {
      #if ENABLED(PIDTEMPBED)
        case H_BED: tune_temp = autotune_temp_bed; break;
      #endif
      #if ENABLED(PIDTEMPCHAMBER)
        case H_CHAMBER: tune_temp = autotune_temp_chamber; break;
      #endif
      default:
        #if ENABLED(PIDTEMP)
          tune_temp = autotune_temp[hid]; break;
        #else
          return;
        #endif
    }
    sprintf_P(cmd, PSTR("M303 U1 E%i S%i"), hid, tune_temp);
    queue.inject(cmd);
    ui.return_to_status();
  }

#endif // PID_AUTOTUNE_MENU

#if ENABLED(PID_EDIT_MENU)

  float raw_Ki, raw_Kd; // place-holders for Ki and Kd edits

  // Helpers for editing PID Ki & Kd values
  // grab the PID value out of the temp variable; scale it; then update the PID driver
  void copy_and_scalePID_i(int16_t e) {
    switch (e) {
      #if ENABLED(PIDTEMPBED)
        case H_BED: thermalManager.temp_bed.pid.Ki = scalePID_i(raw_Ki); break;
      #endif
      #if ENABLED(PIDTEMPCHAMBER)
        case H_CHAMBER: thermalManager.temp_chamber.pid.Ki = scalePID_i(raw_Ki); break;
      #endif
      default:
        #if ENABLED(PIDTEMP)
          PID_PARAM(Ki, e) = scalePID_i(raw_Ki);
          thermalManager.updatePID();
        #endif
        break;
    }
  }
  void copy_and_scalePID_d(int16_t e) {
    switch (e) {
      #if ENABLED(PIDTEMPBED)
        case H_BED: thermalManager.temp_bed.pid.Kd = scalePID_d(raw_Kd); break;
      #endif
      #if ENABLED(PIDTEMPCHAMBER)
        case H_CHAMBER: thermalManager.temp_chamber.pid.Kd = scalePID_d(raw_Kd); break;
      #endif
      default:
        #if ENABLED(PIDTEMP)
          PID_PARAM(Kd, e) = scalePID_d(raw_Kd);
          thermalManager.updatePID();
        #endif
        break;
    }
  }

  #define _DEFINE_PIDTEMP_BASE_FUNCS(N) \
    void copy_and_scalePID_i_E##N() { copy_and_scalePID_i(N); } \
    void copy_and_scalePID_d_E##N() { copy_and_scalePID_d(N); }

#else

  #define _DEFINE_PIDTEMP_BASE_FUNCS(N) //

#endif

#if ENABLED(PID_AUTOTUNE_MENU)
  #define DEFINE_PIDTEMP_FUNCS(N) \
    _DEFINE_PIDTEMP_BASE_FUNCS(N); \
    void lcd_autotune_callback_E##N() { _lcd_autotune(heater_id_t(N)); }
#else
  #define DEFINE_PIDTEMP_FUNCS(N) _DEFINE_PIDTEMP_BASE_FUNCS(N);
#endif

#if HAS_HOTEND
  DEFINE_PIDTEMP_FUNCS(0);
  #if ENABLED(PID_PARAMS_PER_HOTEND)
    REPEAT_S(1, HOTENDS, DEFINE_PIDTEMP_FUNCS)
  #endif
#endif

#if BOTH(AUTOTEMP, HAS_TEMP_HOTEND) || EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
  #define SHOW_MENU_ADVANCED_TEMPERATURE 1
#endif

//
// Advanced Settings > Temperature
//
#if SHOW_MENU_ADVANCED_TEMPERATURE

  void menu_advanced_temperature() {
    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);
    //
    // Autotemp, Min, Max, Fact
    //
    #if BOTH(AUTOTEMP, HAS_TEMP_HOTEND)
      EDIT_ITEM(bool, MSG_AUTOTEMP, &planner.autotemp_enabled);
      EDIT_ITEM(int3, MSG_MIN, &planner.autotemp_min, 0, thermalManager.hotend_max_target(0));
      EDIT_ITEM(int3, MSG_MAX, &planner.autotemp_max, 0, thermalManager.hotend_max_target(0));
      EDIT_ITEM(float42_52, MSG_FACTOR, &planner.autotemp_factor, 0, 10);
    #endif

    //
    // PID-P, PID-I, PID-D, PID-C, PID Autotune
    // PID-P E1, PID-I E1, PID-D E1, PID-C E1, PID Autotune E1
    // PID-P E2, PID-I E2, PID-D E2, PID-C E2, PID Autotune E2
    // PID-P E3, PID-I E3, PID-D E3, PID-C E3, PID Autotune E3
    // PID-P E4, PID-I E4, PID-D E4, PID-C E4, PID Autotune E4
    // PID-P E5, PID-I E5, PID-D E5, PID-C E5, PID Autotune E5
    //

    #if ENABLED(PID_EDIT_MENU)
      #define _PID_EDIT_ITEMS_TMPL(N,T) \
        raw_Ki = unscalePID_i(T.pid.Ki); \
        raw_Kd = unscalePID_d(T.pid.Kd); \
        EDIT_ITEM_FAST_N(float41sign, N, MSG_PID_P_E, &T.pid.Kp, 1, 9990); \
        EDIT_ITEM_FAST_N(float52sign, N, MSG_PID_I_E, &raw_Ki, 0.01f, 9990, []{ copy_and_scalePID_i(N); }); \
        EDIT_ITEM_FAST_N(float41sign, N, MSG_PID_D_E, &raw_Kd, 1, 9990, []{ copy_and_scalePID_d(N); })

      #define __PID_HOTEND_MENU_ITEMS(N) \
        raw_Ki = unscalePID_i(PID_PARAM(Ki, N)); \
        raw_Kd = unscalePID_d(PID_PARAM(Kd, N)); \
        EDIT_ITEM_FAST_N(float41sign, N, MSG_PID_P_E, &PID_PARAM(Kp, N), 1, 9990); \
        EDIT_ITEM_FAST_N(float52sign, N, MSG_PID_I_E, &raw_Ki, 0.01f, 9990, []{ copy_and_scalePID_i(N); }); \
        EDIT_ITEM_FAST_N(float41sign, N, MSG_PID_D_E, &raw_Kd, 1, 9990, []{ copy_and_scalePID_d(N); })

      #if ENABLED(PID_EXTRUSION_SCALING)
        #define _PID_HOTEND_MENU_ITEMS(N) \
          __PID_HOTEND_MENU_ITEMS(N); \
          EDIT_ITEM_N(float4, N, MSG_PID_C_E, &PID_PARAM(Kc, N), 1, 9990)
      #else
        #define _PID_HOTEND_MENU_ITEMS(N) __PID_HOTEND_MENU_ITEMS(N)
      #endif

      #if ENABLED(PID_FAN_SCALING)
        #define _HOTEND_PID_EDIT_MENU_ITEMS(N) \
          _PID_HOTEND_MENU_ITEMS(N); \
          EDIT_ITEM_N(float4, N, MSG_PID_F_E, &PID_PARAM(Kf, N), 1, 9990)
      #else
        #define _HOTEND_PID_EDIT_MENU_ITEMS(N) _PID_HOTEND_MENU_ITEMS(N)
      #endif

    #else

      #define _HOTEND_PID_EDIT_MENU_ITEMS(N) NOOP

    #endif

    #if ENABLED(PID_AUTOTUNE_MENU)
      #define HOTEND_PID_EDIT_MENU_ITEMS(N) \
        _HOTEND_PID_EDIT_MENU_ITEMS(N); \
        EDIT_ITEM_FAST_N(int3, N, MSG_PID_AUTOTUNE_E, &autotune_temp[N], 150, thermalManager.hotend_max_target(N), []{ _lcd_autotune(heater_id_t(MenuItemBase::itemIndex)); });
    #else
      #define HOTEND_PID_EDIT_MENU_ITEMS(N) _HOTEND_PID_EDIT_MENU_ITEMS(N);
    #endif

    HOTEND_PID_EDIT_MENU_ITEMS(0);
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      REPEAT_S(1, HOTENDS, HOTEND_PID_EDIT_MENU_ITEMS)
    #endif

    #if ENABLED(PIDTEMPBED)
      #if ENABLED(PID_EDIT_MENU)
        _PID_EDIT_ITEMS_TMPL(H_BED, thermalManager.temp_bed);
      #endif
      #if ENABLED(PID_AUTOTUNE_MENU)
        EDIT_ITEM_FAST_N(int3, H_BED, MSG_PID_AUTOTUNE_E, &autotune_temp_bed, PREHEAT_1_TEMP_BED, BED_MAX_TARGET, []{ _lcd_autotune(H_BED); });
      #endif
    #endif

    #if ENABLED(PIDTEMPCHAMBER)
      #if ENABLED(PID_EDIT_MENU)
        _PID_EDIT_ITEMS_TMPL(H_CHAMBER, thermalManager.temp_chamber);
      #endif
      #if ENABLED(PID_AUTOTUNE_MENU)
        EDIT_ITEM_FAST_N(int3, H_CHAMBER, MSG_PID_AUTOTUNE_E, &autotune_temp_chamber, PREHEAT_1_TEMP_CHAMBER, CHAMBER_MAX_TARGET, []{ _lcd_autotune(H_CHAMBER); });
      #endif
    #endif

    END_MENU();
  }

#endif // SHOW_MENU_ADVANCED_TEMPERATURE

#if DISABLED(SLIM_LCD_MENUS)

  // M203 / M205 Velocity options
  void menu_advanced_velocity() {
    // M203 Max Feedrate
    constexpr xyze_feedrate_t max_fr_edit =
      #ifdef MAX_FEEDRATE_EDIT_VALUES
        MAX_FEEDRATE_EDIT_VALUES
      #elif ENABLED(LIMITED_MAX_FR_EDITING)
        DEFAULT_MAX_FEEDRATE
      #else
        LOGICAL_AXIS_ARRAY(9999, 9999, 9999, 9999, 9999, 9999, 9999)
      #endif
    ;
    #if ENABLED(LIMITED_MAX_FR_EDITING) && !defined(MAX_FEEDRATE_EDIT_VALUES)
      const xyze_feedrate_t max_fr_edit_scaled = max_fr_edit * 2;
    #else
      const xyze_feedrate_t &max_fr_edit_scaled = max_fr_edit;
    #endif

    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);

    #define EDIT_VMAX(N) EDIT_ITEM_FAST(float5, MSG_VMAX_##N, &planner.settings.max_feedrate_mm_s[_AXIS(N)], 1, max_fr_edit_scaled[_AXIS(N)])
    LINEAR_AXIS_CODE(EDIT_VMAX(A), EDIT_VMAX(B), EDIT_VMAX(C), EDIT_VMAX(I), EDIT_VMAX(J), EDIT_VMAX(K));

    #if E_STEPPERS
      EDIT_ITEM_FAST(float5, MSG_VMAX_E, &planner.settings.max_feedrate_mm_s[E_AXIS_N(active_extruder)], 1, max_fr_edit_scaled.e);
    #endif
    #if ENABLED(DISTINCT_E_FACTORS)
      LOOP_L_N(n, E_STEPPERS)
        EDIT_ITEM_FAST_N(float5, n, MSG_VMAX_EN, &planner.settings.max_feedrate_mm_s[E_AXIS_N(n)], 1, max_fr_edit_scaled.e);
    #endif

    // M205 S Min Feedrate
    EDIT_ITEM_FAST(float5, MSG_VMIN, &planner.settings.min_feedrate_mm_s, 0, 9999);

    // M205 T Min Travel Feedrate
    EDIT_ITEM_FAST(float5, MSG_VTRAV_MIN, &planner.settings.min_travel_feedrate_mm_s, 0, 9999);

    END_MENU();
  }

  // M201 / M204 Accelerations
  void menu_advanced_acceleration() {
    const float max_accel = _MAX(planner.settings.max_acceleration_mm_per_s2[A_AXIS], planner.settings.max_acceleration_mm_per_s2[B_AXIS], planner.settings.max_acceleration_mm_per_s2[C_AXIS]);

    // M201 settings
    constexpr xyze_ulong_t max_accel_edit =
      #ifdef MAX_ACCEL_EDIT_VALUES
        MAX_ACCEL_EDIT_VALUES
      #elif ENABLED(LIMITED_MAX_ACCEL_EDITING)
        DEFAULT_MAX_ACCELERATION
      #else
        LOGICAL_AXIS_ARRAY(99000, 99000, 99000, 99000, 99000, 99000, 99000)
      #endif
    ;
    #if ENABLED(LIMITED_MAX_ACCEL_EDITING) && !defined(MAX_ACCEL_EDIT_VALUES)
      const xyze_ulong_t max_accel_edit_scaled = max_accel_edit * 2;
    #else
      const xyze_ulong_t &max_accel_edit_scaled = max_accel_edit;
    #endif

    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);

    // M204 P Acceleration
    EDIT_ITEM_FAST(float5_25, MSG_ACC, &planner.settings.acceleration, 25, max_accel);

    #if HAS_EXTRUDERS
      // M204 R Retract Acceleration
      EDIT_ITEM_FAST(float5, MSG_A_RETRACT, &planner.settings.retract_acceleration, 100, planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(active_extruder)]);
    #endif

    // M204 T Travel Acceleration
    EDIT_ITEM_FAST(float5_25, MSG_A_TRAVEL, &planner.settings.travel_acceleration, 25, max_accel);

    #define EDIT_AMAX(Q,L) EDIT_ITEM_FAST(long5_25, MSG_AMAX_##Q, &planner.settings.max_acceleration_mm_per_s2[_AXIS(Q)], L, max_accel_edit_scaled[_AXIS(Q)], []{ planner.reset_acceleration_rates(); })
    LINEAR_AXIS_CODE(
      EDIT_AMAX(A, 100), EDIT_AMAX(B, 100), EDIT_AMAX(C, 10),
      EDIT_AMAX(I,  10), EDIT_AMAX(J,  10), EDIT_AMAX(K, 10)
    );

    #if ENABLED(DISTINCT_E_FACTORS)
      EDIT_ITEM_FAST(long5_25, MSG_AMAX_E, &planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(active_extruder)], 100, max_accel_edit_scaled.e, []{ planner.reset_acceleration_rates(); });
      LOOP_L_N(n, E_STEPPERS)
        EDIT_ITEM_FAST_N(long5_25, n, MSG_AMAX_EN, &planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(n)], 100, max_accel_edit_scaled.e, []{
          if (MenuItemBase::itemIndex == active_extruder)
            planner.reset_acceleration_rates();
       });
    #elif E_STEPPERS
      EDIT_ITEM_FAST(long5_25, MSG_AMAX_E, &planner.settings.max_acceleration_mm_per_s2[E_AXIS], 100, max_accel_edit_scaled.e, []{ planner.reset_acceleration_rates(); });
    #endif

    #ifdef XY_FREQUENCY_LIMIT
      EDIT_ITEM(int8, MSG_XY_FREQUENCY_LIMIT, &planner.xy_freq_limit_hz, 0, 100, planner.refresh_frequency_limit, true);
      editable.uint8 = uint8_t(LROUND(planner.xy_freq_min_speed_factor * 255)); // percent to u8
      EDIT_ITEM(percent, MSG_XY_FREQUENCY_FEEDRATE, &editable.uint8, 3, 255, []{ planner.set_min_speed_factor_u8(editable.uint8); }, true);
    #endif

    END_MENU();
  }

  #if HAS_CLASSIC_JERK

    void menu_advanced_jerk() {
      START_MENU();
      BACK_ITEM(MSG_ADVANCED_SETTINGS);

      #if HAS_JUNCTION_DEVIATION
        #if ENABLED(LIN_ADVANCE)
          EDIT_ITEM(float43, MSG_JUNCTION_DEVIATION, &planner.junction_deviation_mm, 0.001f, 0.3f, planner.recalculate_max_e_jerk);
        #else
          EDIT_ITEM(float43, MSG_JUNCTION_DEVIATION, &planner.junction_deviation_mm, 0.001f, 0.5f);
        #endif
      #endif

      constexpr xyze_float_t max_jerk_edit =
        #ifdef MAX_JERK_EDIT_VALUES
          MAX_JERK_EDIT_VALUES
        #elif ENABLED(LIMITED_JERK_EDITING)
          { LOGICAL_AXIS_LIST((DEFAULT_EJERK) * 2,
                              (DEFAULT_XJERK) * 2, (DEFAULT_YJERK) * 2, (DEFAULT_ZJERK) * 2,
                              (DEFAULT_IJERK) * 2, (DEFAULT_JJERK) * 2, (DEFAULT_KJERK) * 2) }
        #else
          { LOGICAL_AXIS_LIST(990, 990, 990, 990, 990, 990, 990) }
        #endif
      ;
      #define EDIT_JERK(N) EDIT_ITEM_FAST(float3, MSG_V##N##_JERK, &planner.max_jerk[_AXIS(N)], 1, max_jerk_edit[_AXIS(N)])
      #if ENABLED(DELTA)
        #define EDIT_JERK_C() EDIT_JERK(C)
      #else
        #define EDIT_JERK_C() EDIT_ITEM_FAST(float52sign, MSG_VC_JERK, &planner.max_jerk.c, 0.1f, max_jerk_edit.c)
      #endif
      LINEAR_AXIS_CODE(
        EDIT_JERK(A), EDIT_JERK(B), EDIT_JERK_C(),
        EDIT_JERK(I), EDIT_JERK(J), EDIT_JERK(K)
      );

      #if HAS_EXTRUDERS
        EDIT_ITEM_FAST(float52sign, MSG_VE_JERK, &planner.max_jerk.e, 0.1f, max_jerk_edit.e);
      #endif

      END_MENU();
    }

  #endif

  // M851 - Z Probe Offsets
  #if HAS_BED_PROBE
    void menu_probe_offsets() {
      START_MENU();
      BACK_ITEM(MSG_ADVANCED_SETTINGS);
      #if HAS_PROBE_XY_OFFSET
        EDIT_ITEM(float31sign, MSG_ZPROBE_XOFFSET, &probe.offset.x, -(X_BED_SIZE), X_BED_SIZE);
        EDIT_ITEM(float31sign, MSG_ZPROBE_YOFFSET, &probe.offset.y, -(Y_BED_SIZE), Y_BED_SIZE);
      #endif
      EDIT_ITEM(LCD_Z_OFFSET_TYPE, MSG_ZPROBE_ZOFFSET, &probe.offset.z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);

      #if ENABLED(PROBE_OFFSET_WIZARD)
        SUBMENU(MSG_PROBE_WIZARD, goto_probe_offset_wizard);
      #endif

      #if ENABLED(X_AXIS_TWIST_COMPENSATION)
        SUBMENU(MSG_XATC, xatc_wizard_continue);
      #endif

      END_MENU();
    }
  #endif

#endif // !SLIM_LCD_MENUS

// M92 Steps-per-mm
void menu_advanced_steps_per_mm() {
  START_MENU();
  BACK_ITEM(MSG_ADVANCED_SETTINGS);

  #define EDIT_QSTEPS(Q) EDIT_ITEM_FAST(float61, MSG_##Q##_STEPS, &planner.settings.axis_steps_per_mm[_AXIS(Q)], 5, 9999, []{ planner.refresh_positioning(); })
  LINEAR_AXIS_CODE(
    EDIT_QSTEPS(A), EDIT_QSTEPS(B), EDIT_QSTEPS(C),
    EDIT_QSTEPS(I), EDIT_QSTEPS(J), EDIT_QSTEPS(K)
  );

  #if ENABLED(DISTINCT_E_FACTORS)
    LOOP_L_N(n, E_STEPPERS)
      EDIT_ITEM_FAST_N(float61, n, MSG_EN_STEPS, &planner.settings.axis_steps_per_mm[E_AXIS_N(n)], 5, 9999, []{
        const uint8_t e = MenuItemBase::itemIndex;
        if (e == active_extruder)
          planner.refresh_positioning();
        else
          planner.mm_per_step[E_AXIS_N(e)] = 1.0f / planner.settings.axis_steps_per_mm[E_AXIS_N(e)];
      });
  #elif E_STEPPERS
    EDIT_ITEM_FAST(float61, MSG_E_STEPS, &planner.settings.axis_steps_per_mm[E_AXIS], 5, 9999, []{ planner.refresh_positioning(); });
  #endif

  END_MENU();
}

void menu_advanced_settings() {
  const bool is_busy = printer_busy();

  #if ENABLED(SD_FIRMWARE_UPDATE)
    bool sd_update_state = settings.sd_update_status();
  #endif

  START_MENU();
  BACK_ITEM(MSG_CONFIGURATION);

  #if DISABLED(SLIM_LCD_MENUS)

    #if HAS_M206_COMMAND
      //
      // Set Home Offsets
      //
      ACTION_ITEM(MSG_SET_HOME_OFFSETS, []{ queue.inject(F("M428")); ui.return_to_status(); });
    #endif

    // M203 / M205 - Feedrate items
    SUBMENU(MSG_VELOCITY, menu_advanced_velocity);

    // M201 - Acceleration items
    SUBMENU(MSG_ACCELERATION, menu_advanced_acceleration);

    #if HAS_CLASSIC_JERK
      // M205 - Max Jerk
      SUBMENU(MSG_JERK, menu_advanced_jerk);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(float43, MSG_JUNCTION_DEVIATION, &planner.junction_deviation_mm, 0.001f, 0.3f
        OPTARG(LIN_ADVANCE, planner.recalculate_max_e_jerk)
      );
    #endif

    // M851 - Z Probe Offsets
    #if HAS_BED_PROBE
      if (!is_busy) SUBMENU(MSG_ZPROBE_OFFSETS, menu_probe_offsets);
    #endif

  #endif // !SLIM_LCD_MENUS

  // M92 - Steps Per mm
  if (!is_busy)
    SUBMENU(MSG_STEPS_PER_MM, menu_advanced_steps_per_mm);

  #if ENABLED(BACKLASH_GCODE)
    SUBMENU(MSG_BACKLASH, menu_backlash);
  #endif

  #if HAS_MOTOR_CURRENT_DAC
    SUBMENU(MSG_DRIVE_STRENGTH, menu_dac);
  #endif
  #if HAS_MOTOR_CURRENT_PWM
    SUBMENU(MSG_DRIVE_STRENGTH, menu_pwm);
  #endif

  #if HAS_TRINAMIC_CONFIG
    SUBMENU(MSG_TMC_DRIVERS, menu_tmc);
  #endif

  #if SHOW_MENU_ADVANCED_TEMPERATURE
    SUBMENU(MSG_TEMPERATURE, menu_advanced_temperature);
  #endif

  #if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
    SUBMENU(MSG_FILAMENT, menu_advanced_filament);
  #elif ENABLED(LIN_ADVANCE)
    #if EXTRUDERS == 1
      EDIT_ITEM(float42_52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 10);
    #elif HAS_MULTI_EXTRUDER
      LOOP_L_N(n, E_STEPPERS)
        EDIT_ITEM_N(float42_52, n, MSG_ADVANCE_K_E, &planner.extruder_advance_K[n], 0, 10);
    #endif
  #endif

  // M540 S - Abort on endstop hit when SD printing
  #if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)
    EDIT_ITEM(bool, MSG_ENDSTOP_ABORT, &planner.abort_on_endstop_hit);
  #endif

  #if ENABLED(SD_FIRMWARE_UPDATE)
    EDIT_ITEM(bool, MSG_MEDIA_UPDATE, &sd_update_state, []{
      //
      // Toggle the SD Firmware Update state in EEPROM
      //
      const bool new_state = !settings.sd_update_status(),
                 didset = settings.set_sd_update_status(new_state);
      ui.completion_feedback(didset);
      ui.return_to_status();
      if (new_state) LCD_MESSAGE(MSG_RESET_PRINTER); else ui.reset_status();
    });
  #endif

  #if ENABLED(PASSWORD_FEATURE)
    SUBMENU(MSG_PASSWORD_SETTINGS, password.access_menu_password);
  #endif

  #if ENABLED(EEPROM_SETTINGS) && DISABLED(SLIM_LCD_MENUS)
    CONFIRM_ITEM(MSG_INIT_EEPROM,
      MSG_BUTTON_INIT, MSG_BUTTON_CANCEL,
      ui.init_eeprom, nullptr,
      GET_TEXT(MSG_INIT_EEPROM), (const char *)nullptr, PSTR("?")
    );
  #endif

  END_MENU();
}

#endif // HAS_MARLINUI_MENU
