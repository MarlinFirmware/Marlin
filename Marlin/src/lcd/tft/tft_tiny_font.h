/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Tiny digits font for small labels, e.g., Z values on a dense UBL mesh map.
 * Independent of the menu font: Canvas draws CANVAS_ADD_TINY_TEXT items with it.
 * Include after ui_common.h, which decides HAS_TFT_TINY_FONT.
 */

#include "../../inc/MarlinConfig.h"

#if HAS_TFT_TINY_FONT

#include "tft_string.h"

extern const uint8_t NotoSans_Medium_Digits[];

namespace TinyFont {

  inline const unifont_t* header() { return (const unifont_t*)NotoSans_Medium_Digits; }
  inline uint8_t  format()      { return header()->format; }
  inline int8_t   ascent()      { return header()->fontAscent; }
  inline uint8_t  height()      { return header()->fontAscent - header()->fontDescent; }
  inline uint8_t  digitHeight() { return header()->capitalAHeight; }

  // Walk the few glyphs to find one. Returns nullptr for unsupported characters.
  inline glyph_t* glyph(const uint16_t c) {
    const unifont_t * const f = header();
    if (!WITHIN(c, f->fontStartEncoding, f->fontEndEncoding)) return nullptr;
    const uint8_t bpp = f->format & 0x0F;
    uint8_t *p = (uint8_t *)NotoSans_Medium_Digits + sizeof(unifont_t);
    for (uint16_t u = f->fontStartEncoding; u <= c; ++u) {
      if (*p == NO_GLYPH) { if (u == c) return nullptr; ++p; continue; }
      glyph_t * const g = (glyph_t *)p;
      if (u == c) return g;
      p += sizeof(glyph_t) + glyph_data_size(g, bpp);
    }
    return nullptr;
  }

  inline uint16_t width(const char *s) {
    uint16_t w = 0;
    for (; *s; ++s) if (const glyph_t * const g = glyph(*s)) w += g->dWidth;
    return w;
  }

} // TinyFont

#endif // HAS_TFT_TINY_FONT
