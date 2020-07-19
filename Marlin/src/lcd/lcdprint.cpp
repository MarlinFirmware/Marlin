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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * lcdprint.cpp
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_SPI_LCD

#include "lcdprint.h"

/**
 * lcd_put_u8str_ind_P
 * Print a string with an index substituted within it
 */
lcd_uint_t lcd_put_u8str_ind_P(PGM_P const pstr, const int8_t ind, PGM_P const inStr/*=nullptr*/, const lcd_uint_t maxlen/*=LCD_WIDTH*/) {
  uint8_t *p = (uint8_t*)pstr;
  int8_t n = maxlen;
  while (n > 0) {
    wchar_t ch;
    p = get_utf8_value_cb(p, read_byte_rom, &ch);
    if (!ch) break;
    if (ch == '=' || ch == '~' || ch == '*') {
      if (ind >= 0) {
        if (ch == '*') { lcd_put_wchar('E'); n--; }
        if (n) { lcd_put_wchar(ind + ((ch == '=') ? '0' : LCD_FIRST_TOOL)); n--; }
      }
      else {
        PGM_P const b = ind == -2 ? GET_TEXT(MSG_CHAMBER) : GET_TEXT(MSG_BED);
        n -= lcd_put_u8str_max_P(b, n * (MENU_FONT_WIDTH)) / (MENU_FONT_WIDTH);
      }
      if (n) n -= lcd_put_u8str_max_P((PGM_P)p, n * (MENU_FONT_WIDTH)) / (MENU_FONT_WIDTH);
      continue;
    }
    else if (ch == '$' && inStr) {
      n -= lcd_put_u8str_max_P(inStr, n * (MENU_FONT_WIDTH)) / (MENU_FONT_WIDTH);
      continue;
    }

    lcd_put_wchar(ch);
    n--;
  }
  return n;
}

#endif // HAS_SPI_LCD
