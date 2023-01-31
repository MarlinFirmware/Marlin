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

#include "../../../../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_TFT

#include <stdint.h>

// Unifont Symbols 32pt, capital 'A' heigth: 20px, width: 100%
extern const uint8_t Unifont_Symbols_20[90] = {
  129,20,8,0,10,0,28,252, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  10,24,48,32,10,0,12,0,12,0,51,0,51,0,51,0,51,0,51,0,51,0,51,0,51,0,63,0,63,0,63,0,63,0,63,0,63,0,255,192,255,192,255,192,255,192,255,192,255,192,63,0,63,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  8,8,8,16,4,12,60,60,195,195,195,195,60,60,
  // 0x0a - replacement for 0x2026 used in Greek languange files  …
  14,4,8,16,2,0,195,12,195,12,195,12,195,12,
};

#endif // HAS_GRAPHICAL_TFT
