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

#if HAS_MARLINUI_MENU

#include "menu_item.h"

#include "../../MarlinCore.h"

#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

#if HAS_FANCHECK
  #include "../../feature/fancheck.h"
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

#if ENABLED(SOUND_MENU_ITEM)
  #include "../../libs/buzzer.h"
#endif

#include "../../core/debug_out.h"

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
      TERN_(HAS_MARLINUI_HD44780, ui.set_custom_characters(CHARSET_MENU));
      return;
    }
    bar_percent += (int8_t)ui.encoderPosition;
    LIMIT(bar_percent, 0, 100);
    ui.encoderPosition = 0;
    MenuItem_static::draw(0, GET_TEXT_F(MSG_PROGRESS_BAR_TEST), SS_DEFAULT|SS_INVERT);
    lcd_put_int((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2, bar_percent); lcd_put_u8str(F("%"));
    lcd_moveto(0, LCD_HEIGHT - 1); ui.draw_progress_bar(bar_percent);
  }

  void _progress_bar_test() {
    ui.goto_screen(progress_bar_test);
    TERN_(HAS_MARLINUI_HD44780, ui.set_custom_characters(CHARSET_INFO));
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

#if HAS_MULTI_EXTRUDER

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
      EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_WIPE_RETRACT, &toolchange_settings.wipe_retract, 0, 100);
      EDIT_ITEM_FAST(uint8, MSG_SINGLENOZZLE_FAN_SPEED, &toolchange_settings.fan_speed, 0, 255);
      EDIT_ITEM_FAST(uint8, MSG_SINGLENOZZLE_FAN_TIME, &toolchange_settings.fan_time, 1, 30);
    #endif
    EDIT_ITEM(float3, MSG_TOOL_CHANGE_ZLIFT, &toolchange_settings.z_raise, 0, 10);
    END_MENU();
  }

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)

    #include "../../module/motion.h" // for active_extruder
    #include "../../gcode/queue.h"

    void menu_toolchange_migration() {
      FSTR_P const msg_migrate = GET_TEXT_F(MSG_TOOL_MIGRATION_SWAP);

      START_MENU();
      BACK_ITEM(MSG_CONFIGURATION);

      // Auto mode ON/OFF
      EDIT_ITEM(bool, MSG_TOOL_MIGRATION_AUTO, &migration.automode);
      EDIT_ITEM(uint8, MSG_TOOL_MIGRATION_END, &migration.last, 0, EXTRUDERS - 1);

      // Migrate to a chosen extruder
      EXTRUDER_LOOP() {
        if (e != active_extruder) {
          ACTION_ITEM_N_F(e, msg_migrate, []{
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
      if (active_extruder && all_axes_trusted()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        active_extruder = 0;
      }
    };

    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    #if ENABLED(DUAL_X_CARRIAGE)
      EDIT_ITEM_FAST_N(float42_52, X_AXIS, MSG_HOTEND_OFFSET_A, &hotend_offset[1].x, float(X2_HOME_POS - 25), float(X2_HOME_POS + 25), _recalc_offsets);
    #else
      EDIT_ITEM_FAST_N(float42_52, X_AXIS, MSG_HOTEND_OFFSET_A, &hotend_offset[1].x, -99.0, 99.0, _recalc_offsets);
    #endif
    EDIT_ITEM_FAST_N(float42_52, Y_AXIS, MSG_HOTEND_OFFSET_A, &hotend_offset[1].y, -99.0, 99.0, _recalc_offsets);
    EDIT_ITEM_FAST_N(float42_52, Z_AXIS, MSG_HOTEND_OFFSET_A, &hotend_offset[1].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    #endif
    END_MENU();
  }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  void menu_idex() {
    const bool need_g28 = axes_should_home(_BV(Y_AXIS)|_BV(Z_AXIS));

    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);

    GCODES_ITEM(MSG_IDEX_MODE_AUTOPARK,  F("M605S1\nG28X\nG1X0"));
    GCODES_ITEM(MSG_IDEX_MODE_DUPLICATE, need_g28
      ? F("M605S1\nT0\nG28\nM605S2\nG28X\nG1X0")         // If Y or Z is not homed, do a full G28 first
      : F("M605S1\nT0\nM605S2\nG28X\nG1X0")
    );
    GCODES_ITEM(MSG_IDEX_MODE_MIRRORED_COPY, need_g28
      ? F("M605S1\nT0\nG28\nM605S2\nG28X\nG1X0\nM605S3") // If Y or Z is not homed, do a full G28 first
      : F("M605S1\nT0\nM605S2\nG28 X\nG1X0\nM605S3")
    );
    GCODES_ITEM(MSG_IDEX_MODE_FULL_CTRL, F("M605S0\nG28X"));

    EDIT_ITEM(float42_52, MSG_IDEX_DUPE_GAP, &duplicate_extruder_x_offset, (X2_MIN_POS) - (X1_MIN_POS), (X_BED_SIZE) - 20);

    END_MENU();
  }

#endif

#if ENABLED(BLTOUCH)

  #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
    void bltouch_report() {
      PGMSTR(mode0, "OD");
      PGMSTR(mode1, "5V");
      DEBUG_ECHOPGM("BLTouch Mode: ");
      DEBUG_ECHOPGM_P(bltouch.od_5v_mode ? mode1 : mode0);
      DEBUG_ECHOLNPGM(" (Default " TERN(BLTOUCH_SET_5V_MODE, "5V", "OD") ")");
      char mess[21];
      strcpy_P(mess, PSTR("BLTouch Mode: "));
      strcpy_P(&mess[15], bltouch.od_5v_mode ? mode1 : mode0);
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
    #ifdef BLTOUCH_HS_MODE
      EDIT_ITEM(bool, MSG_BLTOUCH_SPEED_MODE, &bltouch.high_speed_mode);
    #endif
    #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
      CONFIRM_ITEM(MSG_BLTOUCH_5V_MODE, MSG_BLTOUCH_5V_MODE, MSG_BUTTON_CANCEL, bltouch._set_5V_mode, nullptr, GET_TEXT_F(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_OD_MODE, MSG_BLTOUCH_OD_MODE, MSG_BUTTON_CANCEL, bltouch._set_OD_mode, nullptr, GET_TEXT_F(MSG_BLTOUCH_MODE_CHANGE));
      ACTION_ITEM(MSG_BLTOUCH_MODE_STORE, bltouch._mode_store);
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_5V, MSG_BLTOUCH_MODE_STORE_5V, MSG_BUTTON_CANCEL, bltouch.mode_conv_5V, nullptr, GET_TEXT_F(MSG_BLTOUCH_MODE_CHANGE));
      CONFIRM_ITEM(MSG_BLTOUCH_MODE_STORE_OD, MSG_BLTOUCH_MODE_STORE_OD, MSG_BUTTON_CANCEL, bltouch.mode_conv_OD, nullptr, GET_TEXT_F(MSG_BLTOUCH_MODE_CHANGE));
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
    GCODES_ITEM(MSG_TOUCHMI_INIT, F("M851 Z0\nG28\nG1 F200 Z0"));
    SUBMENU(MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    GCODES_ITEM(MSG_TOUCHMI_SAVE, F("M500\nG1 F200 Z10"));
    GCODES_ITEM(MSG_TOUCHMI_ZTEST, F("G28\nG1 F200 Z0"));
    END_MENU();
  }

#endif

#if ENABLED(CONTROLLER_FAN_MENU)

  #include "../../feature/controllerfan.h"

  void menu_controller_fan() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    EDIT_ITEM_FAST(percent, MSG_CONTROLLER_FAN_IDLE_SPEED, &controllerFan.settings.idle_speed, CONTROLLERFAN_SPEED_MIN, 255);
    EDIT_ITEM(bool, MSG_CONTROLLER_FAN_AUTO_ON, &controllerFan.settings.auto_mode);
    if (controllerFan.settings.auto_mode) {
      EDIT_ITEM_FAST(percent, MSG_CONTROLLER_FAN_SPEED, &controllerFan.settings.active_speed, CONTROLLERFAN_SPEED_MIN, 255);
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
    #if HAS_MULTI_EXTRUDER
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_SWAP, &fwretract.settings.swap_retract_length, 0, 100);
    #endif
    EDIT_ITEM(float3, MSG_CONTROL_RETRACTF, &fwretract.settings.retract_feedrate_mm_s, 1, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_ZHOP, &fwretract.settings.retract_zraise, 0, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER, &fwretract.settings.retract_recover_extra, -100, 100);
    #if HAS_MULTI_EXTRUDER
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER_SWAP, &fwretract.settings.swap_retract_recover_extra, -100, 100);
    #endif
    EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVERF, &fwretract.settings.retract_recover_feedrate_mm_s, 1, 999);
    #if HAS_MULTI_EXTRUDER
      EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVER_SWAPF, &fwretract.settings.swap_retract_recover_feedrate_mm_s, 1, 999);
    #endif
    END_MENU();
  }

#endif

#if HAS_PREHEAT && DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings() {
    #define _MINTEMP_ITEM(N) HEATER_##N##_MINTEMP,
    #define _MAXTEMP_ITEM(N) HEATER_##N##_MAXTEMP,
    #define MINTEMP_ALL _MIN(REPEAT(HOTENDS, _MINTEMP_ITEM) 999)
    #define MAXTEMP_ALL _MAX(REPEAT(HOTENDS, _MAXTEMP_ITEM) 0)
    const uint8_t m = MenuItemBase::itemIndex;
    START_MENU();
    STATIC_ITEM_F(ui.get_preheat_label(m), SS_DEFAULT|SS_INVERT);
    BACK_ITEM(MSG_CONFIGURATION);
    #if HAS_FAN
      editable.uint8 = uint8_t(ui.material_preset[m].fan_speed);
      EDIT_ITEM_N(percent, m, MSG_FAN_SPEED, &editable.uint8, 0, 255, []{ ui.material_preset[MenuItemBase::itemIndex].fan_speed = editable.uint8; });
    #endif
    #if HAS_TEMP_HOTEND
      EDIT_ITEM(int3, MSG_NOZZLE, &ui.material_preset[m].hotend_temp, MINTEMP_ALL, MAXTEMP_ALL - (HOTEND_OVERSHOOT));
    #endif
    #if HAS_HEATED_BED
      EDIT_ITEM(int3, MSG_BED, &ui.material_preset[m].bed_temp, BED_MINTEMP, BED_MAX_TARGET);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(CUSTOM_MENU_CONFIG)

  void _lcd_custom_menus_configuration_gcode(FSTR_P const fstr) {
    queue.inject(fstr);
    TERN_(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
    TERN_(CUSTOM_MENU_CONFIG_SCRIPT_RETURN, ui.return_to_status());
  }

  void custom_menus_configuration() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);

    #define HAS_CUSTOM_ITEM_CONF(N) (defined(CONFIG_MENU_ITEM_##N##_DESC) && defined(CONFIG_MENU_ITEM_##N##_GCODE))

    #ifdef CUSTOM_MENU_CONFIG_SCRIPT_DONE
      #define _DONE_SCRIPT "\n" CUSTOM_MENU_CONFIG_SCRIPT_DONE
    #else
      #define _DONE_SCRIPT ""
    #endif
    #define GCODE_LAMBDA_CONF(N) []{ _lcd_custom_menus_configuration_gcode(F(CONFIG_MENU_ITEM_##N##_GCODE _DONE_SCRIPT)); }
    #define _CUSTOM_ITEM_CONF(N) ACTION_ITEM_F(F(CONFIG_MENU_ITEM_##N##_DESC), GCODE_LAMBDA_CONF(N));
    #define _CUSTOM_ITEM_CONF_CONFIRM(N)            \
      SUBMENU_F(F(CONFIG_MENU_ITEM_##N##_DESC), []{ \
          MenuItem_confirm::confirm_screen(         \
            GCODE_LAMBDA_CONF(N), nullptr,          \
            F(CONFIG_MENU_ITEM_##N##_DESC "?")      \
          );                                        \
        })

    #define CUSTOM_ITEM_CONF(N) do{ \
      constexpr char c = CONFIG_MENU_ITEM_##N##_GCODE[strlen(CONFIG_MENU_ITEM_##N##_GCODE) - 1]; \
      static_assert(c != '\n' && c != '\r', "CONFIG_MENU_ITEM_" STRINGIFY(N) "_GCODE cannot have a newline at the end. Please remove it."); \
      if (ENABLED(CONFIG_MENU_ITEM_##N##_CONFIRM)) \
        _CUSTOM_ITEM_CONF_CONFIRM(N); \
      else \
        _CUSTOM_ITEM_CONF(N); \
    }while(0)

    #if HAS_CUSTOM_ITEM_CONF(1)
      CUSTOM_ITEM_CONF(1);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(2)
      CUSTOM_ITEM_CONF(2);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(3)
      CUSTOM_ITEM_CONF(3);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(4)
      CUSTOM_ITEM_CONF(4);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(5)
      CUSTOM_ITEM_CONF(5);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(6)
      CUSTOM_ITEM_CONF(6);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(7)
      CUSTOM_ITEM_CONF(7);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(8)
      CUSTOM_ITEM_CONF(8);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(9)
      CUSTOM_ITEM_CONF(9);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(10)
      CUSTOM_ITEM_CONF(10);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(11)
      CUSTOM_ITEM_CONF(11);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(12)
      CUSTOM_ITEM_CONF(12);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(13)
      CUSTOM_ITEM_CONF(13);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(14)
      CUSTOM_ITEM_CONF(14);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(15)
      CUSTOM_ITEM_CONF(15);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(16)
      CUSTOM_ITEM_CONF(16);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(17)
      CUSTOM_ITEM_CONF(17);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(18)
      CUSTOM_ITEM_CONF(18);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(19)
      CUSTOM_ITEM_CONF(19);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(20)
      CUSTOM_ITEM_CONF(20);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(21)
      CUSTOM_ITEM_CONF(21);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(22)
      CUSTOM_ITEM_CONF(22);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(23)
      CUSTOM_ITEM_CONF(23);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(24)
      CUSTOM_ITEM_CONF(24);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(25)
      CUSTOM_ITEM_CONF(25);
    #endif
    END_MENU();
  }

#endif // CUSTOM_MENU_CONFIG

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

  #if ENABLED(CUSTOM_MENU_CONFIG)
    if (TERN1(CUSTOM_MENU_CONFIG_ONLY_IDLE, !busy)) {
      #ifdef CUSTOM_MENU_CONFIG_TITLE
        SUBMENU_F(F(CUSTOM_MENU_CONFIG_TITLE), custom_menus_configuration);
      #else
        SUBMENU(MSG_CUSTOM_COMMANDS, custom_menus_configuration);
      #endif
    }
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
  #if HAS_MULTI_EXTRUDER
    SUBMENU(MSG_TOOL_CHANGE, menu_tool_change);
    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      SUBMENU(MSG_TOOL_MIGRATION, menu_toolchange_migration);
    #endif
  #endif

  #if HAS_LCD_BRIGHTNESS
    EDIT_ITEM_FAST(uint8, MSG_BRIGHTNESS, &ui.brightness, LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.refresh_brightness, true);
  #endif
  #if HAS_LCD_CONTRAST && LCD_CONTRAST_MIN < LCD_CONTRAST_MAX
    EDIT_ITEM_FAST(uint8, MSG_CONTRAST, &ui.contrast, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX, ui.refresh_contrast, true);
  #endif

  //
  // Set display backlight / sleep timeout
  //
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    EDIT_ITEM(uint8, MSG_SCREEN_TIMEOUT, &ui.backlight_timeout_minutes, ui.backlight_timeout_min, ui.backlight_timeout_max, ui.refresh_backlight_timeout);
  #elif HAS_DISPLAY_SLEEP
    EDIT_ITEM(uint8, MSG_SCREEN_TIMEOUT, &ui.sleep_timeout_minutes, ui.sleep_timeout_min, ui.sleep_timeout_max, ui.refresh_screen_timeout);
  #endif

  #if ENABLED(FWRETRACT)
    SUBMENU(MSG_RETRACT, menu_config_retract);
  #endif

  #if HAS_FILAMENT_SENSOR
    EDIT_ITEM(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
  #endif

  #if HAS_FANCHECK
    EDIT_ITEM(bool, MSG_FANCHECK, &fan_check.enabled);
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    EDIT_ITEM(bool, MSG_OUTAGE_RECOVERY, &recovery.enabled, recovery.changed);
  #endif

  // Preheat configurations
  #if HAS_PREHEAT && DISABLED(SLIM_LCD_MENUS)
    LOOP_L_N(m, PREHEAT_COUNT)
      SUBMENU_N_f(m, ui.get_preheat_label(m), MSG_PREHEAT_M_SETTINGS, _menu_configuration_preheat_settings);
  #endif

  #if ENABLED(SOUND_MENU_ITEM)
    EDIT_ITEM(bool, MSG_SOUND, &ui.sound_on, []{ ui.chirp(); });
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    if (!busy) ACTION_ITEM(MSG_LOAD_EEPROM, ui.load_settings);
  #endif

  if (!busy) ACTION_ITEM(MSG_RESTORE_DEFAULTS, ui.reset_settings);

  END_MENU();
}

#endif // HAS_MARLINUI_MENU
