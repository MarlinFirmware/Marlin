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

// NotoSans Medium Symbols 22pt, capital 'A' heigth: 16px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_16[166] = {
  130,16,8,0,10,0,21,250, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  13,27,108,15,1,251,1,255,208,0,3,234,240,0,3,128,240,0,3,224,240,0,3,128,240,0,3,224,240,0,3,128,240,0,3,156,240,0,3,253,240,0,3,173,240,0,3,173,240,0,3,253,240,0,3,173,240,0,3,253,240,0,3,173,240,0,3,173,240,0,11,253,248,0,61,45,46,0,116,255,139,0,242,255,227,64,227,255,243,128,227,255,243,64,177,255,215,0,56,127,79,0,31,64,188,0,7,255,224,0,0,89,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  8,8,16,9,1,8,11,208,62,184,176,44,240,29,176,44,126,184,31,224,0,0,
  // 0x0a - replacement for 0x2026 used in Greek languange files  …
  16,4,16,18,1,255,56,3,128,56,125,11,208,189,125,7,192,124,0,0,0,0,
};

#endif // HAS_GRAPHICAL_TFT
