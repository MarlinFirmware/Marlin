/****************
 * text_box.cpp *
 ****************/

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

#include "ftdi_extended.h"

#if ENABLED(FTDI_EXTENDED)

#define IS_LINE_SEPARATOR(c) c == '\n' || c == '\t'
#define IS_WORD_SEPARATOR(c) c == ' '
#define IS_SEPARATOR(c) IS_LINE_SEPARATOR(c) || IS_WORD_SEPARATOR(c)

namespace FTDI {
  /**
   * Given a str, end will be set to the position at which a line needs to
   * be broken so that the display width is less than w. The line will also
   * be broken after a '\n'. Returns the display width of the line.
   */
  static uint16_t find_line_break(const FontMetrics &utf8_fm, const CLCD::FontMetrics &clcd_fm, const uint16_t w, const char *start, const char *&end, bool use_utf8) {
    const char *p = start;
    end = start;
    uint16_t lw = 0, result = 0;
    for (;;) {
      const char *next = p;
      const utf8_char_t c = get_utf8_char_and_inc(next);
      // Decide whether to break the string at this location
      if (IS_SEPARATOR(c) || c == '\0' ) {
        end = p;
        result = lw;
      }
      if (IS_LINE_SEPARATOR(c) || c == '\0') break;
      // Measure the next character
      const uint16_t cw = use_utf8 ? utf8_fm.get_char_width(c) : clcd_fm.char_widths[(uint8_t)c];
      // Stop processing once string exceeds the display width
      if (lw + cw > w) break;
      // Now add the length of the current character to the tally.
      lw += cw;
      p = next;
    }
    if (end == start) {
      end = p;
      result = lw;
    }
    return result;
  }

  /**
   * This function returns a measurements of the word-wrapped text box.
   */
  static void measure_text_box(const FontMetrics &utf8_fm, const CLCD::FontMetrics &clcd_fm, const char *str, uint16_t &width, uint16_t &height, bool use_utf8) {
    const char *line_start = (const char*)str;
    const char *line_end;
    const uint16_t wrap_width = width;
    width = height = 0;
    for (;;) {
      const uint16_t line_width = find_line_break(utf8_fm, clcd_fm, wrap_width, line_start, line_end, use_utf8);
      width  = max(width, line_width);
      height += utf8_fm.get_height();
      if (IS_SEPARATOR(*line_end)) line_end++;
      if (*line_end == '\0') break;
      if (line_end == line_start) break;
      line_start = line_end;
    }
  }

  /**
   * This function draws text inside a bounding box, doing word wrapping and using the largest font that will fit.
   */
  void draw_text_box(CommandProcessor& cmd, int x, int y, int w, int h, const char *str, uint16_t options, uint8_t font) {
    #if ENABLED(TOUCH_UI_USE_UTF8)
      const bool use_utf8 = has_utf8_chars(str);
    #else
      constexpr bool use_utf8 = false;
    #endif
    uint16_t box_width, box_height;

    FontMetrics utf8_fm(font);
    CLCD::FontMetrics clcd_fm;
    clcd_fm.load(font);

    // Shrink the font until we find a font that fits
    for (;;) {
      box_width = w;
      measure_text_box(utf8_fm, clcd_fm, str, box_width, box_height, use_utf8);
      if (box_width <= (uint16_t)w && box_height <= (uint16_t)h) break;
      if (font == 26) break;
      utf8_fm.load(--font);
      clcd_fm.load(font);
    }

    const uint16_t dx = (options & OPT_RIGHTX) ? w :
                        (options & OPT_CENTERX) ? w / 2 : 0,
                   dy = (options & OPT_BOTTOMY) ? (h - box_height) :
                        (options & OPT_CENTERY) ? (h - box_height) / 2 : 0;

    const char *line_start = str, *line_end;
    for (;;) {
      find_line_break(utf8_fm, clcd_fm, w, line_start, line_end, use_utf8);

      const size_t line_len = line_end - line_start;
      if (line_len) {
        #if ENABLED(TOUCH_UI_USE_UTF8)
          if (use_utf8)
            draw_utf8_text(cmd, x + dx, y + dy, line_start, utf8_fm.fs, options & ~(OPT_CENTERY | OPT_BOTTOMY), line_len);
          else
        #endif
          {
            cmd.CLCD::CommandFifo::text(x + dx, y + dy, font, options & ~(OPT_CENTERY | OPT_BOTTOMY));
            cmd.CLCD::CommandFifo::str(line_start, line_len);
          }
      }
      y += utf8_fm.get_height();

      if (IS_SEPARATOR(*line_end)) line_end++;
      if (*line_end == '\0') break;
      if (line_end == line_start) break;
      line_start = line_end;
    }
  }

  void draw_text_box(CommandProcessor& cmd, int x, int y, int w, int h, FSTR_P fstr, uint16_t options, uint8_t font) {
    #ifdef __AVR__
      char str[strlen_P(FTOP(fstr)) + 1];
      strcpy_P(str, FTOP(fstr));
      draw_text_box(cmd, x, y, w, h, (const char*) str, options, font);
    #else
      draw_text_box(cmd, x, y, w, h, FTOP(fstr), options, font);
    #endif
  }
} // namespace FTDI

#endif // FTDI_EXTENDED
