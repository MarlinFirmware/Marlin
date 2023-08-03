/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// Unifont Symbols 48pt, capital 'A' height: 30px, width: 100%
extern const uint8_t Unifont_Symbols_30[140] = {
  129,30,8,0,10,0,42,250, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  15,36,72,48,15,0,3,128,3,128,3,128,28,112,28,112,28,112,28,112,28,112,28,112,28,112,28,112,28,112,28,112,28,112,28,112,31,240,31,240,31,240,31,240,31,240,31,240,31,240,31,240,31,240,255,254,255,254,255,254,255,254,255,254,255,254,255,254,255,254,255,254,31,240,31,240,31,240,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  12,12,24,24,6,18,31,128,31,128,31,128,224,112,224,112,224,112,224,112,224,112,224,112,31,128,31,128,31,128,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  21,6,18,24,3,0,224,112,56,224,112,56,224,112,56,224,112,56,224,112,56,224,112,56,
};

#endif // HAS_GRAPHICAL_TFT
