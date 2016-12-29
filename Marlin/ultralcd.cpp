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

#include "ultralcd.h"
#if ENABLED(ULTRA_LCD)
#include "Marlin.h"
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "stepper.h"
#include "configuration_store.h"
#include "utility.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "buzzer.h"
#endif

#if ENABLED(BLTOUCH)
  #include "endstops.h"
#endif

#if ENABLED(PRINTCOUNTER)
  #include "printcounter.h"
  #include "duration_t.h"
#endif

int lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

#if ENABLED(FILAMENT_LCD_DISPLAY)
  millis_t previous_lcd_status_ms = 0;
#endif

uint8_t lcd_status_message_level;
char lcd_status_message[3 * (LCD_WIDTH) + 1] = WELCOME_MSG; // worst case is kana with up to 3*LCD_WIDTH+1

#if ENABLED(DOGLCD)
  #include "ultralcd_impl_DOGM.h"
#else
  #include "ultralcd_impl_HD44780.h"
#endif

// The main status screen
void lcd_status_screen();

millis_t next_lcd_update_ms;

uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; // Set when the LCD needs to draw, decrements after every draw. Set to 2 in LCD routines so the LCD gets at least 1 full redraw (first redraw is partial)
uint16_t max_display_update_time = 0;

#if ENABLED(DOGLCD)
  bool drawing_screen = false;
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "stepper_dac.h" //was dac_mcp4728.h MarlinMain uses stepper dac for the m-codes
  uint16_t driverPercent[XYZE];
#endif

#if ENABLED(ULTIPANEL)

  // place-holders for Ki and Kd edits
  float raw_Ki, raw_Kd;

  /**
   * REVERSE_MENU_DIRECTION
   *
   * To reverse the menu direction we need a general way to reverse
   * the direction of the encoder everywhere. So encoderDirection is
   * added to allow the encoder to go the other way.
   *
   * This behavior is limited to scrolling Menus and SD card listings,
   * and is disabled in other contexts.
   */
  #if ENABLED(REVERSE_MENU_DIRECTION)
    int8_t encoderDirection = 1;
    #define ENCODER_DIRECTION_NORMAL() (encoderDirection = 1)
    #define ENCODER_DIRECTION_MENUS() (encoderDirection = -1)
  #else
    #define ENCODER_DIRECTION_NORMAL() ;
    #define ENCODER_DIRECTION_MENUS() ;
  #endif

  int8_t encoderDiff; // updated from interrupt context and added to encoderPosition every LCD update

  millis_t manual_move_start_time = 0;
  int8_t manual_move_axis = (int8_t)NO_AXIS;
  #if EXTRUDERS > 1
    int8_t manual_move_e_index = 0;
  #else
    #define manual_move_e_index 0
  #endif

  bool encoderRateMultiplierEnabled;
  int32_t lastEncoderMovementMillis;

  #if HAS_POWER_SWITCH
    extern bool powersupply;
  #endif

  const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;
  void lcd_main_menu();
  void lcd_tune_menu();
  void lcd_prepare_menu();
  void lcd_move_menu();
  void lcd_control_menu();
  void lcd_control_temperature_menu();
  void lcd_control_temperature_preheat_material1_settings_menu();
  void lcd_control_temperature_preheat_material2_settings_menu();
  void lcd_control_motion_menu();
  void lcd_control_volumetric_menu();

  #if ENABLED(DAC_STEPPER_CURRENT)
    void dac_driver_commit();
    void dac_driver_getValues();
    void lcd_dac_menu();
    void lcd_dac_write_eeprom();
  #endif

  #if ENABLED(LCD_INFO_MENU)
    #if ENABLED(PRINTCOUNTER)
      void lcd_info_stats_menu();
    #endif
    void lcd_info_thermistors_menu();
    void lcd_info_board_menu();
    void lcd_info_menu();
  #endif // LCD_INFO_MENU

  #if ENABLED(FILAMENT_CHANGE_FEATURE)
    void lcd_filament_change_toocold_menu();
    void lcd_filament_change_option_menu();
    void lcd_filament_change_init_message();
    void lcd_filament_change_unload_message();
    void lcd_filament_change_insert_message();
    void lcd_filament_change_load_message();
    void lcd_filament_change_extrude_message();
    void lcd_filament_change_resume_message();
  #endif

  #if HAS_LCD_CONTRAST
    void lcd_set_contrast();
  #endif

  #if ENABLED(FWRETRACT)
    void lcd_control_retract_menu();
  #endif

  #if ENABLED(DELTA_CALIBRATION_MENU)
    void lcd_delta_calibrate_menu();
  #endif

  #if ENABLED(MANUAL_BED_LEVELING)
    #include "mesh_bed_leveling.h"
  #endif

  // Function pointer to menu functions.
  typedef void (*screenFunc_t)();

  // Different types of actions that can be used in menu items.
  #define menu_action_back(dummy) _menu_action_back()
  void _menu_action_back();
  void menu_action_submenu(screenFunc_t data);
  void menu_action_gcode(const char* pgcode);
  void menu_action_function(screenFunc_t data);
  void menu_action_setting_edit_bool(const char* pstr, bool* ptr);
  void menu_action_setting_edit_int3(const char* pstr, int* ptr, int minValue, int maxValue);
  void menu_action_setting_edit_float3(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float32(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float43(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float5(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float51(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float52(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_float62(const char* pstr, float* ptr, float minValue, float maxValue);
  void menu_action_setting_edit_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue);
  void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_int3(const char* pstr, int* ptr, int minValue, int maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float3(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float32(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float43(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float5(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float51(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float52(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_float62(const char* pstr, float* ptr, float minValue, float maxValue, screenFunc_t callbackFunc);
  void menu_action_setting_edit_callback_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue, screenFunc_t callbackFunc);

  #if ENABLED(SDSUPPORT)
    void lcd_sdcard_menu();
    void menu_action_sdfile(const char* filename, char* longFilename);
    void menu_action_sddirectory(const char* filename, char* longFilename);
  #endif

  /* Helper macros for menus */

  #ifndef ENCODER_FEEDRATE_DEADZONE
    #define ENCODER_FEEDRATE_DEADZONE 10
  #endif
  #ifndef ENCODER_STEPS_PER_MENU_ITEM
    #define ENCODER_STEPS_PER_MENU_ITEM 5
  #endif
  #ifndef ENCODER_PULSES_PER_STEP
    #define ENCODER_PULSES_PER_STEP 1
  #endif

  #ifndef TALL_FONT_CORRECTION
    #define TALL_FONT_CORRECTION 0
  #endif

  /**
   * START_SCREEN_OR_MENU generates init code for a screen or menu
   *
   *   encoderLine is the position based on the encoder
   *   encoderTopLine is the top menu line to display
   *   _lcdLineNr is the index of the LCD line (e.g., 0-3)
   *   _menuLineNr is the menu item to draw and process
   *   _thisItemNr is the index of each MENU_ITEM or STATIC_ITEM
   *   _countedItems is the total number of items in the menu (after one call)
   */
  #define START_SCREEN_OR_MENU(LIMIT) \
    ENCODER_DIRECTION_MENUS(); \
    encoderRateMultiplierEnabled = false; \
    if (encoderPosition > 0x8000) encoderPosition = 0; \
    static int8_t _countedItems = 0; \
    int8_t encoderLine = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM; \
    if (_countedItems > 0 && encoderLine >= _countedItems - LIMIT) { \
      encoderLine = max(0, _countedItems - LIMIT); \
      encoderPosition = encoderLine * (ENCODER_STEPS_PER_MENU_ITEM); \
    }

  #define SCREEN_OR_MENU_LOOP() \
    int8_t _menuLineNr = encoderTopLine, _thisItemNr; \
    for (int8_t _lcdLineNr = 0; _lcdLineNr < LCD_HEIGHT - TALL_FONT_CORRECTION; _lcdLineNr++, _menuLineNr++) { \
      _thisItemNr = 0

  /**
   * START_SCREEN  Opening code for a screen having only static items.
   *               Do simplified scrolling of the entire screen.
   *
   * START_MENU    Opening code for a screen with menu items.
   *               Scroll as-needed to keep the selected line in view.
   */
  #define START_SCREEN() \
    START_SCREEN_OR_MENU(LCD_HEIGHT - TALL_FONT_CORRECTION); \
    encoderTopLine = encoderLine; \
    bool _skipStatic = false; \
    SCREEN_OR_MENU_LOOP()

  #define START_MENU() \
    START_SCREEN_OR_MENU(1); \
    screen_changed = false; \
    NOMORE(encoderTopLine, encoderLine); \
    if (encoderLine >= encoderTopLine + LCD_HEIGHT - TALL_FONT_CORRECTION) { \
      encoderTopLine = encoderLine - (LCD_HEIGHT - TALL_FONT_CORRECTION - 1); \
    } \
    bool _skipStatic = true; \
    SCREEN_OR_MENU_LOOP()

  /**
   * MENU_ITEM generates draw & handler code for a menu item, potentially calling:
   *
   *   lcd_implementation_drawmenu_[type](sel, row, label, arg3...)
   *   menu_action_[type](arg3...)
   *
   * Examples:
   *   MENU_ITEM(back, MSG_WATCH, 0 [dummy parameter] )
   *   or
   *   MENU_BACK(MSG_WATCH)
   *     lcd_implementation_drawmenu_back(sel, row, PSTR(MSG_WATCH))
   *     menu_action_back()
   *
   *   MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause)
   *     lcd_implementation_drawmenu_function(sel, row, PSTR(MSG_PAUSE_PRINT), lcd_sdcard_pause)
   *     menu_action_function(lcd_sdcard_pause)
   *
   *   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
   *   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
   *     lcd_implementation_drawmenu_setting_edit_int3(sel, row, PSTR(MSG_SPEED), PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
   *     menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
   *
   */
  #define _MENU_ITEM_PART_1(TYPE, LABEL, ...) \
    if (_menuLineNr == _thisItemNr) { \
      if (lcdDrawUpdate) \
        lcd_implementation_drawmenu_ ## TYPE(encoderLine == _thisItemNr, _lcdLineNr, PSTR(LABEL), ## __VA_ARGS__); \
      if (lcd_clicked && encoderLine == _thisItemNr) {

  #define _MENU_ITEM_PART_2(TYPE, ...) \
        menu_action_ ## TYPE(__VA_ARGS__); \
        if (screen_changed) return; \
      } \
    } \
    ++_thisItemNr

  #define MENU_ITEM(TYPE, LABEL, ...) do { \
      _skipStatic = false; \
      _MENU_ITEM_PART_1(TYPE, LABEL, ## __VA_ARGS__); \
      _MENU_ITEM_PART_2(TYPE, ## __VA_ARGS__); \
    } while(0)

  #define MENU_BACK(LABEL) MENU_ITEM(back, LABEL, 0)

  // Used to print static text with no visible cursor.
  // Parameters: label [, bool center [, bool invert [, char *value] ] ]
  #define STATIC_ITEM(LABEL, ...) \
    if (_menuLineNr == _thisItemNr) { \
      if (_skipStatic && encoderLine <= _thisItemNr) { \
        encoderPosition += ENCODER_STEPS_PER_MENU_ITEM; \
        lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT; \
      } \
      if (lcdDrawUpdate) \
        lcd_implementation_drawmenu_static(_lcdLineNr, PSTR(LABEL), ## __VA_ARGS__); \
    } \
    ++_thisItemNr

  #define END_SCREEN() \
    } \
    _countedItems = _thisItemNr

  #define END_MENU() \
    } \
    _countedItems = _thisItemNr; \
    UNUSED(_skipStatic)

  #if ENABLED(ENCODER_RATE_MULTIPLIER)

    //#define ENCODER_RATE_MULTIPLIER_DEBUG  // If defined, output the encoder steps per second value

    /**
     * MENU_MULTIPLIER_ITEM generates drawing and handling code for a multiplier menu item
     */
    #define MENU_MULTIPLIER_ITEM(type, label, ...) do { \
        _MENU_ITEM_PART_1(type, label, ## __VA_ARGS__); \
        encoderRateMultiplierEnabled = true; \
        lastEncoderMovementMillis = 0; \
        _MENU_ITEM_PART_2(type, ## __VA_ARGS__); \
      } while(0)

  #endif //ENCODER_RATE_MULTIPLIER

  #define MENU_ITEM_DUMMY() do { _thisItemNr++; } while(0)
  #define MENU_ITEM_EDIT(type, label, ...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label), ## __VA_ARGS__)
  #define MENU_ITEM_EDIT_CALLBACK(type, label, ...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## __VA_ARGS__)
  #if ENABLED(ENCODER_RATE_MULTIPLIER)
    #define MENU_MULTIPLIER_ITEM_EDIT(type, label, ...) MENU_MULTIPLIER_ITEM(setting_edit_ ## type, label, PSTR(label), ## __VA_ARGS__)
    #define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(type, label, ...) MENU_MULTIPLIER_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## __VA_ARGS__)
  #else //!ENCODER_RATE_MULTIPLIER
    #define MENU_MULTIPLIER_ITEM_EDIT(type, label, ...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label), ## __VA_ARGS__)
    #define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(type, label, ...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## __VA_ARGS__)
  #endif //!ENCODER_RATE_MULTIPLIER

  /** Used variables to keep track of the menu */
  volatile uint8_t buttons;  //the last checked buttons in a bit array.
  #if ENABLED(REPRAPWORLD_KEYPAD)
    volatile uint8_t buttons_reprapworld_keypad; // to store the keypad shift register values
  #endif

  #if ENABLED(LCD_HAS_SLOW_BUTTONS)
    volatile uint8_t slow_buttons; // Bits of the pressed buttons.
  #endif
  int8_t encoderTopLine;              /* scroll offset in the current menu */
  millis_t next_button_update_ms;
  uint8_t lastEncoderBits;
  uint32_t encoderPosition;
  #if PIN_EXISTS(SD_DETECT)
    uint8_t lcd_sd_status;
  #endif

  typedef struct {
    screenFunc_t menu_function;
    uint32_t encoder_position;
  } menuPosition;

  screenFunc_t currentScreen = lcd_status_screen; // pointer to the currently active menu handler

  menuPosition screen_history[10];
  uint8_t screen_history_depth = 0;
  bool screen_changed;

  // LCD and menu clicks
  bool lcd_clicked, wait_for_unclick, defer_return_to_status, no_reentrance;

  // Variables used when editing values.
  const char* editLabel;
  void* editValue;
  int32_t minEditValue, maxEditValue;
  screenFunc_t callbackFunc;              // call this after editing

  /**
   * General function to go directly to a screen
   */
  void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder = 0) {
    if (currentScreen != screen) {
      currentScreen = screen;
      encoderPosition = encoder;
      if (screen == lcd_status_screen) {
        defer_return_to_status = false;
        screen_history_depth = 0;
      }
      lcd_implementation_clear();
      #if ENABLED(LCD_PROGRESS_BAR)
        // For LCD_PROGRESS_BAR re-initialize custom characters
        lcd_set_custom_characters(screen == lcd_status_screen);
      #endif
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
      screen_changed = true;
      #if ENABLED(DOGLCD)
        drawing_screen = false;
      #endif
    }
  }

  /**
   * Synchronize safely while holding the current screen
   * This blocks all further screen or stripe updates once called
   */
  inline void lcd_synchronize() {
    lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, PSTR(MSG_MOVING));
    if (no_reentrance) return;
    no_reentrance = true;
    screenFunc_t old_screen = currentScreen;
    lcd_goto_screen(lcd_synchronize);
    stepper.synchronize();
    no_reentrance = false;
    lcd_goto_screen(old_screen);
  }

  inline void lcd_wait_for_homing() {
    no_reentrance = true;
    while (!axis_homed[X_AXIS] || !axis_homed[Y_AXIS] || !axis_homed[Z_AXIS]) idle();
    no_reentrance = true;
  }

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

#endif // ULTIPANEL

/**
 *
 * "Info Screen"
 *
 * This is very display-dependent, so the lcd implementation draws this.
 */

void lcd_status_screen() {

  #if ENABLED(ULTIPANEL)
    ENCODER_DIRECTION_NORMAL();
    encoderRateMultiplierEnabled = false;
  #endif

  #if ENABLED(LCD_PROGRESS_BAR)
    millis_t ms = millis();
    #if DISABLED(PROGRESS_MSG_ONCE)
      if (ELAPSED(ms, progress_bar_ms + PROGRESS_BAR_MSG_TIME + PROGRESS_BAR_BAR_TIME)) {
        progress_bar_ms = ms;
      }
    #endif
    #if PROGRESS_MSG_EXPIRE > 0
      // Handle message expire
      if (expire_status_ms > 0) {
        #if ENABLED(SDSUPPORT)
          if (card.isFileOpen()) {
            // Expire the message when printing is active
            if (IS_SD_PRINTING) {
              if (ELAPSED(ms, expire_status_ms)) {
                lcd_status_message[0] = '\0';
                expire_status_ms = 0;
              }
            }
            else {
              expire_status_ms += LCD_UPDATE_INTERVAL;
            }
          }
          else {
            expire_status_ms = 0;
          }
        #else
          expire_status_ms = 0;
        #endif //SDSUPPORT
      }
    #endif
  #endif //LCD_PROGRESS_BAR

  lcd_implementation_status_screen();

  #if ENABLED(ULTIPANEL)

    if (lcd_clicked) {
      #if ENABLED(FILAMENT_LCD_DISPLAY)
        previous_lcd_status_ms = millis();  // get status message to show up for a while
      #endif
      lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
        #if ENABLED(LCD_PROGRESS_BAR)
          false
        #endif
      );
      lcd_goto_screen(lcd_main_menu);
    }

    #if ENABLED(ULTIPANEL_FEEDMULTIPLY)
      int new_frm = feedrate_percentage + (int32_t)encoderPosition;
      // Dead zone at 100% feedrate
      if ((feedrate_percentage < 100 && new_frm > 100) || (feedrate_percentage > 100 && new_frm < 100)) {
        feedrate_percentage = 100;
        encoderPosition = 0;
      }
      else if (feedrate_percentage == 100) {
        if ((int32_t)encoderPosition > ENCODER_FEEDRATE_DEADZONE) {
          feedrate_percentage += (int32_t)encoderPosition - (ENCODER_FEEDRATE_DEADZONE);
          encoderPosition = 0;
        }
        else if ((int32_t)encoderPosition < -(ENCODER_FEEDRATE_DEADZONE)) {
          feedrate_percentage += (int32_t)encoderPosition + ENCODER_FEEDRATE_DEADZONE;
          encoderPosition = 0;
        }
      }
      else {
        feedrate_percentage = new_frm;
        encoderPosition = 0;
      }
    #endif // ULTIPANEL_FEEDMULTIPLY

    feedrate_percentage = constrain(feedrate_percentage, 10, 999);

  #endif //ULTIPANEL
}

/**
 *
 * draw the kill screen
 *
 */
void kill_screen(const char* lcd_msg) {
  lcd_init();
  lcd_setalertstatuspgm(lcd_msg);
  #if ENABLED(DOGLCD)
    u8g.firstPage();
    do {
      lcd_kill_screen();
    } while (u8g.nextPage());
  #else
    lcd_kill_screen();
  #endif
}

#if ENABLED(ULTIPANEL)

  inline void line_to_current(AxisEnum axis) {
    planner.buffer_line_kinematic(current_position, MMM_TO_MMS(manual_feedrate_mm_m[axis]), active_extruder);
  }

  #if ENABLED(SDSUPPORT)

    void lcd_sdcard_pause() {
      card.pauseSDPrint();
      print_job_timer.pause();
    }

    void lcd_sdcard_resume() {
      card.startFileprint();
      print_job_timer.start();
    }

    void lcd_sdcard_stop() {
      card.stopSDPrint();
      clear_command_queue();
      quickstop_stepper();
      print_job_timer.stop();
      #if ENABLED(AUTOTEMP)
        thermalManager.autotempShutdown();
      #endif
      wait_for_heatup = false;
      lcd_setstatus(MSG_PRINT_ABORTED, true);
    }

  #endif //SDSUPPORT

  #if ENABLED(MENU_ITEM_CASE_LIGHT)

    extern bool case_light_on;
    extern void update_case_light();

    void toggle_case_light() {
      case_light_on = !case_light_on;
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
      update_case_light();
    }

  #endif // MENU_ITEM_CASE_LIGHT

  #if ENABLED(LCD_PROGRESS_BAR_TEST)

    static void progress_bar_test() {
      static int8_t bar_percent = 0;
      if (lcd_clicked) {
        lcd_goto_previous_menu();
        lcd_set_custom_characters(false);
        return;
      }
      bar_percent += (int8_t)encoderPosition;
      bar_percent = constrain(bar_percent, 0, 100);
      encoderPosition = 0;
      lcd_implementation_drawmenu_static(0, PSTR(MSG_PROGRESS_BAR_TEST), true, true);
      lcd.setCursor((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2);
      lcd.print(itostr3(bar_percent)); lcd.print('%');
      lcd.setCursor(0, LCD_HEIGHT - 1); lcd_draw_progress_bar(bar_percent);
    }

    void _progress_bar_test() {
      lcd_goto_screen(progress_bar_test);
      lcd_set_custom_characters();
    }

  #endif // LCD_PROGRESS_BAR_TEST

  #if HAS_DEBUG_MENU

    void lcd_debug_menu() {
      START_MENU();

      MENU_BACK(MSG_MAIN); // ^ Main

      #if ENABLED(LCD_PROGRESS_BAR_TEST)
        MENU_ITEM(submenu, MSG_PROGRESS_BAR_TEST, _progress_bar_test);
      #endif

      END_MENU();
    }

  #endif // HAS_DEBUG_MENU

  /**
   *
   * "Main" menu
   *
   */

  void lcd_main_menu() {
    START_MENU();
    MENU_BACK(MSG_WATCH);

    //
    // Debug Menu when certain options are enabled
    //
    #if HAS_DEBUG_MENU
      MENU_ITEM(submenu, MSG_DEBUG_MENU, lcd_debug_menu);
    #endif

    //
    // Switch case light on/off
    //
    #if ENABLED(MENU_ITEM_CASE_LIGHT)
      if (case_light_on)
        MENU_ITEM(function, MSG_LIGHTS_OFF, toggle_case_light);
      else
        MENU_ITEM(function, MSG_LIGHTS_ON, toggle_case_light);
    #endif

    #if ENABLED(BLTOUCH)
      if (!endstops.z_probe_enabled && TEST_BLTOUCH())
        MENU_ITEM(gcode, MSG_BLTOUCH_RESET, PSTR("M280 P" STRINGIFY(Z_ENDSTOP_SERVO_NR) " S" STRINGIFY(BLTOUCH_RESET)));
    #endif

    if (planner.movesplanned() || IS_SD_PRINTING) {
      MENU_ITEM(submenu, MSG_TUNE, lcd_tune_menu);
    }
    else {
      MENU_ITEM(submenu, MSG_PREPARE, lcd_prepare_menu);
      #if ENABLED(DELTA_CALIBRATION_MENU)
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE, lcd_delta_calibrate_menu);
      #endif
    }
    MENU_ITEM(submenu, MSG_CONTROL, lcd_control_menu);

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
          MENU_ITEM(submenu, MSG_CARD_MENU, lcd_sdcard_menu);
          #if !PIN_EXISTS(SD_DETECT)
            MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
          #endif
        }
      }
      else {
        MENU_ITEM(submenu, MSG_NO_CARD, lcd_sdcard_menu);
        #if !PIN_EXISTS(SD_DETECT)
          MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
        #endif
      }
    #endif //SDSUPPORT

    #if ENABLED(LCD_INFO_MENU)
      MENU_ITEM(submenu, MSG_INFO_MENU, lcd_info_menu);
    #endif

    END_MENU();
  }

  /**
   *
   * "Tune" submenu items
   *
   */

  /**
   * Set the home offset based on the current_position
   */
  void lcd_set_home_offsets() {
    // M428 Command
    enqueue_and_echo_commands_P(PSTR("M428"));
    lcd_return_to_status();
  }

  #if ENABLED(BABYSTEPPING)

    long babysteps_done = 0;

    void _lcd_babystep(const AxisEnum axis, const char* msg) {
      if (lcd_clicked) { defer_return_to_status = false; return lcd_goto_previous_menu(); }
      ENCODER_DIRECTION_NORMAL();
      if (encoderPosition) {
        int babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR);
        encoderPosition = 0;
        lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
        thermalManager.babystep_axis(axis, babystep_increment);
        babysteps_done += babystep_increment;
      }
      if (lcdDrawUpdate)
        lcd_implementation_drawedit(msg, ftostr43sign(
          ((1000 * babysteps_done) * planner.steps_to_mm[axis]) * 0.001f
        ));
    }

    #if ENABLED(BABYSTEP_XY)
      void _lcd_babystep_x() { _lcd_babystep(X_AXIS, PSTR(MSG_BABYSTEPPING_X)); }
      void _lcd_babystep_y() { _lcd_babystep(Y_AXIS, PSTR(MSG_BABYSTEPPING_Y)); }
      void lcd_babystep_x() { lcd_goto_screen(_lcd_babystep_x); babysteps_done = 0; defer_return_to_status = true; }
      void lcd_babystep_y() { lcd_goto_screen(_lcd_babystep_y); babysteps_done = 0; defer_return_to_status = true; }
    #endif
    void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEPPING_Z)); }
    void lcd_babystep_z() { lcd_goto_screen(_lcd_babystep_z); babysteps_done = 0; defer_return_to_status = true; }

  #endif //BABYSTEPPING

  /**
   * Watch temperature callbacks
   */
  #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
    #if TEMP_SENSOR_0 != 0
      void watch_temp_callback_E0() { thermalManager.start_watching_heater(0); }
    #endif
    #if HOTENDS > 1 && TEMP_SENSOR_1 != 0
      void watch_temp_callback_E1() { thermalManager.start_watching_heater(1); }
    #endif // HOTENDS > 1
    #if HOTENDS > 2 && TEMP_SENSOR_2 != 0
      void watch_temp_callback_E2() { thermalManager.start_watching_heater(2); }
    #endif // HOTENDS > 2
    #if HOTENDS > 3 && TEMP_SENSOR_3 != 0
      void watch_temp_callback_E3() { thermalManager.start_watching_heater(3); }
    #endif // HOTENDS > 3
  #else
    #if TEMP_SENSOR_0 != 0
      void watch_temp_callback_E0() {}
    #endif
    #if HOTENDS > 1 && TEMP_SENSOR_1 != 0
      void watch_temp_callback_E1() {}
    #endif // HOTENDS > 1
    #if HOTENDS > 2 && TEMP_SENSOR_2 != 0
      void watch_temp_callback_E2() {}
    #endif // HOTENDS > 2
    #if HOTENDS > 3 && TEMP_SENSOR_3 != 0
      void watch_temp_callback_E3() {}
    #endif // HOTENDS > 3
  #endif

  #if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
    #if TEMP_SENSOR_BED != 0
      void watch_temp_callback_bed() { thermalManager.start_watching_bed(); }
    #endif
  #else
    #if TEMP_SENSOR_BED != 0
      void watch_temp_callback_bed() {}
    #endif
  #endif

  #if ENABLED(FILAMENT_CHANGE_FEATURE)
    void lcd_enqueue_filament_change() {
      if (!DEBUGGING(DRYRUN) && thermalManager.tooColdToExtrude(active_extruder)) {
        lcd_save_previous_screen();
        lcd_goto_screen(lcd_filament_change_toocold_menu);
        return;
      }
      lcd_filament_change_show_message(FILAMENT_CHANGE_MESSAGE_INIT);
      enqueue_and_echo_commands_P(PSTR("M600"));
    }
  #endif

  /**
   *
   * "Tune" submenu
   *
   */
  void lcd_tune_menu() {
    START_MENU();

    //
    // ^ Main
    //
    MENU_BACK(MSG_MAIN);

    //
    // Speed:
    //
    MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999);

    // Manual bed leveling, Bed Z:
    #if ENABLED(MANUAL_BED_LEVELING)
      MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
    #endif

    //
    // Nozzle:
    // Nozzle [1-4]:
    //
    #if HOTENDS == 1
      #if TEMP_SENSOR_0 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
      #endif
    #else //HOTENDS > 1
      #if TEMP_SENSOR_0 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N1, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
      #endif
      #if TEMP_SENSOR_1 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N2, &thermalManager.target_temperature[1], 0, HEATER_1_MAXTEMP - 15, watch_temp_callback_E1);
      #endif
      #if HOTENDS > 2
        #if TEMP_SENSOR_2 != 0
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N3, &thermalManager.target_temperature[2], 0, HEATER_2_MAXTEMP - 15, watch_temp_callback_E2);
        #endif
        #if HOTENDS > 3
          #if TEMP_SENSOR_3 != 0
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N4, &thermalManager.target_temperature[3], 0, HEATER_3_MAXTEMP - 15, watch_temp_callback_E3);
          #endif
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1

    //
    // Bed:
    //
    #if TEMP_SENSOR_BED != 0
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.target_temperature_bed, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
    #endif

    //
    // Fan Speed:
    //
    #if FAN_COUNT > 0
      #if HAS_FAN0
        #if FAN_COUNT > 1
          #define MSG_1ST_FAN_SPEED MSG_FAN_SPEED " 1"
        #else
          #define MSG_1ST_FAN_SPEED MSG_FAN_SPEED
        #endif
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_1ST_FAN_SPEED, &fanSpeeds[0], 0, 255);
      #endif
      #if HAS_FAN1
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED " 2", &fanSpeeds[1], 0, 255);
      #endif
      #if HAS_FAN2
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED " 3", &fanSpeeds[2], 0, 255);
      #endif
    #endif // FAN_COUNT > 0

    //
    // Flow:
    // Flow 1:
    // Flow 2:
    // Flow 3:
    // Flow 4:
    //
    #if EXTRUDERS == 1
      MENU_ITEM_EDIT(int3, MSG_FLOW, &flow_percentage[0], 10, 999);
    #else // EXTRUDERS > 1
      MENU_ITEM_EDIT(int3, MSG_FLOW, &flow_percentage[active_extruder], 10, 999);
      MENU_ITEM_EDIT(int3, MSG_FLOW MSG_N1, &flow_percentage[0], 10, 999);
      MENU_ITEM_EDIT(int3, MSG_FLOW MSG_N2, &flow_percentage[1], 10, 999);
      #if EXTRUDERS > 2
        MENU_ITEM_EDIT(int3, MSG_FLOW MSG_N3, &flow_percentage[2], 10, 999);
        #if EXTRUDERS > 3
          MENU_ITEM_EDIT(int3, MSG_FLOW MSG_N4, &flow_percentage[3], 10, 999);
        #endif //EXTRUDERS > 3
      #endif //EXTRUDERS > 2
    #endif //EXTRUDERS > 1

    //
    // Babystep X:
    // Babystep Y:
    // Babystep Z:
    //
    #if ENABLED(BABYSTEPPING)
      #if ENABLED(BABYSTEP_XY)
        MENU_ITEM(submenu, MSG_BABYSTEP_X, lcd_babystep_x);
        MENU_ITEM(submenu, MSG_BABYSTEP_Y, lcd_babystep_y);
      #endif //BABYSTEP_XY
      MENU_ITEM(submenu, MSG_BABYSTEP_Z, lcd_babystep_z);
    #endif

    //
    // Change filament
    //
    #if ENABLED(FILAMENT_CHANGE_FEATURE)
       MENU_ITEM(function, MSG_FILAMENTCHANGE, lcd_enqueue_filament_change);
    #endif

    END_MENU();
  }

  /**
   *
   * "Driver current control" submenu items
   *
   */
  #if ENABLED(DAC_STEPPER_CURRENT)
    void dac_driver_getValues() { LOOP_XYZE(i) driverPercent[i] = dac_current_get_percent((AxisEnum)i); }

    void dac_driver_commit() { dac_current_set_percents(driverPercent); }

    void dac_driver_eeprom_write() { dac_commit_eeprom(); }

    void lcd_dac_menu() {
      dac_driver_getValues();
      START_MENU();
      MENU_BACK(MSG_CONTROL);
      MENU_ITEM_EDIT_CALLBACK(int3, MSG_X " " MSG_DAC_PERCENT, &driverPercent[X_AXIS], 0, 100, dac_driver_commit);
      MENU_ITEM_EDIT_CALLBACK(int3, MSG_Y " " MSG_DAC_PERCENT, &driverPercent[Y_AXIS], 0, 100, dac_driver_commit);
      MENU_ITEM_EDIT_CALLBACK(int3, MSG_Z " " MSG_DAC_PERCENT, &driverPercent[Z_AXIS], 0, 100, dac_driver_commit);
      MENU_ITEM_EDIT_CALLBACK(int3, MSG_E " " MSG_DAC_PERCENT, &driverPercent[E_AXIS], 0, 100, dac_driver_commit);
      MENU_ITEM(function, MSG_DAC_EEPROM_WRITE, dac_driver_eeprom_write);
      END_MENU();
    }
  #endif

  constexpr int heater_maxtemp[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP);

  /**
   *
   * "Prepare" submenu items
   *
   */
  void _lcd_preheat(int endnum, const float temph, const float tempb, const int fan) {
    if (temph > 0) thermalManager.setTargetHotend(min(heater_maxtemp[endnum], temph), endnum);
    #if TEMP_SENSOR_BED != 0
      thermalManager.setTargetBed(tempb);
    #else
      UNUSED(tempb);
    #endif
    #if FAN_COUNT > 0
      #if FAN_COUNT > 1
        fanSpeeds[active_extruder < FAN_COUNT ? active_extruder : 0] = fan;
      #else
        fanSpeeds[0] = fan;
      #endif
    #else
      UNUSED(fan);
    #endif
    lcd_return_to_status();
  }

  #if TEMP_SENSOR_0 != 0
    void lcd_preheat_material1_hotend0() { _lcd_preheat(0, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
    void lcd_preheat_material2_hotend0() { _lcd_preheat(0, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
  #endif

  #if HOTENDS > 1
    void lcd_preheat_material1_hotend1() { _lcd_preheat(1, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
    void lcd_preheat_material2_hotend1() { _lcd_preheat(1, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
    #if HOTENDS > 2
      void lcd_preheat_material1_hotend2() { _lcd_preheat(2, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
      void lcd_preheat_material2_hotend2() { _lcd_preheat(2, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
      #if HOTENDS > 3
        void lcd_preheat_material1_hotend3() { _lcd_preheat(3, lcd_preheat_hotend_temp[0], lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
        void lcd_preheat_material2_hotend3() { _lcd_preheat(3, lcd_preheat_hotend_temp[1], lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
      #endif
    #endif

    void lcd_preheat_material1_hotend0123() {
      #if HOTENDS > 1
        thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 1);
        #if HOTENDS > 2
          thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 2);
          #if HOTENDS > 3
            thermalManager.setTargetHotend(lcd_preheat_hotend_temp[0], 3);
          #endif
        #endif
      #endif
      lcd_preheat_material1_hotend0();
    }
    void lcd_preheat_material2_hotend0123() {
      #if HOTENDS > 1
        thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 1);
        #if HOTENDS > 2
          thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 2);
          #if HOTENDS > 3
            thermalManager.setTargetHotend(lcd_preheat_hotend_temp[1], 3);
          #endif
        #endif
      #endif
      lcd_preheat_material2_hotend0();
    }

  #endif // HOTENDS > 1

  #if TEMP_SENSOR_BED != 0
    void lcd_preheat_material1_bedonly() { _lcd_preheat(0, 0, lcd_preheat_bed_temp[0], lcd_preheat_fan_speed[0]); }
    void lcd_preheat_material2_bedonly() { _lcd_preheat(0, 0, lcd_preheat_bed_temp[1], lcd_preheat_fan_speed[1]); }
  #endif

  #if TEMP_SENSOR_0 != 0 && (TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_BED != 0)

    void lcd_preheat_material1_menu() {
      START_MENU();
      MENU_BACK(MSG_PREPARE);
      #if HOTENDS == 1
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_material1_hotend0);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H1, lcd_preheat_material1_hotend0);
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H2, lcd_preheat_material1_hotend1);
        #if HOTENDS > 2
          MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H3, lcd_preheat_material1_hotend2);
          #if HOTENDS > 3
            MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H4, lcd_preheat_material1_hotend3);
          #endif
        #endif
        MENU_ITEM(function, MSG_PREHEAT_1_ALL, lcd_preheat_material1_hotend0123);
      #endif
      #if TEMP_SENSOR_BED != 0
        MENU_ITEM(function, MSG_PREHEAT_1_BEDONLY, lcd_preheat_material1_bedonly);
      #endif
      END_MENU();
    }

    void lcd_preheat_material2_menu() {
      START_MENU();
      MENU_BACK(MSG_PREPARE);
      #if HOTENDS == 1
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_material2_hotend0);
      #else
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H1, lcd_preheat_material2_hotend0);
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H2, lcd_preheat_material2_hotend1);
        #if HOTENDS > 2
          MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H3, lcd_preheat_material2_hotend2);
          #if HOTENDS > 3
            MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H4, lcd_preheat_material2_hotend3);
          #endif
        #endif
        MENU_ITEM(function, MSG_PREHEAT_2_ALL, lcd_preheat_material2_hotend0123);
      #endif
      #if TEMP_SENSOR_BED != 0
        MENU_ITEM(function, MSG_PREHEAT_2_BEDONLY, lcd_preheat_material2_bedonly);
      #endif
      END_MENU();
    }

  #endif // TEMP_SENSOR_0 && (TEMP_SENSOR_1 || TEMP_SENSOR_2 || TEMP_SENSOR_3 || TEMP_SENSOR_BED)

  void lcd_cooldown() {
    #if FAN_COUNT > 0
      for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
    #endif
    thermalManager.disable_all_heaters();
    lcd_return_to_status();
  }

  #if ENABLED(SDSUPPORT) && ENABLED(MENU_ADDAUTOSTART)

    void lcd_autostart_sd() {
      card.autostart_index = 0;
      card.setroot();
      card.checkautostart(true);
    }

  #endif

  #if ENABLED(MANUAL_BED_LEVELING)

    /**
     *
     * "Prepare" > "Bed Leveling" handlers
     *
     */

    static uint8_t _lcd_level_bed_position;

    // Utility to go to the next mesh point
    // A raise is added between points if Z_HOMING_HEIGHT is in use
    // Note: During Manual Bed Leveling the homed Z position is MESH_HOME_SEARCH_Z
    // Z position will be restored with the final action, a G28
    inline void _mbl_goto_xy(float x, float y) {
      if (no_reentrance) return;
      current_position[Z_AXIS] = LOGICAL_Z_POSITION(MESH_HOME_SEARCH_Z + Z_HOMING_HEIGHT);
      line_to_current(Z_AXIS);
      current_position[X_AXIS] = LOGICAL_X_POSITION(x);
      current_position[Y_AXIS] = LOGICAL_Y_POSITION(y);
      planner.buffer_line_kinematic(current_position, MMM_TO_MMS(XY_PROBE_SPEED), active_extruder);
      #if Z_HOMING_HEIGHT > 0
        current_position[Z_AXIS] = LOGICAL_Z_POSITION(MESH_HOME_SEARCH_Z);
        line_to_current(Z_AXIS);
      #endif
      lcd_synchronize();
    }

    void _lcd_level_goto_next_point();

    void _lcd_level_bed_done() {
      if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_DONE));
      lcdDrawUpdate =
        #if ENABLED(DOGLCD)
          LCDVIEW_CALL_REDRAW_NEXT
        #else
          LCDVIEW_CALL_NO_REDRAW
        #endif
      ;
    }

    /**
     * Step 7: Get the Z coordinate, then goto next point or exit
     */
    void _lcd_level_bed_get_z() {
      ENCODER_DIRECTION_NORMAL();

      if (no_reentrance) goto KeepDrawing;

      // Encoder wheel adjusts the Z position
      if (encoderPosition) {
        refresh_cmd_timeout();
        current_position[Z_AXIS] += float((int32_t)encoderPosition) * (MBL_Z_STEP);
        NOLESS(current_position[Z_AXIS], 0);
        NOMORE(current_position[Z_AXIS], MESH_HOME_SEARCH_Z * 2);
        line_to_current(Z_AXIS);
        lcdDrawUpdate =
          #if ENABLED(DOGLCD)
            LCDVIEW_CALL_REDRAW_NEXT
          #else
            LCDVIEW_REDRAW_NOW
          #endif
        ;
        encoderPosition = 0;
      }

      static bool debounce_click = false;
      if (lcd_clicked) {
        if (!debounce_click) {
          debounce_click = true; // ignore multiple "clicks" in a row
          mbl.set_zigzag_z(_lcd_level_bed_position++, current_position[Z_AXIS]);
          if (_lcd_level_bed_position == (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS)) {
            lcd_goto_screen(_lcd_level_bed_done);

            current_position[Z_AXIS] = MESH_HOME_SEARCH_Z + Z_HOMING_HEIGHT;
            line_to_current(Z_AXIS);
            lcd_synchronize();

            mbl.set_has_mesh(true);
            enqueue_and_echo_commands_P(PSTR("G28"));
            lcd_return_to_status();
            //LCD_MESSAGEPGM(MSG_LEVEL_BED_DONE);
            #if HAS_BUZZER
              lcd_buzz(200, 659);
              lcd_buzz(200, 698);
            #endif
          }
          else {
            lcd_goto_screen(_lcd_level_goto_next_point);
          }
        }
      }
      else {
        debounce_click = false;
      }

KeepDrawing:
      // Update on first display, then only on updates to Z position
      // Show message above on clicks instead
      if (lcdDrawUpdate) {
        float v = current_position[Z_AXIS] - MESH_HOME_SEARCH_Z;
        lcd_implementation_drawedit(PSTR(MSG_MOVE_Z), ftostr43sign(v + (v < 0 ? -0.0001 : 0.0001), '+'));
      }

    }

    /**
     * Step 6: Display "Next point: 1 / 9" while waiting for move to finish
     */
    void _lcd_level_bed_moving() {
      if (lcdDrawUpdate) {
        char msg[10];
        sprintf_P(msg, PSTR("%i / %u"), (int)(_lcd_level_bed_position + 1), (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS));
        lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_NEXT_POINT), msg);
      }

      lcdDrawUpdate =
        #if ENABLED(DOGLCD)
          LCDVIEW_CALL_REDRAW_NEXT
        #else
          LCDVIEW_CALL_NO_REDRAW
        #endif
      ;
    }

    /**
     * Step 5: Initiate a move to the next point
     */
    void _lcd_level_goto_next_point() {
      // Set the menu to display ahead of blocking call
      lcd_goto_screen(_lcd_level_bed_moving);

      // _mbl_goto_xy runs the menu loop until the move is done
      int8_t px, py;
      mbl.zigzag(_lcd_level_bed_position, px, py);
      _mbl_goto_xy(mbl.get_probe_x(px), mbl.get_probe_y(py));

      // After the blocking function returns, change menus
      lcd_goto_screen(_lcd_level_bed_get_z);
    }

    /**
     * Step 4: Display "Click to Begin", wait for click
     *         Move to the first probe position
     */
    void _lcd_level_bed_homing_done() {
      if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_WAITING));
      if (lcd_clicked) {
        _lcd_level_bed_position = 0;
        current_position[Z_AXIS] = MESH_HOME_SEARCH_Z
          #if Z_HOME_DIR > 0
            + Z_MAX_POS
          #endif
        ;
        planner.set_position_mm(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        lcd_goto_screen(_lcd_level_goto_next_point);
      }
    }

    /**
     * Step 3: Display "Homing XYZ" - Wait for homing to finish
     */
    void _lcd_level_bed_homing() {
      if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_HOMING), NULL);
      lcdDrawUpdate =
        #if ENABLED(DOGLCD)
          LCDVIEW_CALL_REDRAW_NEXT
        #else
          LCDVIEW_CALL_NO_REDRAW
        #endif
      ;
      if (no_reentrance) return;
      lcd_wait_for_homing();
      lcd_goto_screen(_lcd_level_bed_homing_done);
    }

    /**
     * Step 2: Continue Bed Leveling...
     */
    void _lcd_level_bed_continue() {
      defer_return_to_status = true;
      axis_homed[X_AXIS] = axis_homed[Y_AXIS] = axis_homed[Z_AXIS] = false;
      mbl.reset();
      enqueue_and_echo_commands_P(PSTR("G28"));
      lcd_goto_screen(_lcd_level_bed_homing);
    }

    /**
     * Step 1: MBL entry-point: "Cancel" or "Level Bed"
     */
    void lcd_level_bed() {
      START_MENU();
      MENU_BACK(MSG_LEVEL_BED_CANCEL);
      MENU_ITEM(submenu, MSG_LEVEL_BED, _lcd_level_bed_continue);
      END_MENU();
    }

  #endif  // MANUAL_BED_LEVELING

  /**
   *
   * "Prepare" submenu
   *
   */

  void lcd_prepare_menu() {
    START_MENU();

    //
    // ^ Main
    //
    MENU_BACK(MSG_MAIN);

    //
    // Move Axis
    //
    #if ENABLED(DELTA)
      if (axis_homed[Z_AXIS])
    #endif
        MENU_ITEM(submenu, MSG_MOVE_AXIS, lcd_move_menu);

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
    // Level Bed
    //
    #if HAS_ABL
      MENU_ITEM(gcode, MSG_LEVEL_BED,
        axis_homed[X_AXIS] && axis_homed[Y_AXIS] ? PSTR("G29") : PSTR("G28\nG29")
      );
    #elif ENABLED(MANUAL_BED_LEVELING)
      MENU_ITEM(submenu, MSG_LEVEL_BED, lcd_level_bed);
    #endif

    //
    // Set Home Offsets
    //
    MENU_ITEM(function, MSG_SET_HOME_OFFSETS, lcd_set_home_offsets);
    //MENU_ITEM(gcode, MSG_SET_ORIGIN, PSTR("G92 X0 Y0 Z0"));

    //
    // Disable Steppers
    //
    MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));

    //
    // Preheat PLA
    // Preheat ABS
    //
    #if TEMP_SENSOR_0 != 0
      #if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_BED != 0
        MENU_ITEM(submenu, MSG_PREHEAT_1, lcd_preheat_material1_menu);
        MENU_ITEM(submenu, MSG_PREHEAT_2, lcd_preheat_material2_menu);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_material1_hotend0);
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_material2_hotend0);
      #endif
      //
      // Change filament
      //
      #if ENABLED(FILAMENT_CHANGE_FEATURE)
        MENU_ITEM(function, MSG_FILAMENTCHANGE, lcd_enqueue_filament_change);
      #endif
    #endif

    //
    // Cooldown
    //
    bool has_heat = false;
    HOTEND_LOOP() if (thermalManager.target_temperature[e]) { has_heat = true; break; }
    #if HAS_TEMP_BED
      if (thermalManager.target_temperature_bed) has_heat = true;
    #endif
    if (has_heat) MENU_ITEM(function, MSG_COOLDOWN, lcd_cooldown);

    //
    // BLTouch Self-Test and Reset
    //
    #if ENABLED(BLTOUCH)
      MENU_ITEM(gcode, MSG_BLTOUCH_SELFTEST, PSTR("M280 P" STRINGIFY(Z_ENDSTOP_SERVO_NR) " S" STRINGIFY(BLTOUCH_SELFTEST)));
      if (!endstops.z_probe_enabled && TEST_BLTOUCH())
        MENU_ITEM(gcode, MSG_BLTOUCH_RESET, PSTR("M280 P" STRINGIFY(Z_ENDSTOP_SERVO_NR) " S" STRINGIFY(BLTOUCH_RESET)));
    #endif

    //
    // Switch power on/off
    //
    #if HAS_POWER_SWITCH
      if (powersupply)
        MENU_ITEM(gcode, MSG_SWITCH_PS_OFF, PSTR("M81"));
      else
        MENU_ITEM(gcode, MSG_SWITCH_PS_ON, PSTR("M80"));
    #endif

    //
    // Autostart
    //
    #if ENABLED(SDSUPPORT) && ENABLED(MENU_ADDAUTOSTART)
      MENU_ITEM(function, MSG_AUTOSTART, lcd_autostart_sd);
    #endif

    END_MENU();
  }

  float move_menu_scale;

  #if ENABLED(DELTA_CALIBRATION_MENU)

    void lcd_move_z();
    void lcd_delta_calibrate_menu();

    void _lcd_calibrate_homing() {
      if (lcdDrawUpdate) lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, PSTR(MSG_LEVEL_BED_HOMING));
      lcdDrawUpdate =
        #if ENABLED(DOGLCD)
          LCDVIEW_CALL_REDRAW_NEXT
        #else
          LCDVIEW_CALL_NO_REDRAW
        #endif
      ;
      if (axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS])
        lcd_goto_previous_menu();
    }

    void _lcd_delta_calibrate_home() {
      enqueue_and_echo_commands_P(PSTR("G28"));
      lcd_goto_screen(_lcd_calibrate_homing);
    }

    // Move directly to the tower position with uninterpolated moves
    // If we used interpolated moves it would cause this to become re-entrant
    void _goto_tower_pos(const float &a) {
      if (no_reentrance) return;

      current_position[Z_AXIS] = max(Z_HOMING_HEIGHT, Z_CLEARANCE_BETWEEN_PROBES) + (DELTA_PRINTABLE_RADIUS) / 5;
      line_to_current(Z_AXIS);

      current_position[X_AXIS] = a < 0 ? X_HOME_POS : sin(a) * -(DELTA_PRINTABLE_RADIUS);
      current_position[Y_AXIS] = a < 0 ? Y_HOME_POS : cos(a) *  (DELTA_PRINTABLE_RADIUS);
      line_to_current(Z_AXIS);

      current_position[Z_AXIS] = 4.0;
      line_to_current(Z_AXIS);

      lcd_synchronize();

      move_menu_scale = 0.1;
      lcd_goto_screen(lcd_move_z);
    }

    void _goto_tower_x() { _goto_tower_pos(RADIANS(120)); }
    void _goto_tower_y() { _goto_tower_pos(RADIANS(240)); }
    void _goto_tower_z() { _goto_tower_pos(0); }
    void _goto_center()  { _goto_tower_pos(-1); }

    void lcd_delta_calibrate_menu() {
      START_MENU();
      MENU_BACK(MSG_MAIN);
      MENU_ITEM(submenu, MSG_AUTO_HOME, _lcd_delta_calibrate_home);
      if (axis_homed[Z_AXIS]) {
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_X, _goto_tower_x);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Y, _goto_tower_y);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Z, _goto_tower_z);
        MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_CENTER, _goto_center);
      }
      END_MENU();
    }

  #endif // DELTA_CALIBRATION_MENU

  /**
   * If the most recent manual move hasn't been fed to the planner yet,
   * and the planner can accept one, send immediately
   */
  inline void manage_manual_move() {
    if (manual_move_axis != (int8_t)NO_AXIS && ELAPSED(millis(), manual_move_start_time) && !planner.is_full()) {
      planner.buffer_line_kinematic(current_position, MMM_TO_MMS(manual_feedrate_mm_m[manual_move_axis]), manual_move_e_index);
      manual_move_axis = (int8_t)NO_AXIS;
    }
  }

  /**
   * Set a flag that lcd_update() should start a move
   * to "current_position" after a short delay.
   */
  inline void manual_move_to_current(AxisEnum axis
    #if E_MANUAL > 1
      , int8_t eindex=-1
    #endif
  ) {
    #if E_MANUAL > 1
      if (axis == E_AXIS) manual_move_e_index = eindex >= 0 ? eindex : active_extruder;
    #endif
    manual_move_start_time = millis() + (move_menu_scale < 0.99 ? 0UL : 250UL); // delay for bigger moves
    manual_move_axis = (int8_t)axis;
  }

  /**
   *
   * "Prepare" > "Move Axis" submenu
   *
   */

  void _lcd_move_xyz(const char* name, AxisEnum axis) {
    if (lcd_clicked) { return lcd_goto_previous_menu(); }
    ENCODER_DIRECTION_NORMAL();
    if (encoderPosition) {
      refresh_cmd_timeout();

      // Limit to software endstops, if enabled
      float min = (soft_endstops_enabled && min_software_endstops) ? soft_endstop_min[axis] : current_position[axis] - 1000,
            max = (soft_endstops_enabled && max_software_endstops) ? soft_endstop_max[axis] : current_position[axis] + 1000;

      // Get the new position
      current_position[axis] += float((int32_t)encoderPosition) * move_menu_scale;

      // Delta limits XY based on the current offset from center
      // This assumes the center is 0,0
      #if ENABLED(DELTA)
        if (axis != Z_AXIS) {
          max = sqrt(sq(DELTA_PRINTABLE_RADIUS) - sq(current_position[Y_AXIS - axis]));
          min = -max;
        }
      #endif

      // Limit only when trying to move towards the limit
      if ((int32_t)encoderPosition < 0) NOLESS(current_position[axis], min);
      if ((int32_t)encoderPosition > 0) NOMORE(current_position[axis], max);

      manual_move_to_current(axis);

      encoderPosition = 0;
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
    }
    if (lcdDrawUpdate) lcd_implementation_drawedit(name, ftostr41sign(current_position[axis]));
  }
  void lcd_move_x() { _lcd_move_xyz(PSTR(MSG_MOVE_X), X_AXIS); }
  void lcd_move_y() { _lcd_move_xyz(PSTR(MSG_MOVE_Y), Y_AXIS); }
  void lcd_move_z() { _lcd_move_xyz(PSTR(MSG_MOVE_Z), Z_AXIS); }
  void _lcd_move_e(
    #if E_MANUAL > 1
      int8_t eindex=-1
    #endif
  ) {
    if (lcd_clicked) { return lcd_goto_previous_menu(); }
    ENCODER_DIRECTION_NORMAL();
    if (encoderPosition) {
      current_position[E_AXIS] += float((int32_t)encoderPosition) * move_menu_scale;
      encoderPosition = 0;
      manual_move_to_current(E_AXIS
        #if E_MANUAL > 1
          , eindex
        #endif
      );
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
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
            #endif
          #endif
        }
      #endif
      lcd_implementation_drawedit(pos_label, ftostr41sign(current_position[E_AXIS]));
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
      #endif
    #endif
  #endif

  /**
   *
   * "Prepare" > "Move Xmm" > "Move XYZ" submenu
   *
   */

  screenFunc_t _manual_move_func_ptr;

  void lcd_move_menu_10mm() { move_menu_scale = 10.0; lcd_goto_screen(_manual_move_func_ptr); }
  void lcd_move_menu_1mm()  { move_menu_scale =  1.0; lcd_goto_screen(_manual_move_func_ptr); }
  void lcd_move_menu_01mm() { move_menu_scale =  0.1; lcd_goto_screen(_manual_move_func_ptr); }

  void _lcd_move_distance_menu(AxisEnum axis, screenFunc_t func) {
    _manual_move_func_ptr = func;
    START_MENU();
    if (LCD_HEIGHT >= 4) {
      switch(axis) {
        case X_AXIS:
          STATIC_ITEM(MSG_MOVE_X, true, true); break;
        case Y_AXIS:
          STATIC_ITEM(MSG_MOVE_Y, true, true); break;
        case Z_AXIS:
          STATIC_ITEM(MSG_MOVE_Z, true, true); break;
        default:
          STATIC_ITEM(MSG_MOVE_E, true, true); break;
      }
    }
    MENU_BACK(MSG_MOVE_AXIS);
    if (axis == X_AXIS || axis == Y_AXIS)
      MENU_ITEM(submenu, MSG_MOVE_10MM, lcd_move_menu_10mm);
    MENU_ITEM(submenu, MSG_MOVE_1MM, lcd_move_menu_1mm);
    MENU_ITEM(submenu, MSG_MOVE_01MM, lcd_move_menu_01mm);
    END_MENU();
  }
  void lcd_move_get_x_amount()        { _lcd_move_distance_menu(X_AXIS, lcd_move_x); }
  void lcd_move_get_y_amount()        { _lcd_move_distance_menu(Y_AXIS, lcd_move_y); }
  void lcd_move_get_z_amount()        { _lcd_move_distance_menu(Z_AXIS, lcd_move_z); }
  void lcd_move_get_e_amount()        { _lcd_move_distance_menu(E_AXIS, lcd_move_e); }
  #if E_MANUAL > 1
    void lcd_move_get_e0_amount()     { _lcd_move_distance_menu(E_AXIS, lcd_move_e0); }
    void lcd_move_get_e1_amount()     { _lcd_move_distance_menu(E_AXIS, lcd_move_e1); }
    #if E_MANUAL > 2
      void lcd_move_get_e2_amount()   { _lcd_move_distance_menu(E_AXIS, lcd_move_e2); }
      #if E_MANUAL > 3
        void lcd_move_get_e3_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e3); }
      #endif
    #endif
  #endif

  /**
   *
   * "Prepare" > "Move Axis" submenu
   *
   */

  #if IS_KINEMATIC
    #define _MOVE_XYZ_ALLOWED (axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS])
    #if ENABLED(DELTA)
      #define _MOVE_XY_ALLOWED (current_position[Z_AXIS] <= delta_clip_start_height)
      void lcd_lower_z_to_clip_height() {
        if (!no_reentrance) {
          current_position[Z_AXIS] = delta_clip_start_height;
          line_to_current(Z_AXIS);
          lcd_synchronize();
        }
      }
    #else
      #define _MOVE_XY_ALLOWED true
    #endif
  #else
    #define _MOVE_XYZ_ALLOWED true
    #define _MOVE_XY_ALLOWED true
  #endif

  void lcd_move_menu() {
    START_MENU();
    MENU_BACK(MSG_PREPARE);

    if (_MOVE_XYZ_ALLOWED) {
      if (_MOVE_XY_ALLOWED) {
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

    #if ENABLED(SWITCHING_EXTRUDER)
      if (active_extruder)
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
      else
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));
    #endif

    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_get_e_amount);
    #if E_MANUAL > 1
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E1, lcd_move_get_e0_amount);
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E2, lcd_move_get_e1_amount);
      #if E_MANUAL > 2
        MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E3, lcd_move_get_e2_amount);
        #if E_MANUAL > 3
          MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E4, lcd_move_get_e3_amount);
        #endif
      #endif
    #endif

    END_MENU();
  }

  /**
   *
   * "Control" submenu
   *
   */

  void lcd_control_menu() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    MENU_ITEM(submenu, MSG_TEMPERATURE, lcd_control_temperature_menu);
    MENU_ITEM(submenu, MSG_MOTION, lcd_control_motion_menu);
    MENU_ITEM(submenu, MSG_VOLUMETRIC, lcd_control_volumetric_menu);

    #if HAS_LCD_CONTRAST
      //MENU_ITEM_EDIT(int3, MSG_CONTRAST, &lcd_contrast, 0, 63);
      MENU_ITEM(submenu, MSG_CONTRAST, lcd_set_contrast);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(submenu, MSG_RETRACT, lcd_control_retract_menu);
    #endif
    #if ENABLED(DAC_STEPPER_CURRENT)
      MENU_ITEM(submenu, MSG_DRIVE_STRENGTH, lcd_dac_menu);
    #endif

    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
      MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
    #endif
    MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
    END_MENU();
  }

  /**
   *
   * "Temperature" submenu
   *
   */

  #if ENABLED(PID_AUTOTUNE_MENU)

    #if ENABLED(PIDTEMP)
      int autotune_temp[HOTENDS] = ARRAY_BY_HOTENDS1(150);
    #endif

    #if ENABLED(PIDTEMPBED)
      int autotune_temp_bed = 70;
    #endif

    void _lcd_autotune(int e) {
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
      enqueue_and_echo_command(cmd);
    }

  #endif //PID_AUTOTUNE_MENU

  #if ENABLED(PIDTEMP)

    // Helpers for editing PID Ki & Kd values
    // grab the PID value out of the temp variable; scale it; then update the PID driver
    void copy_and_scalePID_i(int e) {
      #if DISABLED(PID_PARAMS_PER_HOTEND) || HOTENDS == 1
        UNUSED(e);
      #endif
      PID_PARAM(Ki, e) = scalePID_i(raw_Ki);
      thermalManager.updatePID();
    }
    void copy_and_scalePID_d(int e) {
      #if DISABLED(PID_PARAMS_PER_HOTEND) || HOTENDS == 1
        UNUSED(e);
      #endif
      PID_PARAM(Kd, e) = scalePID_d(raw_Kd);
      thermalManager.updatePID();
    }
    #define _PIDTEMP_BASE_FUNCTIONS(eindex) \
      void copy_and_scalePID_i_E ## eindex() { copy_and_scalePID_i(eindex); } \
      void copy_and_scalePID_d_E ## eindex() { copy_and_scalePID_d(eindex); }

    #if ENABLED(PID_AUTOTUNE_MENU)
      #define _PIDTEMP_FUNCTIONS(eindex) \
        _PIDTEMP_BASE_FUNCTIONS(eindex); \
        void lcd_autotune_callback_E ## eindex() { _lcd_autotune(eindex); }
    #else
      #define _PIDTEMP_FUNCTIONS(eindex) _PIDTEMP_BASE_FUNCTIONS(eindex)
    #endif

    _PIDTEMP_FUNCTIONS(0)
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      #if HOTENDS > 1
        _PIDTEMP_FUNCTIONS(1)
        #if HOTENDS > 2
          _PIDTEMP_FUNCTIONS(2)
          #if HOTENDS > 3
            _PIDTEMP_FUNCTIONS(3)
          #endif //HOTENDS > 3
        #endif //HOTENDS > 2
      #endif //HOTENDS > 1
    #endif //PID_PARAMS_PER_HOTEND

  #endif //PIDTEMP

  /**
   *
   * "Control" > "Temperature" submenu
   *
   */
  void lcd_control_temperature_menu() {
    START_MENU();

    //
    // ^ Control
    //
    MENU_BACK(MSG_CONTROL);

    //
    // Nozzle:
    // Nozzle [1-4]:
    //
    #if HOTENDS == 1
      #if TEMP_SENSOR_0 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
      #endif
    #else //HOTENDS > 1
      #if TEMP_SENSOR_0 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N1, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
      #endif
      #if TEMP_SENSOR_1 != 0
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N2, &thermalManager.target_temperature[1], 0, HEATER_1_MAXTEMP - 15, watch_temp_callback_E1);
      #endif
      #if HOTENDS > 2
        #if TEMP_SENSOR_2 != 0
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N3, &thermalManager.target_temperature[2], 0, HEATER_2_MAXTEMP - 15, watch_temp_callback_E2);
        #endif
        #if HOTENDS > 3
          #if TEMP_SENSOR_3 != 0
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_N4, &thermalManager.target_temperature[3], 0, HEATER_3_MAXTEMP - 15, watch_temp_callback_E3);
          #endif
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1

    //
    // Bed:
    //
    #if TEMP_SENSOR_BED != 0
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.target_temperature_bed, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
    #endif

    //
    // Fan Speed:
    //
    #if FAN_COUNT > 0
      #if HAS_FAN0
        #if FAN_COUNT > 1
          #define MSG_1ST_FAN_SPEED MSG_FAN_SPEED " 1"
        #else
          #define MSG_1ST_FAN_SPEED MSG_FAN_SPEED
        #endif
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_1ST_FAN_SPEED, &fanSpeeds[0], 0, 255);
      #endif
      #if HAS_FAN1
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED " 2", &fanSpeeds[1], 0, 255);
      #endif
      #if HAS_FAN2
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED " 3", &fanSpeeds[2], 0, 255);
      #endif
    #endif // FAN_COUNT > 0

    //
    // Autotemp, Min, Max, Fact
    //
    #if ENABLED(AUTOTEMP) && (TEMP_SENSOR_0 != 0)
      MENU_ITEM_EDIT(bool, MSG_AUTOTEMP, &planner.autotemp_enabled);
      MENU_ITEM_EDIT(float3, MSG_MIN, &planner.autotemp_min, 0, HEATER_0_MAXTEMP - 15);
      MENU_ITEM_EDIT(float3, MSG_MAX, &planner.autotemp_max, 0, HEATER_0_MAXTEMP - 15);
      MENU_ITEM_EDIT(float32, MSG_FACTOR, &planner.autotemp_factor, 0.0, 1.0);
    #endif

    //
    // PID-P, PID-I, PID-D, PID-C, PID Autotune
    // PID-P E1, PID-I E1, PID-D E1, PID-C E1, PID Autotune E1
    // PID-P E2, PID-I E2, PID-D E2, PID-C E2, PID Autotune E2
    // PID-P E3, PID-I E3, PID-D E3, PID-C E3, PID Autotune E3
    // PID-P E4, PID-I E4, PID-D E4, PID-C E4, PID Autotune E4
    //
    #if ENABLED(PIDTEMP)

      #define _PID_BASE_MENU_ITEMS(ELABEL, eindex) \
        raw_Ki = unscalePID_i(PID_PARAM(Ki, eindex)); \
        raw_Kd = unscalePID_d(PID_PARAM(Kd, eindex)); \
        MENU_ITEM_EDIT(float52, MSG_PID_P ELABEL, &PID_PARAM(Kp, eindex), 1, 9990); \
        MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I ELABEL, &raw_Ki, 0.01, 9990, copy_and_scalePID_i_E ## eindex); \
        MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D ELABEL, &raw_Kd, 1, 9990, copy_and_scalePID_d_E ## eindex)

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
          #endif //HOTENDS > 3
        #endif //HOTENDS > 2
      #else //!PID_PARAMS_PER_HOTEND || HOTENDS == 1
        PID_MENU_ITEMS("", 0);
      #endif //!PID_PARAMS_PER_HOTEND || HOTENDS == 1

    #endif //PIDTEMP

    //
    // Preheat Material 1 conf
    //
    MENU_ITEM(submenu, MSG_PREHEAT_1_SETTINGS, lcd_control_temperature_preheat_material1_settings_menu);

    //
    // Preheat Material 2 conf
    //
    MENU_ITEM(submenu, MSG_PREHEAT_2_SETTINGS, lcd_control_temperature_preheat_material2_settings_menu);
    END_MENU();
  }

  void _lcd_control_temperature_preheat_settings_menu(uint8_t material) {
    #if HOTENDS > 3
      #define MINTEMP_ALL MIN4(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP)
      #define MAXTEMP_ALL MAX4(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP)
    #elif HOTENDS > 2
      #define MINTEMP_ALL MIN3(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP)
      #define MAXTEMP_ALL MAX3(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP)
    #elif HOTENDS > 1
      #define MINTEMP_ALL min(HEATER_0_MINTEMP, HEATER_1_MINTEMP)
      #define MAXTEMP_ALL max(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP)
    #else
      #define MINTEMP_ALL HEATER_0_MINTEMP
      #define MAXTEMP_ALL HEATER_0_MAXTEMP
    #endif
    START_MENU();
    MENU_BACK(MSG_TEMPERATURE);
    MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &lcd_preheat_fan_speed[material], 0, 255);
    #if TEMP_SENSOR_0 != 0
      MENU_ITEM_EDIT(int3, MSG_NOZZLE, &lcd_preheat_hotend_temp[material], MINTEMP_ALL, MAXTEMP_ALL - 15);
    #endif
    #if TEMP_SENSOR_BED != 0
      MENU_ITEM_EDIT(int3, MSG_BED, &lcd_preheat_bed_temp[material], BED_MINTEMP, BED_MAXTEMP - 15);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
    #endif
    END_MENU();
  }

  /**
   *
   * "Temperature" > "Preheat Material 1 conf" submenu
   *
   */
  void lcd_control_temperature_preheat_material1_settings_menu() { _lcd_control_temperature_preheat_settings_menu(0); }

  /**
   *
   * "Temperature" > "Preheat Material 2 conf" submenu
   *
   */
  void lcd_control_temperature_preheat_material2_settings_menu() { _lcd_control_temperature_preheat_settings_menu(1); }

  void _reset_acceleration_rates() { planner.reset_acceleration_rates(); }
  #if ENABLED(DISTINCT_E_FACTORS)
    void _reset_e_acceleration_rate(const uint8_t e) { if (e == active_extruder) _reset_acceleration_rates(); }
    void _reset_e0_acceleration_rate() { _reset_e_acceleration_rate(0); }
    void _reset_e1_acceleration_rate() { _reset_e_acceleration_rate(1); }
    #if E_STEPPERS > 2
      void _reset_e2_acceleration_rate() { _reset_e_acceleration_rate(2); }
      #if E_STEPPERS > 3
        void _reset_e3_acceleration_rate() { _reset_e_acceleration_rate(3); }
      #endif
    #endif
  #endif

  void _planner_refresh_positioning() { planner.refresh_positioning(); }
  #if ENABLED(DISTINCT_E_FACTORS)
    void _planner_refresh_e_positioning(const uint8_t e) {
      if (e == active_extruder)
        _planner_refresh_positioning();
      else
        planner.steps_to_mm[e] = 1.0 / planner.axis_steps_per_mm[e];
    }
    void _planner_refresh_e0_positioning() { _reset_e_acceleration_rate(0); }
    void _planner_refresh_e1_positioning() { _reset_e_acceleration_rate(1); }
    #if E_STEPPERS > 2
      void _planner_refresh_e2_positioning() { _reset_e_acceleration_rate(2); }
      #if E_STEPPERS > 3
        void _planner_refresh_e3_positioning() { _reset_e_acceleration_rate(3); }
      #endif
    #endif
  #endif

  /**
   *
   * "Control" > "Motion" submenu
   *
   */
  void lcd_control_motion_menu() {
    START_MENU();
    MENU_BACK(MSG_CONTROL);
    #if HAS_BED_PROBE
      MENU_ITEM_EDIT(float32, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
    #endif
    // Manual bed leveling, Bed Z:
    #if ENABLED(MANUAL_BED_LEVELING)
      MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
    #endif
    MENU_ITEM_EDIT(float5, MSG_ACC, &planner.acceleration, 10, 99000);
    MENU_ITEM_EDIT(float3, MSG_VX_JERK, &planner.max_jerk[X_AXIS], 1, 990);
    MENU_ITEM_EDIT(float3, MSG_VY_JERK, &planner.max_jerk[Y_AXIS], 1, 990);
    #if ENABLED(DELTA)
      MENU_ITEM_EDIT(float3, MSG_VZ_JERK, &planner.max_jerk[Z_AXIS], 1, 990);
    #else
      MENU_ITEM_EDIT(float52, MSG_VZ_JERK, &planner.max_jerk[Z_AXIS], 0.1, 990);
    #endif
    MENU_ITEM_EDIT(float3, MSG_VE_JERK, &planner.max_jerk[E_AXIS], 1, 990);

    //
    // M203 Settings
    //
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_X, &planner.max_feedrate_mm_s[X_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Y, &planner.max_feedrate_mm_s[Y_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Z, &planner.max_feedrate_mm_s[Z_AXIS], 1, 999);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E, &planner.max_feedrate_mm_s[E_AXIS + active_extruder], 1, 999);
      MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E1, &planner.max_feedrate_mm_s[E_AXIS], 1, 999);
      MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E2, &planner.max_feedrate_mm_s[E_AXIS + 1], 1, 999);
      #if E_STEPPERS > 2
        MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E3, &planner.max_feedrate_mm_s[E_AXIS + 2], 1, 999);
        #if E_STEPPERS > 3
          MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E3, &planner.max_feedrate_mm_s[E_AXIS + 3], 1, 999);
        #endif
      #endif
    #else
      MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E, &planner.max_feedrate_mm_s[E_AXIS], 1, 999);
    #endif

    MENU_ITEM_EDIT(float3, MSG_VMIN, &planner.min_feedrate_mm_s, 0, 999);
    MENU_ITEM_EDIT(float3, MSG_VTRAV_MIN, &planner.min_travel_feedrate_mm_s, 0, 999);

    //
    // M201 Settings
    //
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_X, &planner.max_acceleration_mm_per_s2[X_AXIS], 100, 99000, _reset_acceleration_rates);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Y, &planner.max_acceleration_mm_per_s2[Y_AXIS], 100, 99000, _reset_acceleration_rates);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Z, &planner.max_acceleration_mm_per_s2[Z_AXIS], 10, 99000, _reset_acceleration_rates);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &planner.max_acceleration_mm_per_s2[E_AXIS + active_extruder], 100, 99000, _reset_acceleration_rates);
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E1, &planner.max_acceleration_mm_per_s2[E_AXIS], 100, 99000, _reset_e0_acceleration_rate);
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E2, &planner.max_acceleration_mm_per_s2[E_AXIS + 1], 100, 99000, _reset_e1_acceleration_rate);
      #if E_STEPPERS > 2
        MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E3, &planner.max_acceleration_mm_per_s2[E_AXIS + 2], 100, 99000, _reset_e2_acceleration_rate);
        #if E_STEPPERS > 3
          MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E4, &planner.max_acceleration_mm_per_s2[E_AXIS + 3], 100, 99000, _reset_e3_acceleration_rate);
        #endif
      #endif
    #else
      MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &planner.max_acceleration_mm_per_s2[E_AXIS], 100, 99000, _reset_acceleration_rates);
    #endif

    MENU_ITEM_EDIT(float5, MSG_A_RETRACT, &planner.retract_acceleration, 100, 99000);
    MENU_ITEM_EDIT(float5, MSG_A_TRAVEL, &planner.travel_acceleration, 100, 99000);

    //
    // M92 Settings
    //
    MENU_ITEM_EDIT_CALLBACK(float62, MSG_XSTEPS, &planner.axis_steps_per_mm[X_AXIS], 5, 9999, _planner_refresh_positioning);
    MENU_ITEM_EDIT_CALLBACK(float62, MSG_YSTEPS, &planner.axis_steps_per_mm[Y_AXIS], 5, 9999, _planner_refresh_positioning);
    MENU_ITEM_EDIT_CALLBACK(float62, MSG_ZSTEPS, &planner.axis_steps_per_mm[Z_AXIS], 5, 9999, _planner_refresh_positioning);

    #if ENABLED(DISTINCT_E_FACTORS)
      MENU_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.axis_steps_per_mm[E_AXIS + active_extruder], 5, 9999, _planner_refresh_positioning);
      MENU_ITEM_EDIT_CALLBACK(float62, MSG_E1STEPS, &planner.axis_steps_per_mm[E_AXIS], 5, 9999, _planner_refresh_e0_positioning);
      MENU_ITEM_EDIT_CALLBACK(float62, MSG_E2STEPS, &planner.axis_steps_per_mm[E_AXIS + 1], 5, 9999, _planner_refresh_e1_positioning);
      #if E_STEPPERS > 2
        MENU_ITEM_EDIT_CALLBACK(float62, MSG_E3STEPS, &planner.axis_steps_per_mm[E_AXIS + 2], 5, 9999, _planner_refresh_e2_positioning);
        #if E_STEPPERS > 3
          MENU_ITEM_EDIT_CALLBACK(float62, MSG_E4STEPS, &planner.axis_steps_per_mm[E_AXIS + 3], 5, 9999, _planner_refresh_e3_positioning);
        #endif
      #endif
    #else
      MENU_ITEM_EDIT_CALLBACK(float62, MSG_ESTEPS, &planner.axis_steps_per_mm[E_AXIS], 5, 9999, _planner_refresh_positioning);
    #endif

    #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
      MENU_ITEM_EDIT(bool, MSG_ENDSTOP_ABORT, &stepper.abort_on_endstop_hit);
    #endif
    END_MENU();
  }

  /**
   *
   * "Control" > "Filament" submenu
   *
   */
  void lcd_control_volumetric_menu() {
    START_MENU();
    MENU_BACK(MSG_CONTROL);

    MENU_ITEM_EDIT_CALLBACK(bool, MSG_VOLUMETRIC_ENABLED, &volumetric_enabled, calculate_volumetric_multipliers);

    if (volumetric_enabled) {
      #if EXTRUDERS == 1
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM, &filament_size[0], 1.5, 3.25, calculate_volumetric_multipliers);
      #else //EXTRUDERS > 1
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E1, &filament_size[0], 1.5, 3.25, calculate_volumetric_multipliers);
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E2, &filament_size[1], 1.5, 3.25, calculate_volumetric_multipliers);
        #if EXTRUDERS > 2
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E3, &filament_size[2], 1.5, 3.25, calculate_volumetric_multipliers);
          #if EXTRUDERS > 3
            MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_DIAM MSG_DIAM_E4, &filament_size[3], 1.5, 3.25, calculate_volumetric_multipliers);
          #endif //EXTRUDERS > 3
        #endif //EXTRUDERS > 2
      #endif //EXTRUDERS > 1
    }

    END_MENU();
  }

  /**
   *
   * "Control" > "Contrast" submenu
   *
   */
  #if HAS_LCD_CONTRAST
    void lcd_set_contrast() {
      if (lcd_clicked) { return lcd_goto_previous_menu(); }
      ENCODER_DIRECTION_NORMAL();
      if (encoderPosition) {
        set_lcd_contrast(lcd_contrast + encoderPosition);
        encoderPosition = 0;
        lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
      }
      if (lcdDrawUpdate) {
        lcd_implementation_drawedit(PSTR(MSG_CONTRAST),
          #if LCD_CONTRAST_MAX >= 100
            itostr3(lcd_contrast)
          #else
            itostr2(lcd_contrast)
          #endif
        );
      }
    }
  #endif // HAS_LCD_CONTRAST

  /**
   *
   * "Control" > "Retract" submenu
   *
   */
  #if ENABLED(FWRETRACT)

    void lcd_control_retract_menu() {
      START_MENU();
      MENU_BACK(MSG_CONTROL);
      MENU_ITEM_EDIT(bool, MSG_AUTORETRACT, &autoretract_enabled);
      MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT, &retract_length, 0, 100);
      #if EXTRUDERS > 1
        MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_SWAP, &retract_length_swap, 0, 100);
      #endif
      MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACTF, &retract_feedrate_mm_s, 1, 999);
      MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_ZLIFT, &retract_zlift, 0, 999);
      MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_RECOVER, &retract_recover_length, -100, 100);
      #if EXTRUDERS > 1
        MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_RECOVER_SWAP, &retract_recover_length_swap, -100, 100);
      #endif
      MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVERF, &retract_recover_feedrate_mm_s, 1, 999);
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
      card.updir();
      encoderTopLine = 0;
      screen_changed = true;
      lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;
    }

    /**
     *
     * "Print from SD" submenu
     *
     */
    void lcd_sdcard_menu() {
      ENCODER_DIRECTION_MENUS();
      if (!lcdDrawUpdate && !lcd_clicked) return; // nothing to do (so don't thrash the SD card)
      uint16_t fileCnt = card.getnrfilenames();
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
          card.getfilename(
             #if ENABLED(SDCARD_RATHERRECENTFIRST)
               fileCnt-1 -
             #endif
             i
          );

          if (card.filenameIsDir)
            MENU_ITEM(sddirectory, MSG_CARD_MENU, card.filename, card.longFilename);
          else
            MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename);
        }
        else {
          MENU_ITEM_DUMMY();
        }
      }
      END_MENU();
    }

  #endif //SDSUPPORT

  #if ENABLED(LCD_INFO_MENU)

    #if ENABLED(PRINTCOUNTER)
      /**
       *
       * About Printer > Statistics submenu
       *
       */
      void lcd_info_stats_menu() {
        if (lcd_clicked) { return lcd_goto_previous_menu(); }

        char buffer[21];
        printStatistics stats = print_job_timer.getStats();

        START_SCREEN();                                                                                // 12345678901234567890
        STATIC_ITEM(MSG_INFO_PRINT_COUNT ": ", false, false, itostr3left(stats.totalPrints));          // Print Count: 999
        STATIC_ITEM(MSG_INFO_COMPLETED_PRINTS": ", false, false, itostr3left(stats.finishedPrints)); // Completed  : 666

        duration_t elapsed = stats.printTime;
        elapsed.toString(buffer);

        STATIC_ITEM(MSG_INFO_PRINT_TIME ": ", false, false);                                           // Total print Time:
        STATIC_ITEM("", false, false, buffer);                                                         // 99y 364d 23h 59m 59s

        elapsed = stats.longestPrint;
        elapsed.toString(buffer);

        STATIC_ITEM(MSG_INFO_PRINT_LONGEST ": ", false, false);                                        // Longest job time:
        STATIC_ITEM("", false, false, buffer);                                                         // 99y 364d 23h 59m 59s

        sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int(stats.filamentUsed / 100) % 10);
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
    void lcd_info_thermistors_menu() {
      if (lcd_clicked) { return lcd_goto_previous_menu(); }
      START_SCREEN();
      #define THERMISTOR_ID TEMP_SENSOR_0
      #include "thermistornames.h"
      STATIC_ITEM("T0: " THERMISTOR_NAME, false, true);
      STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_0_MINTEMP), false);
      STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_0_MAXTEMP), false);

      #if TEMP_SENSOR_1 != 0
        #undef THERMISTOR_ID
        #define THERMISTOR_ID TEMP_SENSOR_1
        #include "thermistornames.h"
        STATIC_ITEM("T1: " THERMISTOR_NAME, false, true);
        STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_1_MINTEMP), false);
        STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_1_MAXTEMP), false);
      #endif

      #if TEMP_SENSOR_2 != 0
        #undef THERMISTOR_ID
        #define THERMISTOR_ID TEMP_SENSOR_2
        #include "thermistornames.h"
        STATIC_ITEM("T2: " THERMISTOR_NAME, false, true);
        STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_2_MINTEMP), false);
        STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_2_MAXTEMP), false);
      #endif

      #if TEMP_SENSOR_3 != 0
        #undef THERMISTOR_ID
        #define THERMISTOR_ID TEMP_SENSOR_3
        #include "thermistornames.h"
        STATIC_ITEM("T3: " THERMISTOR_NAME, false, true);
        STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_3_MINTEMP), false);
        STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_3_MAXTEMP), false);
      #endif

      #if TEMP_SENSOR_BED != 0
        #undef THERMISTOR_ID
        #define THERMISTOR_ID TEMP_SENSOR_BED
        #include "thermistornames.h"
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
    void lcd_info_board_menu() {
      if (lcd_clicked) { return lcd_goto_previous_menu(); }
      START_SCREEN();
      STATIC_ITEM(BOARD_NAME, true, true);                           // MyPrinterController
      STATIC_ITEM(MSG_INFO_BAUDRATE ": " STRINGIFY(BAUDRATE), true); // Baud: 250000
      STATIC_ITEM(MSG_INFO_PROTOCOL ": " PROTOCOL_VERSION, true);    // Protocol: 1.0
      #ifdef POWER_SUPPLY
        #if (POWER_SUPPLY == 1)
          STATIC_ITEM(MSG_INFO_PSU ": ATX", true);  // Power Supply: ATX
        #elif (POWER_SUPPLY == 2)
          STATIC_ITEM(MSG_INFO_PSU ": XBox", true); // Power Supply: XBox
        #endif
      #endif // POWER_SUPPLY
      END_SCREEN();
    }

    /**
     *
     * About Printer > Printer Info
     *
     */
    void lcd_info_printer_menu() {
      if (lcd_clicked) { return lcd_goto_previous_menu(); }
      START_SCREEN();
      STATIC_ITEM(MSG_MARLIN, true, true);                             // Marlin
      STATIC_ITEM(SHORT_BUILD_VERSION, true);                          // x.x.x-Branch
      STATIC_ITEM(STRING_DISTRIBUTION_DATE, true);                     // YYYY-MM-DD HH:MM
      STATIC_ITEM(MACHINE_NAME, true);                                 // My3DPrinter
      STATIC_ITEM(WEBSITE_URL, true);                                  // www.my3dprinter.com
      STATIC_ITEM(MSG_INFO_EXTRUDERS ": " STRINGIFY(EXTRUDERS), true); // Extruders: 2
      END_SCREEN();
    }

    /**
     *
     * "About Printer" submenu
     *
     */
    void lcd_info_menu() {
      START_MENU();
      MENU_BACK(MSG_MAIN);
      MENU_ITEM(submenu, MSG_INFO_PRINTER_MENU, lcd_info_printer_menu);        // Printer Info >
      MENU_ITEM(submenu, MSG_INFO_BOARD_MENU, lcd_info_board_menu);            // Board Info >
      MENU_ITEM(submenu, MSG_INFO_THERMISTOR_MENU, lcd_info_thermistors_menu); // Thermistors >
      #if ENABLED(PRINTCOUNTER)
        MENU_ITEM(submenu, MSG_INFO_STATS_MENU, lcd_info_stats_menu);          // Printer Statistics >
      #endif
      END_MENU();
    }
  #endif // LCD_INFO_MENU

  #if ENABLED(FILAMENT_CHANGE_FEATURE)
    void lcd_filament_change_toocold_menu() {
      START_MENU();
      STATIC_ITEM(MSG_HEATING_FAILED_LCD, true, true);
      MENU_BACK(MSG_FILAMENTCHANGE);
      END_MENU();
    }

    void lcd_filament_change_resume_print() {
      filament_change_menu_response = FILAMENT_CHANGE_RESPONSE_RESUME_PRINT;
      lcd_goto_screen(lcd_status_screen);
    }

    void lcd_filament_change_extrude_more() {
      filament_change_menu_response = FILAMENT_CHANGE_RESPONSE_EXTRUDE_MORE;
    }

    void lcd_filament_change_option_menu() {
      START_MENU();
      #if LCD_HEIGHT > 2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_OPTION_HEADER, true, false);
      #endif
      MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_RESUME, lcd_filament_change_resume_print);
      MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_EXTRUDE, lcd_filament_change_extrude_more);
      END_MENU();
    }

    void lcd_filament_change_init_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_1);
      #ifdef MSG_FILAMENT_CHANGE_INIT_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_INIT_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_unload_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_1);
      #ifdef MSG_FILAMENT_CHANGE_UNLOAD_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_UNLOAD_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_insert_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_1);
      #ifdef MSG_FILAMENT_CHANGE_INSERT_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_INSERT_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_load_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_1);
      #ifdef MSG_FILAMENT_CHANGE_LOAD_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_LOAD_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_extrude_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_EXTRUDE_1);
      #ifdef MSG_FILAMENT_CHANGE_EXTRUDE_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_EXTRUDE_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_EXTRUDE_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_EXTRUDE_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_resume_message() {
      START_SCREEN();
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, true, true);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_1);
      #ifdef MSG_FILAMENT_CHANGE_RESUME_2
        STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_2);
      #endif
      #ifdef MSG_FILAMENT_CHANGE_RESUME_3
        STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_3);
      #endif
      END_SCREEN();
    }

    void lcd_filament_change_show_message(const FilamentChangeMessage message) {
      switch (message) {
        case FILAMENT_CHANGE_MESSAGE_INIT:
          defer_return_to_status = true;
          lcd_goto_screen(lcd_filament_change_init_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_UNLOAD:
          lcd_goto_screen(lcd_filament_change_unload_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_INSERT:
          lcd_goto_screen(lcd_filament_change_insert_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_LOAD:
          lcd_goto_screen(lcd_filament_change_load_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_EXTRUDE:
          lcd_goto_screen(lcd_filament_change_extrude_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_OPTION:
          filament_change_menu_response = FILAMENT_CHANGE_RESPONSE_WAIT_FOR;
          lcd_goto_screen(lcd_filament_change_option_menu);
          break;
        case FILAMENT_CHANGE_MESSAGE_RESUME:
          lcd_goto_screen(lcd_filament_change_resume_message);
          break;
        case FILAMENT_CHANGE_MESSAGE_STATUS:
          lcd_return_to_status();
          break;
      }
    }

  #endif // FILAMENT_CHANGE_FEATURE

  /**
   *
   * Functions for editing single values
   *
   * The "menu_edit_type" macro generates the functions needed to edit a numerical value.
   *
   * For example, menu_edit_type(int, int3, itostr3, 1) expands into these functions:
   *
   *   bool _menu_edit_int3();
   *   void menu_edit_int3(); // edit int (interactively)
   *   void menu_edit_callback_int3(); // edit int (interactively) with callback on completion
   *   void _menu_action_setting_edit_int3(const char * const pstr, int * const ptr, const int minValue, const int maxValue);
   *   void menu_action_setting_edit_int3(const char * const pstr, int * const ptr, const int minValue, const int maxValue);
   *   void menu_action_setting_edit_callback_int3(const char * const pstr, int * const ptr, const int minValue, const int maxValue, const screenFunc_t callback); // edit int with callback
   *
   * You can then use one of the menu macros to present the edit interface:
   *   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
   *
   * This expands into a more primitive menu item:
   *   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
   *
   *
   * Also: MENU_MULTIPLIER_ITEM_EDIT, MENU_ITEM_EDIT_CALLBACK, and MENU_MULTIPLIER_ITEM_EDIT_CALLBACK
   *
   *       menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
   */
  #define menu_edit_type(_type, _name, _strFunc, _scale) \
    bool _menu_edit_ ## _name () { \
      ENCODER_DIRECTION_NORMAL(); \
      if ((int32_t)encoderPosition < 0) encoderPosition = 0; \
      if ((int32_t)encoderPosition > maxEditValue) encoderPosition = maxEditValue; \
      if (lcdDrawUpdate) \
        lcd_implementation_drawedit(editLabel, _strFunc(((_type)((int32_t)encoderPosition + minEditValue)) * (1.0 / _scale))); \
      if (lcd_clicked) { \
        *((_type*)editValue) = ((_type)((int32_t)encoderPosition + minEditValue)) * (1.0 / _scale); \
        lcd_goto_previous_menu(); \
      } \
      return lcd_clicked; \
    } \
    void menu_edit_ ## _name () { _menu_edit_ ## _name(); } \
    void menu_edit_callback_ ## _name () { if (_menu_edit_ ## _name ()) (*callbackFunc)(); } \
    void _menu_action_setting_edit_ ## _name (const char * const pstr, _type* const ptr, const _type minValue, const _type maxValue) { \
      lcd_save_previous_screen(); \
      \
      lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; \
      \
      editLabel = pstr; \
      editValue = ptr; \
      minEditValue = minValue * _scale; \
      maxEditValue = maxValue * _scale - minEditValue; \
      encoderPosition = (*ptr) * _scale - minEditValue; \
    } \
    void menu_action_setting_edit_ ## _name (const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue) { \
      _menu_action_setting_edit_ ## _name(pstr, ptr, minValue, maxValue); \
      currentScreen = menu_edit_ ## _name; \
    }\
    void menu_action_setting_edit_callback_ ## _name (const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue, const screenFunc_t callback) { \
      _menu_action_setting_edit_ ## _name(pstr, ptr, minValue, maxValue); \
      currentScreen = menu_edit_callback_ ## _name; \
      callbackFunc = callback; \
    } \
    typedef void _name

  menu_edit_type(int, int3, itostr3, 1);
  menu_edit_type(float, float3, ftostr3, 1.0);
  menu_edit_type(float, float32, ftostr32, 100.0);
  menu_edit_type(float, float43, ftostr43sign, 1000.0);
  menu_edit_type(float, float5, ftostr5rj, 0.01);
  menu_edit_type(float, float51, ftostr51sign, 10.0);
  menu_edit_type(float, float52, ftostr52sign, 100.0);
  menu_edit_type(float, float62, ftostr62rj, 100.0);
  menu_edit_type(unsigned long, long5, ftostr5rj, 0.01);

  /**
   *
   * Handlers for RepRap World Keypad input
   *
   */
  #if ENABLED(REPRAPWORLD_KEYPAD)
    void _reprapworld_keypad_move(AxisEnum axis, int dir) {
      move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
      encoderPosition = dir;
      switch (axis) {
        case X_AXIS: lcd_move_x(); break;
        case Y_AXIS: lcd_move_y(); break;
        case Z_AXIS: lcd_move_z();
        default: break;
      }
    }
    void reprapworld_keypad_move_z_up()    { _reprapworld_keypad_move(Z_AXIS,  1); }
    void reprapworld_keypad_move_z_down()  { _reprapworld_keypad_move(Z_AXIS, -1); }
    void reprapworld_keypad_move_x_left()  { _reprapworld_keypad_move(X_AXIS, -1); }
    void reprapworld_keypad_move_x_right() { _reprapworld_keypad_move(X_AXIS,  1); }
    void reprapworld_keypad_move_y_up()    { _reprapworld_keypad_move(Y_AXIS, -1); }
    void reprapworld_keypad_move_y_down()  { _reprapworld_keypad_move(Y_AXIS,  1); }
    void reprapworld_keypad_move_home()    { enqueue_and_echo_commands_P(PSTR("G28")); } // move all axes home and wait
    void reprapworld_keypad_move_menu()    { lcd_goto_screen(lcd_move_menu); }

    inline void handle_reprapworld_keypad() {

      static uint8_t keypad_debounce = 0;

      if (!REPRAPWORLD_KEYPAD_PRESSED) {
        if (keypad_debounce > 0) keypad_debounce--;
      }
      else if (!keypad_debounce) {
        keypad_debounce = 2;

        if (REPRAPWORLD_KEYPAD_MOVE_MENU)       reprapworld_keypad_move_menu();

        #if DISABLED(DELTA) && Z_HOME_DIR == -1
          if (REPRAPWORLD_KEYPAD_MOVE_Z_UP)     reprapworld_keypad_move_z_up();
        #endif

        if (axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS]) {
          #if ENABLED(DELTA) || Z_HOME_DIR != -1
            if (REPRAPWORLD_KEYPAD_MOVE_Z_UP)   reprapworld_keypad_move_z_up();
          #endif
          if (REPRAPWORLD_KEYPAD_MOVE_Z_DOWN)   reprapworld_keypad_move_z_down();
          if (REPRAPWORLD_KEYPAD_MOVE_X_LEFT)   reprapworld_keypad_move_x_left();
          if (REPRAPWORLD_KEYPAD_MOVE_X_RIGHT)  reprapworld_keypad_move_x_right();
          if (REPRAPWORLD_KEYPAD_MOVE_Y_DOWN)   reprapworld_keypad_move_y_down();
          if (REPRAPWORLD_KEYPAD_MOVE_Y_UP)     reprapworld_keypad_move_y_up();
        }
        else {
          if (REPRAPWORLD_KEYPAD_MOVE_HOME)     reprapworld_keypad_move_home();
        }
      }
    }

  #endif // REPRAPWORLD_KEYPAD

  /**
   *
   * Audio feedback for controller clicks
   *
   */
  void lcd_buzz(long duration, uint16_t freq) {
    #if ENABLED(LCD_USE_I2C_BUZZER)
      lcd.buzz(duration, freq);
    #elif PIN_EXISTS(BEEPER)
      buzzer.tone(duration, freq);
    #else
      UNUSED(duration); UNUSED(freq);
    #endif
  }

  void lcd_quick_feedback() {
    lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;
    buttons = 0;
    next_button_update_ms = millis() + 500;

    // Buzz and wait. The delay is needed for buttons to settle!
    lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);
    #if ENABLED(LCD_USE_I2C_BUZZER)
      delay(10);
    #elif PIN_EXISTS(BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #endif
  }

  /**
   *
   * Menu actions
   *
   */
  void _menu_action_back() { lcd_goto_previous_menu(); }
  void menu_action_submenu(screenFunc_t func) { lcd_save_previous_screen(); lcd_goto_screen(func); }
  void menu_action_gcode(const char* pgcode) { enqueue_and_echo_commands_P(pgcode); }
  void menu_action_function(screenFunc_t func) { (*func)(); }

  #if ENABLED(SDSUPPORT)

    void menu_action_sdfile(const char* filename, char* longFilename) {
      UNUSED(longFilename);
      card.openAndPrintFile(filename);
      lcd_return_to_status();
    }

    void menu_action_sddirectory(const char* filename, char* longFilename) {
      UNUSED(longFilename);
      card.chdir(filename);
      encoderPosition = 0;
      screen_changed = true;
      lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;
    }

  #endif //SDSUPPORT

  void menu_action_setting_edit_bool(const char* pstr, bool* ptr) {UNUSED(pstr); *ptr = !(*ptr); lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }
  void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, screenFunc_t callback) {
    menu_action_setting_edit_bool(pstr, ptr);
    (*callback)();
  }

#endif // ULTIPANEL

void lcd_init() {

  lcd_implementation_init(
    #if ENABLED(LCD_PROGRESS_BAR)
      true
    #endif
  );

  #if ENABLED(NEWPANEL)
    #if BUTTON_EXISTS(EN1)
      SET_INPUT(BTN_EN1);
      WRITE(BTN_EN1, HIGH);
    #endif

    #if BUTTON_EXISTS(EN2)
      SET_INPUT(BTN_EN2);
      WRITE(BTN_EN2, HIGH);
    #endif

    #if BUTTON_EXISTS(ENC)
      SET_INPUT(BTN_ENC);
      WRITE(BTN_ENC, HIGH);
    #endif

    #if ENABLED(REPRAPWORLD_KEYPAD)
      SET_OUTPUT(SHIFT_CLK);
      OUT_WRITE(SHIFT_LD, HIGH);
      SET_INPUT_PULLUP(SHIFT_OUT);
    #endif

    #if BUTTON_EXISTS(UP)
      SET_INPUT(BTN_UP);
    #endif
    #if BUTTON_EXISTS(DWN)
      SET_INPUT(BTN_DWN);
    #endif
    #if BUTTON_EXISTS(LFT)
      SET_INPUT(BTN_LFT);
    #endif
    #if BUTTON_EXISTS(RT)
      SET_INPUT(BTN_RT);
    #endif

  #else // !NEWPANEL

    #if ENABLED(SR_LCD_2W_NL) // Non latching 2 wire shift register
      SET_OUTPUT(SR_DATA_PIN);
      SET_OUTPUT(SR_CLK_PIN);
    #elif defined(SHIFT_CLK)
      SET_OUTPUT(SHIFT_CLK);
      OUT_WRITE(SHIFT_LD, HIGH);
      OUT_WRITE(SHIFT_EN, LOW);
      SET_INPUT_PULLUP(SHIFT_OUT);
    #endif // SR_LCD_2W_NL

  #endif // !NEWPANEL

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT(SD_DETECT_PIN);
    WRITE(SD_DETECT_PIN, HIGH);
    lcd_sd_status = 2; // UNKNOWN
  #endif

  #if ENABLED(LCD_HAS_SLOW_BUTTONS)
    slow_buttons = 0;
  #endif

  lcd_buttons_update();

  #if ENABLED(ULTIPANEL)
    encoderDiff = 0;
  #endif
}

int lcd_strlen(const char* s) {
  int i = 0, j = 0;
  while (s[i]) {
    #if ENABLED(MAPPER_NON)
      j++;
    #else
      if ((s[i] & 0xC0u) != 0x80u) j++;
    #endif
    i++;
  }
  return j;
}

int lcd_strlen_P(const char* s) {
  int j = 0;
  while (pgm_read_byte(s)) {
    #if ENABLED(MAPPER_NON)
      j++;
    #else
      if ((pgm_read_byte(s) & 0xC0u) != 0x80u) j++;
    #endif
    s++;
  }
  return j;
}

bool lcd_blink() {
  static uint8_t blink = 0;
  static millis_t next_blink_ms = 0;
  millis_t ms = millis();
  if (ELAPSED(ms, next_blink_ms)) {
    blink ^= 0xFF;
    next_blink_ms = ms + 1000 - LCD_UPDATE_INTERVAL / 2;
  }
  return blink != 0;
}

/**
 * Update the LCD, read encoder buttons, etc.
 *   - Read button states
 *   - Check the SD Card slot state
 *   - Act on RepRap World keypad input
 *   - Update the encoder position
 *   - Apply acceleration to the encoder position
 *   - Set lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NOW on controller events
 *   - Reset the Info Screen timeout if there's any input
 *   - Update status indicators, if any
 *
 *   Run the current LCD menu handler callback function:
 *   - Call the handler only if lcdDrawUpdate != LCDVIEW_NONE
 *   - Before calling the handler, LCDVIEW_CALL_NO_REDRAW => LCDVIEW_NONE
 *   - Call the menu handler. Menu handlers should do the following:
 *     - If a value changes, set lcdDrawUpdate to LCDVIEW_REDRAW_NOW and draw the value
 *       (Encoder events automatically set lcdDrawUpdate for you.)
 *     - if (lcdDrawUpdate) { redraw }
 *     - Before exiting the handler set lcdDrawUpdate to:
 *       - LCDVIEW_CLEAR_CALL_REDRAW to clear screen and set LCDVIEW_CALL_REDRAW_NEXT.
 *       - LCDVIEW_REDRAW_NOW or LCDVIEW_NONE to keep drawingm but only in this loop.
 *       - LCDVIEW_CALL_REDRAW_NEXT to keep drawing and draw on the next loop also.
 *       - LCDVIEW_CALL_NO_REDRAW to keep drawing (or start drawing) with no redraw on the next loop.
 *     - NOTE: For graphical displays menu handlers may be called 2 or more times per loop,
 *             so don't change lcdDrawUpdate without considering this.
 *
 *   After the menu handler callback runs (or not):
 *   - Clear the LCD if lcdDrawUpdate == LCDVIEW_CLEAR_CALL_REDRAW
 *   - Update lcdDrawUpdate for the next loop (i.e., move one state down, usually)
 *
 * No worries. This function is only called from the main thread.
 */
void lcd_update() {

  #if ENABLED(ULTIPANEL)
    static millis_t return_to_status_ms = 0;
    manage_manual_move();

    lcd_buttons_update();

    // If the action button is pressed...
    if (LCD_CLICKED) {
      if (!wait_for_unclick) {           // If not waiting for a debounce release:
        wait_for_unclick = true;         //  Set debounce flag to ignore continous clicks
        lcd_clicked = !wait_for_user;    //  Keep the click if not waiting for a user-click
        wait_for_user = false;           //  Any click clears wait for user
        lcd_quick_feedback();            //  Always make a click sound
      }
    }
    else wait_for_unclick = false;
  #endif

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)

    bool sd_status = IS_SD_INSERTED;
    if (sd_status != lcd_sd_status && lcd_detected()) {

      if (sd_status) {
        card.initsd();
        if (lcd_sd_status != 2) LCD_MESSAGEPGM(MSG_SD_INSERTED);
      }
      else {
        card.release();
        if (lcd_sd_status != 2) LCD_MESSAGEPGM(MSG_SD_REMOVED);
      }

      lcd_sd_status = sd_status;
      lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;
      lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
        #if ENABLED(LCD_PROGRESS_BAR)
          currentScreen == lcd_status_screen
        #endif
      );
    }

  #endif //SDSUPPORT && SD_DETECT_PIN

  millis_t ms = millis();
  if (ELAPSED(ms, next_lcd_update_ms)
    #if ENABLED(DOGLCD)
      || drawing_screen
    #endif
    ) {

    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;

    #if ENABLED(LCD_HAS_STATUS_INDICATORS)
      lcd_implementation_update_indicators();
    #endif

    #if ENABLED(ULTIPANEL)

      #if ENABLED(LCD_HAS_SLOW_BUTTONS)
        slow_buttons = lcd_implementation_read_slow_buttons(); // buttons which take too long to read in interrupt context
      #endif

      #if ENABLED(REPRAPWORLD_KEYPAD)
        handle_reprapworld_keypad();
      #endif

      bool encoderPastThreshold = (abs(encoderDiff) >= ENCODER_PULSES_PER_STEP);
      if (encoderPastThreshold || lcd_clicked) {
        if (encoderPastThreshold) {
          int32_t encoderMultiplier = 1;

          #if ENABLED(ENCODER_RATE_MULTIPLIER)

            if (encoderRateMultiplierEnabled) {
              int32_t encoderMovementSteps = abs(encoderDiff) / ENCODER_PULSES_PER_STEP;

              if (lastEncoderMovementMillis != 0) {
                // Note that the rate is always calculated between to passes through the
                // loop and that the abs of the encoderDiff value is tracked.
                float encoderStepRate = (float)(encoderMovementSteps) / ((float)(ms - lastEncoderMovementMillis)) * 1000.0;

                if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
                else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

                #if ENABLED(ENCODER_RATE_MULTIPLIER_DEBUG)
                  SERIAL_ECHO_START;
                  SERIAL_ECHOPAIR("Enc Step Rate: ", encoderStepRate);
                  SERIAL_ECHOPAIR("  Multiplier: ", encoderMultiplier);
                  SERIAL_ECHOPAIR("  ENCODER_10X_STEPS_PER_SEC: ", ENCODER_10X_STEPS_PER_SEC);
                  SERIAL_ECHOPAIR("  ENCODER_100X_STEPS_PER_SEC: ", ENCODER_100X_STEPS_PER_SEC);
                  SERIAL_EOL;
                #endif //ENCODER_RATE_MULTIPLIER_DEBUG
              }

              lastEncoderMovementMillis = ms;
            } // encoderRateMultiplierEnabled
          #endif //ENCODER_RATE_MULTIPLIER

          encoderPosition += (encoderDiff * encoderMultiplier) / ENCODER_PULSES_PER_STEP;
          encoderDiff = 0;
        }
        return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
        lcdDrawUpdate =
          #if ENABLED(DOGLCD)
            LCDVIEW_CALL_REDRAW_NEXT
          #else
            LCDVIEW_REDRAW_NOW
          #endif
        ;
      }
    #endif // ULTIPANEL

    // We arrive here every ~100ms when idling often enough.
    // Instead of tracking the changes simply redraw the Info Screen ~1 time a second.
    static int8_t lcd_status_update_delay = 1; // first update one loop delayed
    if (
      #if ENABLED(ULTIPANEL)
        currentScreen == lcd_status_screen &&
      #endif
      !lcd_status_update_delay--
    ) {
      lcd_status_update_delay = 9
        #if ENABLED(DOGLCD)
          + 3
        #endif
      ;
      max_display_update_time--;
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
    }

    // then we want to use 1/2 of the time only.
    uint16_t bbr2 = planner.block_buffer_runtime() >> 1;

    #if ENABLED(DOGLCD)
      if ((lcdDrawUpdate || drawing_screen) && (!bbr2 || (bbr2 > max_display_update_time)))
    #else
      if (lcdDrawUpdate && (!bbr2 || (bbr2 > max_display_update_time)))
    #endif
    {
      #if ENABLED(DOGLCD)
        if (!drawing_screen)
      #endif
        {
          switch (lcdDrawUpdate) {
            case LCDVIEW_CALL_NO_REDRAW:
              lcdDrawUpdate = LCDVIEW_NONE;
              break;
            case LCDVIEW_CLEAR_CALL_REDRAW: // set by handlers, then altered after (rarely occurs here)
            case LCDVIEW_CALL_REDRAW_NEXT:  // set by handlers, then altered after (never occurs here?)
              lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
            case LCDVIEW_REDRAW_NOW:        // set above, or by a handler through LCDVIEW_CALL_REDRAW_NEXT
            case LCDVIEW_NONE:
              break;
          } // switch
        }
      #if ENABLED(ULTIPANEL)
        #define CURRENTSCREEN() (*currentScreen)(), lcd_clicked = false
      #else
        #define CURRENTSCREEN() lcd_status_screen()
      #endif

      #if ENABLED(DOGLCD)  // Changes due to different driver architecture of the DOGM display
        if (!drawing_screen) {
          u8g.firstPage();
          drawing_screen = 1;
        }
        lcd_setFont(FONT_MENU);
        u8g.setColorIndex(1);
        CURRENTSCREEN();
        if (drawing_screen && (drawing_screen = u8g.nextPage())) {
          NOLESS(max_display_update_time, millis() - ms);
          return;
        }
      #else
        CURRENTSCREEN();
      #endif
      NOLESS(max_display_update_time, millis() - ms);
    }

    #if ENABLED(ULTIPANEL)

      // Return to Status Screen after a timeout
      if (currentScreen == lcd_status_screen || defer_return_to_status)
        return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
      else if (ELAPSED(ms, return_to_status_ms))
        lcd_return_to_status();

    #endif // ULTIPANEL

    #if ENABLED(DOGLCD)
      if (!drawing_screen)
    #endif
      {
        switch (lcdDrawUpdate) {
          case LCDVIEW_CLEAR_CALL_REDRAW:
            lcd_implementation_clear();
          case LCDVIEW_CALL_REDRAW_NEXT:
            lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
            break;
          case LCDVIEW_REDRAW_NOW:
            lcdDrawUpdate = LCDVIEW_NONE;
            break;
          case LCDVIEW_NONE:
            break;
        } // switch
      }
  } // ELAPSED(ms, next_lcd_update_ms)
}

void set_utf_strlen(char* s, uint8_t n) {
  uint8_t i = 0, j = 0;
  while (s[i] && (j < n)) {
    #if ENABLED(MAPPER_NON)
      j++;
    #else
      if ((s[i] & 0xC0u) != 0x80u) j++;
    #endif
    i++;
  }
  while (j++ < n) s[i++] = ' ';
  s[i] = '\0';
}

void lcd_finishstatus(bool persist=false) {
  set_utf_strlen(lcd_status_message, LCD_WIDTH);
  #if !(ENABLED(LCD_PROGRESS_BAR) && (PROGRESS_MSG_EXPIRE > 0))
    UNUSED(persist);
  #endif

  #if ENABLED(LCD_PROGRESS_BAR)
    progress_bar_ms = millis();
    #if PROGRESS_MSG_EXPIRE > 0
      expire_status_ms = persist ? 0 : progress_bar_ms + PROGRESS_MSG_EXPIRE;
    #endif
  #endif
  lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

  #if ENABLED(FILAMENT_LCD_DISPLAY)
    previous_lcd_status_ms = millis();  //get status message to show up for a while
  #endif
}

#if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
  void dontExpireStatus() { expire_status_ms = 0; }
#endif

bool lcd_hasstatus() { return (lcd_status_message[0] != '\0'); }

void lcd_setstatus(const char* const message, bool persist) {
  if (lcd_status_message_level > 0) return;
  strncpy(lcd_status_message, message, 3 * (LCD_WIDTH));
  lcd_finishstatus(persist);
}

void lcd_setstatuspgm(const char* const message, uint8_t level) {
  if (level < lcd_status_message_level) return;
  lcd_status_message_level = level;
  strncpy_P(lcd_status_message, message, 3 * (LCD_WIDTH));
  lcd_finishstatus(level > 0);
}

void lcd_setalertstatuspgm(const char* const message) {
  lcd_setstatuspgm(message, 1);
  #if ENABLED(ULTIPANEL)
    lcd_return_to_status();
  #endif
}

void lcd_reset_alert_level() { lcd_status_message_level = 0; }

#if HAS_LCD_CONTRAST
  void set_lcd_contrast(const int value) {
    lcd_contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
    u8g.setContrast(lcd_contrast);
  }
#endif

#if ENABLED(ULTIPANEL)

  /**
   * Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
   * These values are independent of which pins are used for EN_A and EN_B indications
   * The rotary encoder part is also independent to the chipset used for the LCD
   */
  #if defined(EN_A) && defined(EN_B)
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
  #endif

  #define GET_BUTTON_STATES(DST) \
    uint8_t new_##DST = 0; \
    WRITE(SHIFT_LD, LOW); \
    WRITE(SHIFT_LD, HIGH); \
    for (int8_t i = 0; i < 8; i++) { \
      new_##DST >>= 1; \
      if (READ(SHIFT_OUT)) SBI(new_##DST, 7); \
      WRITE(SHIFT_CLK, HIGH); \
      WRITE(SHIFT_CLK, LOW); \
    } \
    DST = ~new_##DST; //invert it, because a pressed switch produces a logical 0


  /**
   * Read encoder buttons from the hardware registers
   * Warning: This function is called from interrupt context!
   */
  void lcd_buttons_update() {
    millis_t now = millis();
    if (ELAPSED(now, next_button_update_ms)) {

      #if ENABLED(NEWPANEL)
        uint8_t newbutton = 0;

        #if BUTTON_EXISTS(EN1)
          if (BUTTON_PRESSED(EN1)) newbutton |= EN_A;
        #endif

        #if BUTTON_EXISTS(EN2)
          if (BUTTON_PRESSED(EN2)) newbutton |= EN_B;
        #endif

        #if BUTTON_EXISTS(ENC)
          if (BUTTON_PRESSED(ENC)) newbutton |= EN_C;
        #endif

        #if LCD_HAS_DIRECTIONAL_BUTTONS

          // Manage directional buttons
          #if ENABLED(REVERSE_MENU_DIRECTION)
            #define _ENCODER_UD_STEPS (ENCODER_STEPS_PER_MENU_ITEM * encoderDirection)
          #else
            #define _ENCODER_UD_STEPS ENCODER_STEPS_PER_MENU_ITEM
          #endif
          #if ENABLED(REVERSE_ENCODER_DIRECTION)
            #define ENCODER_UD_STEPS _ENCODER_UD_STEPS
            #define ENCODER_LR_PULSES ENCODER_PULSES_PER_STEP
          #else
            #define ENCODER_UD_STEPS -(_ENCODER_UD_STEPS)
            #define ENCODER_LR_PULSES -(ENCODER_PULSES_PER_STEP)
          #endif

          if (false) {
            // for the else-ifs below
          }
          #if BUTTON_EXISTS(UP)
            else if (BUTTON_PRESSED(UP)) {
              encoderDiff = -(ENCODER_UD_STEPS);
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(DWN)
            else if (BUTTON_PRESSED(DWN)) {
              encoderDiff = ENCODER_UD_STEPS;
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(LFT)
            else if (BUTTON_PRESSED(LFT)) {
              encoderDiff = -(ENCODER_LR_PULSES);
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(RT)
            else if (BUTTON_PRESSED(RT)) {
              encoderDiff = ENCODER_LR_PULSES;
              next_button_update_ms = now + 300;
            }
          #endif

        #endif // LCD_HAS_DIRECTIONAL_BUTTONS

        buttons = newbutton;
        #if ENABLED(LCD_HAS_SLOW_BUTTONS)
          buttons |= slow_buttons;
        #endif
        #if ENABLED(REPRAPWORLD_KEYPAD)
          GET_BUTTON_STATES(buttons_reprapworld_keypad);
        #endif
      #else
        GET_BUTTON_STATES(buttons);
      #endif //!NEWPANEL

    } // next_button_update_ms

    // Manage encoder rotation
    #if ENABLED(REVERSE_MENU_DIRECTION) && ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff -= encoderDirection)
      #define ENCODER_DIFF_CCW (encoderDiff += encoderDirection)
    #elif ENABLED(REVERSE_MENU_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff += encoderDirection)
      #define ENCODER_DIFF_CCW (encoderDiff -= encoderDirection)
    #elif ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff--)
      #define ENCODER_DIFF_CCW (encoderDiff++)
    #else
      #define ENCODER_DIFF_CW  (encoderDiff++)
      #define ENCODER_DIFF_CCW (encoderDiff--)
    #endif
    #define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: ENCODER_DIFF_CW; break; case _E2: ENCODER_DIFF_CCW; }

    uint8_t enc = 0;
    if (buttons & EN_A) enc |= B01;
    if (buttons & EN_B) enc |= B10;
    if (enc != lastEncoderBits) {
      switch (enc) {
        case encrot0: ENCODER_SPIN(encrot3, encrot1); break;
        case encrot1: ENCODER_SPIN(encrot0, encrot2); break;
        case encrot2: ENCODER_SPIN(encrot1, encrot3); break;
        case encrot3: ENCODER_SPIN(encrot2, encrot0); break;
      }
    }
    lastEncoderBits = enc;
  }

  #if (ENABLED(LCD_I2C_TYPE_MCP23017) || ENABLED(LCD_I2C_TYPE_MCP23008)) && ENABLED(DETECT_DEVICE)
    bool lcd_detected() { return lcd.LcdDetected() == 1; }
  #else
    bool lcd_detected() { return true; }
  #endif

#endif // ULTIPANEL

#endif // ULTRA_LCD
