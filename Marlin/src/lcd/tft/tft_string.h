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

#include "../fontutils.h"

#define NO_GLYPH  0xFF

/*
 * Marlin fonts with optional antialiasing. Fonts use unifont_t font header and glyph_t glyphs headers.
 * Number of glyphs (FontEndEncoding - FontStartEncoding) can not exceed 256 (TBD).
 * Some glyphs may be left undefined with NO_GLYPH
 */
#define FONT_MARLIN_GLYPHS      0x80
#define FONT_MARLIN_GLYPHS_1BPP 0x81
#define FONT_MARLIN_GLYPHS_2BPP 0x82
#define FONT_MARLIN_GLYPHS_4BPP 0x84

/*
 * TFT fonts with optional antialiasing. Fonts use unifont_t font header and uniglyph_t glyphs headers.
 * Each glyph is prepended with its unicode.
 * Designed to be used for Japanese, Korean, Simplified Chinese and Traditional Chinese glyphs.
 *
 * IMPORTANT NOTES:
 *   - glyphs fast search method REQUIRES glyphs to be ordered by unicode
 *   - last glyph's code MUST be FontEndEncoding
 */
#define FONT_MARLIN_HIEROGLYPHS      0xA0
#define FONT_MARLIN_HIEROGLYPHS_1BPP 0xA1
#define FONT_MARLIN_HIEROGLYPHS_2BPP 0xA2
#define FONT_MARLIN_HIEROGLYPHS_4BPP 0xA4

#define _LATIN_EXTENDED_A     1
#define _CYRILLIC             2
#define _GREEK                3
#define _KATAKANA             4
#define _KOREAN               5
#define _VIETNAMESE           6
#define _SIMPLIFIED_CHINESE   7
#define _TRADITIONAL_CHINESE  8

#define LCODE_cz      _LATIN_EXTENDED_A
#define LCODE_hr      _LATIN_EXTENDED_A
#define LCODE_pl      _LATIN_EXTENDED_A
#define LCODE_sk      _LATIN_EXTENDED_A
#define LCODE_tr      _LATIN_EXTENDED_A
#define LCODE_bg      _CYRILLIC
#define LCODE_ru      _CYRILLIC
#define LCODE_uk      _CYRILLIC
#define LCODE_el      _GREEK
#define LCODE_el_CY   _GREEK
#define LCODE_jp_kana _KATAKANA
#define LCODE_ko_KR   _KOREAN
#define LCODE_vi      _VIETNAMESE
#define LCODE_zh_CN   _SIMPLIFIED_CHINESE
#define LCODE_zh_TW   _TRADITIONAL_CHINESE

#define _LCODE(N) (CAT(LCODE_, LCD_LANGUAGE) == N)

#if _LCODE(_LATIN_EXTENDED_A)
  #define FONT_EXTRA    Latin_Extended_A
  #define EXTRA_GLYPHS  128
#elif _LCODE(_CYRILLIC)
  #define FONT_EXTRA    Cyrillic
  #define EXTRA_GLYPHS  145
#elif _LCODE(_GREEK)
  #define FONT_EXTRA    Greek
  #define EXTRA_GLYPHS  73
#elif _LCODE(_KATAKANA)
  #define FONT_EXTRA    Katakana
  #define EXTRA_GLYPHS  102
#elif _LCODE(_KOREAN)
  #define FONT_EXTRA    Korean
  #define EXTRA_GLYPHS  110
#elif _LCODE(_VIETNAMESE)
  #define FONT_EXTRA    Vietnamese
  #define EXTRA_GLYPHS  107
#elif _LCODE(_SIMPLIFIED_CHINESE)
  #define FONT_EXTRA    Simplified_Chinese
  #define EXTRA_GLYPHS  373
#elif _LCODE(_TRADITIONAL_CHINESE)
  #define FONT_EXTRA    Traditional_Chinese
  #define EXTRA_GLYPHS  307
#else // Basin Latin (0x0020 - 0x007f) and Latin-1 Supplement (0x0080-0x00ff) characters only
  #define EXTRA_GLYPHS  0
#endif

#undef _LCODE
#undef LCODE_cz
#undef LCODE_hr
#undef LCODE_pl
#undef LCODE_sk
#undef LCODE_tr
#undef LCODE_bg
#undef LCODE_ru
#undef LCODE_uk
#undef LCODE_el
#undef LCODE_el_CY
#undef LCODE_jp_kana
#undef LCODE_ko_KR
#undef LCODE_vi
#undef LCODE_zh_CN
#undef LCODE_zh_TW

#define NOTOSANS      1
#define UNIFONT       2
#define HELVETICA     3

#ifndef TFT_FONT
  #define TFT_FONT NOTOSANS
#endif

#if TFT_FONT == NOTOSANS
  #define FONT_FAMILY       NotoSans_Medium
#elif TFT_FONT == UNIFONT
  #define FONT_FAMILY       Unifont
#elif TFT_FONT == HELVETICA
  #define FONT_FAMILY       Helvetica
  #ifdef FONT_EXTRA
    #error "Helvetica font does not have symbols required for selected LCD_LANGUAGE."
  #endif
#else
  #error "Invalid TFT_FONT value."
#endif

// TFT font with unicode support
typedef struct __attribute__((__packed__)) {
  uint8_t  Format;
  uint8_t  CapitalAHeight;    // Not really needed, but helps with data alingment for uint16_t variables
  uint16_t FontStartEncoding;
  uint16_t FontEndEncoding;
   int8_t  FontAscent;
   int8_t  FontDescent;
} unifont_t;

// TFT glyphs
typedef struct __attribute__((__packed__)) {
  uint8_t BBXWidth;
  uint8_t BBXHeight;
  uint8_t DataSize;
   int8_t DWidth;
   int8_t BBXOffsetX;
   int8_t BBXOffsetY;
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

    static uint8_t  font_type() { return font_header->Format; };
    static uint16_t font_ascent() { return font_header->FontAscent; }
    static uint16_t font_height() { return font_header->FontAscent - font_header->FontDescent; }

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
    static uint16_t vcenter(const uint16_t height) { return (height + font_header->CapitalAHeight + 1) / 2 > font_header->FontAscent ? (height + font_header->CapitalAHeight + 1) / 2 - font_header->FontAscent : 0 ; }
};

extern TFT_String tft_string;
