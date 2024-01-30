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
#pragma once

#include "../../../inc/MarlinConfigPre.h"

#define NOTOSANS      1
#define UNIFONT       2
#define HELVETICA     3

#ifndef TFT_FONT
  #define TFT_FONT NOTOSANS
#endif

#define _LATIN_EXTENDED_A     1
#define _CYRILLIC             2
#define _GREEK                3
#define _KATAKANA             4
#define _KOREAN               5
#define _VIETNAMESE           6
#define _SIMPLIFIED_CHINESE   7
#define _TRADITIONAL_CHINESE  8

#define LCODE_cz      _LATIN_EXTENDED_A
#define LCODE_hr      _LATIN_EXTENDED_A
#define LCODE_pl      _LATIN_EXTENDED_A
#define LCODE_sk      _LATIN_EXTENDED_A
#define LCODE_tr      _LATIN_EXTENDED_A
#define LCODE_bg      _CYRILLIC
#define LCODE_ru      _CYRILLIC
#define LCODE_uk      _CYRILLIC
#define LCODE_el      _GREEK
#define LCODE_el_CY   _GREEK
#define LCODE_jp_kana _KATAKANA
#define LCODE_ko_KR   _KOREAN
#define LCODE_vi      _VIETNAMESE
#define LCODE_zh_CN   _SIMPLIFIED_CHINESE
#define LCODE_zh_TW   _TRADITIONAL_CHINESE

#define _LCODE(N) (CAT(LCODE_, LCD_LANGUAGE) == N)

#if _LCODE(_LATIN_EXTENDED_A)
  #define FONT_EXTRA    Latin_Extended_A
  #define EXTRA_GLYPHS  128
#elif _LCODE(_CYRILLIC)
  #define FONT_EXTRA    Cyrillic
  #define EXTRA_GLYPHS  145
#elif _LCODE(_GREEK)
  #define FONT_EXTRA    Greek
  #define EXTRA_GLYPHS  73
#elif _LCODE(_KATAKANA)
  #define FONT_EXTRA    Katakana
  #define EXTRA_GLYPHS  102
#elif _LCODE(_KOREAN)
  #define FONT_EXTRA    Korean
  #define EXTRA_GLYPHS  110
#elif _LCODE(_VIETNAMESE)
  #define FONT_EXTRA    Vietnamese
  #define EXTRA_GLYPHS  107
#elif _LCODE(_SIMPLIFIED_CHINESE)
  #define FONT_EXTRA    Simplified_Chinese
  #define EXTRA_GLYPHS  373
#elif _LCODE(_TRADITIONAL_CHINESE)
  #define FONT_EXTRA    Traditional_Chinese
  #define EXTRA_GLYPHS  307
#else // Basin Latin (0x0020 - 0x007f) and Latin-1 Supplement (0x0080-0x00ff) characters only
  #define EXTRA_GLYPHS  0
#endif

#undef _LCODE
#undef LCODE_cz
#undef LCODE_hr
#undef LCODE_pl
#undef LCODE_sk
#undef LCODE_tr
#undef LCODE_bg
#undef LCODE_ru
#undef LCODE_uk
#undef LCODE_el
#undef LCODE_el_CY
#undef LCODE_jp_kana
#undef LCODE_ko_KR
#undef LCODE_vi
#undef LCODE_zh_CN
#undef LCODE_zh_TW

#if TFT_FONT == NOTOSANS
  #define FONT_FAMILY       NotoSans_Medium
#elif TFT_FONT == UNIFONT
  #define FONT_FAMILY       Unifont
#elif TFT_FONT == HELVETICA
  #define FONT_FAMILY       Helvetica
  #ifdef FONT_EXTRA
    #error "Helvetica font does not have symbols required for selected LCD_LANGUAGE."
  #endif
#else
  #error "Invalid TFT_FONT value."
#endif
