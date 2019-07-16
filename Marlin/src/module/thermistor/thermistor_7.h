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

// R25 = 100 kOhm, beta25 = 3974 K, 4.7 kOhm pull-up, Honeywell 135-104LAG-J01
const short temptable_7[][2] PROGMEM = {
  { OV(   1), 941 },
  { OV(  19), 362 },
  { OV(  37), 299 }, // top rating 300C
  { OV(  55), 266 },
  { OV(  73), 245 },
  { OV(  91), 229 },
  { OV( 109), 216 },
  { OV( 127), 206 },
  { OV( 145), 197 },
  { OV( 163), 190 },
  { OV( 181), 183 },
  { OV( 199), 177 },
  { OV( 217), 171 },
  { OV( 235), 166 },
  { OV( 253), 162 },
  { OV( 271), 157 },
  { OV( 289), 153 },
  { OV( 307), 149 },
  { OV( 325), 146 },
  { OV( 343), 142 },
  { OV( 361), 139 },
  { OV( 379), 135 },
  { OV( 397), 132 },
  { OV( 415), 129 },
  { OV( 433), 126 },
  { OV( 451), 123 },
  { OV( 469), 121 },
  { OV( 487), 118 },
  { OV( 505), 115 },
  { OV( 523), 112 },
  { OV( 541), 110 },
  { OV( 559), 107 },
  { OV( 577), 105 },
  { OV( 595), 102 },
  { OV( 613),  99 },
  { OV( 631),  97 },
  { OV( 649),  94 },
  { OV( 667),  92 },
  { OV( 685),  89 },
  { OV( 703),  86 },
  { OV( 721),  84 },
  { OV( 739),  81 },
  { OV( 757),  78 },
  { OV( 775),  75 },
  { OV( 793),  72 },
  { OV( 811),  69 },
  { OV( 829),  66 },
  { OV( 847),  62 },
  { OV( 865),  59 },
  { OV( 883),  55 },
  { OV( 901),  51 },
  { OV( 919),  46 },
  { OV( 937),  41 },
  { OV( 955),  35 },
  { OV( 973),  27 },
  { OV( 991),  17 },
  { OV(1009),   1 },
  { OV(1023),   0 } // to allow internal 0 degrees C
};
