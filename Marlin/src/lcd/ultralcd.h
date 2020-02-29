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
#pragma once

#include "../inc/MarlinConfig.h"

#if HAS_BUZZER
  #include "../libs/buzzer.h"
#endif

#define HAS_ENCODER_ACTION (HAS_LCD_MENU || ENABLED(ULTIPANEL_FEEDMULTIPLY))
#define HAS_ENCODER_WHEEL  ((!HAS_ADC_BUTTONS && ENABLED(NEWPANEL)) || BUTTONS_EXIST(EN1, EN2))
#define HAS_DIGITAL_BUTTONS (HAS_ENCODER_WHEEL || ANY_BUTTON(ENC, BACK, UP, DWN, LFT, RT))
#define HAS_SHIFT_ENCODER   (!HAS_ADC_BUTTONS && (ENABLED(REPRAPWORLD_KEYPAD) || (HAS_SPI_LCD && DISABLED(NEWPANEL))))

// I2C buttons must be read in the main thread
#define HAS_SLOW_BUTTONS EITHER(LCD_I2C_VIKI, LCD_I2C_PANELOLU2)

#if HAS_SPI_LCD

  #include "../MarlinCore.h"

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #include "../feature/pause.h"
    #include "../module/motion.h" // for active_extruder
  #endif

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

  #if ENABLED(TOUCH_BUTTONS)
    #define LCD_UPDATE_INTERVAL 50
  #else
    #define LCD_UPDATE_INTERVAL 100
  #endif

  #if HAS_LCD_MENU

    #if HAS_GRAPHICAL_LCD
      #define SETCURSOR(col, row) lcd_moveto(col * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
      #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_PIXEL_WIDTH - (len) * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
    #else
      #define SETCURSOR(col, row) lcd_moveto(col, row)
      #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_WIDTH - (len), row)
    #endif

    #include "lcdprint.h"

    void _wrap_string(uint8_t &col, uint8_t &row, const char * const string, read_byte_cb_t cb_read_byte, const bool wordwrap=false);
    inline void wrap_string_P(uint8_t &col, uint8_t &row, PGM_P const pstr, const bool wordwrap=false) { _wrap_string(col, row, pstr, read_byte_rom, wordwrap); }
    inline void wrap_string(uint8_t &col, uint8_t &row, const char * const string, const bool wordwrap=false) { _wrap_string(col, row, string, read_byte_ram, wordwrap); }

    #if ENABLED(SDSUPPORT)
      #include "../sd/cardreader.h"
    #endif

    typedef void (*screenFunc_t)();
    typedef void (*menuAction_t)();

    // Manual Movement
    extern float move_menu_scale;

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      void lcd_pause_show_message(const PauseMessage message,
                                  const PauseMode mode=PAUSE_MODE_SAME,
                                  const uint8_t extruder=active_extruder);
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      void lcd_mesh_edit_setup(const float &initial);
      float lcd_mesh_edit();
    #endif

  #endif // HAS_LCD_MENU

#endif

// REPRAPWORLD_KEYPAD (and ADC_KEYPAD)
#if ENABLED(REPRAPWORLD_KEYPAD)
  #define BTN_OFFSET          0 // Bit offset into buttons for shift register values

  #define BLEN_KEYPAD_F3      0
  #define BLEN_KEYPAD_F2      1
  #define BLEN_KEYPAD_F1      2
  #define BLEN_KEYPAD_DOWN    3
  #define BLEN_KEYPAD_RIGHT   4
  #define BLEN_KEYPAD_MIDDLE  5
  #define BLEN_KEYPAD_UP      6
  #define BLEN_KEYPAD_LEFT    7

  #define EN_KEYPAD_F1      _BV(BTN_OFFSET + BLEN_KEYPAD_F1)
  #define EN_KEYPAD_F2      _BV(BTN_OFFSET + BLEN_KEYPAD_F2)
  #define EN_KEYPAD_F3      _BV(BTN_OFFSET + BLEN_KEYPAD_F3)
  #define EN_KEYPAD_DOWN    _BV(BTN_OFFSET + BLEN_KEYPAD_DOWN)
  #define EN_KEYPAD_RIGHT   _BV(BTN_OFFSET + BLEN_KEYPAD_RIGHT)
  #define EN_KEYPAD_MIDDLE  _BV(BTN_OFFSET + BLEN_KEYPAD_MIDDLE)
  #define EN_KEYPAD_UP      _BV(BTN_OFFSET + BLEN_KEYPAD_UP)
  #define EN_KEYPAD_LEFT    _BV(BTN_OFFSET + BLEN_KEYPAD_LEFT)

  #define RRK(B) (keypad_buttons & (B))

  #ifdef EN_C
    #define BUTTON_CLICK() ((buttons & EN_C) || RRK(EN_KEYPAD_MIDDLE))
  #else
    #define BUTTON_CLICK() RRK(EN_KEYPAD_MIDDLE)
  #endif

#endif

#if HAS_DIGITAL_BUTTONS

  // Wheel spin pins where BA is 00, 10, 11, 01 (1 bit always changes)
  #define BLEN_A 0
  #define BLEN_B 1

  #define EN_A _BV(BLEN_A)
  #define EN_B _BV(BLEN_B)

  #define BUTTON_PRESSED(BN) !READ(BTN_## BN)

  #if BUTTON_EXISTS(ENC) || ENABLED(TOUCH_BUTTONS)
    #define BLEN_C 2
    #define EN_C _BV(BLEN_C)
  #endif

  #if ENABLED(LCD_I2C_VIKI)

    #include <LiquidTWI2.h>

    #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

    // button and encoder bit positions within 'buttons'
    #define B_LE (BUTTON_LEFT   << B_I2C_BTN_OFFSET)      // The remaining normalized buttons are all read via I2C
    #define B_UP (BUTTON_UP     << B_I2C_BTN_OFFSET)
    #define B_MI (BUTTON_SELECT << B_I2C_BTN_OFFSET)
    #define B_DW (BUTTON_DOWN   << B_I2C_BTN_OFFSET)
    #define B_RI (BUTTON_RIGHT  << B_I2C_BTN_OFFSET)

    #if BUTTON_EXISTS(ENC)                                // The pause/stop/restart button is connected to BTN_ENC when used
      #define B_ST (EN_C)                                 // Map the pause/stop/resume button into its normalized functional name
      #define BUTTON_CLICK() (buttons & (B_MI|B_RI|B_ST)) // Pause/stop also acts as click until a proper pause/stop is implemented.
    #else
      #define BUTTON_CLICK() (buttons & (B_MI|B_RI))
    #endif

    // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update

  #elif ENABLED(LCD_I2C_PANELOLU2)

    #if !BUTTON_EXISTS(ENC) // Use I2C if not directly connected to a pin

      #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

      #define B_MI (PANELOLU2_ENCODER_C << B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

      #define BUTTON_CLICK() (buttons & B_MI)

    #endif

  #endif

#else

  #undef BUTTON_EXISTS
  #define BUTTON_EXISTS(...) false

  // Shift register bits correspond to buttons:
  #define BL_LE 7   // Left
  #define BL_UP 6   // Up
  #define BL_MI 5   // Middle
  #define BL_DW 4   // Down
  #define BL_RI 3   // Right
  #define BL_ST 2   // Red Button
  #define B_LE (_BV(BL_LE))
  #define B_UP (_BV(BL_UP))
  #define B_MI (_BV(BL_MI))
  #define B_DW (_BV(BL_DW))
  #define B_RI (_BV(BL_RI))
  #define B_ST (_BV(BL_ST))

  #ifndef BUTTON_CLICK
    #define BUTTON_CLICK() (buttons & (B_MI|B_ST))
  #endif

#endif

#if BUTTON_EXISTS(BACK) || ENABLED(TOUCH_BUTTONS)
  #define BLEN_D 3
  #define EN_D _BV(BLEN_D)
  #define LCD_BACK_CLICKED() (buttons & EN_D)
#else
  #define LCD_BACK_CLICKED() false
#endif

#ifndef BUTTON_CLICK
  #ifdef EN_C
    #define BUTTON_CLICK() (buttons & EN_C)
  #else
    #define BUTTON_CLICK() false
  #endif
#endif

#if HAS_GRAPHICAL_LCD
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

////////////////////////////////////////////
//////////// MarlinUI Singleton ////////////
////////////////////////////////////////////

class MarlinUI {
public:

  MarlinUI() {
    #if HAS_LCD_MENU
      currentScreen = status_screen;
    #endif
  }

  #if HAS_BUZZER
    static void buzz(const long duration, const uint16_t freq);
  #endif

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)
    static void update_indicators();
  #endif

  // LCD implementations
  static void clear_lcd();
  static void init_lcd();

  #if HAS_DISPLAY

    static void init();
    static void update();
    static void set_alert_status_P(PGM_P const message);

    static char status_message[];
    static bool has_status();

    static uint8_t alert_level; // Higher levels block lower levels
    static inline void reset_alert_level() { alert_level = 0; }

    #if ENABLED(STATUS_MESSAGE_SCROLLING)
      static uint8_t status_scroll_offset;
      static void advance_status_scroll();
      static char* status_and_len(uint8_t &len);
    #endif

    static void abort_print();
    static void pause_print();
    static void resume_print();

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
        #if BOTH(LCD_SET_PROGRESS_MANUALLY, USE_M73_REMAINING_TIME)
          static uint32_t remaining_time;
          FORCE_INLINE static void set_remaining_time(const uint32_t r) { remaining_time = r; }
          FORCE_INLINE static uint32_t get_remaining_time() { return remaining_time; }
          FORCE_INLINE static void reset_remaining_time() { set_remaining_time(0); }
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

    #if HAS_SPI_LCD

      static millis_t next_button_update_ms;

      static bool detected();

      static LCDViewAction lcdDrawUpdate;
      FORCE_INLINE static bool should_draw() { return bool(lcdDrawUpdate); }
      FORCE_INLINE static void refresh(const LCDViewAction type) { lcdDrawUpdate = type; }
      FORCE_INLINE static void refresh() { refresh(LCDVIEW_CLEAR_CALL_REDRAW); }

      #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
        static void draw_custom_bootscreen(const uint8_t frame=0);
        static void show_custom_bootscreen();
      #endif

      #if ENABLED(SHOW_BOOTSCREEN)
        static void draw_marlin_bootscreen(const bool line2=false);
        static void show_marlin_bootscreen();
        static void show_bootscreen();
      #endif

      #if HAS_GRAPHICAL_LCD

        static bool drawing_screen, first_page;

        static void set_font(const MarlinFont font_nr);

      #else

        static constexpr bool drawing_screen = false, first_page = true;

        static void set_custom_characters(const HD44780CharSet screen_charset=CHARSET_INFO);

        #if ENABLED(LCD_PROGRESS_BAR)
          static millis_t progress_bar_ms;  // Start time for the current progress bar cycle
          static void draw_progress_bar(const uint8_t percent);
          #if PROGRESS_MSG_EXPIRE > 0
            static millis_t expire_status_ms; // = 0
            FORCE_INLINE static void reset_progress_bar_timeout() { expire_status_ms = 0; }
          #endif
        #endif

      #endif

      static uint8_t lcd_status_update_delay;

      #if HAS_LCD_CONTRAST
        static int16_t contrast;
        static void set_contrast(const int16_t value);
        FORCE_INLINE static void refresh_contrast() { set_contrast(contrast); }
      #endif

      #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
        static millis_t next_filament_display;
      #endif

      static void quick_feedback(const bool clear_buttons=true);
      #if HAS_BUZZER
        static void completion_feedback(const bool good=true);
      #endif

      #if DISABLED(LIGHTWEIGHT_UI)
        static void draw_status_message(const bool blink);
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        static void draw_hotend_status(const uint8_t row, const uint8_t extruder);
      #endif

      static void status_screen();

    #else
      static void refresh() {}
    #endif

    static bool get_blink();
    static void kill_screen(PGM_P const lcd_error, PGM_P const lcd_component);
    static void draw_kill_screen();
    static void set_status(const char* const message, const bool persist=false);
    static void set_status_P(PGM_P const message, const int8_t level=0);
    static void status_printf_P(const uint8_t level, PGM_P const fmt, ...);
    static void reset_status(const bool no_welcome=false);

  #else // No LCD

    // Send status to host as a notification
    void set_status(const char* message, const bool=false);
    void set_status_P(PGM_P message, const int8_t=0);
    void status_printf_P(const uint8_t, PGM_P message, ...);

    static inline void init() {}
    static inline void update() {}
    static inline void refresh() {}
    static inline void return_to_status() {}
    static inline void set_alert_status_P(PGM_P const) {}
    static inline void reset_status(const bool=false) {}
    static inline void reset_alert_level() {}
    static constexpr bool has_status() { return false; }

  #endif

  #if HAS_LCD_MENU

    #if ENABLED(TOUCH_BUTTONS)
      static uint8_t touch_buttons;
      static uint8_t repeat_delay;
    #endif

    #if ENABLED(ENCODER_RATE_MULTIPLIER)
      static bool encoderRateMultiplierEnabled;
      static millis_t lastEncoderMovementMillis;
      static void enable_encoder_multiplier(const bool onoff);
    #endif

    #if ENABLED(SDSUPPORT)
      #if ENABLED(SCROLL_LONG_FILENAMES)
        static uint8_t filename_scroll_pos, filename_scroll_max;
      #endif
      static const char * scrolled_filename(CardReader &theCard, const uint8_t maxlen, uint8_t hash, const bool doScroll);
    #endif

    #if IS_KINEMATIC
      static bool processing_manual_move;
    #else
      static constexpr bool processing_manual_move = false;
    #endif

    #if E_MANUAL > 1
      static int8_t manual_move_e_index;
    #else
      static constexpr int8_t manual_move_e_index = 0;
    #endif

    static int16_t preheat_hotend_temp[2], preheat_bed_temp[2];
    static uint8_t preheat_fan_speed[2];

    // Select Screen (modal NO/YES style dialog)
    static bool selection;
    static void set_selection(const bool sel) { selection = sel; }
    static bool update_selection();

    static void manage_manual_move();

    static bool lcd_clicked;
    static bool use_click();

    static void synchronize(PGM_P const msg=nullptr);

    static screenFunc_t currentScreen;
    static bool screen_changed;
    static void goto_screen(const screenFunc_t screen, const uint16_t encoder=0, const uint8_t top=0, const uint8_t items=0);
    static void save_previous_screen();

    // goto_previous_screen and go_back may also be used as menu item callbacks
    #if ENABLED(TURBO_BACK_MENU_ITEM)
      static void _goto_previous_screen(const bool is_back);
      static inline void goto_previous_screen() { _goto_previous_screen(false); }
      static inline void go_back()              { _goto_previous_screen(true); }
    #else
      static void _goto_previous_screen();
      FORCE_INLINE static void goto_previous_screen() { _goto_previous_screen(); }
      FORCE_INLINE static void go_back()              { _goto_previous_screen(); }
    #endif

    static void return_to_status();
    static inline bool on_status_screen() { return currentScreen == status_screen; }
    FORCE_INLINE static void run_current_screen() { (*currentScreen)(); }

    #if ENABLED(LIGHTWEIGHT_UI)
      static void lcd_in_status(const bool inStatus);
    #endif

    FORCE_INLINE static void defer_status_screen(const bool defer=true) {
      #if LCD_TIMEOUT_TO_STATUS
        defer_return_to_status = defer;
      #else
        UNUSED(defer);
      #endif
    }

    static inline void goto_previous_screen_no_defer() {
      defer_status_screen(false);
      goto_previous_screen();
    }

    #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
      static void reselect_last_file();
    #endif

    #if ENABLED(G26_MESH_VALIDATION)
      FORCE_INLINE static void chirp() {
        #if HAS_BUZZER
          buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);
        #endif
      }
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      static void ubl_plot(const uint8_t x_plot, const uint8_t y_plot);
    #endif

    static void draw_select_screen_prompt(PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr);

  #elif HAS_SPI_LCD

    static constexpr bool lcd_clicked = false;
    static constexpr bool on_status_screen() { return true; }
    FORCE_INLINE static void run_current_screen() { status_screen(); }

  #endif

  #define LCD_HAS_WAIT_FOR_MOVE EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION) || (ENABLED(LCD_BED_LEVELING) && EITHER(PROBE_MANUALLY, MESH_BED_LEVELING))

  #if LCD_HAS_WAIT_FOR_MOVE
    static bool wait_for_move;
  #else
    static constexpr bool wait_for_move = false;
  #endif

  #if HAS_LCD_MENU && EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)
    static bool external_control;
    FORCE_INLINE static void capture() { external_control = true; }
    FORCE_INLINE static void release() { external_control = false; }
  #else
    static constexpr bool external_control = false;
  #endif

  #if HAS_ENCODER_ACTION

    static volatile uint8_t buttons;
    #if ENABLED(REPRAPWORLD_KEYPAD)
      static volatile uint8_t keypad_buttons;
      static bool handle_keypad();
    #endif
    #if HAS_SLOW_BUTTONS
      static volatile uint8_t slow_buttons;
      static uint8_t read_slow_buttons();
    #endif

    static void update_buttons();
    static inline bool button_pressed() { return BUTTON_CLICK(); }
    #if EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)
      static void wait_for_release();
    #endif

    static uint32_t encoderPosition;

    #if ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODERBASE -1
    #else
      #define ENCODERBASE +1
    #endif

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
      #if ENABLED(REVERSE_MENU_DIRECTION)
        encoderDirection = -(ENCODERBASE);
      #endif
    }

    FORCE_INLINE static void encoder_direction_select() {
      #if ENABLED(REVERSE_SELECT_DIRECTION)
        encoderDirection = -(ENCODERBASE);
      #endif
    }

  #else

    static inline void update_buttons() {}

  #endif

private:

  static void _synchronize();

  #if HAS_DISPLAY
    static void finish_status(const bool persist);
  #endif

  #if HAS_SPI_LCD
    #if HAS_LCD_MENU
      #if LCD_TIMEOUT_TO_STATUS
        static bool defer_return_to_status;
      #else
        static constexpr bool defer_return_to_status = false;
      #endif
    #endif
    static void draw_status_screen();
  #endif
};

extern MarlinUI ui;

#define LCD_MESSAGEPGM_P(x)      ui.set_status_P(x)
#define LCD_ALERTMESSAGEPGM_P(x) ui.set_alert_status_P(x)

#define LCD_MESSAGEPGM(x)        LCD_MESSAGEPGM_P(GET_TEXT(x))
#define LCD_ALERTMESSAGEPGM(x)   LCD_ALERTMESSAGEPGM_P(GET_TEXT(x))
