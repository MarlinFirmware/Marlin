/*******************
 * font_bitmap.cpp *
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

#ifdef FTDI_EXTENDED

namespace FTDI {

  void write_rle_data(uint16_t addr, const uint8_t *data, size_t n) {
    for (; n > 2; n -= 2) {
      uint8_t count = pgm_read_byte(data++);
      uint8_t value = pgm_read_byte(data++);
      while (count--) CLCD::mem_write_8(addr++, value);
    }
  }

  void set_font_bitmap(CommandProcessor& cmd, CLCD::FontMetrics &fm, uint8_t handle) {
    cmd.cmd(BITMAP_HANDLE(handle));
    cmd.cmd(BITMAP_SOURCE(fm.ptr));
    cmd.bitmap_layout(fm.format, fm.stride, fm.height);
    cmd.bitmap_size(BILINEAR, BORDER, BORDER, fm.width, fm.height);
  }

  void ext_vertex2ii(CommandProcessor &cmd, int x, int y, uint8_t handle, uint8_t cell) {
    if (x < 0 || y < 0 || x > 511 || y > 511) {
      cmd.cmd(BITMAP_HANDLE(handle));
      cmd.cmd(CELL(cell));
      cmd.cmd(VERTEX2F(x * 16, y * 16));
    } else {
      cmd.cmd(VERTEX2II(x, y, handle, cell));
    }
  }

} // namespace FTDI

#endif // FTDI_EXTENDED
