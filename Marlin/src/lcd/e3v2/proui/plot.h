/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * DWIN Single var plot
 * Author: Miguel A. Risco-Castillo
 * Version: 2.2.3
 * Date: 2023/01/29
 */

#include "dwinui.h"

class Plot {
public:
  static void draw(const frame_rect_t &frame, const_celsius_float_t max, const_celsius_float_t ref=0);
  static void update(const_celsius_float_t value);

private:
  static struct PlotData {
    uint16_t graphpoints, r, x2, y2, yP = 0;
    frame_rect_t graphframe = {0};
    float scale = 0;
  } data;
};

extern Plot plot;
