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

// 100k GE Sensing AL03006-58.2K-97-G1 (4.7k pullup)
const short temptable_9[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 936 },
  {   36 * OVERSAMPLENR, 300 },
  {   71 * OVERSAMPLENR, 246 },
  {  106 * OVERSAMPLENR, 218 },
  {  141 * OVERSAMPLENR, 199 },
  {  176 * OVERSAMPLENR, 185 },
  {  211 * OVERSAMPLENR, 173 },
  {  246 * OVERSAMPLENR, 163 },
  {  281 * OVERSAMPLENR, 155 },
  {  316 * OVERSAMPLENR, 147 },
  {  351 * OVERSAMPLENR, 140 },
  {  386 * OVERSAMPLENR, 134 },
  {  421 * OVERSAMPLENR, 128 },
  {  456 * OVERSAMPLENR, 122 },
  {  491 * OVERSAMPLENR, 117 },
  {  526 * OVERSAMPLENR, 112 },
  {  561 * OVERSAMPLENR, 107 },
  {  596 * OVERSAMPLENR, 102 },
  {  631 * OVERSAMPLENR,  97 },
  {  666 * OVERSAMPLENR,  92 },
  {  701 * OVERSAMPLENR,  87 },
  {  736 * OVERSAMPLENR,  81 },
  {  771 * OVERSAMPLENR,  76 },
  {  806 * OVERSAMPLENR,  70 },
  {  841 * OVERSAMPLENR,  63 },
  {  876 * OVERSAMPLENR,  56 },
  {  911 * OVERSAMPLENR,  48 },
  {  946 * OVERSAMPLENR,  38 },
  {  981 * OVERSAMPLENR,  23 },
  { 1005 * OVERSAMPLENR,   5 },
  { 1016 * OVERSAMPLENR,   0 }
};
