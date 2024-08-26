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

#define REVERSE_TEMP_SENSOR_RANGE_1022 1

// Pt1000 with 1k0 pullup
constexpr temp_entry_t temptable_1022[] PROGMEM = {
  PtLine(  0, 1000, 2200),
  PtLine( 25, 1000, 2200),
  PtLine( 50, 1000, 2200),
  PtLine( 75, 1000, 2200),
  PtLine(100, 1000, 2200),
  PtLine(125, 1000, 2200),
  PtLine(150, 1000, 2200),
  PtLine(175, 1000, 2200),
  PtLine(200, 1000, 2200),
  PtLine(225, 1000, 2200),
  PtLine(250, 1000, 2200),
  PtLine(275, 1000, 2200),
  PtLine(300, 1000, 2200),
  PtLine(350, 1000, 2200),
  PtLine(400, 1000, 2200),
  PtLine(450, 1000, 2200),
  PtLine(500, 1000, 2200)
};
