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

// User-defined table 2
// Dummy Thermistor table.. It will ALWAYS read a fixed value.

const short temptable_999[][2] PROGMEM = {
  {  OV(   0), 240 },
  {  OV(   5), 235 },
  {  OV(  18), 230 },
  {  OV(  64), 220 },
  {  OV( 113), 210 },
  {  OV( 169), 200 },
  {  OV( 245), 190 },
  {  OV( 340), 180 },
  {  OV( 454), 170 },
  {  OV( 597), 160 },
  {  OV( 941), 140 },
  {  OV(1470), 120 },
  {  OV(2039), 100 },
  {  OV(2640),  80 },
  {  OV(3240),  60 },
  {  OV(3828),  40 },
  {  OV(4095),  28 }
};
