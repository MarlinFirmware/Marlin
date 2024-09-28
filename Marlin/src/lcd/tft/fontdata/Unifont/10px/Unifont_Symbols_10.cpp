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

#include "../../fontdata.h"

#if HAS_GRAPHICAL_TFT && TFT_FONT == UNIFONT

// Unifont Symbols 16pt, capital 'A' height: 10px, width: 100%
extern const uint8_t Unifont_Symbols_10[44] = {
  129,10,8,0,10,0,14,254, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  5,12,12,16,5,0,32,80,80,80,80,112,112,112,248,248,248,112,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  4,4,4,8,2,6,96,144,144,96,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  7,2,2,8,1,0,146,146,
};

#endif // HAS_GRAPHICAL_TFT
