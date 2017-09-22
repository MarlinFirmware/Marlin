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

// 200k ATC Semitec 204GT-2 (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_52[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 500 },
  {  125 * OVERSAMPLENR, 300 }, // top rating 300C
  {  142 * OVERSAMPLENR, 290 },
  {  162 * OVERSAMPLENR, 280 },
  {  185 * OVERSAMPLENR, 270 },
  {  211 * OVERSAMPLENR, 260 },
  {  240 * OVERSAMPLENR, 250 },
  {  274 * OVERSAMPLENR, 240 },
  {  312 * OVERSAMPLENR, 230 },
  {  355 * OVERSAMPLENR, 220 },
  {  401 * OVERSAMPLENR, 210 },
  {  452 * OVERSAMPLENR, 200 },
  {  506 * OVERSAMPLENR, 190 },
  {  563 * OVERSAMPLENR, 180 },
  {  620 * OVERSAMPLENR, 170 },
  {  677 * OVERSAMPLENR, 160 },
  {  732 * OVERSAMPLENR, 150 },
  {  783 * OVERSAMPLENR, 140 },
  {  830 * OVERSAMPLENR, 130 },
  {  871 * OVERSAMPLENR, 120 },
  {  906 * OVERSAMPLENR, 110 },
  {  935 * OVERSAMPLENR, 100 },
  {  958 * OVERSAMPLENR,  90 },
  {  976 * OVERSAMPLENR,  80 },
  {  990 * OVERSAMPLENR,  70 },
  { 1000 * OVERSAMPLENR,  60 },
  { 1008 * OVERSAMPLENR,  50 },
  { 1013 * OVERSAMPLENR,  40 },
  { 1017 * OVERSAMPLENR,  30 },
  { 1019 * OVERSAMPLENR,  20 },
  { 1021 * OVERSAMPLENR,  10 },
  { 1022 * OVERSAMPLENR,   0 }
};
