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

// 100k Cartridge Thermistor (104GTA-2) (1K PULLUP -- NOT NORMAL)
// ATC Semitec 104GTA-2 (Use if 104GT-2 thermistor table is not giving correct readings)
// NOT VERIFIED EXPERIMENTALLY. Source: http://www.semitec.co.jp/uploads/english/sites/2/2017/03/P18-NT-Thermistor.pdf
// Listed as Product Number: 104NT-4-R025H43G by the source
// Calculated using 1.0kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_56[][2] PROGMEM = {
  {   67 * OVERSAMPLENR, 300 }, // top rating 300C
  {   77 * OVERSAMPLENR, 290 },
  {   87 * OVERSAMPLENR, 280 },
  {  100 * OVERSAMPLENR, 270 },
  {  115 * OVERSAMPLENR, 260 },
  {  133 * OVERSAMPLENR, 250 },
  {  154 * OVERSAMPLENR, 240 },
  {  179 * OVERSAMPLENR, 230 },
  {  208 * OVERSAMPLENR, 220 },
  {  242 * OVERSAMPLENR, 210 },
  {  281 * OVERSAMPLENR, 200 },
  {  326 * OVERSAMPLENR, 190 },
  {  378 * OVERSAMPLENR, 180 },
  {  434 * OVERSAMPLENR, 170 },
  {  496 * OVERSAMPLENR, 160 },
  {  561 * OVERSAMPLENR, 150 },
  {  627 * OVERSAMPLENR, 140 },
  {  693 * OVERSAMPLENR, 130 },
  {  755 * OVERSAMPLENR, 120 },
  {  811 * OVERSAMPLENR, 110 },
  {  860 * OVERSAMPLENR, 100 },
  {  901 * OVERSAMPLENR,  90 },
  {  934 * OVERSAMPLENR,  80 },
  {  960 * OVERSAMPLENR,  70 },
  {  979 * OVERSAMPLENR,  60 },
  {  993 * OVERSAMPLENR,  50 },
  { 1003 * OVERSAMPLENR,  40 },
  { 1010 * OVERSAMPLENR,  30 },
  { 1015 * OVERSAMPLENR,  20 },
  { 1018 * OVERSAMPLENR,  10 },
  { 1020 * OVERSAMPLENR,   0 }
};
