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

// 10k thermistor
const short temptable_4[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 430 },
  {   54 * OVERSAMPLENR, 137 },
  {  107 * OVERSAMPLENR, 107 },
  {  160 * OVERSAMPLENR,  91 },
  {  213 * OVERSAMPLENR,  80 },
  {  266 * OVERSAMPLENR,  71 },
  {  319 * OVERSAMPLENR,  64 },
  {  372 * OVERSAMPLENR,  57 },
  {  425 * OVERSAMPLENR,  51 },
  {  478 * OVERSAMPLENR,  46 },
  {  531 * OVERSAMPLENR,  41 },
  {  584 * OVERSAMPLENR,  35 },
  {  637 * OVERSAMPLENR,  30 },
  {  690 * OVERSAMPLENR,  25 },
  {  743 * OVERSAMPLENR,  20 },
  {  796 * OVERSAMPLENR,  14 },
  {  849 * OVERSAMPLENR,   7 },
  {  902 * OVERSAMPLENR,   0 },
  {  955 * OVERSAMPLENR, -11 },
  { 1008 * OVERSAMPLENR, -35 }
};
