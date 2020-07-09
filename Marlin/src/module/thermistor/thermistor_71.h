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

// R25 = 100 kOhm, beta25 = 3974 K, 4.7 kOhm pull-up, Honeywell 135-104LAF-J01
// R0 = 100000 Ohm
// T0 = 25 °C
// Beta = 3974
// R1 = 0 Ohm
// R2 = 4700 Ohm
const short temptable_71[][2] PROGMEM = {
  { OV(  35), 300 },
  { OV(  51), 269 },
  { OV(  59), 258 },
  { OV(  64), 252 },
  { OV(  71), 244 },
  { OV(  81), 235 },
  { OV(  87), 230 },
  { OV(  92), 226 },
  { OV( 102), 219 },
  { OV( 110), 214 },
  { OV( 115), 211 },
  { OV( 126), 205 },
  { OV( 128), 204 },
  { OV( 130), 203 },
  { OV( 132), 202 },
  { OV( 134), 201 },
  { OV( 136), 200 },
  { OV( 147), 195 },
  { OV( 154), 192 },
  { OV( 159), 190 },
  { OV( 164), 188 },
  { OV( 172), 185 },
  { OV( 175), 184 },
  { OV( 178), 183 },
  { OV( 181), 182 },
  { OV( 184), 181 },
  { OV( 187), 180 },
  { OV( 190), 179 },
  { OV( 193), 178 },
  { OV( 196), 177 },
  { OV( 199), 176 },
  { OV( 202), 175 },
  { OV( 205), 174 },
  { OV( 208), 173 },
  { OV( 215), 171 },
  { OV( 237), 165 },
  { OV( 256), 160 },
  { OV( 300), 150 },
  { OV( 351), 140 },
  { OV( 470), 120 },
  { OV( 504), 115 },
  { OV( 538), 110 },
  { OV( 745),  80 },
  { OV( 770),  76 },
  { OV( 806),  70 },
  { OV( 829),  66 },
  { OV( 860),  60 },
  { OV( 879),  56 },
  { OV( 888),  54 },
  { OV( 905),  50 },
  { OV( 924),  45 },
  { OV( 940),  40 },
  { OV( 955),  35 },
  { OV( 972),  28 },
  { OV( 974),  27 },
  { OV( 976),  26 },
  { OV( 978),  25 },
  { OV( 980),  24 },
  { OV( 987),  20 },
  { OV( 995),  15 },
  { OV(1001),  10 },
  { OV(1006),   5 },
  { OV(1010),   0 }
};
