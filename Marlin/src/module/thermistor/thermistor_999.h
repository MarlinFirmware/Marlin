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
  {  OV( 290), 250 },
  {  OV( 352), 240 },
  {  OV( 432), 230 },
  {  OV( 528), 220 },
  {  OV( 644), 210 },
  {  OV( 756), 200 },
  {  OV( 899), 190 },
  {  OV(1072), 180 },
  {  OV(1244), 170 },
  {  OV(1460), 160 },
  {  OV(1896), 140 },
  {  OV(2320), 120 },
  {  OV(2750), 100 },
  {  OV(3185),  80 },
  {  OV(3600),  60 },
  {  OV(3950),  40 },
  {  OV(4095),  28 }
};
