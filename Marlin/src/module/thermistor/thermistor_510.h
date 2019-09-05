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

// R25 = 100 kOhm, beta25 = ??? K, 4.7 kOhm pull-up
// Unknown 100k thermistor shipped with E3D clone.
const short temptable_510[][2] PROGMEM = {
  { OV(   1), 713 },
  { OV(  20), 300 }, // top rating 300C
  { OV(  25), 290 },
  { OV(  30), 280 },
  { OV(  36), 270 },
  { OV(  41), 260 },
  { OV(  49), 250 },
  { OV(  58), 240 },
  { OV(  68), 230 },
  { OV(  81), 220 },
  { OV(  96), 210 },
  { OV( 114), 200 },
  { OV( 137), 190 },
  { OV( 163), 180 },
  { OV( 194), 170 },
  { OV( 232), 160 },
  { OV( 277), 150 },
  { OV( 327), 140 },
  { OV( 386), 130 },
  { OV( 448), 120 },
  { OV( 520), 110 },
  { OV( 592), 100 },
  { OV( 665),  90 },
  { OV( 736),  80 },
  { OV( 800),  70 },
  { OV( 857),  60 },
  { OV( 902),  50 },
  { OV( 927),  40 },
  { OV( 965),  30 },
  { OV( 986),  20 },
  { OV(1000),  10 },
  { OV(1010),   0 }
};
