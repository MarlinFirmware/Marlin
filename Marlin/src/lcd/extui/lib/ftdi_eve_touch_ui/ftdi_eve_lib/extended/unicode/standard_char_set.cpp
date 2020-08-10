/*************************
 * standard_char_set.cpp *
 *************************/

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

#include "../ftdi_extended.h"

#if defined(FTDI_EXTENDED) && ENABLED(TOUCH_UI_USE_UTF8)

  constexpr static uint8_t std_font = 31;

  /* Lookup table of the char widths for standard ROMFONT 31 */

  uint8_t FTDI::StandardCharSet::std_char_width(char c) {
    static const uint8_t tbl[] PROGMEM = {
      10, 11, 15, 26, 25, 31, 26, 10, 15, 14, 18, 24,  9, 18, 11, 17, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 10, 10, 21, 23, 22, 20, 37, 27, 27, 26,
      28, 23, 22, 28, 29, 12, 23, 26, 22, 35, 29, 28, 26, 29, 27, 26, 26, 28,
      27, 36, 27, 26, 25, 12, 18, 12, 18, 21, 13, 23, 24, 22, 24, 22, 15, 24,
      24, 10, 11, 22, 10, 36, 24, 24, 24, 24, 15, 22, 14, 24, 21, 32, 21, 21,
      22, 15, 10, 15, 29, 10
    };
    return pgm_read_byte(&tbl[c - ' ']);
  }

  /**
   * Load bitmap data into RAMG. This function is called once at the start
   * of the program.
   *
   * Parameters:
   *
   *   addr  - Address in RAMG where the font data is written
   */

  void FTDI::StandardCharSet::load_data(uint32_t) {
  }

  /**
   * Populates the bitmap handles for the custom into the display list.
   * This function is called once at the start of each display list.
   *
   * Parameters:
   *
   *   cmd  - Object used for writing to the FTDI chip command queue.
   */

  void FTDI::StandardCharSet::load_bitmaps(CommandProcessor& cmd) {
    CLCD::FontMetrics std_fm(std_font);
    set_font_bitmap(cmd, std_fm, std_font);
  }

  /**
   * Renders a character at location x and y. The x position is incremented
   * by the width of the character.
   *
   * Parameters:
   *
   *   cmd  - If non-NULL the symbol is drawn to the screen.
   *          If NULL, only increment position for text measurement.
   *
   *   x, y - The location at which to draw the character. On output,
   *          incremented to the location of the next character.
   *
   *   fs   - A scaling object used to scale the font. The display will
   *          already be configured to scale bitmaps, but positions
   *          must be scaled using fs.scale()
   *
   *   c    - The unicode code point to draw. If the renderer does not
   *          support the character, it should draw nothing.
   */

  bool FTDI::StandardCharSet::render_glyph(CommandProcessor* cmd, int &x, int &y, font_size_t fs, utf8_char_t c) {
    uint8_t which = (c >= ' ' && c < 128) ? c : '?';
    uint8_t width = std_char_width(which);

    if (c == '\t') {
      // Special handling for the tab character
      which = ' ';
      width = std_char_width(' ');
    }

    // Draw the character
    if (cmd) ext_vertex2ii(*cmd, x, y, std_font, which);

    // Increment X to the next character position
    x += fs.scale(width);
    return true;
  }

#endif // FTDI_EXTENDED && TOUCH_UI_USE_UTF8
