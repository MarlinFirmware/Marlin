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
#pragma once

// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up, QU-BD silicone bed QWG-104F-3950 thermistor
constexpr temp_entry_t temptable_11[] PROGMEM = {
  { OV(   1), 938 },
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
  { OV( 221), 170 },
  { OV( 241), 165 },
  { OV( 261), 160 },
  { OV( 281), 155 },
  { OV( 301), 150 },
  { OV( 331), 144 },
  { OV( 361), 139 },
  { OV( 391), 133 },
  { OV( 421), 128 },
  { OV( 451), 123 },
  { OV( 491), 117 },
  { OV( 531), 111 },
  { OV( 571), 105 },
  { OV( 611), 100 },
  { OV( 641),  95 },
  { OV( 681),  90 },
  { OV( 711),  85 },
  { OV( 751),  79 },
  { OV( 791),  72 },
  { OV( 811),  69 },
  { OV( 831),  65 },
  { OV( 871),  57 },
  { OV( 881),  55 },
  { OV( 901),  51 },
  { OV( 921),  45 },
  { OV( 941),  39 },
  { OV( 971),  28 },
  { OV( 981),  23 },
  { OV( 991),  17 },
  { OV(1001),   9 },
  { OV(1021), -27 }
};
