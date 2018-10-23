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
#include "../../module/stepper.h"
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

#if ENABLED(FILAMENT_LCD_DISPLAY)
  #include "../../feature/filwidth.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../module/endstops.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(LED_CONTROL_MENU)
  #include "../../feature/leds/leds.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../feature/runout.h"
#endif

////////////////////////////////////////////
///////////////// Externs //////////////////
////////////////////////////////////////////

// LCD Updates
#if HAS_GRAPHICAL_LCD
  extern bool drawing_screen;
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

#if ENABLED(PIDTEMP)
  float raw_Ki, raw_Kd; // place-holders for Ki and Kd edits
#endif

#ifndef TALL_FONT_CORRECTION
  #define TALL_FONT_CORRECTION 0
#endif

bool no_reentry = false;
constexpr int8_t menu_bottom = LCD_HEIGHT - (TALL_FONT_CORRECTION);

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

void menu_tune();
void menu_movement();
void menu_configuration();
void menu_temperature();
void menu_advanced_settings();

#if DISABLED(SLIM_LCD_MENUS)
  void menu_preheat_material1_settings();
  void menu_preheat_material2_settings();
#endif

#if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
  void menu_advanced_filament();
#endif

#if ENABLED(LCD_INFO_MENU)
  #if ENABLED(PRINTCOUNTER)
    void menu_info_stats();
  #endif
  void menu_info_thermistors();
  void menu_info_board();
  void menu_info();
#endif

#if ENABLED(LED_CONTROL_MENU)
  void menu_led();
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void menu_change_filament();
  #else
    void menu_temp_e0_filament_change();
  #endif
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  void dac_driver_commit();
  void dac_driver_getValues();
  void menu_dac();
  void lcd_dac_write_eeprom();
#endif

#if ENABLED(FWRETRACT)
  #include "../../feature/fwretract.h"
  void menu_config_retract();
#endif

#if ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION)
  void menu_delta_calibrate();
#endif

#if ENABLED(MESH_BED_LEVELING) && ENABLED(LCD_BED_LEVELING)
  #include "../../feature/bedlevel/mbl/mesh_bed_leveling.h"
#endif

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float lcd_z_fade_height;
  void _lcd_set_z_fade_height() { set_z_fade_height(lcd_z_fade_height); }
#endif

#if ENABLED(BABYSTEPPING)
  long babysteps_done = 0;
#endif

bool printer_busy() { return planner.movesplanned() || IS_SD_PRINTING(); }

float move_menu_scale;

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

extern millis_t manual_move_start_time;
extern int8_t manual_move_axis;

#if IS_KINEMATIC
  extern float manual_move_offset;
#endif

#if ENABLED(DUAL_X_CARRIAGE) || E_MANUAL > 1
  extern int8_t manual_move_e_index;
#endif

#if ENABLED(MANUAL_E_MOVES_RELATIVE)
  float manual_move_e_origin = 0;
#endif


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
    if (encoderLine >= encoderTopLine + menu_bottom)
      encoderTopLine = encoderLine - menu_bottom + 1;
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

#if ENABLED(SDSUPPORT)

  void lcd_sdcard_pause() {
    card.pauseSDPrint();
    print_job_timer.pause();
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M125"));
    #endif
    lcd_reset_status();
  }

  void lcd_sdcard_resume() {
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M24"));
    #else
      card.startFileprint();
      print_job_timer.start();
    #endif
    lcd_reset_status();
  }

  void lcd_sdcard_stop() {
    wait_for_heatup = wait_for_user = false;
    card.abort_sd_printing = true;
    lcd_setstatusPGM(PSTR(MSG_PRINT_ABORTED), -1);
    lcd_return_to_status();
  }

#endif // SDSUPPORT

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

#if EXTRUDERS > 1
  void menu_tool_change() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    #if ENABLED(SINGLENOZZLE)
      MENU_ITEM_EDIT(float3, MSG_FILAMENT_SWAP_LENGTH, &toolchange_settings.swap_length, 0, 200);
      MENU_MULTIPLIER_ITEM_EDIT(int4, MSG_SINGLENOZZLE_RETRACT_SPD, &toolchange_settings.retract_speed, 10, 5400);
      MENU_MULTIPLIER_ITEM_EDIT(int4, MSG_SINGLENOZZLE_PRIME_SPD, &toolchange_settings.prime_speed, 10, 5400);
    #endif
    MENU_ITEM_EDIT(float3, MSG_TOOL_CHANGE_ZLIFT, &toolchange_settings.z_raise, 0, 10);
    END_MENU();
  }
#endif

#if ENABLED(MENU_ITEM_CASE_LIGHT)

  #include "../../feature/caselight.h"

  void menu_case_light() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_CASE_LIGHT_BRIGHTNESS, &case_light_brightness, 0, 255, update_case_light, true);
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
    END_MENU();
  }
#endif // MENU_ITEM_CASE_LIGHT

#if ENABLED(BLTOUCH)

  /**
   *
   * "BLTouch" submenu
   *
   */
  static void menu_bltouch() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    MENU_ITEM(gcode, MSG_BLTOUCH_RESET, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_RESET)));
    MENU_ITEM(gcode, MSG_BLTOUCH_SELFTEST, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_SELFTEST)));
    MENU_ITEM(gcode, MSG_BLTOUCH_DEPLOY, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_DEPLOY)));
    MENU_ITEM(gcode, MSG_BLTOUCH_STOW, PSTR("M280 P" STRINGIFY(Z_PROBE_SERVO_NR) " S" STRINGIFY(BLTOUCH_STOW)));
    END_MENU();
  }

#endif // BLTOUCH

#if ENABLED(LCD_PROGRESS_BAR_TEST)

  static void progress_bar_test() {
    static int8_t bar_percent = 0;
    if (use_click()) {
      lcd_goto_previous_menu();
      lcd_set_custom_characters(CHARSET_MENU);
      return;
    }
    bar_percent += (int8_t)encoderPosition;
    bar_percent = constrain(bar_percent, 0, 100);
    encoderPosition = 0;
    lcd_implementation_drawmenu_static(0, PSTR(MSG_PROGRESS_BAR_TEST), true, true);
    lcd_moveto((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2);
    lcd_put_u8str(int(bar_percent)); lcd_put_wchar('%');
    lcd_moveto(0, LCD_HEIGHT - 1); lcd_draw_progress_bar(bar_percent);
  }

  void _progress_bar_test() {
    lcd_goto_screen(progress_bar_test);
    lcd_set_custom_characters();
  }

#endif // LCD_PROGRESS_BAR_TEST

#if HAS_DEBUG_MENU

  void menu_debug() {
    START_MENU();

    MENU_BACK(MSG_MAIN);

    #if ENABLED(LCD_PROGRESS_BAR_TEST)
      MENU_ITEM(submenu, MSG_PROGRESS_BAR_TEST, _progress_bar_test);
    #endif

    END_MENU();
  }

#endif // HAS_DEBUG_MENU

/**
 * IDEX submenu
 */
#if ENABLED(DUAL_X_CARRIAGE)
  static void _recalc_IDEX_settings() {
    if (active_extruder) {                      // For the 2nd extruder re-home so the next tool-change gets the new offsets.
      enqueue_and_echo_commands_P(PSTR("G28")); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
      active_extruder = 0;
    }
  }

  static void menu_IDEX() {
    START_MENU();
    MENU_BACK(MSG_MAIN);

    MENU_ITEM(gcode, MSG_IDEX_MODE_AUTOPARK,  PSTR("M605 S1\nG28 X\nG1 X100"));
    const bool need_g28 = !(TEST(axis_known_position, Y_AXIS) && TEST(axis_known_position, Z_AXIS));
    MENU_ITEM(gcode, MSG_IDEX_MODE_DUPLICATE, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100")                // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100")
    );
    //MENU_ITEM(gcode, MSG_IDEX_MODE_SCALED_COPY, need_g28
    //  ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")  // If Y or Z is not homed, do a full G28 first
    //  : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")
    //);
    MENU_ITEM(gcode, MSG_IDEX_MODE_FULL_CTRL, PSTR("M605 S0\nG28 X"));
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52, MSG_IDEX_X_OFFSET , &hotend_offset[X_AXIS][1], MIN(X2_HOME_POS, X2_MAX_POS) - 25.0, MAX(X2_HOME_POS, X2_MAX_POS) + 25.0, _recalc_IDEX_settings);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52, MSG_IDEX_Y_OFFSET , &hotend_offset[Y_AXIS][1], -10.0, 10.0, _recalc_IDEX_settings);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float52, MSG_IDEX_Z_OFFSET , &hotend_offset[Z_AXIS][1], -10.0, 10.0, _recalc_IDEX_settings);
    MENU_ITEM(gcode, MSG_IDEX_SAVE_OFFSETS, PSTR("M500"));
    END_MENU();
  }
#endif // DUAL_X_CARRIAGE

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

/**
 *
 * "Main" menu
 *
 */

#if ENABLED(SDSUPPORT) && ENABLED(MENU_ADDAUTOSTART)

  void lcd_autostart_sd() { card.beginautostart(); }

#endif

void menu_main() {
  START_MENU();
  MENU_BACK(MSG_WATCH);

  #if ENABLED(SDSUPPORT)
    if (card.cardOK) {
      if (card.isFileOpen()) {
        if (card.sdprinting)
          MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause);
        else
          MENU_ITEM(function, MSG_RESUME_PRINT, lcd_sdcard_resume);
        MENU_ITEM(function, MSG_STOP_PRINT, lcd_sdcard_stop);
      }
      else {
        MENU_ITEM(submenu, MSG_CARD_MENU, menu_sdcard);
        #if !PIN_EXISTS(SD_DETECT)
          MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
        #endif
      }
    }
    else {
      MENU_ITEM(submenu, MSG_NO_CARD, menu_sdcard);
      #if !PIN_EXISTS(SD_DETECT)
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
      #endif
    }
  #endif // SDSUPPORT

  const bool busy = printer_busy();
  if (busy)
    MENU_ITEM(submenu, MSG_TUNE, menu_tune);
  else {
    MENU_ITEM(submenu, MSG_MOTION, menu_movement);
    MENU_ITEM(submenu, MSG_TEMPERATURE, menu_temperature);
  }

  MENU_ITEM(submenu, MSG_CONFIGURATION, menu_configuration);

  #if ENABLED(CUSTOM_USER_MENUS)
    MENU_ITEM(submenu, MSG_USER_MENU, _menu_user);
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #if E_STEPPERS == 1 && DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      if (thermalManager.targetHotEnoughToExtrude(active_extruder))
        MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600 B0"));
      else
        MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_temp_e0_filament_change);
    #else
      MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_change_filament);
    #endif
  #endif

  #if ENABLED(LCD_INFO_MENU)
    MENU_ITEM(submenu, MSG_INFO_MENU, menu_info);
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    MENU_ITEM(submenu, MSG_LED_CONTROL, menu_led);
  #endif

  //
  // Switch power on/off
  //
  #if HAS_POWER_SWITCH
    if (powersupply_on)
      MENU_ITEM(gcode, MSG_SWITCH_PS_OFF, PSTR("M81"));
    else
      MENU_ITEM(gcode, MSG_SWITCH_PS_ON, PSTR("M80"));
  #endif

  //
  // Autostart
  //
  #if ENABLED(SDSUPPORT) && ENABLED(MENU_ADDAUTOSTART)
    if (!busy)
      MENU_ITEM(function, MSG_AUTOSTART, lcd_autostart_sd);
  #endif

  END_MENU();
}

/**
 *
 * "Tune" submenu items
 *
 */

#if ENABLED(BABYSTEPPING)

  void _lcd_babystep(const AxisEnum axis, PGM_P msg) {
    if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
    ENCODER_DIRECTION_NORMAL();
    if (encoderPosition) {
      const int16_t babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR);
      encoderPosition = 0;
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
      thermalManager.babystep_axis(axis, babystep_increment);
      babysteps_done += babystep_increment;
    }
    if (lcdDrawUpdate)
      lcd_implementation_drawedit(msg, ftostr43sign(planner.steps_to_mm[axis] * babysteps_done));
  }

  #if ENABLED(BABYSTEP_XY)
    void _lcd_babystep_x() { _lcd_babystep(X_AXIS, PSTR(MSG_BABYSTEP_X)); }
    void _lcd_babystep_y() { _lcd_babystep(Y_AXIS, PSTR(MSG_BABYSTEP_Y)); }
    void lcd_babystep_x() { lcd_goto_screen(_lcd_babystep_x); babysteps_done = 0; defer_return_to_status = true; }
    void lcd_babystep_y() { lcd_goto_screen(_lcd_babystep_y); babysteps_done = 0; defer_return_to_status = true; }
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

  #else // !BABYSTEP_ZPROBE_OFFSET

    void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEP_Z)); }
    void lcd_babystep_z() { lcd_goto_screen(_lcd_babystep_z); babysteps_done = 0; defer_return_to_status = true; }

  #endif // !BABYSTEP_ZPROBE_OFFSET

#endif // BABYSTEPPING

#if ENABLED(AUTO_BED_LEVELING_UBL)

  float mesh_edit_value, mesh_edit_accumulator; // We round mesh_edit_value to 2.5 decimal places. So we keep a
                                                // separate value that doesn't lose precision.
  static int16_t ubl_encoderPosition = 0;

  static void _lcd_mesh_fine_tune(PGM_P msg) {
    defer_return_to_status = true;
    if (ubl.encoder_diff) {
      ubl_encoderPosition = (ubl.encoder_diff > 0) ? 1 : -1;
      ubl.encoder_diff = 0;

      mesh_edit_accumulator += float(ubl_encoderPosition) * 0.005f * 0.5f;
      mesh_edit_value = mesh_edit_accumulator;
      encoderPosition = 0;
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;

      const int32_t rounded = (int32_t)(mesh_edit_value * 1000);
      mesh_edit_value = float(rounded - (rounded % 5L)) / 1000;
    }

    if (lcdDrawUpdate) {
      lcd_implementation_drawedit(msg, ftostr43sign(mesh_edit_value));
      #if ENABLED(MESH_EDIT_GFX_OVERLAY)
        _lcd_zoffset_overlay_gfx(mesh_edit_value);
      #endif
    }
  }

  void _lcd_mesh_edit_NOP() {
    defer_return_to_status = true;
  }

  float lcd_mesh_edit() {
    lcd_goto_screen(_lcd_mesh_edit_NOP);
    lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
    _lcd_mesh_fine_tune(PSTR("Mesh Editor"));
    return mesh_edit_value;
  }

  void lcd_mesh_edit_setup(const float &initial) {
    mesh_edit_value = mesh_edit_accumulator = initial;
    lcd_goto_screen(_lcd_mesh_edit_NOP);
  }

  void _lcd_z_offset_edit() {
    _lcd_mesh_fine_tune(PSTR("Z-Offset: "));
  }

  float lcd_z_offset_edit() {
    lcd_goto_screen(_lcd_z_offset_edit);
    return mesh_edit_value;
  }

  void lcd_z_offset_edit_setup(const float &initial) {
    mesh_edit_value = mesh_edit_accumulator = initial;
    lcd_goto_screen(_lcd_z_offset_edit);
  }

#endif // AUTO_BED_LEVELING_UBL


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

// Refresh the E factor after changing flow
inline void _lcd_refresh_e_factor_0() { planner.refresh_e_factor(0); }
#if EXTRUDERS > 1
  inline void _lcd_refresh_e_factor() { planner.refresh_e_factor(active_extruder); }
  inline void _lcd_refresh_e_factor_1() { planner.refresh_e_factor(1); }
  #if EXTRUDERS > 2
    inline void _lcd_refresh_e_factor_2() { planner.refresh_e_factor(2); }
    #if EXTRUDERS > 3
      inline void _lcd_refresh_e_factor_3() { planner.refresh_e_factor(3); }
      #if EXTRUDERS > 4
        inline void _lcd_refresh_e_factor_4() { planner.refresh_e_factor(4); }
        #if EXTRUDERS > 5
          inline void _lcd_refresh_e_factor_5() { planner.refresh_e_factor(5); }
        #endif // EXTRUDERS > 5
      #endif // EXTRUDERS > 4
    #endif // EXTRUDERS > 3
  #endif // EXTRUDERS > 2
#endif // EXTRUDERS > 1

/**
 *
 * "Tune" submenu
 *
 */
void menu_tune() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  //
  // Speed:
  //
  MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999);

  //
  // Manual bed leveling, Bed Z:
  //
  #if ENABLED(MESH_BED_LEVELING) && ENABLED(LCD_BED_LEVELING)
    MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
  #endif

  //
  // Nozzle:
  // Nozzle [1-4]:
  //
  #if HOTENDS == 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
  #else // HOTENDS > 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N1, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N2, &thermalManager.target_temperature[1], 0, HEATER_1_MAXTEMP - 15, watch_temp_callback_E1);
    #if HOTENDS > 2
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N3, &thermalManager.target_temperature[2], 0, HEATER_2_MAXTEMP - 15, watch_temp_callback_E2);
      #if HOTENDS > 3
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N4, &thermalManager.target_temperature[3], 0, HEATER_3_MAXTEMP - 15, watch_temp_callback_E3);
        #if HOTENDS > 4
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N5, &thermalManager.target_temperature[4], 0, HEATER_4_MAXTEMP - 15, watch_temp_callback_E4);
          #if HOTENDS > 5
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N6, &thermalManager.target_temperature[5], 0, HEATER_5_MAXTEMP - 15, watch_temp_callback_E5);
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.target_temperature_bed, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
  #endif

  //
  // Fan Speed:
  //
  #if FAN_COUNT > 0
    #if HAS_FAN0
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED FAN_SPEED_1_SUFFIX, &fan_speed[0], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED FAN_SPEED_1_SUFFIX, &new_fan_speed[0], 3, 255);
      #endif
    #endif
    #if HAS_FAN1
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED " 2", &fan_speed[1], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED " 2", &new_fan_speed[1], 3, 255);
      #endif
    #endif
    #if HAS_FAN2
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED " 3", &fan_speed[2], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED " 3", &new_fan_speed[2], 3, 255);
      #endif
    #endif
  #endif // FAN_COUNT > 0

  //
  // Flow:
  // Flow [1-5]:
  //
  #if EXTRUDERS == 1
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW, &planner.flow_percentage[0], 10, 999, _lcd_refresh_e_factor_0);
  #else // EXTRUDERS > 1
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW, &planner.flow_percentage[active_extruder], 10, 999, _lcd_refresh_e_factor);
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N1, &planner.flow_percentage[0], 10, 999, _lcd_refresh_e_factor_0);
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N2, &planner.flow_percentage[1], 10, 999, _lcd_refresh_e_factor_1);
    #if EXTRUDERS > 2
      MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N3, &planner.flow_percentage[2], 10, 999, _lcd_refresh_e_factor_2);
      #if EXTRUDERS > 3
        MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N4, &planner.flow_percentage[3], 10, 999, _lcd_refresh_e_factor_3);
        #if EXTRUDERS > 4
          MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N5, &planner.flow_percentage[4], 10, 999, _lcd_refresh_e_factor_4);
          #if EXTRUDERS > 5
            MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_N6, &planner.flow_percentage[5], 10, 999, _lcd_refresh_e_factor_5);
          #endif // EXTRUDERS > 5
        #endif // EXTRUDERS > 4
      #endif // EXTRUDERS > 3
    #endif // EXTRUDERS > 2
  #endif // EXTRUDERS > 1

  //
  // Babystep X:
  // Babystep Y:
  // Babystep Z:
  //
  #if ENABLED(BABYSTEPPING)
    #if ENABLED(BABYSTEP_XY)
      MENU_ITEM(submenu, MSG_BABYSTEP_X, lcd_babystep_x);
      MENU_ITEM(submenu, MSG_BABYSTEP_Y, lcd_babystep_y);
    #endif
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    #else
      MENU_ITEM(submenu, MSG_BABYSTEP_Z, lcd_babystep_z);
    #endif
  #endif

  END_MENU();
}

/**
 *
 * "Driver current control" submenu items
 *
 */
#if ENABLED(DAC_STEPPER_CURRENT)

  #include "../../feature/dac/stepper_dac.h" //was dac_mcp4728.h MarlinMain uses stepper dac for the m-codes
  uint8_t driverPercent[XYZE];

  void dac_driver_getValues() { LOOP_XYZE(i) driverPercent[i] = dac_current_get_percent((AxisEnum)i); }

  void dac_driver_commit() { dac_current_set_percents(driverPercent); }

  void dac_driver_eeprom_write() { dac_commit_eeprom(); }

  void menu_dac() {
    dac_driver_getValues();
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_X " " MSG_DAC_PERCENT, &driverPercent[X_AXIS], 0, 100, dac_driver_commit);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_Y " " MSG_DAC_PERCENT, &driverPercent[Y_AXIS], 0, 100, dac_driver_commit);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_Z " " MSG_DAC_PERCENT, &driverPercent[Z_AXIS], 0, 100, dac_driver_commit);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_E " " MSG_DAC_PERCENT, &driverPercent[E_AXIS], 0, 100, dac_driver_commit);
    MENU_ITEM(function, MSG_DAC_EEPROM_WRITE, dac_driver_eeprom_write);
    END_MENU();
  }

#endif // DAC_STEPPER_CURRENT

#if HAS_MOTOR_CURRENT_PWM

  void menu_pwm() {
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_X MSG_Y, &stepper.motor_current_setting[0], 100, 2000, Stepper::refresh_motor_power);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_Z, &stepper.motor_current_setting[1], 100, 2000, Stepper::refresh_motor_power);
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_E, &stepper.motor_current_setting[2], 100, 2000, Stepper::refresh_motor_power);
    #endif
    END_MENU();
  }

#endif // HAS_MOTOR_CURRENT_PWM

/**
 *
 * "Temperature" submenu items
 *
 */
void _lcd_preheat(const int16_t endnum, const int16_t temph, const int16_t tempb, const uint8_t fan) {
  if (temph > 0) thermalManager.setTargetHotend(MIN(heater_maxtemp[endnum], temph), endnum);
  #if HAS_HEATED_BED
    if (tempb >= 0) thermalManager.setTargetBed(tempb);
  #else
    UNUSED(tempb);
  #endif
  #if FAN_COUNT > 0
    #if FAN_COUNT > 1
      fan_speed[active_extruder < FAN_COUNT ? active_extruder : 0] = fan;
    #else
      fan_speed[0] = fan;
    #endif
  #else
    UNUSED(fan);
  #endif
  lcd_return_to_status();
}

#if HAS_TEMP_HOTEND
  void lcd_preheat_m1_e0_only() { _lcd_preheat(0, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
  void lcd_preheat_m2_e0_only() { _lcd_preheat(0, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
  #if HAS_HEATED_BED
    void lcd_preheat_m1_e0() { _lcd_preheat(0, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
    void lcd_preheat_m2_e0() { _lcd_preheat(0, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
  #endif
#endif

#if HOTENDS > 1
  void lcd_preheat_m1_e1_only() { _lcd_preheat(1, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
  void lcd_preheat_m2_e1_only() { _lcd_preheat(1, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
  #if HAS_HEATED_BED
    void lcd_preheat_m1_e1() { _lcd_preheat(1, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
    void lcd_preheat_m2_e1() { _lcd_preheat(1, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
  #endif
  #if HOTENDS > 2
    void lcd_preheat_m1_e2_only() { _lcd_preheat(2, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
    void lcd_preheat_m2_e2_only() { _lcd_preheat(2, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
    #if HAS_HEATED_BED
      void lcd_preheat_m1_e2() { _lcd_preheat(2, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
      void lcd_preheat_m2_e2() { _lcd_preheat(2, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
    #endif
    #if HOTENDS > 3
      void lcd_preheat_m1_e3_only() { _lcd_preheat(3, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
      void lcd_preheat_m2_e3_only() { _lcd_preheat(3, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
      #if HAS_HEATED_BED
        void lcd_preheat_m1_e3() { _lcd_preheat(3, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
        void lcd_preheat_m2_e3() { _lcd_preheat(3, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
      #endif
      #if HOTENDS > 4
        void lcd_preheat_m1_e4_only() { _lcd_preheat(4, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
        void lcd_preheat_m2_e4_only() { _lcd_preheat(4, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
        #if HAS_HEATED_BED
          void lcd_preheat_m1_e4() { _lcd_preheat(4, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
          void lcd_preheat_m2_e4() { _lcd_preheat(4, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
        #endif
        #if HOTENDS > 5
          void lcd_preheat_m1_e5_only() { _lcd_preheat(5, lcd_preheat_hotend_temp[0], -1, lcd_preheat_fan_speed[0]); }
          void lcd_preheat_m2_e5_only() { _lcd_preheat(5, lcd_preheat_hotend_temp[1], -1, lcd_preheat_fan_speed[1]); }
          #if HAS_HEATED_BED
            void lcd_preheat_m1_e5() { _lcd_preheat(5, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
            void lcd_preheat_m2_e5() { _lcd_preheat(5, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
          #endif
        #endif // HOTENDS > 5
      #endif // HOTENDS > 4
    #endif // HOTENDS > 3
  #endif // HOTENDS > 2

  void lcd_preheat_m1_all() {
    #if HOTENDS > 1
      thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 1);
      #if HOTENDS > 2
        thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 2);
        #if HOTENDS > 3
          thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 3);
          #if HOTENDS > 4
            thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 4);
            #if HOTENDS > 5
              thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      lcd_preheat_m1_e0();
    #else
      lcd_preheat_m1_e0_only();
    #endif
  }
  void lcd_preheat_m2_all() {
    #if HOTENDS > 1
      thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 1);
      #if HOTENDS > 2
        thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 2);
        #if HOTENDS > 3
          thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 3);
          #if HOTENDS > 4
            thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 4);
            #if HOTENDS > 5
              thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      lcd_preheat_m2_e0();
    #else
      lcd_preheat_m2_e0_only();
    #endif
  }

#endif // HOTENDS > 1

#if HAS_HEATED_BED
  void lcd_preheat_m1_bedonly() { _lcd_preheat(0, 0, lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
  void lcd_preheat_m2_bedonly() { _lcd_preheat(0, 0, lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
#endif

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  void menu_preheat_m1() {
    START_MENU();
    MENU_BACK(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0);
        MENU_ITEM(function, MSG_PREHEAT_1_END, lcd_preheat_m1_e0_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0_only);
      #endif
    #elif HOTENDS > 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H1, lcd_preheat_m1_e0);
        MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E1, lcd_preheat_m1_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H2, lcd_preheat_m1_e1);
        MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E2, lcd_preheat_m1_e1_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H1, lcd_preheat_m1_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H2, lcd_preheat_m1_e1_only);
      #endif
      #if HOTENDS > 2
        #if HAS_HEATED_BED
          MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H3, lcd_preheat_m1_e2);
          MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E3, lcd_preheat_m1_e2_only);
        #else
          MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H3, lcd_preheat_m1_e2_only);
        #endif
        #if HOTENDS > 3
          #if HAS_HEATED_BED
            MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H4, lcd_preheat_m1_e3);
            MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E4, lcd_preheat_m1_e3_only);
          #else
            MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H4, lcd_preheat_m1_e3_only);
          #endif
          #if HOTENDS > 4
            #if HAS_HEATED_BED
              MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H5, lcd_preheat_m1_e4);
              MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E5, lcd_preheat_m1_e4_only);
            #else
              MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H5, lcd_preheat_m1_e4_only);
            #endif
            #if HOTENDS > 5
              #if HAS_HEATED_BED
                MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H6, lcd_preheat_m1_e5);
                MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E6, lcd_preheat_m1_e5_only);
              #else
                MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H6, lcd_preheat_m1_e5_only);
              #endif
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
      MENU_ITEM(function, MSG_PREHEAT_1_ALL, lcd_preheat_m1_all);
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      MENU_ITEM(function, MSG_PREHEAT_1_BEDONLY, lcd_preheat_m1_bedonly);
    #endif
    END_MENU();
  }

  void menu_preheat_m2() {
    START_MENU();
    MENU_BACK(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0);
        MENU_ITEM(function, MSG_PREHEAT_2_END, lcd_preheat_m2_e0_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0_only);
      #endif
    #elif HOTENDS > 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H1, lcd_preheat_m2_e0);
        MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E1, lcd_preheat_m2_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H2, lcd_preheat_m2_e1);
        MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E2, lcd_preheat_m2_e1_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H1, lcd_preheat_m2_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H2, lcd_preheat_m2_e1_only);
      #endif
      #if HOTENDS > 2
        #if HAS_HEATED_BED
          MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H3, lcd_preheat_m2_e2);
          MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E3, lcd_preheat_m2_e2_only);
        #else
          MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H3, lcd_preheat_m2_e2_only);
        #endif
        #if HOTENDS > 3
          #if HAS_HEATED_BED
            MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H4, lcd_preheat_m2_e3);
            MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E4, lcd_preheat_m2_e3_only);
          #else
            MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H4, lcd_preheat_m2_e3_only);
          #endif
          #if HOTENDS > 4
            #if HAS_HEATED_BED
              MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H5, lcd_preheat_m2_e4);
              MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E5, lcd_preheat_m2_e4_only);
            #else
              MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H5, lcd_preheat_m2_e4_only);
            #endif
            #if HOTENDS > 5
              #if HAS_HEATED_BED
                MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H6, lcd_preheat_m2_e5);
                MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E6, lcd_preheat_m2_e5_only);
              #else
                MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H6, lcd_preheat_m2_e5_only);
              #endif
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
      MENU_ITEM(function, MSG_PREHEAT_2_ALL, lcd_preheat_m2_all);
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      MENU_ITEM(function, MSG_PREHEAT_2_BEDONLY, lcd_preheat_m2_bedonly);
    #endif
    END_MENU();
  }

#endif // HAS_TEMP_HOTEND || HAS_HEATED_BED

void lcd_cooldown() {
  zero_fan_speeds();
  thermalManager.disable_all_heaters();
  lcd_return_to_status();
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
  static void lcd_store_settings()   { lcd_completion_feedback(settings.save()); }
  static void lcd_load_settings()    { lcd_completion_feedback(settings.load()); }
#endif

void _lcd_draw_homing() {
  constexpr uint8_t line = (LCD_HEIGHT - 1) / 2;
  if (lcdDrawUpdate) lcd_implementation_drawmenu_static(line, PSTR(MSG_LEVEL_BED_HOMING));
  lcdDrawUpdate = LCDVIEW_CALL_NO_REDRAW;
}

#if ENABLED(LEVEL_BED_CORNERS)

  /**
   * Level corners, starting in the front-left corner.
   */
  static int8_t bed_corner;
  void _lcd_goto_next_corner() {
    line_to_z(4.0);
    switch (bed_corner) {
      case 0:
        current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
        current_position[Y_AXIS] = Y_MIN_BED + LEVEL_CORNERS_INSET;
        break;
      case 1:
        current_position[X_AXIS] = X_MAX_BED - LEVEL_CORNERS_INSET;
        break;
      case 2:
        current_position[Y_AXIS] = Y_MAX_BED - LEVEL_CORNERS_INSET;
        break;
      case 3:
        current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
        break;
      #if ENABLED(LEVEL_CENTER_TOO)
        case 4:
          current_position[X_AXIS] = X_CENTER;
          current_position[Y_AXIS] = Y_CENTER;
          break;
      #endif
    }
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[X_AXIS]), active_extruder);
    line_to_z(0.0);
    if (++bed_corner > 3
      #if ENABLED(LEVEL_CENTER_TOO)
        + 1
      #endif
    ) bed_corner = 0;
  }

  void _lcd_corner_submenu() {
    START_MENU();
    MENU_ITEM(function,
      #if ENABLED(LEVEL_CENTER_TOO)
        MSG_LEVEL_BED_NEXT_POINT
      #else
        MSG_NEXT_CORNER
      #endif
      , _lcd_goto_next_corner);
    MENU_ITEM(function, MSG_BACK, lcd_goto_previous_menu_no_defer);
    END_MENU();
  }

  void _lcd_level_bed_corners_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) {
      bed_corner = 0;
      lcd_goto_screen(_lcd_corner_submenu);
      _lcd_goto_next_corner();
    }
  }

  void _lcd_level_bed_corners() {
    defer_return_to_status = true;
    if (!all_axes_known()) {
      axis_homed = 0;
      enqueue_and_echo_commands_P(PSTR("G28"));
    }
    lcd_goto_screen(_lcd_level_bed_corners_homing);
  }

#endif // LEVEL_BED_CORNERS

#if ENABLED(LCD_BED_LEVELING) && (ENABLED(PROBE_MANUALLY) || ENABLED(MESH_BED_LEVELING))

  /**
   *
   * "Motion" > "Level Bed" handlers
   *
   */

  static uint8_t manual_probe_index;

  // LCD probed points are from defaults
  constexpr uint8_t total_probe_points = (
    #if ENABLED(AUTO_BED_LEVELING_3POINT)
      3
    #elif ABL_GRID || ENABLED(MESH_BED_LEVELING)
      GRID_MAX_POINTS
    #endif
  );

  bool lcd_wait_for_move;

  //
  // Bed leveling is done. Wait for G29 to complete.
  // A flag is used so that this can release control
  // and allow the command queue to be processed.
  //
  // When G29 finishes the last move:
  // - Raise Z to the "manual probe height"
  // - Don't return until done.
  //
  // ** This blocks the command queue! **
  //
  void _lcd_level_bed_done() {
    if (!lcd_wait_for_move) {
      #if MANUAL_PROBE_HEIGHT > 0 && DISABLED(MESH_BED_LEVELING)
        // Display "Done" screen and wait for moves to complete
        line_to_z(MANUAL_PROBE_HEIGHT);
        lcd_synchronize(PSTR(MSG_LEVEL_BED_DONE));
      #endif
      lcd_goto_previous_menu_no_defer();
      lcd_completion_feedback();
    }
    if (lcdDrawUpdate) lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, PSTR(MSG_LEVEL_BED_DONE));
    lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
  }

  void _lcd_level_goto_next_point();

  /**
   * Step 7: Get the Z coordinate, click goes to the next point or exits
   */
  void _lcd_level_bed_get_z() {
    ENCODER_DIRECTION_NORMAL();

    if (use_click()) {

      //
      // Save the current Z position and move
      //

      // If done...
      if (++manual_probe_index >= total_probe_points) {
        //
        // The last G29 records the point and enables bed leveling
        //
        lcd_wait_for_move = true;
        lcd_goto_screen(_lcd_level_bed_done);
        #if ENABLED(MESH_BED_LEVELING)
          enqueue_and_echo_commands_P(PSTR("G29 S2"));
        #elif ENABLED(PROBE_MANUALLY)
          enqueue_and_echo_commands_P(PSTR("G29 V1"));
        #endif
      }
      else
        _lcd_level_goto_next_point();

      return;
    }

    //
    // Encoder knob or keypad buttons adjust the Z position
    //
    if (encoderPosition) {
      const float z = current_position[Z_AXIS] + float((int32_t)encoderPosition) * (MBL_Z_STEP);
      line_to_z(constrain(z, -(LCD_PROBE_Z_RANGE) * 0.5f, (LCD_PROBE_Z_RANGE) * 0.5f));
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
      encoderPosition = 0;
    }

    //
    // Draw on first display, then only on Z change
    //
    if (lcdDrawUpdate) {
      const float v = current_position[Z_AXIS];
      lcd_implementation_drawedit(PSTR(MSG_MOVE_Z), ftostr43sign(v + (v < 0 ? -0.0001f : 0.0001f), '+'));
    }
  }

  /**
   * Step 6: Display "Next point: 1 / 9" while waiting for move to finish
   */
  void _lcd_level_bed_moving() {
    if (lcdDrawUpdate) {
      char msg[10];
      sprintf_P(msg, PSTR("%i / %u"), (int)(manual_probe_index + 1), total_probe_points);
      lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_NEXT_POINT), msg);
    }
    lcdDrawUpdate = LCDVIEW_CALL_NO_REDRAW;
    if (!lcd_wait_for_move) lcd_goto_screen(_lcd_level_bed_get_z);
  }

  /**
   * Step 5: Initiate a move to the next point
   */
  void _lcd_level_goto_next_point() {
    lcd_goto_screen(_lcd_level_bed_moving);

    // G29 Records Z, moves, and signals when it pauses
    lcd_wait_for_move = true;
    #if ENABLED(MESH_BED_LEVELING)
      enqueue_and_echo_commands_P(manual_probe_index ? PSTR("G29 S2") : PSTR("G29 S1"));
    #elif ENABLED(PROBE_MANUALLY)
      enqueue_and_echo_commands_P(PSTR("G29 V1"));
    #endif
  }

  /**
   * Step 4: Display "Click to Begin", wait for click
   *         Move to the first probe position
   */
  void _lcd_level_bed_homing_done() {
    if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_WAITING));
    if (use_click()) {
      manual_probe_index = 0;
      _lcd_level_goto_next_point();
    }
  }

  /**
   * Step 3: Display "Homing XYZ" - Wait for homing to finish
   */
  void _lcd_level_bed_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) lcd_goto_screen(_lcd_level_bed_homing_done);
  }

  #if ENABLED(PROBE_MANUALLY)
    extern bool g29_in_progress;
  #endif

  /**
   * Step 2: Continue Bed Leveling...
   */
  void _lcd_level_bed_continue() {
    defer_return_to_status = true;
    axis_homed = 0;
    lcd_goto_screen(_lcd_level_bed_homing);
    enqueue_and_echo_commands_P(PSTR("G28"));
  }

#elif ENABLED(AUTO_BED_LEVELING_UBL)

  void _lcd_ubl_level_bed();

  static int16_t ubl_storage_slot = 0,
             custom_hotend_temp = 190,
             side_points = 3,
             ubl_fillin_amount = 5,
             ubl_height_amount = 1,
             n_edit_pts = 1,
             x_plot = 0,
             y_plot = 0;

  #if HAS_HEATED_BED
    static int16_t custom_bed_temp = 50;
  #endif

  /**
   * UBL Build Custom Mesh Command
   */
  void _lcd_ubl_build_custom_mesh() {
    char UBL_LCD_GCODE[20];
    enqueue_and_echo_commands_P(PSTR("G28"));
    #if HAS_HEATED_BED
      sprintf_P(UBL_LCD_GCODE, PSTR("M190 S%i"), custom_bed_temp);
      lcd_enqueue_command(UBL_LCD_GCODE);
    #endif
    sprintf_P(UBL_LCD_GCODE, PSTR("M109 S%i"), custom_hotend_temp);
    lcd_enqueue_command(UBL_LCD_GCODE);
    enqueue_and_echo_commands_P(PSTR("G29 P1"));
  }

  /**
   * UBL Custom Mesh submenu
   *
   * << Build Mesh
   *    Hotend Temp: ---
   *    Bed Temp: ---
   *    Build Custom Mesh
   */
  void _lcd_ubl_custom_mesh() {
    START_MENU();
    MENU_BACK(MSG_UBL_BUILD_MESH_MENU);
    MENU_ITEM_EDIT(int3, MSG_UBL_HOTEND_TEMP_CUSTOM, &custom_hotend_temp, EXTRUDE_MINTEMP, (HEATER_0_MAXTEMP - 10));
    #if HAS_HEATED_BED
      MENU_ITEM_EDIT(int3, MSG_UBL_BED_TEMP_CUSTOM, &custom_bed_temp, BED_MINTEMP, (BED_MAXTEMP - 15));
    #endif
    MENU_ITEM(function, MSG_UBL_BUILD_CUSTOM_MESH, _lcd_ubl_build_custom_mesh);
    END_MENU();
  }

  /**
   * UBL Adjust Mesh Height Command
   */
  void _lcd_ubl_adjust_height_cmd() {
    char UBL_LCD_GCODE[16];
    const int ind = ubl_height_amount > 0 ? 9 : 10;
    strcpy_P(UBL_LCD_GCODE, PSTR("G29 P6 C -"));
    sprintf_P(&UBL_LCD_GCODE[ind], PSTR(".%i"), ABS(ubl_height_amount));
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Adjust Mesh Height submenu
   *
   * << Edit Mesh
   *    Height Amount: ---
   *    Adjust Mesh Height
   * << Info Screen
   */
  void _menu_ubl_height_adjust() {
    START_MENU();
    MENU_BACK(MSG_UBL_EDIT_MESH_MENU);
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_UBL_MESH_HEIGHT_AMOUNT, &ubl_height_amount, -9, 9, _lcd_ubl_adjust_height_cmd);
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  /**
   * UBL Edit Mesh submenu
   *
   * << UBL Tools
   *    Fine Tune All
   *    Fine Tune Closest
   *  - Adjust Mesh Height >>
   * << Info Screen
   */
  void _lcd_ubl_edit_mesh() {
    START_MENU();
    MENU_BACK(MSG_UBL_TOOLS);
    MENU_ITEM(gcode, MSG_UBL_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
    MENU_ITEM(gcode, MSG_UBL_FINE_TUNE_CLOSEST, PSTR("G29 P4 T"));
    MENU_ITEM(submenu, MSG_UBL_MESH_HEIGHT_ADJUST, _menu_ubl_height_adjust);
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  /**
   * UBL Validate Custom Mesh Command
   */
  void _lcd_ubl_validate_custom_mesh() {
    char UBL_LCD_GCODE[24];
    const int temp =
      #if HAS_HEATED_BED
        custom_bed_temp
      #else
        0
      #endif
    ;
    sprintf_P(UBL_LCD_GCODE, PSTR("G26 C B%i H%i P"), temp, custom_hotend_temp);
    lcd_enqueue_commands_P(PSTR("G28"));
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Validate Mesh submenu
   *
   * << UBL Tools
   *    Mesh Validation with Material 1
   *    Mesh Validation with Material 2
   *    Validate Custom Mesh
   * << Info Screen
   */
  void _lcd_ubl_validate_mesh() {
    START_MENU();
    MENU_BACK(MSG_UBL_TOOLS);
    #if HAS_HEATED_BED
      MENU_ITEM(gcode, MSG_UBL_VALIDATE_MESH_M1, PSTR("G28\nG26 C B" STRINGIFY(PREHEAT_1_TEMP_BED) " H" STRINGIFY(PREHEAT_1_TEMP_HOTEND) " P"));
      MENU_ITEM(gcode, MSG_UBL_VALIDATE_MESH_M2, PSTR("G28\nG26 C B" STRINGIFY(PREHEAT_2_TEMP_BED) " H" STRINGIFY(PREHEAT_2_TEMP_HOTEND) " P"));
    #else
      MENU_ITEM(gcode, MSG_UBL_VALIDATE_MESH_M1, PSTR("G28\nG26 C B0 H" STRINGIFY(PREHEAT_1_TEMP_HOTEND) " P"));
      MENU_ITEM(gcode, MSG_UBL_VALIDATE_MESH_M2, PSTR("G28\nG26 C B0 H" STRINGIFY(PREHEAT_2_TEMP_HOTEND) " P"));
    #endif
    MENU_ITEM(function, MSG_UBL_VALIDATE_CUSTOM_MESH, _lcd_ubl_validate_custom_mesh);
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  /**
   * UBL Grid Leveling Command
   */
  void _lcd_ubl_grid_level_cmd() {
    char UBL_LCD_GCODE[10];
    sprintf_P(UBL_LCD_GCODE, PSTR("G29 J%i"), side_points);
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Grid Leveling submenu
   *
   * << UBL Tools
   *    Side points: ---
   *    Level Mesh
   */
  void _lcd_ubl_grid_level() {
    START_MENU();
    MENU_BACK(MSG_UBL_TOOLS);
    MENU_ITEM_EDIT(int3, MSG_UBL_SIDE_POINTS, &side_points, 2, 6);
    MENU_ITEM(function, MSG_UBL_MESH_LEVEL, _lcd_ubl_grid_level_cmd);
    END_MENU();
  }

  /**
   * UBL Mesh Leveling submenu
   *
   * << UBL Tools
   *    3-Point Mesh Leveling
   *  - Grid Mesh Leveling >>
   * << Info Screen
   */
  void _lcd_ubl_mesh_leveling() {
    START_MENU();
    MENU_BACK(MSG_UBL_TOOLS);
    MENU_ITEM(gcode, MSG_UBL_3POINT_MESH_LEVELING, PSTR("G29 J0"));
    MENU_ITEM(submenu, MSG_UBL_GRID_MESH_LEVELING, _lcd_ubl_grid_level);
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  /**
   * UBL Fill-in Amount Mesh Command
   */
  void _lcd_ubl_fillin_amount_cmd() {
    char UBL_LCD_GCODE[16];
    sprintf_P(UBL_LCD_GCODE, PSTR("G29 P3 R C.%i"), ubl_fillin_amount);
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Fill-in Mesh submenu
   *
   * << Build Mesh
   *    Fill-in Amount: ---
   *    Fill-in Mesh
   *    Smart Fill-in
   *    Manual Fill-in
   * << Info Screen
   */
  void _menu_ubl_fillin() {
    START_MENU();
    MENU_BACK(MSG_UBL_BUILD_MESH_MENU);
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_UBL_FILLIN_AMOUNT, &ubl_fillin_amount, 0, 9, _lcd_ubl_fillin_amount_cmd);
    MENU_ITEM(gcode, MSG_UBL_SMART_FILLIN, PSTR("G29 P3 T0"));
    MENU_ITEM(gcode, MSG_UBL_MANUAL_FILLIN, PSTR("G29 P2 B T0"));
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  void _lcd_ubl_invalidate() {
    ubl.invalidate();
    SERIAL_PROTOCOLLNPGM("Mesh invalidated.");
  }

  /**
   * UBL Build Mesh submenu
   *
   * << UBL Tools
   *    Build Mesh with Material 1
   *    Build Mesh with Material 2
   *  - Build Custom Mesh >>
   *    Build Cold Mesh
   *  - Fill-in Mesh >>
   *    Continue Bed Mesh
   *    Invalidate All
   *    Invalidate Closest
   * << Info Screen
   */
  void _lcd_ubl_build_mesh() {
    START_MENU();
    MENU_BACK(MSG_UBL_TOOLS);
    #if HAS_HEATED_BED
      MENU_ITEM(gcode, MSG_UBL_BUILD_MESH_M1, PSTR(
        "G28\n"
        "M190 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\n"
        "M109 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND) "\n"
        "G29 P1\n"
        "M104 S0\n"
        "M140 S0"
      ));
      MENU_ITEM(gcode, MSG_UBL_BUILD_MESH_M2, PSTR(
        "G28\n"
        "M190 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\n"
        "M109 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND) "\n"
        "G29 P1\n"
        "M104 S0\n"
        "M140 S0"
      ));
    #else
      MENU_ITEM(gcode, MSG_UBL_BUILD_MESH_M1, PSTR(
        "G28\n"
        "M109 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND) "\n"
        "G29 P1\n"
        "M104 S0"
      ));
      MENU_ITEM(gcode, MSG_UBL_BUILD_MESH_M2, PSTR(
        "G28\n"
        "M109 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND) "\n"
        "G29 P1\n"
        "M104 S0"
      ));
    #endif
    MENU_ITEM(submenu, MSG_UBL_BUILD_CUSTOM_MESH, _lcd_ubl_custom_mesh);
    MENU_ITEM(gcode, MSG_UBL_BUILD_COLD_MESH, PSTR("G28\nG29 P1"));
    MENU_ITEM(submenu, MSG_UBL_FILLIN_MESH, _menu_ubl_fillin);
    MENU_ITEM(gcode, MSG_UBL_CONTINUE_MESH, PSTR("G29 P1 C"));
    MENU_ITEM(function, MSG_UBL_INVALIDATE_ALL, _lcd_ubl_invalidate);
    MENU_ITEM(gcode, MSG_UBL_INVALIDATE_CLOSEST, PSTR("G29 I"));
    MENU_ITEM(function, MSG_WATCH, lcd_return_to_status);
    END_MENU();
  }

  /**
   * UBL Load Mesh Command
   */
  void _lcd_ubl_load_mesh_cmd() {
    char UBL_LCD_GCODE[25];
    sprintf_P(UBL_LCD_GCODE, PSTR("G29 L%i"), ubl_storage_slot);
    lcd_enqueue_command(UBL_LCD_GCODE);
    sprintf_P(UBL_LCD_GCODE, PSTR("M117 " MSG_MESH_LOADED), ubl_storage_slot);
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Save Mesh Command
   */
  void _lcd_ubl_save_mesh_cmd() {
    char UBL_LCD_GCODE[25];
    sprintf_P(UBL_LCD_GCODE, PSTR("G29 S%i"), ubl_storage_slot);
    lcd_enqueue_command(UBL_LCD_GCODE);
    sprintf_P(UBL_LCD_GCODE, PSTR("M117 " MSG_MESH_SAVED), ubl_storage_slot);
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL Mesh Storage submenu
   *
   * << Unified Bed Leveling
   *    Memory Slot: ---
   *    Load Bed Mesh
   *    Save Bed Mesh
   */
  void _lcd_ubl_storage_mesh() {
    int16_t a = settings.calc_num_meshes();
    START_MENU();
    MENU_BACK(MSG_UBL_LEVEL_BED);
    if (!WITHIN(ubl_storage_slot, 0, a - 1)) {
      STATIC_ITEM(MSG_NO_STORAGE);
    }
    else {
      MENU_ITEM_EDIT(int3, MSG_UBL_STORAGE_SLOT, &ubl_storage_slot, 0, a - 1);
      MENU_ITEM(function, MSG_UBL_LOAD_MESH, _lcd_ubl_load_mesh_cmd);
      MENU_ITEM(function, MSG_UBL_SAVE_MESH, _lcd_ubl_save_mesh_cmd);
    }
    END_MENU();
  }

  /**
   * UBL LCD "radar" map homing
   */
  void _lcd_ubl_output_map_lcd();

  void _lcd_ubl_map_homing() {
    defer_return_to_status = true;
    _lcd_draw_homing();
    if (all_axes_homed()) {
      ubl.lcd_map_control = true; // Return to the map screen
      lcd_goto_screen(_lcd_ubl_output_map_lcd);
    }
  }

  /**
   * UBL LCD "radar" map point editing
   */
  void _lcd_ubl_map_lcd_edit_cmd() {
    char UBL_LCD_GCODE[50], str[10], str2[10];
    dtostrf(pgm_read_float(&ubl._mesh_index_to_xpos[x_plot]), 0, 2, str);
    dtostrf(pgm_read_float(&ubl._mesh_index_to_ypos[y_plot]), 0, 2, str2);
    snprintf_P(UBL_LCD_GCODE, sizeof(UBL_LCD_GCODE), PSTR("G29 P4 X%s Y%s R%i"), str, str2, n_edit_pts);
    lcd_enqueue_command(UBL_LCD_GCODE);
  }

  /**
   * UBL LCD Map Movement
   */
  void ubl_map_move_to_xy() {
    current_position[X_AXIS] = pgm_read_float(&ubl._mesh_index_to_xpos[x_plot]);
    current_position[Y_AXIS] = pgm_read_float(&ubl._mesh_index_to_ypos[y_plot]);
    planner.buffer_line(current_position, MMM_TO_MMS(XY_PROBE_SPEED), active_extruder);
  }

  /**
   * UBL LCD "radar" map
   */
  void set_current_from_steppers_for_axis(const AxisEnum axis);
  void sync_plan_position();

  void _lcd_do_nothing() {}
  void _lcd_hard_stop() {
    const screenFunc_t old_screen = currentScreen;
    currentScreen = _lcd_do_nothing;
    planner.quick_stop();
    currentScreen = old_screen;
    set_current_from_steppers_for_axis(ALL_AXES);
    sync_plan_position();
  }

  void _lcd_ubl_output_map_lcd() {
    static int16_t step_scaler = 0;

    if (use_click()) return _lcd_ubl_map_lcd_edit_cmd();
    ENCODER_DIRECTION_NORMAL();

    if (encoderPosition) {
      step_scaler += (int32_t)encoderPosition;
      x_plot += step_scaler / (ENCODER_STEPS_PER_MENU_ITEM);
      if (ABS(step_scaler) >= ENCODER_STEPS_PER_MENU_ITEM) step_scaler = 0;
      encoderPosition = 0;
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
    }

    // Encoder to the right (++)
    if (x_plot >= GRID_MAX_POINTS_X) { x_plot = 0; y_plot++; }
    if (y_plot >= GRID_MAX_POINTS_Y) y_plot = 0;

    // Encoder to the left (--)
    if (x_plot <= GRID_MAX_POINTS_X - (GRID_MAX_POINTS_X + 1)) { x_plot = GRID_MAX_POINTS_X - 1; y_plot--; }
    if (y_plot <= GRID_MAX_POINTS_Y - (GRID_MAX_POINTS_Y + 1)) y_plot = GRID_MAX_POINTS_Y - 1;

    // Prevent underrun/overrun of plot numbers
    x_plot = constrain(x_plot, GRID_MAX_POINTS_X - (GRID_MAX_POINTS_X + 1), GRID_MAX_POINTS_X + 1);
    y_plot = constrain(y_plot, GRID_MAX_POINTS_Y - (GRID_MAX_POINTS_Y + 1), GRID_MAX_POINTS_Y + 1);

    // Determine number of points to edit
    #if IS_KINEMATIC
      n_edit_pts = 9; //TODO: Delta accessible edit points
    #else
      const bool xc = WITHIN(x_plot, 1, GRID_MAX_POINTS_X - 2),
                 yc = WITHIN(y_plot, 1, GRID_MAX_POINTS_Y - 2);
      n_edit_pts = yc ? (xc ? 9 : 6) : (xc ? 6 : 4); // Corners
    #endif

    if (lcdDrawUpdate) {
      lcd_implementation_ubl_plot(x_plot, y_plot);

      if (planner.movesplanned()) // If the nozzle is already moving, cancel the move.
        _lcd_hard_stop();

      ubl_map_move_to_xy();       // Move to new location
    }
  }

  /**
   * UBL Homing before LCD map
   */
  void _lcd_ubl_output_map_lcd_cmd() {
    if (!all_axes_known()) {
      axis_homed = 0;
      enqueue_and_echo_commands_P(PSTR("G28"));
    }
    lcd_goto_screen(_lcd_ubl_map_homing);
  }

  /**
   * UBL Output map submenu
   *
   * << Unified Bed Leveling
   *  Output for Host
   *  Output for CSV
   *  Off Printer Backup
   *  Output Mesh Map
   */
  void _lcd_ubl_output_map() {
    START_MENU();
    MENU_BACK(MSG_UBL_LEVEL_BED);
    MENU_ITEM(gcode, MSG_UBL_OUTPUT_MAP_HOST, PSTR("G29 T0"));
    MENU_ITEM(gcode, MSG_UBL_OUTPUT_MAP_CSV, PSTR("G29 T1"));
    MENU_ITEM(gcode, MSG_UBL_OUTPUT_MAP_BACKUP, PSTR("G29 S-1"));
    MENU_ITEM(function, MSG_UBL_OUTPUT_MAP, _lcd_ubl_output_map_lcd_cmd);
    END_MENU();
  }

  /**
   * UBL Tools submenu
   *
   * << Unified Bed Leveling
   *  - Build Mesh >>
   *  - Validate Mesh >>
   *  - Edit Mesh >>
   *  - Mesh Leveling >>
   */
  void _menu_ubl_tools() {
    START_MENU();
    MENU_BACK(MSG_UBL_LEVEL_BED);
    MENU_ITEM(submenu, MSG_UBL_BUILD_MESH_MENU, _lcd_ubl_build_mesh);
    MENU_ITEM(gcode, MSG_UBL_MANUAL_MESH, PSTR("G29 I999\nG29 P2 B T0"));
    MENU_ITEM(submenu, MSG_UBL_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
    MENU_ITEM(submenu, MSG_UBL_EDIT_MESH_MENU, _lcd_ubl_edit_mesh);
    MENU_ITEM(submenu, MSG_UBL_MESH_LEVELING, _lcd_ubl_mesh_leveling);
    END_MENU();
  }

  /**
   * UBL Step-By-Step submenu
   *
   * << Unified Bed Leveling
   *    1 Build Cold Mesh
   *    2 Smart Fill-in
   *  - 3 Validate Mesh >>
   *    4 Fine Tune All
   *  - 5 Validate Mesh >>
   *    6 Fine Tune All
   *    7 Save Bed Mesh
   */
  void _lcd_ubl_step_by_step() {
    START_MENU();
    MENU_BACK(MSG_UBL_LEVEL_BED);
    MENU_ITEM(gcode, "1 " MSG_UBL_BUILD_COLD_MESH, PSTR("G28\nG29 P1"));
    MENU_ITEM(gcode, "2 " MSG_UBL_SMART_FILLIN, PSTR("G29 P3 T0"));
    MENU_ITEM(submenu, "3 " MSG_UBL_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
    MENU_ITEM(gcode, "4 " MSG_UBL_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
    MENU_ITEM(submenu, "5 " MSG_UBL_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
    MENU_ITEM(gcode, "6 " MSG_UBL_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
    MENU_ITEM(function, "7 " MSG_UBL_SAVE_MESH, _lcd_ubl_save_mesh_cmd);
    END_MENU();
  }

  /**
   * UBL System submenu
   *
   * << Motion
   *  - Manually Build Mesh >>
   *  - Activate UBL >>
   *  - Deactivate UBL >>
   *  - Step-By-Step UBL >>
   *  - Mesh Storage >>
   *  - Output Map >>
   *  - UBL Tools >>
   *  - Output UBL Info >>
   */

  void _lcd_ubl_level_bed() {
    START_MENU();
    MENU_BACK(MSG_MOTION);
    MENU_ITEM(gcode, MSG_UBL_ACTIVATE_MESH, PSTR("G29 A"));
    MENU_ITEM(gcode, MSG_UBL_DEACTIVATE_MESH, PSTR("G29 D"));
    MENU_ITEM(submenu, MSG_UBL_STEP_BY_STEP_MENU, _lcd_ubl_step_by_step);
    MENU_ITEM(function, MSG_UBL_MESH_EDIT, _lcd_ubl_output_map_lcd_cmd);
    MENU_ITEM(submenu, MSG_UBL_STORAGE_MESH_MENU, _lcd_ubl_storage_mesh);
    MENU_ITEM(submenu, MSG_UBL_OUTPUT_MAP, _lcd_ubl_output_map);
    MENU_ITEM(submenu, MSG_UBL_TOOLS, _menu_ubl_tools);
    MENU_ITEM(gcode, MSG_UBL_INFO_UBL, PSTR("G29 W"));
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float3, MSG_Z_FADE_HEIGHT, &lcd_z_fade_height, 0, 100, _lcd_set_z_fade_height);
    #endif
    END_MENU();
  }

#endif // AUTO_BED_LEVELING_UBL

#if ENABLED(LCD_BED_LEVELING) || (HAS_LEVELING && DISABLED(SLIM_LCD_MENUS))
  void _lcd_toggle_bed_leveling() { set_bed_leveling_enabled(!planner.leveling_active); }
#endif

#if ENABLED(LCD_BED_LEVELING)

  /**
   * Step 1: Bed Level entry-point
   *
   * << Motion
   *    Auto Home           (if homing needed)
   *    Leveling On/Off     (if data exists, and homed)
   *    Fade Height: ---    (Req: ENABLE_LEVELING_FADE_HEIGHT)
   *    Mesh Z Offset: ---  (Req: MESH_BED_LEVELING)
   *    Z Probe Offset: --- (Req: HAS_BED_PROBE, Opt: BABYSTEP_ZPROBE_OFFSET)
   *    Level Bed >
   *    Level Corners >     (if homed)
   *    Load Settings       (Req: EEPROM_SETTINGS)
   *    Save Settings       (Req: EEPROM_SETTINGS)
   */
  void menu_bed_leveling() {
    START_MENU();
    MENU_BACK(MSG_MOTION);

    const bool is_homed = all_axes_known();

    // Auto Home if not using manual probing
    #if DISABLED(PROBE_MANUALLY) && DISABLED(MESH_BED_LEVELING)
      if (!is_homed) MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
    #endif

    // Level Bed
    #if ENABLED(PROBE_MANUALLY) || ENABLED(MESH_BED_LEVELING)
      // Manual leveling uses a guided procedure
      MENU_ITEM(submenu, MSG_LEVEL_BED, _lcd_level_bed_continue);
    #else
      // Automatic leveling can just run the G-code
      MENU_ITEM(gcode, MSG_LEVEL_BED, is_homed ? PSTR("G29") : PSTR("G28\nG29"));
    #endif

    // Homed and leveling is valid? Then leveling can be toggled.
    if (is_homed && leveling_is_valid()) {
      bool new_level_state = planner.leveling_active;
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_BED_LEVELING, &new_level_state, _lcd_toggle_bed_leveling);
    }

    // Z Fade Height
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float3, MSG_Z_FADE_HEIGHT, &lcd_z_fade_height, 0, 100, _lcd_set_z_fade_height);
    #endif

    //
    // MBL Z Offset
    //
    #if ENABLED(MESH_BED_LEVELING)
      MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
    #endif

    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    #elif HAS_BED_PROBE
      MENU_ITEM_EDIT(float52, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
    #endif

    #if ENABLED(LEVEL_BED_CORNERS)
      // Move to the next corner for leveling
      MENU_ITEM(submenu, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
    #endif

    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
      MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }

#endif // LCD_BED_LEVELING

/**
 *
 * "Movement" submenu
 *
 */

void menu_movement() {
  START_MENU();

  //
  // ^ Main
  //
  MENU_BACK(MSG_MAIN);

  //
  // Move Axis
  //
  #if ENABLED(DELTA)
    if (all_axes_homed())
  #endif
      MENU_ITEM(submenu, MSG_MOVE_AXIS, menu_move);

  //
  // Auto Home
  //
  MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
  #if ENABLED(INDIVIDUAL_AXIS_HOMING_MENU)
    MENU_ITEM(gcode, MSG_AUTO_HOME_X, PSTR("G28 X"));
    MENU_ITEM(gcode, MSG_AUTO_HOME_Y, PSTR("G28 Y"));
    MENU_ITEM(gcode, MSG_AUTO_HOME_Z, PSTR("G28 Z"));
  #endif

  //
  // TMC Z Calibration
  //
  #if ENABLED(TMC_Z_CALIBRATION)
    MENU_ITEM(gcode, MSG_TMC_Z_CALIBRATION, PSTR("G28\nM915"));
  #endif

  //
  // Level Bed
  //
  #if ENABLED(AUTO_BED_LEVELING_UBL)

    MENU_ITEM(submenu, MSG_UBL_LEVEL_BED, _lcd_ubl_level_bed);

  #elif ENABLED(LCD_BED_LEVELING)

    #if ENABLED(PROBE_MANUALLY)
      if (!g29_in_progress)
    #endif
        MENU_ITEM(submenu, MSG_BED_LEVELING, menu_bed_leveling);

  #elif HAS_LEVELING && DISABLED(SLIM_LCD_MENUS)

    #if DISABLED(PROBE_MANUALLY)
      MENU_ITEM(gcode, MSG_LEVEL_BED, PSTR("G28\nG29"));
    #endif
    if (leveling_is_valid()) {
      bool new_level_state = planner.leveling_active;
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_BED_LEVELING, &new_level_state, _lcd_toggle_bed_leveling);
    }
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float3, MSG_Z_FADE_HEIGHT, &lcd_z_fade_height, 0, 100, _lcd_set_z_fade_height);
    #endif

  #endif

  #if ENABLED(LEVEL_BED_CORNERS) && DISABLED(LCD_BED_LEVELING)
      MENU_ITEM(function, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  #endif

  //
  // Disable Steppers
  //
  MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));

  END_MENU();
}

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

/**
 * Set a flag that lcd_update() should start a move
 * to "current_position" after a short delay.
 */
inline void manual_move_to_current(AxisEnum axis
  #if E_MANUAL > 1
    , const int8_t eindex=-1
  #endif
) {
  #if ENABLED(DUAL_X_CARRIAGE) || E_MANUAL > 1
    #if E_MANUAL > 1
      if (axis == E_AXIS)
    #endif
        manual_move_e_index = eindex >= 0 ? eindex : active_extruder;
  #endif
  manual_move_start_time = millis() + (move_menu_scale < 0.99f ? 0UL : 250UL); // delay for bigger moves
  manual_move_axis = (int8_t)axis;
}

/**
 *
 * "Motion" > "Move Axis" submenu
 *
 */

void _lcd_move_xyz(PGM_P name, AxisEnum axis) {
  if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
  ENCODER_DIRECTION_NORMAL();
  if (encoderPosition && !processing_manual_move) {

    // Start with no limits to movement
    float min = current_position[axis] - 1000,
          max = current_position[axis] + 1000;

    // Limit to software endstops, if enabled
    #if ENABLED(MIN_SOFTWARE_ENDSTOPS) || ENABLED(MAX_SOFTWARE_ENDSTOPS)
      if (soft_endstops_enabled) switch (axis) {
        case X_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
            min = soft_endstop_min[X_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
            max = soft_endstop_max[X_AXIS];
          #endif
          break;
        case Y_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
            min = soft_endstop_min[Y_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
            max = soft_endstop_max[Y_AXIS];
          #endif
          break;
        case Z_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
            min = soft_endstop_min[Z_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
            max = soft_endstop_max[Z_AXIS];
          #endif
        default: break;
      }
    #endif // MIN_SOFTWARE_ENDSTOPS || MAX_SOFTWARE_ENDSTOPS

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const float diff = float((int32_t)encoderPosition) * move_menu_scale;
    #if IS_KINEMATIC
      manual_move_offset += diff;
      if ((int32_t)encoderPosition < 0)
        NOLESS(manual_move_offset, min - current_position[axis]);
      else
        NOMORE(manual_move_offset, max - current_position[axis]);
    #else
      current_position[axis] += diff;
      if ((int32_t)encoderPosition < 0)
        NOLESS(current_position[axis], min);
      else
        NOMORE(current_position[axis], max);
    #endif

    manual_move_to_current(axis);
    lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
  }
  encoderPosition = 0;
  if (lcdDrawUpdate) {
    const float pos = NATIVE_TO_LOGICAL(processing_manual_move ? destination[axis] : current_position[axis]
      #if IS_KINEMATIC
        + manual_move_offset
      #endif
    , axis);
    lcd_implementation_drawedit(name, move_menu_scale >= 0.1f ? ftostr41sign(pos) : ftostr43sign(pos));
  }
}
void lcd_move_x() { _lcd_move_xyz(PSTR(MSG_MOVE_X), X_AXIS); }
void lcd_move_y() { _lcd_move_xyz(PSTR(MSG_MOVE_Y), Y_AXIS); }
void lcd_move_z() { _lcd_move_xyz(PSTR(MSG_MOVE_Z), Z_AXIS); }
void _lcd_move_e(
  #if E_MANUAL > 1
    const int8_t eindex=-1
  #endif
) {
  if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
  ENCODER_DIRECTION_NORMAL();
  if (encoderPosition) {
    if (!processing_manual_move) {
      const float diff = float((int32_t)encoderPosition) * move_menu_scale;
      #if IS_KINEMATIC
        manual_move_offset += diff;
      #else
        current_position[E_AXIS] += diff;
      #endif
      manual_move_to_current(E_AXIS
        #if E_MANUAL > 1
          , eindex
        #endif
      );
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
    }
    encoderPosition = 0;
  }
  if (lcdDrawUpdate) {
    PGM_P pos_label;
    #if E_MANUAL == 1
      pos_label = PSTR(MSG_MOVE_E);
    #else
      switch (eindex) {
        default: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E1); break;
        case 1: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E2); break;
        #if E_MANUAL > 2
          case 2: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E3); break;
          #if E_MANUAL > 3
            case 3: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E4); break;
            #if E_MANUAL > 4
              case 4: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E5); break;
              #if E_MANUAL > 5
                case 5: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E6); break;
              #endif // E_MANUAL > 5
            #endif // E_MANUAL > 4
          #endif // E_MANUAL > 3
        #endif // E_MANUAL > 2
      }
    #endif // E_MANUAL > 1

    lcd_implementation_drawedit(pos_label, ftostr41sign(current_position[E_AXIS]
      #if IS_KINEMATIC
        + manual_move_offset
      #endif
      #if ENABLED(MANUAL_E_MOVES_RELATIVE)
        - manual_move_e_origin
      #endif
    ));
  }
}

void lcd_move_e() { _lcd_move_e(); }
#if E_MANUAL > 1
  void lcd_move_e0() { _lcd_move_e(0); }
  void lcd_move_e1() { _lcd_move_e(1); }
  #if E_MANUAL > 2
    void lcd_move_e2() { _lcd_move_e(2); }
    #if E_MANUAL > 3
      void lcd_move_e3() { _lcd_move_e(3); }
      #if E_MANUAL > 4
        void lcd_move_e4() { _lcd_move_e(4); }
        #if E_MANUAL > 5
          void lcd_move_e5() { _lcd_move_e(5); }
        #endif // E_MANUAL > 5
      #endif // E_MANUAL > 4
    #endif // E_MANUAL > 3
  #endif // E_MANUAL > 2
#endif // E_MANUAL > 1

/**
 *
 * "Motion" > "Move Xmm" > "Move XYZ" submenu
 *
 */

screenFunc_t _manual_move_func_ptr;

void _goto_manual_move(const float scale) {
  defer_return_to_status = true;
  move_menu_scale = scale;
  lcd_goto_screen(_manual_move_func_ptr);
}
void menu_move_10mm() { _goto_manual_move(10); }
void menu_move_1mm()  { _goto_manual_move( 1); }
void menu_move_01mm() { _goto_manual_move( 0.1f); }

void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=-1) {
  _manual_move_func_ptr = func;
  START_MENU();
  if (LCD_HEIGHT >= 4) {
    switch (axis) {
      case X_AXIS:
        STATIC_ITEM(MSG_MOVE_X, true, true); break;
      case Y_AXIS:
        STATIC_ITEM(MSG_MOVE_Y, true, true); break;
      case Z_AXIS:
        STATIC_ITEM(MSG_MOVE_Z, true, true); break;
      default:
        #if ENABLED(MANUAL_E_MOVES_RELATIVE)
          manual_move_e_origin = current_position[E_AXIS];
        #endif
        STATIC_ITEM(MSG_MOVE_E, true, true);
        break;
    }
  }
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(eindex >= 0 ? eindex : active_extruder))
      MENU_BACK(MSG_HOTEND_TOO_COLD);
    else
  #endif
  {
    MENU_BACK(MSG_MOVE_AXIS);
    MENU_ITEM(submenu, MSG_MOVE_10MM, menu_move_10mm);
    MENU_ITEM(submenu, MSG_MOVE_1MM, menu_move_1mm);
    MENU_ITEM(submenu, MSG_MOVE_01MM, menu_move_01mm);
  }
  END_MENU();
}
void lcd_move_get_x_amount()        { _menu_move_distance(X_AXIS, lcd_move_x); }
void lcd_move_get_y_amount()        { _menu_move_distance(Y_AXIS, lcd_move_y); }
void lcd_move_get_z_amount()        { _menu_move_distance(Z_AXIS, lcd_move_z); }
void lcd_move_get_e_amount()        { _menu_move_distance(E_AXIS, lcd_move_e, -1); }
#if E_MANUAL > 1
  void lcd_move_get_e0_amount()     { _menu_move_distance(E_AXIS, lcd_move_e0, 0); }
  void lcd_move_get_e1_amount()     { _menu_move_distance(E_AXIS, lcd_move_e1, 1); }
  #if E_MANUAL > 2
    void lcd_move_get_e2_amount()   { _menu_move_distance(E_AXIS, lcd_move_e2, 2); }
    #if E_MANUAL > 3
      void lcd_move_get_e3_amount() { _menu_move_distance(E_AXIS, lcd_move_e3, 3); }
      #if E_MANUAL > 4
        void lcd_move_get_e4_amount() { _menu_move_distance(E_AXIS, lcd_move_e4, 4); }
        #if E_MANUAL > 5
          void lcd_move_get_e5_amount() { _menu_move_distance(E_AXIS, lcd_move_e5, 5); }
        #endif // E_MANUAL > 5
      #endif // E_MANUAL > 4
    #endif // E_MANUAL > 3
  #endif // E_MANUAL > 2
#endif // E_MANUAL > 1

/**
 *
 * "Motion" > "Move Axis" submenu
 *
 */

#if ENABLED(DELTA)
  void lcd_lower_z_to_clip_height() {
    line_to_z(delta_clip_start_height);
    lcd_synchronize();
  }
#endif

void menu_move() {
  START_MENU();
  MENU_BACK(MSG_MOTION);

  #if HAS_SOFTWARE_ENDSTOPS && ENABLED(SOFT_ENDSTOPS_MENU_ITEM)
    MENU_ITEM_EDIT(bool, MSG_LCD_SOFT_ENDSTOPS, &soft_endstops_enabled);
  #endif

  if (
    #if IS_KINEMATIC || ENABLED(NO_MOTION_BEFORE_HOMING)
      all_axes_homed()
    #else
      true
    #endif
  ) {
    if (
      #if ENABLED(DELTA)
        current_position[Z_AXIS] <= delta_clip_start_height
      #else
        true
      #endif
    ) {
      MENU_ITEM(submenu, MSG_MOVE_X, lcd_move_get_x_amount);
      MENU_ITEM(submenu, MSG_MOVE_Y, lcd_move_get_y_amount);
    }
    #if ENABLED(DELTA)
      else
        MENU_ITEM(function, MSG_FREE_XY, lcd_lower_z_to_clip_height);
    #endif

    MENU_ITEM(submenu, MSG_MOVE_Z, lcd_move_get_z_amount);
  }
  else
    MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));

  #if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE)

    #if EXTRUDERS == 6
      switch (active_extruder) {
        case 0: MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1")); break;
        case 1: MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0")); break;
        case 2: MENU_ITEM(gcode, MSG_SELECT " " MSG_E4, PSTR("T3")); break;
        case 3: MENU_ITEM(gcode, MSG_SELECT " " MSG_E3, PSTR("T2")); break;
        case 4: MENU_ITEM(gcode, MSG_SELECT " " MSG_E6, PSTR("T5")); break;
        case 5: MENU_ITEM(gcode, MSG_SELECT " " MSG_E5, PSTR("T4")); break;
      }
    #elif EXTRUDERS == 5 || EXTRUDERS == 4
      switch (active_extruder) {
        case 0: MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1")); break;
        case 1: MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0")); break;
        case 2: MENU_ITEM(gcode, MSG_SELECT " " MSG_E4, PSTR("T3")); break;
        case 3: MENU_ITEM(gcode, MSG_SELECT " " MSG_E3, PSTR("T2")); break;
      }
    #elif EXTRUDERS == 3
      if (active_extruder < 2) {
        if (active_extruder)
          MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
        else
          MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));
      }
    #else
      if (active_extruder)
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
      else
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));
    #endif

  #elif ENABLED(DUAL_X_CARRIAGE)

    if (active_extruder)
      MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
    else
      MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));

  #endif

  #if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE)

    // Only the current...
    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_get_e_amount);
    // ...and the non-switching
    #if E_MANUAL == 5
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E5, lcd_move_get_e4_amount);
    #elif E_MANUAL == 3
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E3, lcd_move_get_e2_amount);
    #endif

  #else

    // Independent extruders with one E-stepper per hotend
    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_get_e_amount);
    #if E_MANUAL > 1
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E1, lcd_move_get_e0_amount);
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E2, lcd_move_get_e1_amount);
      #if E_MANUAL > 2
        MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E3, lcd_move_get_e2_amount);
        #if E_MANUAL > 3
          MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E4, lcd_move_get_e3_amount);
          #if E_MANUAL > 4
            MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E5, lcd_move_get_e4_amount);
            #if E_MANUAL > 5
              MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E6, lcd_move_get_e5_amount);
            #endif // E_MANUAL > 5
          #endif // E_MANUAL > 4
        #endif // E_MANUAL > 3
      #endif // E_MANUAL > 2
    #endif // E_MANUAL > 1

  #endif

  END_MENU();
}

/**
 *
 * "Configuration" submenu
 *
 */

#if HAS_LCD_CONTRAST
  void lcd_callback_set_contrast() { set_lcd_contrast(lcd_contrast); }
#endif

static void lcd_factory_settings() {
  settings.reset();
  lcd_completion_feedback();
}

#if ENABLED(EEPROM_SETTINGS) && DISABLED(SLIM_LCD_MENUS)

  static void lcd_init_eeprom() {
    lcd_completion_feedback(settings.init_eeprom());
    lcd_goto_previous_menu();
  }

  static void lcd_init_eeprom_confirm() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);
    MENU_ITEM(function, MSG_INIT_EEPROM, lcd_init_eeprom);
    END_MENU();
  }

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

  const bool busy = printer_busy();
  if (!busy) {
    //
    // Delta Calibration
    //
    #if ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION)
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE, menu_delta_calibrate);
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      MENU_ITEM(submenu, MSG_IDEX_MENU, menu_IDEX);
    #endif

    #if ENABLED(BLTOUCH)
      MENU_ITEM(submenu, MSG_BLTOUCH, menu_bltouch);
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
  #if ENABLED(MENU_ITEM_CASE_LIGHT)
    if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
      MENU_ITEM(submenu, MSG_CASE_LIGHT, menu_case_light);
    else
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_CASE_LIGHT, (bool*)&case_light_on, update_case_light);
  #endif

  #if HAS_LCD_CONTRAST
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_CONTRAST, &lcd_contrast, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX, lcd_callback_set_contrast, true);
  #endif
  #if ENABLED(FWRETRACT)
    MENU_ITEM(submenu, MSG_RETRACT, menu_config_retract);
  #endif
  #if ENABLED(DAC_STEPPER_CURRENT)
    MENU_ITEM(submenu, MSG_DRIVE_STRENGTH, menu_dac);
  #endif
  #if HAS_MOTOR_CURRENT_PWM
    MENU_ITEM(submenu, MSG_DRIVE_STRENGTH, menu_pwm);
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    MENU_ITEM_EDIT(bool, MSG_RUNOUT_SENSOR_ENABLE, &runout.enabled);
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

/**
 *
 * "Temperature" submenu
 *
 */

#if ENABLED(PID_AUTOTUNE_MENU)

  #if ENABLED(PIDTEMP)
    int16_t autotune_temp[HOTENDS] = ARRAY_BY_HOTENDS1(150);
  #endif

  #if ENABLED(PIDTEMPBED)
    int16_t autotune_temp_bed = 70;
  #endif

  void _lcd_autotune(int16_t e) {
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

#if ENABLED(PIDTEMP)

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

  #if ENABLED(PID_AUTOTUNE_MENU)
    #define DEFINE_PIDTEMP_FUNCS(N) \
      _DEFINE_PIDTEMP_BASE_FUNCS(N); \
      void lcd_autotune_callback_E ## N() { _lcd_autotune(N); } typedef void _pid_##N##_void
  #else
    #define DEFINE_PIDTEMP_FUNCS(N) _DEFINE_PIDTEMP_BASE_FUNCS(N) typedef void _pid_##N##_void
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

#endif // PIDTEMP

/**
 *
 * "Temperature" submenu
 *
 */
void menu_temperature() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  //
  // Nozzle:
  // Nozzle [1-5]:
  //
  #if HOTENDS == 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
  #else // HOTENDS > 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N1, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N2, &thermalManager.target_temperature[1], 0, HEATER_1_MAXTEMP - 15, watch_temp_callback_E1);
    #if HOTENDS > 2
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N3, &thermalManager.target_temperature[2], 0, HEATER_2_MAXTEMP - 15, watch_temp_callback_E2);
      #if HOTENDS > 3
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N4, &thermalManager.target_temperature[3], 0, HEATER_3_MAXTEMP - 15, watch_temp_callback_E3);
        #if HOTENDS > 4
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N5, &thermalManager.target_temperature[4], 0, HEATER_4_MAXTEMP - 15, watch_temp_callback_E4);
          #if HOTENDS > 5
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N6, &thermalManager.target_temperature[5], 0, HEATER_5_MAXTEMP - 15, watch_temp_callback_E5);
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.target_temperature_bed, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
  #endif

  //
  // Fan Speed:
  //
  #if FAN_COUNT > 0
    #if HAS_FAN0
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED FAN_SPEED_1_SUFFIX, &fan_speed[0], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED FAN_SPEED_1_SUFFIX, &new_fan_speed[0], 3, 255);
      #endif
    #endif
    #if HAS_FAN1
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED " 2", &fan_speed[1], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED " 2", &new_fan_speed[1], 3, 255);
      #endif
    #endif
    #if HAS_FAN2
      MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_FAN_SPEED " 3", &fan_speed[2], 0, 255);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(int8, MSG_EXTRA_FAN_SPEED " 3", &new_fan_speed[2], 3, 255);
      #endif
    #endif
  #endif // FAN_COUNT > 0

  #if HAS_TEMP_HOTEND

    //
    // Cooldown
    //
    bool has_heat = false;
    HOTEND_LOOP() if (thermalManager.target_temperature[HOTEND_INDEX]) { has_heat = true; break; }
    #if HAS_TEMP_BED
      if (thermalManager.target_temperature_bed) has_heat = true;
    #endif
    if (has_heat) MENU_ITEM(function, MSG_COOLDOWN, lcd_cooldown);

    //
    // Preheat for Material 1 and 2
    //
    #if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_4 != 0 || HAS_HEATED_BED
      MENU_ITEM(submenu, MSG_PREHEAT_1, menu_preheat_m1);
      MENU_ITEM(submenu, MSG_PREHEAT_2, menu_preheat_m2);
    #else
      MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0_only);
      MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0_only);
    #endif

  #endif // HAS_TEMP_HOTEND

  END_MENU();
}

/**
 *
 * "Advanced Settings" -> "Temperature" submenu
 *
 */
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
    MENU_ITEM_EDIT(float52, MSG_FACTOR, &planner.autotemp_factor, 0, 1);
  #endif

  //
  // PID-P, PID-I, PID-D, PID-C, PID Autotune
  // PID-P E1, PID-I E1, PID-D E1, PID-C E1, PID Autotune E1
  // PID-P E2, PID-I E2, PID-D E2, PID-C E2, PID Autotune E2
  // PID-P E3, PID-I E3, PID-D E3, PID-C E3, PID Autotune E3
  // PID-P E4, PID-I E4, PID-D E4, PID-C E4, PID Autotune E4
  // PID-P E5, PID-I E5, PID-D E5, PID-C E5, PID Autotune E5
  //
  #if ENABLED(PIDTEMP)

    #define _PID_BASE_MENU_ITEMS(ELABEL, eindex) \
      raw_Ki = unscalePID_i(PID_PARAM(Ki, eindex)); \
      raw_Kd = unscalePID_d(PID_PARAM(Kd, eindex)); \
      MENU_ITEM_EDIT(float52sign, MSG_PID_P ELABEL, &PID_PARAM(Kp, eindex), 1, 9990); \
      MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_PID_I ELABEL, &raw_Ki, 0.01f, 9990, copy_and_scalePID_i_E ## eindex); \
      MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_PID_D ELABEL, &raw_Kd, 1, 9990, copy_and_scalePID_d_E ## eindex)

    #if ENABLED(PID_EXTRUSION_SCALING)
      #define _PID_MENU_ITEMS(ELABEL, eindex) \
        _PID_BASE_MENU_ITEMS(ELABEL, eindex); \
        MENU_ITEM_EDIT(float3, MSG_PID_C ELABEL, &PID_PARAM(Kc, eindex), 1, 9990)
    #else
      #define _PID_MENU_ITEMS(ELABEL, eindex) _PID_BASE_MENU_ITEMS(ELABEL, eindex)
    #endif

    #if ENABLED(PID_AUTOTUNE_MENU)
      #define PID_MENU_ITEMS(ELABEL, eindex) \
        _PID_MENU_ITEMS(ELABEL, eindex); \
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_PID_AUTOTUNE ELABEL, &autotune_temp[eindex], 150, heater_maxtemp[eindex] - 15, lcd_autotune_callback_E ## eindex)
    #else
      #define PID_MENU_ITEMS(ELABEL, eindex) _PID_MENU_ITEMS(ELABEL, eindex)
    #endif

    #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1
      PID_MENU_ITEMS(" " MSG_E1, 0);
      PID_MENU_ITEMS(" " MSG_E2, 1);
      #if HOTENDS > 2
        PID_MENU_ITEMS(" " MSG_E3, 2);
        #if HOTENDS > 3
          PID_MENU_ITEMS(" " MSG_E4, 3);
          #if HOTENDS > 4
            PID_MENU_ITEMS(" " MSG_E5, 4);
            #if HOTENDS > 5
              PID_MENU_ITEMS(" " MSG_E6, 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #else // !PID_PARAMS_PER_HOTEND || HOTENDS == 1
      PID_MENU_ITEMS("", 0);
    #endif // !PID_PARAMS_PER_HOTEND || HOTENDS == 1

  #endif // PIDTEMP

  END_MENU();
}

#if DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings(const uint8_t material) {
    #if HOTENDS > 5
      #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP, HEATER_5_MINTEMP)
      #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP)
    #elif HOTENDS > 4
      #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP)
      #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP)
    #elif HOTENDS > 3
      #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP)
      #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP)
    #elif HOTENDS > 2
      #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP)
      #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP)
    #elif HOTENDS > 1
      #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP)
      #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP)
    #else
      #define MINTEMP_ALL HEATER_0_MINTEMP
      #define MAXTEMP_ALL HEATER_0_MAXTEMP
    #endif
    START_MENU();
    MENU_BACK(MSG_CONFIGURATION);
    MENU_ITEM_EDIT(int8, MSG_FAN_SPEED, &lcd_preheat_fan_speed[material], 0, 255);
    #if HAS_TEMP_HOTEND
      MENU_ITEM_EDIT(int3, MSG_NOZZLE, &lcd_preheat_hotend_temp[material], MINTEMP_ALL, MAXTEMP_ALL - 15);
    #endif
    #if HAS_HEATED_BED
      MENU_ITEM_EDIT(int3, MSG_BED, &lcd_preheat_bed_temp[material], BED_MINTEMP, BED_MAXTEMP - 15);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
    #endif
    END_MENU();
  }

  /**
   *
   * "Temperature" > "Preheat Material 1 conf" submenu
   *
   */
  void menu_preheat_material1_settings() { _menu_configuration_preheat_settings(0); }

  /**
   *
   * "Temperature" > "Preheat Material 2 conf" submenu
   *
   */
  void menu_preheat_material2_settings() { _menu_configuration_preheat_settings(1); }

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
        planner.steps_to_mm[E_AXIS + e] = 1.0f / planner.settings.axis_steps_per_mm[E_AXIS + e];
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
    MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_A, &planner.settings.max_feedrate_mm_s[A_AXIS], 1, 999);
    MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_B, &planner.settings.max_feedrate_mm_s[B_AXIS], 1, 999);
    MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_C, &planner.settings.max_feedrate_mm_s[C_AXIS], 1, 999);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E, &planner.settings.max_feedrate_mm_s[E_AXIS + active_extruder], 1, 999);
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E1, &planner.settings.max_feedrate_mm_s[E_AXIS], 1, 999);
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E2, &planner.settings.max_feedrate_mm_s[E_AXIS + 1], 1, 999);
      #if E_STEPPERS > 2
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E3, &planner.settings.max_feedrate_mm_s[E_AXIS + 2], 1, 999);
        #if E_STEPPERS > 3
          MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E4, &planner.settings.max_feedrate_mm_s[E_AXIS + 3], 1, 999);
          #if E_STEPPERS > 4
            MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E5, &planner.settings.max_feedrate_mm_s[E_AXIS + 4], 1, 999);
            #if E_STEPPERS > 5
              MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VMAX MSG_E6, &planner.settings.max_feedrate_mm_s[E_AXIS + 5], 1, 999);
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
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_A, &planner.settings.max_acceleration_mm_per_s2[A_AXIS], 100, 99000, _reset_acceleration_rates);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_B, &planner.settings.max_acceleration_mm_per_s2[B_AXIS], 100, 99000, _reset_acceleration_rates);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_C, &planner.settings.max_acceleration_mm_per_s2[C_AXIS], 10, 99000, _reset_acceleration_rates);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + active_extruder], 100, 99000, _reset_acceleration_rates);
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E1, &planner.settings.max_acceleration_mm_per_s2[E_AXIS], 100, 99000, _reset_e0_acceleration_rate);
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E2, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + 1], 100, 99000, _reset_e1_acceleration_rate);
      #if E_STEPPERS > 2
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E3, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + 2], 100, 99000, _reset_e2_acceleration_rate);
        #if E_STEPPERS > 3
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E4, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + 3], 100, 99000, _reset_e3_acceleration_rate);
          #if E_STEPPERS > 4
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E5, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + 4], 100, 99000, _reset_e4_acceleration_rate);
            #if E_STEPPERS > 5
              MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E6, &planner.settings.max_acceleration_mm_per_s2[E_AXIS + 5], 100, 99000, _reset_e5_acceleration_rate);
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
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VA_JERK, &planner.max_jerk[A_AXIS], 1, 990);
      MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VB_JERK, &planner.max_jerk[B_AXIS], 1, 990);
      #if ENABLED(DELTA)
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VC_JERK, &planner.max_jerk[C_AXIS], 1, 990);
      #else
        MENU_MULTIPLIER_ITEM_EDIT(float52sign, MSG_VC_JERK, &planner.max_jerk[C_AXIS], 0.1f, 990);
      #endif
      #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_VE_JERK, &planner.max_jerk[E_AXIS], 1, 990);
      #endif
    #endif

    END_MENU();
  }

  // M92 Steps-per-mm
  void menu_advanced_steps_per_mm() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_ASTEPS, &planner.settings.axis_steps_per_mm[A_AXIS], 5, 9999, _planner_refresh_positioning);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_BSTEPS, &planner.settings.axis_steps_per_mm[B_AXIS], 5, 9999, _planner_refresh_positioning);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_CSTEPS, &planner.settings.axis_steps_per_mm[C_AXIS], 5, 9999, _planner_refresh_positioning);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.settings.axis_steps_per_mm[E_AXIS + active_extruder], 5, 9999, _planner_refresh_positioning);
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E1STEPS, &planner.settings.axis_steps_per_mm[E_AXIS], 5, 9999, _planner_refresh_e0_positioning);
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E2STEPS, &planner.settings.axis_steps_per_mm[E_AXIS + 1], 5, 9999, _planner_refresh_e1_positioning);
      #if E_STEPPERS > 2
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E3STEPS, &planner.settings.axis_steps_per_mm[E_AXIS + 2], 5, 9999, _planner_refresh_e2_positioning);
        #if E_STEPPERS > 3
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E4STEPS, &planner.settings.axis_steps_per_mm[E_AXIS + 3], 5, 9999, _planner_refresh_e3_positioning);
          #if E_STEPPERS > 4
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E5STEPS, &planner.settings.axis_steps_per_mm[E_AXIS + 4], 5, 9999, _planner_refresh_e4_positioning);
            #if E_STEPPERS > 5
              MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_E6STEPS, &planner.settings.axis_steps_per_mm[E_AXIS + 5], 5, 9999, _planner_refresh_e5_positioning);
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #else
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.settings.axis_steps_per_mm[E_AXIS], 5, 9999, _planner_refresh_positioning);
    #endif

    END_MENU();
  }

#endif // !SLIM_LCD_MENUS

/**
 *
 * "Advanced Settings" submenu
 *
 */

#if HAS_M206_COMMAND
  /**
   * Set the home offset based on the current_position
   */
  void lcd_set_home_offsets() {
    // M428 Command
    enqueue_and_echo_commands_P(PSTR("M428"));
    lcd_return_to_status();
  }
#endif

#if ENABLED(SD_FIRMWARE_UPDATE)
  /**
   * Toggle the SD Firmware Update state in EEPROM
   */
  static void _lcd_toggle_sd_update() {
    const bool new_state = !settings.sd_update_status();
    lcd_completion_feedback(settings.set_sd_update_status(new_state));
    lcd_return_to_status();
    if (new_state) LCD_MESSAGEPGM(MSG_RESET_PRINTER); else lcd_reset_status();
  }
#endif

void menu_advanced_settings() {
  START_MENU();
  MENU_BACK(MSG_CONFIGURATION);

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
  #elif HAS_BED_PROBE
    MENU_ITEM_EDIT(float52, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
  #endif

  #if DISABLED(SLIM_LCD_MENUS)

    #if HAS_M206_COMMAND
      //
      // Set Home Offsets
      //
      MENU_ITEM(function, MSG_SET_HOME_OFFSETS, lcd_set_home_offsets);
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

  MENU_ITEM(submenu, MSG_TEMPERATURE, menu_advanced_temperature);

  #if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
    MENU_ITEM(submenu, MSG_FILAMENT, menu_advanced_filament);
  #elif ENABLED(LIN_ADVANCE)
    #if EXTRUDERS == 1
      MENU_ITEM_EDIT(float52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 999);
    #elif EXTRUDERS > 1
      MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E1, &planner.extruder_advance_K[0], 0, 999);
      MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E2, &planner.extruder_advance_K[1], 0, 999);
      #if EXTRUDERS > 2
        MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E3, &planner.extruder_advance_K[2], 0, 999);
        #if EXTRUDERS > 3
          MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E4, &planner.extruder_advance_K[3], 0, 999);
          #if EXTRUDERS > 4
            MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E5, &planner.extruder_advance_K[4], 0, 999);
            #if EXTRUDERS > 5
              MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E6, &planner.extruder_advance_K[5], 0, 999);
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

#if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
  /**
   *
   * "Advanced Settings" > "Filament" submenu
   *
   */
  void menu_advanced_filament() {
    START_MENU();
    MENU_BACK(MSG_ADVANCED_SETTINGS);

    #if ENABLED(LIN_ADVANCE)
      #if EXTRUDERS == 1
        MENU_ITEM_EDIT(float52, MSG_ADVANCE_K, &planner.extruder_advance_K[0], 0, 999);
      #elif EXTRUDERS > 1
        MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E1, &planner.extruder_advance_K[0], 0, 999);
        MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E2, &planner.extruder_advance_K[1], 0, 999);
        #if EXTRUDERS > 2
          MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E3, &planner.extruder_advance_K[2], 0, 999);
          #if EXTRUDERS > 3
            MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E4, &planner.extruder_advance_K[3], 0, 999);
            #if EXTRUDERS > 4
              MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E5, &planner.extruder_advance_K[4], 0, 999);
              #if EXTRUDERS > 5
                MENU_ITEM_EDIT(float52, MSG_ADVANCE_K MSG_E6, &planner.extruder_advance_K[5], 0, 999);
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
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM, &planner.filament_size[active_extruder], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E1, &planner.filament_size[0], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E2, &planner.filament_size[1], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
          #if EXTRUDERS > 2
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E3, &planner.filament_size[2], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
            #if EXTRUDERS > 3
              MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E4, &planner.filament_size[3], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
              #if EXTRUDERS > 4
                MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E5, &planner.filament_size[4], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
                #if EXTRUDERS > 5
                  MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E6, &planner.filament_size[5], 1.5f, 3.25f, planner.calculate_volumetric_multipliers);
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
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD, &fc_settings[active_extruder].unload_length, 0, extrude_maxlength);
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E1, &fc_settings[0].unload_length, 0, extrude_maxlength);
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E2, &fc_settings[1].unload_length, 0, extrude_maxlength);
        #if EXTRUDERS > 2
          MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E3, &fc_settings[2].unload_length, 0, extrude_maxlength);
          #if EXTRUDERS > 3
            MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E4, &fc_settings[3].unload_length, 0, extrude_maxlength);
            #if EXTRUDERS > 4
              MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E5, &fc_settings[4].unload_length, 0, extrude_maxlength);
              #if EXTRUDERS > 5
                MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_UNLOAD MSG_DIAM_E6, &fc_settings[5].unload_length, 0, extrude_maxlength);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1

      #if EXTRUDERS == 1
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD, &fc_settings[0].load_length, 0, extrude_maxlength);
      #else // EXTRUDERS > 1
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD, &fc_settings[active_extruder].load_length, 0, extrude_maxlength);
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E1, &fc_settings[0].load_length, 0, extrude_maxlength);
        MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E2, &fc_settings[1].load_length, 0, extrude_maxlength);
        #if EXTRUDERS > 2
          MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E3, &fc_settings[2].load_length, 0, extrude_maxlength);
          #if EXTRUDERS > 3
            MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E4, &fc_settings[3].load_length, 0, extrude_maxlength);
            #if EXTRUDERS > 4
              MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E5, &fc_settings[4].load_length, 0, extrude_maxlength);
              #if EXTRUDERS > 5
                MENU_MULTIPLIER_ITEM_EDIT(float3, MSG_FILAMENT_LOAD MSG_DIAM_E6, &fc_settings[5].load_length, 0, extrude_maxlength);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1
    #endif

    END_MENU();
  }
#endif // !NO_VOLUMETRICS || ADVANCED_PAUSE_FEATURE

/**
 *
 * "Configuration" > "Retract" submenu
 *
 */
#if ENABLED(FWRETRACT)

  void menu_config_retract() {
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    #if ENABLED(FWRETRACT_AUTORETRACT)
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_AUTORETRACT, &fwretract.autoretract_enabled, fwretract.refresh_autoretract);
    #endif
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT, &fwretract.settings.retract_length, 0, 100);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_SWAP, &fwretract.settings.swap_retract_length, 0, 100);
    #endif
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACTF, &fwretract.settings.retract_feedrate_mm_s, 1, 999);
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_ZHOP, &fwretract.settings.retract_zraise, 0, 999);
    MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_RECOVER, &fwretract.settings.retract_recover_length, -100, 100);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float52sign, MSG_CONTROL_RETRACT_RECOVER_SWAP, &fwretract.settings.swap_retract_recover_length, -100, 100);
    #endif
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVERF, &fwretract.settings.retract_recover_feedrate_mm_s, 1, 999);
    #if EXTRUDERS > 1
      MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVER_SWAPF, &fwretract.settings.swap_retract_recover_feedrate_mm_s, 1, 999);
    #endif
    END_MENU();
  }

#endif // FWRETRACT

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

#if ENABLED(LCD_INFO_MENU)

  #if ENABLED(PRINTCOUNTER)
    /**
     *
     * About Printer > Statistics submenu
     *
     */
    void menu_info_stats() {
      if (use_click()) { return lcd_goto_previous_menu(); }

      char buffer[21];
      printStatistics stats = print_job_timer.getStats();

      START_SCREEN();                                                                                // 12345678901234567890
      STATIC_ITEM(MSG_INFO_PRINT_COUNT ": ", false, false, itostr3left(stats.totalPrints));          // Print Count: 999
      STATIC_ITEM(MSG_INFO_COMPLETED_PRINTS": ", false, false, itostr3left(stats.finishedPrints));   // Completed  : 666

      duration_t elapsed = stats.printTime;
      elapsed.toString(buffer);

      STATIC_ITEM(MSG_INFO_PRINT_TIME ": ", false, false);                                           // Total print Time:
      STATIC_ITEM("", false, false, buffer);                                                         // 99y 364d 23h 59m 59s

      elapsed = stats.longestPrint;
      elapsed.toString(buffer);

      STATIC_ITEM(MSG_INFO_PRINT_LONGEST ": ", false, false);                                        // Longest job time:
      STATIC_ITEM("", false, false, buffer);                                                         // 99y 364d 23h 59m 59s

      sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int16_t(stats.filamentUsed / 100) % 10);
      STATIC_ITEM(MSG_INFO_PRINT_FILAMENT ": ", false, false);                                       // Extruded total:
      STATIC_ITEM("", false, false, buffer);                                                         // 125m
      END_SCREEN();
    }
  #endif // PRINTCOUNTER

  /**
   *
   * About Printer > Thermistors
   *
   */
  void menu_info_thermistors() {
    if (use_click()) { return lcd_goto_previous_menu(); }
    START_SCREEN();
    #define THERMISTOR_ID TEMP_SENSOR_0
    #include "../thermistornames.h"
    STATIC_ITEM("T0: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_0_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_0_MAXTEMP), false);

    #if TEMP_SENSOR_1 != 0
      #undef THERMISTOR_ID
      #define THERMISTOR_ID TEMP_SENSOR_1
      #include "../thermistornames.h"
      STATIC_ITEM("T1: " THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_1_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_1_MAXTEMP), false);
    #endif

    #if TEMP_SENSOR_2 != 0
      #undef THERMISTOR_ID
      #define THERMISTOR_ID TEMP_SENSOR_2
      #include "../thermistornames.h"
      STATIC_ITEM("T2: " THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_2_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_2_MAXTEMP), false);
    #endif

    #if TEMP_SENSOR_3 != 0
      #undef THERMISTOR_ID
      #define THERMISTOR_ID TEMP_SENSOR_3
      #include "../thermistornames.h"
      STATIC_ITEM("T3: " THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_3_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_3_MAXTEMP), false);
    #endif

    #if TEMP_SENSOR_4 != 0
      #undef THERMISTOR_ID
      #define THERMISTOR_ID TEMP_SENSOR_4
      #include "../thermistornames.h"
      STATIC_ITEM("T4: " THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_4_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_4_MAXTEMP), false);
    #endif

    #if HAS_HEATED_BED
      #undef THERMISTOR_ID
      #define THERMISTOR_ID TEMP_SENSOR_BED
      #include "../thermistornames.h"
      STATIC_ITEM("TBed:" THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(BED_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(BED_MAXTEMP), false);
    #endif
    END_SCREEN();
  }

  /**
   *
   * About Printer > Board Info
   *
   */
  void menu_info_board() {
    if (use_click()) { return lcd_goto_previous_menu(); }
    START_SCREEN();
    STATIC_ITEM(BOARD_NAME, true, true);                           // MyPrinterController
    STATIC_ITEM(MSG_INFO_BAUDRATE ": " STRINGIFY(BAUDRATE), true); // Baud: 250000
    STATIC_ITEM(MSG_INFO_PROTOCOL ": " PROTOCOL_VERSION, true);    // Protocol: 1.0
    #if POWER_SUPPLY == 0
      STATIC_ITEM(MSG_INFO_PSU ": Generic", true);
    #elif POWER_SUPPLY == 1
      STATIC_ITEM(MSG_INFO_PSU ": ATX", true);  // Power Supply: ATX
    #elif POWER_SUPPLY == 2
      STATIC_ITEM(MSG_INFO_PSU ": XBox", true); // Power Supply: XBox
    #endif
    END_SCREEN();
  }

  /**
   *
   * About Printer > Printer Info
   *
   */
  void menu_info_printer() {
    if (use_click()) { return lcd_goto_previous_menu(); }
    START_SCREEN();
    STATIC_ITEM(MSG_MARLIN, true, true);                             // Marlin
    STATIC_ITEM(SHORT_BUILD_VERSION, true);                          // x.x.x-Branch
    STATIC_ITEM(STRING_DISTRIBUTION_DATE, true);                     // YYYY-MM-DD HH:MM
    STATIC_ITEM(MACHINE_NAME, true);                                 // My3DPrinter
    STATIC_ITEM(WEBSITE_URL, true);                                  // www.my3dprinter.com
    STATIC_ITEM(MSG_INFO_EXTRUDERS ": " STRINGIFY(EXTRUDERS), true); // Extruders: 2
    #if ENABLED(AUTO_BED_LEVELING_3POINT)
      STATIC_ITEM(MSG_3POINT_LEVELING, true);                        // 3-Point Leveling
    #elif ENABLED(AUTO_BED_LEVELING_LINEAR)
      STATIC_ITEM(MSG_LINEAR_LEVELING, true);                        // Linear Leveling
    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
      STATIC_ITEM(MSG_BILINEAR_LEVELING, true);                      // Bi-linear Leveling
    #elif ENABLED(AUTO_BED_LEVELING_UBL)
      STATIC_ITEM(MSG_UBL_LEVELING, true);                           // Unified Bed Leveling
    #elif ENABLED(MESH_BED_LEVELING)
      STATIC_ITEM(MSG_MESH_LEVELING, true);                          // Mesh Leveling
    #endif
    END_SCREEN();
  }

  /**
   *
   * "About Printer" submenu
   *
   */
  void menu_info() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    MENU_ITEM(submenu, MSG_INFO_PRINTER_MENU, menu_info_printer);        // Printer Info >
    MENU_ITEM(submenu, MSG_INFO_BOARD_MENU, menu_info_board);            // Board Info >
    MENU_ITEM(submenu, MSG_INFO_THERMISTOR_MENU, menu_info_thermistors); // Thermistors >
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(submenu, MSG_INFO_STATS_MENU, menu_info_stats);          // Printer Statistics >
    #endif
    END_MENU();
  }
#endif // LCD_INFO_MENU

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
