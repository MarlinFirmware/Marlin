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
#pragma once

#include "../inc/MarlinConfig.h"

#if HAS_SPI_LCD

  #include "../Marlin.h"

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #include "../feature/pause.h"
    #include "../module/motion.h" // for active_extruder
  #endif

#endif

#if HAS_BUZZER
  #include "../libs/buzzer.h"
#endif

#if HAS_GRAPHICAL_LCD

  #ifndef LCD_PIXEL_WIDTH
    #define LCD_PIXEL_WIDTH 128
  #endif
  #ifndef LCD_PIXEL_HEIGHT
    #define LCD_PIXEL_HEIGHT 64
  #endif

  // LCD selection
  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    #define U8G_CLASS U8GLIB_ST7920_128X64_4X
    #if DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
      #define U8G_PARAM LCD_PINS_RS
    #else
      #define U8G_PARAM LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS
    #endif

  #elif ENABLED(U8GLIB_ST7920)
    // RepRap Discount Full Graphics Smart Controller
    #if DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
      #define U8G_CLASS U8GLIB_ST7920_128X64_4X_HAL
      #define U8G_PARAM LCD_PINS_RS // 2 stripes, HW SPI (shared with SD card, on AVR does not use standard LCD adapter)
    #else
      //#define U8G_CLASS U8GLIB_ST7920_128X64_4X
      //#define U8G_PARAM LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS     // Original u8glib device. 2 stripes, SW SPI
      #define U8G_CLASS U8GLIB_ST7920_128X64_RRD
      #define U8G_PARAM LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS       // Number of stripes can be adjusted in ultralcd_st7920_u8glib_rrd.h with PAGE_HEIGHT
                                                                        // AVR version ignores these pin settings
                                                                        // HAL version uses these pin settings
    #endif

  #elif ENABLED(CARTESIO_UI)
    // The CartesioUI display
    //#define U8G_CLASS U8GLIB_DOGM128_2X
    //#define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0   // 4 stripes
    #define U8G_CLASS U8GLIB_DOGM128_2X
    #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                              // 4 stripes

  #elif ENABLED(U8GLIB_LM6059_AF)
    // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
    //#define U8G_CLASS U8GLIB_LM6059
    //#define U8G_PARAM DOGLCD_CS, DOGLCD_A0                            // 8 stripes
    #define U8G_CLASS U8GLIB_LM6059_2X
    #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                              // 4 stripes

  #elif ENABLED(U8GLIB_ST7565_64128N)
    // The MaKrPanel, Mini Viki, Viki 2.0 & AZSMZ 12864 ST7565 controller
    #define SMART_RAMPS (MB(RAMPS_SMART_EFB) || MB(RAMPS_SMART_EEB) || MB(RAMPS_SMART_EFF) || MB(RAMPS_SMART_EEF) || MB(RAMPS_SMART_SF))
    #if DOGLCD_SCK == SCK_PIN && DOGLCD_MOSI == MOSI_PIN && !SMART_RAMPS
      #define U8G_CLASS U8GLIB_64128N_2X_HAL
      #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                            // using HW-SPI
    #else
      #define U8G_CLASS U8GLIB_64128N_2X_HAL
      #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0   // using SW-SPI
    #endif

  #elif ENABLED(MKS_12864OLED_SSD1306)
    // MKS 128x64 (SSD1306) OLED I2C LCD
    #define U8G_CLASS U8GLIB_SSD1306_128X64
    #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0     // 8 stripes
    //#define U8G_CLASS U8GLIB_SSD1306_128X64_2X
    //#define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0   // 4 stripes

  #elif ENABLED(U8GLIB_SSD1306)
    // Generic support for SSD1306 OLED I2C LCDs
    //#define U8G_CLASS U8GLIB_SSD1306_128X64_2X_I2C_2_WIRE
    //#define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)           // 4 stripes
    #define U8G_CLASS U8GLIB_SSD1306_128X64_2X
    #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)             // 4 stripes

  #elif ENABLED(MKS_12864OLED)
    // MKS 128x64 (SH1106) OLED I2C LCD
    #define U8G_CLASS U8GLIB_SH1106_128X64
    #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0     // 8 stripes
    //#define U8G_CLASS U8GLIB_SH1106_128X64_2X
    //#define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0   // 4 stripes
  #elif ENABLED(U8GLIB_SH1106)
    // Generic support for SH1106 OLED I2C LCDs
    //#define U8G_CLASS U8GLIB_SH1106_128X64_2X_I2C_2_WIRE
    //#define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)           // 4 stripes
    #define U8G_CLASS U8GLIB_SH1106_128X64_2X
    #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)             // 4 stripes
  #elif ENABLED(U8GLIB_SSD1309)
    // Generic support for SSD1309 OLED I2C LCDs
    #define U8G_CLASS U8GLIB_SSD1309_128X64
    #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)
  #elif ENABLED(MINIPANEL)
    // The MINIPanel display
    //#define U8G_CLASS U8GLIB_MINI12864
    //#define U8G_PARAM DOGLCD_CS, DOGLCD_A0                            // 8 stripes
    #define U8G_CLASS U8GLIB_MINI12864_2X
    #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                              // 4 stripes
  #elif ENABLED(U8GLIB_SH1106_EINSTART)
    // Connected via motherboard header
    #define U8G_CLASS U8GLIB_SH1106_128X64
    #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, LCD_PINS_DC, LCD_PINS_RS
  #else
    // for regular DOGM128 display with HW-SPI
    //#define U8G_CLASS U8GLIB_DOGM128
    //#define U8G_PARAM DOGLCD_CS, DOGLCD_A0                            // HW-SPI Com: CS, A0  // 8 stripes
    #define U8G_CLASS U8GLIB_DOGM128_2X
    #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                              // HW-SPI Com: CS, A0 // 4 stripes
  #endif

  #include <U8glib.h>
  #include "dogm/HAL_LCD_class_defines.h"
  extern U8G_CLASS u8g;

  // For selective rendering within a Y range
  #define PAGE_UNDER(yb) (u8g.getU8g()->current_page.y0 <= (yb))
  #define PAGE_CONTAINS(ya, yb) (PAGE_UNDER(yb) && u8g.getU8g()->current_page.y1 >= (ya))

  // Only Western languages support big / small fonts
  #if DISABLED(DISPLAY_CHARSET_ISO10646_1)
    #undef USE_BIG_EDIT_FONT
    #undef USE_SMALL_INFOFONT
  #endif

  #define MENU_FONT_NAME    ISO10646_1_5x7
  #define MENU_FONT_WIDTH    6
  #define MENU_FONT_ASCENT  10
  #define MENU_FONT_DESCENT  2
  #define MENU_FONT_HEIGHT  (MENU_FONT_ASCENT + MENU_FONT_DESCENT)

  #if ENABLED(USE_BIG_EDIT_FONT)
    #define EDIT_FONT_NAME    u8g_font_9x18
    #define EDIT_FONT_WIDTH    9
    #define EDIT_FONT_ASCENT  10
    #define EDIT_FONT_DESCENT  3
  #else
    #define EDIT_FONT_NAME    MENU_FONT_NAME
    #define EDIT_FONT_WIDTH   MENU_FONT_WIDTH
    #define EDIT_FONT_ASCENT  MENU_FONT_ASCENT
    #define EDIT_FONT_DESCENT MENU_FONT_DESCENT
  #endif
  #define EDIT_FONT_HEIGHT (EDIT_FONT_ASCENT + EDIT_FONT_DESCENT)

  // Get the Ascent, Descent, and total Height for the Info Screen font
  #if ENABLED(USE_SMALL_INFOFONT)
    extern const u8g_fntpgm_uint8_t u8g_font_6x9[];
    #define INFO_FONT_ASCENT 7
  #else
    #define INFO_FONT_ASCENT 8
  #endif
  #define INFO_FONT_DESCENT 2
  #define INFO_FONT_HEIGHT (INFO_FONT_ASCENT + INFO_FONT_DESCENT)

  enum MarlinFont : uint8_t {
    FONT_STATUSMENU = 1,
    FONT_EDIT,
    FONT_MENU
  };

  #if ENABLED(LIGHTWEIGHT_UI)
    void lcd_in_status(const bool inStatus);
  #endif

#endif // HAS_GRAPHICAL_LCD

#define HAS_ENCODER_ACTION (HAS_LCD_MENU || ENABLED(ULTIPANEL_FEEDMULTIPLY))

#if HAS_SPI_LCD

  enum LCDViewAction : uint8_t {
    LCDVIEW_NONE,
    LCDVIEW_REDRAW_NOW,
    LCDVIEW_CALL_REDRAW_NEXT,
    LCDVIEW_CLEAR_CALL_REDRAW,
    LCDVIEW_CALL_NO_REDRAW
  };

  #if ENABLED(ADC_KEYPAD)
    uint8_t get_ADC_keyValue();
  #endif

  #if HAS_GRAPHICAL_LCD
    #define SETCURSOR(col, row) lcd_moveto(col * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
    #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_PIXEL_WIDTH - len * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
  #else
    #define SETCURSOR(col, row) lcd_moveto(col, row)
    #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_WIDTH - len, row)
  #endif

  #define LCD_UPDATE_INTERVAL 100
  #define BUTTON_EXISTS(BN) (defined(BTN_## BN) && BTN_## BN >= 0)
  #define BUTTON_PRESSED(BN) !READ(BTN_## BN)

  #if HAS_LCD_MENU

    typedef void (*screenFunc_t)();
    typedef void (*menuAction_t)();

    // Manual Movement
    constexpr float manual_feedrate_mm_m[XYZE] = MANUAL_FEEDRATE;
    extern float move_menu_scale;

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      void lcd_advanced_pause_show_message(const AdvancedPauseMessage message,
                                           const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_SAME,
                                           const uint8_t extruder=active_extruder);
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      void lcd_mesh_edit_setup(const float &initial);
      float lcd_mesh_edit();
    #endif

  #endif // HAS_LCD_MENU

#endif

#if HAS_DIGITAL_ENCODER

  // Wheel spin pins where BA is 00, 10, 11, 01 (1 bit always changes)
  #define BLEN_A 0
  #define BLEN_B 1

  #define EN_A _BV(BLEN_A)
  #define EN_B _BV(BLEN_B)

  #if BUTTON_EXISTS(ENC)
    #define BLEN_C 2
    #define EN_C _BV(BLEN_C)
  #endif

  #if BUTTON_EXISTS(BACK)
    #define BLEN_D 3
    #define EN_D _BV(BLEN_D)
    #define LCD_BACK_CLICKED() (buttons & EN_D)
  #endif

  #if ENABLED(REPRAPWORLD_KEYPAD)
    #define REPRAPWORLD_BTN_OFFSET          0 // Bit offset into buttons for shift register values

    #define BLEN_REPRAPWORLD_KEYPAD_F3      0
    #define BLEN_REPRAPWORLD_KEYPAD_F2      1
    #define BLEN_REPRAPWORLD_KEYPAD_F1      2
    #define BLEN_REPRAPWORLD_KEYPAD_DOWN    3
    #define BLEN_REPRAPWORLD_KEYPAD_RIGHT   4
    #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE  5
    #define BLEN_REPRAPWORLD_KEYPAD_UP      6
    #define BLEN_REPRAPWORLD_KEYPAD_LEFT    7

    #define EN_REPRAPWORLD_KEYPAD_F1        (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F1))
    #define EN_REPRAPWORLD_KEYPAD_F2        (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F2))
    #define EN_REPRAPWORLD_KEYPAD_F3        (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F3))
    #define EN_REPRAPWORLD_KEYPAD_DOWN      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_DOWN))
    #define EN_REPRAPWORLD_KEYPAD_RIGHT     (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_RIGHT))
    #define EN_REPRAPWORLD_KEYPAD_MIDDLE    (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_MIDDLE))
    #define EN_REPRAPWORLD_KEYPAD_UP        (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_UP))
    #define EN_REPRAPWORLD_KEYPAD_LEFT      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_LEFT))

    #define RRK(B) (buttons_reprapworld_keypad & (B))

    #ifdef EN_C
      #define BUTTON_CLICK() ((buttons & EN_C) || RRK(EN_REPRAPWORLD_KEYPAD_MIDDLE))
    #else
      #define BUTTON_CLICK() RRK(EN_REPRAPWORLD_KEYPAD_MIDDLE)
    #endif

  #elif ENABLED(LCD_I2C_VIKI)

    #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

    // button and encoder bit positions within 'buttons'
    #define B_LE (BUTTON_LEFT   << B_I2C_BTN_OFFSET)    // The remaining normalized buttons are all read via I2C
    #define B_UP (BUTTON_UP     << B_I2C_BTN_OFFSET)
    #define B_MI (BUTTON_SELECT << B_I2C_BTN_OFFSET)
    #define B_DW (BUTTON_DOWN   << B_I2C_BTN_OFFSET)
    #define B_RI (BUTTON_RIGHT  << B_I2C_BTN_OFFSET)

    #if BUTTON_EXISTS(ENC)                                // The pause/stop/restart button is connected to BTN_ENC when used
      #define B_ST (EN_C)                                 // Map the pause/stop/resume button into its normalized functional name
      #define BUTTON_CLICK() (buttons & (B_MI|B_RI|B_ST))  // Pause/stop also acts as click until a proper pause/stop is implemented.
    #else
      #define BUTTON_CLICK() (buttons & (B_MI|B_RI))
    #endif

    // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
    #define LCD_HAS_SLOW_BUTTONS

  #elif ENABLED(LCD_I2C_PANELOLU2)

    #if !BUTTON_EXISTS(ENC) // Use I2C if not directly connected to a pin

      #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

      #define B_MI (PANELOLU2_ENCODER_C << B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

      #define BUTTON_CLICK() (buttons & B_MI)

      // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
      #define LCD_HAS_SLOW_BUTTONS

    #endif

  #endif

#else

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
  #define BUTTON_CLICK() (buttons & (B_MI|B_ST))

#endif

#ifndef BUTTON_CLICK
  #ifdef EN_C
    #define BUTTON_CLICK() (buttons & EN_C)
  #else
    #define BUTTON_CLICK() false
  #endif
#endif

#define LCD_MESSAGEPGM(x)      ui.setstatusPGM(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) ui.setalertstatusPGM(PSTR(x))

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

  static inline void buzz(const long duration, const uint16_t freq) {
    #if ENABLED(LCD_USE_I2C_BUZZER)
      lcd.buzz(duration, freq);
    #elif PIN_EXISTS(BEEPER)
      buzzer.tone(duration, freq);
    #else
      UNUSED(duration); UNUSED(freq);
    #endif
  }

  // LCD implementations
  static void clear_lcd();
  static void init_lcd();

  #if HAS_SPI_LCD || ENABLED(MALYAN_LCD) || ENABLED(EXTENSIBLE_UI)
    static void init();
    static void update();
    static bool detected();
    static void setalertstatusPGM(PGM_P message);
  #else // NO LCD
    static inline void init() {}
    static inline void update() {}
    static constexpr bool detected() { return true; }
    static inline void setalertstatusPGM(PGM_P message) { UNUSED(message); }
  #endif

  #if HAS_SPI_LCD || ENABLED(EXTENSIBLE_UI)

    #if HAS_SPI_LCD

      static LCDViewAction lcdDrawUpdate;
      static inline bool should_draw() { return bool(lcdDrawUpdate); }
      static inline void refresh(const LCDViewAction type) { lcdDrawUpdate = type; }
      static inline void refresh() { refresh(LCDVIEW_CLEAR_CALL_REDRAW); }

      #if ENABLED(SHOW_BOOTSCREEN)
        static void show_bootscreen();
      #endif

      #if HAS_GRAPHICAL_LCD

        static bool drawing_screen, first_page;

        static void set_font(const MarlinFont font_nr);

      #else

        static constexpr bool drawing_screen = false, first_page = true;

        enum HD44780CharSet : uint8_t { CHARSET_MENU, CHARSET_INFO, CHARSET_BOOT };

        static void set_custom_characters(
          #if ENABLED(LCD_PROGRESS_BAR) || ENABLED(SHOW_BOOTSCREEN)
            const HD44780CharSet screen_charset=CHARSET_INFO
          #endif
        );

        #if ENABLED(LCD_PROGRESS_BAR)
          static millis_t progress_bar_ms;  // Start time for the current progress bar cycle
          #if PROGRESS_MSG_EXPIRE > 0
            static millis_t MarlinUI::expire_status_ms; // = 0
            static inline void reset_progress_bar_timeout() { expire_status_ms = 0; }
          #endif
          #define LCD_SET_CHARSET(C) set_custom_characters(C)
        #else
          #define LCD_SET_CHARSET(C) set_custom_characters()
        #endif

      #endif

      // Status message
      static char status_message[];
      #if ENABLED(STATUS_MESSAGE_SCROLLING)
        static uint8_t status_scroll_offset;
      #endif

      static uint8_t lcd_status_update_delay;
      static uint8_t status_message_level;      // Higher levels block lower levels
      static inline void reset_alert_level() { status_message_level = 0; }

      #if HAS_PRINT_PROGRESS
        #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
          static uint8_t progress_bar_percent;
          static void set_progress(const uint8_t progress) { progress_bar_percent = MIN(progress, 100); }
        #endif
        static uint8_t get_progress();
      #else
        static constexpr uint8_t get_progress() { return 0; }
      #endif

      #if HAS_LCD_CONTRAST
        static int16_t contrast;
        static void set_contrast(const int16_t value);
        static inline void refresh_contrast() { set_contrast(contrast); }
      #endif

      #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
        static millis_t next_filament_display;
      #endif

      static void quick_feedback(const bool clear_buttons=true);
      static void completion_feedback(const bool good=true);

      #if DISABLED(LIGHTWEIGHT_UI)
        static void draw_status_message(const bool blink);
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        static void draw_hotend_status(const uint8_t row, const uint8_t extruder);
      #endif

      static void status_screen();

    #else

      static void refresh();
      static void reset_alert_level();

    #endif

    static bool get_blink();
    static void kill_screen(PGM_P const lcd_msg);
    static void draw_kill_screen();
    static bool hasstatus();
    static void setstatus(const char* const message, const bool persist=false);
    static void setstatusPGM(PGM_P const message, const int8_t level=0);
    static void status_printf_P(const uint8_t level, PGM_P const fmt, ...);
    static void reset_status();

  #else // MALYAN_LCD or NO LCD

    static inline void refresh() {}
    static constexpr bool hasstatus() { return false; }
    static inline void setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
    static inline void setstatusPGM(PGM_P const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }
    static inline void status_printf_P(const uint8_t level, PGM_P const fmt, ...) { UNUSED(level); UNUSED(fmt); }
    static inline void reset_status() {}
    static inline void reset_alert_level() {}

  #endif

  #if HAS_LCD_MENU

    #if ENABLED(ENCODER_RATE_MULTIPLIER)
      static bool encoderRateMultiplierEnabled;
      static millis_t lastEncoderMovementMillis;
      static void enable_encoder_multiplier(const bool onoff);
    #endif

    #if ENABLED(SCROLL_LONG_FILENAMES)
      static uint8_t filename_scroll_pos, filename_scroll_max;
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

    static void manage_manual_move();

    static bool lcd_clicked;
    static bool use_click();

    static void synchronize(PGM_P const msg=NULL);

    static screenFunc_t currentScreen;
    static void goto_screen(const screenFunc_t screen, const uint32_t encoder=0);
    static void save_previous_screen();
    static void goto_previous_screen();
    static void return_to_status();
    static inline bool on_status_screen() { return currentScreen == status_screen; }
    static inline void run_current_screen() { (*currentScreen)(); }

    static inline void defer_status_screen(const bool defer) {
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
      static inline void chirp() { buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ); }
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      static void ubl_plot(const uint8_t x, const uint8_t inverted_y);
    #endif

  #elif HAS_SPI_LCD

    static constexpr bool lcd_clicked = false;
    static constexpr bool on_status_screen() { return true; }
    static inline void run_current_screen() { status_screen(); }

  #endif

  #if ENABLED(LCD_BED_LEVELING) && (ENABLED(PROBE_MANUALLY) || ENABLED(MESH_BED_LEVELING))
    static bool wait_for_bl_move;
  #else
    static constexpr bool wait_for_bl_move = false;
  #endif

  #if HAS_LCD_MENU && (ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION))
    static bool external_control;
    FORCE_INLINE static void capture() { external_control = true; }
    FORCE_INLINE static void release() { external_control = false; }
  #else
    static constexpr bool external_control = false;
  #endif

  #if HAS_ENCODER_ACTION

    static volatile uint8_t buttons;
    #if ENABLED(LCD_HAS_SLOW_BUTTONS)
      static volatile uint8_t slow_buttons;
      static uint8_t read_slow_buttons();
    #endif
    static void update_buttons();
    static inline bool button_pressed() { return BUTTON_CLICK(); }
    #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
      static void wait_for_release();
    #endif

    static uint32_t encoderPosition;

    #if ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODERBASE -1
    #else
      #define ENCODERBASE +1
    #endif
    #if ENABLED(REVERSE_MENU_DIRECTION)
      static int8_t encoderDirection;
      static inline void encoder_direction_normal() { encoderDirection = +(ENCODERBASE); }
      static inline void encoder_direction_menus()  { encoderDirection = -(ENCODERBASE); }
    #else
      static constexpr int8_t encoderDirection = ENCODERBASE;
      static inline void encoder_direction_normal() { }
      static inline void encoder_direction_menus()  { }
    #endif

  #else

    static inline void update_buttons() { }

  #endif

private:

  static void _synchronize();

  #if HAS_SPI_LCD
    #if HAS_LCD_MENU
      #if LCD_TIMEOUT_TO_STATUS
        static bool defer_return_to_status;
      #else
        static constexpr bool defer_return_to_status = false;
      #endif
    #endif
    static void draw_status_screen();
    static void finishstatus(const bool persist);
  #endif
};

extern MarlinUI ui;
