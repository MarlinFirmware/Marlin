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

// NotoSans Medium Symbols 19pt, capital 'A' heigth: 14px, width: 100%
extern const uint8_t NotoSans_Medium_Symbols_14[131] = {
  130,14,8,0,10,0,18,251, // unifont_t
  // 0x08 - LCD_STR_THERMOMETER a.k.a 0x1f321  🌡
  11,25,75,13,1,251,1,84,0,11,254,0,15,7,0,15,135,0,14,7,0,14,7,0,15,135,0,14,39,0,15,247,0,14,119,0,15,247,0,14,119,0,15,247,0,14,119,0,14,119,0,47,247,192,120,180,240,247,253,56,223,255,28,223,255,28,235,254,44,117,244,116,46,86,208,6,254,64,0,0,0,
  // 0x09 - LCD_STR_DEGREE a.k.a 0x00b0  °
  6,7,14,8,1,7,26,64,122,224,224,112,208,112,240,176,63,192,0,0,
  // 0x0a - replacement for 0x2026 used in Greek languange files  …
  13,4,16,15,1,255,16,4,1,0,188,47,7,192,184,31,7,192,0,0,0,0,
};

#endif // HAS_GRAPHICAL_TFT
