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

 // 100k Cartridge Thermistor (104GTA-2)
 // ATC Semitec 104GTA-2 (Use if 104GT-2 thermistor table is not giving correct readings)
 // Verified by Reece Kibble. Source: http://www.semitec.co.jp/uploads/english/sites/2/2017/03/P18-NT-Thermistor.pdf
 // Listed as Product Number: 104NT-4-R025H43G by the source
 // Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
 const short temptable_53[][2] PROGMEM = {
   {    1 * OVERSAMPLENR, 640 },
   {   15 * OVERSAMPLENR, 300 }, // top rating 300C
   {   17 * OVERSAMPLENR, 290 },
   {   20 * OVERSAMPLENR, 280 },
   {   23 * OVERSAMPLENR, 270 },
   {   27 * OVERSAMPLENR, 260 },
   {   32 * OVERSAMPLENR, 250 },
   {   37 * OVERSAMPLENR, 240 },
   {   44 * OVERSAMPLENR, 230 },
   {   53 * OVERSAMPLENR, 220 },
   {   63 * OVERSAMPLENR, 210 },
   {   76 * OVERSAMPLENR, 200 },
   {   93 * OVERSAMPLENR, 190 },
   {  113 * OVERSAMPLENR, 180 },
   {  139 * OVERSAMPLENR, 170 },
   {  171 * OVERSAMPLENR, 160 },
   {  210 * OVERSAMPLENR, 150 },
   {  258 * OVERSAMPLENR, 140 },
   {  316 * OVERSAMPLENR, 130 },
   {  383 * OVERSAMPLENR, 120 },
   {  459 * OVERSAMPLENR, 110 },
   {  541 * OVERSAMPLENR, 100 },
   {  625 * OVERSAMPLENR,  90 },
   {  707 * OVERSAMPLENR,  80 },
   {  782 * OVERSAMPLENR,  70 },
   {  846 * OVERSAMPLENR,  60 },
   {  897 * OVERSAMPLENR,  50 },
   {  937 * OVERSAMPLENR,  40 },
   {  966 * OVERSAMPLENR,  30 },
   {  986 * OVERSAMPLENR,  20 },
   { 1000 * OVERSAMPLENR,  10 },
   { 1009 * OVERSAMPLENR,   0 }
 };
