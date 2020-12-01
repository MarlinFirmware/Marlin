/******************
 * font_bitmaps.h *
 ******************/

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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

class CommandProcessor;

namespace FTDI {
  void write_rle_data(uint16_t addr, const uint8_t *data, size_t n);
  void set_font_bitmap(CommandProcessor& cmd, CLCD::FontMetrics &fm, uint8_t handle);
  void ext_vertex2ii(CommandProcessor &cmd, int x, int y, uint8_t handle, uint8_t cell);
}
