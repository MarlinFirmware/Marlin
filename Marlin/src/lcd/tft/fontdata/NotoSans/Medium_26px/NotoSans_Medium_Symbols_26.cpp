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

// NotoSans_Medium_Symbols 36pt, capital 'A' height: 26px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_26[112] = {
  130,26,8,0,10,0,34,247, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  21,44,8,24,2,248,0,15,255,248,0,0,0,63,255,253,0,0,0,63,170,254,0,0,0,61,0,62,0,0,0,61,0,62,0,0,0,63,192,62,0,0,0,61,0,62,0,0,0,61,0,62,0,0,0,61,0,62,0,0,0,63,192,62,0,0,0,61,0,62,0,0,0,61,0,62,0,0,0,61,126,62,0,0,0,63,254,62,0,0,0,62,190,62,0,0,0,61,126,62,0,0,0,61,126,62,0,0,0,62,190,62,0,0,0,63,254,62,0,0,0,61,126,62,0,0,0,61,126,62,0,0,0,61,190,62,0,0,0,63,254,62,0,0,0,61,126,62,0,0,0,61,126,62,0,0,0,61,126,62,0,0,0,255,254,63,64,0,3,253,190,47,224,0,15,208,126,3,248,0,47,66,255,128,253,0,62,15,255,244,63,0,188,47,255,252,47,0,252,63,255,254,15,64,248,127,255,255,15,128,248,127,255,255,15,128,248,63,255,255,15,64,188,63,255,253,31,0,125,15,255,248,63,0,63,2,255,208,189,0,31,192,20,2,252,0,11,248,0,31,240,0,2,255,234,255,192,0,0,127,255,254,0,0,0,6,255,208,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  12,12,36,15,2,14,6,255,64,47,255,240,127,155,248,252,0,252,248,0,125,248,0,62,248,0,61,252,0,189,127,70,252,47,255,240,11,255,128,0,16,0,
  // 0x0a - replacement for 0x2026 used in Greek language files  …
  25,6,42,29,2,255,26,0,1,160,0,41,0,63,192,11,248,0,255,64,127,208,15,252,0,255,128,127,208,15,252,0,255,128,63,192,7,248,0,255,0,5,0,0,80,0,20,0,
};

#endif // HAS_GRAPHICAL_TFT
