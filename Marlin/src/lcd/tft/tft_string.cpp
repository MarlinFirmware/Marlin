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
#include "../utf8.h"
#include "../marlinui.h"

//#define DEBUG_TFT_FONT
#define DEBUG_OUT ENABLED(DEBUG_TFT_FONT)
#include "../../core/debug_out.h"

glyph_t *TFT_String::glyphs[256];
unifont_t *TFT_String::font_header;
#if EXTRA_GLYPHS
  uint8_t *TFT_String::glyphs_extra[EXTRA_GLYPHS];
  unifont_t *TFT_String::font_header_extra;
  uint16_t TFT_String::extra_count;
#endif


uint16_t TFT_String::data[];
uint16_t TFT_String::span;
uint8_t TFT_String::length;

void TFT_String::set_font(const uint8_t *font) {
  font_header = (unifont_t *)font;
  uint16_t glyph;

  for (glyph = 0; glyph < 256; glyph++) glyphs[glyph] = nullptr;

  #if EXTRA_GLYPHS
    font_header_extra = nullptr;
    extra_count = 0;

    for (glyph = 0; glyph < EXTRA_GLYPHS; glyph++) glyphs_extra[glyph] = nullptr;
  #endif

  DEBUG_ECHOLNPGM("Format: ",            ((unifont_t *)font_header)->Format);
  DEBUG_ECHOLNPGM("capitalAHeight: ",    ((unifont_t *)font_header)->capitalAHeight);
  DEBUG_ECHOLNPGM("fontStartEncoding: ", ((unifont_t *)font_header)->fontStartEncoding);
  DEBUG_ECHOLNPGM("fontEndEncoding: ",   ((unifont_t *)font_header)->fontEndEncoding);
  DEBUG_ECHOLNPGM("fontAscent: ",        ((unifont_t *)font_header)->fontAscent);
  DEBUG_ECHOLNPGM("fontDescent: ",       ((unifont_t *)font_header)->fontDescent);

  add_glyphs(font);
}

void TFT_String::add_glyphs(const uint8_t *font) {
  uint16_t unicode, fontStartEncoding, fontEndEncoding;
  uint8_t *pointer;

  fontStartEncoding = ((unifont_t *)font)->fontStartEncoding;
  fontEndEncoding = ((unifont_t *)font)->fontEndEncoding;
  pointer = (uint8_t *)font + sizeof(unifont_t);

  if (fontEndEncoding < 0x0100) { // base and symbol fonts
    for (unicode = fontStartEncoding; unicode <= fontEndEncoding; unicode++) {
      if (*pointer != NO_GLYPH) {
        glyphs[unicode] = (glyph_t *)pointer;
        pointer += sizeof(glyph_t) + ((glyph_t *)pointer)->dataSize;
      }
      else
        pointer++;
    }
  }

  #if EXTRA_GLYPHS
    if (fontStartEncoding >= 0x0100) {
      font_header_extra = (unifont_t *)font;
      if (((*font) & 0xF0) == FONT_MARLIN_GLYPHS ) {  // FONT_MARLIN_GLYPHS
        for (unicode = fontStartEncoding; unicode <= fontEndEncoding; unicode++) {
            if (unicode == fontStartEncoding + EXTRA_GLYPHS) {
              DEBUG_ECHOLNPGM("Too many glyphs. Increase EXTRA_GLYPHS to ", fontEndEncoding - fontStartEncoding + 1);
              break;
            }
            if (*pointer != NO_GLYPH) {
              glyphs_extra[unicode - fontStartEncoding] = pointer;
              pointer += sizeof(glyph_t) + ((glyph_t *)pointer)->dataSize;
            }
            else
              pointer++;
        }
      }
      else {  // FONT_MARLIN_HIEROGLYPHS
        for (uint16_t i = 0;; i++) {
          if (i == EXTRA_GLYPHS) {
            DEBUG_ECHOLNPGM("Too many glyphs. Increase EXTRA_GLYPHS");
            break;
          }
          glyphs_extra[i] = pointer;
          unicode = *(uint16_t *) pointer;
          pointer += sizeof(uniglyph_t) + ((uniglyph_t *)pointer)->glyph.dataSize;
          extra_count = i + 1;
          if (unicode == fontEndEncoding)
            break;
        }
      }
    }
  #endif
}

glyph_t *TFT_String::glyph(uint16_t character) {
  if (character == 0x2026) character = 0x0a;  /* character 0x2026 "…" is remaped to 0x0a and should be part of symbols font */
  if (character < 0x00ff) return glyphs[character] ?: glyphs['?'];    /* Use '?' for unknown glyphs */

  #if EXTRA_GLYPHS
    if (font_header_extra == nullptr || character < font_header_extra->fontStartEncoding || character > font_header_extra->fontEndEncoding) return glyphs['?'];

    if ((font_header_extra->Format & 0xF0) == FONT_MARLIN_GLYPHS) {
      if (glyphs_extra[character - font_header_extra->fontStartEncoding])
        return (glyph_t *)glyphs_extra[character - font_header_extra->fontStartEncoding];
    }
    else {
      #if 0
        // Slow search method that that does not care if glyphs are ordered by unicode
        for (uint16_t i = 0; i < extra_count; i++) {
          if (character == ((uniglyph_t *)glyphs_extra[i])->unicode)
            return &(((uniglyph_t *)glyphs_extra[i])->glyph);
        }
      #else
        // Fast search method that REQUIRES glyphs to be ordered by unicode
        uint16_t min = 0, max = extra_count-1, next = extra_count/2;
        /**
         * while() condition check has to be at the end of the loop to support fonts with single glyph
         * Technically it is not a error and it causes no harm, so let it be
         */
        do {
          uint16_t unicode = ((uniglyph_t *)glyphs_extra[next])->unicode;
          if (character == unicode)
            return &(((uniglyph_t *)glyphs_extra[next])->glyph);

          if (character > unicode) {
            if (next == min) break;
            min = next;
            next = (min + max + 1) / 2;
          }
          else {
            max = next;
            next = (min + max) / 2;
          }
        } while (min < max);
      #endif
    }
  #endif

  return glyphs['?'];
}

void TFT_String::set() {
  *data = 0x00;
  span = 0;
  length = 0;
}

/**
 * Add a string, applying substitutions for the following characters:
 *
 *   $ displays the string given by fstr or cstr
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */
void TFT_String::add(const char *tpl, const int8_t index, const char *cstr/*=nullptr*/, FSTR_P const fstr/*=nullptr*/) {
  lchar_t wc;

  while (*tpl) {
    tpl = get_utf8_value_cb(tpl, read_byte_ram, wc);
    const uint16_t ch = uint16_t(wc);

    if (ch == '{' || ch == '~' || ch == '*') {
      if (index >= 0) {
        int8_t inum = index + ((ch == '{') ? 0 : LCD_FIRST_TOOL);
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
      add_character(AXIS_CHAR(index));
    else
      add_character(ch);
  }
  eol();
}

void TFT_String::add(const char *cstr, uint8_t max_len/*=MAX_STRING_LENGTH*/) {
  lchar_t wc;
  while (*cstr && max_len) {
    cstr = get_utf8_value_cb(cstr, read_byte_ram, wc);
    const uint16_t ch = uint16_t(wc);
    add_character(ch);
    max_len--;
  }
  eol();
}

void TFT_String::add_character(const uint16_t character) {
  // Combining Diacritical Marks are not supported
  if (character >= 0x0300 && character <= 0x036f)
    return;

  if (length < MAX_STRING_LENGTH) {
    data[length] = character;
    length++;
    span += glyph(character)->dWidth;
  }
}

void TFT_String::rtrim(const uint16_t character) {
  while (length) {
    if (data[length - 1] == 0x20 || data[length - 1] == character) {
      length--;
      span -= glyph(data[length])->dWidth;
      eol();
    }
    else
      break;
  }
}

void TFT_String::ltrim(const uint16_t character) {
  uint16_t i, j;
  for (i = 0; (i < length) && (data[i] == 0x20 || data[i] == character); i++) {
    span -= glyph(data[i])->dWidth;
  }
  if (i == 0) return;
  for (j = 0; i < length; data[j++] = data[i++]);
  length = j;
  eol();
}

void TFT_String::trim(const uint16_t character) {
  rtrim(character);
  ltrim(character);
}

TFT_String tft_string;

#endif // HAS_GRAPHICAL_TFT
