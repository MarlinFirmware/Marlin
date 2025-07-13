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
 * lcdprint.cpp
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_LCDPRINT

#include "marlinui.h"
#include "lcdprint.h"

/**
 * lcd_put_u8str_P
 *
 * Print a string with optional substitutions:
 *
 *   $ displays the clipped string given by fstr or cstr
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 *
 * Return the number of characters emitted
 */
lcd_uint_t lcd_put_u8str_P(PGM_P const ptpl, const int8_t ind, const char *cstr/*=nullptr*/, FSTR_P const fstr/*=nullptr*/, const lcd_uint_t maxlen/*=MAX_MESSAGE_SIZE*/) {
  char estr[maxlen * LANG_CHARSIZE + 2];
  const lcd_uint_t outlen = expand_u8str_P(estr, ptpl, ind, cstr, fstr, maxlen);
  lcd_put_u8str_max(estr, maxlen * (MENU_FONT_WIDTH));
  return outlen;
}

// Calculate UTF8 width with a simple check
int calculateWidth(PGM_P const pstr) {
  if (!USE_WIDE_GLYPH) return utf8_strlen_P(pstr) * MENU_FONT_WIDTH;
  const uint8_t prop = 2;
  const uint8_t *p = (uint8_t*)pstr;
  int n = 0;

  do {
    lchar_t wc;
    p = get_utf8_value_cb(p, read_byte_rom, wc);
    if (!wc) break;
    n += (wc > 255) ? prop : 1;
  } while (1);

  return n * MENU_FONT_WIDTH;
}

#endif // HAS_LCDPRINT
