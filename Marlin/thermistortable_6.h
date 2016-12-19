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

// 100k Epcos thermistor
const short temptable_6[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 350 },
  {   28 * OVERSAMPLENR, 250 }, // top rating 250C
  {   31 * OVERSAMPLENR, 245 },
  {   35 * OVERSAMPLENR, 240 },
  {   39 * OVERSAMPLENR, 235 },
  {   42 * OVERSAMPLENR, 230 },
  {   44 * OVERSAMPLENR, 225 },
  {   49 * OVERSAMPLENR, 220 },
  {   53 * OVERSAMPLENR, 215 },
  {   62 * OVERSAMPLENR, 210 },
  {   71 * OVERSAMPLENR, 205 }, // fitted graphically
  {   78 * OVERSAMPLENR, 200 }, // fitted graphically
  {   94 * OVERSAMPLENR, 190 },
  {  102 * OVERSAMPLENR, 185 },
  {  116 * OVERSAMPLENR, 170 },
  {  143 * OVERSAMPLENR, 160 },
  {  183 * OVERSAMPLENR, 150 },
  {  223 * OVERSAMPLENR, 140 },
  {  270 * OVERSAMPLENR, 130 },
  {  318 * OVERSAMPLENR, 120 },
  {  383 * OVERSAMPLENR, 110 },
  {  413 * OVERSAMPLENR, 105 },
  {  439 * OVERSAMPLENR, 100 },
  {  484 * OVERSAMPLENR,  95 },
  {  513 * OVERSAMPLENR,  90 },
  {  607 * OVERSAMPLENR,  80 },
  {  664 * OVERSAMPLENR,  70 },
  {  781 * OVERSAMPLENR,  60 },
  {  810 * OVERSAMPLENR,  55 },
  {  849 * OVERSAMPLENR,  50 },
  {  914 * OVERSAMPLENR,  45 },
  {  914 * OVERSAMPLENR,  40 },
  {  935 * OVERSAMPLENR,  35 },
  {  954 * OVERSAMPLENR,  30 },
  {  970 * OVERSAMPLENR,  25 },
  {  978 * OVERSAMPLENR,  22 },
  { 1008 * OVERSAMPLENR,   3 },
  { 1023 * OVERSAMPLENR,   0 } // to allow internal 0 degrees C
};
