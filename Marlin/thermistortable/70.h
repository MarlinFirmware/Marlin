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

// bqh2 stock thermistor
const short temptable_70[][2] PROGMEM = {
  {   22 * OVERSAMPLENR, 300 },
  {   24 * OVERSAMPLENR, 295 },
  {   25 * OVERSAMPLENR, 290 },
  {   27 * OVERSAMPLENR, 285 },
  {   29 * OVERSAMPLENR, 280 },
  {   32 * OVERSAMPLENR, 275 },
  {   34 * OVERSAMPLENR, 270 },
  {   37 * OVERSAMPLENR, 265 },
  {   40 * OVERSAMPLENR, 260 },
  {   43 * OVERSAMPLENR, 255 },
  {   46 * OVERSAMPLENR, 250 },
  {   50 * OVERSAMPLENR, 245 },
  {   54 * OVERSAMPLENR, 240 },
  {   59 * OVERSAMPLENR, 235 },
  {   64 * OVERSAMPLENR, 230 },
  {   70 * OVERSAMPLENR, 225 },
  {   76 * OVERSAMPLENR, 220 },
  {   83 * OVERSAMPLENR, 215 },
  {   90 * OVERSAMPLENR, 210 },
  {   99 * OVERSAMPLENR, 205 },
  {  108 * OVERSAMPLENR, 200 },
  {  118 * OVERSAMPLENR, 195 },
  {  129 * OVERSAMPLENR, 190 },
  {  141 * OVERSAMPLENR, 185 },
  {  154 * OVERSAMPLENR, 180 },
  {  169 * OVERSAMPLENR, 175 },
  {  185 * OVERSAMPLENR, 170 },
  {  203 * OVERSAMPLENR, 165 },
  {  222 * OVERSAMPLENR, 160 },
  {  243 * OVERSAMPLENR, 155 },
  {  266 * OVERSAMPLENR, 150 },
  {  290 * OVERSAMPLENR, 145 },
  {  317 * OVERSAMPLENR, 140 },
  {  346 * OVERSAMPLENR, 135 },
  {  376 * OVERSAMPLENR, 130 },
  {  408 * OVERSAMPLENR, 125 },
  {  442 * OVERSAMPLENR, 120 },
  {  477 * OVERSAMPLENR, 115 },
  {  513 * OVERSAMPLENR, 110 },
  {  551 * OVERSAMPLENR, 105 },
  {  588 * OVERSAMPLENR, 100 },
  {  626 * OVERSAMPLENR,  95 },
  {  663 * OVERSAMPLENR,  90 },
  {  699 * OVERSAMPLENR,  85 },
  {  735 * OVERSAMPLENR,  80 },
  {  768 * OVERSAMPLENR,  75 },
  {  800 * OVERSAMPLENR,  70 },
  {  829 * OVERSAMPLENR,  65 },
  {  856 * OVERSAMPLENR,  60 },
  {  881 * OVERSAMPLENR,  55 },
  {  903 * OVERSAMPLENR,  50 },
  {  922 * OVERSAMPLENR,  45 },
  {  939 * OVERSAMPLENR,  40 },
  {  954 * OVERSAMPLENR,  35 },
  {  966 * OVERSAMPLENR,  30 },
  {  977 * OVERSAMPLENR,  25 },
  {  986 * OVERSAMPLENR,  20 },
  {  994 * OVERSAMPLENR,  15 },
  { 1000 * OVERSAMPLENR,  10 },
  { 1005 * OVERSAMPLENR,   5 },
  { 1009 * OVERSAMPLENR,   0 } // safety
};
