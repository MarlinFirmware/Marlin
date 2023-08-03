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

// NotoSans_Medium_Symbols 40pt, capital 'A' height: 29px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_29[158] = {
  130,29,8,0,10,0,38,246, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  23,49,38,27,2,247,0,7,255,255,0,0,0,31,255,255,192,0,0,47,255,255,208,0,0,47,64,11,208,0,0,47,0,7,208,0,0,47,144,7,208,0,0,47,244,7,208,0,0,47,0,7,208,0,0,47,0,7,208,0,0,47,0,7,208,0,0,47,244,7,208,0,0,47,144,7,208,0,0,47,0,7,208,0,0,47,5,71,208,0,0,47,31,199,208,0,0,47,255,199,208,0,0,47,175,199,208,0,0,47,31,199,208,0,0,47,31,199,208,0,0,47,95,199,208,0,0,47,255,199,208,0,0,47,95,199,208,0,0,47,31,199,208,0,0,47,31,199,208,0,0,47,175,199,208,0,0,47,255,199,208,0,0,47,31,199,208,0,0,47,31,199,208,0,0,47,31,199,208,0,0,127,191,199,240,0,2,255,191,199,253,0,11,248,31,192,255,64,31,208,47,208,47,192,63,66,255,253,11,224,127,11,255,255,67,240,189,31,255,255,194,244,252,47,255,255,208,248,252,63,255,255,224,252,252,63,255,255,224,252,252,63,255,255,224,248,189,31,255,255,193,248,126,15,255,255,131,240,63,67,255,254,11,240,31,208,111,224,31,192,11,248,0,0,191,128,3,255,64,11,254,0,0,191,255,255,244,0,0,15,255,255,128,0,0,0,111,228,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  13,13,52,17,2,16,1,191,144,0,11,255,252,0,63,255,255,0,127,64,127,64,189,0,31,192,252,0,15,192,252,0,15,192,252,0,15,192,190,0,47,128,63,144,191,64,47,255,254,0,7,255,248,0,0,106,64,0,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  27,6,42,32,3,255,127,64,2,252,0,15,224,255,192,7,255,0,47,244,255,192,7,255,0,63,248,255,192,7,255,0,47,248,191,128,3,253,0,31,240,25,0,0,96,0,1,64,
};

#endif // HAS_GRAPHICAL_TFT
