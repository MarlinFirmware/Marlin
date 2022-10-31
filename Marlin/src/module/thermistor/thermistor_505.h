/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// ZONESTAR hotbed QWG-104F-3950 thermistor
constexpr temp_entry_t temptable_505[] PROGMEM = {
  { OV(   1), 938 },
  { OV(   8), 320 },
  { OV(  16), 300 },
  { OV(  27), 290 },
  { OV(  36), 272 },
  { OV(  47), 258 },
  { OV(  56), 248 },
  { OV(  68), 245 },
  { OV(  78), 237 },
  { OV(  89), 228 },
  { OV(  99), 221 },
  { OV( 110), 215 },
  { OV( 120), 209 },
  { OV( 131), 204 },
  { OV( 141), 199 },
  { OV( 151), 195 },
  { OV( 161), 190 },
  { OV( 171), 187 },
  { OV( 181), 183 },
  { OV( 201), 179 },
  { OV( 221), 170 },
  { OV( 251), 165 },
  { OV( 261), 160 },
  { OV( 321), 150 },
  { OV( 361), 144 },
  { OV( 401), 140 },
  { OV( 421), 133 },
  { OV( 451), 130 },
  { OV( 551), 120 },
  { OV( 571), 117 },
  { OV( 596), 110 },
  { OV( 626), 105 },
  { OV( 666), 100 },
  { OV( 677),  95 },
  { OV( 697),  90 },
  { OV( 717),  85 },
  { OV( 727),  79 },
  { OV( 750),  72 },
  { OV( 789),  69 },
  { OV( 819),  65 },
  { OV( 861),  57 },
  { OV( 870),  55 },
  { OV( 881),  51 },
  { OV( 911),  45 },
  { OV( 922),  39 },
  { OV( 968),  28 },
  { OV( 977),  25 },
  { OV( 985),  23 },
  { OV( 993),  20 },
  { OV( 999),  18 },
  { OV(1004),  15 },
  { OV(1008),  12 },
  { OV(1012),   8 },
  { OV(1016),   5 },
  { OV(1020),   0 },
  { OV(1023),  -5 }
};
