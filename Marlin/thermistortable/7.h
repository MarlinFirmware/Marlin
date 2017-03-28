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

// 100k Honeywell 135-104LAG-J01
const short temptable_7[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 941 },
  {   19 * OVERSAMPLENR, 362 },
  {   37 * OVERSAMPLENR, 299 }, // top rating 300C
  {   55 * OVERSAMPLENR, 266 },
  {   73 * OVERSAMPLENR, 245 },
  {   91 * OVERSAMPLENR, 229 },
  {  109 * OVERSAMPLENR, 216 },
  {  127 * OVERSAMPLENR, 206 },
  {  145 * OVERSAMPLENR, 197 },
  {  163 * OVERSAMPLENR, 190 },
  {  181 * OVERSAMPLENR, 183 },
  {  199 * OVERSAMPLENR, 177 },
  {  217 * OVERSAMPLENR, 171 },
  {  235 * OVERSAMPLENR, 166 },
  {  253 * OVERSAMPLENR, 162 },
  {  271 * OVERSAMPLENR, 157 },
  {  289 * OVERSAMPLENR, 153 },
  {  307 * OVERSAMPLENR, 149 },
  {  325 * OVERSAMPLENR, 146 },
  {  343 * OVERSAMPLENR, 142 },
  {  361 * OVERSAMPLENR, 139 },
  {  379 * OVERSAMPLENR, 135 },
  {  397 * OVERSAMPLENR, 132 },
  {  415 * OVERSAMPLENR, 129 },
  {  433 * OVERSAMPLENR, 126 },
  {  451 * OVERSAMPLENR, 123 },
  {  469 * OVERSAMPLENR, 121 },
  {  487 * OVERSAMPLENR, 118 },
  {  505 * OVERSAMPLENR, 115 },
  {  523 * OVERSAMPLENR, 112 },
  {  541 * OVERSAMPLENR, 110 },
  {  559 * OVERSAMPLENR, 107 },
  {  577 * OVERSAMPLENR, 105 },
  {  595 * OVERSAMPLENR, 102 },
  {  613 * OVERSAMPLENR,  99 },
  {  631 * OVERSAMPLENR,  97 },
  {  649 * OVERSAMPLENR,  94 },
  {  667 * OVERSAMPLENR,  92 },
  {  685 * OVERSAMPLENR,  89 },
  {  703 * OVERSAMPLENR,  86 },
  {  721 * OVERSAMPLENR,  84 },
  {  739 * OVERSAMPLENR,  81 },
  {  757 * OVERSAMPLENR,  78 },
  {  775 * OVERSAMPLENR,  75 },
  {  793 * OVERSAMPLENR,  72 },
  {  811 * OVERSAMPLENR,  69 },
  {  829 * OVERSAMPLENR,  66 },
  {  847 * OVERSAMPLENR,  62 },
  {  865 * OVERSAMPLENR,  59 },
  {  883 * OVERSAMPLENR,  55 },
  {  901 * OVERSAMPLENR,  51 },
  {  919 * OVERSAMPLENR,  46 },
  {  937 * OVERSAMPLENR,  41 },
  {  955 * OVERSAMPLENR,  35 },
  {  973 * OVERSAMPLENR,  27 },
  {  991 * OVERSAMPLENR,  17 },
  { 1009 * OVERSAMPLENR,   1 },
  { 1023 * OVERSAMPLENR,   0 } // to allow internal 0 degrees C
};
