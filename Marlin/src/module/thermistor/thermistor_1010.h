/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

// Pt1000 with 1k0 pullup
const temp_entry_t temptable_1010[] PROGMEM = {
  PtLine(  0, 1000, 1000),
  PtLine( 25, 1000, 1000),
  PtLine( 50, 1000, 1000),
  PtLine( 75, 1000, 1000),
  PtLine(100, 1000, 1000),
  PtLine(125, 1000, 1000),
  PtLine(150, 1000, 1000),
  PtLine(175, 1000, 1000),
  PtLine(200, 1000, 1000),
  PtLine(225, 1000, 1000),
  PtLine(250, 1000, 1000),
  PtLine(275, 1000, 1000),
  PtLine(300, 1000, 1000)
};
