/********************
 * multi_language.h *
 ********************/

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

// Setting the unused languages equal to each other allows
// the compiler to optimize away the conditionals

#ifndef LCD_LANGUAGE_2
  #define LCD_LANGUAGE_2 LCD_LANGUAGE
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

#define _GET_LANG(LANG) Language_##LANG
#define GET_LANG(LANG) _GET_LANG(LANG)

#if NUM_LANGUAGES > 1
  extern uint8_t lang;
  #define GET_TEXT(MSG) ( \
    lang == 0 ? GET_LANG(LCD_LANGUAGE)::MSG : \
    lang == 1 ? GET_LANG(LCD_LANGUAGE_2)::MSG : \
    lang == 2 ? GET_LANG(LCD_LANGUAGE_3)::MSG : \
    lang == 3 ? GET_LANG(LCD_LANGUAGE_4)::MSG : \
                GET_LANG(LCD_LANGUAGE_5)::MSG \
    )
  #define MAX_LANG_CHARSIZE max(GET_LANG(LCD_LANGUAGE)::CHARSIZE, \
                                GET_LANG(LCD_LANGUAGE_2)::CHARSIZE, \
                                GET_LANG(LCD_LANGUAGE_3)::CHARSIZE, \
                                GET_LANG(LCD_LANGUAGE_4)::CHARSIZE, \
                                GET_LANG(LCD_LANGUAGE_5)::CHARSIZE)
#else
  #define GET_TEXT(MSG) GET_LANG(LCD_LANGUAGE)::MSG
  #define MAX_LANG_CHARSIZE GET_LANG(LCD_LANGUAGE)::CHARSIZE
#endif
#define GET_TEXT_F(MSG) (const __FlashStringHelper*)GET_TEXT(MSG)

#define MSG_CONCAT(A,B) pgm_p_pair_t(GET_TEXT(A),GET_TEXT(B))
