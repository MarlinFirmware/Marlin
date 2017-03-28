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

// 100k 0603 SMD Vishay NTCS0603E3104FXT (4.7k pullup)
const short temptable_8[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 704 },
  {   54 * OVERSAMPLENR, 216 },
  {  107 * OVERSAMPLENR, 175 },
  {  160 * OVERSAMPLENR, 152 },
  {  213 * OVERSAMPLENR, 137 },
  {  266 * OVERSAMPLENR, 125 },
  {  319 * OVERSAMPLENR, 115 },
  {  372 * OVERSAMPLENR, 106 },
  {  425 * OVERSAMPLENR,  99 },
  {  478 * OVERSAMPLENR,  91 },
  {  531 * OVERSAMPLENR,  85 },
  {  584 * OVERSAMPLENR,  78 },
  {  637 * OVERSAMPLENR,  71 },
  {  690 * OVERSAMPLENR,  65 },
  {  743 * OVERSAMPLENR,  58 },
  {  796 * OVERSAMPLENR,  50 },
  {  849 * OVERSAMPLENR,  42 },
  {  902 * OVERSAMPLENR,  31 },
  {  955 * OVERSAMPLENR,  17 },
  { 1008 * OVERSAMPLENR,   0 }
};
