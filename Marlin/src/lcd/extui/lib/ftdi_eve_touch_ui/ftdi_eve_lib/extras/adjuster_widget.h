/*********************
 * adjuster_widget.h *
 *********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - Cocoa Press                          *
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
#include "../extended/screen_types.h"

void draw_adjuster_value(
  CommandProcessor& cmd,
  int16_t x, int16_t y, int16_t w, int16_t h,
  float value, progmem_str units = nullptr,
  int8_t width = 5, uint8_t precision = 1
);

void draw_adjuster(
  CommandProcessor& cmd,
  int16_t x, int16_t y, int16_t w, int16_t h,
  uint8_t tag,
  float value, progmem_str units = nullptr,
  int8_t width = 5, uint8_t precision = 1,
  draw_mode_t what = BOTH
);
