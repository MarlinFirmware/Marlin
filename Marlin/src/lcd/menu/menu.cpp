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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(ULTIPANEL)

#include "menu.h"

#include "../ultralcd.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../module/printcounter.h"
#include "../../gcode/gcode.h"
#include "../../gcode/queue.h"
#include "../../module/configuration_store.h"
#include "../../module/tool_change.h"
#include "../../Marlin.h"

#include <stdarg.h>

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../feature/pause.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/power_loss_recovery.h"
#endif

#if ENABLED(PRINTCOUNTER) && ENABLED(LCD_INFO_MENU)
  #include "../../libs/duration_t.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(LED_CONTROL_MENU)
  #include "../../feature/leds/leds.h"
#endif

////////////////////////////////////////////
///////////// Global Variables /////////////
////////////////////////////////////////////

// Buttons
volatile uint8_t buttons;
#if ENABLED(REPRAPWORLD_KEYPAD)
  volatile uint8_t buttons_reprapworld_keypad;
#endif

// Menu Navigation
int8_t encoderTopLine;
typedef struct {
  screenFunc_t menu_function;
  uint32_t encoder_position;
} menuPosition;
menuPosition screen_history[6];
uint8_t screen_history_depth = 0;
bool screen_changed, defer_return_to_status;

// Value Editing
PGM_P editLabel;
void *editValue;
int32_t minEditValue, maxEditValue;
screenFunc_t callbackFunc;
bool liveEdit;

bool no_reentry = false;

// Initialized by settings.load()
int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2];
uint8_t lcd_preheat_fan_speed[2];

////////////////////////////////////////////
//////// Menu Navigation & History /////////
////////////////////////////////////////////

void lcd_status_screen();

void lcd_return_to_status() { lcd_goto_screen(lcd_status_screen); }

void lcd_save_previous_screen() {
  if (screen_history_depth < COUNT(screen_history)) {
    screen_history[screen_history_depth].menu_function = currentScreen;
    screen_history[screen_history_depth].encoder_position = encoderPosition;
    ++screen_history_depth;
  }
}

void lcd_goto_previous_menu() {
  if (screen_history_depth > 0) {
    --screen_history_depth;
    lcd_goto_screen(
      screen_history[screen_history_depth].menu_function,
      screen_history[screen_history_depth].encoder_position
    );
  }
  else
    lcd_return_to_status();
}

void lcd_goto_previous_menu_no_defer() {
  defer_return_to_status = false;
  lcd_goto_previous_menu();
}

////////////////////////////////////////////
/////////////// Menu Actions ///////////////
////////////////////////////////////////////

void _menu_action_back() { lcd_goto_previous_menu(); }
void menu_action_submenu(screenFunc_t func) { lcd_save_previous_screen(); lcd_goto_screen(func); }
void menu_action_gcode(PGM_P pgcode) { enqueue_and_echo_commands_P(pgcode); }
void menu_action_function(screenFunc_t func) { (*func)(); }

#if ENABLED(SDSUPPORT)

  void menu_action_sdfile(CardReader &theCard) {
    #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
      last_sdfile_encoderPosition = encoderPosition;  // Save which file was selected for later use
    #endif
    card.openAndPrintFile(theCard.filename);
    lcd_return_to_status();
    lcd_reset_status();
  }

  void menu_action_sddirectory(CardReader &theCard) {
    card.chdir(theCard.filename);
    encoderTopLine = 0;
    encoderPosition = 2 * ENCODER_STEPS_PER_MENU_ITEM;
    screen_changed = true;
    #if HAS_GRAPHICAL_LCD
      drawing_screen = false;
    #endif
    lcd_refresh();
  }

#endif // SDSUPPORT

////////////////////////////////////////////
/////////// Menu Editing Actions ///////////
////////////////////////////////////////////

/**
 * Functions for editing single values
 *
 * The "DEFINE_MENU_EDIT_TYPE" macro generates the functions needed to edit a numerical value.
 *
 * For example, DEFINE_MENU_EDIT_TYPE(int16_t, int3, itostr3, 1) expands into these functions:
 *
 *   bool _menu_edit_int3();
 *   void menu_edit_int3(); // edit int16_t (interactively)
 *   void menu_edit_callback_int3(); // edit int16_t (interactively) with callback on completion
 *   void _menu_action_setting_edit_int3(PGM_P const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue);
 *   void menu_action_setting_edit_int3(PGM_P const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue);
 *   void menu_action_setting_edit_callback_int3(PGM_P const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue, const screenFunc_t callback, const bool live); // edit int16_t with callback
 *
 * You can then use one of the menu macros to present the edit interface:
 *   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
 *
 * This expands into a more primitive menu item:
 *   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
 *
 * ...which calls:
 *       menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
 */
#define DEFINE_MENU_EDIT_TYPE(TYPE, NAME, STRFUNC, SCALE) \
  bool _menu_edit_ ## NAME() { \
    ENCODER_DIRECTION_NORMAL(); \
    if ((int32_t)encoderPosition < 0) encoderPosition = 0; \
    if ((int32_t)encoderPosition > maxEditValue) encoderPosition = maxEditValue; \
    if (lcdDrawUpdate) \
      lcd_implementation_drawedit(editLabel, STRFUNC(((TYPE)((int32_t)encoderPosition + minEditValue)) * (1.0f / SCALE))); \
    if (lcd_clicked || (liveEdit && lcdDrawUpdate)) { \
      TYPE value = ((TYPE)((int32_t)encoderPosition + minEditValue)) * (1.0f / SCALE); \
      if (editValue != NULL) *((TYPE*)editValue) = value; \
      if (callbackFunc && (liveEdit || lcd_clicked)) (*callbackFunc)(); \
      if (lcd_clicked) lcd_goto_previous_menu(); \
    } \
    return use_click(); \
  } \
  void menu_edit_ ## NAME() { _menu_edit_ ## NAME(); } \
  void _menu_action_setting_edit_ ## NAME(PGM_P const pstr, TYPE* const ptr, const TYPE minValue, const TYPE maxValue) { \
    lcd_save_previous_screen(); \
    lcd_refresh(); \
    \
    editLabel = pstr; \
    editValue = ptr; \
    minEditValue = minValue * SCALE; \
    maxEditValue = maxValue * SCALE - minEditValue; \
    encoderPosition = (*ptr) * SCALE - minEditValue; \
  } \
  void menu_action_setting_edit_callback_ ## NAME(PGM_P const pstr, TYPE * const ptr, const TYPE minValue, const TYPE maxValue, const screenFunc_t callback/*=NULL*/, const bool live/*=false*/) { \
    _menu_action_setting_edit_ ## NAME(pstr, ptr, minValue, maxValue); \
    currentScreen = menu_edit_ ## NAME; \
    callbackFunc = callback; \
    liveEdit = live; \
  } \
  typedef void NAME##_void

DEFINE_MENU_EDIT_TYPE(int16_t, int3, itostr3, 1);
DEFINE_MENU_EDIT_TYPE(int16_t, int4, itostr4sign, 1);
DEFINE_MENU_EDIT_TYPE(uint8_t, int8, i8tostr3, 1);
DEFINE_MENU_EDIT_TYPE(float, float3, ftostr3, 1);
DEFINE_MENU_EDIT_TYPE(float, float52, ftostr52, 100);
DEFINE_MENU_EDIT_TYPE(float, float43, ftostr43sign, 1000);
DEFINE_MENU_EDIT_TYPE(float, float5, ftostr5rj, 0.01f);
DEFINE_MENU_EDIT_TYPE(float, float51, ftostr51sign, 10);
DEFINE_MENU_EDIT_TYPE(float, float52sign, ftostr52sign, 100);
DEFINE_MENU_EDIT_TYPE(float, float62, ftostr62rj, 100);
DEFINE_MENU_EDIT_TYPE(uint32_t, long5, ftostr5rj, 0.01f);

void menu_action_setting_edit_bool(PGM_P pstr, bool* ptr) { UNUSED(pstr); *ptr ^= true; lcd_refresh(); }
void menu_action_setting_edit_callback_bool(PGM_P pstr, bool* ptr, screenFunc_t callback) {
  menu_action_setting_edit_bool(pstr, ptr);
  (*callback)();
}

////////////////////////////////////////////
///////////////// Menu Tree ////////////////
////////////////////////////////////////////

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void menu_change_filament();
  #else
    void menu_temp_e0_filament_change();
  #endif
#endif

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float lcd_z_fade_height;
  void _lcd_set_z_fade_height() { set_z_fade_height(lcd_z_fade_height); }
#endif

bool printer_busy() { return planner.movesplanned() || IS_SD_PRINTING(); }

#if HAS_CHARACTER_LCD && (ENABLED(LCD_PROGRESS_BAR) || ENABLED(LCD_PROGRESS_BAR_TEST) || ENABLED(AUTO_BED_LEVELING_UBL))
  void lcd_set_custom_characters(
    #if ENABLED(LCD_PROGRESS_BAR) || ENABLED(SHOW_BOOTSCREEN)
      const uint8_t screen_charset=CHARSET_INFO
    #endif
  );
#endif

/**
 * General function to go directly to a screen
 */
void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder/*=0*/) {
  if (currentScreen != screen) {

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      // Shadow for editing the fade height
      lcd_z_fade_height = planner.z_fade_height;
    #endif

    #if ENABLED(DOUBLECLICK_FOR_Z_BABYSTEPPING) && ENABLED(BABYSTEPPING)
      static millis_t doubleclick_expire_ms = 0;
      // Going to menu_main from status screen? Remember first click time.
      // Going back to status screen within a very short time? Go to Z babystepping.
      if (screen == menu_main) {
        if (currentScreen == lcd_status_screen)
          doubleclick_expire_ms = millis() + DOUBLECLICK_MAX_INTERVAL;
      }
      else if (screen == lcd_status_screen && currentScreen == menu_main && PENDING(millis(), doubleclick_expire_ms)) {
        if (printer_busy()) {
          screen =
            #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
              lcd_babystep_zoffset
            #else
              lcd_babystep_z
            #endif
          ;
        }
        #if ENABLED(MOVE_Z_WHEN_IDLE)
          else {
            move_menu_scale = MOVE_Z_IDLE_MULTIPLICATOR;
            screen = lcd_move_z;
          }
        #endif
      }
    #endif

    currentScreen = screen;
    encoderPosition = encoder;
    if (screen == lcd_status_screen) {
      defer_return_to_status = false;
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.lcd_map_control = false;
      #endif
      screen_history_depth = 0;
    }
    lcd_implementation_clear();
    // Re-initialize custom characters that may be re-used
    #if HAS_CHARACTER_LCD && ENABLED(AUTO_BED_LEVELING_UBL)
      if (!ubl.lcd_map_control) {
        lcd_set_custom_characters(
          #if ENABLED(LCD_PROGRESS_BAR)
            screen == lcd_status_screen ? CHARSET_INFO : CHARSET_MENU
          #endif
        );
      }
    #elif ENABLED(LCD_PROGRESS_BAR)
      lcd_set_custom_characters(screen == lcd_status_screen ? CHARSET_INFO : CHARSET_MENU);
    #endif
    lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
    screen_changed = true;
    #if HAS_GRAPHICAL_LCD
      drawing_screen = false;
    #endif
  }
}

////////////////////////////////////////////
///////////// Manual Movement //////////////
////////////////////////////////////////////

//
// Display the synchronize screen until moves are
// finished, and don't return to the caller until
// done. ** This blocks the command queue! **
//
static PGM_P sync_message;

void _lcd_synchronize() {
  if (lcdDrawUpdate) lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, sync_message);
  if (no_reentry) return;
  // Make this the current handler till all moves are done
  no_reentry = true;
  const screenFunc_t old_screen = currentScreen;
  lcd_goto_screen(_lcd_synchronize);
  planner.synchronize(); // idle() is called until moves complete
  no_reentry = false;
  lcd_goto_screen(old_screen);
}

// Display the synchronize screen with a custom message
// ** This blocks the command queue! **
void lcd_synchronize(PGM_P const msg/*=NULL*/) {
  static const char moving[] PROGMEM = MSG_MOVING;
  sync_message = msg ? msg : moving;
  _lcd_synchronize();
}

/**
 * Scrolling for menus and other line-based screens
 *
 *   encoderLine is the position based on the encoder
 *   encoderTopLine is the top menu line to display
 *   _lcdLineNr is the index of the LCD line (e.g., 0-3)
 *   _menuLineNr is the menu item to draw and process
 *   _thisItemNr is the index of each MENU_ITEM or STATIC_ITEM
 *   screen_items is the total number of items in the menu (after one call)
 */
int8_t encoderLine, screen_items;
void scroll_screen(const uint8_t limit, const bool is_menu) {
  ENCODER_DIRECTION_MENUS();
  ENCODER_RATE_MULTIPLY(false);
  if (encoderPosition > 0x8000) encoderPosition = 0;
  if (first_page) {
    encoderLine = encoderPosition / (ENCODER_STEPS_PER_MENU_ITEM);
    screen_changed = false;
  }
  if (screen_items > 0 && encoderLine >= screen_items - limit) {
    encoderLine = MAX(0, screen_items - limit);
    encoderPosition = encoderLine * (ENCODER_STEPS_PER_MENU_ITEM);
  }
  if (is_menu) {
    NOMORE(encoderTopLine, encoderLine);
    if (encoderLine >= encoderTopLine + LCD_HEIGHT)
      encoderTopLine = encoderLine - LCD_HEIGHT + 1;
  }
  else
    encoderTopLine = encoderLine;
}

void lcd_completion_feedback(const bool good/*=true*/) {
  if (good) {
    lcd_buzz(100, 659);
    lcd_buzz(100, 698);
  }
  else lcd_buzz(20, 440);
}

inline void line_to_current_z() {
  planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[Z_AXIS]), active_extruder);
}

void line_to_z(const float &z) {
  current_position[Z_AXIS] = z;
  line_to_current_z();
}

#if ENABLED(POWER_LOSS_RECOVERY)

  static void lcd_power_loss_recovery_resume() {
    char cmd[20];

    // Return to status now
    lcd_return_to_status();

    // Turn leveling off and home
    enqueue_and_echo_commands_P(PSTR("M420 S0\nG28 R0"
      #if ENABLED(MARLIN_DEV_MODE)
        " S"
      #elif !IS_KINEMATIC
        " X Y"
      #endif
    ));

    #if HAS_HEATED_BED
      const int16_t bt = job_recovery_info.target_temperature_bed;
      if (bt) {
        // Restore the bed temperature
        sprintf_P(cmd, PSTR("M190 S%i"), bt);
        enqueue_and_echo_command(cmd);
      }
    #endif

    // Restore all hotend temperatures
    HOTEND_LOOP() {
      const int16_t et = job_recovery_info.target_temperature[e];
      if (et) {
        #if HOTENDS > 1
          sprintf_P(cmd, PSTR("T%i"), e);
          enqueue_and_echo_command(cmd);
        #endif
        sprintf_P(cmd, PSTR("M109 S%i"), et);
        enqueue_and_echo_command(cmd);
      }
    }

    #if HOTENDS > 1
      sprintf_P(cmd, PSTR("T%i"), job_recovery_info.active_hotend);
      enqueue_and_echo_command(cmd);
    #endif

    // Restore print cooling fan speeds
    for (uint8_t i = 0; i < FAN_COUNT; i++) {
      uint8_t f = job_recovery_info.fan_speed[i];
      if (f) {
        sprintf_P(cmd, PSTR("M106 P%i S%i"), i, f);
        enqueue_and_echo_command(cmd);
      }
    }

    // Start draining the job recovery command queue
    job_recovery_phase = JOB_RECOVERY_YES;
  }

  static void lcd_power_loss_recovery_cancel() {
    card.removeJobRecoveryFile();
    card.autostart_index = 0;
    lcd_return_to_status();
  }

  void menu_job_recovery() {
    defer_return_to_status = true;
    START_MENU();
    STATIC_ITEM(MSG_POWER_LOSS_RECOVERY);
    MENU_ITEM(function, MSG_RESUME_PRINT, lcd_power_loss_recovery_resume);
    MENU_ITEM(function, MSG_STOP_PRINT, lcd_power_loss_recovery_cancel);
    END_MENU();
  }

#endif // POWER_LOSS_RECOVERY

#if ENABLED(CUSTOM_USER_MENUS)

  #ifdef USER_SCRIPT_DONE
    #define _DONE_SCRIPT "\n" USER_SCRIPT_DONE
  #else
    #define _DONE_SCRIPT ""
  #endif

  void _lcd_user_gcode(PGM_P const cmd) {
    enqueue_and_echo_commands_P(cmd);
    #if ENABLED(USER_SCRIPT_AUDIBLE_FEEDBACK)
      lcd_completion_feedback();
    #endif
    #if ENABLED(USER_SCRIPT_RETURN)
      lcd_return_to_status();
    #endif
  }

  #if defined(USER_DESC_1) && defined(USER_GCODE_1)
    void lcd_user_gcode_1() { _lcd_user_gcode(PSTR(USER_GCODE_1 _DONE_SCRIPT)); }
  #endif
  #if defined(USER_DESC_2) && defined(USER_GCODE_2)
    void lcd_user_gcode_2() { _lcd_user_gcode(PSTR(USER_GCODE_2 _DONE_SCRIPT)); }
  #endif
  #if defined(USER_DESC_3) && defined(USER_GCODE_3)
    void lcd_user_gcode_3() { _lcd_user_gcode(PSTR(USER_GCODE_3 _DONE_SCRIPT)); }
  #endif
  #if defined(USER_DESC_4) && defined(USER_GCODE_4)
    void lcd_user_gcode_4() { _lcd_user_gcode(PSTR(USER_GCODE_4 _DONE_SCRIPT)); }
  #endif
  #if defined(USER_DESC_5) && defined(USER_GCODE_5)
    void lcd_user_gcode_5() { _lcd_user_gcode(PSTR(USER_GCODE_5 _DONE_SCRIPT)); }
  #endif

  void _menu_user() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    #if defined(USER_DESC_1) && defined(USER_GCODE_1)
      MENU_ITEM(function, USER_DESC_1, lcd_user_gcode_1);
    #endif
    #if defined(USER_DESC_2) && defined(USER_GCODE_2)
      MENU_ITEM(function, USER_DESC_2, lcd_user_gcode_2);
    #endif
    #if defined(USER_DESC_3) && defined(USER_GCODE_3)
      MENU_ITEM(function, USER_DESC_3, lcd_user_gcode_3);
    #endif
    #if defined(USER_DESC_4) && defined(USER_GCODE_4)
      MENU_ITEM(function, USER_DESC_4, lcd_user_gcode_4);
    #endif
    #if defined(USER_DESC_5) && defined(USER_GCODE_5)
      MENU_ITEM(function, USER_DESC_5, lcd_user_gcode_5);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)

  void lcd_babystep_zoffset() {
    if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
    defer_return_to_status = true;
    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
      const bool do_probe = (active_extruder == 0);
    #else
      constexpr bool do_probe = true;
    #endif
    ENCODER_DIRECTION_NORMAL();
    if (encoderPosition) {
      const int16_t babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR);
      encoderPosition = 0;

      const float diff = planner.steps_to_mm[Z_AXIS] * babystep_increment,
                  new_probe_offset = zprobe_zoffset + diff,
                  new_offs =
                    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
                      do_probe ? new_probe_offset : hotend_offset[Z_AXIS][active_extruder] - diff
                    #else
                      new_probe_offset
                    #endif
                  ;
      if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {

        thermalManager.babystep_axis(Z_AXIS, babystep_increment);

        if (do_probe) zprobe_zoffset = new_offs;
        #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
          else hotend_offset[Z_AXIS][active_extruder] = new_offs;
        #endif

        lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
      }
    }
    if (lcdDrawUpdate) {
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        if (!do_probe)
          lcd_implementation_drawedit(PSTR(MSG_IDEX_Z_OFFSET), ftostr43sign(hotend_offset[Z_AXIS][active_extruder]));
        else
      #endif
          lcd_implementation_drawedit(PSTR(MSG_ZPROBE_ZOFFSET), ftostr43sign(zprobe_zoffset));

      #if ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY)
        if (do_probe) _lcd_zoffset_overlay_gfx(zprobe_zoffset);
      #endif
    }
  }

#endif // BABYSTEP_ZPROBE_OFFSET

/**
 * Watch temperature callbacks
 */
#if HAS_TEMP_HOTEND
  #if WATCH_HOTENDS
    #define _WATCH_FUNC(N) thermalManager.start_watching_heater(N)
  #else
    #define _WATCH_FUNC(N) NOOP
  #endif
  void watch_temp_callback_E0() { _WATCH_FUNC(0); }
  #if HOTENDS > 1
    void watch_temp_callback_E1() { _WATCH_FUNC(1); }
    #if HOTENDS > 2
      void watch_temp_callback_E2() { _WATCH_FUNC(2); }
      #if HOTENDS > 3
        void watch_temp_callback_E3() { _WATCH_FUNC(3); }
        #if HOTENDS > 4
          void watch_temp_callback_E4() { _WATCH_FUNC(4); }
          #if HOTENDS > 5
            void watch_temp_callback_E5() { _WATCH_FUNC(5); }
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1
#endif // HAS_TEMP_HOTEND

void watch_temp_callback_bed() {
  #if WATCH_THE_BED
    thermalManager.start_watching_bed();
  #endif
}

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(PID_AUTOTUNE_MENU) || ENABLED(ADVANCED_PAUSE_FEATURE)

  void lcd_enqueue_command(const char * const cmd) {
    no_reentry = true;
    enqueue_and_echo_command_now(cmd);
    no_reentry = false;
  }

  void lcd_enqueue_commands_P(PGM_P const cmd) {
    no_reentry = true;
    enqueue_and_echo_commands_now_P(cmd);
    no_reentry = false;
  }

#endif

#if ENABLED(EEPROM_SETTINGS)
  void lcd_store_settings()   { lcd_completion_feedback(settings.save()); }
  void lcd_load_settings()    { lcd_completion_feedback(settings.load()); }
#endif

void _lcd_draw_homing() {
  constexpr uint8_t line = (LCD_HEIGHT - 1) / 2;
  if (lcdDrawUpdate) lcd_implementation_drawmenu_static(line, PSTR(MSG_LEVEL_BED_HOMING));
  lcdDrawUpdate = LCDVIEW_CALL_NO_REDRAW;
}

#if ENABLED(LCD_BED_LEVELING) || (HAS_LEVELING && DISABLED(SLIM_LCD_MENUS))
  void _lcd_toggle_bed_leveling() { set_bed_leveling_enabled(!planner.leveling_active); }
#endif

#if ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION)

  void _man_probe_pt(const float &rx, const float &ry) {
    do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
    do_blocking_move_to_xy(rx, ry);

    lcd_synchronize();
    move_menu_scale = MAX(PROBE_MANUALLY_STEP, MIN_STEPS_PER_SEGMENT / float(DEFAULT_XYZ_STEPS_PER_UNIT));
    lcd_goto_screen(lcd_move_z);
  }

#endif // DELTA_CALIBRATION_MENU || DELTA_AUTO_CALIBRATION

#if ENABLED(DELTA_AUTO_CALIBRATION)

  float lcd_probe_pt(const float &rx, const float &ry) {
    _man_probe_pt(rx, ry);
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    defer_return_to_status = true;
    wait_for_user = true;
    while (wait_for_user) idle();
    KEEPALIVE_STATE(IN_HANDLER);
    lcd_goto_previous_menu_no_defer();
    return current_position[Z_AXIS];
  }

#endif // DELTA_AUTO_CALIBRATION

#if ENABLED(DELTA_CALIBRATION_MENU)

  void _lcd_calibrate_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) lcd_goto_previous_menu();
  }

  void _lcd_delta_calibrate_home() {
    enqueue_and_echo_commands_P(PSTR("G28"));
    lcd_goto_screen(_lcd_calibrate_homing);
  }

  void _goto_tower_x() { _man_probe_pt(cos(RADIANS(210)) * delta_calibration_radius, sin(RADIANS(210)) * delta_calibration_radius); }
  void _goto_tower_y() { _man_probe_pt(cos(RADIANS(330)) * delta_calibration_radius, sin(RADIANS(330)) * delta_calibration_radius); }
  void _goto_tower_z() { _man_probe_pt(cos(RADIANS( 90)) * delta_calibration_radius, sin(RADIANS( 90)) * delta_calibration_radius); }
  void _goto_center()  { _man_probe_pt(0,0); }

#endif // DELTA_CALIBRATION_MENU

#if ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION)

  void _recalc_delta_settings() {
    #if HAS_LEVELING
      reset_bed_level(); // After changing kinematics bed-level data is no longer valid
    #endif
    recalc_delta_settings();
  }

  void lcd_delta_settings() {
    START_MENU();
    MENU_BACK(MSG_DELTA_CALIBRATE);
    MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_HEIGHT, &delta_height, delta_height - 10, delta_height + 10, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Ex", &delta_endstop_adj[A_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Ey", &delta_endstop_adj[B_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Ez", &delta_endstop_adj[C_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_RADIUS, &delta_radius, delta_radius - 5, delta_radius + 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Tx", &delta_tower_angle_trim[A_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Ty", &delta_tower_angle_trim[B_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float43, "Tz", &delta_tower_angle_trim[C_AXIS], -5, 5, _recalc_delta_settings);
    MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_DIAG_ROD, &delta_diagonal_rod, delta_diagonal_rod - 5, delta_diagonal_rod + 5, _recalc_delta_settings);
    END_MENU();
  }

  void menu_delta_calibrate() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    #if ENABLED(DELTA_AUTO_CALIBRATION)
      MENU_ITEM(gcode, MSG_DELTA_AUTO_CALIBRATE, PSTR("G33"));
      MENU_ITEM(gcode, MSG_DELTA_HEIGHT_CALIBRATE, PSTR("G33 S P1"));
      MENU_ITEM(gcode, MSG_DELTA_Z_OFFSET_CALIBRATE, PSTR("G33 P-1"));
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
        MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
      #endif
    #endif
    MENU_ITEM(submenu, MSG_DELTA_SETTINGS, lcd_delta_settings);
    #if ENABLED(DELTA_CALIBRATION_MENU)
      MENU_ITEM(submenu, MSG_AUTO_HOME, _lcd_delta_calibrate_home);
      if (all_axes_homed()) {
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_X, _goto_tower_x);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Y, _goto_tower_y);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Z, _goto_tower_z);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_CENTER, _goto_center);
      }
    #endif
    END_MENU();
  }

#endif // DELTA_CALIBRATION_MENU || DELTA_AUTO_CALIBRATION

#if ENABLED(SDSUPPORT)

  #if !PIN_EXISTS(SD_DETECT)
    void lcd_sd_refresh() {
      card.initsd();
      encoderTopLine = 0;
    }
  #endif

  void lcd_sd_updir() {
    encoderPosition = card.updir() ? ENCODER_STEPS_PER_MENU_ITEM : 0;
    encoderTopLine = 0;
    screen_changed = true;
    lcd_refresh();
  }

  /**
   *
   * "Print from SD" submenu
   *
   */

  #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
    uint32_t last_sdfile_encoderPosition = 0xFFFF;

    void lcd_reselect_last_file() {
      if (last_sdfile_encoderPosition == 0xFFFF) return;
      #if HAS_GRAPHICAL_LCD
        // Some of this is a hack to force the screen update to work.
        // TODO: Fix the real issue that causes this!
        lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
        lcd_synchronize();
        safe_delay(50);
        lcd_synchronize();
        lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
        drawing_screen = screen_changed = true;
      #endif

      lcd_goto_screen(menu_sdcard, last_sdfile_encoderPosition);
      defer_return_to_status = true;
      last_sdfile_encoderPosition = 0xFFFF;

      #if HAS_GRAPHICAL_LCD
        lcd_update();
      #endif
    }
  #endif

  void menu_sdcard() {
    ENCODER_DIRECTION_MENUS();

    const uint16_t fileCnt = card.get_num_Files();

    START_MENU();
    MENU_BACK(MSG_MAIN);
    card.getWorkDirName();
    if (card.filename[0] == '/') {
      #if !PIN_EXISTS(SD_DETECT)
        MENU_ITEM(function, LCD_STR_REFRESH MSG_REFRESH, lcd_sd_refresh);
      #endif
    }
    else {
      MENU_ITEM(function, LCD_STR_FOLDER "..", lcd_sd_updir);
    }

    for (uint16_t i = 0; i < fileCnt; i++) {
      if (_menuLineNr == _thisItemNr) {
        const uint16_t nr =
          #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
            fileCnt - 1 -
          #endif
        i;

        #if ENABLED(SDCARD_SORT_ALPHA)
          card.getfilename_sorted(nr);
        #else
          card.getfilename(nr);
        #endif

        if (card.filenameIsDir)
          MENU_ITEM(sddirectory, MSG_CARD_MENU, card);
        else
          MENU_ITEM(sdfile, MSG_CARD_MENU, card);
      }
      else {
        MENU_ITEM_DUMMY();
      }
    }
    END_MENU();
  }

#endif // SDSUPPORT

/**
 *
 * LED Menu
 *
 */

#if ENABLED(LED_CONTROL_MENU)

  #if ENABLED(LED_COLOR_PRESETS)

    void menu_led_presets() {
      START_MENU();
      #if LCD_HEIGHT > 2
        STATIC_ITEM(MSG_LED_PRESETS, true, true);
      #endif
      MENU_BACK(MSG_LED_CONTROL);
      MENU_ITEM(function, MSG_SET_LEDS_WHITE, leds.set_white);
      MENU_ITEM(function, MSG_SET_LEDS_RED, leds.set_red);
      MENU_ITEM(function, MSG_SET_LEDS_ORANGE, leds.set_orange);
      MENU_ITEM(function, MSG_SET_LEDS_YELLOW,leds.set_yellow);
      MENU_ITEM(function, MSG_SET_LEDS_GREEN, leds.set_green);
      MENU_ITEM(function, MSG_SET_LEDS_BLUE, leds.set_blue);
      MENU_ITEM(function, MSG_SET_LEDS_INDIGO, leds.set_indigo);
      MENU_ITEM(function, MSG_SET_LEDS_VIOLET, leds.set_violet);
      END_MENU();
    }
  #endif // LED_COLOR_PRESETS

  void menu_led_custom() {
    START_MENU();
    MENU_BACK(MSG_LED_CONTROL);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_INTENSITY_R, &leds.color.r, 0, 255, leds.update, true);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_INTENSITY_G, &leds.color.g, 0, 255, leds.update, true);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_INTENSITY_B, &leds.color.b, 0, 255, leds.update, true);
    #if ENABLED(RGBW_LED) || ENABLED(NEOPIXEL_LED)
      MENU_ITEM_EDIT_CALLBACK(int8, MSG_INTENSITY_W, &leds.color.w, 0, 255, leds.update, true);
      #if ENABLED(NEOPIXEL_LED)
        MENU_ITEM_EDIT_CALLBACK(int8, MSG_LED_BRIGHTNESS, &leds.color.i, 0, 255, leds.update, true);
      #endif
    #endif
    END_MENU();
  }

  void menu_led() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    bool led_on = leds.lights_on;
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_LEDS, &led_on, leds.toggle);
    MENU_ITEM(function, MSG_SET_LEDS_DEFAULT, leds.set_default);
    #if ENABLED(LED_COLOR_PRESETS)
      MENU_ITEM(submenu, MSG_LED_PRESETS, menu_led_presets);
    #endif
    MENU_ITEM(submenu, MSG_CUSTOM_LEDS, menu_led_custom);
    END_MENU();
  }

#endif // LED_CONTROL_MENU

/**
 *
 * Filament Change Feature Screens
 *
 */
#if ENABLED(ADVANCED_PAUSE_FEATURE)

  /**
   *
   * "Change Filament" > "Change/Unload/Load Filament" submenu
   *
   */
  static AdvancedPauseMode _change_filament_temp_mode; // =ADVANCED_PAUSE_MODE_PAUSE_PRINT
  static int8_t _change_filament_temp_extruder; // =0

  static PGM_P _change_filament_temp_command() {
    switch (_change_filament_temp_mode) {
      case ADVANCED_PAUSE_MODE_LOAD_FILAMENT:
        return PSTR("M701 T%d");
      case ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT:
        return _change_filament_temp_extruder >= 0 ? PSTR("M702 T%d") : PSTR("M702 ;%d");
      case ADVANCED_PAUSE_MODE_PAUSE_PRINT:
      default:
        return PSTR("M600 B0 T%d");
    }
    return PSTR(MSG_FILAMENTCHANGE);
  }

  void _change_filament_temp(const uint16_t temperature) {
    char cmd[11];
    sprintf_P(cmd, _change_filament_temp_command(), _change_filament_temp_extruder);
    thermalManager.setTargetHotend(temperature, _change_filament_temp_extruder);
    lcd_enqueue_command(cmd);
  }
  void _menu_change_filament_temp_1() { _change_filament_temp(PREHEAT_1_TEMP_HOTEND); }
  void _menu_change_filament_temp_2() { _change_filament_temp(PREHEAT_2_TEMP_HOTEND); }
  void _menu_change_filament_temp_custom() { _change_filament_temp(thermalManager.target_temperature[_change_filament_temp_extruder]); }

  static PGM_P change_filament_header(const AdvancedPauseMode mode) {
    switch (mode) {
      case ADVANCED_PAUSE_MODE_LOAD_FILAMENT:
        return PSTR(MSG_FILAMENTLOAD);
      case ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT:
        return PSTR(MSG_FILAMENTUNLOAD);
      default: break;
    }
    return PSTR(MSG_FILAMENTCHANGE);
  }

  void _menu_temp_filament_op(const AdvancedPauseMode mode, const int8_t extruder) {
    _change_filament_temp_mode = mode;
    _change_filament_temp_extruder = extruder;
    START_MENU();
    if (LCD_HEIGHT >= 4) STATIC_ITEM_P(change_filament_header(mode), true, true);
    MENU_BACK(MSG_BACK);
    MENU_ITEM(submenu, MSG_PREHEAT_1, _menu_change_filament_temp_1);
    MENU_ITEM(submenu, MSG_PREHEAT_2, _menu_change_filament_temp_2);
    uint16_t max_temp;
    switch (extruder) {
      default: max_temp = HEATER_0_MAXTEMP;
      #if HOTENDS > 1
        case 1: max_temp = HEATER_1_MAXTEMP; break;
        #if HOTENDS > 2
          case 2: max_temp = HEATER_2_MAXTEMP; break;
          #if HOTENDS > 3
            case 3: max_temp = HEATER_3_MAXTEMP; break;
            #if HOTENDS > 4
              case 4: max_temp = HEATER_4_MAXTEMP; break;
              #if HOTENDS > 5
                case 5: max_temp = HEATER_5_MAXTEMP; break;
              #endif
            #endif
          #endif
        #endif
      #endif
    }
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_PREHEAT_CUSTOM, &thermalManager.target_temperature[_change_filament_temp_extruder], EXTRUDE_MINTEMP, max_temp - 15, _menu_change_filament_temp_custom);
    END_MENU();
  }
  void menu_temp_e0_filament_change()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_PAUSE_PRINT, 0); }
  void menu_temp_e0_filament_load()    { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_LOAD_FILAMENT, 0); }
  void menu_temp_e0_filament_unload()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, 0); }
  #if E_STEPPERS > 1
    void menu_temp_e1_filament_change()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_PAUSE_PRINT, 1); }
    void menu_temp_e1_filament_load()    { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_LOAD_FILAMENT, 1); }
    void menu_temp_e1_filament_unload()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, 1); }
    #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
      void menu_unload_filament_all_temp() { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, -1); }
    #endif
    #if E_STEPPERS > 2
      void menu_temp_e2_filament_change()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_PAUSE_PRINT, 2); }
      void menu_temp_e2_filament_load()    { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_LOAD_FILAMENT, 2); }
      void menu_temp_e2_filament_unload()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, 2); }
      #if E_STEPPERS > 3
        void menu_temp_e3_filament_change()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_PAUSE_PRINT, 3); }
        void menu_temp_e3_filament_load()    { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_LOAD_FILAMENT, 3); }
        void menu_temp_e3_filament_unload()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, 3); }
        #if E_STEPPERS > 4
          void menu_temp_e4_filament_change()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_PAUSE_PRINT, 4); }
          void menu_temp_e4_filament_load()    { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_LOAD_FILAMENT, 4); }
          void menu_temp_e4_filament_unload()  { _menu_temp_filament_op(ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, 4); }
        #endif // E_STEPPERS > 4
      #endif // E_STEPPERS > 3
    #endif // E_STEPPERS > 2
  #endif // E_STEPPERS > 1

  /**
   *
   * "Change Filament" submenu
   *
   */
  #if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void menu_change_filament() {
      START_MENU();
      MENU_BACK(MSG_MAIN);

      // Change filament
      #if E_STEPPERS == 1
        PGM_P msg0 = PSTR(MSG_FILAMENTCHANGE);
        if (thermalManager.targetTooColdToExtrude(active_extruder))
          MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_change);
        else
          MENU_ITEM_P(gcode, msg0, PSTR("M600 B0"));
      #else
        PGM_P msg0 = PSTR(MSG_FILAMENTCHANGE " " MSG_E1);
        PGM_P msg1 = PSTR(MSG_FILAMENTCHANGE " " MSG_E2);
        if (thermalManager.targetTooColdToExtrude(0))
          MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_change);
        else
          MENU_ITEM_P(gcode, msg0, PSTR("M600 B0 T0"));
        if (thermalManager.targetTooColdToExtrude(1))
          MENU_ITEM_P(submenu, msg1, menu_temp_e1_filament_change);
        else
          MENU_ITEM_P(gcode, msg1, PSTR("M600 B0 T1"));
        #if E_STEPPERS > 2
          PGM_P msg2 = PSTR(MSG_FILAMENTCHANGE " " MSG_E3);
          if (thermalManager.targetTooColdToExtrude(2))
            MENU_ITEM_P(submenu, msg2, menu_temp_e2_filament_change);
          else
            MENU_ITEM_P(gcode, msg2, PSTR("M600 B0 T2"));
          #if E_STEPPERS > 3
            PGM_P msg3 = PSTR(MSG_FILAMENTCHANGE " " MSG_E4);
            if (thermalManager.targetTooColdToExtrude(3))
              MENU_ITEM_P(submenu, msg3, menu_temp_e3_filament_change);
            else
              MENU_ITEM_P(gcode, msg3, PSTR("M600 B0 T3"));
            #if E_STEPPERS > 4
              PGM_P msg4 = PSTR(MSG_FILAMENTCHANGE " " MSG_E5);
              if (thermalManager.targetTooColdToExtrude(4))
                MENU_ITEM_P(submenu, msg4, menu_temp_e4_filament_change);
              else
                MENU_ITEM_P(gcode, msg4, PSTR("M600 B0 T4"));
              #if E_STEPPERS > 5
                PGM_P msg5 = PSTR(MSG_FILAMENTCHANGE " " MSG_E6);
                if (thermalManager.targetTooColdToExtrude(5))
                  MENU_ITEM_P(submenu, msg5, menu_temp_e5_filament_change);
                else
                  MENU_ITEM_P(gcode, msg5, PSTR("M600 B0 T5"));
              #endif // E_STEPPERS > 5
            #endif // E_STEPPERS > 4
          #endif // E_STEPPERS > 3
        #endif // E_STEPPERS > 2
      #endif // E_STEPPERS == 1

      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        if (!printer_busy()) {
          // Load filament
          #if E_STEPPERS == 1
            PGM_P msg0 = PSTR(MSG_FILAMENTLOAD);
            if (thermalManager.targetTooColdToExtrude(active_extruder))
              MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_load);
            else
              MENU_ITEM_P(gcode, msg0, PSTR("M701"));
          #else
            PGM_P msg0 = PSTR(MSG_FILAMENTLOAD " " MSG_E1);
            PGM_P msg1 = PSTR(MSG_FILAMENTLOAD " " MSG_E2);
            if (thermalManager.targetTooColdToExtrude(0))
              MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_load);
            else
              MENU_ITEM_P(gcode, msg0, PSTR("M701 T0"));
            if (thermalManager.targetTooColdToExtrude(1))
              MENU_ITEM_P(submenu, msg1, menu_temp_e1_filament_load);
            else
              MENU_ITEM_P(gcode, msg1, PSTR("M701 T1"));
            #if E_STEPPERS > 2
              PGM_P msg2 = PSTR(MSG_FILAMENTLOAD " " MSG_E3);
              if (thermalManager.targetTooColdToExtrude(2))
                MENU_ITEM_P(submenu, msg2, menu_temp_e2_filament_load);
              else
                MENU_ITEM_P(gcode, msg2, PSTR("M701 T2"));
              #if E_STEPPERS > 3
                PGM_P msg3 = PSTR(MSG_FILAMENTLOAD " " MSG_E4);
                if (thermalManager.targetTooColdToExtrude(3))
                  MENU_ITEM_P(submenu, msg3, menu_temp_e3_filament_load);
                else
                  MENU_ITEM_P(gcode, msg3, PSTR("M701 T3"));
                #if E_STEPPERS > 4
                  PGM_P msg4 = PSTR(MSG_FILAMENTLOAD " " MSG_E5);
                  if (thermalManager.targetTooColdToExtrude(4))
                    MENU_ITEM_P(submenu, msg4, menu_temp_e4_filament_load);
                  else
                    MENU_ITEM_P(gcode, msg4, PSTR("M701 T4"));
                  #if E_STEPPERS > 5
                    PGM_P msg5 = PSTR(MSG_FILAMENTLOAD " " MSG_E6);
                    if (thermalManager.targetTooColdToExtrude(5))
                      MENU_ITEM_P(submenu, msg5, menu_temp_e5_filament_load);
                    else
                      MENU_ITEM_P(gcode, msg5, PSTR("M701 T5"));
                  #endif // E_STEPPERS > 5
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS == 1

          // Unload filament
          #if E_STEPPERS == 1
            if (thermalManager.targetHotEnoughToExtrude(active_extruder))
              MENU_ITEM(gcode, MSG_FILAMENTUNLOAD, PSTR("M702"));
            else
              MENU_ITEM(submenu, MSG_FILAMENTUNLOAD, menu_temp_e0_filament_unload);
          #else
            #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
              if (thermalManager.targetHotEnoughToExtrude(0)
                #if E_STEPPERS > 1
                  && thermalManager.targetHotEnoughToExtrude(1)
                  #if E_STEPPERS > 2
                    && thermalManager.targetHotEnoughToExtrude(2)
                    #if E_STEPPERS > 3
                      && thermalManager.targetHotEnoughToExtrude(3)
                      #if E_STEPPERS > 4
                        && thermalManager.targetHotEnoughToExtrude(4)
                        #if E_STEPPERS > 5
                          && thermalManager.targetHotEnoughToExtrude(5)
                        #endif // E_STEPPERS > 5
                      #endif // E_STEPPERS > 4
                    #endif // E_STEPPERS > 3
                  #endif // E_STEPPERS > 2
                #endif // E_STEPPERS > 1
              )
                MENU_ITEM(gcode, MSG_FILAMENTUNLOAD_ALL, PSTR("M702"));
            else
              MENU_ITEM(submenu, MSG_FILAMENTUNLOAD_ALL, menu_unload_filament_all_temp);
            #endif
            if (thermalManager.targetHotEnoughToExtrude(0))
              MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E1, PSTR("M702 T0"));
            else
              MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E1, menu_temp_e0_filament_unload);
            if (thermalManager.targetHotEnoughToExtrude(1))
              MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E2, PSTR("M702 T1"));
            else
              MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E2, menu_temp_e1_filament_unload);
            #if E_STEPPERS > 2
              if (thermalManager.targetHotEnoughToExtrude(2))
                MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E3, PSTR("M702 T2"));
              else
                MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E3, menu_temp_e2_filament_unload);
              #if E_STEPPERS > 3
                if (thermalManager.targetHotEnoughToExtrude(3))
                  MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E4, PSTR("M702 T3"));
                else
                  MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E4, menu_temp_e3_filament_unload);
                #if E_STEPPERS > 4
                  if (thermalManager.targetHotEnoughToExtrude(4))
                    MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E5, PSTR("M702 T4"));
                  else
                    MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E5, menu_temp_e4_filament_unload);
                  #if E_STEPPERS > 5
                    if (thermalManager.targetHotEnoughToExtrude(5))
                      MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E6, PSTR("M702 T5"));
                    else
                      MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E6, menu_temp_e5_filament_unload);
                  #endif // E_STEPPERS > 5
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS == 1
        }
      #endif

      END_MENU();
    }
  #endif

  static AdvancedPauseMode advanced_pause_mode = ADVANCED_PAUSE_MODE_PAUSE_PRINT;
  static uint8_t hotend_status_extruder = 0;

  static PGM_P advanced_pause_header() {
    switch (advanced_pause_mode) {
      case ADVANCED_PAUSE_MODE_LOAD_FILAMENT:
        return PSTR(MSG_FILAMENT_CHANGE_HEADER_LOAD);
      case ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT:
        return PSTR(MSG_FILAMENT_CHANGE_HEADER_UNLOAD);
      default: break;
    }
    return PSTR(MSG_FILAMENT_CHANGE_HEADER_PAUSE);
  }

  // Portions from STATIC_ITEM...
  #define HOTEND_STATUS_ITEM() do { \
    if (_menuLineNr == _thisItemNr) { \
      if (lcdDrawUpdate) { \
        lcd_implementation_drawmenu_static(_lcdLineNr, PSTR(MSG_FILAMENT_CHANGE_NOZZLE), false, true); \
        lcd_implementation_hotend_status(_lcdLineNr, hotend_status_extruder); \
      } \
      if (_skipStatic && encoderLine <= _thisItemNr) { \
        encoderPosition += ENCODER_STEPS_PER_MENU_ITEM; \
        ++encoderLine; \
      } \
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT; \
    } \
    ++_thisItemNr; \
  }while(0)

  void lcd_advanced_pause_resume_print() {
    advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_RESUME_PRINT;
  }

  void lcd_advanced_pause_extrude_more() {
    advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE;
  }

  void menu_advanced_pause_option() {
    START_MENU();
    #if LCD_HEIGHT > 2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_OPTION_HEADER, true, false);
    #endif
    MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_RESUME, lcd_advanced_pause_resume_print);
    MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_PURGE, lcd_advanced_pause_extrude_more);
    END_MENU();
  }

  void lcd_advanced_pause_init_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_1);
    #ifdef MSG_FILAMENT_CHANGE_INIT_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_2);
      #define __FC_LINES_A 3
    #else
      #define __FC_LINES_A 2
    #endif
    #ifdef MSG_FILAMENT_CHANGE_INIT_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_3);
      #define _FC_LINES_A (__FC_LINES_A + 1)
    #else
      #define _FC_LINES_A __FC_LINES_A
    #endif
    #if LCD_HEIGHT > _FC_LINES_A + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_unload_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_1);
    #ifdef MSG_FILAMENT_CHANGE_UNLOAD_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_2);
      #define __FC_LINES_B 3
    #else
      #define __FC_LINES_B 2
    #endif
    #ifdef MSG_FILAMENT_CHANGE_UNLOAD_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_3);
      #define _FC_LINES_B (__FC_LINES_B + 1)
    #else
      #define _FC_LINES_B __FC_LINES_B
    #endif
    #if LCD_HEIGHT > _FC_LINES_B + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_wait_for_nozzles_to_heat() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_HEATING_1);
    #ifdef MSG_FILAMENT_CHANGE_HEATING_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEATING_2);
      #define _FC_LINES_C 3
    #else
      #define _FC_LINES_C 2
    #endif
    #if LCD_HEIGHT > _FC_LINES_C + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_heat_nozzle() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_HEAT_1);
    #ifdef MSG_FILAMENT_CHANGE_INSERT_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEAT_2);
      #define _FC_LINES_D 3
    #else
      #define _FC_LINES_D 2
    #endif
    #if LCD_HEIGHT > _FC_LINES_D + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_insert_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_1);
    #ifdef MSG_FILAMENT_CHANGE_INSERT_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_2);
      #define __FC_LINES_E 3
    #else
      #define __FC_LINES_E 2
    #endif
    #ifdef MSG_FILAMENT_CHANGE_INSERT_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_3);
      #define _FC_LINES_E (__FC_LINES_E + 1)
    #else
      #define _FC_LINES_E __FC_LINES_E
    #endif
    #if LCD_HEIGHT > _FC_LINES_E + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_load_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_1);
    #ifdef MSG_FILAMENT_CHANGE_LOAD_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_2);
      #define __FC_LINES_F 3
    #else
      #define __FC_LINES_F 2
    #endif
    #ifdef MSG_FILAMENT_CHANGE_LOAD_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_3);
      #define _FC_LINES_F (__FC_LINES_F + 1)
    #else
      #define _FC_LINES_F __FC_LINES_F
    #endif
    #if LCD_HEIGHT > _FC_LINES_F + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  void lcd_advanced_pause_purge_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_1);
    #ifdef MSG_FILAMENT_CHANGE_PURGE_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_2);
      #define __FC_LINES_G 3
    #else
      #define __FC_LINES_G 2
    #endif
    #ifdef MSG_FILAMENT_CHANGE_PURGE_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_3);
      #define _FC_LINES_G (__FC_LINES_G + 1)
    #else
      #define _FC_LINES_G __FC_LINES_G
    #endif
    #if LCD_HEIGHT > _FC_LINES_G + 1
      STATIC_ITEM(" ");
    #endif
    HOTEND_STATUS_ITEM();
    END_SCREEN();
  }

  #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
    void menu_advanced_pause_continuous_purge() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_1);
      #ifdef MSG_FILAMENT_CHANGE_PURGE_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_2);
        #define __FC_LINES_G 3
      #else
        #define __FC_LINES_G 2
      #endif
      #ifdef MSG_FILAMENT_CHANGE_PURGE_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_3);
        #define _FC_LINES_G (__FC_LINES_G + 1)
      #else
        #define _FC_LINES_G __FC_LINES_G
      #endif
      #if LCD_HEIGHT > _FC_LINES_G + 1
        STATIC_ITEM(" ");
      #endif
      HOTEND_STATUS_ITEM();
      STATIC_ITEM(MSG_USERWAIT);
      END_SCREEN();
    }
  #endif

  void lcd_advanced_pause_resume_message() {
    START_SCREEN();
    STATIC_ITEM_P(advanced_pause_header(), true, true);
    STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_1);
    #ifdef MSG_FILAMENT_CHANGE_RESUME_2
      STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_2);
    #endif
    #ifdef MSG_FILAMENT_CHANGE_RESUME_3
      STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_3);
    #endif
    END_SCREEN();
  }

  FORCE_INLINE screenFunc_t ap_message_screen(const AdvancedPauseMessage message) {
    switch (message) {
      case ADVANCED_PAUSE_MESSAGE_INIT: return lcd_advanced_pause_init_message;
      case ADVANCED_PAUSE_MESSAGE_UNLOAD: return lcd_advanced_pause_unload_message;
      case ADVANCED_PAUSE_MESSAGE_INSERT: return lcd_advanced_pause_insert_message;
      case ADVANCED_PAUSE_MESSAGE_LOAD: return lcd_advanced_pause_load_message;
      case ADVANCED_PAUSE_MESSAGE_PURGE: return lcd_advanced_pause_purge_message;
      case ADVANCED_PAUSE_MESSAGE_RESUME: return lcd_advanced_pause_resume_message;
      case ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE: return lcd_advanced_pause_heat_nozzle;
      case ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT: return lcd_advanced_pause_wait_for_nozzles_to_heat;
      case ADVANCED_PAUSE_MESSAGE_OPTION: advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_WAIT_FOR;
                                          return menu_advanced_pause_option;
      #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
        case ADVANCED_PAUSE_MESSAGE_CONTINUOUS_PURGE: return menu_advanced_pause_continuous_purge;
      #endif
      case ADVANCED_PAUSE_MESSAGE_STATUS:
      default: break;
    }
    return NULL;
  }

  void lcd_advanced_pause_show_message(
    const AdvancedPauseMessage message,
    const AdvancedPauseMode mode/*=ADVANCED_PAUSE_MODE_PAUSE_PRINT*/,
    const uint8_t extruder/*=active_extruder*/
  ) {
    advanced_pause_mode = mode;
    hotend_status_extruder = extruder;
    const screenFunc_t next_screen = ap_message_screen(message);
    if (next_screen) {
      defer_return_to_status = true;
      lcd_goto_screen(next_screen);
    }
    else
      lcd_return_to_status();
  }

#endif // ADVANCED_PAUSE_FEATURE

#endif // ULTIPANEL
