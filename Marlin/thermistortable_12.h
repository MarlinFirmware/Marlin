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

// 100k 0603 SMD Vishay NTCS0603E3104FXT (4.7k pullup) (calibrated for Makibox hot bed)
const short temptable_12[][2] PROGMEM = {
  {   35 * OVERSAMPLENR, 180 }, // top rating 180C
  {  211 * OVERSAMPLENR, 140 },
  {  233 * OVERSAMPLENR, 135 },
  {  261 * OVERSAMPLENR, 130 },
  {  290 * OVERSAMPLENR, 125 },
  {  328 * OVERSAMPLENR, 120 },
  {  362 * OVERSAMPLENR, 115 },
  {  406 * OVERSAMPLENR, 110 },
  {  446 * OVERSAMPLENR, 105 },
  {  496 * OVERSAMPLENR, 100 },
  {  539 * OVERSAMPLENR,  95 },
  {  585 * OVERSAMPLENR,  90 },
  {  629 * OVERSAMPLENR,  85 },
  {  675 * OVERSAMPLENR,  80 },
  {  718 * OVERSAMPLENR,  75 },
  {  758 * OVERSAMPLENR,  70 },
  {  793 * OVERSAMPLENR,  65 },
  {  822 * OVERSAMPLENR,  60 },
  {  841 * OVERSAMPLENR,  55 },
  {  875 * OVERSAMPLENR,  50 },
  {  899 * OVERSAMPLENR,  45 },
  {  926 * OVERSAMPLENR,  40 },
  {  946 * OVERSAMPLENR,  35 },
  {  962 * OVERSAMPLENR,  30 },
  {  977 * OVERSAMPLENR,  25 },
  {  987 * OVERSAMPLENR,  20 },
  {  995 * OVERSAMPLENR,  15 },
  { 1001 * OVERSAMPLENR,  10 },
  { 1010 * OVERSAMPLENR,   0 },
  { 1023 * OVERSAMPLENR, -40 }
};
