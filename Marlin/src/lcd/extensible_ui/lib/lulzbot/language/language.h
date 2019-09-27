/**************
 * language.h *
 **************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/
#pragma once

typedef const char Language_Str[];

// Count how many languages are defined.

#if defined(LCD_LANGUAGE_5)
  #define NUM_LANGUAGES 5
#elif defined(LCD_LANGUAGE_4)
  #define NUM_LANGUAGES 4
#elif defined(LCD_LANGUAGE_3)
  #define NUM_LANGUAGES 3
#elif defined(LCD_LANGUAGE_2)
  #define NUM_LANGUAGES 2
#else
  #define NUM_LANGUAGES 1
#endif

// Set undefined languages equal to the last and
// let the compiler optimize out the duplicates

#ifndef LCD_LANGUAGE_1
  #define LCD_LANGUAGE_1 LCD_LANGUAGE
#endif

#ifndef LCD_LANGUAGE_2
  #define LCD_LANGUAGE_2 LCD_LANGUAGE_1
#endif

#ifndef LCD_LANGUAGE_3
  #define LCD_LANGUAGE_3 LCD_LANGUAGE_2
#endif

#ifndef LCD_LANGUAGE_4
  #define LCD_LANGUAGE_4 LCD_LANGUAGE_3
#endif

#ifndef LCD_LANGUAGE_5
  #define LCD_LANGUAGE_5 LCD_LANGUAGE_4
#endif

// Indirection required to paste together the namespace name

#define _GET_LANG(LANG) Language_##LANG
#define GET_LANG(LANG) _GET_LANG(LANG)

#if NUM_LANGUAGES > 1
  extern uint8_t lang;
  // The compiler does a good job of "flattening" out this
  // if statement when there are fewer than five languages.
  #define GET_TEXT(MSG) ( \
    lang == 0 ? GET_LANG(LCD_LANGUAGE_1)::MSG : \
    lang == 1 ? GET_LANG(LCD_LANGUAGE_2)::MSG : \
    lang == 2 ? GET_LANG(LCD_LANGUAGE_3)::MSG : \
    lang == 3 ? GET_LANG(LCD_LANGUAGE_4)::MSG : \
                GET_LANG(LCD_LANGUAGE_5)::MSG \
    )
#else
  #define GET_TEXT(MSG) GET_LANG(LCD_LANGUAGE_1)::MSG
#endif
#define GET_TEXTF(MSG) reinterpret_cast<const __FlashStringHelper *>(GET_TEXT(MSG))

#define GET_LANGUAGE_NAME(N) GET_LANG(LCD_LANGUAGE_##N)::LANGUAGE

// All the language tables go here

#include "language_en.h"
#include "language_de.h"
#include "language_fr.h"
