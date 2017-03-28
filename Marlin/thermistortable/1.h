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

// 100k bed thermistor
const short temptable_1[][2] PROGMEM = {
  {   23 * OVERSAMPLENR, 300 },
  {   25 * OVERSAMPLENR, 295 },
  {   27 * OVERSAMPLENR, 290 },
  {   28 * OVERSAMPLENR, 285 },
  {   31 * OVERSAMPLENR, 280 },
  {   33 * OVERSAMPLENR, 275 },
  {   35 * OVERSAMPLENR, 270 },
  {   38 * OVERSAMPLENR, 265 },
  {   41 * OVERSAMPLENR, 260 },
  {   44 * OVERSAMPLENR, 255 },
  {   48 * OVERSAMPLENR, 250 },
  {   52 * OVERSAMPLENR, 245 },
  {   56 * OVERSAMPLENR, 240 },
  {   61 * OVERSAMPLENR, 235 },
  {   66 * OVERSAMPLENR, 230 },
  {   71 * OVERSAMPLENR, 225 },
  {   78 * OVERSAMPLENR, 220 },
  {   84 * OVERSAMPLENR, 215 },
  {   92 * OVERSAMPLENR, 210 },
  {  100 * OVERSAMPLENR, 205 },
  {  109 * OVERSAMPLENR, 200 },
  {  120 * OVERSAMPLENR, 195 },
  {  131 * OVERSAMPLENR, 190 },
  {  143 * OVERSAMPLENR, 185 },
  {  156 * OVERSAMPLENR, 180 },
  {  171 * OVERSAMPLENR, 175 },
  {  187 * OVERSAMPLENR, 170 },
  {  205 * OVERSAMPLENR, 165 },
  {  224 * OVERSAMPLENR, 160 },
  {  245 * OVERSAMPLENR, 155 },
  {  268 * OVERSAMPLENR, 150 },
  {  293 * OVERSAMPLENR, 145 },
  {  320 * OVERSAMPLENR, 140 },
  {  348 * OVERSAMPLENR, 135 },
  {  379 * OVERSAMPLENR, 130 },
  {  411 * OVERSAMPLENR, 125 },
  {  445 * OVERSAMPLENR, 120 },
  {  480 * OVERSAMPLENR, 115 },
  {  516 * OVERSAMPLENR, 110 },
  {  553 * OVERSAMPLENR, 105 },
  {  591 * OVERSAMPLENR, 100 },
  {  628 * OVERSAMPLENR,  95 },
  {  665 * OVERSAMPLENR,  90 },
  {  702 * OVERSAMPLENR,  85 },
  {  737 * OVERSAMPLENR,  80 },
  {  770 * OVERSAMPLENR,  75 },
  {  801 * OVERSAMPLENR,  70 },
  {  830 * OVERSAMPLENR,  65 },
  {  857 * OVERSAMPLENR,  60 },
  {  881 * OVERSAMPLENR,  55 },
  {  903 * OVERSAMPLENR,  50 },
  {  922 * OVERSAMPLENR,  45 },
  {  939 * OVERSAMPLENR,  40 },
  {  954 * OVERSAMPLENR,  35 },
  {  966 * OVERSAMPLENR,  30 },
  {  977 * OVERSAMPLENR,  25 },
  {  985 * OVERSAMPLENR,  20 },
  {  993 * OVERSAMPLENR,  15 },
  {  999 * OVERSAMPLENR,  10 },
  { 1004 * OVERSAMPLENR,   5 },
  { 1008 * OVERSAMPLENR,   0 },
  { 1012 * OVERSAMPLENR,  -5 },
  { 1016 * OVERSAMPLENR, -10 },
  { 1020 * OVERSAMPLENR, -15 }
};
