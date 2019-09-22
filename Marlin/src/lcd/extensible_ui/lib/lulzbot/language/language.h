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

enum class String_Indices { LANGUAGE_STRINGS, COUNT };

typedef const char Language_Str[];
typedef const char* const Language_Strings[int(String_Indices::COUNT)];
typedef const Language_Strings* const Language_List[];

#ifndef TOUCH_UI_LANGUAGE_MENU
  // Default mode, support only one language.
  #define __GET_TEXTF(MSG,LANG) Language_##LANG::MSG
  #define _GET_TEXTF(MSG,LANG) __GET_TEXTF(MSG,LANG)
  #define GET_TEXTF(MSG) reinterpret_cast<const __FlashStringHelper *>(_GET_TEXTF(MSG,LCD_LANGUAGE))
  #define GET_TEXT(MSG) _GET_TEXTF(MSG,LCD_LANGUAGE)
  #define MAKE_LANGUAGE_STRINGS()
#else
  // Support multiple languages at run-time.
  uint8_t get_language_count();
  void set_language(uint8_t index);
  const char *get_text(String_Indices index);
  const char *get_text(uint8_t lang, String_Indices index);
  #define GET_TEXT(MSG) get_text(String_Indices::MSG)
  #define GET_TEXTF(MSG) reinterpret_cast<const __FlashStringHelper *>(get_text(String_Indices::MSG))
  #define MAKE_LANGUAGE_STRINGS() PROGMEM Language_Strings strings = { LANGUAGE_STRINGS }
#endif