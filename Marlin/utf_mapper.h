#ifndef UTF_MAPPER_H
#define UTF_MAPPER_H

#include  "language.h"

#ifdef DOGLCD
  #define HARDWARE_CHAR_OUT u8g.print
#else
  #define HARDWARE_CHAR_OUT lcd.write
#endif

#if !(defined( SIMULATE_ROMFONT )) && defined( DOGLCD )
  #if defined( DISPLAY_CHARSET_ISO10646_1 )
    #define MAPPER_ONE_TO_ONE
  #elif defined( DISPLAY_CHARSET_ISO10646_5 )
    #define MAPPER_ONE_TO_ONE
  #elif defined( DISPLAY_CHARSET_ISO10646_KANA )
    #define MAPPER_ONE_TO_ONE
  #endif
#else // SIMULATE_ROMFONT
  #if defined( DISPLAY_CHARSET_HD44780_JAPAN )
    #if defined( MAPPER_C2C3 )
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f          This is fair for symbols
             0x20,0x3f,0xec,0xed,0x3f,0x5c,0x7c,0x3f,0x22,0x63,0x61,0x7f,0x3f,0x3f,0x52,0xb0,  // c2a
  //          ' '       ¢    £         ­    l         "    c    a    «              R
             0xdf,0x3f,0x32,0x33,0x27,0xe4,0xf1,0xa5,0x2c,0x31,0xdf,0x7e,0x3f,0x3f,0x3f,0x3f,  // c2b but relatively bad for letters.
  //          °         2    3    `    N    p    .    ,    1    °    »
             0x3f,0x3f,0x3f,0x3f,0xe1,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,  // c38
  //                              ä
             0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0xef,0x78,0x3f,0x3f,0x3f,0x3f,0xf5,0x3f,0x3f,0xe2,  // c39 missing characters display as '?'
  //                                        ö     x                       ü              ß
             0x3f,0x3f,0x3f,0x3f,0xe1,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,  // c3a
  //                              ä
             0x3f,0xee,0x3f,0x3f,0x3f,0x3f,0xef,0xfd,0x3f,0x3f,0x3f,0x3f,0xf5,0x3f,0x3f,0x3f   // c3b
  //               n                        ö    ÷                        ü
           };
    #elif defined( MAPPER_E382E383 )
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
             0x3d,0xb1,0xb1,0xa8,0xb2,0xa9,0xb3,0xaa,0xb4,0xab,0xb5,0xb6,0xb6,0xb7,0xb7,0xb8,  // e382a Please test and correct
  //          =    ア    ア    ィ    イ    ゥ    ウ    ェ    エ    ォ    オ   ガ    ガ    キ   キ    ク
             0xb8,0xb9,0xb9,0xba,0xba,0xbb,0xbb,0xbc,0xbc,0xbd,0xbd,0xbe,0xbe,0xbf,0xbf,0xc0,  // e382b
  //          ク    ケ    ケ   コ    コ    サ    サ    シ    シ   ス    ス    セ    セ    ソ   ソ    タ
             0xc0,0xc1,0xc1,0xc2,0xc2,0xc2,0xc3,0xc3,0xc4,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,  // e3838
  //          タ    チ    チ    ッ    ッ    ッ    テ   テ    ト    ト    ナ    ニ    ヌ    ネ   ノ    ハ
             0xca,0xca,0xcb,0xcb,0xcb,0xcc,0xcc,0xcc,0xcd,0xcd,0xcd,0xce,0xce,0xce,0xcf,0xd0,  // e3839
  //          ハ    ハ    ヒ   ヒ    ヒ     フ    フ   フ    ヘ    ヘ    ヘ    ホ    ホ    ホ   マ    ミ
             0xd1,0xd2,0xd3,0xd4,0xd4,0xd5,0xd5,0xae,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdc,  // e383a
  //          ム    メ    モ    ャ   ャ    ユ    ユ    ョ    ヨ    ラ    リ    ル    レ    ロ   ワ    ワ
             0xec,0xa7,0xa6,0xdd,0xcc,0x3f,0x3f,0x3f,0x3f,0x3f,0xa6,0xa5,0xb0,0xa4,0xa4,0x3f   // e383b
  //          ヰ    ヱ    ヲ    ン    フ    ?    ?   ?    ?    ?    ヲ    ・    ー    ヽ    ヽ   ?
           };
    #elif defined( MAPPER_D0D1 )
      #error( "Cyrillic on a japanese dsplay makes no sense. There are no matching symbols.");
    #endif

  #elif defined( DISPLAY_CHARSET_HD44780_WESTERN )
    #if defined( MAPPER_C2C3 )
    :
      const PROGMEM uint8_t utf_recode[] =
           { // 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f   This is relative complete.
             0x20,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0x22,0xa9,0xaa,0xab,0x3f,0x3f,0xae,0x3f,  // c2a ¡¢£¤¥¦§¨©ª«¬­®¯
  //         ' '   ¡    ¢    £    ¤    ¥    ¦    §    "    ©    ª    «    ?    ?    ®    ?
             0xb0,0xb1,0xb2,0xb3,0x27,0xb5,0xb6,0xb7,0x2c,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,  // c2b °±²³´µ¶·¸¹º»¼½¾¿
  //          °    ±    ³    ²    ?    µ    ¶    ·    ,    ¹    º    »    ¼    ½    ¾    ¿
             0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,  // c38 ÀÁÃÄÅÆÇÈÉÊËÌÍÎÏ
  //          À    Á    Â    Ã    Ä    Å    Æ    Ç    È    É    Ê    Ë    Ì    Í    Î    Ï
             0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,  // c39 ÐÑÓÔÕÖ×ØÙÚÛÜÝÞß
  //          Ð    Ñ    Ò    Ó    Ô    Õ    Ö    ×    Ø    Ù    Ú    Û    Ü    Ý    Þ    ß
             0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,  // c3a àáãäåæçèéêëìíîï 
  //          à    á    â    ã    ä    å    æ    ç    è    é    ê    ë    ì    í    î    ï
             0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff   // c3b ðñóôõö÷øùúûüýþÿ
  //          ð    ñ    ò    ó    ô    õ    ö    ÷    ø    ù    ú    û    ü    ý    þ    ÿ
           };
    #elif defined( MAPPER_D0D1 )
      #define MAPPER_D0D1_MOD
      const PROGMEM uint8_t utf_recode[] =
           {//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
             0x41,0x80,0x42,0x92,0x81,0x45,0x82,0x83,0x84,0x85,0x4b,0x86,0x4d,0x48,0x4f,0x87,  // d0a
  //          A    Б    B    Г    Д    E    Ж    З    И    Й    K    Л    M    H    O    П
             0x50,0x43,0x54,0x88,0xd8,0x58,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x62,0x8f,0xac,0xad,  // d0b
  //          P    C    T    У    Ф    X    Ч    ч    Ш    Щ    Ъ    Ы    b    Э    Ю    Я
             0x61,0x36,0x42,0x92,0x81,0x65,0x82,0xb3,0x84,0x85,0x6b,0x86,0x4d,0x48,0x6f,0x87,  // d18
  //          a    6    B    Г    Д    e    Ж    ³    И    Й    k    Л    M    H    o    П
             0x70,0x63,0x54,0x79,0xd8,0x78,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x62,0x8f,0xac,0xad   // d19
  //          p    c    T    y    Ф    x    Ч    ч    Ш    Щ    Ъ    Ы    b    Э    Ю    Я
            };
    #elif defined( MAPPER_E382E383 )
      #error( "Katakana on a western display makes no sense. There are no matching symbols." );
    #endif

  #elif defined( DISPLAY_CHARSET_HD44780_CYRILLIC )
    #if defined( MAPPER_D0D1 )
      #define MAPPER_D0D1_MOD
      // it is a Russian alphabet translation
      // except 0401 --> 0xa2 = Ё, 0451 --> 0xb5 = ё
      const PROGMEM uint8_t utf_recode[] =
             { 0x41,0xa0,0x42,0xa1,0xe0,0x45,0xa3,0xa4,   // unicode U+0400 to U+047f
  //            A   Б->Ё  B    Г    Д    E    Ж    З      // 0  Ѐ Ё Ђ Ѓ Є Ѕ І Ї
               0xa5,0xa6,0x4b,0xa7,0x4d,0x48,0x4f,0xa8,   //    Ј Љ Њ Ћ Ќ Ѝ Ў Џ
  //            И    Й    K    Л    M    H    O    П      // 1  А Б В Г Д Е Ж З
               0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,   //    И Й К Л М Н О П
  //            P    C    T    У    Ф    X    Ч    ч      // 2  Р С Т У Ф Х Г Ч
               0xac,0xe2,0xad,0xae,0x62,0xaf,0xb0,0xb1,   //    Ш Щ Ъ Ы Ь Э Ю Я
  //            Ш    Щ    Ъ    Ы    b    Э    Ю    Я      // 3  а б в г д е ж з
               0x61,0xb2,0xb3,0xb4,0xe3,0x65,0xb6,0xb7,   //    и й к л м н о п
  //            a   б->ё  в    г    д    e    ж    з      // 4  р с т у ф х ц ч
               0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x6f,0xbe,   //    ш щ ъ ы ь э ю я
  //            и    й    к    л    м    н    o    п      // 5  ѐ ё ђ ѓ є ѕ і ї
               0x70,0x63,0xbf,0x79,0xe4,0x78,0xe5,0xc0,   //    ј љ њ ћ ќ ѝ ў џ
  //            p    c    т    y    ф    x    ц    ч      // 6  Ѡ ѡ Ѣ ѣ Ѥ ѥ Ѧ ѧ
               0xc1,0xe6,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7    //    Ѫ ѩ Ѫ ѫ Ѭ ѭ Ѯ ѯ
  //            ш    щ    ъ    ы    ь    э    ю    я      // 7  Ѱ ѱ Ѳ ѳ Ѵ ѵ Ѷ ѷ
             };                                           //    ѻ ѹ Ѻ ѻ Ѽ ѽ Ѿ ѿ
    #elif defined( MAPPER_C2C3 )
      #error( "Western languages on a cyrillic display makes no sense. There are no matching symbols." );
    #elif defined( MAPPER_E382E383 )
      #error( "Katakana on a cyrillic display makes no sense. There are no matching symbols." );
    #endif
  #else
    #error("Something went wrong in the selection of DISPLAY_CHARSET_HD44780's");
  #endif // DISPLAY_CHARSET_HD44780_CYRILLIC
#endif // SIMULATE_ROMFONT

#if defined( MAPPER_NON )
  char charset_mapper(char c){
    HARDWARE_CHAR_OUT( c );
    return 1;
  }
#elif defined( MAPPER_C2C3 )
  uint8_t utf_hi_char; // UTF-8 high part
  bool seen_c2 = false;
  char charset_mapper(char c){
    uint8_t d = c;
    if ( d >= 0x80 ) { // UTF-8 handling
      if ( (d >= 0xc0) && (!seen_c2) ) {
        utf_hi_char = d - 0xc2;
        seen_c2 = true;
        return 0;
      }
      else if (seen_c2){
        d &= 0x3f;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT( (char) pgm_read_byte_near( utf_recode + d + ( utf_hi_char << 6 ) - 0x20 ) );
        #else
          HARDWARE_CHAR_OUT( (char) (0x80 + ( utf_hi_char << 6 ) + d) ) ;
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
#elif defined( MAPPER_D0D1_MOD )
  uint8_t utf_hi_char; // UTF-8 high part
  bool seen_d5 = false;
  char charset_mapper(char c){
    // it is a Russian alphabet translation
    // except 0401 --> 0xa2 = Ё, 0451 --> 0xb5 = ё
    uint8_t d = c;
    if ( d >= 0x80 ) { // UTF-8 handling
      if ((d >= 0xd0) && (!seen_d5)) {
        utf_hi_char = d - 0xd0;
        seen_d5 = true;
        return 0;
      } else if (seen_d5) {
          d &= 0x3f;
          if ( !utf_hi_char && ( d == 1 )) {
            HARDWARE_CHAR_OUT((char) 0xa2 ); // Ё
        } else if ((utf_hi_char == 1) && (d == 0x11)) {
            HARDWARE_CHAR_OUT((char) 0xb5 ); // ё
          } else {
            HARDWARE_CHAR_OUT((char) pgm_read_byte_near( utf_recode + d + ( utf_hi_char << 6 ) - 0x10 ) );
          }
        }
        else {
          HARDWARE_CHAR_OUT('?');
        }
    } else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_d5 = false;
    return 1;
  }
#elif defined( MAPPER_D0D1 )
  uint8_t utf_hi_char; // UTF-8 high part
  bool seen_d5 = false;
  char charset_mapper(char c){
    uint8_t d = c;
    if ( d >= 0x80u ) { // UTF-8 handling
      if ((d >= 0xd0u) && (!seen_d5)) {
        utf_hi_char = d - 0xd0u;
        seen_d5 = true;
        return 0;
      } else if (seen_d5) {
          d &= 0x3fu;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT( (char) pgm_read_byte_near( utf_recode + d + ( utf_hi_char << 6 ) - 0x20 ) );
        #else
          HARDWARE_CHAR_OUT( (char) (0xa0u + ( utf_hi_char << 6 ) + d ) ) ;
        #endif
      } else {
        HARDWARE_CHAR_OUT('?');
      }
    } else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_d5 = false;
    return 1;
  }
#elif defined( MAPPER_E382E383 )
  uint8_t utf_hi_char; // UTF-8 high part
  bool seen_e3 = false;
  bool seen_82_83 = false;
  char charset_mapper(char c){
  uint8_t d = c;
    if ( d >= 0x80 ) { // UTF-8 handling
      if ( (d == 0xe3) && (seen_e3 == false)) {
        seen_e3 = true;
        return 0;      // eat 0xe3
      } else if ( (d >= 0x82) && (seen_e3 == true) && (seen_82_83 == false)) {
        utf_hi_char = d - 0x82;
        seen_82_83 = true;
        return 0;
      } else if ((seen_e3 == true) && (seen_82_83 == true)){
        d &= 0x3f;
        #ifndef MAPPER_ONE_TO_ONE
          HARDWARE_CHAR_OUT( (char) pgm_read_byte_near( utf_recode + d + ( utf_hi_char << 6 ) - 0x20 ) );
        #else
          HARDWARE_CHAR_OUT( (char) (0x80 + ( utf_hi_char << 6 ) + d ) ) ;
        #endif
      } else {
        HARDWARE_CHAR_OUT((char) '?' );
      }
    } else {
      HARDWARE_CHAR_OUT((char) c );
    }
    seen_e3 = false;
    seen_82_83 = false;
    return 1;
  }
#else
  #error "You have to define one of the DISPLAY_INPUT_CODE_MAPPERs in your language_xx.h file" // should not occur because (en) will set.
#endif // code mappers

#endif // UTF_MAPPER_H
