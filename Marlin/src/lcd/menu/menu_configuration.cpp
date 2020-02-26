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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Configuration Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"

#include "../../module/configuration_store.h"

#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/power_loss_recovery.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
  #if ENABLED(BLTOUCH)
    #include "../../feature/bltouch.h"
  #endif
#endif

#define HAS_DEBUG_MENU ENABLED(LCD_PROGRESS_BAR_TEST)

void menu_advanced_settings();
#if EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)
  void menu_delta_calibrate();
#endif

#if ENABLED(LCD_PROGRESS_BAR_TEST)

  #include "../lcdprint.h"

  static void progress_bar_test() {
    static int8_t bar_percent = 0;
    if (ui.use_click()) {
      ui.goto_previous_screen();
      ui.set_custom_characters(CHARSET_MENU);
      return;
    }
    bar_percent += (int8_t)ui.encoderPosition;
    LIMIT(bar_percent, 0, 100);
    ui.encoderPosition = 0;
    MenuItem_static::draw(0, GET_TEXT(MSG_PROGRESS_BAR_TEST), SS_CENTER|SS_INVERT);
    lcd_put_int((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2, bar_percent); lcd_put_wchar('%');
    lcd_moveto(0, LCD_HEIGHT - 1); ui.draw_progress_bar(bar_percent);
  }

  void _progress_bar_test() {
    ui.goto_screen(progress_bar_test);
    ui.set_custom_characters(CHARSET_INFO);
  }

#endif // LCD_PROGRESS_BAR_TEST

#if HAS_DEBUG_MENU

  void menu_debug() {
    START_MENU();

    BACK_ITEM(MSG_CONFIGURATION);

    #if ENABLED(LCD_PROGRESS_BAR_TEST)
      SUBMENU(MSG_PROGRESS_BAR_TEST, _progress_bar_test);
    #endif

    END_MENU();
  }

#endif

#if EXTRUDERS > 1

  #include "../../module/tool_change.h"

  void menu_tool_change() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      static constexpr float max_extrude =
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          EXTRUDE_MAXLENGTH
        #else
          500
        #endif
      ;
      EDIT_ITEM(float3, MSG_FILAMENT_SWAP_LENGTH, &toolchange_settings.swap_length, 0, max_extrude);
      EDIT_ITEM(float3, MSG_FILAMENT_PURGE_LENGTH, &toolchange_settings.extra_prime, 0, max_extrude);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_RETRACT_SPD, &toolchange_settings.retract_speed, 10, 5400);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_PRIME_SPD, &toolchange_settings.prime_speed, 10, 5400);
    #endif
    EDIT_ITEM(float3, MSG_TOOL_CHANGE_ZLIFT, &toolchange_settings.z_raise, 0, 10);
    END_MENU();
  }

#endif

#if HAS_HOTEND_OFFSET
  #include "../../module/motion.h"
  #include "../../gcode/queue.h"

  void menu_tool_offsets() {

    auto _recalc_offsets = []{
      if (active_extruder && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        active_extruder = 0;
      }
    };

    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    #if ENABLED(DUAL_X_CARRIAGE)
      EDIT_ITEM_FAST(float51, MSG_HOTEND_OFFSET_X, &hotend_offset[1].x, float(X2_HOME_POS - 25), float(X2_HOME_POS + 25), _recalc_offsets);
    #else
      EDIT_ITEM_FAST(float41sign, MSG_HOTEND_OFFSET_X, &hotend_offset[1].x, -99.0, 99.0, _recalc_offsets);
    #endif
    EDIT_ITEM_FAST(float41sign, MSG_HOTEND_OFFSET_Y, &hotend_offset[1].y, -99.0, 99.0, _recalc_offsets);
    EDIT_ITEM_FAST(float41sign, MSG_HOTEND_OFFSET_Z, &hotend_offset[1].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  void menu_idex() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);

    GCODES_ITEM(MSG_IDEX_MODE_AUTOPARK,  PSTR("M605 S1\nG28 X\nG1 X100"));
    const bool need_g28 = !(TEST(axis_known_position, Y_AXIS) && TEST(axis_known_position, Z_AXIS));
    GCODES_ITEM(MSG_IDEX_MODE_DUPLICATE, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100")                // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100")
    );
    GCODES_ITEM(MSG_IDEX_MODE_MIRRORED_COPY, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")  // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")
    );
    GCODES_ITEM(MSG_IDEX_MODE_FULL_CTRL, PSTR("M605 S0\nG28 X"));
    END_MENU();
  }

#endif

#if ENABLED(BLTOUCH)

  #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
    void bltouch_report() {
      SERIAL_ECHOLNPAIR("EEPROM Last BLTouch Mode - ", (int)bltouch.last_written_mode);
      SERIAL_ECHOLNPGM("Configuration BLTouch Mode - "
        #if ENABLED(BLTOUCH_SET_5V_MODE)
          "5V"
        #else
          "OD"
        #endif
      );
      char mess[21];
      strcpy_P(mess, PSTR("BLTouch Mode - "));
      strcpy_P(&mess[15], bltouch.last_written_mode ? PSTR("5V") : PSTR("OD"));
      ui.set_status(mess);
      ui.return_to_status();
    }
  #endif

  void menu_bltouch() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    ACTION_ITEM(MSG_BLTOUCH_RESET, bltouch._reset);
    ACTION_ITEM(MSG_BLTOUCH_SELFTEST, bltouch._selftest);
    ACTION_ITEM(MSG_BLTOUCH_DEPLOY, bltouch._deploy);
    ACTION_ITEM(MSG_BLTOUCH_STOW, bltouch._stow);
    ACTION_ITEM(MSG_BLTOUCH_SW_MODE, bltouch._set_SW_mode);
    #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
      CONFIRM_ITEM(MSG_BLTOUCH_5V_MODE, MSG_BLTOUCH_5V_MODE, MSG_BUTTON_CANCEL, bltouch._set_5V_mode, ui.goto_previous_screen, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_OD_MODE, MSG_BLTOUCH_OD_MODE, MSG_BUTTON_CANCEL, bltouch._set_OD_mode, ui.goto_previous_screen, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      ACTION_ITEM(MSG_BLTOUCH_MODE_STORE, bltouch._mode_store);
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_5V, MSG_BLTOUCH_MODE_STORE_5V, MSG_BUTTON_CANCEL, bltouch.mode_conv_5V, ui.goto_previous_screen, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_OD, MSG_BLTOUCH_MODE_STORE_OD, MSG_BUTTON_CANCEL, bltouch.mode_conv_OD, ui.goto_previous_screen, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      ACTION_ITEM(MSG_BLTOUCH_MODE_ECHO, bltouch_report);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(TOUCH_MI_PROBE)
  void menu_touchmi() {
    START_MENU();
    ui.defer_status_screen();
    BACK_ITEM(MSG_CONFIGURATION);
    GCODES_ITEM(MSG_TOUCHMI_INIT, PSTR("M851 Z0\nG28\nG1 F200 Z0"));
    SUBMENU(MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    GCODES_ITEM(MSG_TOUCHMI_SAVE, PSTR("M500\nG1 F200 Z10"));
    GCODES_ITEM(MSG_TOUCHMI_ZTEST, PSTR("G28\nG1 F200 Z0"));
    END_MENU();
  }
#endif

#if ENABLED(CASE_LIGHT_MENU)

  #include "../../feature/caselight.h"

  #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)

    void menu_case_light() {
      START_MENU();
      BACK_ITEM(MSG_CONFIGURATION);
      EDIT_ITEM(percent, MSG_CASE_LIGHT_BRIGHTNESS, &case_light_brightness, 0, 255, update_case_light, true);
      EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
      END_MENU();
    }

  #endif

#endif

#if ENABLED(FWRETRACT)

  #include "../../feature/fwretract.h"

  void menu_config_retract() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    #if ENABLED(FWRETRACT_AUTORETRACT)
      EDIT_ITEM(bool, MSG_AUTORETRACT, &fwretract.autoretract_enabled, fwretract.refresh_autoretract);
    #endif
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT, &fwretract.settings.retract_length, 0, 100);
    #if EXTRUDERS > 1
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_SWAP, &fwretract.settings.swap_retract_length, 0, 100);
    #endif
    EDIT_ITEM(float3, MSG_CONTROL_RETRACTF, &fwretract.settings.retract_feedrate_mm_s, 1, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_ZHOP, &fwretract.settings.retract_zraise, 0, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER, &fwretract.settings.retract_recover_extra, -100, 100);
    #if EXTRUDERS > 1
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER_SWAP, &fwretract.settings.swap_retract_recover_extra, -100, 100);
    #endif
    EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVERF, &fwretract.settings.retract_recover_feedrate_mm_s, 1, 999);
    #if EXTRUDERS > 1
      EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVER_SWAPF, &fwretract.settings.swap_retract_recover_feedrate_mm_s, 1, 999);
    #endif
    END_MENU();
  }

#endif

#if DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings(const uint8_t material) {
    #define _MINTEMP_ITEM(N) HEATER_##N##_MINTEMP,
    #define _MAXTEMP_ITEM(N) HEATER_##N##_MAXTEMP,
    #define MINTEMP_ALL _MIN(REPEAT(HOTENDS, _MINTEMP_ITEM) 999)
    #define MAXTEMP_ALL _MAX(REPEAT(HOTENDS, _MAXTEMP_ITEM) 0)
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    EDIT_ITEM(percent, MSG_FAN_SPEED, &ui.preheat_fan_speed[material], 0, 255);
    #if HAS_TEMP_HOTEND
      EDIT_ITEM(int3, MSG_NOZZLE, &ui.preheat_hotend_temp[material], MINTEMP_ALL, MAXTEMP_ALL - 15);
    #endif
    #if HAS_HEATED_BED
      EDIT_ITEM(int3, MSG_BED, &ui.preheat_bed_temp[material], BED_MINTEMP, BED_MAXTEMP - 10);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }

  void menu_preheat_material1_settings() { _menu_configuration_preheat_settings(0); }
  void menu_preheat_material2_settings() { _menu_configuration_preheat_settings(1); }

#endif

void menu_configuration() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  //
  // Debug Menu when certain options are enabled
  //
  #if HAS_DEBUG_MENU
    SUBMENU(MSG_DEBUG_MENU, menu_debug);
  #endif

  SUBMENU(MSG_ADVANCED_SETTINGS, menu_advanced_settings);

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    SUBMENU(MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
  #elif HAS_BED_PROBE
    EDIT_ITEM(LCD_Z_OFFSET_TYPE, MSG_ZPROBE_ZOFFSET, &probe.offset.z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
  #endif

  const bool busy = printer_busy();
  if (!busy) {
    #if EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)
      SUBMENU(MSG_DELTA_CALIBRATE, menu_delta_calibrate);
    #endif

    #if HAS_HOTEND_OFFSET
      SUBMENU(MSG_OFFSETS_MENU, menu_tool_offsets);
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      SUBMENU(MSG_IDEX_MENU, menu_idex);
    #endif

    #if ENABLED(BLTOUCH)
      SUBMENU(MSG_BLTOUCH, menu_bltouch);
    #endif

    #if ENABLED(TOUCH_MI_PROBE)
      SUBMENU(MSG_TOUCHMI_PROBE, menu_touchmi);
    #endif
  }

  //
  // Set single nozzle filament retract and prime length
  //
  #if EXTRUDERS > 1
    SUBMENU(MSG_TOOL_CHANGE, menu_tool_change);
  #endif

  //
  // Set Case light on/off/brightness
  //
  #if ENABLED(CASE_LIGHT_MENU)
    #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
      if (true
        #if DISABLED(CASE_LIGHT_USE_NEOPIXEL)
          && PWM_PIN(CASE_LIGHT_PIN)
        #endif
      )
        SUBMENU(MSG_CASE_LIGHT, menu_case_light);
      else
    #endif
        EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
  #endif

  #if HAS_LCD_CONTRAST
    EDIT_ITEM(int3, MSG_CONTRAST, &ui.contrast, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX, ui.refresh_contrast, true);
  #endif
  #if ENABLED(FWRETRACT)
    SUBMENU(MSG_RETRACT, menu_config_retract);
  #endif

  #if HAS_FILAMENT_SENSOR
    EDIT_ITEM(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    EDIT_ITEM(bool, MSG_OUTAGE_RECOVERY, &recovery.enabled, recovery.changed);
  #endif

  #if DISABLED(SLIM_LCD_MENUS)
    // Preheat configurations
    SUBMENU(MSG_PREHEAT_1_SETTINGS, menu_preheat_material1_settings);
    SUBMENU(MSG_PREHEAT_2_SETTINGS, menu_preheat_material2_settings);
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, lcd_store_settings);
    if (!busy)
      ACTION_ITEM(MSG_LOAD_EEPROM, lcd_load_settings);
  #endif

  if (!busy)
    ACTION_ITEM(MSG_RESTORE_DEFAULTS, []{
      settings.reset();
      #if HAS_BUZZER
        ui.completion_feedback();
      #endif
    });

  END_MENU();
}

#endif // HAS_LCD_MENU
