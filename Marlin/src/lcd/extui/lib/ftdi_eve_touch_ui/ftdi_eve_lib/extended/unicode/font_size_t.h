/*****************
 * font_size_t.h *
 *****************/

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

#pragma once

class CommandProcessor;

namespace FTDI {

  /* The unicode rendering of different font sizes happens by scaling a
   * large-sized font bitmap using the FTDI bitmap transformation matrix.
   * This keeps us from having to have load bitmaps for all font sizes.
   *
   * The font_size_t class helps manage this scaling factor.
   */
  class font_size_t {
    private:
      // Standard height for font bitmaps
      static constexpr uint8_t std_height = 49;

      // 8.8 fixed point scaling coefficient
      uint16_t coefficient;

      font_size_t(uint16_t v) : coefficient(v) {}
    public:
      font_size_t() : coefficient(256) {}

      static uint8_t get_romfont_height(uint8_t font);

      static font_size_t from_romfont(uint8_t size);

      template<typename T> T scale(T val) const {return (int32_t(val) * 256 / coefficient);}

      uint8_t get_height() const;
      uint16_t get_coefficient() const {return coefficient;}
  };
}
