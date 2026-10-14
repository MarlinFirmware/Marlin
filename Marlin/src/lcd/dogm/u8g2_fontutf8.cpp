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

/**
 * @file    u8g2_fontutf8.cpp
 * @brief   UTF-8 font support for U8g2lib
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8G2

#include "u8g2_fontutf8.h"
#include <limits.h>

static const uint8_t *lang_font = nullptr;

void uxg_SetUtf8Fonts(const uint8_t * const font) { lang_font = font; }

/**
 * @brief Select the font containing a glyph, preferring the language font above 0xFF
 *
 * @return the encoding to draw, or '?' (in the default font) if no font has the glyph
 */
static uint16_t select_glyph(u8g2_t *pu8g2, const uint8_t * const fnt_default, const lchar_t wc) {
  if (wc <= 0xFFFF) {
    if (wc > 0xFF && lang_font) {
      u8g2_SetFont(pu8g2, lang_font);
      if (u8g2_IsGlyph(pu8g2, wc)) return wc;
    }
    u8g2_SetFont(pu8g2, fnt_default);
    if (u8g2_IsGlyph(pu8g2, wc)) return wc;
  }
  u8g2_SetFont(pu8g2, fnt_default);
  return '?';
}

/**
 * @brief Draw (or just measure) one character
 *
 * @return pixels advanced, or 0 if the character won't fit in the remaining width
 */
static unsigned int process_lchar(u8g2_t *pu8g2, const uint8_t * const fnt_default, const unsigned int x, const unsigned int y, const lchar_t wc, const unsigned int max_width, const bool draw) {
  const uint16_t enc = select_glyph(pu8g2, fnt_default, wc);
  const int8_t w = u8g2_GetGlyphWidth(pu8g2, enc);
  if (w <= 0 || unsigned(w) > max_width) return 0;
  if (draw) u8g2_DrawGlyph(pu8g2, x, y, enc);
  return w;
}

/**
 * @brief Draw (or just measure) a UTF-8 string from RAM or ROM
 *
 * @return pixels advanced
 */
static unsigned int process_utf8(u8g2_t *pu8g2, const unsigned int x, const unsigned int y, const char *utf8_msg, read_byte_cb_t cb_read_byte, const pixel_len_t max_width, const bool draw) {
  const uint8_t * const fnt_default = pu8g2->font;
  const unsigned int limit = max_width == PIXEL_LEN_NOLIMIT ? UINT_MAX : max_width;
  unsigned int adv = 0;
  const uint8_t *p = (const uint8_t*)utf8_msg;
  for (;;) {
    lchar_t wc;
    p = get_utf8_value_cb(p, cb_read_byte, wc);
    if (!wc) break;
    const unsigned int w = process_lchar(pu8g2, fnt_default, x + adv, y, wc, limit - adv, draw);
    if (!w) break;
    adv += w;
  }
  u8g2_SetFont(pu8g2, fnt_default);
  return adv;
}

unsigned int uxg_DrawLchar(u8g2_t *pu8g2, unsigned int x, unsigned int y, const lchar_t &wc, const pixel_len_t max_width) {
  const uint8_t * const fnt_default = pu8g2->font;
  const unsigned int adv = process_lchar(pu8g2, fnt_default, x, y, wc, max_width == PIXEL_LEN_NOLIMIT ? UINT_MAX : max_width, true);
  u8g2_SetFont(pu8g2, fnt_default);
  return adv;
}

unsigned int uxg_DrawUtf8Str(u8g2_t *pu8g2, unsigned int x, unsigned int y, const char *utf8_msg, const pixel_len_t max_width) {
  return process_utf8(pu8g2, x, y, utf8_msg, read_byte_ram, max_width, true);
}

unsigned int uxg_DrawUtf8StrP(u8g2_t *pu8g2, unsigned int x, unsigned int y, PGM_P utf8_msg, const pixel_len_t max_width) {
  return process_utf8(pu8g2, x, y, utf8_msg, read_byte_rom, max_width, true);
}

int uxg_GetUtf8StrPixelWidth(u8g2_t *pu8g2, const char *utf8_msg) {
  return process_utf8(pu8g2, 0, 0, utf8_msg, read_byte_ram, PIXEL_LEN_NOLIMIT, false);
}

int uxg_GetUtf8StrPixelWidthP(u8g2_t *pu8g2, PGM_P utf8_msg) {
  return process_utf8(pu8g2, 0, 0, utf8_msg, read_byte_rom, PIXEL_LEN_NOLIMIT, false);
}

#endif // HAS_MARLINUI_U8G2
