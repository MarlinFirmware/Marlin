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

// NotoSans_Medium_Symbols 37pt, capital 'A' height: 27px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_27[118] = {
  130,27,8,0,10,0,35,247, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  21,45,14,25,2,248,0,11,255,253,0,0,0,31,255,255,0,0,0,47,170,191,64,0,0,46,0,31,64,0,0,46,0,31,64,0,0,47,208,31,64,0,0,46,0,31,64,0,0,46,0,31,64,0,0,46,0,31,64,0,0,46,0,31,64,0,0,47,208,31,64,0,0,46,0,31,64,0,0,46,63,31,64,0,0,47,191,31,64,0,0,47,191,31,64,0,0,46,63,31,64,0,0,46,63,31,64,0,0,46,127,31,64,0,0,47,255,31,64,0,0,46,63,31,64,0,0,46,63,31,64,0,0,46,63,31,64,0,0,47,255,31,64,0,0,47,127,31,64,0,0,46,63,31,64,0,0,46,63,31,64,0,0,63,191,31,128,0,1,255,255,31,240,0,7,244,63,7,248,0,31,192,127,64,253,0,63,67,255,244,63,0,126,15,255,254,31,64,189,47,255,255,15,128,252,63,255,255,143,192,252,63,255,255,139,192,252,63,255,255,139,192,252,47,255,255,79,192,189,15,255,255,15,128,127,7,255,252,47,64,63,128,191,224,127,0,15,224,0,1,252,0,7,253,0,11,244,0,1,255,234,255,208,0,0,47,255,255,0,0,0,2,255,224,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  12,12,36,16,2,15,1,170,64,15,255,224,63,255,252,190,0,253,252,0,63,248,0,63,248,0,63,252,0,63,190,0,189,63,235,252,31,255,240,1,254,64,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  26,6,42,30,2,255,26,64,1,164,0,26,64,63,208,7,252,0,127,192,127,224,7,253,0,191,208,127,208,7,253,0,191,208,63,192,3,252,0,63,192,5,0,0,80,0,5,0,
};

#endif // HAS_GRAPHICAL_TFT
