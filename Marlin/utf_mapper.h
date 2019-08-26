/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef UTF_MAPPER_H
#define UTF_MAPPER_H

#include "language.h"

#if ENABLED(DOGLCD)
  #define HARDWARE_CHAR_OUT(C) u8g.print((char)(C))
#else
  #define HARDWARE_CHAR_OUT(C) lcd.write((char)(C))
#endif

#if DISABLED(SIMULATE_ROMFONT) && ENABLED(DOGLCD)
  #if ENABLED(DISPLAY_CHARSET_ISO10646_1)     \
   || ENABLED(DISPLAY_CHARSET_ISO10646_5)     \
   || ENABLED(DISPLAY_CHARSET_ISO10646_KANA)  \
   || ENABLED(DISPLAY_CHARSET_ISO10646_GREEK) \
   || ENABLED(DISPLAY_CHARSET_ISO10646_TR)
    #define MAPPER_ONE_TO_ONE
  #endif
#else // SIMULATE_ROMFONT || !DOGLCD
  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    #if ENABLED(MAPPER_C2C3)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f          This is fair for symbols
               0x20,0x3F,0xEC,0xED,0x3F,0x5C,0x7C,0x3F,0x22,0x63,0x61,0x7F,0x3F,0x3F,0x52,0xB0,  // c2a
             //' '        Вў    ВЈ         В­     l         "    c    a    В«              R
               0xDF,0x3F,0x32,0x33,0x27,0xE4,0xF1,0xA5,0x2C,0x31,0xDF,0x7E,0x3F,0x3F,0x3F,0x3F,  // c2b but relatively bad for letters.
             // В°         2    3    `    N    p    .    ,    1    В°    В»
               0x3F,0x3F,0x3F,0x3F,0xE1,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,  // c38
             //                     Г¤
               0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0xEF,0x78,0x3F,0x3F,0x3F,0x3F,0xF5,0x3F,0x3F,0xE2,  // c39 missing characters display as '?'
             //                               Г¶    x                        Гј              Гџ
               0x3F,0x3F,0x3F,0x3F,0xE1,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,  // c3a
             //                     Г¤
               0x3F,0xEE,0x3F,0x3F,0x3F,0x3F,0xEF,0xFD,0x3F,0x3F,0x3F,0x3F,0xF5,0x3F,0x3F,0x3F   // c3b
             //      n                        Г¶    Г·                        Гј
           };
    #elif ENABLED(MAPPER_E382E383)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
               0x3D,0xB1,0xB1,0xA8,0xB2,0xA9,0xB3,0xAA,0xB4,0xAB,0xB5,0xB6,0xB6,0xB7,0xB7,0xB8,  // e382a Please test and correct
             // =    г‚ў    г‚ў    г‚Ј    г‚¤    г‚Ґ    г‚¦    г‚§    г‚Ё    г‚©    г‚Є   г‚¬    г‚¬    г‚­   г‚­    г‚Ї
               0xB8,0xB9,0xB9,0xBA,0xBA,0xBB,0xBB,0xBC,0xBC,0xBD,0xBD,0xBE,0xBE,0xBF,0xBF,0xC0,  // e382b
             // г‚Ї    г‚±    г‚±   г‚і    г‚і    г‚µ    г‚µ    г‚·    г‚·   г‚№    г‚№    г‚»    г‚»    г‚Ѕ   г‚Ѕ    г‚ї
               0xC0,0xC1,0xC1,0xC2,0xC2,0xC2,0xC3,0xC3,0xC4,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,  // e3838
             // г‚ї    гѓЃ    гѓЃ    гѓѓ    гѓѓ    гѓѓ    гѓ†   гѓ†    гѓ€    гѓ€    гѓЉ    гѓ‹    гѓЊ    гѓЌ   гѓЋ    гѓЏ
               0xCA,0xCA,0xCB,0xCB,0xCB,0xCC,0xCC,0xCC,0xCD,0xCD,0xCD,0xCE,0xCE,0xCE,0xCF,0xD0,  // e3839
             // гѓЏ    гѓЏ    гѓ’   гѓ’    гѓ’     гѓ•    гѓ•   гѓ•    гѓ�    гѓ�    гѓ�    гѓ›    гѓ›    гѓ›   гѓћ    гѓџ
               0xD1,0xD2,0xD3,0xD4,0xD4,0xD5,0xD5,0xAE,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDC,  // e383a
             // гѓ     гѓЎ    гѓў    гѓЈ   гѓЈ    гѓ¦    гѓ¦    гѓ§    гѓЁ    гѓ©    гѓЄ    гѓ«    гѓ¬    гѓ­   гѓЇ    гѓЇ
               0xEC,0xA7,0xA6,0xDD,0xCC,0x3F,0x3F,0x3F,0x3F,0x3F,0xA6,0xA5,0xB0,0xA4,0xA4,0x3F   // e383b
             // гѓ°    гѓ±    гѓІ    гѓі    гѓ•    ?    ?   ?    ?    ?    гѓІ    гѓ»    гѓј    гѓЅ    гѓЅ   ?
           };
    #elif ENABLED(MAPPER_D0D1)
      #error "Cyrillic on a JAPANESE display makes no sense. There are no matching symbols."
    #endif

  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    #if ENABLED(MAPPER_C2C3)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f   This is relative complete.
               0x20,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0x22,0xA9,0xAA,0xAB,0x3F,0x3F,0xAE,0x3F,  // c2aВ ВЎВўВЈВ¤ВҐВ¦В§ВЁВ©ВЄВ«В¬В­В®ВЇ
             //' '   ВЎ    Вў    ВЈ    В¤    ВҐ    В¦    В§    "    В©    ВЄ    В«    ?    ?    В®    ?
               0xB0,0xB1,0xB2,0xB3,0x27,0xB5,0xB6,0xB7,0x2C,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,  // c2b В°В±ВІВіВґВµВ¶В·ВёВ№ВєВ»ВјВЅВѕВї
             // В°    В±    Ві    ВІ    ?    Вµ    В¶    В·    ,    В№    Вє    В»    Вј    ВЅ    Вѕ    Вї
               0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,  // c38 ГЂГЃГѓГ„Г…Г†Г‡Г€Г‰ГЉГ‹ГЊГЌГЋГЏ
             // ГЂ    ГЃ    Г‚    Гѓ    Г„    Г…    Г†    Г‡    Г€    Г‰    ГЉ    Г‹    ГЊ    ГЌ    ГЋ    ГЏ
               0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,  // c39 ГђГ‘Г“Г”Г•Г–Г—Г�Г™ГљГ›ГњГќГћГџ
             // Гђ    Г‘    Г’    Г“    Г”    Г•    Г–    Г—    Г�    Г™    Гљ    Г›    Гњ    Гќ    Гћ    Гџ
               0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,  // c3a Г ГЎГЈГ¤ГҐГ¦Г§ГЁГ©ГЄГ«Г¬Г­Г®ГЇ
             // Г     ГЎ    Гў    ГЈ    Г¤    ГҐ    Г¦    Г§    ГЁ    Г©    ГЄ    Г«    Г¬    Г­    Г®    ГЇ
               0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF   // c3b Г°Г±ГіГґГµГ¶Г·ГёГ№ГєГ»ГјГЅГѕГї
             // Г°    Г±    ГІ    Гі    Гґ    Гµ    Г¶    Г·    Гё    Г№    Гє    Г»    Гј    ГЅ    Гѕ    Гї
           };
    #elif ENABLED(MAPPER_D0D1)
      #define MAPPER_D0D1_MOD
      const PROGMEM uint8_t utf_recode[] =
           {//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
               0x41,0x80,0x42,0x92,0x81,0x45,0x82,0x83,0x84,0x85,0x4B,0x86,0x4D,0x48,0x4F,0x87,  // d0a
             // A    Р‘    B    Р“    Р”    E    Р–    Р—    Р�    Р™    K    Р›    M    H    O    Рџ
               0x50,0x43,0x54,0x88,0xD8,0x58,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x62,0x8F,0xAC,0xAD,  // d0b
             // P    C    T    РЈ    Р¤    X    Р§    С‡    РЁ    Р©    РЄ    Р«    b    Р­    Р®    РЇ
               0x61,0x36,0x42,0x92,0x81,0x65,0x82,0xB3,0x84,0x85,0x6B,0x86,0x4D,0x48,0x6F,0x87,  // d18
             // a    6    B    Р“    Р”    e    Р–    Ві    Р�    Р™    k    Р›    M    H    o    Рџ
               0x70,0x63,0x54,0x79,0xD8,0x78,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x62,0x8F,0xAC,0xAD   // d19
             // p    c    T    y    Р¤    x    Р§    С‡    РЁ    Р©    РЄ    Р«    b    Р­    Р®    РЇ
            };
    #elif ENABLED(MAPPER_E382E383)
      #error "Katakana on a WESTERN display makes no sense. There are no matching symbols."
    #endif

  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    #if ENABLED(MAPPER_D0D1)
      #define MAPPER_D0D1_MOD
      // it is a Russian alphabet translation
      // except 0401 --> 0xA2 = РЃ, 0451 --> 0xB5 = С‘
      const PROGMEM uint8_t utf_recode[] =
               { 0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,   // unicode U+0400 to U+047f
               // A   Р‘->РЃ  B    Р“    Р”    E    Р–    Р—      // 0  РЂ РЃ Р‚ Рѓ Р„ Р… Р† Р‡
                 0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,   //    Р€ Р‰ РЉ Р‹ РЊ РЌ РЋ РЏ
               // Р�    Р™    K    Р›    M    H    O    Рџ      // 1  Рђ Р‘ Р’ Р“ Р” Р• Р– Р—
                 0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,   //    Р� Р™ Рљ Р› Рњ Рќ Рћ Рџ
               // P    C    T    РЈ    Р¤    X    Р§    С‡      // 2  Р  РЎ Рў РЈ Р¤ РҐ Р“ Р§
                 0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,   //    РЁ Р© РЄ Р« Р¬ Р­ Р® РЇ
               // РЁ    Р©    РЄ    Р«    b    Р­    Р®    РЇ      // 3  Р° Р± РІ Рі Рґ Рµ Р¶ Р·
                 0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,   //    Рё Р№ Рє Р» Рј РЅ Рѕ Рї
               // a   Р±->С‘  РІ    Рі    Рґ    e    Р¶    Р·      // 4  СЂ СЃ С‚ Сѓ С„ С… С† С‡
                 0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,   //    С€ С‰ СЉ С‹ СЊ СЌ СЋ СЏ
               // Рё    Р№    Рє    Р»    Рј    РЅ    o    Рї      // 5  Сђ С‘ С’ С“ С” С• С– С—
                 0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,   //    С� С™ Сљ С› Сњ Сќ Сћ Сџ
               // p    c    С‚    y    С„    x    С†    С‡      // 6  С  СЎ Сў СЈ С¤ СҐ С¦ С§
                 0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7    //    СЄ С© СЄ С« С¬ С­ С® СЇ
               // С€    С‰    СЉ    С‹    СЊ    СЌ    СЋ      СЏ      // 7  С° С± СІ Сі Сґ Сµ С¶ С·
             };                                             //    С» С№ Сє С» Сј СЅ Сѕ Сї
    #elif ENABLED(MAPPER_C2C3)
      #error "Western languages on a CYRILLIC display makes no sense. There are no matching symbols."
    #elif ENABLED(MAPPER_E382E383)
      #error "Katakana on a CYRILLIC display makes no sense. There are no matching symbols."
    #endif
  #else
    #error "Something went wrong in the setting of DISPLAY_CHARSET_HD44780"
  #endif // DISPLAY_CHARSET_HD44780
#endif // SIMULATE_ROMFONT

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80u)

#if ENABLED(MAPPER_C2C3)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_c2 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
      if (d >= 0xC0u && !seen_c2) {
        utf_hi_char = d - 0xC2u;
        seen_c2 = true;
        return 0;
      }
      else if (seen_c2) {
        d &= 0x3Fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0x80u + (utf_hi_char << 6) + d);
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_c2 = false;
    return 1;
  }

#elif ENABLED(MAPPER_C2C3_TR)

  // the C2C3-mapper extended for the 6 altered symbols from C4 and C5 range.

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_c2 = false,
                seen_c4 = false,
                seen_c5 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
           if (d == 0xC4u) { seen_c4 = true; return 0; }
      else if (d == 0xC5u) { seen_c5 = true; return 0; }
      else if (d >= 0xC0u && !seen_c2) {
        utf_hi_char = d - 0xC2u;
        seen_c2 = true;
        return 0;
      }
      else if (seen_c4) {
        switch(d) {
          case 0x9Eu: d = 0xD0u; break;
          case 0x9Fu: d = 0xF0u; break;
          case 0xB0u: d = 0xDDu; break;
          case 0xB1u: d = 0xFDu; break;
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c5) {
        switch(d) {
          case 0x9Eu: d = 0xDEu; break;
          case 0x9Fu: d = 0xFEu; break;
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c2) {
        d &= 0x3Fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0x80u + (utf_hi_char << 6) + d);
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_c2 = seen_c4 = seen_c5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_CECF)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_ce = false;
    uint8_t d = c;
    if (d >= 0x80) { // UTF-8 handling
      if (d >= 0xC0 && !seen_ce) {
        utf_hi_char = d - 0xCE;
        seen_ce = true;
        return 0;
      }
      else if (seen_ce) {
        d &= 0x3F;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0x80 + (utf_hi_char << 6) + d);
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_ce = false;
    return 1;
  }

#elif ENABLED(MAPPER_CECF)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_ce = false;
    uint8_t d = c;
    if (d >= 0x80) { // UTF-8 handling
      if (d >= 0xC0 && !seen_ce) {
        utf_hi_char = d - 0xCE;
        seen_ce = true;
        return 0;
      }
      else if (seen_ce) {
        d &= 0x3F;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0x80 + (utf_hi_char << 6) + d);
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_ce = false;
    return 1;
  }

#elif ENABLED(MAPPER_D0D1_MOD)

  char charset_mapper(const char c) {
    // it is a Russian alphabet translation
    // except 0401 --> 0xA2 = РЃ, 0451 --> 0xB5 = С‘
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_d5 = false;
    uint8_t d = c;
    if (d >= 0x80) { // UTF-8 handling
      if (d >= 0xD0 && !seen_d5) {
        utf_hi_char = d - 0xD0;
        seen_d5 = true;
        return 0;
      }
      else if (seen_d5) {
        d &= 0x3F;
        if (!utf_hi_char && d == 1) {
          HARDWARE_CHAR_OUT(0xA2); // РЃ
        }
        else if (utf_hi_char == 1 && d == 0x11) {
          HARDWARE_CHAR_OUT(0xB5); // С‘
        }
        else {
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x10));
        }
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_d5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_D0D1)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_d5 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
      if (d >= 0xD0u && !seen_d5) {
        utf_hi_char = d - 0xD0u;
        seen_d5 = true;
        return 0;
      }
      else if (seen_d5) {
        d &= 0x3Fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0xA0u + (utf_hi_char << 6) + d);
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT(c);
    }
    seen_d5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_E382E383)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_e3 = false,
                seen_82_83 = false;
    uint8_t d = c;
    if (d >= 0x80) { // UTF-8 handling
      if (d == 0xE3 && !seen_e3) {
        seen_e3 = true;
        return 0;      // eat 0xE3
      }
      else if (d >= 0x82 && seen_e3 && !seen_82_83) {
        utf_hi_char = d - 0x82;
        seen_82_83 = true;
        return 0;
      }
      else if (seen_e3 && seen_82_83) {
        d &= 0x3F;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT(pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT(0x80 + (utf_hi_char << 6) + d);
        #endif
      }
      else
        HARDWARE_CHAR_OUT('?');
    }
    else
      HARDWARE_CHAR_OUT(c);

    seen_e3 = false;
    seen_82_83 = false;
    return 1;
  }

#elif ENABLED(MAPPER_C3C4C5_PL)

  /**
   * Д„ C4 84 = 80
   * Д… C4 85 = 81
   * Д† C4 86 = 82
   * Д‡ C4 87 = 83
   * Д� C4 98 = 84
   * Д™ C4 99 = 85
   * ЕЃ C5 81 = 86
   * Е‚ C5 82 = 87
   * Еѓ C5 83 = 88
   * Е„ C5 84 = 89
   * Г“ C3 93 = 8A
   * Гі C3 B3 = 8B
   * Ељ C5 9A = 8C
   * Е› C5 9B = 8D
   * Е№ C5 B9 = 8E
   * Еє C5 BA = 8F
   * Е» C5 BB = 90
   * Еј C5 BC = 91
   */

  char charset_mapper(const char c) {
    static bool seen_c3 = false,
                seen_c4 = false,
                seen_c5 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
           if (d == 0xC4u) { seen_c4 = true; return 0; }
      else if (d == 0xC5u) { seen_c5 = true; return 0; }
      else if (d == 0xC3u) { seen_c3 = true; return 0; }
      else if (seen_c4) {
        switch(d) {
          case 0x84u ... 0x87u: d -= 4; break;  //Д„ - Д‡
          case 0x98u ... 0x99u: d -= 20; break; //Д� i Д™
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c5) {
        switch(d) {
          case 0x81u ... 0x84u: d += 5; break;  //ЕЃ - Е„
          case 0x9Au ... 0x9Bu: d -= 0x0Eu; break; //Ељ i Е›
          case 0xB9u ... 0xBCu: d -= 0x2Bu; break; //Е№ - Еј
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c3) {
        switch(d) {
          case 0x93u: d = 0x8Au; break; //Г“
          case 0xB3u: d = 0x8Bu; break; //Гі
          d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }

    }
    else
      HARDWARE_CHAR_OUT(c);

    seen_c3 = seen_c4 = seen_c5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_C3C4C5_CZ)

  /**
   * ГЃ C3 81 = 80
   * Г‰ C3 89 = 81
   * ГЌ C3 8D = 82
   * Г“ C3 93 = 83
   * Гљ C3 9A = 84
   * Гќ C3 9D = 85
   * ГЎ C3 A1 = 86
   * Г© C3 A9 = 87
   * Г­ C3 AD = 88
   * Гі C3 B3 = 89
   * Гє C3 BA = 8A
   * ГЅ C3 BD = 8B
   * ДЊ C4 8C = 8C
   * ДЌ C4 8D = 8D
   * ДЋ C4 8E = 8E
   * ДЏ C4 8F = 8F
   * Дљ C4 9A = 90
   * Д› C4 9B = 91
   * Е‡ C5 87 = 92
   * Е€ C5 88 = 93
   * Е� C5 98 = 94
   * Е™ C5 99 = 95
   * Е  C5 A0 = 96
   * ЕЎ C5 A1 = 97
   * Е¤ C5 A4 = 98
   * ЕҐ C5 A5 = 99
   * Е® C5 AE = 9A
   * ЕЇ C5 AF = 9B
   * ЕЅ C5 BD = 9C
   * Еѕ C5 BE = 9D
   */

  char charset_mapper(const char c) {
    static bool seen_c3 = false,
                seen_c4 = false,
                seen_c5 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
           if (d == 0xC4u) { seen_c4 = true; return 0; }
      else if (d == 0xC5u) { seen_c5 = true; return 0; }
      else if (d == 0xC3u) { seen_c3 = true; return 0; }
      else if (seen_c4) {
        switch(d) {
          case 0x8Cu ... 0x8Fu: break;          // ДЊДЌДЋДЏ Mapping 1:1
          case 0x9Au ... 0x9Bu: d -= 10; break; // ДљД›
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c5) {
        switch(d) {
          case 0x87u ... 0x88u: d += 0x0Bu; break;  // Е‡Е€
          case 0x98u ... 0x99u: d -= 0x04u; break;  // Е�Е™
          case 0xA0u ... 0xA1u: d -= 0x0Au; break;  // Е ЕЎ
          case 0xA4u ... 0xA5u: d -= 0x0Cu; break;  // Е¤ЕҐ
          case 0xAEu ... 0xAFu: d -= 0x14u; break;  // Е®ЕЇ
          case 0xBDu ... 0xBEu: d -= 0x21u; break;  // ЕЅЕѕ
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c3) {
        switch(d) {
          case 0x81u: d = 0x80u; break;  // ГЃ
          case 0x89u: d = 0x81u; break;  // Г‰
          case 0x8Du: d = 0x82u; break;  // ГЌ
          case 0x93u: d = 0x83u; break;  // Г“
          case 0x9Au: d = 0x84u; break;  // Гљ
          case 0x9Du: d = 0x85u; break;  // Гќ
          case 0xA1u: d = 0x86u; break;  // ГЎ
          case 0xA9u: d = 0x87u; break;  // Г©
          case 0xADu: d = 0x88u; break;  // Г­
          case 0xB3u: d = 0x89u; break;  // Гі
          case 0xBAu: d = 0x8Au; break;  // Гє
          case 0xBDu: d = 0x8Bu; break;  // ГЅ
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }

    }
    else
      HARDWARE_CHAR_OUT(c);

    seen_c3 = seen_c4 = seen_c5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_C3C4C5_SK)

  /**
   * ГЃ C3 81 = 80
   * Г„ C3 84 = 81
   * Г‰ C3 89 = 82
   * ГЌ C3 8D = 83
   * Г“ C3 93 = 84
   * Г” C3 94 = 85
   * Гљ C3 9A = 86
   * Гќ C3 9D = 87
   * ГЎ C3 A1 = 88
   * Г¤ C3 A4 = 89
   * Г© C3 A9 = 8A
   * Г­ C3 AD = 8B
   * Гі C3 B3 = 8C
   * Гґ C3 B4 = 8D
   * Гє C3 BA = 8E
   * ГЅ C3 BD = 8F
   * ДЊ C4 8C = 90
   * ДЌ C4 8D = 91
   * ДЋ C4 8E = 92
   * ДЏ C4 8F = 93
   * Д№ C4 B9 = 94
   * Дє C4 BA = 95
   * ДЅ C4 BD = 96
   * Дѕ C4 BE = 97
   * Е‡ C5 87 = 98
   * Е€ C5 88 = 99
   * Е” C5 94 = 9A
   * Е• C5 95 = 9B
   * Е  C5 A0 = 9C
   * ЕЎ C5 A1 = 9D
   * Е¤ C5 A4 = 9E
   * ЕҐ C5 A5 = 9F
   * ЕЅ C5 BD = A0
   * Еѕ C5 BE = A1
   */

  char charset_mapper(const char c) {
    static bool seen_c3 = false,
                seen_c4 = false,
                seen_c5 = false;
    uint8_t d = c;
    if (d >= 0x80u) { // UTF-8 handling
           if (d == 0xC4u) { seen_c4 = true; return 0; }
      else if (d == 0xC5u) { seen_c5 = true; return 0; }
      else if (d == 0xC3u) { seen_c3 = true; return 0; }
      else if (seen_c4) {
        switch(d) {
          case 0x8Cu ... 0x8Fu: d += 0x04u; break;  // ДЊДЌДЋДЏ
          case 0xB9u ... 0xBAu: d -= 0x25u; break;  // Д№Дє
          case 0xBDu ... 0xBEu: d -= 0x27u; break;  // ДЅДѕ
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c5) {
        switch(d) {
          case 0x87u ... 0x88u: d += 0x11u; break;  // Е‡Е€
          case 0x94u ... 0x95u: d += 0x06u; break;  // Е”Е•
          case 0xA0u ... 0xA1u: d -= 0x04u; break;  // Е ЕЎ
          case 0xA4u ... 0xA5u: d -= 0x06u; break;  // Е¤ЕҐ
          case 0xBDu ... 0xBEu: d -= 0x1Du; break;  // ЕЅЕѕ
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }
      else if (seen_c3) {
        switch(d) {
          case 0x81u: d = 0x80u; break;  // ГЃ
          case 0x84u: d = 0x81u; break;  // Г„
          case 0x89u: d = 0x82u; break;  // Г‰
          case 0x8Du: d = 0x83u; break;  // ГЌ
          case 0x93u: d = 0x84u; break;  // Г“
          case 0x94u: d = 0x85u; break;  // Г”
          case 0x9Au: d = 0x86u; break;  // Гљ
          case 0x9Du: d = 0x87u; break;  // Гќ
          case 0xA1u: d = 0x88u; break;  // ГЎ
          case 0xA4u: d = 0x89u; break;  // Г¤
          case 0xA9u: d = 0x8Au; break;  // Г©
          case 0xADu: d = 0x8Bu; break;  // Г­
          case 0xB3u: d = 0x8Cu; break;  // Гі
          case 0xB4u: d = 0x8Du; break;  // Гґ
          case 0xBAu: d = 0x8Eu; break;  // Гє
          case 0xBDu: d = 0x8Fu; break;  // ГЅ
          default: d = '?';
        }
        HARDWARE_CHAR_OUT(d);
      }

    }
    else
      HARDWARE_CHAR_OUT(c);

    seen_c3 = seen_c4 = seen_c5 = false;
    return 1;
  }

#else

  #define MAPPER_NON

  #undef PRINTABLE
  #define PRINTABLE(C) true

  char charset_mapper(const char c) {
    HARDWARE_CHAR_OUT(c);
    return 1;
  }

#endif // code mappers

#endif // UTF_MAPPER_H

