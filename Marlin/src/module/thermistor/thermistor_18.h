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

// ATC Semitec 204GT-2 (4.7k pullup) Dagoma.Fr - MKS_Base_DKU001327 - version (measured/tested/approved)
constexpr temp_entry_t temptable_18[] PROGMEM = {
  { OV(   1), 713 },
  { OV(  17), 284 },
  { OV(  20), 275 },
  { OV(  23), 267 },
  { OV(  27), 257 },
  { OV(  31), 250 },
  { OV(  37), 240 },
  { OV(  43), 232 },
  { OV(  51), 222 },
  { OV(  61), 213 },
  { OV(  73), 204 },
  { OV(  87), 195 },
  { OV( 106), 185 },
  { OV( 128), 175 },
  { OV( 155), 166 },
  { OV( 189), 156 },
  { OV( 230), 146 },
  { OV( 278), 137 },
  { OV( 336), 127 },
  { OV( 402), 117 },
  { OV( 476), 107 },
  { OV( 554),  97 },
  { OV( 635),  87 },
  { OV( 713),  78 },
  { OV( 784),  68 },
  { OV( 846),  58 },
  { OV( 897),  49 },
  { OV( 937),  39 },
  { OV( 966),  30 },
  { OV( 986),  20 },
  { OV(1000),  10 },
  { OV(1010),   0 },
  { OV(1024),-273 } // for safety
};
