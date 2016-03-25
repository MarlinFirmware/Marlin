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

/**
 * TEST
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_TEST_H
#define LANGUAGE_TEST_H

// Select ONE of the following Mappers.
// They decide what to do with a symbol in the area of [0x80:0xFF]. They take a symbol of this language file and make them point
// into an array with 128 cells, where they'll find the place of the symbol of the font in use.
//
// a.)For ASCII coded Language_xx.h files like (en) there are no occurrences of symbols above 0x7F so no mapper is needed.
//   If such a symbol appears it is mapped directly into the font. This is the case for the language files we used until now, with all the STR_XX or
//   "\xxx" symbols. All Symbols are only one byte long.
// b.) For Unicoded Language_xx.h files (currently ru, de and kana_utf8 ) the non ASCII [0x00-0x7F] symbols are represented by more then one byte.
//   In the case of two bytes the first is pointing to a 'codepage' and the second to a place in the codepage. These codepages contain 64 symbols.
//   So two of them can be mapped. For most of the European languages the necessary symbols are contained in the pages C2 and C3. Cyrillic uses D0
//   and D1.
// c.) For katakana (one of the Japanese symbol sets) Unicode uses 3 bytes. Here the second byte also points to a codepage and byte 3 to the symbol.
//   I hope the pages E282 and E283 are sufficient to write katakana.
//   Kanji (an other Japanese symbol set) uses far more than two codepages. So currently I don't see a chance to map the Unicodes. Its not
//   impossible to have a close to direct mapping but will need giant conversion tables and fonts (we don't want to have in a embedded system).


#define MAPPER_NON         // For direct ascii codes ( until now all languages except ru, de, fi, kana_utf8, ... )
//#define MAPPER_C2C3        // For most European languages when language file is in utf8
//#define MAPPER_D0D1        // For Cyrillic
//#define MAPPER_E382E383    // For Katakana

// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT

// Select the better font for full graphic displays.
//#define DISPLAY_CHARSET_ISO10646_1
//#define DISPLAY_CHARSET_ISO10646_5
//#define DISPLAY_CHARSET_ISO10646_KANA



// next 5 lines select variants in this file only
#define DISPLAYTEST
//#define WEST
//#define CYRIL
//#define KANA


// TESTSTRINGS

#define STRG_ASCII_2 " !\"#$%&'()*+,-./"
#define STRG_ASCII_3 "0123456789:;<=>?"
#define STRG_ASCII_4 "@ABCDEFGHIJKLMNO"
#define STRG_ASCII_5 "PQRSTUVWXYZ[\]^_"
#define STRG_ASCII_6 "`abcdefghijklmno"
#define STRG_ASCII_7 "pqrstuvwxyz{|}~"

#define STRG_C2_8 ""
#define STRG_C2_9 ""
#define STRG_C2_a " ¡¢£¤¥¦§¨©ª«¬­®¯"
#define STRG_C2_b "°±²³´µ¶·¸¹º»¼½¾¿"
#define STRG_C3_8 "ÈÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ"
#define STRG_C3_9 "ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
#define STRG_C3_a "àáâãäåæçèéêëìíîï"
#define STRG_C3_b "ðñòóôõö÷øùúûüýþÿ"

#define STRG_D0_8 "ЀЁЂЃЄЅІЇЈЉЊЋЌЍЎЏ"
#define STRG_D0_9 "АБВГДЕЖЗИЙКЛМНОП"
#define STRG_D0_a "РСТУФХЦЧШЩЪЫЬЭЮЯ"
#define STRG_D0_b "абвгдежзийклмноп"
#define STRG_D1_8 "рстуфхцчшщъыьэюя"
#define STRG_D1_9 "ѐёђѓєѕіїјљњћќѝўџ"
#define STRG_D1_a "ѠѡѢѣѤѥѦѧѨѩѪѫѬѭѮѯ"
#define STRG_D1_b "ѰѱѲѳѴѵѶѷѸѹѺѻѼѽѾѿ"

#define STRG_E382_8 "よめもゃやゅゆょよらりるれろゎわ"
#define STRG_E382_9 "ゐゑをんゔゕゖ゗゘゙゚゛ ゜ゝゞゟ"
#define STRG_E382_a "゠ァアィイゥウェエォオカガキギク"
#define STRG_E382_b "グケゲコゴサザシジスズセゼソゾタ"
#define STRG_E383_8 "トチヂッツヅテデトドナニヌネノハ"
#define STRG_E383_9 "バパヒビピフブプヘベペホボポマミ"
#define STRG_E383_a "ムメモャヤュユョヨラリルレロヮワ"
#define STRG_E383_b "ヰヱヲンヴヵヶヷヸヹヺ・ーヽヾヿ"

#define STRG_OKTAL_0 "\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
#define STRG_OKTAL_1 "\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
#define STRG_OKTAL_2 "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
#define STRG_OKTAL_3 "\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
#define STRG_OKTAL_4 "\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
#define STRG_OKTAL_5 "\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
#define STRG_OKTAL_6 "\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
#define STRG_OKTAL_7 "\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
#define STRG_OKTAL_8 "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
#define STRG_OKTAL_9 "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
#define STRG_OKTAL_a "\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
#define STRG_OKTAL_b "\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
#define STRG_OKTAL_c "\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
#define STRG_OKTAL_d "\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
#define STRG_OKTAL_e "\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
#define STRG_OKTAL_f "\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377"

#if ENABLED(DISPLAYTEST)
  #define WELCOME_MSG                         "Language TEST"

  #define MSG_WATCH                           "Display test"
  #define MSG_PREPARE                         STRG_OKTAL_b
  #define MSG_CONTROL                         STRG_OKTAL_c
#endif

#if ENABLED(WEST)
  #define WELCOME_MSG                         "Language TEST"

  #define MSG_WATCH                           "\001\002\003\004\005\006\007\010\011"
  #define MSG_PREPARE                         "UTF8"
  #define MSG_CONTROL                         "ASCII"

  //#define MSG_MAIN                            ".."
  #define MSG_DISABLE_STEPPERS                STRG_C2_8
  #define MSG_AUTO_HOME                       STRG_C2_9
  #define MSG_SET_HOME_OFFSETS                STRG_C2_a
  #define MSG_PREHEAT_PLA                     STRG_C2_b
  #define MSG_PREHEAT_ABS                     STRG_C3_8
  #define MSG_COOLDOWN                        STRG_C3_9
  #define MSG_SWITCH_PS_OFF                   STRG_C3_a
  #define MSG_MOVE_AXIS                       STRG_C3_b

  #define MSG_MAIN                            STRG_OKTAL_2
  #define MSG_TEMPERATURE                     STRG_OKTAL_3
  #define MSG_MOTION                          STRG_OKTAL_4
  #define MSG_VOLUMETRIC                      STRG_OKTAL_5
  #define MSG_CONTRAST                        STRG_OKTAL_6
  #define MSG_RESTORE_FAILSAFE                STRG_OKTAL_7

  #define MSG_NOZZLE                          STRG_OKTAL_8
  #define MSG_FAN_SPEED                       STRG_OKTAL_9
  #define MSG_AUTOTEMP                        STRG_OKTAL_a
  #define MSG_MIN                             STRG_OKTAL_b
  #define MSG_MAX                             STRG_OKTAL_c
  #define MSG_FACTOR                          STRG_OKTAL_d
  #define MSG_PID_P                           STRG_OKTAL_e
  #define MSG_PID_I                           STRG_OKTAL_f

#endif

#if ENABLED(CYRIL)
  #define WELCOME_MSG                         "Language TEST"

  #define MSG_WATCH                           "\001\002\003\004\005\006\007\010\011"
  #define MSG_PREPARE                         "UTF8"
  #define MSG_CONTROL                         "ASCII"

  //#define MSG_MAIN                            ".."
  #define MSG_DISABLE_STEPPERS                STRG_D0_8
  #define MSG_AUTO_HOME                       STRG_D0_9
  #define MSG_SET_HOME_OFFSETS                STRG_D0_a
  #define MSG_PREHEAT_PLA                     STRG_D0_b
  #define MSG_PREHEAT_ABS                     STRG_D1_8
  #define MSG_COOLDOWN                        STRG_D1_9
  #define MSG_SWITCH_PS_OFF                   STRG_D1_a
  #define MSG_MOVE_AXIS                       STRG_D1_b

  #define MSG_MAIN                            STRG_OKTAL_2
  #define MSG_TEMPERATURE                     STRG_OKTAL_3
  #define MSG_MOTION                          STRG_OKTAL_4
  #define MSG_VOLUMETRIC                      STRG_OKTAL_5
  #define MSG_CONTRAST                        STRG_OKTAL_6
  #define MSG_RESTORE_FAILSAFE                STRG_OKTAL_7

  #define MSG_NOZZLE                          STRG_OKTAL_8
  #define MSG_FAN_SPEED                       STRG_OKTAL_9
  #define MSG_AUTOTEMP                        STRG_OKTAL_a
  #define MSG_MIN                             STRG_OKTAL_b
  #define MSG_MAX                             STRG_OKTAL_c
  #define MSG_FACTOR                          STRG_OKTAL_d
  #define MSG_PID_P                           STRG_OKTAL_e
  #define MSG_PID_I                           STRG_OKTAL_f

#endif

#if ENABLED(KANA)
  #define WELCOME_MSG                         "Language TEST"

  #define MSG_WATCH                           "\001\002\003\004\005\006\007\010\011"
  #define MSG_PREPARE                         "UTF8"
  #define MSG_CONTROL                         "ASCII"

  //#define MSG_MAIN                            ".."
  #define MSG_DISABLE_STEPPERS                STRG_E382_8
  #define MSG_AUTO_HOME                       STRG_E382_9
  #define MSG_SET_HOME_OFFSETS                STRG_E382_a
  #define MSG_PREHEAT_PLA                     STRG_E382_b
  #define MSG_PREHEAT_ABS                     STRG_E383_8
  #define MSG_COOLDOWN                        STRG_E383_9
  #define MSG_SWITCH_PS_OFF                   STRG_E383_a
  #define MSG_MOVE_AXIS                       STRG_E383_b

  #define MSG_MAIN                            STRG_OKTAL_2
  #define MSG_TEMPERATURE                     STRG_OKTAL_3
  #define MSG_MOTION                          STRG_OKTAL_4
  #define MSG_VOLUMETRIC                      STRG_OKTAL_5
  #define MSG_CONTRAST                        STRG_OKTAL_6
  #define MSG_RESTORE_FAILSAFE                STRG_OKTAL_7

  #define MSG_NOZZLE                          STRG_OKTAL_8
  #define MSG_FAN_SPEED                       STRG_OKTAL_9
  #define MSG_AUTOTEMP                        STRG_OKTAL_a
  #define MSG_MIN                             STRG_OKTAL_b
  #define MSG_MAX                             STRG_OKTAL_c
  #define MSG_FACTOR                          STRG_OKTAL_d
  #define MSG_PID_P                           STRG_OKTAL_e
  #define MSG_PID_I                           STRG_OKTAL_f
#endif

#endif // LANGUAGE_TEST_H
