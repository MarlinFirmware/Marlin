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

#include "../utf8.h"

#define NO_GLYPH  0xFF

/**
 * Marlin fonts with optional antialiasing. Fonts use unifont_t font header and glyph_t glyphs headers.
 * Number of glyphs (fontEndEncoding - fontStartEncoding) can not exceed 256 (TBD).
 * Some glyphs may be left undefined with NO_GLYPH
 */
#define FONT_MARLIN_GLYPHS      0x80
#define FONT_MARLIN_GLYPHS_1BPP 0x81
#define FONT_MARLIN_GLYPHS_2BPP 0x82
#define FONT_MARLIN_GLYPHS_4BPP 0x84

/**
 * TFT fonts with optional antialiasing. Fonts use unifont_t font header and uniglyph_t glyphs headers.
 * Each glyph is prepended with its unicode.
 * Designed to be used for Japanese, Korean, Simplified Chinese and Traditional Chinese glyphs.
 *
 * IMPORTANT NOTES:
 *   - glyphs fast search method REQUIRES glyphs to be ordered by unicode
 *   - last glyph's code MUST be fontEndEncoding
 */
#define FONT_MARLIN_HIEROGLYPHS      0xA0
#define FONT_MARLIN_HIEROGLYPHS_1BPP 0xA1
#define FONT_MARLIN_HIEROGLYPHS_2BPP 0xA2
#define FONT_MARLIN_HIEROGLYPHS_4BPP 0xA4

#include "fontdata/fontdata.h"

// TFT font with unicode support
typedef struct __attribute__((__packed__)) {
  uint8_t  format;
  uint8_t  capitalAHeight;    // Not really needed, but helps with data alignment for uint16_t variables
  uint16_t fontStartEncoding;
  uint16_t fontEndEncoding;
  int8_t   fontAscent;
  int8_t   fontDescent;
} unifont_t;

// TFT glyphs
typedef struct __attribute__((__packed__)) {
  uint8_t bbxWidth;
  uint8_t bbxHeight;
  uint8_t dataSize;
   int8_t dWidth;
   int8_t bbxOffsetX;
   int8_t bbxOffsetY;
} glyph_t;

// unicode-prepended TFT glyphs
typedef struct __attribute__((__packed__)) {
  uint16_t unicode;
   glyph_t glyph;
} uniglyph_t;

#define MAX_STRING_LENGTH   64

class TFT_String {
  private:
    static glyph_t *glyphs[256];
    static unifont_t *font_header;
    #if EXTRA_GLYPHS
      static uint8_t *glyphs_extra[EXTRA_GLYPHS];
      static unifont_t *font_header_extra;
      static uint16_t extra_count;
    #endif

    static uint16_t data[MAX_STRING_LENGTH + 1];
    static uint16_t span;   // in pixels

    static void add_character(const uint16_t character);
    static void eol() { data[length] = 0; }

  public:
    static uint8_t  length;  // in characters

    static void set_font(const uint8_t *font);
    static void add_glyphs(const uint8_t *font);

    static uint8_t  font_type() { return font_header->format; };
    static uint16_t font_ascent() { return font_header->fontAscent; }
    static uint16_t font_height() { return font_header->fontAscent - font_header->fontDescent; }

    static glyph_t *glyph(uint16_t character);
    static glyph_t *glyph(uint16_t *character) { return glyph(*character); }

    /**
     * @brief Set the string empty
     */
    static void set();

    /**
     * @brief Append an unicode character and EOL
     *
     * @param character The unicode character
     */
    static void add(const uint16_t character) { add_character(character); eol(); }
    static void set(const lchar_t &character) { set(); add(character); }

    /**
     * @brief Append / Set C-string
     *
     * @param cstr The string
     * @param max_len Character limit
     */
    static void add(const char *cstr, uint8_t max_len=MAX_STRING_LENGTH);
    static void set(const char *cstr, uint8_t max_len=MAX_STRING_LENGTH) { set(); add(cstr, max_len); }

    /**
     * @brief Append / Set F-string
     *
     * @param fstr The string
     * @param max_len Character limit
     */
    static void add(FSTR_P const fstr, uint8_t max_len=MAX_STRING_LENGTH) { add(FTOP(fstr), max_len); }
    static void set(FSTR_P const fstr, uint8_t max_len=MAX_STRING_LENGTH) { set(FTOP(fstr), max_len); }

    /**
     * @brief Append / Set C-string with optional substitution
     *
     * @param tpl A string with optional substitution
     * @param index An index
     * @param cstr An SRAM C-string to use for $ substitution
     * @param fstr A ROM F-string to use for $ substitution
     */
    static void add(const char *tpl, const int8_t index, const char *cstr=nullptr, FSTR_P const fstr=nullptr);
    static void set(const char *tpl, const int8_t index, const char *cstr=nullptr, FSTR_P const fstr=nullptr) { set(); add(tpl, index, cstr, fstr); }

    /**
     * @brief Append / Set F-string with optional substitution
     *
     * @param ftpl A ROM F-string with optional substitution
     * @param index An index
     * @param cstr An SRAM C-string to use for $ substitution
     * @param fstr A ROM F-string to use for $ substitution
     */
    static void add(FSTR_P const ftpl, const int8_t index, const char *cstr=nullptr, FSTR_P const fstr=nullptr) { add(FTOP(ftpl), index, cstr, fstr); }
    static void set(FSTR_P const ftpl, const int8_t index, const char *cstr=nullptr, FSTR_P const fstr=nullptr) { set(); add(ftpl, index, cstr, fstr); }

    // Common string operations
    static void trim(const uint16_t character=' ');
    static void rtrim(const uint16_t character=' ');
    static void ltrim(const uint16_t character=' ');
    static void truncate(const uint8_t maxlen) { if (length > maxlen) { length = maxlen; eol(); } }

    // Accessors
    static uint16_t *string() { return data; }
    static uint16_t width() { return span; }
    static uint16_t center(const uint16_t width) { return span > width ? 0 : (width - span) / 2; }
    static uint16_t vcenter(const uint16_t height) {
      const uint16_t mid = (height + font_header->capitalAHeight + 1) / 2;
      return mid > font_header->fontAscent ? mid - font_header->fontAscent : 0;
    }
};

extern TFT_String tft_string;
