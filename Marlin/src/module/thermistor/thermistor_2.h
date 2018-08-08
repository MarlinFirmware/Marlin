/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

//
// R25 = 200 kOhm, beta25 = 4338 K, 4.7 kOhm pull-up, ATC Semitec 204GT-2
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
//
const short temptable_2[][2] PROGMEM = {
  { OV(   1), 848 },
  { OV(  30), 300 }, // top rating 300C
  { OV(  34), 290 },
  { OV(  39), 280 },
  { OV(  46), 270 },
  { OV(  53), 260 },
  { OV(  63), 250 },
  { OV(  74), 240 },
  { OV(  87), 230 },
  { OV( 104), 220 },
  { OV( 124), 210 },
  { OV( 148), 200 },
  { OV( 176), 190 },
  { OV( 211), 180 },
  { OV( 252), 170 },
  { OV( 301), 160 },
  { OV( 357), 150 },
  { OV( 420), 140 },
  { OV( 489), 130 },
  { OV( 562), 120 },
  { OV( 636), 110 },
  { OV( 708), 100 },
  { OV( 775),  90 },
  { OV( 835),  80 },
  { OV( 884),  70 },
  { OV( 924),  60 },
  { OV( 955),  50 },
  { OV( 977),  40 },
  { OV( 993),  30 },
  { OV(1004),  20 },
  { OV(1012),  10 },
  { OV(1016),   0 }
};
