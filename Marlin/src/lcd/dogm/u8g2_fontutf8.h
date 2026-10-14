/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * @file    u8g2_fontutf8.h
 * @brief   UTF-8 font API for U8g2lib
 *
 * U8g2 fonts address glyphs by Unicode codepoint directly, so each language
 * needs just one font (fontdata_<lang>) holding every glyph above 0xFF used
 * by that language. Characters up to 0xFF come from the current menu font.
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8G2

#include <U8g2lib.h>
#include "../utf8.h"

// the macro to indicate a UTF-8 string
// You should to save the C/C++ source in UTF-8 encoding!
#define _UxGT(a) a

// The language font generated into fontdata/langdata_<lang>_u8g2.h
#define _LANG_FONT_INFO(L) fontdata_##L
#define LANG_FONT_INFO(L) _LANG_FONT_INFO(L)

// Set the font for characters above 0xFF. May be nullptr.
void uxg_SetUtf8Fonts(const uint8_t * const lang_font);

unsigned int uxg_DrawLchar(u8g2_t *pu8g2, unsigned int x, unsigned int y, const lchar_t &ch, const pixel_len_t max_width);
unsigned int uxg_DrawUtf8Str(u8g2_t *pu8g2, unsigned int x, unsigned int y, const char *utf8_msg, const pixel_len_t max_width);
unsigned int uxg_DrawUtf8StrP(u8g2_t *pu8g2, unsigned int x, unsigned int y, PGM_P utf8_msg, const pixel_len_t max_width);

int uxg_GetUtf8StrPixelWidth(u8g2_t *pu8g2, const char *utf8_msg);
int uxg_GetUtf8StrPixelWidthP(u8g2_t *pu8g2, PGM_P utf8_msg);

#endif // HAS_MARLINUI_U8G2
