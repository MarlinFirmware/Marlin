/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// R25 = 100 kOhm, beta25 = 3960 K, 4.7 kOhm pull-up, RS thermistor 198-961
const short temptable_10[][2] PROGMEM = {
  { OV(   1), 929 },
  { OV(  36), 299 },
  { OV(  71), 246 },
  { OV( 106), 217 },
  { OV( 141), 198 },
  { OV( 176), 184 },
  { OV( 211), 173 },
  { OV( 246), 163 },
  { OV( 281), 154 },
  { OV( 316), 147 },
  { OV( 351), 140 },
  { OV( 386), 134 },
  { OV( 421), 128 },
  { OV( 456), 122 },
  { OV( 491), 117 },
  { OV( 526), 112 },
  { OV( 561), 107 },
  { OV( 596), 102 },
  { OV( 631),  97 },
  { OV( 666),  91 },
  { OV( 701),  86 },
  { OV( 736),  81 },
  { OV( 771),  76 },
  { OV( 806),  70 },
  { OV( 841),  63 },
  { OV( 876),  56 },
  { OV( 911),  48 },
  { OV( 946),  38 },
  { OV( 981),  23 },
  { OV(1005),   5 },
  { OV(1016),   0 }
};
