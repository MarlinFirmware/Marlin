/**********************
 * western_char_set.h *
 **********************/

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

namespace FTDI {
  class WesternCharSet {
    private:
      static uint32_t bitmap_addr;
    public:
      static uint32_t load_data(uint32_t addr);
      static void load_bitmaps(CommandProcessor&);
      static bool render_glyph(CommandProcessor*, int &x, int &y, font_size_t, utf8_char_t);
  };
}
