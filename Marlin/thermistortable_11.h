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

// QU-BD silicone bed QWG-104F-3950 thermistor
const short temptable_11[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 938 },
  {   31 * OVERSAMPLENR, 314 },
  {   41 * OVERSAMPLENR, 290 },
  {   51 * OVERSAMPLENR, 272 },
  {   61 * OVERSAMPLENR, 258 },
  {   71 * OVERSAMPLENR, 247 },
  {   81 * OVERSAMPLENR, 237 },
  {   91 * OVERSAMPLENR, 229 },
  {  101 * OVERSAMPLENR, 221 },
  {  111 * OVERSAMPLENR, 215 },
  {  121 * OVERSAMPLENR, 209 },
  {  131 * OVERSAMPLENR, 204 },
  {  141 * OVERSAMPLENR, 199 },
  {  151 * OVERSAMPLENR, 195 },
  {  161 * OVERSAMPLENR, 190 },
  {  171 * OVERSAMPLENR, 187 },
  {  181 * OVERSAMPLENR, 183 },
  {  191 * OVERSAMPLENR, 179 },
  {  201 * OVERSAMPLENR, 176 },
  {  221 * OVERSAMPLENR, 170 },
  {  241 * OVERSAMPLENR, 165 },
  {  261 * OVERSAMPLENR, 160 },
  {  281 * OVERSAMPLENR, 155 },
  {  301 * OVERSAMPLENR, 150 },
  {  331 * OVERSAMPLENR, 144 },
  {  361 * OVERSAMPLENR, 139 },
  {  391 * OVERSAMPLENR, 133 },
  {  421 * OVERSAMPLENR, 128 },
  {  451 * OVERSAMPLENR, 123 },
  {  491 * OVERSAMPLENR, 117 },
  {  531 * OVERSAMPLENR, 111 },
  {  571 * OVERSAMPLENR, 105 },
  {  611 * OVERSAMPLENR, 100 },
  {  641 * OVERSAMPLENR,  95 },
  {  681 * OVERSAMPLENR,  90 },
  {  711 * OVERSAMPLENR,  85 },
  {  751 * OVERSAMPLENR,  79 },
  {  791 * OVERSAMPLENR,  72 },
  {  811 * OVERSAMPLENR,  69 },
  {  831 * OVERSAMPLENR,  65 },
  {  871 * OVERSAMPLENR,  57 },
  {  881 * OVERSAMPLENR,  55 },
  {  901 * OVERSAMPLENR,  51 },
  {  921 * OVERSAMPLENR,  45 },
  {  941 * OVERSAMPLENR,  39 },
  {  971 * OVERSAMPLENR,  28 },
  {  981 * OVERSAMPLENR,  23 },
  {  991 * OVERSAMPLENR,  17 },
  { 1001 * OVERSAMPLENR,   9 },
  { 1021 * OVERSAMPLENR, -27 }
};
