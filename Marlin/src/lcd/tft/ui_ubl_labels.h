/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * UBL mesh map labels: Pick the font for the Z values drawn in each cell of ubl_plot().
 * Include after the ui_<W>x<H>.h layout. Also included by common-dependencies.h,
 * so the tiny font is only compiled when it's used.
 *
 * A short label ("-.13") is about 2.4 x FONT_SIZE wide in NotoSans and Helvetica, 3.2 x FONT_SIZE in Unifont.
 *  - UBL_MAP_LABELS_MENU_FONT : The cells fit labels in the menu font.
 *  - HAS_TFT_TINY_FONT        : The cells are too small for the menu font, so use the tiny digits font
 *                               (fontdata/NotoSans/NotoSans_Medium_Digits.cpp, about 300 bytes).
 *                               Disable with TFT_NO_TINY_FONT to show only the selected point's Z.
 * Labels are also measured at run time. If they don't fit, the map shows colored points.
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

  #define UBL_CELL_W            ((UBL_GRID_W - 4) / (GRID_MAX_POINTS_X))
  #define UBL_CELL_H            ((UBL_GRID_H - 4) / (GRID_MAX_POINTS_Y))

  #if TFT_FONT == UNIFONT
    #define _UBL_LABEL_W(S)     ((S) * 32 / 10)
  #else
    #define _UBL_LABEL_W(S)     ((S) * 24 / 10)
  #endif
  #ifndef UBL_LABEL_GAP
    #define UBL_LABEL_GAP       8     // Minimum space between labels, so they read as separate values
  #endif
  #define _UBL_LABEL_FITS(W,H)  (UBL_CELL_W >= (W) + (UBL_LABEL_GAP) && UBL_CELL_H >= (H) + 2)

  #define UBL_TINY_LABEL_W      22    // "-.13" in NotoSans_Medium_Digits
  #define UBL_TINY_LABEL_H      11    // Digit height in NotoSans_Medium_Digits

  #if _UBL_LABEL_FITS(_UBL_LABEL_W(FONT_SIZE), FONT_SIZE)
    #define UBL_MAP_LABELS_MENU_FONT 1
  #elif DISABLED(TFT_NO_TINY_FONT) && _UBL_LABEL_FITS(UBL_TINY_LABEL_W, UBL_TINY_LABEL_H)
    #define HAS_TFT_TINY_FONT 1
  #endif

#endif
