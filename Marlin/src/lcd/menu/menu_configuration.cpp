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
// Configuration Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu_item.h"

#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
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
    MenuItem_static::draw(0, GET_TEXT(MSG_PROGRESS_BAR_TEST), SS_DEFAULT|SS_INVERT);
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
      static constexpr float max_extrude = TERN(PREVENT_LENGTHY_EXTRUDE, EXTRUDE_MAXLENGTH, 500);
      #if ENABLED(TOOLCHANGE_PARK)
        EDIT_ITEM(bool, MSG_FILAMENT_PARK_ENABLED, &toolchange_settings.enable_park);
      #endif
      EDIT_ITEM(float3, MSG_FILAMENT_SWAP_LENGTH, &toolchange_settings.swap_length, 0, max_extrude);
      EDIT_ITEM(float41sign, MSG_FILAMENT_SWAP_EXTRA, &toolchange_settings.extra_resume, -10, 10);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_RETRACT_SPEED, &toolchange_settings.retract_speed, 10, 5400);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_UNRETRACT_SPEED, &toolchange_settings.unretract_speed, 10, 5400);
      EDIT_ITEM(float3, MSG_FILAMENT_PURGE_LENGTH, &toolchange_settings.extra_prime, 0, max_extrude);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_PRIME_SPEED, &toolchange_settings.prime_speed, 10, 5400);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_FAN_SPEED, &toolchange_settings.fan_speed, 0, 255);
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_FAN_TIME, &toolchange_settings.fan_time, 1, 30);
    #endif
    EDIT_ITEM(float3, MSG_TOOL_CHANGE_ZLIFT, &toolchange_settings.z_raise, 0, 10);
    END_MENU();
  }

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)

    #include "../../module/motion.h" // for active_extruder

    void menu_toolchange_migration() {
      PGM_P const msg_migrate = GET_TEXT(MSG_TOOL_MIGRATION_SWAP);

      START_MENU();
      BACK_ITEM(MSG_CONFIGURATION);

      // Auto mode ON/OFF
      EDIT_ITEM(bool, MSG_TOOL_MIGRATION_AUTO, &migration.automode);
      EDIT_ITEM(uint8, MSG_TOOL_MIGRATION_END, &migration.last, 0, EXTRUDERS - 1);

      // Migrate to a chosen extruder
      LOOP_L_N(s, EXTRUDERS) {
        if (s != active_extruder) {
          ACTION_ITEM_N_P(s, msg_migrate, []{
            char cmd[12];
            sprintf_P(cmd, PSTR("M217 T%i"), int(MenuItemBase::itemIndex));
            queue.inject(cmd);
          });
        }
      }
      END_MENU();
    }
  #endif

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
      EDIT_ITEM_FAST(float42_52, MSG_HOTEND_OFFSET_X, &hotend_offset[1].x, float(X2_HOME_POS - 25), float(X2_HOME_POS + 25), _recalc_offsets);
    #else
      EDIT_ITEM_FAST(float42_52, MSG_HOTEND_OFFSET_X, &hotend_offset[1].x, -99.0, 99.0, _recalc_offsets);
    #endif
    EDIT_ITEM_FAST(float42_52, MSG_HOTEND_OFFSET_Y, &hotend_offset[1].y, -99.0, 99.0, _recalc_offsets);
    EDIT_ITEM_FAST(float42_52, MSG_HOTEND_OFFSET_Z, &hotend_offset[1].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    #endif
    END_MENU();
  }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  void menu_idex() {
    const bool need_g28 = !(TEST(axis_known_position, Y_AXIS) && TEST(axis_known_position, Z_AXIS));

    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);

    GCODES_ITEM(MSG_IDEX_MODE_AUTOPARK,  PSTR("M605 S1\nG28 X\nG1 X100"));
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
      SERIAL_ECHOLNPGM("Configuration BLTouch Mode - " TERN(BLTOUCH_SET_5V_MODE, "5V", "OD"));
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
      CONFIRM_ITEM(MSG_BLTOUCH_5V_MODE, MSG_BLTOUCH_5V_MODE, MSG_BUTTON_CANCEL, bltouch._set_5V_mode, nullptr, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_OD_MODE, MSG_BLTOUCH_OD_MODE, MSG_BUTTON_CANCEL, bltouch._set_OD_mode, nullptr, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      ACTION_ITEM(MSG_BLTOUCH_MODE_STORE, bltouch._mode_store);
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_5V, MSG_BLTOUCH_MODE_STORE_5V, MSG_BUTTON_CANCEL, bltouch.mode_conv_5V, nullptr, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_OD, MSG_BLTOUCH_MODE_STORE_OD, MSG_BUTTON_CANCEL, bltouch.mode_conv_OD, nullptr, GET_TEXT(MSG_BLTOUCH_MODE_CHANGE));
      ACTION_ITEM(MSG_BLTOUCH_MODE_ECHO, bltouch_report);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(TOUCH_MI_PROBE)

  void menu_touchmi() {
    ui.defer_status_screen();
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    GCODES_ITEM(MSG_TOUCHMI_INIT, PSTR("M851 Z0\nG28\nG1 F200 Z0"));
    SUBMENU(MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    GCODES_ITEM(MSG_TOUCHMI_SAVE, PSTR("M500\nG1 F200 Z10"));
    GCODES_ITEM(MSG_TOUCHMI_ZTEST, PSTR("G28\nG1 F200 Z0"));
    END_MENU();
  }

#endif

#if ENABLED(CONTROLLER_FAN_MENU)

  #include "../../feature/controllerfan.h"

  void menu_controller_fan() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    EDIT_ITEM_FAST(percent, MSG_CONTROLLER_FAN_IDLE_SPEED, &controllerFan.settings.idle_speed, _MAX(1, CONTROLLERFAN_SPEED_MIN) - 1, 255);
    EDIT_ITEM(bool, MSG_CONTROLLER_FAN_AUTO_ON, &controllerFan.settings.auto_mode);
    if (controllerFan.settings.auto_mode) {
      EDIT_ITEM_FAST(percent, MSG_CONTROLLER_FAN_SPEED, &controllerFan.settings.active_speed, _MAX(1, CONTROLLERFAN_SPEED_MIN) - 1, 255);
      EDIT_ITEM(uint16_4, MSG_CONTROLLER_FAN_DURATION, &controllerFan.settings.duration, 0, 4800);
    }
    END_MENU();
  }

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

#if PREHEAT_COUNT && DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings() {
    #define _MINTEMP_ITEM(N) HEATER_##N##_MINTEMP,
    #define _MAXTEMP_ITEM(N) HEATER_##N##_MAXTEMP,
    #define MINTEMP_ALL _MIN(REPEAT(HOTENDS, _MINTEMP_ITEM) 999)
    #define MAXTEMP_ALL _MAX(REPEAT(HOTENDS, _MAXTEMP_ITEM) 0)
    const uint8_t m = MenuItemBase::itemIndex;
    START_MENU();
    STATIC_ITEM_P(ui.get_preheat_label(m), SS_DEFAULT|SS_INVERT);
    BACK_ITEM(MSG_CONFIGURATION);
    #if HAS_FAN
      editable.uint8 = uint8_t(ui.material_preset[m].fan_speed);
      EDIT_ITEM_N(percent, m, MSG_FAN_SPEED, &editable.uint8, 0, 255, []{ ui.material_preset[MenuItemBase::itemIndex].fan_speed = editable.uint8; });
    #endif
    #if HAS_TEMP_HOTEND
      EDIT_ITEM(uint16_3, MSG_NOZZLE, &ui.material_preset[m].hotend_temp, MINTEMP_ALL, MAXTEMP_ALL - HOTEND_OVERSHOOT);
    #endif
    #if HAS_HEATED_BED
      EDIT_ITEM(uint16_3, MSG_BED, &ui.material_preset[m].bed_temp, BED_MINTEMP, BED_MAX_TARGET);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    #endif
    END_MENU();
  }

#endif

void menu_configuration() {
  const bool busy = printer_busy();

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

  //
  // Set Fan Controller speed
  //
  #if ENABLED(CONTROLLER_FAN_MENU)
    SUBMENU(MSG_CONTROLLER_FAN, menu_controller_fan);
  #endif

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
    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      SUBMENU(MSG_TOOL_MIGRATION, menu_toolchange_migration);
    #endif
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

  // Preheat configurations
  #if PREHEAT_COUNT && DISABLED(SLIM_LCD_MENUS)
    LOOP_L_N(m, PREHEAT_COUNT)
      SUBMENU_N_S(m, ui.get_preheat_label(m), MSG_PREHEAT_M_SETTINGS, _menu_configuration_preheat_settings);
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    if (!busy) ACTION_ITEM(MSG_LOAD_EEPROM, ui.load_settings);
  #endif

  if (!busy) ACTION_ITEM(MSG_RESTORE_DEFAULTS, ui.reset_settings);

  END_MENU();
}

#endif // HAS_LCD_MENU
