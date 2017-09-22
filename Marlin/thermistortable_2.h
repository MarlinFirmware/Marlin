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
// 200k ATC Semitec 204GT-2
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
//
const short temptable_2[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 848 },
  {   30 * OVERSAMPLENR, 300 }, // top rating 300C
  {   34 * OVERSAMPLENR, 290 },
  {   39 * OVERSAMPLENR, 280 },
  {   46 * OVERSAMPLENR, 270 },
  {   53 * OVERSAMPLENR, 260 },
  {   63 * OVERSAMPLENR, 250 },
  {   74 * OVERSAMPLENR, 240 },
  {   87 * OVERSAMPLENR, 230 },
  {  104 * OVERSAMPLENR, 220 },
  {  124 * OVERSAMPLENR, 210 },
  {  148 * OVERSAMPLENR, 200 },
  {  176 * OVERSAMPLENR, 190 },
  {  211 * OVERSAMPLENR, 180 },
  {  252 * OVERSAMPLENR, 170 },
  {  301 * OVERSAMPLENR, 160 },
  {  357 * OVERSAMPLENR, 150 },
  {  420 * OVERSAMPLENR, 140 },
  {  489 * OVERSAMPLENR, 130 },
  {  562 * OVERSAMPLENR, 120 },
  {  636 * OVERSAMPLENR, 110 },
  {  708 * OVERSAMPLENR, 100 },
  {  775 * OVERSAMPLENR,  90 },
  {  835 * OVERSAMPLENR,  80 },
  {  884 * OVERSAMPLENR,  70 },
  {  924 * OVERSAMPLENR,  60 },
  {  955 * OVERSAMPLENR,  50 },
  {  977 * OVERSAMPLENR,  40 },
  {  993 * OVERSAMPLENR,  30 },
  { 1004 * OVERSAMPLENR,  20 },
  { 1012 * OVERSAMPLENR,  10 },
  { 1016 * OVERSAMPLENR,   0 }
};
