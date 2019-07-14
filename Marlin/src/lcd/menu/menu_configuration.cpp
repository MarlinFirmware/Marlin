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
void menu_delta_calibrate();

static void lcd_factory_settings() {
  settings.reset();
  ui.completion_feedback();
}

#if ENABLED(LCD_PROGRESS_BAR_TEST)

  #include "../lcdprint.h"

  static void progress_bar_test() {
    ui.encoder_direction_normal();
    static int8_t bar_percent = 0;
    if (ui.use_click()) {
      ui.goto_previous_screen();
      ui.set_custom_characters(CHARSET_MENU);
      return;
    }
    bar_percent += (int8_t)ui.encoderPosition;
    LIMIT(bar_percent, 0, 100);
    ui.encoderPosition = 0;
    draw_menu_item_static(0, PSTR(MSG_PROGRESS_BAR_TEST), true, true);
    lcd_moveto((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2);
    lcd_put_int(bar_percent); lcd_put_wchar('%');
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

    MENU_BACK(MSG_CONFIGURATION);

    #if ENABLED(LCD_PROGRESS_BAR_TEST)
      MENU_ITEM(submenu, MSG_PROGRESS_BAR_TEST, _progress_bar_test);
    #endif

    END_MENU();
  }

#endif

#if EXTRUDERS > 1

  #include "../../module/tool_change.h"

  void menu_tool_change() {
    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      static constexpr float max_extrude =
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          EXTRUDE_MAXLENGTH
        #else
          500
        #endif
      ;
      MENU_ITEM_EDIT(float3, MSG_FILAMENT_SWAP_LENGTH, &toolchange_settings.swap_length, 0, max_extrude);
      MENU_ITEM_EDIT(float3, MSG_FILAMENT_PURGE_LENGTH, &toolchange_settings.extra_prime, 0, max_extrude);
      MENU_MULTIPLIER_ITEM_EDIT(int4, MSG_SINGLENOZZLE_RETRACT_SPD, &toolchange_settings.retract_speed, 10, 5400);
      MENU_MULTIPLIER_ITEM_EDIT(int4, MSG_SINGLENOZZLE_PRIME_SPD, &toolchange_settings.prime_speed, 10, 5400);
    #endif
    MENU_ITEM_EDIT(float3, MSG_TOOL_CHANGE_ZLIFT, &toolchange_settings.z_raise, 0, 10);
    END_MENU();
  }

#endif

#if HAS_HOTEND_OFFSET
  #include "../../module/motion.h"
  #include "../../gcode/queue.h"

  void menu_tool_offsets() {

    auto _recalc_offsets = []{
      if (active_extruder && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(PSTR("G28")); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        active_extruder = 0;
      }
    };

    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);
    #if ENABLED(DUAL_X_CARRIAGE)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float51, MSG_X_OFFSET, &hotend_offset[X_AXIS][1], float(X2_HOME_POS - 25), float(X2_HOME_POS + 25), _recalc_offsets);
    #else
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52sign, MSG_X_OFFSET, &hotend_offset[X_AXIS][1], -10.0, 10.0, _recalc_offsets);
    #endif
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52sign, MSG_Y_OFFSET, &hotend_offset[Y_AXIS][1], -10.0, 10.0, _recalc_offsets);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52sign, MSG_Z_OFFSET, &hotend_offset[Z_AXIS][1], Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  void menu_idex() {
    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);

    MENU_ITEM(gcode, MSG_IDEX_MODE_AUTOPARK,  PSTR("M605 S1\nG28 X\nG1 X100"));
    const bool need_g28 = !(TEST(axis_known_position, Y_AXIS) && TEST(axis_known_position, Z_AXIS));
    MENU_ITEM(gcode, MSG_IDEX_MODE_DUPLICATE, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100")                // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100")
    );
    MENU_ITEM(gcode, MSG_IDEX_MODE_MIRRORED_COPY, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")  // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")
    );
    MENU_ITEM(gcode, MSG_IDEX_MODE_FULL_CTRL, PSTR("M605 S0\nG28 X"));
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
    MENU_BACK(MSG_CONFIGURATION);
    MENU_ITEM(function, MSG_BLTOUCH_RESET, bltouch._reset);
    MENU_ITEM(function, MSG_BLTOUCH_SELFTEST, bltouch._selftest);
    MENU_ITEM(function, MSG_BLTOUCH_DEPLOY, bltouch._deploy);
    MENU_ITEM(function, MSG_BLTOUCH_STOW, bltouch._stow);
    MENU_ITEM(function, MSG_BLTOUCH_SW_MODE, bltouch._set_SW_mode);
    #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
      MENU_ITEM(submenu, MSG_BLTOUCH_5V_MODE, []{
        do_select_screen(PSTR(MSG_BLTOUCH_5V_MODE), PSTR(MSG_BUTTON_CANCEL), bltouch._set_5V_mode, ui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      MENU_ITEM(submenu, MSG_BLTOUCH_OD_MODE, []{
        do_select_screen(PSTR(MSG_BLTOUCH_OD_MODE), PSTR(MSG_BUTTON_CANCEL), bltouch._set_OD_mode, ui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      MENU_ITEM(function, MSG_BLTOUCH_MODE_STORE, bltouch._mode_store);
      MENU_ITEM(submenu, MSG_BLTOUCH_MODE_STORE_5V, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_STORE_5V), PSTR(MSG_BUTTON_CANCEL), bltouch.mode_conv_5V, ui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      MENU_ITEM(submenu, MSG_BLTOUCH_MODE_STORE_OD, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_STORE_OD), PSTR(MSG_BUTTON_CANCEL), bltouch.mode_conv_OD, ui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      MENU_ITEM(function, MSG_BLTOUCH_MODE_ECHO, bltouch_report);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(TOUCH_MI_PROBE)
  void menu_touchmi() {
    START_MENU();
    ui.defer_status_screen();
    MENU_BACK(MSG_CONFIGURATION);
    MENU_ITEM(gcode, MSG_TOUCHMI_INIT, PSTR("M851 Z0\nG28\nG1 F200 Z0"));
    MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    MENU_ITEM(gcode, MSG_TOUCHMI_SAVE, PSTR("M500\nG1 F200 Z10"));
    MENU_ITEM(gcode, MSG_TOUCHMI_ZTEST, PSTR("G28\nG1 F200 Z0"));
    END_MENU();
  }
#endif

#if ENABLED(CASE_LIGHT_MENU)

  #include "../../feature/caselight.h"

  #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)

    void menu_case_light() {
      START_MENU();
      MENU_BACK(MSG_CONFIGURATION);
      MENU_ITEM_EDIT_CALLBACK(uint8, MSG_CASE_LIGHT_BRIGHTNESS, &case_light_brightness, 0, 255, update_case_light, true);
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
      END_MENU();
    }

  #endif

#endif

#if ENABLED(FWRETRACT)

  #include "../../feature/fwretract.h"

  void menu_config_retract() {
    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);
    #if ENABLED(FWRETRACT_AUTORETRACT)
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_AUTORETRACT, &fwretract.autoretract_enabled, fwretract.refresh_autoretract);
    #endif
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT, &fwretract.settings.retract_length, 0, 100);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_SWAP, &fwretract.settings.swap_retract_length, 0, 100);
    #endif
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACTF, &fwretract.settings.retract_feedrate_mm_s, 1, 999);
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_ZHOP, &fwretract.settings.retract_zraise, 0, 999);
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_RECOVER, &fwretract.settings.retract_recover_extra, -100, 100);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_RECOVER_SWAP, &fwretract.settings.swap_retract_recover_extra, -100, 100);
    #endif
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVERF, &fwretract.settings.retract_recover_feedrate_mm_s, 1, 999);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVER_SWAPF, &fwretract.settings.swap_retract_recover_feedrate_mm_s, 1, 999);
    #endif
    END_MENU();
  }

#endif

#if DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings(const uint8_t material) {
    #if HOTENDS > 5
      #define MINTEMP_ALL _MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP, HEATER_5_MINTEMP)
      #define MAXTEMP_ALL _MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP)
    #elif HOTENDS > 4
      #define MINTEMP_ALL _MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP)
      #define MAXTEMP_ALL _MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP)
    #elif HOTENDS > 3
      #define MINTEMP_ALL _MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP)
      #define MAXTEMP_ALL _MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP)
    #elif HOTENDS > 2
      #define MINTEMP_ALL _MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP)
      #define MAXTEMP_ALL _MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP)
    #elif HOTENDS > 1
      #define MINTEMP_ALL _MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP)
      #define MAXTEMP_ALL _MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP)
    #else
      #define MINTEMP_ALL HEATER_0_MINTEMP
      #define MAXTEMP_ALL HEATER_0_MAXTEMP
    #endif
    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);
    MENU_ITEM_EDIT(percent, MSG_FAN_SPEED, &ui.preheat_fan_speed[material], 0, 255);
    #if HAS_TEMP_HOTEND
      MENU_ITEM_EDIT(int3, MSG_NOZZLE, &ui.preheat_hotend_temp[material], MINTEMP_ALL, MAXTEMP_ALL - 15);
    #endif
    #if HAS_HEATED_BED
      MENU_ITEM_EDIT(int3, MSG_BED, &ui.preheat_bed_temp[material], BED_MINTEMP, BED_MAXTEMP - 10);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }

  void menu_preheat_material1_settings() { _menu_configuration_preheat_settings(0); }
  void menu_preheat_material2_settings() { _menu_configuration_preheat_settings(1); }

#endif

void menu_configuration() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  //
  // Debug Menu when certain options are enabled
  //
  #if HAS_DEBUG_MENU
    MENU_ITEM(submenu, MSG_DEBUG_MENU, menu_debug);
  #endif

  MENU_ITEM(submenu, MSG_ADVANCED_SETTINGS, menu_advanced_settings);

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
  #elif HAS_BED_PROBE
    MENU_ITEM_EDIT(float52, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
  #endif

  const bool busy = printer_busy();
  if (!busy) {
    //
    // Delta Calibration
    //
    #if EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE, menu_delta_calibrate);
    #endif

    #if HAS_HOTEND_OFFSET
      MENU_ITEM(submenu, MSG_OFFSETS_MENU, menu_tool_offsets);
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      MENU_ITEM(submenu, MSG_IDEX_MENU, menu_idex);
    #endif

    #if ENABLED(BLTOUCH)
      MENU_ITEM(submenu, MSG_BLTOUCH, menu_bltouch);
    #endif

    #if ENABLED(TOUCH_MI_PROBE)
      MENU_ITEM(submenu, MSG_TOUCHMI_PROBE, menu_touchmi);
    #endif
  }

  //
  // Set single nozzle filament retract and prime length
  //
  #if EXTRUDERS > 1
    MENU_ITEM(submenu, MSG_TOOL_CHANGE, menu_tool_change);
  #endif

  //
  // Set Case light on/off/brightness
  //
  #if ENABLED(CASE_LIGHT_MENU)
    #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
      if (PWM_PIN(CASE_LIGHT_PIN))
        MENU_ITEM(submenu, MSG_CASE_LIGHT, menu_case_light);
      else
    #endif
        MENU_ITEM_EDIT_CALLBACK(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
  #endif

  #if HAS_LCD_CONTRAST
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_CONTRAST, &ui.contrast, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX, ui.refresh_contrast, true);
  #endif
  #if ENABLED(FWRETRACT)
    MENU_ITEM(submenu, MSG_RETRACT, menu_config_retract);
  #endif

  #if HAS_FILAMENT_SENSOR
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_OUTAGE_RECOVERY, &recovery.enabled, recovery.changed);
  #endif

  #if DISABLED(SLIM_LCD_MENUS)
    // Preheat configurations
    MENU_ITEM(submenu, MSG_PREHEAT_1_SETTINGS, menu_preheat_material1_settings);
    MENU_ITEM(submenu, MSG_PREHEAT_2_SETTINGS, menu_preheat_material2_settings);
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
    if (!busy)
      MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
  #endif

  if (!busy)
    MENU_ITEM(function, MSG_RESTORE_FAILSAFE, lcd_factory_settings);

  END_MENU();
}

#endif // HAS_LCD_MENU
