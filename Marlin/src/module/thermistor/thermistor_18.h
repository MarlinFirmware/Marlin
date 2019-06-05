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
 
// 18 is ATC Semitec 204GT-2 (4.7k pullup) Dagoma.Fr - MKS_Base_DKU001327 - version (measured/tested/approved)
const short temptable_18[][2] PROGMEM = {
  {1*OVERSAMPLENR,713},
  {17*OVERSAMPLENR,284},
  {20*OVERSAMPLENR,275},
  {23*OVERSAMPLENR,267},
  {27*OVERSAMPLENR,257},
  {31*OVERSAMPLENR,250},
  {37*OVERSAMPLENR,240},
  {43*OVERSAMPLENR,232},
  {51*OVERSAMPLENR,222},
  {61*OVERSAMPLENR,213},
  {73*OVERSAMPLENR,204},
  {87*OVERSAMPLENR,195},
  {106*OVERSAMPLENR,185},
  {128*OVERSAMPLENR,175},
  {155*OVERSAMPLENR,166},
  {189*OVERSAMPLENR,156},
  {230*OVERSAMPLENR,146},
  {278*OVERSAMPLENR,137},
  {336*OVERSAMPLENR,127},
  {402*OVERSAMPLENR,117},
  {476*OVERSAMPLENR,107},
  {554*OVERSAMPLENR,97},
  {635*OVERSAMPLENR,87},
  {713*OVERSAMPLENR,78},
  {784*OVERSAMPLENR,68},
  {846*OVERSAMPLENR,58},
  {897*OVERSAMPLENR,49},
  {937*OVERSAMPLENR,39},
  {966*OVERSAMPLENR,30},
  {986*OVERSAMPLENR,20},
  {1000*OVERSAMPLENR,10},
  {1010*OVERSAMPLENR,0},
  {1024*OVERSAMPLENR,-273} // for safety
};
