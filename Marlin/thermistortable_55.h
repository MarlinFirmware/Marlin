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

// 100k ATC Semitec 104GT-2 (Used on ParCan) (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_55[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 500 },
  {   76 * OVERSAMPLENR, 300 },
  {   87 * OVERSAMPLENR, 290 },
  {  100 * OVERSAMPLENR, 280 },
  {  114 * OVERSAMPLENR, 270 },
  {  131 * OVERSAMPLENR, 260 },
  {  152 * OVERSAMPLENR, 250 },
  {  175 * OVERSAMPLENR, 240 },
  {  202 * OVERSAMPLENR, 230 },
  {  234 * OVERSAMPLENR, 220 },
  {  271 * OVERSAMPLENR, 210 },
  {  312 * OVERSAMPLENR, 200 },
  {  359 * OVERSAMPLENR, 190 },
  {  411 * OVERSAMPLENR, 180 },
  {  467 * OVERSAMPLENR, 170 },
  {  527 * OVERSAMPLENR, 160 },
  {  590 * OVERSAMPLENR, 150 },
  {  652 * OVERSAMPLENR, 140 },
  {  713 * OVERSAMPLENR, 130 },
  {  770 * OVERSAMPLENR, 120 },
  {  822 * OVERSAMPLENR, 110 },
  {  867 * OVERSAMPLENR, 100 },
  {  905 * OVERSAMPLENR,  90 },
  {  936 * OVERSAMPLENR,  80 },
  {  961 * OVERSAMPLENR,  70 },
  {  979 * OVERSAMPLENR,  60 },
  {  993 * OVERSAMPLENR,  50 },
  { 1003 * OVERSAMPLENR,  40 },
  { 1010 * OVERSAMPLENR,  30 },
  { 1015 * OVERSAMPLENR,  20 },
  { 1018 * OVERSAMPLENR,  10 },
  { 1020 * OVERSAMPLENR,   0 }
};
