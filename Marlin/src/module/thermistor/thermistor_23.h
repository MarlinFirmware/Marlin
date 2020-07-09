/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

// 100k hotbed thermistor with 4.7k pull up to 3.3v and 220R to analog input as in GTM32 Pro vB
const short temptable_23[][2] PROGMEM = {
  { OV(   1), 938 },
  { OV(  11), 423 },
  { OV(  21), 351 },
  { OV(  31), 314 },
  { OV(  41), 290 },
  { OV(  51), 272 },
  { OV(  61), 258 },
  { OV(  71), 247 },
  { OV(  81), 237 },
  { OV(  91), 229 },
  { OV( 101), 221 },
  { OV( 111), 215 },
  { OV( 121), 209 },
  { OV( 131), 204 },
  { OV( 141), 199 },
  { OV( 151), 195 },
  { OV( 161), 190 },
  { OV( 171), 187 },
  { OV( 181), 183 },
  { OV( 191), 179 },
  { OV( 201), 176 },
  { OV( 211), 173 },
  { OV( 221), 170 },
  { OV( 231), 167 },
  { OV( 241), 165 },
  { OV( 251), 162 },
  { OV( 261), 160 },
  { OV( 271), 157 },
  { OV( 281), 155 },
  { OV( 291), 153 },
  { OV( 301), 150 },
  { OV( 311), 148 },
  { OV( 321), 146 },
  { OV( 331), 144 },
  { OV( 341), 142 },
  { OV( 351), 140 },
  { OV( 361), 139 },
  { OV( 371), 137 },
  { OV( 381), 135 },
  { OV( 391), 133 },
  { OV( 401), 131 },
  { OV( 411), 130 },
  { OV( 421), 128 },
  { OV( 431), 126 },
  { OV( 441), 125 },
  { OV( 451), 123 },
  { OV( 461), 122 },
  { OV( 471), 120 },
  { OV( 481), 119 },
  { OV( 491), 117 },
  { OV( 501), 116 },
  { OV( 511), 114 },
  { OV( 521), 113 },
  { OV( 531), 111 },
  { OV( 541), 110 },
  { OV( 551), 108 },
  { OV( 561), 107 },
  { OV( 571), 105 },
  { OV( 581), 104 },
  { OV( 591), 102 },
  { OV( 601), 101 },
  { OV( 611), 100 },
  { OV( 621),  98 },
  { OV( 631),  97 },
  { OV( 641),  95 },
  { OV( 651),  94 },
  { OV( 661),  92 },
  { OV( 671),  91 },
  { OV( 681),  90 },
  { OV( 691),  88 },
  { OV( 701),  87 },
  { OV( 711),  85 },
  { OV( 721),  84 },
  { OV( 731),  82 },
  { OV( 741),  81 },
  { OV( 751),  79 },
  { OV( 761),  77 },
  { OV( 771),  76 },
  { OV( 781),  74 },
  { OV( 791),  72 },
  { OV( 801),  71 },
  { OV( 811),  69 },
  { OV( 821),  67 },
  { OV( 831),  65 },
  { OV( 841),  63 },
  { OV( 851),  62 },
  { OV( 861),  60 },
  { OV( 871),  57 },
  { OV( 881),  55 },
  { OV( 891),  53 },
  { OV( 901),  51 },
  { OV( 911),  48 },
  { OV( 921),  45 },
  { OV( 931),  42 },
  { OV( 941),  39 },
  { OV( 951),  36 },
  { OV( 961),  32 },
  { OV( 971),  28 },
  { OV( 981),  25 },
  { OV( 991),  23 },
  { OV(1001),  21 },
  { OV(1011),  19 },
  { OV(1021),   5 }
};
