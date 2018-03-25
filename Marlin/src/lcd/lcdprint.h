/**
 * @file    lcdprint.h
 * @brief   LCD print api
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#ifndef _LCDPRINT_H
#define _LCDPRINT_H

#include "fontutils.h"

#if DISABLED(DOGLCD)
  #define _UxGT(a) a
#else
  #include "u8g_fontutf8.h"
#endif

#define PRINTABLE(C) (((C) & 0xC0u) != 0x80u)

#ifdef __cplusplus
  extern "C" {
#endif

int lcd_glyph_height(void);

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
 * @brief Draw a ROM UTF-8 string
 *
 * @param utf8_str_P : the ROM UTF-8 string
 * @param max_length : the pixel length of the string allowed (or number of slots in HD44780)
 *
 * @return the pixel width
 *
 * Draw a ROM UTF-8 string
 */
int lcd_put_u8str_max_rom(const char * utf8_str_P, pixel_len_t max_length);

void lcd_moveto(int col, int row);

#ifdef __cplusplus
  }
#endif

#define lcd_put_u8str_rom(str) lcd_put_u8str_max_rom(str, PIXEL_LEN_NOLIMIT)

inline int lcd_put_u8str(const char* str) { return lcd_put_u8str_max(str, PIXEL_LEN_NOLIMIT); }

inline int lcd_put_wchar(wchar_t c) { return lcd_put_wchar_max(c, PIXEL_LEN_NOLIMIT); }

#endif // _LCDPRINT_H
