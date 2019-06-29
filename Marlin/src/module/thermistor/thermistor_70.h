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

// Stock BQ Hephestos 2 100k thermistor.
// Created on 29/12/2017 with an ambient temperature of 20C.
// ANENG AN8009 DMM with a K-type probe used for measurements.

// R25 = 100 kOhm, beta25 = 4100 K, 4.7 kOhm pull-up, bqh2 stock thermistor
const short temptable_70[][2] PROGMEM = {
  { OV(  18), 270 },
  { OV(  27), 248 },
  { OV(  34), 234 },
  { OV(  45), 220 },
  { OV(  61), 205 },
  { OV(  86), 188 },
  { OV( 123), 172 },
  { OV( 420), 110 },
  { OV( 590),  90 },
  { OV( 845),  56 },
  { OV( 970),  25 },
  { OV( 986),  20 },
  { OV( 994),  15 },
  { OV(1000),  10 },
  { OV(1005),   5 },
  { OV(1009),   0 } // safety
};
