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

#include "../inc/MarlinConfig.h"

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * These are common LCD 128x64 pixel graphic displays.
 */
#include "ultralcd.h"

/*
#if ENABLED(U8GLIB_ST7565_64128N)
  #include "dogm/ultralcd_st7565_u8glib_VIKI.h"
#elif ENABLED(U8GLIB_ST7920)
  #include "dogm/ultralcd_st7920_u8glib_rrd.h"
#endif
*/

#include "dogm/dogm_bitmaps.h"

#if ENABLED(SDSUPPORT)
  #include "../libs/duration_t.h"
#endif

#include <U8glib.h>
#include "fontutils.h"
#include "u8g_fontutf8.h"

#include "dogm/HAL_LCD_class_defines.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../feature/bedlevel/ubl/ubl.h"
#endif

// Only Western languages support big / small fonts
#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
  #undef USE_BIG_EDIT_FONT
  #undef USE_SMALL_INFOFONT
#endif

#if ENABLED(USE_SMALL_INFOFONT)
  #include "dogm/dogm_font_data_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
  #define INFO_FONT_HEIGHT 7
#else
  #define FONT_STATUSMENU_NAME FONT_MENU_NAME
  #define INFO_FONT_HEIGHT 8
#endif

// The Marlin special symbols is now in the dogm_font_data_ISO10646_1.h
#define FONT_SPECIAL_NAME ISO10646_1_5x7

#include LANGUAGE_DATA_INCL(LCD_LANGUAGE)

#include "dogm/dogm_font_data_ISO10646_1.h"
#define FONT_MENU_NAME ISO10646_1_5x7

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
  #define DOG_CHAR_HEIGHT_EDIT 18
#else
  #define FONT_MENU_EDIT_NAME FONT_MENU_NAME
  #define DOG_CHAR_WIDTH_EDIT  DOG_CHAR_WIDTH
  #define DOG_CHAR_HEIGHT_EDIT DOG_CHAR_HEIGHT
#endif

#ifndef TALL_FONT_CORRECTION
  #define TALL_FONT_CORRECTION 0
#endif

#define START_COL              0

// LCD selection
#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  #ifdef DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
    U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_RS); // 2 stripes, HW SPI (shared with SD card)
  #else
    U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS); // Original u8glib device. 2 stripes, SW SPI
  #endif

#elif ENABLED(U8GLIB_ST7920)
  // RepRap Discount Full Graphics Smart Controller
  #if DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
    U8GLIB_ST7920_128X64_4X_HAL u8g(LCD_PINS_RS); // 2 stripes, HW SPI (shared with SD card, on AVR does not use standard LCD adapter)
  #else
    //U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS); // Original u8glib device. 2 stripes, SW SPI
    U8GLIB_ST7920_128X64_RRD u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS); // Number of stripes can be adjusted in ultralcd_st7920_u8glib_rrd.h with PAGE_HEIGHT
                                                                           // AVR version ignores these pin settings
                                                                           // HAL version uses these pin settings
  #endif

#elif ENABLED(CARTESIO_UI)
  // The CartesioUI display
    //U8GLIB_DOGM128_2X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0); // 4 stripes
    U8GLIB_DOGM128_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes

#elif ENABLED(U8GLIB_LM6059_AF)
  // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
    //U8GLIB_LM6059 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    U8GLIB_LM6059_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes

#elif ENABLED(U8GLIB_ST7565_64128N)
  // The MaKrPanel, Mini Viki, Viki 2.0 & AZSMZ 12864 ST7565 controller
  #define SMART_RAMPS (MB(RAMPS_SMART_EFB) || MB(RAMPS_SMART_EEB) || MB(RAMPS_SMART_EFF) || MB(RAMPS_SMART_EEF) || MB(RAMPS_SMART_SF))
  #if DOGLCD_SCK == SCK_PIN && DOGLCD_MOSI == MOSI_PIN && !SMART_RAMPS
    U8GLIB_64128N_2X_HAL u8g(DOGLCD_CS, DOGLCD_A0);  // using HW-SPI
  #else
    U8GLIB_64128N_2X_HAL u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);  // using SW-SPI
  #endif

#elif ENABLED(MKS_12864OLED_SSD1306)
  // MKS 128x64 (SSD1306) OLED I2C LCD
    U8GLIB_SSD1306_128X64 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);      // 8 stripes
    //U8GLIB_SSD1306_128X64_2X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0); // 4 stripes

#elif ENABLED(U8GLIB_SSD1306)
  // Generic support for SSD1306 OLED I2C LCDs
    //U8GLIB_SSD1306_128X64_2X_I2C_2_WIRE  u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
    U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes

#elif ENABLED(MKS_12864OLED)
  // MKS 128x64 (SH1106) OLED I2C LCD
    U8GLIB_SH1106_128X64 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);      // 8 stripes
    //U8GLIB_SH1106_128X64_2X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0); // 4 stripes
#elif ENABLED(U8GLIB_SH1106)
  // Generic support for SH1106 OLED I2C LCDs
    //U8GLIB_SH1106_128X64_2X_I2C_2_WIRE  u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
    U8GLIB_SH1106_128X64_2X u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
#elif ENABLED(U8GLIB_SSD1309)
  // Generic support for SSD1309 OLED I2C LCDs
  U8GLIB_SSD1309_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);
#elif ENABLED(MINIPANEL)
  // The MINIPanel display
    //U8GLIB_MINI12864 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    U8GLIB_MINI12864_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes
#else
  // for regular DOGM128 display with HW-SPI
    //U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0  // 8 stripes
    U8GLIB_DOGM128_2X u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0 // 4 stripes
#endif

U8GLIB *pu8g = &u8g;

#ifndef LCD_PIXEL_WIDTH
  #define LCD_PIXEL_WIDTH 128
#endif
#ifndef LCD_PIXEL_HEIGHT
  #define LCD_PIXEL_HEIGHT 64
#endif

#include "lcdprint.h"

int16_t lcd_contrast; // Initialized by settings.load()
static char currentfont = 0;

// The current graphical page being rendered
u8g_page_t &page = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;

// For selective rendering within a Y range
#define PAGE_UNDER(yb) (u8g.getU8g()->current_page.y0 <= (yb))
#define PAGE_CONTAINS(ya, yb) (PAGE_UNDER(yb) && u8g.getU8g()->current_page.y1 >= (ya))

static void lcd_setFont(const char font_nr) {
  switch (font_nr) {
    case FONT_STATUSMENU : {u8g.setFont(FONT_STATUSMENU_NAME); currentfont = FONT_STATUSMENU;}; break;
    default:
    case FONT_MENU       : {u8g.setFont(FONT_MENU_NAME); currentfont = FONT_MENU;}; break;
    case FONT_SPECIAL    : {u8g.setFont(FONT_SPECIAL_NAME); currentfont = FONT_SPECIAL;}; break;
    case FONT_MENU_EDIT  : {u8g.setFont(FONT_MENU_EDIT_NAME); currentfont = FONT_MENU_EDIT;}; break;
  }
}

#if ENABLED(SHOW_BOOTSCREEN)

  #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)

    void lcd_custom_bootscreen() {
      constexpr u8g_uint_t left = (LCD_PIXEL_WIDTH  - (CUSTOM_BOOTSCREEN_BMPWIDTH)) / 2,
                           top = (LCD_PIXEL_HEIGHT - (CUSTOM_BOOTSCREEN_BMPHEIGHT)) / 2;
      #if ENABLED(CUSTOM_BOOTSCREEN_INVERTED)
        constexpr u8g_uint_t right = left + CUSTOM_BOOTSCREEN_BMPWIDTH,
                            bottom = top + CUSTOM_BOOTSCREEN_BMPHEIGHT;
      #endif
      u8g.firstPage();
      do {
        u8g.drawBitmapP(
          left, top,
          CEILING(CUSTOM_BOOTSCREEN_BMPWIDTH, 8), CUSTOM_BOOTSCREEN_BMPHEIGHT, custom_start_bmp
        );
        #if ENABLED(CUSTOM_BOOTSCREEN_INVERTED)
          u8g.setColorIndex(1);
          if (top) u8g.drawBox(0, 0, LCD_PIXEL_WIDTH, top);
          if (left) u8g.drawBox(0, top, left, CUSTOM_BOOTSCREEN_BMPHEIGHT);
          if (right < LCD_PIXEL_WIDTH) u8g.drawBox(right, top, LCD_PIXEL_WIDTH - right, CUSTOM_BOOTSCREEN_BMPHEIGHT);
          if (bottom < LCD_PIXEL_HEIGHT) u8g.drawBox(0, bottom, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT - bottom);
        #endif
      } while (u8g.nextPage());
      safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);
    }

  #endif // SHOW_CUSTOM_BOOTSCREEN

  void lcd_bootscreen() {
    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      lcd_custom_bootscreen();
    #endif

    constexpr uint8_t offy =
      #if ENABLED(START_BMPHIGH)
        (LCD_PIXEL_HEIGHT - (START_BMPHEIGHT)) / 2
      #else
        DOG_CHAR_HEIGHT
      #endif
    ;

    const uint8_t width = u8g.getWidth(), height = u8g.getHeight(),
                  offx = (width - (START_BMPWIDTH)) / 2;

    u8g.firstPage();
    do {
      u8g.drawBitmapP(offx, offy, (START_BMPWIDTH + 7) / 8, START_BMPHEIGHT, start_bmp);
      lcd_setFont(FONT_MENU);
      #ifndef STRING_SPLASH_LINE2
        const uint8_t txt1X = width - (sizeof(STRING_SPLASH_LINE1) - 1) * (DOG_CHAR_WIDTH);
        u8g.drawStr(txt1X, (height + DOG_CHAR_HEIGHT) / 2, STRING_SPLASH_LINE1);
      #else
        const uint8_t txt1X = (width - (sizeof(STRING_SPLASH_LINE1) - 1) * (DOG_CHAR_WIDTH)) / 2,
                      txt2X = (width - (sizeof(STRING_SPLASH_LINE2) - 1) * (DOG_CHAR_WIDTH)) / 2;
        u8g.drawStr(txt1X, height - (DOG_CHAR_HEIGHT) * 3 / 2, STRING_SPLASH_LINE1);
        u8g.drawStr(txt2X, height - (DOG_CHAR_HEIGHT) * 1 / 2, STRING_SPLASH_LINE2);
      #endif
    } while (u8g.nextPage());
    safe_delay(BOOTSCREEN_TIMEOUT);
  }

#endif // SHOW_BOOTSCREEN

#if ENABLED(LIGHTWEIGHT_UI)
  #include "dogm/status_screen_lite_ST7920.h"
#else
  #include "dogm/status_screen_DOGM.h"
#endif

// Initialize or re-initialize the LCD
static void lcd_implementation_init() {

  #if PIN_EXISTS(LCD_BACKLIGHT) // Enable LCD backlight
    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
  #endif

  #if ENABLED(MKS_12864OLED) || ENABLED(MKS_12864OLED_SSD1306)
    SET_OUTPUT(LCD_PINS_DC);
    OUT_WRITE(LCD_PINS_RS, LOW);
    _delay_ms(500);
    WRITE(LCD_PINS_RS, HIGH);
  #endif

  #if PIN_EXISTS(LCD_RESET)
    OUT_WRITE(LCD_RESET_PIN, LOW); // perform a clean hardware reset
    _delay_ms(5);
    OUT_WRITE(LCD_RESET_PIN, HIGH);
    _delay_ms(5); // delay to allow the display to initalize
  #endif

  #if PIN_EXISTS(LCD_RESET) || ENABLED(MKS_12864OLED) || ENABLED(MKS_12864OLED_SSD1306)
    u8g.begin();
  #endif

  #if DISABLED(MINIPANEL) // setContrast not working for Mini Panel
    u8g.setContrast(lcd_contrast);
  #endif

  #if ENABLED(LCD_SCREEN_ROT_90)
    u8g.setRot90();   // Rotate screen by 90°
  #elif ENABLED(LCD_SCREEN_ROT_180)
    u8g.setRot180();  // Rotate screen by 180°
  #elif ENABLED(LCD_SCREEN_ROT_270)
    u8g.setRot270();  // Rotate screen by 270°
  #endif

  uxg_SetUtf8Fonts (g_fontinfo, NUM_ARRAY(g_fontinfo));
}

// The kill screen is displayed for unrecoverable conditions
void lcd_kill_screen() {
  #if ENABLED(LIGHTWEIGHT_UI)
    ST7920_Lite_Status_Screen::clear_text_buffer();
  #endif
  const uint8_t h4 = u8g.getHeight() / 4;
  u8g.firstPage();
  do {
    lcd_setFont(FONT_MENU);
    lcd_moveto(0, h4 * 1);
    lcd_put_u8str(lcd_status_message);
    lcd_moveto(0, h4 * 2);
    lcd_put_u8str_rom(PSTR(MSG_HALTED));
    lcd_moveto(0, h4 * 3);
    lcd_put_u8str_rom(PSTR(MSG_PLEASE_RESET));
  } while (u8g.nextPage());
}

void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

#if ENABLED(ULTIPANEL)

  uint8_t row_y1, row_y2;
  uint8_t constexpr row_height = DOG_CHAR_HEIGHT + 2 * (TALL_FONT_CORRECTION);

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    static void lcd_implementation_hotend_status(const uint8_t row, const uint8_t extruder=active_extruder) {
      row_y1 = row * row_height + 1;
      row_y2 = row_y1 + row_height - 1;

      if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return;

      lcd_moveto(LCD_PIXEL_WIDTH - 11 * (DOG_CHAR_WIDTH), row_y2);
      lcd_put_wchar('E');
      lcd_put_wchar((char)('1' + extruder));
      lcd_put_wchar(' ');
      lcd_put_u8str(itostr3(thermalManager.degHotend(extruder)));
      lcd_put_wchar('/');

      if (lcd_blink() || !thermalManager.is_heater_idle(extruder))
        lcd_put_u8str(itostr3(thermalManager.degTargetHotend(extruder)));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  // Set the colors for a menu item based on whether it is selected
  static bool lcd_implementation_mark_as_selected(const uint8_t row, const bool isSelected) {
    row_y1 = row * row_height + 1;
    row_y2 = row_y1 + row_height - 1;

    if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return false;

    if (isSelected) {
      #if ENABLED(MENU_HOLLOW_FRAME)
        u8g.drawHLine(0, row_y1 + 1, LCD_PIXEL_WIDTH);
        u8g.drawHLine(0, row_y2 + 2, LCD_PIXEL_WIDTH);
      #else
        u8g.setColorIndex(1); // black on white
        u8g.drawBox(0, row_y1 + 2, LCD_PIXEL_WIDTH, row_height - 1);
        u8g.setColorIndex(0); // white on black
      #endif
    }
    #if DISABLED(MENU_HOLLOW_FRAME)
      else {
        u8g.setColorIndex(1); // unmarked text is black on white
      }
    #endif

    if (!PAGE_CONTAINS(row_y1, row_y2)) return false;

    lcd_moveto((START_COL) * (DOG_CHAR_WIDTH), row_y2);
    return true;
  }

  // Draw a static line of text in the same idiom as a menu item
  static void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char* valstr=NULL) {

    if (lcd_implementation_mark_as_selected(row, invert)) {

      uint8_t n = LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH) * (START_COL); // pixel width of string allowed

      if (center && !valstr) {
        int8_t pad = (LCD_WIDTH - utf8_strlen_P(pstr)) / 2;
        while (--pad >= 0) { lcd_put_wchar(' '); n--; }
      }
      n -= lcd_put_u8str_max_rom(pstr, n);
      if (NULL != valstr) {
        n -= lcd_put_u8str_max(valstr, n);
      }

      while (n - DOG_CHAR_WIDTH > 0) { n -= lcd_put_wchar(' '); }
    }
  }

  // Draw a generic menu item
  static void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char* pstr, const char pre_char, const char post_char) {
    UNUSED(pre_char);

    if (lcd_implementation_mark_as_selected(row, isSelected)) {
      uint8_t n = LCD_WIDTH - (START_COL) - 2;
      n *= DOG_CHAR_WIDTH;
      n -= lcd_put_u8str_max_rom(pstr, n);
      while (n - DOG_CHAR_WIDTH > 0) { n -= lcd_put_wchar(' '); }
      lcd_moveto(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH), row_y2);
      lcd_put_wchar(post_char);
      lcd_put_wchar(' ');
    }
  }

  // Macros for specific types of menu items
  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

  // Draw a menu item with an editable value
  static void _drawmenu_setting_edit_generic(const bool isSelected, const uint8_t row, const char* pstr, const char* const data, const bool pgm) {
    if (lcd_implementation_mark_as_selected(row, isSelected)) {
      const uint8_t vallen = (pgm ? utf8_strlen_P(data) : utf8_strlen((char*)data));
      uint8_t n = LCD_WIDTH - (START_COL) - 2 - vallen;
      n *= DOG_CHAR_WIDTH;
      n -= lcd_put_u8str_max_rom(pstr, n);
      lcd_put_wchar(':');
      while (n - DOG_CHAR_WIDTH > 0) { n -= lcd_put_wchar(' '); }
      lcd_moveto(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH) * vallen, row_y2);
      if (pgm) lcd_put_u8str_rom(data); else lcd_put_u8str((char*)data);
    }
  }

  // Macros for edit items
  #define lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, false)
  #define lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, true)

  #define DRAWMENU_SETTING_EDIT_GENERIC(_src) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, _src)
  #define DRAW_BOOL_SETTING(sel, row, pstr, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  void lcd_implementation_drawedit(const char* const pstr, const char* const value=NULL) {
    const uint8_t labellen = utf8_strlen_P(pstr),
                  vallen = utf8_strlen(value);

    uint8_t rows = (labellen > LCD_WIDTH - 2 - vallen) ? 2 : 1;

    #if ENABLED(USE_BIG_EDIT_FONT)
      constexpr uint8_t lcd_width_edit = (LCD_PIXEL_WIDTH) / (DOG_CHAR_WIDTH_EDIT);

      uint8_t lcd_width, char_width;
      if (labellen <= lcd_width_edit - 1) {
        if (labellen + vallen + 2 >= lcd_width_edit) rows = 2;
        lcd_width = lcd_width_edit + 1;
        char_width = DOG_CHAR_WIDTH_EDIT;
        lcd_setFont(FONT_MENU_EDIT);
      }
      else {
        lcd_width = LCD_WIDTH - (START_COL);
        char_width = DOG_CHAR_WIDTH;
        lcd_setFont(FONT_MENU);
      }
    #else
      constexpr uint8_t lcd_width = LCD_WIDTH - (START_COL),
                        char_width = DOG_CHAR_WIDTH;
    #endif

    // Center either one or two rows
    const uint8_t segmentHeight = u8g.getHeight() / (rows + 1); // 1 / (rows+1) = 1/2 or 1/3
    uint8_t baseline = segmentHeight + (DOG_CHAR_HEIGHT_EDIT + 1) / 2;

    bool onpage = PAGE_CONTAINS(baseline + 1 - (DOG_CHAR_HEIGHT_EDIT), baseline);
    if (onpage) {
      lcd_moveto(0, baseline);
      lcd_put_u8str_rom(pstr);
    }

    if (value != NULL) {
      lcd_put_wchar(':');
      if (rows == 2) {
        baseline += segmentHeight;
        onpage = PAGE_CONTAINS(baseline + 1 - (DOG_CHAR_HEIGHT_EDIT), baseline);
      }
      if (onpage) {
        lcd_moveto(((lcd_width - 1) - (vallen + 1)) * char_width, baseline); // Right-justified, leaving padded by spaces
        lcd_put_wchar(' '); // overwrite char if value gets shorter
        lcd_put_u8str(value);
      }
    }
  }

  #if ENABLED(SDSUPPORT)

    static void _drawmenu_sd(const bool isSelected, const uint8_t row, const char* const pstr, const char* filename, char* const longFilename, const bool isDir) {
      UNUSED(pstr);

      lcd_implementation_mark_as_selected(row, isSelected);

      if (!PAGE_CONTAINS(row_y1, row_y2)) return;

      constexpr uint8_t maxlen = LCD_WIDTH - (START_COL) - 1;
      const char *outstr = longFilename[0] ? longFilename : filename;
      if (longFilename[0]) {
        #if ENABLED(SCROLL_LONG_FILENAMES)
          if (isSelected) {
            uint8_t name_hash = row;
            for (uint8_t l = FILENAME_LENGTH; l--;)
              name_hash = ((name_hash << 1) | (name_hash >> 7)) ^ filename[l];  // rotate, xor
            if (filename_scroll_hash != name_hash) {                            // If the hash changed...
              filename_scroll_hash = name_hash;                                 // Save the new hash
              filename_scroll_max = max(0, utf8_strlen(longFilename) - maxlen); // Update the scroll limit
              filename_scroll_pos = 0;                                          // Reset scroll to the start
              lcd_status_update_delay = 8;                                      // Don't scroll right away
            }
            outstr += filename_scroll_pos;
          }
        #else
          longFilename[maxlen] = '\0'; // cutoff at screen edge
        #endif
      }

      if (isDir) lcd_put_wchar(LCD_STR_FOLDER[0]);

      int n;
      n = lcd_put_u8str_max(outstr, maxlen * (DOG_CHAR_WIDTH));
      n = maxlen * (DOG_CHAR_WIDTH) - n;
      while (n - DOG_CHAR_WIDTH > 0) { n -= lcd_put_wchar(' '); }
    }

    #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
    #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

  #endif // SDSUPPORT

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    /**
     * UBL LCD "radar" map data
     */
    #define MAP_UPPER_LEFT_CORNER_X 35  // These probably should be moved to the .h file  But for now,
    #define MAP_UPPER_LEFT_CORNER_Y  8  // it is easier to play with things having them here
    #define MAP_MAX_PIXELS_X        53
    #define MAP_MAX_PIXELS_Y        49

    void lcd_implementation_ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {
      // Scale the box pixels appropriately
      uint8_t x_map_pixels = ((MAP_MAX_PIXELS_X - 4) / (GRID_MAX_POINTS_X)) * (GRID_MAX_POINTS_X),
              y_map_pixels = ((MAP_MAX_PIXELS_Y - 4) / (GRID_MAX_POINTS_Y)) * (GRID_MAX_POINTS_Y),

              pixels_per_x_mesh_pnt = x_map_pixels / (GRID_MAX_POINTS_X),
              pixels_per_y_mesh_pnt = y_map_pixels / (GRID_MAX_POINTS_Y),

              x_offset = MAP_UPPER_LEFT_CORNER_X + 1 + (MAP_MAX_PIXELS_X - x_map_pixels - 2) / 2,
              y_offset = MAP_UPPER_LEFT_CORNER_Y + 1 + (MAP_MAX_PIXELS_Y - y_map_pixels - 2) / 2;

      // Clear the Mesh Map

      if (PAGE_CONTAINS(y_offset - 2, y_offset + y_map_pixels + 4)) {
        u8g.setColorIndex(1);  // First draw the bigger box in White so we have a border around the mesh map box
        u8g.drawBox(x_offset - 2, y_offset - 2, x_map_pixels + 4, y_map_pixels + 4);
        if (PAGE_CONTAINS(y_offset, y_offset + y_map_pixels)) {
          u8g.setColorIndex(0);  // Now actually clear the mesh map box
          u8g.drawBox(x_offset, y_offset, x_map_pixels, y_map_pixels);
        }
      }

      // Display Mesh Point Locations

      u8g.setColorIndex(1);
      const uint8_t sx = x_offset + pixels_per_x_mesh_pnt / 2;
            uint8_t  y = y_offset + pixels_per_y_mesh_pnt / 2;
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++, y += pixels_per_y_mesh_pnt)
        if (PAGE_CONTAINS(y, y))
          for (uint8_t i = 0, x = sx; i < GRID_MAX_POINTS_X; i++, x += pixels_per_x_mesh_pnt)
            u8g.drawBox(x, y, 1, 1);

      // Fill in the Specified Mesh Point

      uint8_t inverted_y = GRID_MAX_POINTS_Y - y_plot - 1;  // The origin is typically in the lower right corner.  We need to
                                                            // invert the Y to get it to plot in the right location.

      const uint8_t by = y_offset + inverted_y * pixels_per_y_mesh_pnt;
      if (PAGE_CONTAINS(by, by + pixels_per_y_mesh_pnt))
        u8g.drawBox(
          x_offset + x_plot * pixels_per_x_mesh_pnt, by,
          pixels_per_x_mesh_pnt, pixels_per_y_mesh_pnt
        );

      // Put Relevant Text on Display

      // Show X and Y positions at top of screen
      u8g.setColorIndex(1);
      if (PAGE_UNDER(7)) {
        lcd_moveto(5, 7);
        lcd_put_u8str("X:");
        lcd_put_u8str(ftostr52(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x_plot]))));
        lcd_moveto(74, 7);
        lcd_put_u8str("Y:");
        lcd_put_u8str(ftostr52(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[y_plot]))));
      }

      // Print plot position
      if (PAGE_CONTAINS(LCD_PIXEL_HEIGHT - (INFO_FONT_HEIGHT - 1), LCD_PIXEL_HEIGHT)) {
        lcd_moveto(5, LCD_PIXEL_HEIGHT);
        lcd_put_wchar('(');
        u8g.print(x_plot);
        lcd_put_wchar(',');
        u8g.print(y_plot);
        lcd_put_wchar(')');

        // Show the location value
        lcd_moveto(74, LCD_PIXEL_HEIGHT);
        lcd_put_u8str("Z:");
        if (!isnan(ubl.z_values[x_plot][y_plot]))
          lcd_put_u8str(ftostr43sign(ubl.z_values[x_plot][y_plot]));
        else
          lcd_put_u8str_rom(PSTR(" -----"));
      }

    }

  #endif // AUTO_BED_LEVELING_UBL

#endif // ULTIPANEL

#endif // __ULTRALCD_IMPL_DOGM_H
