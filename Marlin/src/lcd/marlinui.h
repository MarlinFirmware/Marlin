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
#pragma once

#include "../module/motion.h"
#include "buttons.h"

#include "../inc/MarlinConfig.h"

#if HAS_BUZZER
  #include "../libs/buzzer.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "../sd/cardreader.h"
#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "tft_io/touch_calibration.h"
#endif

#if ANY(HAS_LCD_MENU, ULTIPANEL_FEEDMULTIPLY, SOFT_RESET_ON_KILL)
  #define HAS_ENCODER_ACTION 1
#endif

#if HAS_STATUS_MESSAGE
  #define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80U)
#endif

#if E_MANUAL > 1
  #define MULTI_E_MANUAL 1
#endif

#if HAS_DISPLAY
  #include "../module/printcounter.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE) && ANY(HAS_LCD_MENU, EXTENSIBLE_UI, HAS_DWIN_E3V2)
  #include "../feature/pause.h"
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "e3v2/creality/dwin.h"
#elif ENABLED(DWIN_CREALITY_LCD_ENHANCED)
  #include "e3v2/enhanced/dwin.h"
#endif

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80U)

#if HAS_WIRED_LCD

  enum LCDViewAction : uint8_t {
    LCDVIEW_NONE,
    LCDVIEW_REDRAW_NOW,
    LCDVIEW_CALL_REDRAW_NEXT,
    LCDVIEW_CLEAR_CALL_REDRAW,
    LCDVIEW_CALL_NO_REDRAW
  };

  #if HAS_ADC_BUTTONS
    uint8_t get_ADC_keyValue();
  #endif

  #if HAS_LCD_MENU

    #include "lcdprint.h"

    #if !HAS_GRAPHICAL_TFT
      void _wrap_string(uint8_t &col, uint8_t &row, const char * const string, read_byte_cb_t cb_read_byte, const bool wordwrap=false);
      inline void wrap_string_P(uint8_t &col, uint8_t &row, PGM_P const pstr, const bool wordwrap=false) { _wrap_string(col, row, pstr, read_byte_rom, wordwrap); }
      inline void wrap_string(uint8_t &col, uint8_t &row, const char * const string, const bool wordwrap=false) { _wrap_string(col, row, string, read_byte_ram, wordwrap); }
    #endif

    typedef void (*screenFunc_t)();
    typedef void (*menuAction_t)();

  #endif // HAS_LCD_MENU

#endif // HAS_WIRED_LCD

#if EITHER(HAS_WIRED_LCD, DWIN_CREALITY_LCD_JYERSUI)
  #define LCD_UPDATE_INTERVAL TERN(HAS_TOUCH_BUTTONS, 50, 100)
#endif

#if HAS_MARLINUI_U8GLIB
  enum MarlinFont : uint8_t {
    FONT_STATUSMENU = 1,
    FONT_EDIT,
    FONT_MENU
  };
#else
  enum HD44780CharSet : uint8_t {
    CHARSET_MENU,
    CHARSET_INFO,
    CHARSET_BOOT
  };
#endif

#if HAS_PREHEAT
  typedef struct {
    #if HAS_HOTEND
      celsius_t hotend_temp;
    #endif
    #if HAS_HEATED_BED
      celsius_t bed_temp;
    #endif
    #if HAS_FAN
      uint16_t fan_speed;
    #endif
  } preheat_t;
#endif

#if HAS_LCD_MENU

  // Manual Movement class
  class ManualMove {
  private:
    static AxisEnum axis;
    #if MULTI_E_MANUAL
      static int8_t e_index;
    #else
      static int8_t constexpr e_index = 0;
    #endif
    static millis_t start_time;
    #if IS_KINEMATIC
      static xyze_pos_t all_axes_destination;
    #endif
  public:
    static float menu_scale;
    #if IS_KINEMATIC
      static float offset;
    #endif
    template <typename T>
    void set_destination(const T& dest) {
      #if IS_KINEMATIC
        // Moves are segmented, so the entire move is not submitted at once.
        // Using a separate variable prevents corrupting the in-progress move.
        all_axes_destination = current_position;
        all_axes_destination.set(dest);
      #else
        // Moves are submitted as single line to the planner using buffer_line.
        current_position.set(dest);
      #endif
    }
    float axis_value(const AxisEnum axis) {
      return NATIVE_TO_LOGICAL(processing ? destination[axis] : SUM_TERN(IS_KINEMATIC, current_position[axis], offset), axis);
    }
    bool apply_diff(const AxisEnum axis, const_float_t diff, const_float_t min, const_float_t max) {
      #if IS_KINEMATIC
        float &valref = offset;
        const float rmin = min - current_position[axis], rmax = max - current_position[axis];
      #else
        float &valref = current_position[axis];
        const float rmin = min, rmax = max;
      #endif
      valref += diff;
      const float pre = valref;
      if (min != max) {
        if (diff < 0)
          NOLESS(valref, rmin);
        else
          NOMORE(valref, rmax);
      }
      return pre != valref;
    }
    #if IS_KINEMATIC
      static bool processing;
    #else
      static bool constexpr processing = false;
    #endif
    static void task();
    static void soon(const AxisEnum axis OPTARG(MULTI_E_MANUAL, const int8_t eindex=active_extruder));
  };

#endif

////////////////////////////////////////////
//////////// MarlinUI Singleton ////////////
////////////////////////////////////////////

class MarlinUI {
public:

  MarlinUI() {
    TERN_(HAS_LCD_MENU, currentScreen = status_screen);
  }

  #if HAS_MULTI_LANGUAGE
    static uint8_t language;
    static void set_language(const uint8_t lang);
  #endif

  #if HAS_MARLINUI_U8GLIB
    static void update_language_font();
  #endif

  #if ENABLED(SOUND_MENU_ITEM)
    static bool buzzer_enabled; // Initialized by settings.load()
  #else
    static constexpr bool buzzer_enabled = true;
  #endif

  #if HAS_BUZZER
    static void buzz(const long duration, const uint16_t freq);
  #endif

  FORCE_INLINE static void chirp() {
    TERN_(HAS_CHIRP, buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ));
  }

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)
    static void update_indicators();
  #endif

  // LCD implementations
  static void clear_lcd();

  #if BOTH(HAS_LCD_MENU, TOUCH_SCREEN_CALIBRATION)
    static void check_touch_calibration() {
      if (touch_calibration.need_calibration()) currentScreen = touch_calibration_screen;
    }
  #endif

  #if ENABLED(SDSUPPORT)
    #define MEDIA_MENU_GATEWAY TERN(PASSWORD_ON_SD_PRINT_MENU, password.media_gatekeeper, menu_media)
    static void media_changed(const uint8_t old_stat, const uint8_t stat);
  #endif

  #if HAS_LCD_BRIGHTNESS
    #ifndef LCD_BRIGHTNESS_MIN
      #define LCD_BRIGHTNESS_MIN   1
    #endif
    #ifndef LCD_BRIGHTNESS_MAX
      #define LCD_BRIGHTNESS_MAX 255
    #endif
    #ifndef DEFAULT_LCD_BRIGHTNESS
      #define DEFAULT_LCD_BRIGHTNESS LCD_BRIGHTNESS_MAX
    #endif
    static uint8_t brightness;
    static bool backlight;
    static void _set_brightness(); // Implementation-specific
    static void set_brightness(const uint8_t value);
    FORCE_INLINE static void refresh_brightness() { set_brightness(brightness); }
  #endif

  #if HAS_DWIN_E3V2_BASIC
    static void refresh();
  #else
    FORCE_INLINE static void refresh() {
      TERN_(HAS_WIRED_LCD, refresh(LCDVIEW_CLEAR_CALL_REDRAW));
    }
  #endif

  #if HAS_WIRED_LCD
    static bool detected();
    static void init_lcd();
  #else
    static inline bool detected() { return true; }
    static inline void init_lcd() {}
  #endif

  #if HAS_PRINT_PROGRESS
    #if HAS_PRINT_PROGRESS_PERMYRIAD
      typedef uint16_t progress_t;
      #define PROGRESS_SCALE 100U
      #define PROGRESS_MASK 0x7FFF
    #else
      typedef uint8_t progress_t;
      #define PROGRESS_SCALE 1U
      #define PROGRESS_MASK 0x7F
    #endif
    #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
      static progress_t progress_override;
      static void set_progress(const progress_t p) { progress_override = _MIN(p, 100U * (PROGRESS_SCALE)); }
      static void set_progress_done() { progress_override = (PROGRESS_MASK + 1U) + 100U * (PROGRESS_SCALE); }
      static void progress_reset() { if (progress_override & (PROGRESS_MASK + 1U)) set_progress(0); }
      #if ENABLED(SHOW_REMAINING_TIME)
        static inline uint32_t _calculated_remaining_time() {
          const duration_t elapsed = print_job_timer.duration();
          const progress_t progress = _get_progress();
          return progress ? elapsed.value * (100 * (PROGRESS_SCALE) - progress) / progress : 0;
        }
        #if ENABLED(USE_M73_REMAINING_TIME)
          static uint32_t remaining_time;
          FORCE_INLINE static void set_remaining_time(const uint32_t r) { remaining_time = r; }
          FORCE_INLINE static uint32_t get_remaining_time() { return remaining_time ?: _calculated_remaining_time(); }
          FORCE_INLINE static void reset_remaining_time() { set_remaining_time(0); }
        #else
          FORCE_INLINE static uint32_t get_remaining_time() { return _calculated_remaining_time(); }
        #endif
      #endif
    #endif
    static progress_t _get_progress();
    #if HAS_PRINT_PROGRESS_PERMYRIAD
      FORCE_INLINE static uint16_t get_progress_permyriad() { return _get_progress(); }
    #endif
    static uint8_t get_progress_percent() { return uint8_t(_get_progress() / (PROGRESS_SCALE)); }
  #else
    static constexpr uint8_t get_progress_percent() { return 0; }
  #endif

  #if HAS_STATUS_MESSAGE

    #if EITHER(HAS_WIRED_LCD, DWIN_CREALITY_LCD_ENHANCED)
      #if ENABLED(STATUS_MESSAGE_SCROLLING)
        #define MAX_MESSAGE_LENGTH _MAX(LONG_FILENAME_LENGTH, MAX_LANG_CHARSIZE * 2 * (LCD_WIDTH))
      #else
        #define MAX_MESSAGE_LENGTH (MAX_LANG_CHARSIZE * (LCD_WIDTH))
      #endif
    #else
      #define MAX_MESSAGE_LENGTH 63
    #endif

    static char status_message[];
    static uint8_t alert_level; // Higher levels block lower levels

    #if ENABLED(STATUS_MESSAGE_SCROLLING)
      static uint8_t status_scroll_offset;
      static void advance_status_scroll();
      static char* status_and_len(uint8_t &len);
    #endif

    static bool has_status();
    static void reset_status(const bool no_welcome=false);
    static void set_alert_status(FSTR_P const fstr);
    static inline void reset_alert_level() { alert_level = 0; }
  #else
    static constexpr bool has_status() { return false; }
    static inline void reset_status(const bool=false) {}
    static inline void set_alert_status(FSTR_P const) {}
    static inline void reset_alert_level() {}
  #endif

  static void set_status(const char * const cstr, const bool persist=false);
  static void set_status(FSTR_P const fstr, const int8_t level=0);
  static void status_printf(const uint8_t level, FSTR_P const fmt, ...);

  #if EITHER(HAS_DISPLAY, DWIN_CREALITY_LCD_ENHANCED)
    static void kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component);
  #else
    static inline void kill_screen(FSTR_P const, FSTR_P const) {}
  #endif

  #if HAS_DISPLAY

    static void init();
    static void update();

    static void abort_print();
    static void pause_print();
    static void resume_print();
    static void flow_fault();

    #if BOTH(PSU_CONTROL, PS_OFF_CONFIRM)
      static void poweroff();
    #endif

    #if EITHER(HAS_WIRED_LCD, DWIN_CREALITY_LCD_JYERSUI)
      static bool get_blink();
    #endif

    #if HAS_WIRED_LCD

      static millis_t next_button_update_ms;

      static LCDViewAction lcdDrawUpdate;
      FORCE_INLINE static bool should_draw() { return bool(lcdDrawUpdate); }
      FORCE_INLINE static void refresh(const LCDViewAction type) { lcdDrawUpdate = type; }

      #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
        static void draw_custom_bootscreen(const uint8_t frame=0);
        static void show_custom_bootscreen();
      #endif

      #if ENABLED(SHOW_BOOTSCREEN)
        #ifndef BOOTSCREEN_TIMEOUT
          #define BOOTSCREEN_TIMEOUT 2500
        #endif
        static void draw_marlin_bootscreen(const bool line2=false);
        static void show_marlin_bootscreen();
        static void show_bootscreen();
        static void bootscreen_completion(const millis_t sofar);
      #endif

      #if HAS_MARLINUI_U8GLIB
        static void set_font(const MarlinFont font_nr);
      #elif IS_DWIN_MARLINUI
        static void set_font(const uint8_t font_nr);
      #endif

      #if HAS_MARLINUI_HD44780
        static void set_custom_characters(const HD44780CharSet screen_charset=CHARSET_INFO);
      #endif

      #if ENABLED(LCD_PROGRESS_BAR) && !HAS_MARLINUI_U8GLIB
        static millis_t progress_bar_ms;  // Start time for the current progress bar cycle
        static void draw_progress_bar(const uint8_t percent);
        #if PROGRESS_MSG_EXPIRE > 0
          static millis_t expire_status_ms; // = 0
          FORCE_INLINE static void reset_progress_bar_timeout() { expire_status_ms = 0; }
        #endif
      #endif

      static uint8_t lcd_status_update_delay;

      #if HAS_LCD_CONTRAST
        static uint8_t contrast;
        static void _set_contrast(); // Implementation-specific
        static void set_contrast(const uint8_t value);
        FORCE_INLINE static void refresh_contrast() { set_contrast(contrast); }
      #endif

      #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
        static millis_t next_filament_display;
      #endif

      #if HAS_TOUCH_SLEEP
        static void wakeup_screen();
      #endif

      static void quick_feedback(const bool clear_buttons=true);
      #if HAS_BUZZER
        static void completion_feedback(const bool good=true);
      #else
        static inline void completion_feedback(const bool=true) { TERN_(HAS_TOUCH_SLEEP, wakeup_screen()); }
      #endif

      #if DISABLED(LIGHTWEIGHT_UI)
        static void draw_status_message(const bool blink);
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        static void draw_hotend_status(const uint8_t row, const uint8_t extruder);
      #endif

      #if HAS_TOUCH_BUTTONS
        static bool on_edit_screen;
        static void screen_click(const uint8_t row, const uint8_t col, const uint8_t x, const uint8_t y);
      #endif

      static void status_screen();

    #endif

    #if HAS_MARLINUI_U8GLIB
      static bool drawing_screen, first_page;
    #else
      static constexpr bool drawing_screen = false, first_page = true;
    #endif

    #if IS_DWIN_MARLINUI
      static bool did_first_redraw;
    #endif

    #if EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)
      static void zoffset_overlay(const int8_t dir);
      static void zoffset_overlay(const_float_t zvalue);
    #endif

    static void draw_kill_screen();

  #else // No LCD

    static inline void init() {}
    static inline void update() {}
    static inline void return_to_status() {}

  #endif

  #if ENABLED(SDSUPPORT)
    #if BOTH(SCROLL_LONG_FILENAMES, HAS_LCD_MENU)
      #define MARLINUI_SCROLL_NAME 1
    #endif
    #if MARLINUI_SCROLL_NAME
      static uint8_t filename_scroll_pos, filename_scroll_max;
    #endif
    static const char * scrolled_filename(CardReader &theCard, const uint8_t maxlen, uint8_t hash, const bool doScroll);
  #endif

  #if HAS_PREHEAT
    enum PreheatMask : uint8_t { PM_HOTEND = _BV(0), PM_BED = _BV(1), PM_FAN = _BV(2), PM_CHAMBER = _BV(3) };
    static preheat_t material_preset[PREHEAT_COUNT];
    static PGM_P get_preheat_label(const uint8_t m);
    static void apply_preheat(const uint8_t m, const uint8_t pmask, const uint8_t e=active_extruder);
    static inline void preheat_set_fan(const uint8_t m) { TERN_(HAS_FAN, apply_preheat(m, PM_FAN)); }
    static inline void preheat_hotend(const uint8_t m, const uint8_t e=active_extruder) { TERN_(HAS_HOTEND, apply_preheat(m, PM_HOTEND)); }
    static inline void preheat_hotend_and_fan(const uint8_t m, const uint8_t e=active_extruder) { preheat_hotend(m, e); preheat_set_fan(m); }
    static inline void preheat_bed(const uint8_t m) { TERN_(HAS_HEATED_BED, apply_preheat(m, PM_BED)); }
    static inline void preheat_all(const uint8_t m) { apply_preheat(m, 0xFF); }
  #endif

  #if SCREENS_CAN_TIME_OUT
    static inline void reset_status_timeout(const millis_t ms) { return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS; }
  #else
    static inline void reset_status_timeout(const millis_t) {}
  #endif

  #if HAS_LCD_MENU

    #if HAS_TOUCH_BUTTONS
      static uint8_t touch_buttons;
      static uint8_t repeat_delay;
    #else
      static constexpr uint8_t touch_buttons = 0;
    #endif

    #if ENABLED(ENCODER_RATE_MULTIPLIER)
      static bool encoderRateMultiplierEnabled;
      static millis_t lastEncoderMovementMillis;
      static void enable_encoder_multiplier(const bool onoff);
      #define ENCODER_RATE_MULTIPLY(F) (ui.encoderRateMultiplierEnabled = F)
    #else
      #define ENCODER_RATE_MULTIPLY(F) NOOP
    #endif

    // Manual Movement
    static ManualMove manual_move;

    // Select Screen (modal NO/YES style dialog)
    static bool selection;
    static void set_selection(const bool sel) { selection = sel; }
    static bool update_selection();

    static void synchronize(PGM_P const msg=nullptr);

    static screenFunc_t currentScreen;
    static bool screen_changed;
    static void goto_screen(const screenFunc_t screen, const uint16_t encoder=0, const uint8_t top=0, const uint8_t items=0);
    static void push_current_screen();

    // goto_previous_screen and go_back may also be used as menu item callbacks
    static void _goto_previous_screen(TERN_(TURBO_BACK_MENU_ITEM, const bool is_back));
    static inline void goto_previous_screen() { _goto_previous_screen(TERN_(TURBO_BACK_MENU_ITEM, false)); }
    static inline void go_back()              { _goto_previous_screen(TERN_(TURBO_BACK_MENU_ITEM, true)); }

    static void return_to_status();
    static inline bool on_status_screen() { return currentScreen == status_screen; }
    FORCE_INLINE static void run_current_screen() { (*currentScreen)(); }

    #if ENABLED(LIGHTWEIGHT_UI)
      static void lcd_in_status(const bool inStatus);
    #endif

    FORCE_INLINE static bool screen_is_sticky() {
      return TERN1(SCREENS_CAN_TIME_OUT, defer_return_to_status);
    }

    FORCE_INLINE static void defer_status_screen(const bool defer=true) {
      TERN(SCREENS_CAN_TIME_OUT, defer_return_to_status = defer, UNUSED(defer));
    }

    static inline void goto_previous_screen_no_defer() {
      defer_status_screen(false);
      goto_previous_screen();
    }

    #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
      static void reselect_last_file();
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      static void ubl_plot(const uint8_t x_plot, const uint8_t y_plot);
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      static void ubl_mesh_edit_start(const_float_t initial);
      static float ubl_mesh_value();
    #endif

    static void draw_select_screen_prompt(PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr);

  #else

    static constexpr bool on_status_screen() { return true; }

    #if HAS_WIRED_LCD
      FORCE_INLINE static void run_current_screen() { status_screen(); }
    #endif

  #endif

  #if EITHER(HAS_LCD_MENU, EXTENSIBLE_UI)
    static bool lcd_clicked;
    static inline bool use_click() {
      const bool click = lcd_clicked;
      lcd_clicked = false;
      return click;
    }
  #else
    static constexpr bool lcd_clicked = false;
    static inline bool use_click() { return false; }
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE) && ANY(HAS_LCD_MENU, EXTENSIBLE_UI, DWIN_CREALITY_LCD_ENHANCED, DWIN_CREALITY_LCD_JYERSUI)
    static void pause_show_message(const PauseMessage message, const PauseMode mode=PAUSE_MODE_SAME, const uint8_t extruder=active_extruder);
  #else
    static inline void _pause_show_message() {}
    #define pause_show_message(...) _pause_show_message()
  #endif

  //
  // EEPROM: Reset / Init / Load / Store
  //
  #if HAS_LCD_MENU
    static void reset_settings();
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    #if HAS_LCD_MENU
      static void init_eeprom();
      static void load_settings();
      static void store_settings();
    #endif
    #if DISABLED(EEPROM_AUTO_INIT)
      static void eeprom_alert(const uint8_t msgid);
      static inline void eeprom_alert_crc()     { eeprom_alert(0); }
      static inline void eeprom_alert_index()   { eeprom_alert(1); }
      static inline void eeprom_alert_version() { eeprom_alert(2); }
    #endif
  #endif

  //
  // Special handling if a move is underway
  //
  #if ANY(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION, PROBE_OFFSET_WIZARD) || (ENABLED(LCD_BED_LEVELING) && EITHER(PROBE_MANUALLY, MESH_BED_LEVELING))
    #define LCD_HAS_WAIT_FOR_MOVE 1
    static bool wait_for_move;
  #else
    static constexpr bool wait_for_move = false;
  #endif

  //
  // Block interaction while under external control
  //
  #if HAS_LCD_MENU && EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)
    static bool external_control;
    FORCE_INLINE static void capture() { external_control = true; }
    FORCE_INLINE static void release() { external_control = false; }
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      static void external_encoder();
    #endif
  #else
    static constexpr bool external_control = false;
  #endif

  #if HAS_ENCODER_ACTION

    static volatile uint8_t buttons;
    #if IS_RRW_KEYPAD
      static volatile uint8_t keypad_buttons;
      static bool handle_keypad();
    #endif
    #if HAS_SLOW_BUTTONS
      static volatile uint8_t slow_buttons;
      static uint8_t read_slow_buttons();
    #endif

    static void update_buttons();
    static inline bool button_pressed() { return BUTTON_CLICK() || TERN(TOUCH_SCREEN, touch_pressed(), false); }
    #if EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)
      static void wait_for_release();
    #endif

    static uint32_t encoderPosition;

    #define ENCODERBASE (TERN(REVERSE_ENCODER_DIRECTION, -1, +1))

    #if EITHER(REVERSE_MENU_DIRECTION, REVERSE_SELECT_DIRECTION)
      static int8_t encoderDirection;
    #else
      static constexpr int8_t encoderDirection = ENCODERBASE;
    #endif

    FORCE_INLINE static void encoder_direction_normal() {
      #if EITHER(REVERSE_MENU_DIRECTION, REVERSE_SELECT_DIRECTION)
        encoderDirection = ENCODERBASE;
      #endif
    }

    FORCE_INLINE static void encoder_direction_menus() {
      TERN_(REVERSE_MENU_DIRECTION, encoderDirection = -(ENCODERBASE));
    }

    FORCE_INLINE static void encoder_direction_select() {
      TERN_(REVERSE_SELECT_DIRECTION, encoderDirection = -(ENCODERBASE));
    }

  #else

    static inline void update_buttons() {}

  #endif

  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    static void touch_calibration_screen();
  #endif

  #if HAS_GRAPHICAL_TFT
    static void move_axis_screen();
  #endif

private:

  #if SCREENS_CAN_TIME_OUT
    static millis_t return_to_status_ms;
    static bool defer_return_to_status;
  #else
    static constexpr bool defer_return_to_status = false;
  #endif

  #if HAS_STATUS_MESSAGE
    static void finish_status(const bool persist);
  #endif

  #if HAS_WIRED_LCD
    static void draw_status_screen();
    #if HAS_GRAPHICAL_TFT
      static void tft_idle();
      #if ENABLED(TOUCH_SCREEN)
        static bool touch_pressed();
      #endif
    #endif
  #endif
};

extern MarlinUI ui;

#define LCD_MESSAGE_F(S)       ui.set_status(F(S))
#define LCD_MESSAGE(M)         ui.set_status(GET_TEXT_F(M))
#define LCD_ALERTMESSAGE_F(S)  ui.set_alert_status(F(S))
#define LCD_ALERTMESSAGE(M)    ui.set_alert_status(GET_TEXT_F(M))
