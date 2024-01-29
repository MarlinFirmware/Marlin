/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * @file    fontutf8.h
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL/BSD
 */

#include <U8glib-HAL.h>
#include "../utf8.h"

// the macro to indicate a UTF-8 string
// You should to save the C/C++ source in UTF-8 encoding!
// Once you change your UTF-8 strings, you need to call the script uxggenpages.sh to create the font data file fontutf8-data.h
#define _UxGT(a) a

typedef struct _uxg_fontinfo_t {
  uint16_t page;
  uint8_t begin;
  uint8_t end;
  uint16_t size;
  const u8g_fntpgm_uint8_t *fntdata;
} uxg_fontinfo_t;

int uxg_SetUtf8Fonts(const uxg_fontinfo_t * fntinfo, int number); // fntinfo is type of PROGMEM

unsigned int uxg_DrawLchar(u8g_t *pu8g, unsigned int x, unsigned int y, const lchar_t &ch, const pixel_len_t max_length);

unsigned int uxg_DrawUtf8Str(u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg, const pixel_len_t max_length);
unsigned int uxg_DrawUtf8StrP(u8g_t *pu8g, unsigned int x, unsigned int y, PGM_P utf8_msg, const pixel_len_t max_length);

int uxg_GetUtf8StrPixelWidth(u8g_t *pu8g, const char *utf8_msg);
int uxg_GetUtf8StrPixelWidthP(u8g_t *pu8g, PGM_P utf8_msg);

#define uxg_GetFont(puxg) ((puxg)->font)

#define _LANG_FONT_INFO(L) g_fontinfo_##L
#define LANG_FONT_INFO(L) _LANG_FONT_INFO(L)
