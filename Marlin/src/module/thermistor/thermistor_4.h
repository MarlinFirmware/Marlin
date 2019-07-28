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

// R25 = 10 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up, Generic 10k thermistor
const short temptable_4[][2] PROGMEM = {
  { OV(   1), 430 },
  { OV(  54), 137 },
  { OV( 107), 107 },
  { OV( 160),  91 },
  { OV( 213),  80 },
  { OV( 266),  71 },
  { OV( 319),  64 },
  { OV( 372),  57 },
  { OV( 425),  51 },
  { OV( 478),  46 },
  { OV( 531),  41 },
  { OV( 584),  35 },
  { OV( 637),  30 },
  { OV( 690),  25 },
  { OV( 743),  20 },
  { OV( 796),  14 },
  { OV( 849),   7 },
  { OV( 902),   0 },
  { OV( 955), -11 },
  { OV(1008), -35 }
};
