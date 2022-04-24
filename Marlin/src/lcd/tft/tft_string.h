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
#pragma once

#include <stdint.h>

extern const uint8_t ISO10646_1_5x7[];
extern const uint8_t font10x20[];

extern const uint8_t Helvetica12Bold[];

extern const uint8_t Helvetica14[], Helvetica14_symbols[];
extern const uint8_t Helvetica18[], Helvetica18_symbols[];

#define NO_GLYPH          0xFF

typedef struct __attribute__((__packed__)) {
  uint8_t Format;
  uint8_t BBXWidth;
  uint8_t BBXHeight;
   int8_t BBXOffsetX;
   int8_t BBXOffsetY;
  uint8_t CapitalAHeight;
 uint16_t Encoding65Pos;
 uint16_t Encoding97Pos;
  uint8_t FontStartEncoding;
  uint8_t FontEndEncoding;
   int8_t LowerGDescent;
   int8_t FontAscent;
   int8_t FontDescent;
   int8_t FontXAscent;
   int8_t FontXDescent;
} font_t;

typedef struct __attribute__((__packed__)) {
  uint8_t BBXWidth;
  uint8_t BBXHeight;
  uint8_t DataSize;
   int8_t DWidth;
   int8_t BBXOffsetX;
   int8_t BBXOffsetY;
} glyph_t;

#define MAX_STRING_LENGTH   64

class TFT_String {
  private:
    static glyph_t *glyphs[256];
    static font_t *font_header;

    static uint8_t data[MAX_STRING_LENGTH + 1];
    static uint16_t span;   // in pixels
    static uint8_t length;  // in characters

    static void add_character(uint8_t character);
    static void eol() { data[length] = 0x00; }

  public:
    static void set_font(const uint8_t *font);
    static void add_glyphs(const uint8_t *font);

    static font_t *font() { return font_header; };
    static uint16_t font_height() { return font_header->FontAscent - font_header->FontDescent; }
    static glyph_t *glyph(uint8_t character) { return glyphs[character] ?: glyphs[0x3F]; }  /* Use '?' for unknown glyphs */
    static glyph_t *glyph(uint8_t *character) { return glyph(*character); }

    static void set();
    static void add(uint8_t character) { add_character(character); eol(); }
    static void add(uint8_t *string, uint8_t max_len=MAX_STRING_LENGTH);
    static void add(uint8_t *string, int8_t index, uint8_t *itemString=nullptr);
    static void set(uint8_t *string) { set(); add(string); };
    static void set(uint8_t *string, int8_t index, const char *itemString=nullptr) { set(); add(string, index, (uint8_t *)itemString); };
    static void set(const char *string) { set((uint8_t *)string); }
    static void set(const char *string, int8_t index, const char *itemString=nullptr) { set((uint8_t *)string, index, itemString); }
    static void add(const char *string) { add((uint8_t *)string); }

    static void trim(uint8_t character=0x20);
    static void rtrim(uint8_t character=0x20);
    static void ltrim(uint8_t character=0x20);

    static void truncate(uint8_t maxlen) { if (length > maxlen) { length = maxlen; eol(); } }

    static uint16_t width() { return span; }
    static uint8_t *string() { return data; }
    static uint16_t center(uint16_t width) { return span > width ? 0 : (width - span) / 2; }
};

extern TFT_String tft_string;
