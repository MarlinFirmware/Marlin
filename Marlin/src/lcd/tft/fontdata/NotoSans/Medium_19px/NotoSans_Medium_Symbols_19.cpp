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

// NotoSans Medium Symbols 26pt, capital 'A' heigth: 19px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_19[206] = {
  130,19,8,0,10,0,25,249, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  15,32,128,17,1,250,0,127,252,0,0,255,254,0,0,240,15,0,0,252,15,0,0,224,15,0,0,224,15,0,0,224,15,0,0,252,15,0,0,224,15,0,0,231,207,0,0,255,207,0,0,231,207,0,0,231,207,0,0,255,207,0,0,231,207,0,0,231,207,0,0,255,207,0,0,231,207,0,0,231,207,0,2,255,207,64,15,135,199,208,62,11,209,240,120,127,252,180,180,255,254,56,240,255,255,60,240,255,255,60,180,255,254,120,124,63,252,244,62,10,146,224,15,208,31,192,2,255,254,0,0,47,224,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  9,9,27,11,1,10,6,228,0,47,254,0,124,15,64,180,7,128,180,3,192,184,7,128,62,111,0,31,253,0,1,80,0,
  // 0x0a - replacement for 0x2026 used in Greek languange files  …
  18,5,25,21,2,255,16,0,64,1,0,252,3,240,15,192,253,3,244,15,208,252,3,240,15,192,16,0,64,1,0,
};

#endif // HAS_GRAPHICAL_TFT
