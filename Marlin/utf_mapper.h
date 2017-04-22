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

#include  "language.h"

#if ENABLED(DOGLCD)
  #define HARDWARE_CHAR_OUT u8g.print
#else
  #define HARDWARE_CHAR_OUT lcd.write
#endif

#if DISABLED(SIMULATE_ROMFONT) && ENABLED(DOGLCD)
  #if ENABLED(DISPLAY_CHARSET_ISO10646_1)     \
   || ENABLED(DISPLAY_CHARSET_ISO10646_5)     \
   || ENABLED(DISPLAY_CHARSET_ISO10646_KANA)  \
   || ENABLED(DISPLAY_CHARSET_ISO10646_GREEK) \
   || ENABLED(DISPLAY_CHARSET_ISO10646_TR)
    #define MAPPER_ONE_TO_ONE
  #endif
#else // SIMULATE_ROMFONT
  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    #if ENABLED(MAPPER_C2C3)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f          This is fair for symbols
               0x20,0x3F,0xEC,0xED,0x3F,0x5C,0x7C,0x3F,0x22,0x63,0x61,0x7F,0x3F,0x3F,0x52,0xB0,  // c2a
             //' '        ¢    £         ­     l         "    c    a    «              R
               0xDF,0x3F,0x32,0x33,0x27,0xE4,0xF1,0xA5,0x2C,0x31,0xDF,0x7E,0x3F,0x3F,0x3F,0x3F,  // c2b but relatively bad for letters.
             // °         2    3    `    N    p    .    ,    1    °    »
               0x3F,0x3F,0x3F,0x3F,0xE1,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,  // c38
             //                     ä
               0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0xEF,0x78,0x3F,0x3F,0x3F,0x3F,0xF5,0x3F,0x3F,0xE2,  // c39 missing characters display as '?'
             //                               ö    x                        ü              ß
               0x3F,0x3F,0x3F,0x3F,0xE1,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,  // c3a
             //                     ä
               0x3F,0xEE,0x3F,0x3F,0x3F,0x3F,0xEF,0xFD,0x3F,0x3F,0x3F,0x3F,0xF5,0x3F,0x3F,0x3F   // c3b
             //      n                        ö    ÷                        ü
           };
    #elif ENABLED(MAPPER_E382E383)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
               0x3D,0xB1,0xB1,0xA8,0xB2,0xA9,0xB3,0xAA,0xB4,0xAB,0xB5,0xB6,0xB6,0xB7,0xB7,0xB8,  // e382a Please test and correct
             // =    ア    ア    ィ    イ    ゥ    ウ    ェ    エ    ォ    オ   ガ    ガ    キ   キ    ク
               0xB8,0xB9,0xB9,0xBA,0xBA,0xBB,0xBB,0xBC,0xBC,0xBD,0xBD,0xBE,0xBE,0xBF,0xBF,0xC0,  // e382b
             // ク    ケ    ケ   コ    コ    サ    サ    シ    シ   ス    ス    セ    セ    ソ   ソ    タ
               0xC0,0xC1,0xC1,0xC2,0xC2,0xC2,0xC3,0xC3,0xC4,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,  // e3838
             // タ    チ    チ    ッ    ッ    ッ    テ   テ    ト    ト    ナ    ニ    ヌ    ネ   ノ    ハ
               0xCA,0xCA,0xCB,0xCB,0xCB,0xCC,0xCC,0xCC,0xCD,0xCD,0xCD,0xCE,0xCE,0xCE,0xCF,0xD0,  // e3839
             // ハ    ハ    ヒ   ヒ    ヒ     フ    フ   フ    ヘ    ヘ    ヘ    ホ    ホ    ホ   マ    ミ
               0xD1,0xD2,0xD3,0xD4,0xD4,0xD5,0xD5,0xAE,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDC,  // e383a
             // ム    メ    モ    ャ   ャ    ユ    ユ    ョ    ヨ    ラ    リ    ル    レ    ロ   ワ    ワ
               0xEC,0xA7,0xA6,0xDD,0xCC,0x3F,0x3F,0x3F,0x3F,0x3F,0xA6,0xA5,0xB0,0xA4,0xA4,0x3F   // e383b
             // ヰ    ヱ    ヲ    ン    フ    ?    ?   ?    ?    ?    ヲ    ・    ー    ヽ    ヽ   ?
           };
    #elif ENABLED(MAPPER_D0D1)
      #error "Cyrillic on a JAPANESE display makes no sense. There are no matching symbols."
    #endif

  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    #if ENABLED(MAPPER_C2C3)
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f   This is relative complete.
               0x20,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0x22,0xA9,0xAA,0xAB,0x3F,0x3F,0xAE,0x3F,  // c2a ¡¢£¤¥¦§¨©ª«¬­®¯
             //' '   ¡    ¢    £    ¤    ¥    ¦    §    "    ©    ª    «    ?    ?    ®    ?
               0xB0,0xB1,0xB2,0xB3,0x27,0xB5,0xB6,0xB7,0x2C,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,  // c2b °±²³´µ¶·¸¹º»¼½¾¿
             // °    ±    ³    ²    ?    µ    ¶    ·    ,    ¹    º    »    ¼    ½    ¾    ¿
               0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,  // c38 ÀÁÃÄÅÆÇÈÉÊËÌÍÎÏ
             // À    Á    Â    Ã    Ä    Å    Æ    Ç    È    É    Ê    Ë    Ì    Í    Î    Ï
               0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,  // c39 ÐÑÓÔÕÖ×ØÙÚÛÜÝÞß
             // Ð    Ñ    Ò    Ó    Ô    Õ    Ö    ×    Ø    Ù    Ú    Û    Ü    Ý    Þ    ß
               0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,  // c3a àáãäåæçèéêëìíîï
             // à    á    â    ã    ä    å    æ    ç    è    é    ê    ë    ì    í    î    ï
               0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF   // c3b ðñóôõö÷øùúûüýþÿ
             // ð    ñ    ò    ó    ô    õ    ö    ÷    ø    ù    ú    û    ü    ý    þ    ÿ
           };
    #elif ENABLED(MAPPER_D0D1)
      #define MAPPER_D0D1_MOD
      const PROGMEM uint8_t utf_recode[] =
           {//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
               0x41,0x80,0x42,0x92,0x81,0x45,0x82,0x83,0x84,0x85,0x4B,0x86,0x4D,0x48,0x4F,0x87,  // d0a
             // A    Б    B    Г    Д    E    Ж    З    И    Й    K    Л    M    H    O    П
               0x50,0x43,0x54,0x88,0xD8,0x58,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x62,0x8F,0xAC,0xAD,  // d0b
             // P    C    T    У    Ф    X    Ч    ч    Ш    Щ    Ъ    Ы    b    Э    Ю    Я
               0x61,0x36,0x42,0x92,0x81,0x65,0x82,0xB3,0x84,0x85,0x6B,0x86,0x4D,0x48,0x6F,0x87,  // d18
             // a    6    B    Г    Д    e    Ж    ³    И    Й    k    Л    M    H    o    П
               0x70,0x63,0x54,0x79,0xD8,0x78,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x62,0x8F,0xAC,0xAD   // d19
             // p    c    T    y    Ф    x    Ч    ч    Ш    Щ    Ъ    Ы    b    Э    Ю    Я
            };
    #elif ENABLED(MAPPER_E382E383)
      #error "Katakana on a WESTERN display makes no sense. There are no matching symbols."
    #endif

  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    #if ENABLED(MAPPER_D0D1)
      #define MAPPER_D0D1_MOD
      // it is a Russian alphabet translation
      // except 0401 --> 0xA2 = Ё, 0451 --> 0xB5 = ё
      const PROGMEM uint8_t utf_recode[] =
               { 0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,   // unicode U+0400 to U+047f
               // A   Б->Ё  B    Г    Д    E    Ж    З      // 0  Ѐ Ё Ђ Ѓ Є Ѕ І Ї
                 0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,   //    Ј Љ Њ Ћ Ќ Ѝ Ў Џ
               // И    Й    K    Л    M    H    O    П      // 1  А Б В Г Д Е Ж З
                 0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,   //    И Й К Л М Н О П
               // P    C    T    У    Ф    X    Ч    ч      // 2  Р С Т У Ф Х Г Ч
                 0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,   //    Ш Щ Ъ Ы Ь Э Ю Я
               // Ш    Щ    Ъ    Ы    b    Э    Ю    Я      // 3  а б в г д е ж з
                 0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,   //    и й к л м н о п
               // a   б->ё  в    г    д    e    ж    з      // 4  р с т у ф х ц ч
                 0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,   //    ш щ ъ ы ь э ю я
               // и    й    к    л    м    н    o    п      // 5  ѐ ё ђ ѓ є ѕ і ї
                 0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,   //    ј љ њ ћ ќ ѝ ў џ
               // p    c    т    y    ф    x    ц    ч      // 6  Ѡ ѡ Ѣ ѣ Ѥ ѥ Ѧ ѧ
                 0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7    //    Ѫ ѩ Ѫ ѫ Ѭ ѭ Ѯ ѯ
               // ш    щ    ъ    ы    ь    э    ю      я      // 7  Ѱ ѱ Ѳ ѳ Ѵ ѵ Ѷ ѷ
             };                                             //    ѻ ѹ Ѻ ѻ Ѽ ѽ Ѿ ѿ
    #elif ENABLED(MAPPER_C2C3)
      #error "Western languages on a CYRILLIC display makes no sense. There are no matching symbols."
    #elif ENABLED(MAPPER_E382E383)
      #error "Katakana on a CYRILLIC display makes no sense. There are no matching symbols."
    #endif
  #else
    #error "Something went wrong in the setting of DISPLAY_CHARSET_HD44780"
  #endif // DISPLAY_CHARSET_HD44780
#endif // SIMULATE_ROMFONT

#if ENABLED(MAPPER_C2C3)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_c2 = false;
    uint8_t d = c;
    if ( d >= 0x80u ) { // UTF-8 handling
      if ( (d >= 0xC0u) && (!seen_c2) ) {
        utf_hi_char = d - 0xC2u;
        seen_c2 = true;
        return 0;
      }
      else if (seen_c2) {
        d &= 0x3Fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0x80u + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
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
    if ( d >= 0x80u ) { // UTF-8 handling
           if ( d == 0xC4u ) {seen_c4 = true; return 0;}
      else if ( d == 0xC5u ) {seen_c5 = true; return 0;}
      else if ( (d >= 0xC0u) && (!seen_c2) ) {
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
        HARDWARE_CHAR_OUT((char)d) ;
      }
      else if (seen_c5) {
        switch(d) {
          case 0x9Eu: d = 0xDEu; break;
          case 0x9Fu: d = 0xFEu; break;
          default: d = '?';
        }
        HARDWARE_CHAR_OUT((char)d) ;
      }
      else if (seen_c2) {
        d &= 0x3Fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0x80u + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_c2 = seen_c4 = seen_c5 = false;
    return 1;
  }

#elif ENABLED(MAPPER_CECF)

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_ce = false;
    uint8_t d = c;
    if ( d >= 0x80 ) { // UTF-8 handling
      if ( (d >= 0xC0) && (!seen_ce) ) {
        utf_hi_char = d - 0xCE;
        seen_ce = true;
        return 0;
      }
      else if (seen_ce) {
        d &= 0x3F;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0x80 + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char)c);
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
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0x80 + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_ce = false;
    return 1;
  }

#elif ENABLED(MAPPER_D0D1_MOD)

  char charset_mapper(const char c) {
    // it is a Russian alphabet translation
    // except 0401 --> 0xA2 = Ё, 0451 --> 0xB5 = ё
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
          HARDWARE_CHAR_OUT((char) 0xA2); // Ё
        }
        else if (utf_hi_char == 1 && d == 0x11) {
          HARDWARE_CHAR_OUT((char)0xB5); // ё
        }
        else {
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x10));
        }
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
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
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0xA0u + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT('?');
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
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
          HARDWARE_CHAR_OUT((char)pgm_read_byte_near(utf_recode + d + (utf_hi_char << 6) - 0x20));
        #else
          HARDWARE_CHAR_OUT((char)(0x80 + (utf_hi_char << 6) + d)) ;
        #endif
      }
      else {
        HARDWARE_CHAR_OUT((char) '?' );
      }
    }
    else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_e3 = false;
    seen_82_83 = false;
    return 1;
  }

#elif ENABLED(MAPPER_C3C4C5_PL)

  //Ą c4 84 = 80
  //ą c4 85 = 81
  //Ć c4 86 = 82
  //ć c4 87 = 83
  //Ę c4 98 = 84
  //ę c4 99 = 85
  //Ł c5 81 = 86
  //ł c5 82 = 87
  //Ń c5 83 = 88
  //ń c5 84 = 89
  //Ó c3 93 = 8a
  //ó c3 b3 = 8b
  //Ś c5 9a = 8c
  //ś c5 9b = 8d
  //Ź c5 b9 = 8e
  //ź c5 ba = 8f
  //Ż c5 bb = 90
  //ż c5 bc = 91

  char charset_mapper(const char c) {
    static uint8_t utf_hi_char; // UTF-8 high part
    static bool seen_c3 = false,
                seen_c4 = false,
                seen_c5 = false;
    uint8_t d = c;
    if ( d >= 0x80u ) { // UTF-8 handling
           if ( d == 0xC4u ) {seen_c4 = true; return 0;}
      else if ( d == 0xC5u ) {seen_c5 = true; return 0;}
      else if ( d == 0xC3u ) {seen_c3 = true; return 0;}
      else if (seen_c4) {
        switch(d) {
          case 0x84u ... 0x87u: d -= 4; break;  //Ą - ć
          case 0x98u ... 0x99u: d -= 20; break; //Ę i ę
          default: d = '?';
        }
        HARDWARE_CHAR_OUT((char)d) ;
      }
      else if (seen_c5) {
        switch(d) {
          case 0x81u ... 0x84u: d += 5; break;  //Ł - ń
          case 0x9Au ... 0x9Bu: d -= 0x0Eu; break; //Ś i ś
          case 0xB9u ... 0xBCu: d -= 0x2Bu; break; //Ź - ż
          default: d = '?';
        }
        HARDWARE_CHAR_OUT((char)d) ;
      }
      else if (seen_c3) {
    switch(d) {
          case 0x93u: d = 0x8Au; break; //Ó
          case 0xB3u: d = 0x8Bu; break; //ó
          d = '?';
        }
        HARDWARE_CHAR_OUT((char)d) ;
      }

    }
    else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_c3 = seen_c4 = seen_c5 = false;
    return 1;
  }

#else

  #define MAPPER_NON

  char charset_mapper(const char c) {
    HARDWARE_CHAR_OUT( c );
    return 1;
  }

  #endif // code mappers

#endif // UTF_MAPPER_H
