/*****************
 * language.cpp *
 *****************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#include "../compat.h"

#if ENABLED(LULZBOT_TOUCH_UI) && defined(TOUCH_UI_LANGUAGE_MENU)

  #include "language_de.h"
  #include "language_en.h"
  #include "language_fr.h"

  PROGMEM Language_List languages = {
    &Language_de::strings,
    &Language_en::strings,
    &Language_fr::strings
  };

  uint8_t get_language_count() {
    return sizeof(languages)/sizeof(languages[0]);
  }

  static uint8_t lang = 0;

  void set_language(uint8_t l) {
    lang = l;
  };

  const char *get_text(uint8_t lang, String_Indices index) {
    const Language_Strings* strings = (const Language_Strings*) pgm_read_ptr(&languages[lang]);
    return (const char *)pgm_read_ptr(&(*strings)[int(index)]);
  };

  const char *get_text(String_Indices index) {
    return get_text(lang, index);
  };
#endif
