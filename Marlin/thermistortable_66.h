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

// DyzeDesign 500°C Thermistor
const short temptable_66[][2] PROGMEM = {
  {   17.5 * OVERSAMPLENR, 850 },
  {   17.9 * OVERSAMPLENR, 500 },
  {   21.7 * OVERSAMPLENR, 480 },
  {   26.6 * OVERSAMPLENR, 460 },
  {   33.1 * OVERSAMPLENR, 440 },
  {   41.0 * OVERSAMPLENR, 420 },
  {   52.3 * OVERSAMPLENR, 400 },
  {   67.7 * OVERSAMPLENR, 380 },
  {   86.5 * OVERSAMPLENR, 360 },
  {  112.0 * OVERSAMPLENR, 340 },
  {  147.2 * OVERSAMPLENR, 320 },
  {  194.0 * OVERSAMPLENR, 300 },
  {  254.3 * OVERSAMPLENR, 280 },
  {  330.2 * OVERSAMPLENR, 260 },
  {  427.9 * OVERSAMPLENR, 240 },
  {  533.4 * OVERSAMPLENR, 220 },
  {  646.5 * OVERSAMPLENR, 200 },
  {  754.4 * OVERSAMPLENR, 180 },
  {  844.3 * OVERSAMPLENR, 160 },
  {  911.7 * OVERSAMPLENR, 140 },
  {  958.6 * OVERSAMPLENR, 120 },
  {  988.8 * OVERSAMPLENR, 100 },
  { 1006.6 * OVERSAMPLENR,  80 },
  { 1015.8 * OVERSAMPLENR,  60 },
  { 1021.3 * OVERSAMPLENR,  30 },
  {   1023 * OVERSAMPLENR - 1, 25},
  {   1023 * OVERSAMPLENR,  20}
};
