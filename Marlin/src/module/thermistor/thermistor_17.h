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

// Dagoma NTC 100k white thermistor
constexpr temp_entry_t temptable_17[] PROGMEM = {
  { OV(  16),  309 },
  { OV(  18),  307 },
  { OV(  20),  300 },
  { OV(  22),  293 },
  { OV(  26),  284 },
  { OV(  29),  272 },
  { OV(  33),  266 },
  { OV(  36),  260 },
  { OV(  42),  252 },
  { OV(  46),  247 },
  { OV(  48),  244 },
  { OV(  51),  241 },
  { OV(  62),  231 },
  { OV(  73),  222 },
  { OV(  78),  219 },
  { OV(  87),  212 },
  { OV(  98),  207 },
  { OV( 109),  201 },
  { OV( 118),  197 },
  { OV( 131),  191 },
  { OV( 145),  186 },
  { OV( 160),  181 },
  { OV( 177),  175 },
  { OV( 203),  169 },
  { OV( 222),  164 },
  { OV( 256),  156 },
  { OV( 283),  151 },
  { OV( 312),  145 },
  { OV( 343),  140 },
  { OV( 377),  131 },
  { OV( 413),  125 },
  { OV( 454),  119 },
  { OV( 496),  113 },
  { OV( 537),  108 },
  { OV( 578),  102 },
  { OV( 619),   97 },
  { OV( 658),   92 },
  { OV( 695),   87 },
  { OV( 735),   81 },
  { OV( 773),   75 },
  { OV( 808),   70 },
  { OV( 844),   64 },
  { OV( 868),   59 },
  { OV( 892),   54 },
  { OV( 914),   49 },
  { OV( 935),   42 },
  { OV( 951),   38 },
  { OV( 967),   32 },
  { OV( 975),   28 },
  { OV(1000),   20 },
  { OV(1010),   10 },
  { OV(1024), -273 } // for safety
};
