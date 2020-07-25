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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

// R25 = 100 kOhm, beta25 = 4120 K, 4.7 kOhm pull-up, mendel-parts
const temp_entry_t temptable_3[] PROGMEM = {
  { OV(   1), 864 },
  { OV(  21), 300 },
  { OV(  25), 290 },
  { OV(  29), 280 },
  { OV(  33), 270 },
  { OV(  39), 260 },
  { OV(  46), 250 },
  { OV(  54), 240 },
  { OV(  64), 230 },
  { OV(  75), 220 },
  { OV(  90), 210 },
  { OV( 107), 200 },
  { OV( 128), 190 },
  { OV( 154), 180 },
  { OV( 184), 170 },
  { OV( 221), 160 },
  { OV( 265), 150 },
  { OV( 316), 140 },
  { OV( 375), 130 },
  { OV( 441), 120 },
  { OV( 513), 110 },
  { OV( 588), 100 },
  { OV( 734),  80 },
  { OV( 856),  60 },
  { OV( 938),  40 },
  { OV( 986),  20 },
  { OV(1008),   0 },
  { OV(1018), -20 }
};
