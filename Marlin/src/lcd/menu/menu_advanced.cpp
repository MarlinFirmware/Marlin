/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Advanced Settings Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../../module/planner.h"

#if DISABLED(NO_VOLUMETRICS)
  #include "../../gcode/parser.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
  #if ENABLED(BLTOUCH)
    #include "../../module/endstops.h"
  #endif
#endif

#if ENABLED(PIDTEMP)
  #include "../../module/temperature.h"
#endif

void menu_tmc();

#if ENABLED(DAC_STEPPER_CURRENT)

  #include "../../feature/dac/stepper_dac.h"

  uint8_t driverPercent[XYZE];
  inline void dac_driver_getValues() { LOOP_XYZE(i) driverPercent[i] = dac_current_get_percent((AxisEnum)i); }
  static void dac_driver_commit() { dac_current_set_percents(driverPercent); }

  void menu_dac() {
    dac_driver_getValues();
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    #define EDIT_DAC_PERCENT(N) MENU_ITEM_EDIT_CALLBACK(uint8, MSG_##N " " MSG_DAC_PERCENT, &driverPercent[_AXIS(N)], 0, 100, dac_driver_commit)
    EDIT_DAC_PERCENT(X);
    EDIT_DAC_PERCENT(Y);
    EDIT_DAC_PERCENT(Z);
    EDIT_DAC_PERCENT(E);
    MENU_ITEM(function, MSG_DAC_EEPROM_WRITE, dac_commit_eeprom);
    END_MENU();
  }

#endif

#if HAS_MOTOR_CURRENT_PWM

  #include "../../module/stepper.h"

  void menu_pwm() {
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    #define EDIT_CURRENT_PWM(LABEL,I) MENU_ITEM_EDIT_CALLBACK(long5, LABEL, &stepper.motor_current_setting[I], 100, 2000, stepper.refresh_motor_power)
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
      EDIT_CURRENT_PWM(MSG_X MSG_Y, 0);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
      EDIT_CURRENT_PWM(MSG_Z, 1);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
      EDIT_CURRENT_PWM(MSG_E, 2);
    #endif
    END_MENU();
  }

#endif

#if HAS_M206_COMMAND
  //
  // Set the home offset based on the current_position
  //
  void _lcd_set_home_offsets() {
    enqueue_and_echo_commands_P(PSTR("M428"));
    ui.return_to_status();
  }
#endif

#if ENABLED(SD_FIRMWARE_UPDATE)

  #include "../../module/configuration_store.h"

  //
  // Toggle the SD Firmware Update state in EEPROM
  //
  static void _lcd_toggle_sd_update() {
    const bool new_state = !settings.sd_update_status();
    ui.completion_feedback(settings.set_sd_update_status(new_state));
    ui.return_to_status();
    if (new_state) LCD_MESSAGEPGM(MSG_RESET_PRINTER); else ui.reset_status();
  }
#endif

#if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
  //
  // Advanced Settings > Filament
  //
  void menu_advanced_filament() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    #if ENABLED(LIN_ADVANCE)
      #if EXTRUDERS == 1
        MENU_ITEM_EDIT(float52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 999);
      #elif EXTRUDERS > 1
        #define EDIT_ADVANCE_K(N) MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E##N, &planner.extruder_advance_K[N-1], 0, 999)
        EDIT_ADVANCE_K(1);
        EDIT_ADVANCE_K(2);
        #if EXTRUDERS > 2
          EDIT_ADVANCE_K(3);
          #if EXTRUDERS > 3
            EDIT_ADVANCE_K(4);
            #if EXTRUDERS > 4
              EDIT_ADVANCE_K(5);
              #if EXTRUDERS > 5
                EDIT_ADVANCE_K(6);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1
    #endif

    #if DISABLED(NO_VOLUMETRICS)
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_VOLUMETRIC_ENABLED, &parser.volumetric_enabled, planner.calculate_volumetric_multipliers);

      if (parser.volumetric_enabled) {
        #if EXTRUDERS == 1
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM, &planner.filament_size[0], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
        #else // EXTRUDERS > 1
          #define EDIT_FIL_DIAM(N) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E##N, &planner.filament_size[N-1], 1.5f, 3.25f, planner.calculate_volumetric_multipliers)
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM, &planner.filament_size[active_extruder], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
          EDIT_FIL_DIAM(1);
          EDIT_FIL_DIAM(2);
          #if EXTRUDERS > 2
            EDIT_FIL_DIAM(3);
            #if EXTRUDERS > 3
              EDIT_FIL_DIAM(4);
              #if EXTRUDERS > 4
                EDIT_FIL_DIAM(5);
                #if EXTRUDERS > 5
                  EDIT_FIL_DIAM(6);
                #endif // EXTRUDERS > 5
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }
    #endif

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      constexpr float extrude_maxlength =
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          EXTRUDE_MAXLENGTH
        #else
          999
        #endif
      ;

      #if EXTRUDERS == 1
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD, &fc_settings[0].unload_length, 0, extrude_maxlength);
      #else // EXTRUDERS > 1
        #define EDIT_FIL_UNLOAD(N) MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E##N, &fc_settings[N-1].unload_length, 0, extrude_maxlength)
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD, &fc_settings[active_extruder].unload_length, 0, extrude_maxlength);
        EDIT_FIL_UNLOAD(1);
        EDIT_FIL_UNLOAD(2);
        #if EXTRUDERS > 2
          EDIT_FIL_UNLOAD(3);
          #if EXTRUDERS > 3
            EDIT_FIL_UNLOAD(4);
            #if EXTRUDERS > 4
              EDIT_FIL_UNLOAD(5);
              #if EXTRUDERS > 5
                EDIT_FIL_UNLOAD(6);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1

      #if EXTRUDERS == 1
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD, &fc_settings[0].load_length, 0, extrude_maxlength);
      #else // EXTRUDERS > 1
        #define EDIT_FIL_LOAD(N) MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E##N, &fc_settings[N-1].load_length, 0, extrude_maxlength)
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD, &fc_settings[active_extruder].load_length, 0, extrude_maxlength);
        EDIT_FIL_LOAD(1);
        EDIT_FIL_LOAD(2);
        #if EXTRUDERS > 2
          EDIT_FIL_LOAD(3);
          #if EXTRUDERS > 3
            EDIT_FIL_LOAD(4);
            #if EXTRUDERS > 4
              EDIT_FIL_LOAD(5);
              #if EXTRUDERS > 5
                EDIT_FIL_LOAD(6);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1
    #endif

    END_MENU();
  }

#endif // !NO_VOLUMETRICS || ADVANCED_PAUSE_FEATURE

//
// Advanced Settings > Temperature helpers
//

#if ENABLED(PID_AUTOTUNE_MENU)

  #if ENABLED(PIDTEMP)
    int16_t autotune_temp[HOTENDS] = ARRAY_BY_HOTENDS1(150);
  #endif

  #if ENABLED(PIDTEMPBED)
    int16_t autotune_temp_bed = 70;
  #endif

  void _lcd_autotune(const int16_t e) {
    char cmd[30];
    sprintf_P(cmd, PSTR("M303 U1 E%i S%i"), e,
      #if HAS_PID_FOR_BOTH
        e < 0 ? autotune_temp_bed : autotune_temp[e]
      #elif ENABLED(PIDTEMPBED)
        autotune_temp_bed
      #else
        autotune_temp[e]
      #endif
    );
    lcd_enqueue_command(cmd);
  }

#endif // PID_AUTOTUNE_MENU

#if ENABLED(PID_EDIT_MENU)

  float raw_Ki, raw_Kd; // place-holders for Ki and Kd edits

  // Helpers for editing PID Ki & Kd values
  // grab the PID value out of the temp variable; scale it; then update the PID driver
  void copy_and_scalePID_i(int16_t e) {
    #if DISABLED(PID_PARAMS_PER_HOTEND) || HOTENDS == 1
      UNUSED(e);
    #endif
    PID_PARAM(Ki, e) = scalePID_i(raw_Ki);
    thermalManager.updatePID();
  }
  void copy_and_scalePID_d(int16_t e) {
    #if DISABLED(PID_PARAMS_PER_HOTEND) || HOTENDS == 1
      UNUSED(e);
    #endif
    PID_PARAM(Kd, e) = scalePID_d(raw_Kd);
    thermalManager.updatePID();
  }
  #define _DEFINE_PIDTEMP_BASE_FUNCS(N) \
    void copy_and_scalePID_i_E ## N() { copy_and_scalePID_i(N); } \
    void copy_and_scalePID_d_E ## N() { copy_and_scalePID_d(N); }

#else

  #define _DEFINE_PIDTEMP_BASE_FUNCS(N) //

#endif

#if ENABLED(PID_AUTOTUNE_MENU)
  #define DEFINE_PIDTEMP_FUNCS(N) \
    _DEFINE_PIDTEMP_BASE_FUNCS(N); \
    void lcd_autotune_callback_E ## N() { _lcd_autotune(N); } //
#else
  #define DEFINE_PIDTEMP_FUNCS(N) _DEFINE_PIDTEMP_BASE_FUNCS(N); //
#endif

DEFINE_PIDTEMP_FUNCS(0);
#if ENABLED(PID_PARAMS_PER_HOTEND)
  #if HOTENDS > 1
    DEFINE_PIDTEMP_FUNCS(1);
    #if HOTENDS > 2
      DEFINE_PIDTEMP_FUNCS(2);
      #if HOTENDS > 3
        DEFINE_PIDTEMP_FUNCS(3);
        #if HOTENDS > 4
          DEFINE_PIDTEMP_FUNCS(4);
          #if HOTENDS > 5
            DEFINE_PIDTEMP_FUNCS(5);
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1
#endif // PID_PARAMS_PER_HOTEND

#define SHOW_MENU_ADVANCED_TEMPERATURE ((ENABLED(AUTOTEMP) && HAS_TEMP_HOTEND) || ENABLED(PID_AUTOTUNE_MENU) || ENABLED(PID_EDIT_MENU))

//
// Advanced Settings > Temperature
//
#if SHOW_MENU_ADVANCED_TEMPERATURE

  void menu_advanced_temperature() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);
    //
    // Autotemp, Min, Max, Fact
    //
    #if ENABLED(AUTOTEMP) && HAS_TEMP_HOTEND
      MENU_ITEM_EDIT(bool, MSG_AUTOTEMP, &planner.autotemp_enabled);
      MENU_ITEM_EDIT(float3, MSG_MIN, &planner.autotemp_min, 0, float(HEATER_0_MAXTEMP) - 15);
      MENU_ITEM_EDIT(float3, MSG_MAX, &planner.autotemp_max, 0, float(HEATER_0_MAXTEMP) - 15);
      MENU_ITEM_EDIT(float52, MSG_FACTOR, &planner.autotemp_factor, 0, 10);
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

      #define _PID_BASE_MENU_ITEMS(ELABEL, eindex) \
        raw_Ki = unscalePID_i(PID_PARAM(Ki, eindex)); \
        raw_Kd = unscalePID_d(PID_PARAM(Kd, eindex)); \
        MENU_ITEM_EDIT(float52sign, MSG_PID_P ELABEL, &PID_PARAM(Kp, eindex), 1, 9990); \
        MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_PID_I ELABEL, &raw_Ki, 0.01f, 9990, copy_and_scalePID_i_E ## eindex); \
        MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_PID_D ELABEL, &raw_Kd, 1, 9990, copy_and_scalePID_d_E ## eindex)

      #if ENABLED(PID_EXTRUSION_SCALING)
        #define _PID_EDIT_MENU_ITEMS(ELABEL, eindex) \
          _PID_BASE_MENU_ITEMS(ELABEL, eindex); \
          MENU_ITEM_EDIT(float3, MSG_PID_C ELABEL, &PID_PARAM(Kc, eindex), 1, 9990)
      #else
        #define _PID_EDIT_MENU_ITEMS(ELABEL, eindex) _PID_BASE_MENU_ITEMS(ELABEL, eindex)
      #endif

    #else

      #define _PID_EDIT_MENU_ITEMS(ELABEL, eindex) NOOP

    #endif

    #if ENABLED(PID_AUTOTUNE_MENU)
      #define PID_EDIT_MENU_ITEMS(ELABEL, eindex) \
        _PID_EDIT_MENU_ITEMS(ELABEL, eindex); \
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_PID_AUTOTUNE ELABEL, &autotune_temp[eindex], 150, heater_maxtemp[eindex] - 15, lcd_autotune_callback_E ## eindex)
    #else
      #define PID_EDIT_MENU_ITEMS(ELABEL, eindex) _PID_EDIT_MENU_ITEMS(ELABEL, eindex)
    #endif

    #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1
      PID_EDIT_MENU_ITEMS(" " MSG_E1, 0);
      PID_EDIT_MENU_ITEMS(" " MSG_E2, 1);
      #if HOTENDS > 2
        PID_EDIT_MENU_ITEMS(" " MSG_E3, 2);
        #if HOTENDS > 3
          PID_EDIT_MENU_ITEMS(" " MSG_E4, 3);
          #if HOTENDS > 4
            PID_EDIT_MENU_ITEMS(" " MSG_E5, 4);
            #if HOTENDS > 5
              PID_EDIT_MENU_ITEMS(" " MSG_E6, 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #else // !PID_PARAMS_PER_HOTEND || HOTENDS == 1
      PID_EDIT_MENU_ITEMS("", 0);
    #endif // !PID_PARAMS_PER_HOTEND || HOTENDS == 1

    END_MENU();
  }

#endif // SHOW_MENU_ADVANCED_TEMPERATURE

#if DISABLED(SLIM_LCD_MENUS)

  void _reset_acceleration_rates() { planner.reset_acceleration_rates(); }
  #if ENABLED(DISTINCT_E_FACTORS)
    void _reset_e_acceleration_rate(const uint8_t e) { if (e == active_extruder) _reset_acceleration_rates(); }
    void _reset_e0_acceleration_rate() { _reset_e_acceleration_rate(0); }
    void _reset_e1_acceleration_rate() { _reset_e_acceleration_rate(1); }
    #if E_STEPPERS > 2
      void _reset_e2_acceleration_rate() { _reset_e_acceleration_rate(2); }
      #if E_STEPPERS > 3
        void _reset_e3_acceleration_rate() { _reset_e_acceleration_rate(3); }
        #if E_STEPPERS > 4
          void _reset_e4_acceleration_rate() { _reset_e_acceleration_rate(4); }
          #if E_STEPPERS > 5
            void _reset_e5_acceleration_rate() { _reset_e_acceleration_rate(5); }
          #endif // E_STEPPERS > 5
        #endif // E_STEPPERS > 4
      #endif // E_STEPPERS > 3
    #endif // E_STEPPERS > 2
  #endif

  void _planner_refresh_positioning() { planner.refresh_positioning(); }
  #if ENABLED(DISTINCT_E_FACTORS)
    void _planner_refresh_e_positioning(const uint8_t e) {
      if (e == active_extruder)
        _planner_refresh_positioning();
      else
        planner.steps_to_mm[E_AXIS_N(e)] = 1.0f / planner.settings.axis_steps_per_mm[E_AXIS_N(e)];
    }
    void _planner_refresh_e0_positioning() { _planner_refresh_e_positioning(0); }
    void _planner_refresh_e1_positioning() { _planner_refresh_e_positioning(1); }
    #if E_STEPPERS > 2
      void _planner_refresh_e2_positioning() { _planner_refresh_e_positioning(2); }
      #if E_STEPPERS > 3
        void _planner_refresh_e3_positioning() { _planner_refresh_e_positioning(3); }
        #if E_STEPPERS > 4
          void _planner_refresh_e4_positioning() { _planner_refresh_e_positioning(4); }
          #if E_STEPPERS > 5
            void _planner_refresh_e5_positioning() { _planner_refresh_e_positioning(5); }
          #endif // E_STEPPERS > 5
        #endif // E_STEPPERS > 4
      #endif // E_STEPPERS > 3
    #endif // E_STEPPERS > 2
  #endif

  // M203 / M205 Velocity options
  void menu_advanced_velocity() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    // M203 Max Feedrate
    #define EDIT_VMAX(N) MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_##N, &planner.settings.max_feedrate_mm_s[_AXIS(N)], 1, 999)
    EDIT_VMAX(A);
    EDIT_VMAX(B);
    EDIT_VMAX(C);

    #if ENABLED(DISTINCT_E_FACTORS)
      #define EDIT_VMAX_E(N) MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E##N, &planner.settings.max_feedrate_mm_s[E_AXIS_N(N-1)], 1, 999)
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E, &planner.settings.max_feedrate_mm_s[E_AXIS_N(active_extruder)], 1, 999);
      EDIT_VMAX_E(1);
      EDIT_VMAX_E(2);
      #if E_STEPPERS > 2
        EDIT_VMAX_E(3);
        #if E_STEPPERS > 3
          EDIT_VMAX_E(4);
          #if E_STEPPERS > 4
            EDIT_VMAX_E(5);
            #if E_STEPPERS > 5
              EDIT_VMAX_E(6);
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #else
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E, &planner.settings.max_feedrate_mm_s[E_AXIS], 1, 999);
    #endif

    // M205 S Min Feedrate
    MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMIN, &planner.settings.min_feedrate_mm_s, 0, 999);

    // M205 T Min Travel Feedrate
    MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VTRAV_MIN, &planner.settings.min_travel_feedrate_mm_s, 0, 999);

    END_MENU();
  }

  // M201 / M204 Accelerations
  void menu_advanced_acceleration() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    // M204 P Acceleration
    MENU_MULTIPLIER_ITEM_EDIT(float5, MSG_ACC, &planner.settings.acceleration, 10, 99000);

    // M204 R Retract Acceleration
    MENU_MULTIPLIER_ITEM_EDIT(float5, MSG_A_RETRACT, &planner.settings.retract_acceleration, 100, 99000);

    // M204 T Travel Acceleration
    MENU_MULTIPLIER_ITEM_EDIT(float5, MSG_A_TRAVEL, &planner.settings.travel_acceleration, 100, 99000);

    // M201 settings
    #define EDIT_AMAX(Q,L) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_##Q, &planner.settings.max_acceleration_mm_per_s2[_AXIS(Q)], L, 99000, _reset_acceleration_rates)
    EDIT_AMAX(A,100);
    EDIT_AMAX(B,100);
    EDIT_AMAX(C, 10);

    #if ENABLED(DISTINCT_E_FACTORS)
      #define EDIT_AMAX_E(N,E) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E##N, &planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(E)], 100, 99000, _reset_e##E##_acceleration_rate)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(active_extruder)], 100, 99000, _reset_acceleration_rates);
      EDIT_AMAX_E(1,0);
      EDIT_AMAX_E(2,1);
      #if E_STEPPERS > 2
        EDIT_AMAX_E(3,2);
        #if E_STEPPERS > 3
          EDIT_AMAX_E(4,3);
          #if E_STEPPERS > 4
            EDIT_AMAX_E(5,4);
            #if E_STEPPERS > 5
              EDIT_AMAX_E(6,5);
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #else
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &planner.settings.max_acceleration_mm_per_s2[E_AXIS], 100, 99000, _reset_acceleration_rates);
    #endif

    END_MENU();
  }

  // M205 Jerk
  void menu_advanced_jerk() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    #if ENABLED(JUNCTION_DEVIATION)
      #if ENABLED(LIN_ADVANCE)
        MENU_ITEM_EDIT_CALLBACK(float43, MSG_JUNCTION_DEVIATION, &planner.junction_deviation_mm, 0.01f, 0.3f, planner.recalculate_max_e_jerk);
      #else
        MENU_ITEM_EDIT(float43, MSG_JUNCTION_DEVIATION, &planner.junction_deviation_mm, 0.01f, 0.3f);
      #endif
    #endif
    #if HAS_CLASSIC_JERK
      #define EDIT_JERK(N) MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_V##N##_JERK, &planner.max_jerk[_AXIS(N)], 1, 990)
      EDIT_JERK(A);
      EDIT_JERK(B);
      #if ENABLED(DELTA)
        EDIT_JERK(C);
      #else
        MENU_MULTIPLIER_ITEM_EDIT(float52sign, MSG_VC_JERK, &planner.max_jerk[C_AXIS], 0.1f, 990);
      #endif
      #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
        EDIT_JERK(E);
      #endif
    #endif

    END_MENU();
  }

  // M92 Steps-per-mm
  void menu_advanced_steps_per_mm() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    #define EDIT_QSTEPS(Q) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_##Q##STEPS, &planner.settings.axis_steps_per_mm[_AXIS(Q)], 5, 9999, _planner_refresh_positioning)
    EDIT_QSTEPS(A);
    EDIT_QSTEPS(B);
    EDIT_QSTEPS(C);

    #if ENABLED(DISTINCT_E_FACTORS)
      #define EDIT_ESTEPS(N,E) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E##N##STEPS, &planner.settings.axis_steps_per_mm[E_AXIS_N(E)], 5, 9999, _planner_refresh_e##E##_positioning)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.settings.axis_steps_per_mm[E_AXIS_N(active_extruder)], 5, 9999, _planner_refresh_positioning);
      EDIT_ESTEPS(1,0);
      EDIT_ESTEPS(2,1);
      #if E_STEPPERS > 2
        EDIT_ESTEPS(3,2);
        #if E_STEPPERS > 3
          EDIT_ESTEPS(4,3);
          #if E_STEPPERS > 4
            EDIT_ESTEPS(5,4);
            #if E_STEPPERS > 5
              EDIT_ESTEPS(6,5);
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #else
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.settings.axis_steps_per_mm[E_AXIS], 5, 9999, _planner_refresh_positioning);
    #endif

    END_MENU();
  }

  #if ENABLED(EEPROM_SETTINGS)

    #include "../../module/configuration_store.h"

    static void lcd_init_eeprom() {
      ui.completion_feedback(settings.init_eeprom());
      ui.goto_previous_screen();
    }

    static void lcd_init_eeprom_confirm() {
      START_MENU();
      MENU_BACK(MSG_ADVANCED_SETTINGS);
      MENU_ITEM(function, MSG_INIT_EEPROM, lcd_init_eeprom);
      END_MENU();
    }

  #endif

#endif // !SLIM_LCD_MENUS

void menu_advanced_settings() {
  START_MENU();
  MENU_BACK(MSG_CONFIGURATION);

  #if DISABLED(SLIM_LCD_MENUS)

    #if HAS_M206_COMMAND
      //
      // Set Home Offsets
      //
      MENU_ITEM(function, MSG_SET_HOME_OFFSETS, _lcd_set_home_offsets);
    #endif

    // M203 / M205 - Feedrate items
    MENU_ITEM(submenu, MSG_VELOCITY, menu_advanced_velocity);

    // M201 - Acceleration items
    MENU_ITEM(submenu, MSG_ACCELERATION, menu_advanced_acceleration);

    // M205 - Max Jerk
    MENU_ITEM(submenu, MSG_JERK, menu_advanced_jerk);

    if (!printer_busy()) {
      // M92 - Steps Per mm
      MENU_ITEM(submenu, MSG_STEPS_PER_MM, menu_advanced_steps_per_mm);
    }
  #endif // !SLIM_LCD_MENUS

  #if ENABLED(DAC_STEPPER_CURRENT)
    MENU_ITEM(submenu, MSG_DRIVE_STRENGTH, menu_dac);
  #endif
  #if HAS_MOTOR_CURRENT_PWM
    MENU_ITEM(submenu, MSG_DRIVE_STRENGTH, menu_pwm);
  #endif

  #if HAS_TRINAMIC
    MENU_ITEM(submenu, MSG_TMC_DRIVERS, menu_tmc);
  #endif

  #if SHOW_MENU_ADVANCED_TEMPERATURE
    MENU_ITEM(submenu, MSG_TEMPERATURE, menu_advanced_temperature);
  #endif

  #if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
    MENU_ITEM(submenu, MSG_FILAMENT, menu_advanced_filament);
  #elif ENABLED(LIN_ADVANCE)
    #if EXTRUDERS == 1
      MENU_ITEM_EDIT(float52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 999);
    #elif EXTRUDERS > 1
      #define EDIT_ADVANCE_K(N) MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E##N, &planner.extruder_advance_K[N-1], 0, 999)
      EDIT_ADVANCE_K(1);
      EDIT_ADVANCE_K(2);
      #if EXTRUDERS > 2
        EDIT_ADVANCE_K(3);
        #if EXTRUDERS > 3
          EDIT_ADVANCE_K(4);
          #if EXTRUDERS > 4
            EDIT_ADVANCE_K(5);
            #if EXTRUDERS > 5
              EDIT_ADVANCE_K(6);
            #endif // EXTRUDERS > 5
          #endif // EXTRUDERS > 4
        #endif // EXTRUDERS > 3
      #endif // EXTRUDERS > 2
    #endif // EXTRUDERS > 1
  #endif

  // M540 S - Abort on endstop hit when SD printing
  #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
    MENU_ITEM_EDIT(bool, MSG_ENDSTOP_ABORT, &planner.abort_on_endstop_hit);
  #endif

  //
  // BLTouch Self-Test and Reset
  //
  #if ENABLED(BLTOUCH)
    MENU_ITEM(gcode, MSG_BLTOUCH_SELFTEST, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_SELFTEST)));
    if (!endstops.z_probe_enabled && TEST_BLTOUCH())
      MENU_ITEM(gcode, MSG_BLTOUCH_RESET, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_RESET)));
  #endif

  #if ENABLED(SD_FIRMWARE_UPDATE)
    bool sd_update_state = settings.sd_update_status();
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_SD_UPDATE, &sd_update_state, _lcd_toggle_sd_update);
  #endif

  #if ENABLED(EEPROM_SETTINGS) && DISABLED(SLIM_LCD_MENUS)
    MENU_ITEM(submenu, MSG_INIT_EEPROM, lcd_init_eeprom_confirm);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU
