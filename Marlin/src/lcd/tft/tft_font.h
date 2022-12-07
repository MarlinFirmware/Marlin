/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_TFT

#define en       0
#define an       1
#define bg       2
#define ca       3
#define cz       4
#define da       5
#define de       6
#define el       7
#define el_CY    8
#define es       9
#define eu      10
#define fi      11
#define fr      12
#define gl      13
#define hr      14
#define hu      15
#define it      16
#define jp_kana 17
#define ko_KR   18
#define nl      19
#define pl      20
#define pt      21
#define pt_br   22
#define ro      23
#define ru      24
#define sk      25
#define sv      26
#define tr      27
#define uk      28
#define vi      29
#define zh_CN   30
#define zh_TW   31


#if (LCD_LANGUAGE == cz) || (LCD_LANGUAGE == hr) || (LCD_LANGUAGE == pl) || (LCD_LANGUAGE == sk) || (LCD_LANGUAGE == tr)
  #define FONT_EXTRA    Latin_Extended_A
#elif (LCD_LANGUAGE == bg) || (LCD_LANGUAGE == ru) || (LCD_LANGUAGE == uk)
  #define FONT_EXTRA    Cyrilic
#elif (LCD_LANGUAGE == el) || (LCD_LANGUAGE == el_CY)
  #define FONT_EXTRA    Greek
#elif LCD_LANGUAGE == jp_kana
  #define FONT_EXTRA    Katakana
#elif LCD_LANGUAGE == ko_KR
  #define FONT_EXTRA    Korean
#elif LCD_LANGUAGE == vi
  #define FONT_EXTRA    Vietnamese
#elif LCD_LANGUAGE == zh_CN
  #define FONT_EXTRA    Simplified_Chinese
#elif LCD_LANGUAGE == zh_TW
  #define FONT_EXTRA    Traditional_Chinese
#else
  // No extra font needed
#endif

#undef en
#undef an
#undef bg
#undef ca
#undef cz
#undef da
#undef de
#undef el
#undef el_CY
#undef es
#undef eu
#undef fi
#undef fr
#undef gl
#undef hr
#undef hu
#undef it
#undef jp_kana
#undef ko_KR
#undef nl
#undef pl
#undef pt
#undef pt_br
#undef ro
#undef ru
#undef sk
#undef sv
#undef tr
#undef uk
#undef vi
#undef zh_CN
#undef zh_TW

#if TFT_FONT == NOTOSANS
  #ifndef FONT_FAMILY
    #define FONT_FAMILY         NotoSans_Medium
  #endif
#elif TFT_FONT == UNIFONT
  #ifndef FONT_FAMILY
    #define FONT_FAMILY         Unifont
  #endif
#elif TFT_FONT == HELVETICA
  #ifndef FONT_FAMILY
    #define FONT_FAMILY         Helvetica
  #endif
  #ifdef FONT_EXTRA
    #error Helvetica font does not have symbols required for selected LCD_LANGUAGE
  #endif
#else  
  #error "Invalid TFT_FONT value."
#endif

#define JOIN(A,B,C)         CAT(CAT(A, B), C)
#define MENU_FONT_NAME      JOIN(FONT_FAMILY, _, FONT_SIZE)
#define SYMBOLS_FONT_NAME   JOIN(FONT_FAMILY, _Symbols_, FONT_SIZE)

#ifdef FONT_EXTRA
  #define EXTRA_FONT_NAME   JOIN(FONT_FAMILY, JOIN(_, FONT_EXTRA, _), FONT_SIZE)
  extern const uint8_t EXTRA_FONT_NAME[];
#endif

extern const uint8_t MENU_FONT_NAME[];
extern const uint8_t SYMBOLS_FONT_NAME[];

#endif // HAS_GRAPHICAL_TFT
