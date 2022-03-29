/*******************
 * hilbert_curve.h *
 *******************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver Labs, Inc.                  *
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

class hilbert_curve {
  public:
    typedef bool (*callback_ptr)(uint8_t x, uint8_t y, void *data);
    static bool search(callback_ptr func, void *data);
    static bool search_from(uint8_t x, uint8_t y, callback_ptr func, void *data);
    static bool search_from_closest(const xy_pos_t &pos, callback_ptr func, void *data);
  private:
    static bool hilbert(int8_t x, int8_t y, int8_t xi, int8_t xj, int8_t yi, int8_t yj, uint8_t n, callback_ptr func, void *data);
};
