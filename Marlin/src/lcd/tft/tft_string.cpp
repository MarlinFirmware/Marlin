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

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "tft_string.h"
#include "../fontutils.h"

//#define DEBUG_TFT_FONT
#define DEBUG_OUT ENABLED(DEBUG_TFT_FONT)
#include "../../core/debug_out.h"

glyph_t *TFT_String::glyphs[256];
font_t *TFT_String::font_header;

uint8_t TFT_String::data[];
uint16_t TFT_String::span;
uint16_t TFT_String::length;

void TFT_String::set_font(const uint8_t *font) {
  font_header = (font_t *)font;
  uint32_t glyph;

  for (glyph = 0; glyph < 256; glyph++) glyphs[glyph] = NULL;

  DEBUG_ECHOLNPAIR("Format: ", font_header->Format);
  DEBUG_ECHOLNPAIR("BBXWidth: ", font_header->BBXWidth);
  DEBUG_ECHOLNPAIR("BBXHeight: ", font_header->BBXHeight);
  DEBUG_ECHOLNPAIR("BBXOffsetX: ", font_header->BBXOffsetX);
  DEBUG_ECHOLNPAIR("BBXOffsetY: ", font_header->BBXOffsetY);
  DEBUG_ECHOLNPAIR("CapitalAHeight: ", font_header->CapitalAHeight);
  DEBUG_ECHOLNPAIR("Encoding65Pos: ", font_header->Encoding65Pos);
  DEBUG_ECHOLNPAIR("Encoding97Pos: ", font_header->Encoding97Pos);
  DEBUG_ECHOLNPAIR("FontStartEncoding: ", font_header->FontStartEncoding);
  DEBUG_ECHOLNPAIR("FontEndEncoding: ", font_header->FontEndEncoding);
  DEBUG_ECHOLNPAIR("LowerGDescent: ", font_header->LowerGDescent);
  DEBUG_ECHOLNPAIR("FontAscent: ", font_header->FontAscent);
  DEBUG_ECHOLNPAIR("FontDescent: ", font_header->FontDescent);
  DEBUG_ECHOLNPAIR("FontXAscent: ", font_header->FontXAscent);
  DEBUG_ECHOLNPAIR("FontXDescent: ", font_header->FontXDescent);

  add_glyphs(font);
}

void TFT_String::add_glyphs(const uint8_t *font) {
  uint32_t glyph;
  uint8_t *pointer = (uint8_t *)font + sizeof(font_t);

  for (glyph = ((font_t *)font)->FontStartEncoding; glyph <= ((font_t *)font)->FontEndEncoding; glyph++) {
    if (*pointer != NO_GLYPH) {
      glyphs[glyph] = (glyph_t *)pointer;
      pointer += sizeof(glyph_t) + ((glyph_t *)pointer)->DataSize;
    }
    else {
      pointer++;
    }
  }
}

void TFT_String::set() {
  *data = 0x00;
  span = 0;
  length = 0;
}

uint8_t read_byte(uint8_t *byte) { return *byte; }

void TFT_String::add(uint8_t *string, int8_t index, uint8_t *itemString) {
  wchar_t wchar;

  while (*string) {
    string = get_utf8_value_cb(string, read_byte, &wchar);
    if (wchar > 255) wchar |= 0x0080;
    uint8_t ch = uint8_t(wchar & 0x00FF);

    if (ch == '=' || ch == '~' || ch == '*') {
      if (index >= 0) {
        int8_t inum = index + ((ch == '=') ? 0 : LCD_FIRST_TOOL);
        if (ch == '*') add_character('E');
        if (inum >= 10) { add_character('0' + (inum / 10)); inum %= 10; }
        add_character('0' + inum);
      }
      else {
        add(index == -2 ? GET_TEXT(MSG_CHAMBER) : GET_TEXT(MSG_BED));
      }
      continue;
    }
    else if (ch == '$' && itemString) {
      add(itemString);
      continue;
    }

    add_character(ch);
  }
  eol();
}

void TFT_String::add(uint8_t *string) {
  wchar_t wchar;
  while (*string) {
    string = get_utf8_value_cb(string, read_byte, &wchar);
    if (wchar > 255) wchar |= 0x0080;
    uint8_t ch = uint8_t(wchar & 0x00FF);
    add_character(ch);
  }
  eol();
}

void TFT_String::add_character(uint8_t character) {
  if (length < MAX_STRING_LENGTH) {
    data[length] = character;
    length++;
    span += glyph(character)->DWidth;
  }
}

void TFT_String::rtrim(uint8_t character) {
  while (length) {
    if (data[length - 1] == 0x20 || data[length - 1] == character) {
      length--;
      span -= glyph(data[length])->DWidth;
      eol();
    }
    else {
      break;
    }
  }
}

void TFT_String::ltrim(uint8_t character) {
  uint16_t i, j;
  for (i = 0; (i < length) && (data[i] == 0x20 || data[i] == character); i++) {
    span -= glyph(data[i])->DWidth;
  }
  if (i == 0) return;
  for (j = 0; i < length; data[j++] = data[i++]);
  length = j;
  eol();
}

void TFT_String::trim(uint8_t character) {
  rtrim(character);
  ltrim(character);
}

TFT_String tft_string;

#endif // HAS_GRAPHICAL_TFT
