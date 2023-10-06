/*************
 * unicode.h *
 *************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

class CommandProcessor;

namespace FTDI {
  #if ENABLED(TOUCH_UI_USE_UTF8)
    typedef uint16_t utf8_char_t;

    /**
     * Converts a 32-bit codepoint into UTF-8. This compile-time function
     * will be useful until the u8'a' character literal becomes more common.
     */
    constexpr uint32_t utf8(const uint32_t c) {
      return (c < 0x7F )  ? c :
             (c < 0x7FF)  ? (0x0000C080 | ((c & 0b011111000000)             << 2) |  (c & 0b111111)) :
             (c < 0xFFFF) ? (0x00E08080 | ((c & 0b001111000000000000)       << 4) | ((c & 0b111111000000)       << 2) | (c & 0b111111)) :
                            (0xF0808080 | ((c & 0b000111000000000000000000) << 6) | ((c & 0b111111000000000000) << 4) | ((c & 0b111111000000) << 2) | (c & 0b111111));
    }

    /* Returns true if the string has UTF8 string characters */

    bool has_utf8_chars(FSTR_P str);
    bool has_utf8_chars(const char *str);

    /* Returns the next character in a UTF8 string and increments the
     * pointer to the next character */

    utf8_char_t get_utf8_char_and_inc(const char *&c);
    utf8_char_t get_utf8_char_and_inc(char *&c);

    /* Returns the next character in a UTF8 string, without incrementing */

    inline utf8_char_t get_utf8_char(const char *c) { return get_utf8_char_and_inc(c); }

    void load_utf8_data(uint32_t addr);
  #else
    typedef char utf8_char_t;

    inline utf8_char_t get_utf8_char_and_inc(const char *&c) { return *c++; }
    inline utf8_char_t get_utf8_char(const char *c) { return *c; }

    inline void load_utf8_data(uint32_t) {}
  #endif

  void load_utf8_bitmaps(CommandProcessor& cmd);

  uint16_t get_utf8_char_width(utf8_char_t, font_size_t);
  uint16_t get_utf8_text_width(FSTR_P, font_size_t);
  uint16_t get_utf8_text_width(const char *, font_size_t, size_t maxlen=SIZE_MAX);

  void draw_utf8_text(CommandProcessor&, int x, int y, FSTR_P, font_size_t, uint16_t options = 0);
  void draw_utf8_text(CommandProcessor&, int x, int y, const char *, font_size_t, uint16_t options = 0, size_t maxlen=SIZE_MAX);

  // Similar to CLCD::FontMetrics, but can be used with UTF8 encoded strings.

  struct FontMetrics {
    #if ENABLED(TOUCH_UI_USE_UTF8)
      font_size_t fs;
    #else
      CLCD::FontMetrics fm;
    #endif

    inline void load(uint8_t rom_font_size) {
      #if ENABLED(TOUCH_UI_USE_UTF8)
        fs = font_size_t::from_romfont(rom_font_size);
      #else
        fm.load(rom_font_size);
      #endif
    }

    inline uint16_t get_char_width(utf8_char_t c) const {
      #if ENABLED(TOUCH_UI_USE_UTF8)
        return get_utf8_char_width(c, fs);
      #else
        return fm.char_widths[(uint8_t)c];
      #endif
    }

    inline uint8_t get_height() const {
      #if ENABLED(TOUCH_UI_USE_UTF8)
        return fs.get_height();
      #else
        return fm.height;
      #endif
    }

    inline FontMetrics(uint8_t rom_font_size) {
      load(rom_font_size);
    }
  };
}
