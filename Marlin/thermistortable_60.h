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

// Maker's Tool Works Kapton Bed Thermistor
// ./createTemperatureLookup.py --r0=100000 --t0=25 --r1=0 --r2=4700 --beta=3950
// r0: 100000
// t0: 25
// r1: 0 (parallel with rTherm)
// r2: 4700 (series with rTherm)
// beta: 3950
// min adc: 1 at 0.0048828125 V
// max adc: 1023 at 4.9951171875 V
const short temptable_60[][2] PROGMEM = {
  {   51 * OVERSAMPLENR, 272 },
  {   61 * OVERSAMPLENR, 258 },
  {   71 * OVERSAMPLENR, 247 },
  {   81 * OVERSAMPLENR, 237 },
  {   91 * OVERSAMPLENR, 229 },
  {  101 * OVERSAMPLENR, 221 },
  {  131 * OVERSAMPLENR, 204 },
  {  161 * OVERSAMPLENR, 190 },
  {  191 * OVERSAMPLENR, 179 },
  {  231 * OVERSAMPLENR, 167 },
  {  271 * OVERSAMPLENR, 157 },
  {  311 * OVERSAMPLENR, 148 },
  {  351 * OVERSAMPLENR, 140 },
  {  381 * OVERSAMPLENR, 135 },
  {  411 * OVERSAMPLENR, 130 },
  {  441 * OVERSAMPLENR, 125 },
  {  451 * OVERSAMPLENR, 123 },
  {  461 * OVERSAMPLENR, 122 },
  {  471 * OVERSAMPLENR, 120 },
  {  481 * OVERSAMPLENR, 119 },
  {  491 * OVERSAMPLENR, 117 },
  {  501 * OVERSAMPLENR, 116 },
  {  511 * OVERSAMPLENR, 114 },
  {  521 * OVERSAMPLENR, 113 },
  {  531 * OVERSAMPLENR, 111 },
  {  541 * OVERSAMPLENR, 110 },
  {  551 * OVERSAMPLENR, 108 },
  {  561 * OVERSAMPLENR, 107 },
  {  571 * OVERSAMPLENR, 105 },
  {  581 * OVERSAMPLENR, 104 },
  {  591 * OVERSAMPLENR, 102 },
  {  601 * OVERSAMPLENR, 101 },
  {  611 * OVERSAMPLENR, 100 },
  {  621 * OVERSAMPLENR,  98 },
  {  631 * OVERSAMPLENR,  97 },
  {  641 * OVERSAMPLENR,  95 },
  {  651 * OVERSAMPLENR,  94 },
  {  661 * OVERSAMPLENR,  92 },
  {  671 * OVERSAMPLENR,  91 },
  {  681 * OVERSAMPLENR,  90 },
  {  691 * OVERSAMPLENR,  88 },
  {  701 * OVERSAMPLENR,  87 },
  {  711 * OVERSAMPLENR,  85 },
  {  721 * OVERSAMPLENR,  84 },
  {  731 * OVERSAMPLENR,  82 },
  {  741 * OVERSAMPLENR,  81 },
  {  751 * OVERSAMPLENR,  79 },
  {  761 * OVERSAMPLENR,  77 },
  {  771 * OVERSAMPLENR,  76 },
  {  781 * OVERSAMPLENR,  74 },
  {  791 * OVERSAMPLENR,  72 },
  {  801 * OVERSAMPLENR,  71 },
  {  811 * OVERSAMPLENR,  69 },
  {  821 * OVERSAMPLENR,  67 },
  {  831 * OVERSAMPLENR,  65 },
  {  841 * OVERSAMPLENR,  63 },
  {  851 * OVERSAMPLENR,  62 },
  {  861 * OVERSAMPLENR,  60 },
  {  871 * OVERSAMPLENR,  57 },
  {  881 * OVERSAMPLENR,  55 },
  {  891 * OVERSAMPLENR,  53 },
  {  901 * OVERSAMPLENR,  51 },
  {  911 * OVERSAMPLENR,  48 },
  {  921 * OVERSAMPLENR,  45 },
  {  931 * OVERSAMPLENR,  42 },
  {  941 * OVERSAMPLENR,  39 },
  {  951 * OVERSAMPLENR,  36 },
  {  961 * OVERSAMPLENR,  32 },
  {  981 * OVERSAMPLENR,  23 },
  {  991 * OVERSAMPLENR,  17 },
  { 1001 * OVERSAMPLENR,   9 },
  { 1008 * OVERSAMPLENR,   0 }
};
