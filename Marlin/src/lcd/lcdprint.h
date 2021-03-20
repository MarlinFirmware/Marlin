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

#include "../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD
  #include "dogm/u8g_fontutf8.h"
#else
  #define _UxGT(a) a
#endif

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80u)

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
int lcd_put_u8str_max_P(PGM_P utf8_str_P, pixel_len_t max_length);

void lcd_moveto(const uint8_t col, const uint8_t row);

void lcd_put_int(const int i);

inline int lcd_put_u8str_P(PGM_P str) { return lcd_put_u8str_max_P(str, PIXEL_LEN_NOLIMIT); }

inline int lcd_put_u8str(const char* str) { return lcd_put_u8str_max(str, PIXEL_LEN_NOLIMIT); }

inline int lcd_put_wchar(const wchar_t c) { return lcd_put_wchar_max(c, PIXEL_LEN_NOLIMIT); }
