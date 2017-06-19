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

/**
 * ultralcd_impl_DOGM.h
 *
 * Graphics LCD implementation for 128x64 pixel LCDs by STB for ErikZalm/Marlin
 * Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 * License: http://opensource.org/licenses/BSD-3-Clause
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * https://github.com/olikraus/U8glib_Arduino
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef ULTRALCD_IMPL_DOGM_H
#define ULTRALCD_IMPL_DOGM_H

#include "MarlinConfig.h"

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * These are common LCD 128x64 pixel graphic displays.
 */
#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"
#include "dogm_bitmaps.h"
#include "utility.h"
#include "duration_t.h"
#include "temperature.h"

#include <U8glib.h>

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"
#endif

#if ENABLED(SHOW_BOOTSCREEN) && ENABLED(SHOW_CUSTOM_BOOTSCREEN)
  #include "_Bootscreen.h"
#endif

// Only Western languages support big / small fonts
//#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
#if !defined(DISPLAY_CHARSET_HD44780)
  #undef USE_BIG_EDIT_FONT
  #undef USE_SMALL_INFOFONT
#endif

#if ENABLED(USE_SMALL_INFOFONT)
  #include "dogm_font_data_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
#else
  #define FONT_STATUSMENU_NAME FONT_MENU_NAME
#endif

#include "dogm_font_data_Marlin_symbols.h"   // The Marlin special symbols
#define FONT_SPECIAL_NAME Marlin_symbols

#if DISABLED(SIMULATE_ROMFONT)
  #if ENABLED(DISPLAY_CHARSET_ISO10646_1)
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_PL)
    #include "dogm_font_data_ISO10646_1_PL.h"
    #define FONT_MENU_NAME ISO10646_1_PL_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_5)
    #include "dogm_font_data_ISO10646_5_Cyrillic.h"
    #define FONT_MENU_NAME ISO10646_5_Cyrillic_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_KANA)
    #include "dogm_font_data_ISO10646_Kana.h"
    #define FONT_MENU_NAME ISO10646_Kana_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_GREEK)
    #include "dogm_font_data_ISO10646_Greek.h"
    #define FONT_MENU_NAME ISO10646_Greek_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_CN)
    #include "dogm_font_data_ISO10646_CN.h"
    #define FONT_MENU_NAME ISO10646_CN
    #define TALL_FONT_CORRECTION 1
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_TR)
    #include "dogm_font_data_ISO10646_1_tr.h"
    #define FONT_MENU_NAME ISO10646_TR
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #endif
#else // SIMULATE_ROMFONT
  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    #include "dogm_font_data_HD44780_J.h"
    #define FONT_MENU_NAME HD44780_J_5x7
  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    #include "dogm_font_data_HD44780_W.h"
    #define FONT_MENU_NAME HD44780_W_5x7
  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    #include "dogm_font_data_HD44780_C.h"
    #define FONT_MENU_NAME HD44780_C_5x7
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #endif
#endif // SIMULATE_ROMFONT

//#define FONT_STATUSMENU_NAME FONT_MENU_NAME

#define FONT_STATUSMENU 1
#define FONT_SPECIAL 2
#define FONT_MENU_EDIT 3
#define FONT_MENU 4

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH         6
#define DOG_CHAR_HEIGHT        12
#if ENABLED(USE_BIG_EDIT_FONT)
  #define FONT_MENU_EDIT_NAME u8g_font_9x18
  #define DOG_CHAR_WIDTH_EDIT  9
  #define DOG_CHAR_HEIGHT_EDIT 13
  #define LCD_WIDTH_EDIT       14
#else
  #define FONT_MENU_EDIT_NAME FONT_MENU_NAME
  #define DOG_CHAR_WIDTH_EDIT  6
  #define DOG_CHAR_HEIGHT_EDIT 12
  #define LCD_WIDTH_EDIT       22
#endif

#ifndef TALL_FONT_CORRECTION
  #define TALL_FONT_CORRECTION 0
#endif

#define START_COL              0

// LCD selection
#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  extern U8GLIB_ST7920_128X64_4X u8g ;// (LCD_PINS_RS); // 2 stripes
  // U8GLIB_ST7920_128X64 u8g(LCD_PINS_RS); // 8 stripes
#elif ENABLED(U8GLIB_ST7920)
  //U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS); // Original u8glib device. 2 stripes
                                                                            // No 4 stripe device available from u8glib.
  //U8GLIB_ST7920_128X64_1X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS);    // Original u8glib device. 8 stripes
  extern U8GLIB_ST7920_128X64_RRD u8g ;// (0); // Number of stripes can be adjusted in ultralcd_st7920_u8glib_rrd.h with PAGE_HEIGHT
#elif ENABLED(CARTESIO_UI)
  // The CartesioUI display
  #if DOGLCD_MOSI != -1 && DOGLCD_SCK != -1
    // using SW-SPI
    //U8GLIB_DOGM128 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    extern U8GLIB_DOGM128_2X u8g ;// (DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0); // 4 stripes
  #else
    //U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    extern U8GLIB_DOGM128_2X u8g ;// (DOGLCD_CS, DOGLCD_A0); // 4 stripes
  #endif
#elif ENABLED(U8GLIB_LM6059_AF)
  // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
  //U8GLIB_LM6059 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
  extern U8GLIB_LM6059_2X u8g ;// (DOGLCD_CS, DOGLCD_A0); // 4 stripes
#elif ENABLED(MAKRPANEL) || ENABLED(VIKI2) || ENABLED(miniVIKI)
  // The MaKrPanel, Mini Viki, and Viki 2.0, ST7565 controller as well
  //U8GLIB_NHD_C12864 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
  extern U8GLIB_NHD_C12864_2X u8g ;// (DOGLCD_CS, DOGLCD_A0); // 4 stripes
#elif ENABLED(U8GLIB_SSD1306)
  // Generic support for SSD1306 OLED I2C LCDs
  //U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);  // 8 stripes
  extern U8GLIB_SSD1306_128X64_2X u8g ;// (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
#elif ENABLED(U8GLIB_SH1106)
  // Generic support for SH1106 OLED I2C LCDs
  //U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);  // 8 stripes
  extern U8GLIB_SH1106_128X64_2X u8g ;// (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
#elif ENABLED(MINIPANEL)
  // The MINIPanel display
  //U8GLIB_MINI12864 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
  extern U8GLIB_MINI12864_2X u8g ;// (DOGLCD_CS, DOGLCD_A0); // 4 stripes
#else
  // for regular DOGM128 display with HW-SPI
  //U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0  // 8 stripes
  extern U8GLIB_DOGM128_2X u8g ;// (DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0 // 4 stripes
#endif

#ifndef LCD_PIXEL_WIDTH
  #define LCD_PIXEL_WIDTH 128
#endif
#ifndef LCD_PIXEL_HEIGHT
  #define LCD_PIXEL_HEIGHT 64
#endif

extern uint16_t lcd_contrast;
static char currentfont = 0;

// The current graphical page being rendered
extern u8g_page_t &page; // = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;

// For selective rendering within a Y range
#define PAGE_UNDER(yb) (u8g.getU8g()->current_page.y0 <= (yb))
#define PAGE_CONTAINS(ya, yb) (PAGE_UNDER(yb) && u8g.getU8g()->current_page.y1 >= (ya))

void lcd_setFont(const char font_nr);

void lcd_print(const char c);

char lcd_print_and_count(const char c);

void lcd_print(const char *str);
void lcd_printPGM(const char *str);

void lcd_print_utf(const char *str, uint8_t n=LCD_WIDTH);

void lcd_printPGM_utf(const char *str, uint8_t n=LCD_WIDTH);

// Initialize or re-initialize the LCD
void lcd_implementation_init();

void lcd_kill_screen();

void lcd_implementation_clear();

//
// Status Screen
//

#if DISABLED(DOGM_SD_PERCENT)
  #define SD_DURATION_X (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH / 2) - len * (DOG_CHAR_WIDTH / 2))
#else
  #define SD_DURATION_X (LCD_PIXEL_WIDTH - len * DOG_CHAR_WIDTH)
#endif

#if ENABLED(USE_SMALL_INFOFONT)
  #define INFO_FONT_HEIGHT 7
#else
  #define INFO_FONT_HEIGHT 8
#endif

#define XYZ_BASELINE (30 + INFO_FONT_HEIGHT)

#define X_LABEL_POS  3
#define X_VALUE_POS 11
#define XYZ_SPACING 40

#if ENABLED(XYZ_HOLLOW_FRAME)
  #define XYZ_FRAME_TOP 29
  #define XYZ_FRAME_HEIGHT INFO_FONT_HEIGHT + 3
#else
  #define XYZ_FRAME_TOP 30
  #define XYZ_FRAME_HEIGHT INFO_FONT_HEIGHT + 1
#endif

FORCE_INLINE void _draw_centered_temp(const int temp, const uint8_t x, const uint8_t y);

FORCE_INLINE void _draw_heater_status(const uint8_t x, const int8_t heater, const bool blink);

FORCE_INLINE void _draw_axis_label(const AxisEnum axis, const char* const pstr, const bool blink);

inline void lcd_implementation_status_message(const bool blink);

void lcd_implementation_status_screen();

#if ENABLED(ULTIPANEL)

  extern uint8_t row_y1, row_y2;
  extern uint8_t row_height;

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void lcd_implementation_hotend_status(const uint8_t row);

  #endif // ADVANCED_PAUSE_FEATURE

  // Set the colors for a menu item based on whether it is selected
  static void lcd_implementation_mark_as_selected(const uint8_t row, const bool isSelected);

  // Draw a static line of text in the same idiom as a menu item
  void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char* valstr=NULL);

  // Draw a generic menu item
  void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char* pstr, const char pre_char, const char post_char);

  // Macros for specific types of menu items
  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

  // Draw a menu item with an editable value
  void _drawmenu_setting_edit_generic(const bool isSelected, const uint8_t row, const char* pstr, const char* const data, const bool pgm);

  // Macros for edit items
  #define lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, false)
  #define lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, true)

  #define DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(_type, _name, _strFunc) \
    inline void lcd_implementation_drawmenu_setting_edit_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, _strFunc(*(data))); \
    } \
    inline void lcd_implementation_drawmenu_setting_edit_callback_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, _strFunc(*(data))); \
    } \
    inline void lcd_implementation_drawmenu_setting_edit_accessor_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type (*pget)(), void (*pset)(_type), ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, _strFunc(pget())); \
    } \
    typedef void _name##_void

  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(int, int3, itostr3);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float3, ftostr3);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float32, ftostr32);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float43, ftostr43sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float5, ftostr5rj);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float51, ftostr51sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float52, ftostr52sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float62, ftostr62rj);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(unsigned long, long5, ftostr5rj);

  #define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_accessor_bool(sel, row, pstr, pstr2, pget, pset) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  void lcd_implementation_drawedit(const char* const pstr, const char* const value=NULL);

  #if ENABLED(SDSUPPORT)
    #include "cardreader.h"
    void _drawmenu_sd(const bool isSelected, const uint8_t row, const char* const pstr, const char* filename, char* const longFilename, const bool isDir);

    #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
    #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

  #endif // SDSUPPORT

#endif // ULTIPANEL

#if ENABLED(AUTO_BED_LEVELING_UBL)

  /**
   * UBL LCD "radar" map data
   */
  #define MAP_UPPER_LEFT_CORNER_X 35  // These probably should be moved to the .h file  But for now,
  #define MAP_UPPER_LEFT_CORNER_Y 8  // it is easier to play with things having them here
  #define MAP_MAX_PIXELS_X        53
  #define MAP_MAX_PIXELS_Y        49

  void _lcd_ubl_plot_DOGLCD(uint8_t x_plot, uint8_t y_plot);

#endif // AUTO_BED_LEVELING_UBL

#endif // __ULTRALCD_IMPL_DOGM_H
