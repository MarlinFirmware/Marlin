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

/**
 * @file    lcdprint.h
 * @brief   LCD print api
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#pragma once

#include "utf8.h"

#include "../inc/MarlinConfig.h"

#if IS_DWIN_MARLINUI

  #include "e3v2/marlinui/marlinui_dwin.h"

  #define LCD_PIXEL_WIDTH     DWIN_WIDTH
  #define LCD_PIXEL_HEIGHT    DWIN_HEIGHT
  #define LCD_WIDTH           ((LCD_PIXEL_WIDTH)  / (MENU_FONT_WIDTH))
  #define LCD_HEIGHT          ((LCD_PIXEL_HEIGHT) / (MENU_LINE_HEIGHT))

  // The DWIN lcd_moveto function uses row / column, not pixels
  #define LCD_COL_X(col)    (col)
  #define LCD_ROW_Y(row)    (row)
  #define LCD_COL_X_RJ(len) (LCD_WIDTH - LCD_COL_X(len))

#elif HAS_MARLINUI_U8GLIB

  #include "dogm/u8g_fontutf8.h"
  typedef u8g_uint_t lcd_uint_t;
  typedef u8g_int_t lcd_int_t;

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
  #define INFO_FONT_WIDTH   6

  // Graphical LCD uses the menu font size for cursor positioning
  #define LCD_COL_X(col) ((    (col)) * (MENU_FONT_WIDTH))
  #define LCD_ROW_Y(row) ((1 + (row)) * (MENU_LINE_HEIGHT))

#else

  #define _UxGT(a) a
  typedef uint8_t lcd_uint_t;
  typedef int8_t lcd_int_t;

  #define MENU_FONT_WIDTH   1
  #define MENU_FONT_HEIGHT  1
  #define EDIT_FONT_WIDTH   1
  #define EDIT_FONT_HEIGHT  1
  #define INFO_FONT_WIDTH   1
  #define INFO_FONT_HEIGHT  1
  #define LCD_PIXEL_WIDTH   LCD_WIDTH
  #define LCD_PIXEL_HEIGHT  LCD_HEIGHT

  // Character LCD uses direct cursor positioning
  #define LCD_COL_X(col) (col)
  #define LCD_ROW_Y(row) (row)

#endif

#ifndef MENU_LINE_HEIGHT
  #define MENU_LINE_HEIGHT MENU_FONT_HEIGHT
#endif

#ifndef LCD_COL_X_RJ
  #define LCD_COL_X_RJ(len)    (LCD_PIXEL_WIDTH - LCD_COL_X(len))
#endif

#define SETCURSOR(col, row)    lcd_moveto(LCD_COL_X(col), LCD_ROW_Y(row))
#define SETCURSOR_RJ(len, row) lcd_moveto(LCD_COL_X_RJ(len), LCD_ROW_Y(row))
#define SETCURSOR_X(col)       SETCURSOR(col, _lcdLineNr)
#define SETCURSOR_X_RJ(len)    SETCURSOR_RJ(len, _lcdLineNr)

int lcd_glyph_height();

/**
 * @brief Draw a UTF-8 character
 *
 * @param utf8_str : the UTF-8 character
 * @param max_length : the output width limit (in pixels on GLCD)
 *
 * @return the output width (in pixels on GLCD)
 */
int lcd_put_lchar_max(const lchar_t &c, const pixel_len_t max_length);

/**
 * @brief Draw a SRAM UTF-8 string
 *
 * @param utf8_str : the UTF-8 string
 * @param max_length : the output width limit (in pixels on GLCD)
 *
 * @return the output width (in pixels on GLCD)
 */
int lcd_put_u8str_max(const char *utf8_str, const pixel_len_t max_length);

/**
 * Change the print cursor position
 */
void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row);

/**
 * @brief Draw a ROM UTF-8 string
 *
 * @param pstr : the ROM UTF-8 string
 * @param max_length : the output width limit (in pixels on GLCD)
 *
 * @return the output width (in pixels on GLCD)
 */
int lcd_put_u8str_max_P(PGM_P pstr, const pixel_len_t max_length);
inline int lcd_put_u8str_max_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P pstr, const pixel_len_t max_length) {
  lcd_moveto(col, row);
  return lcd_put_u8str_max_P(pstr, max_length);
}
inline int lcd_put_u8str_max(const lcd_uint_t col, const lcd_uint_t row, FSTR_P const fstr, const pixel_len_t max_length) {
  return lcd_put_u8str_max_P(col, row, FTOP(fstr), max_length);
}

/**
 * @brief Draw an integer, left-justified
 *
 * @param i : the integer
 */
void lcd_put_int(const int i);
inline void lcd_put_int(const lcd_uint_t col, const lcd_uint_t row, const int i) {
  lcd_moveto(col, row);
  lcd_put_int(i);
}

/**
 * @brief Draw a ROM UTF-8 string
 *
 * @param i : the integer
 */
inline int lcd_put_u8str_P(PGM_P const pstr) { return lcd_put_u8str_max_P(pstr, PIXEL_LEN_NOLIMIT); }
inline int lcd_put_u8str_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P const pstr) {
  lcd_moveto(col, row);
  return lcd_put_u8str_P(pstr);
}

/**
 * @brief Draw a ROM UTF-8 F-string
 *
 * @param fstr The F-string pointer
 * @return the output width (in pixels on GLCD)
 */
inline int lcd_put_u8str(FSTR_P const fstr) { return lcd_put_u8str_P(FTOP(fstr)); }
inline int lcd_put_u8str(const lcd_uint_t col, const lcd_uint_t row, FSTR_P const fstr) {
  return lcd_put_u8str_P(col, row, FTOP(fstr));
}

/**
 * @brief Draw a string with optional substitution
 * @details Print a string with optional substitutions:
 *   $ displays the clipped string given by fstr or cstr
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 *
 * @param ptpl A ROM string (template)
 * @param ind An index value to use for = ~ * substitution
 * @param cstr An SRAM C-string to use for $ substitution
 * @param fstr A ROM F-string to use for $ substitution
 * @param maxlen The maximum size of the string (in pixels on GLCD)
 * @return the output width (in pixels on GLCD)
 */
lcd_uint_t lcd_put_u8str_P(PGM_P const ptpl, const int8_t ind, const char *cstr=nullptr, FSTR_P const fstr=nullptr, const lcd_uint_t maxlen=LCD_WIDTH);
inline lcd_uint_t lcd_put_u8str_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P const ptpl, const int8_t ind, const char *cstr=nullptr, FSTR_P const fstr=nullptr, const lcd_uint_t maxlen=LCD_WIDTH) {
  lcd_moveto(col, row);
  return lcd_put_u8str_P(ptpl, ind, cstr, fstr, maxlen);
}
/**
 * @brief Draw a ROM UTF-8 F-string with optional substitution
 * @details (See above)
 *
 * @param ftpl A ROM F-string (template)
 * @param ind An index value to use for = ~ * substitution
 * @param cstr An SRAM C-string to use for $ substitution
 * @param fstr A ROM F-string to use for $ substitution
 * @param maxlen The maximum size of the string (in pixels on GLCD)
 * @return the output width (in pixels on GLCD)
 */
inline lcd_uint_t lcd_put_u8str(FSTR_P const ftpl, const int8_t ind, const char *cstr=nullptr, FSTR_P const fstr=nullptr, const lcd_uint_t maxlen=LCD_WIDTH) {
  return lcd_put_u8str_P(FTOP(ftpl), ind, cstr, fstr, maxlen);
}
/**
 * @param col
 * @param row
 */
inline lcd_uint_t lcd_put_u8str(const lcd_uint_t col, const lcd_uint_t row, FSTR_P const ftpl, const int8_t ind, const char *cstr=nullptr, FSTR_P const fstr=nullptr, const lcd_uint_t maxlen=LCD_WIDTH) {
  return lcd_put_u8str_P(col, row, FTOP(ftpl), ind, cstr, fstr, maxlen);
}

/**
 * @brief Draw a SRAM string with no width limit
 *
 * @param str The UTF-8 string
 * @return the output width (in pixels on GLCD)
 */
inline int lcd_put_u8str(const char * const str) { return lcd_put_u8str_max(str, PIXEL_LEN_NOLIMIT); }
/**
 * @param col
 * @param row
 */
inline int lcd_put_u8str(const lcd_uint_t col, const lcd_uint_t row, const char * const str) {
  lcd_moveto(col, row);
  return lcd_put_u8str(str);
}

/**
 * @brief Draw a UTF-8 character with no width limit
 *
 * @param c The lchar to draw
 * @return the output width (in pixels on GLCD)
 */
inline int lcd_put_lchar(const lchar_t &c) { return lcd_put_lchar_max(c, PIXEL_LEN_NOLIMIT); }
/**
 * @param col
 * @param row
 */
inline int lcd_put_lchar(const lcd_uint_t col, const lcd_uint_t row, const lchar_t &c) {
  lcd_moveto(col, row);
  return lcd_put_lchar(c);
}

/**
 * @brief Calculate the width of a ROM UTF-8 string (in pixels on GLCD)
 *
 * @param pstr The ROM-based UTF-8 string
 * @return the string width (in pixels on GLCD)
 */
int calculateWidth(PGM_P const pstr);
/**
 * @brief Calculate the width of a ROM UTF-8 string (in pixels on GLCD)
 *
 * @param pstr The ROM-based UTF-8 string
 * @return the string width (in pixels on GLCD)
 */
inline int calculateWidth(FSTR_P const fstr) { return calculateWidth(FTOP(fstr)); }
