/**
 * @file    lcdprint.h
 * @brief   LCD print api
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#pragma once

#include "fontutils.h"

#include "../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_LCD
  #include "dogm/u8g_fontutf8.h"
  typedef u8g_uint_t lcd_uint_t;
#else
  #define _UxGT(a) a
  typedef uint8_t lcd_uint_t;
#endif

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80u)

int lcd_glyph_height();

int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length);

/**
 * @brief Draw a UTF-8 string
 *
 * @param utf8_str : the UTF-8 string
 * @param max_length : the pixel length of the string allowed (or number of slots in HD44780)
 *
 * @return the pixel width
 *
 * Draw a UTF-8 string
 */
int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length);

/**
 * Set the print baseline position
 */
void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row);

/**
 * @brief Draw a ROM UTF-8 string
 *
 * @param utf8_str_P : the ROM UTF-8 string
 * @param max_length : the pixel length of the string allowed (or number of slots in HD44780)
 *
 * @return the pixel width
 *
 * Draw a ROM UTF-8 string
 */
int lcd_put_u8str_max_P(PGM_P utf8_str_P, pixel_len_t max_length);
inline int lcd_put_u8str_max_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P utf8_str_P, pixel_len_t max_length) {
  lcd_moveto(col, row);
  return lcd_put_u8str_max_P(utf8_str_P, max_length);
}

void lcd_put_int(const int i);
inline void lcd_put_int(const lcd_uint_t col, const lcd_uint_t row, const int i) {
  lcd_moveto(col, row);
  lcd_put_int(i);
}

inline int lcd_put_u8str_P(PGM_P const pstr) { return lcd_put_u8str_max_P(pstr, PIXEL_LEN_NOLIMIT); }
inline int lcd_put_u8str_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P const pstr) {
  lcd_moveto(col, row);
  return lcd_put_u8str_P(pstr);
}

lcd_uint_t lcd_put_u8str_ind_P(PGM_P const pstr, const uint8_t ind, const lcd_uint_t maxlen=LCD_WIDTH);
inline lcd_uint_t lcd_put_u8str_ind_P(const lcd_uint_t col, const lcd_uint_t row, PGM_P const pstr, const uint8_t ind, const lcd_uint_t maxlen=LCD_WIDTH) {
  lcd_moveto(col, row);
  return lcd_put_u8str_ind_P(pstr, ind, maxlen);
}

inline int lcd_put_u8str(const char* str) { return lcd_put_u8str_max(str, PIXEL_LEN_NOLIMIT); }
inline int lcd_put_u8str(const lcd_uint_t col, const lcd_uint_t row, PGM_P const str) {
  lcd_moveto(col, row);
  return lcd_put_u8str(str);
}

inline int lcd_put_wchar(const wchar_t c) { return lcd_put_wchar_max(c, PIXEL_LEN_NOLIMIT); }
inline int lcd_put_wchar(const lcd_uint_t col, const lcd_uint_t row, const wchar_t c) {
  lcd_moveto(col, row);
  return lcd_put_wchar(c);
}
