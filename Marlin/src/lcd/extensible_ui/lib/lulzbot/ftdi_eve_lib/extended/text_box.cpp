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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "ftdi_extended.h"

#ifdef FTDI_EXTENDED

namespace FTDI {
  /**
   * Given a str, end will be set to the position at which a line needs to
   * be broken so that the display width is less than w. The line will also
   * be broken after a '\n'. Returns the display width of the line.
   */
  static uint16_t find_line_break(const CLCD::FontMetrics &fm, uint16_t w, const char *str, const char *&end) {
    const char *p = str;
    end = str + strlen(str);
    uint16_t width = fm.get_text_width(str);
    for(;;) {
      // Find next tentative line break.
      char delim = *(p);
      while (delim && delim != ' ' && delim != '\n') {
        delim = *(++p);
      }
      // Check to see whether to break the line.
      const uint16_t margin = fm.get_text_width("  ");
      const uint16_t lw = p > str ? fm.get_text_width(str, p - str) + margin : 0;
      if (lw < w) {
        width = lw;
        switch (delim) {
          case '\0':
            end = p;
            break;
          case '\n':
            end = ++p;
            break;
          case ' ':
            end = ++p;
            continue;
        }
      }
      return width;
    }
  }

  /**
   * This function returns a measurements of the word-wrapped text box.
   */
  static void measure_text_box(const CLCD::FontMetrics &fm, const char *str, uint16_t &width, uint16_t &height) {
    const char *line_start = (const char*)str;
    const char *line_end;
    const uint16_t wrap_width = width;
    width = height = 0;
    for(;;) {
      uint16_t line_width = find_line_break(fm, wrap_width, line_start, line_end);
      if (line_end == line_start) break;
      width  = max(width, line_width);
      height += fm.height;
      line_start = line_end;
    }
  }

  /**
   * This function draws text inside a bounding box, doing word wrapping and using the largest font that will fit.
   */
  void draw_text_box(CommandProcessor& cmd, int x, int y, int w, int h, const char *str, uint16_t options, uint8_t font) {
    CLCD::FontMetrics fm(font);

    uint16_t box_width, box_height;

    for(;;) {
      box_width = w;
      measure_text_box(fm, str, box_width, box_height);
      if (box_width <= (uint16_t)w && box_height <= (uint16_t)h) break;
      fm.load(--font);
      if (font == 26) break;
    }

    const uint16_t dx = (options & OPT_RIGHTX) ? w : (options & OPT_CENTERX) ? w/2 : 0;
    const uint16_t dy = (options & OPT_CENTERY) ? (h - box_height)/2 : 0;

    const char *line_start = str;
    const char *line_end;
    for(;;) {
      find_line_break(fm, w, line_start, line_end);
      if (line_end == line_start) break;

      const size_t line_len = line_end - line_start;
      if (line_len) {
        char line[line_len + 1];
        strncpy(line, line_start, line_len);
        line[line_len] = 0;
        if (line[line_len - 1] == '\n' || line[line_len - 1] == ' ')
          line[line_len - 1] = 0;

        cmd.CLCD::CommandFifo::text(x + dx, y + dy, font, options & ~OPT_CENTERY);
        cmd.CLCD::CommandFifo::str(line);
      }
      y += fm.height;

      line_start = line_end;
    }
  }

  void draw_text_box(CommandProcessor& cmd, int x, int y, int w, int h, progmem_str pstr, uint16_t options, uint8_t font) {
    char str[strlen_P((const char*)pstr) + 1];
    strcpy_P(str, (const char*)pstr);
    draw_text_box(cmd, x, y, w, h, (const char*) str, options, font);
  }
} // namespace FTDI

#endif // FTDI_EXTENDED
