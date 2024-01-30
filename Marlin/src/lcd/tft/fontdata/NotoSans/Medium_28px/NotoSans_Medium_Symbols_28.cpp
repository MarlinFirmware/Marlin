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

// NotoSans_Medium_Symbols 38pt, capital 'A' height: 28px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_28[140] = {
  130,28,8,0,10,0,37,246, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  22,46,20,25,2,248,0,7,255,253,0,0,0,31,255,255,64,0,0,47,255,255,128,0,0,63,0,31,128,0,0,63,0,15,128,0,0,63,240,15,128,0,0,63,0,15,128,0,0,63,0,15,128,0,0,63,0,15,128,0,0,63,0,15,128,0,0,63,240,15,128,0,0,63,0,15,128,0,0,63,0,15,128,0,0,63,47,79,128,0,0,63,191,79,128,0,0,63,191,79,128,0,0,63,63,79,128,0,0,63,63,79,128,0,0,63,63,79,128,0,0,63,255,79,128,0,0,63,63,79,128,0,0,63,63,79,128,0,0,63,63,79,128,0,0,63,191,79,128,0,0,63,191,79,128,0,0,63,63,79,128,0,0,63,63,79,128,0,0,63,63,79,192,0,0,255,255,79,224,0,3,253,63,75,252,0,15,224,63,64,255,0,47,129,255,224,63,128,63,7,255,253,15,192,189,31,255,255,11,208,252,63,255,255,131,224,252,63,255,255,195,240,252,63,255,255,195,240,252,63,255,255,195,224,189,47,255,255,75,208,127,15,255,255,15,192,63,131,255,252,63,128,15,240,111,144,255,0,7,254,0,27,252,0,1,255,255,255,240,0,0,47,255,255,128,0,0,1,191,228,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  13,13,52,16,2,15,0,85,0,0,11,255,224,0,47,255,252,0,127,65,254,0,253,0,63,0,252,0,47,0,252,0,47,64,252,0,47,0,253,0,63,0,127,129,254,0,47,255,252,0,11,255,224,0,0,85,0,0,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  26,6,42,31,2,255,31,128,0,248,0,11,192,63,224,3,255,0,63,240,127,224,3,255,0,63,240,127,224,3,255,0,63,240,63,208,2,253,0,47,224,5,0,0,84,0,5,64,
};

#endif // HAS_GRAPHICAL_TFT
