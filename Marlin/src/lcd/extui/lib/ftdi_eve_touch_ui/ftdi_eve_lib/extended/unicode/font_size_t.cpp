/*******************
 * font_size_t.cpp *
 *******************/

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

namespace FTDI {
  // Returns the height of a standard FTDI romfont
  uint8_t font_size_t::get_romfont_height(uint8_t font) {
    static const uint8_t tbl[] PROGMEM = {
      8, 8, 16, 16, 13, 17, 20, 22, 29, 38, 16, 20, 25, 28, 36, 49, 63, 83, 108
    };
    return pgm_read_byte(&tbl[font - 16]);
  }

  // Sets the scaling coefficient to match a romfont size
  font_size_t font_size_t::from_romfont(uint8_t font) {
    return font_size_t(uint32_t(std_height) * 256 / get_romfont_height(font));
  }

  // Returns the height of the font
  uint8_t font_size_t::get_height() const {
    return scale(std_height);
  }
}

#endif // FTDI_EXTENDED && TOUCH_UI_USE_UTF8
