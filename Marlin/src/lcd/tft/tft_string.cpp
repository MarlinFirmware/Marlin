/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "tft_string.h"
#include "../fontutils.h"

glyph_t *TFT_String::glyphs[256];
font_t *TFT_String::font_header;

uint8_t TFT_String::data[];
uint16_t TFT_String::span;
uint16_t TFT_String::length;

void TFT_String::set_font(const uint8_t *font) {
  font_header = (font_t *)font;
  uint32_t glyph;

  for (glyph = 0; glyph < 256; glyph++) glyphs[glyph] = NULL;
/*
  SERIAL_ECHO("Format: ");
  SERIAL_ECHOLN(font_header->Format);
  SERIAL_ECHO("BBXWidth: ");
  SERIAL_ECHOLN(font_header->BBXWidth);
  SERIAL_ECHO("BBXHeight: ");
  SERIAL_ECHOLN(font_header->BBXHeight);
  SERIAL_ECHO("BBXOffsetX: ");
  SERIAL_ECHOLN(font_header->BBXOffsetX);
  SERIAL_ECHO("BBXOffsetY: ");
  SERIAL_ECHOLN(font_header->BBXOffsetY);
  SERIAL_ECHO("CapitalAHeight: ");
  SERIAL_ECHOLN(font_header->CapitalAHeight);
  SERIAL_ECHO("Encoding65Pos: ");
  SERIAL_ECHOLN(font_header->Encoding65Pos);
  SERIAL_ECHO("Encoding97Pos: ");
  SERIAL_ECHOLN(font_header->Encoding97Pos);
  SERIAL_ECHO("FontStartEncoding: ");
  SERIAL_ECHOLN(font_header->FontStartEncoding);
  SERIAL_ECHO("FontEndEncoding: ");
  SERIAL_ECHOLN(font_header->FontEndEncoding);
  SERIAL_ECHO("LowerGDescent: ");
  SERIAL_ECHOLN(font_header->LowerGDescent);
  SERIAL_ECHO("FontAscent: ");
  SERIAL_ECHOLN(font_header->FontAscent);
  SERIAL_ECHO("FontDescent: ");
  SERIAL_ECHOLN(font_header->FontDescent);
  SERIAL_ECHO("FontXAscent: ");
  SERIAL_ECHOLN(font_header->FontXAscent);
  SERIAL_ECHO("FontXDescent: ");
  SERIAL_ECHOLN(font_header->FontXDescent);
*/
  add_glyphs(font);
}

void TFT_String::add_glyphs(const uint8_t *font) {
  uint32_t glyph;
  uint8_t *pointer = (uint8_t *)font + sizeof(font_t);

  for (glyph = ((font_t *)font)->FontStartEncoding; glyph <= ((font_t *)font)->FontEndEncoding; glyph++) {
    if (*pointer != NO_GLYPH) {
      glyphs[glyph] = (glyph_t *)pointer;
      pointer += sizeof(glyph_t) + ((glyph_t *)pointer)->DataSize;
    } else {
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

void TFT_String::add(uint8_t *string, uint8_t index) {
  uint8_t character;
  wchar_t wchar;

  while(*string) {
    string = get_utf8_value_cb(string, read_byte, &wchar);
    if (wchar > 255)
      wchar |= 0x0080;
    character = (uint8_t) (wchar & 0x00FF);

    if (character == '=' || character == '~' || character == '*') {
      if (character == '*')
        add_character('E');
      add_character(index + ((character == '=') ? '0' : LCD_FIRST_TOOL));
      break;
    }
    add_character(character);
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
    } else {
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
