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

  // Font IDs
  enum MarlinFont : uint8_t {
    FONT_STATUSMENU = 1,
    FONT_EDIT,
    FONT_MENU
  };
  void lcd_setFont(const MarlinFont font_nr);

  #if ENABLED(LIGHTWEIGHT_UI)
    void lcd_in_status(const bool inStatus);
  #endif

#endif // HAS_GRAPHICAL_LCD

#if HAS_SPI_LCD || ENABLED(MALYAN_LCD) || ENABLED(EXTENSIBLE_UI)
  void lcd_init();
  bool lcd_detected();
  void lcd_update();
  void lcd_setalertstatusPGM(PGM_P message);
  void kill_screen(PGM_P lcd_msg);
#else
  inline void lcd_init() {}
  inline bool lcd_detected() { return true; }
  inline void lcd_update() {}
  inline void lcd_setalertstatusPGM(PGM_P message) { UNUSED(message); }
#endif

#if HAS_SPI_LCD

  #include "../Marlin.h"

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #include "../feature/pause.h"
    #include "../module/motion.h" // for active_extruder
  #endif

  void lcd_status_screen();
  void lcd_return_to_status();
  bool lcd_hasstatus();
  void lcd_setstatus(const char* message, const bool persist=false);
  void lcd_setstatusPGM(PGM_P message, const int8_t level=0);
  void lcd_setalertstatusPGM(PGM_P message);
  void lcd_reset_alert_level();
  void lcd_reset_status();
  void lcd_status_printf_P(const uint8_t level, PGM_P const fmt, ...);
  void lcd_kill_screen();
  void kill_screen(PGM_P lcd_msg);

  enum LCDViewAction : uint8_t {
    LCDVIEW_NONE,
    LCDVIEW_REDRAW_NOW,
    LCDVIEW_CALL_REDRAW_NEXT,
    LCDVIEW_CLEAR_CALL_REDRAW,
    LCDVIEW_CALL_NO_REDRAW
  };

  extern LCDViewAction lcdDrawUpdate;
  inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

  #if HAS_BUZZER
    void lcd_buzz(const long duration, const uint16_t freq);
  #else
    inline void lcd_buzz(const long duration, const uint16_t freq) { UNUSED(duration); UNUSED(freq); }
  #endif

  void lcd_quick_feedback(const bool clear_buttons); // Audible feedback for a button click - could also be visual

  #if ENABLED(LCD_PROGRESS_BAR)
    extern millis_t progress_bar_ms;  // Start time for the current progress bar cycle
    #if PROGRESS_MSG_EXPIRE > 0
      void dontExpireStatus();
    #endif
  #endif

  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    extern uint8_t progress_bar_percent;
  #endif

  #if ENABLED(ADC_KEYPAD)
    uint8_t get_ADC_keyValue();
  #endif

  #if HAS_LCD_CONTRAST
    extern int16_t lcd_contrast;
    void set_lcd_contrast(const int16_t value);
  #endif

  #if HAS_GRAPHICAL_LCD
    #define SETCURSOR(col, row) lcd_moveto(col * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
    #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_PIXEL_WIDTH - len * (MENU_FONT_WIDTH), (row + 1) * (MENU_FONT_HEIGHT))
  #else
    #define SETCURSOR(col, row) lcd_moveto(col, row)
    #define SETCURSOR_RJ(len, row) lcd_moveto(LCD_WIDTH - len, row)
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    void lcd_bootscreen();
  #endif

  #define LCD_UPDATE_INTERVAL 100
  #define BUTTON_EXISTS(BN) (defined(BTN_## BN) && BTN_## BN >= 0)
  #define BUTTON_PRESSED(BN) !READ(BTN_## BN)

  #if HAS_LCD_MENU

    typedef void (*screenFunc_t)();
    typedef void (*menuAction_t)();
    extern screenFunc_t currentScreen;
    void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder=0);

    extern bool lcd_clicked, defer_return_to_status;

    extern int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2];
    extern uint8_t lcd_preheat_fan_speed[2];

    #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
      extern bool lcd_external_control;
    #else
      constexpr bool lcd_external_control = false;
    #endif

    #if ENABLED(LCD_BED_LEVELING)
      extern bool lcd_wait_for_move;
    #else
      constexpr bool lcd_wait_for_move = false;
    #endif

    // Manual Movement
    constexpr float manual_feedrate_mm_m[XYZE] = MANUAL_FEEDRATE;
    extern float move_menu_scale;
    #if IS_KINEMATIC
      extern bool processing_manual_move;
    #else
      constexpr bool processing_manual_move = false;
    #endif

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      void lcd_advanced_pause_show_message(const AdvancedPauseMessage message,
                                           const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT,
                                           const uint8_t extruder=active_extruder);
    #endif

    #if ENABLED(G26_MESH_VALIDATION)
      void lcd_chirp();
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      void lcd_mesh_edit_setup(const float &initial);
      float lcd_mesh_edit();
      void lcd_z_offset_edit_setup(const float &initial);
      float lcd_z_offset_edit();
    #endif

    #if ENABLED(SCROLL_LONG_FILENAMES)
      extern uint8_t filename_scroll_pos, filename_scroll_max;
    #endif

  #endif // HAS_LCD_MENU

  #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
    extern millis_t previous_lcd_status_ms;
  #endif

  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    extern uint8_t status_scroll_offset;
  #endif

  bool lcd_blink();

  #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
    bool is_lcd_clicked();
    void wait_for_release();
  #endif

#elif ENABLED(EXTENSIBLE_UI)

  // These functions are defined elsewhere
  void lcd_setstatus(const char* const message, const bool persist=false);
  void lcd_setstatusPGM(const char* const message, const int8_t level=0);
  void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...);
  void lcd_reset_status();
  void lcd_refresh();
  void lcd_reset_alert_level();
  bool lcd_hasstatus();

#else // MALYAN_LCD or no LCD

  constexpr bool lcd_wait_for_move = false;

  inline void lcd_refresh() {}
  inline bool lcd_hasstatus() { return false; }
  inline void lcd_setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
  inline void lcd_setstatusPGM(PGM_P const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }
  inline void lcd_status_printf_P(const uint8_t level, PGM_P const fmt, ...) { UNUSED(level); UNUSED(fmt); }
  inline void lcd_reset_alert_level() {}
  inline void lcd_reset_status() {}

#endif

#if HAS_LCD_MENU

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
      #define LCD_BACK_CLICKED (buttons & EN_D)
    #endif

  #endif // NEWPANEL

  extern volatile uint8_t buttons;  // The last-checked buttons in a bit array.
  void lcd_buttons_update();

#else

  inline void lcd_buttons_update() {}

#endif

#if ENABLED(LCD_HAS_SLOW_BUTTONS)
  extern volatile uint8_t slow_buttons;
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
    #define LCD_CLICKED() ((buttons & EN_C) || RRK(EN_REPRAPWORLD_KEYPAD_MIDDLE))
  #else
    #define LCD_CLICKED() RRK(EN_REPRAPWORLD_KEYPAD_MIDDLE)
  #endif

#endif // REPRAPWORLD_KEYPAD

#ifndef LCD_CLICKED
  #ifdef EN_C
    #define LCD_CLICKED() (buttons & EN_C)
  #else
    #define LCD_CLICKED() false
  #endif
#endif

extern uint8_t lcd_status_update_delay;
extern char lcd_status_message[];

#define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

// For i2c define BUZZ to use lcd_buzz
#if ENABLED(LCD_USE_I2C_BUZZER)
  #define BUZZ(d,f) lcd_buzz(d, f)
#endif

#if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  void lcd_reselect_last_file();
#endif

#if HAS_GRAPHICAL_LCD
  extern bool drawing_screen, first_page;
#elif HAS_SPI_LCD
  constexpr bool first_page = true;
#endif

// LCD implementations
void lcd_implementation_clear();
void lcd_implementation_init();

#if HAS_CHARACTER_LCD

  enum HD44780CharSet : uint8_t { CHARSET_MENU, CHARSET_INFO, CHARSET_BOOT };

  void lcd_set_custom_characters(
    #if ENABLED(LCD_PROGRESS_BAR) || ENABLED(SHOW_BOOTSCREEN)
      const HD44780CharSet screen_charset=CHARSET_INFO
    #endif
  );
  #if ENABLED(LCD_PROGRESS_BAR)
    #define LCD_SET_CHARSET(C) lcd_set_custom_characters(C)
  #else
    #define LCD_SET_CHARSET(C) lcd_set_custom_characters()
  #endif

#endif
