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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

// 100k hotend thermistor with 4.7k pull up to 3.3v and 220R to analog input as in GTM32 Pro vB
constexpr temp_entry_t temptable_22[] PROGMEM = {
  { OV(   1), 367 },
  { OV(  17), 331 },
  { OV(  33), 297 },
  { OV(  49), 280 },
  { OV(  65), 264 },
  { OV(  81), 253 },
  { OV(  97), 240 },
  { OV( 113), 230 },
  { OV( 129), 219 },
  { OV( 145), 211 },
  { OV( 161), 204 },
  { OV( 177), 197 },
  { OV( 193), 192 },
  { OV( 209), 188 },
  { OV( 225), 181 },
  { OV( 241), 177 },
  { OV( 257), 172 },
  { OV( 273), 169 },
  { OV( 289), 168 },
  { OV( 305), 166 },
  { OV( 321), 162 },
  { OV( 337), 156 },
  { OV( 353), 152 },
  { OV( 369), 150 },
  { OV( 385), 147 },
  { OV( 401), 144 },
  { OV( 417), 141 },
  { OV( 433), 138 },
  { OV( 449), 135 },
  { OV( 465), 133 },
  { OV( 481), 130 },
  { OV( 497), 127 },
  { OV( 513), 125 },
  { OV( 529), 122 },
  { OV( 545), 120 },
  { OV( 561), 118 },
  { OV( 577), 115 },
  { OV( 593), 113 },
  { OV( 609), 111 },
  { OV( 625), 108 },
  { OV( 641), 106 },
  { OV( 657), 103 },
  { OV( 673), 100 },
  { OV( 689),  96 },
  { OV( 705),  93 },
  { OV( 721),  90 },
  { OV( 737),  87 },
  { OV( 753),  86 },
  { OV( 769),  84 },
  { OV( 785),  82 },
  { OV( 801),  79 },
  { OV( 817),  77 },
  { OV( 833),  74 },
  { OV( 849),  71 },
  { OV( 865),  68 },
  { OV( 881),  65 },
  { OV( 897),  60 },
  { OV( 913),  56 },
  { OV( 929),  52 },
  { OV( 945),  47 },
  { OV( 961),  41 },
  { OV( 977),  32 },
  { OV( 993),  24 },
  { OV(1009),  12 },
  { OV(1025),   0 }
};
