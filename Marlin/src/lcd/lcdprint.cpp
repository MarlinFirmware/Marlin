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
 * expand_u8str_P
 *
 * Expand a string with optional substitutions:
 *
 *   $ displays the clipped string given by fstr or cstr
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 *
 * Return the number of characters emitted
 */
lcd_uint_t expand_u8str_P(char * const outstr, PGM_P const ptpl, const int8_t ind, const char *cstr/*=nullptr*/, FSTR_P const fstr/*=nullptr*/, const lcd_uint_t maxlen/*=LCD_WIDTH*/) {
  const uint8_t *p = (uint8_t*)ptpl;
  char *o = outstr;
  int8_t n = maxlen;
  while (n > 0) {
    lchar_t wc;
    uint8_t *psc = (uint8_t *)p;
    p = get_utf8_value_cb(p, read_byte_rom, wc);
    if (!wc) break;
    if (wc == '{' || wc == '~' || wc == '*') {
      if (ind >= 0) {
        if (wc == '*') { *o++ = 'E'; n--; }
        if (n) {
          int8_t inum = ind + ((wc == '{') ? 0 : LCD_FIRST_TOOL);
          if (inum >= 10) {
            *o++ = ('0' + (inum / 10)); n--;
            inum %= 10;
          }
          if (n) { *o++ = '0' + inum; n--; }
        }
      }
      else {
        PGM_P const b = ind == -2 ? GET_TEXT(MSG_CHAMBER) : GET_TEXT(MSG_BED);
        strncpy_P(o, b, n);
        n -= utf8_strlen(o);
        o += strlen(o);
      }
      if (n > 0) {
        strncpy_P(o, (PGM_P)p, n);
        n -= utf8_strlen(o);
        o += strlen(o);
        break;
      }
    }
    else if (wc == '$' && fstr) {
      strncpy_P(o, FTOP(fstr), n);
      n -= utf8_strlen_P(FTOP(fstr));
      o += strlen(o);
    }
    else if (wc == '$' && cstr) {
      strncpy(o, cstr, n);
      n -= utf8_strlen(o);
      o += strlen(o);
    }
    else {
      if (wc == '@')
        *o++ = AXIS_CHAR(ind);
      else
        while (psc != p) *o++ = read_byte_rom(psc++);
      *o = '\0';
      n--;
    }
  }
  return maxlen - n;
}

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
lcd_uint_t lcd_put_u8str_P(PGM_P const ptpl, const int8_t ind, const char *cstr/*=nullptr*/, FSTR_P const fstr/*=nullptr*/, const lcd_uint_t maxlen/*=LCD_WIDTH*/) {
  char estr[maxlen + 2];
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
