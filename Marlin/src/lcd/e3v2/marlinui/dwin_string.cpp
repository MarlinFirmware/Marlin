/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if IS_DWIN_MARLINUI

#include "dwin_string.h"
//#include "../../fontutils.h"

char DWIN_String::data[];
uint16_t DWIN_String::span;
uint8_t DWIN_String::length;

void DWIN_String::set() {
  //*data = 0x00;
  memset(data, 0x00, sizeof(data));
  span = 0;
  length = 0;
}

uint8_t read_byte(const uint8_t *byte) { return *byte; }

/**
 * Add a string, applying substitutions for the following characters:
 *
 *   $ displays the clipped string given by fstr or cstr
 *   = displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */
void DWIN_String::add(const char *tpl, const int8_t index, const char *cstr/*=nullptr*/, FSTR_P const fstr/*=nullptr*/) {
  lchar_t wc;

  while (*tpl) {
    tpl = get_utf8_value_cb(tpl, read_byte, wc);
    if (wc > 255) wc |= 0x0080;
    const uint8_t ch = uint8_t(wc & 0x00FF);

    if (ch == '=' || ch == '~' || ch == '*') {
      if (index >= 0) {
        int8_t inum = index + ((ch == '=') ? 0 : LCD_FIRST_TOOL);
        if (ch == '*') add_character('E');
        if (inum >= 10) { add_character('0' + (inum / 10)); inum %= 10; }
        add_character('0' + inum);
      }
      else
        add(index == -2 ? GET_TEXT_F(MSG_CHAMBER) : GET_TEXT_F(MSG_BED));
    }
    else if (ch == '$' && fstr)
      add(fstr);
    else if (ch == '$' && cstr)
      add(cstr);
    else if (ch == '@')
      add_character(axis_codes[index]);
    else
      add_character(ch);
  }
  eol();
}

void DWIN_String::add(const char *cstr, uint8_t max_len/*=MAX_STRING_LENGTH*/) {
  lchar_t wc;
  while (*cstr && max_len) {
    cstr = get_utf8_value_cb(cstr, read_byte, wc);
    /*
    if (wc > 255) wc |= 0x0080;
    const uint8_t ch = uint8_t(wc & 0x00FF);
    add_character(ch);
    */
    add(wc);
    max_len--;
  }
  eol();
}

void DWIN_String::add(const lchar_t &wc) {
  int ret;
  size_t idx = 0;
  dwin_charmap_t pinval;
  dwin_charmap_t *copy_address = nullptr;
  pinval.uchar = wc;
  pinval.idx = -1;

  // For 8-bit ASCII just print the single character
  char str[] = { '?', 0 };
  if (wc < 255) {
    str[0] = (char)wc;
  }
  else {
    copy_address = nullptr;
    ret = pf_bsearch_r((void *)g_dwin_charmap_device, COUNT(g_dwin_charmap_device), pf_bsearch_cb_comp_dwinmap_pgm, (void *)&pinval, &idx);
    if (ret >= 0) {
      copy_address = (dwin_charmap_t*)(g_dwin_charmap_device + idx);
    }
    else {
      ret = pf_bsearch_r((void *)g_dwin_charmap_common, COUNT(g_dwin_charmap_common), pf_bsearch_cb_comp_dwinmap_pgm, (void *)&pinval, &idx);
      if (ret >= 0)
        copy_address = (dwin_charmap_t*)(g_dwin_charmap_common + idx);
    }
    if (ret >= 0) {
      dwin_charmap_t localval;
      memcpy_P(&localval, copy_address, sizeof(localval));
      str[0] = localval.idx;
      str[1] = localval.idx2;
    }
  }
  if (str[0]) add_character(str[0]);
  if (str[1]) add_character(str[1]);
}

void DWIN_String::add_character(const char character) {
  if (length < MAX_STRING_LENGTH) {
    data[length] = character;
    length++;
    //span += glyph(character)->DWidth;
  }
}

void DWIN_String::rtrim(const char character) {
  while (length) {
    if (data[length - 1] == 0x20 || data[length - 1] == character) {
      length--;
      //span -= glyph(data[length])->DWidth;
      eol();
    }
    else
      break;
  }
}

void DWIN_String::ltrim(const char character) {
  uint16_t i, j;
  for (i = 0; (i < length) && (data[i] == 0x20 || data[i] == character); i++) {
    //span -= glyph(data[i])->DWidth;
  }
  if (i == 0) return;
  for (j = 0; i < length; data[j++] = data[i++]);
  length = j;
  eol();
}

void DWIN_String::trim(const char character) {
  rtrim(character);
  ltrim(character);
}

/* return v1 - v2 */
int dwin_charmap_compare(dwin_charmap_t *v1, dwin_charmap_t *v2) {
  return (v1->uchar < v2->uchar) ? -1 : (v1->uchar > v2->uchar) ? 1 : 0;
}

int pf_bsearch_cb_comp_dwinmap_pgm(void *userdata, size_t idx, void * data_pin) {
  dwin_charmap_t localval;
  dwin_charmap_t *p_dwin_charmap = (dwin_charmap_t *)userdata;
  memcpy_P(&localval, p_dwin_charmap + idx, sizeof(localval));
  return dwin_charmap_compare(&localval, (dwin_charmap_t *)data_pin);
}

DWIN_String dwin_string;

#endif // IS_DWIN_MARLINUI
