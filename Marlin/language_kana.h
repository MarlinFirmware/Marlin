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
 * Japanese (Kana)
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#ifndef LANGUAGE_KANA_H
#define LANGUAGE_KANA_H

// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_KANA
#define CHARSIZE 2

// з‰‡д»®еђЌиЎЁз¤єе®љзѕ©
#define WELCOME_MSG                         MACHINE_NAME " ready."
#define MSG_SD_INSERTED                     "\xb6\xb0\xc4\xde\xb6\xde\xbf\xb3\xc6\xad\xb3\xbb\xda\xcf\xbc\xc0" // "пЅ¶пЅ°пѕ„пѕћпЅ¶пѕћпЅїпЅіпѕ†пЅ­пЅіпЅ»пѕљпѕЏпЅјпѕЂ" ("Card inserted")
#define MSG_SD_REMOVED                      "\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                     // "пЅ¶пЅ°пѕ„пѕћпЅ¶пѕћпЅ±пѕ�пѕЏпЅѕпѕќ" ("Card removed")
#define MSG_LCD_ENDSTOPS                    "Endstops"                                                         // Max length 8 characters
#define MSG_MAIN                            "\xd2\xb2\xdd"                                                     // "пѕ’пЅІпѕќ" ("Main")
#define MSG_AUTOSTART                       "\xbc\xde\xc4\xde\xb3\xb6\xb2\xbc"                                 // "пЅјпѕћпѕ„пѕћпЅіпЅ¶пЅІпЅј" ("Autostart")
#define MSG_DISABLE_STEPPERS                "\xd3\xb0\xc0\xb0\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"             // "пѕ“пЅ°пѕЂпЅ°пѕѓпѕћпѕќпЅ№пѕћпѕќ пЅµпѕЊ" ("Disable steppers")
#define MSG_DEBUG_MENU                      "\xc3\xde\xca\xde\xaf\xb8\xde\xd2\xc6\xad\xb0"                     // "пѕѓпѕћпѕЉпѕћпЅЇпЅёпѕћпѕ’пѕ†пЅ­пЅ°" ("Debug Menu")
#define MSG_PROGRESS_BAR_TEST               "\xcc\xdf\xdb\xb8\xde\xda\xbd\xca\xde\xb0\x20\xc3\xbd\xc4"         // "пѕЊпѕџпѕ›пЅёпѕћпѕљпЅЅпѕЉпѕћпЅ° пѕѓпЅЅпѕ„" ("Progress Bar Test")
#define MSG_AUTO_HOME                       "\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                                 // "пЅ№пѕћпѕќпѕѓпѕќпѕЊпЅЇпЅ·" ("Auto home")
#define MSG_AUTO_HOME_X                     "X\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "XпЅјпѕћпЅё пЅ№пѕћпѕќпѕѓпѕќпѕЊпЅЇпЅ·" ("Home X")
#define MSG_AUTO_HOME_Y                     "Y\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "YпЅјпѕћпЅё пЅ№пѕћпѕќпѕѓпѕќпѕЊпЅЇпЅ·" ("Home Y")
#define MSG_AUTO_HOME_Z                     "Z\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "ZпЅјпѕћпЅё пЅ№пѕћпѕќпѕѓпѕќпѕЊпЅЇпЅ·" ("Home Z")
#define MSG_TMC_Z_CALIBRATION               "Z\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "ZпЅјпѕћпЅё пЅєпЅіпЅѕпЅІ" ("Calibrate Z")
#define MSG_LEVEL_BED_HOMING                "\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7\xc1\xad\xb3"                     // "пЅ№пѕћпѕќпѕѓпѕќпѕЊпЅЇпЅ·пѕЃпЅ­пЅі" ("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               "\xda\xcd\xde\xd8\xdd\xb8\xde\xb6\xb2\xbc"                         // "пѕљпѕЌпѕћпѕ�пѕќпЅёпѕћпЅ¶пЅІпЅј" ("Click to Begin")
#define MSG_LEVEL_BED_NEXT_POINT            "\xc2\xb7\xde\xc9\xbf\xb8\xc3\xb2\xc3\xdd\xcd"                     // "пѕ‚пЅ·пѕћпѕ‰пЅїпЅёпѕѓпЅІпѕѓпѕќпѕЌ" ("Next Point")
#define MSG_LEVEL_BED_DONE                  "\xda\xcd\xde\xd8\xdd\xb8\xde\xb6\xdd\xd8\xae\xb3"                 // "пѕљпѕЌпѕћпѕ�пѕќпЅёпѕћпЅ¶пѕќпѕ�пЅ®пЅі" ("Leveling Done!")
#define MSG_SET_HOME_OFFSETS                "\xb7\xbc\xde\xad\xdd\xb5\xcc\xbe\xaf\xc4\xbe\xaf\xc3\xb2"         // "пЅ·пЅјпѕћпЅ­пѕќпЅµпѕЊпЅѕпЅЇпѕ„пЅѕпЅЇпѕѓпЅІ" ("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            "\xb5\xcc\xbe\xaf\xc4\xb6\xde\xc3\xb7\xd6\xb3\xbb\xda\xcf\xbc\xc0" // "пЅµпѕЊпЅѕпЅЇпѕ„пЅ¶пѕћпѕѓпЅ·пѕ–пЅіпЅ»пѕљпѕЏпЅјпѕЂ" ("Offsets applied")
#define MSG_SET_ORIGIN                      "\xb7\xbc\xde\xad\xdd\xbe\xaf\xc4"                                 // "пЅ·пЅјпѕћпЅ­пѕќпЅѕпЅЇпѕ„" ("Set origin")
#define MSG_PREHEAT_1                       "PLA \xd6\xc8\xc2"                                                 // "PLA пѕ–пѕ€пѕ‚" ("Preheat PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   "PLA \xbd\xcd\xde\xc3\xd6\xc8\xc2"                                 // "PLA пЅЅпѕЌпѕћпѕѓпѕ–пѕ€пѕ‚" (" All")
#define MSG_PREHEAT_1_BEDONLY               "PLA \xcd\xde\xaf\xc4\xde\xd6\xc8\xc2"                             // "PLA пѕЌпѕћпЅЇпѕ„пѕћпѕ–пѕ€пѕ‚" (" Bed")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 "\xbe\xaf\xc3\xb2"                                   // "пЅѕпЅЇпѕѓпЅІ" (" conf")
#define MSG_PREHEAT_2                       "ABS \xd6\xc8\xc2"                                                 // "ABS пѕ–пѕ€пѕ‚" ("Preheat ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   "ABS \xbd\xcd\xde\xc3\xd6\xc8\xc2"                                 // "ABS пЅЅпѕЌпѕћпѕѓпѕ–пѕ€пѕ‚" (" All")
#define MSG_PREHEAT_2_BEDONLY               "ABS \xcd\xde\xaf\xc4\xde\xd6\xc8\xc2"                             // "ABS пѕЌпѕћпЅЇпѕ„пѕћпѕ–пѕ€пѕ‚" (" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 "\xbe\xaf\xc3\xb2"                                   // "пЅѕпЅЇпѕѓпЅІ" (" conf")
#define MSG_COOLDOWN                        "\xb6\xc8\xc2\xc3\xb2\xbc"                                         // "пЅ¶пѕ€пѕ‚пѕѓпЅІпЅј" ("Cooldown")
#define MSG_SWITCH_PS_ON                    "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xdd"                             // "пѕѓпѕћпѕќпЅ№пѕћпѕќ пЅµпѕќ" ("Switch power on")
#define MSG_SWITCH_PS_OFF                   "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"                             // "пѕѓпѕћпѕќпЅ№пѕћпѕќ пЅµпѕЊ" ("Switch power off")
#define MSG_EXTRUDE                         "\xb5\xbc\xc0\xde\xbc"                                             // "пЅµпЅјпѕЂпѕћпЅј" ("Extrude")
#define MSG_RETRACT                         "\xcb\xb7\xba\xd0\xbe\xaf\xc3\xb2"                                 // "пѕ‹пЅ·пЅєпѕђпЅѕпЅЇпѕѓпЅІ" ("Retract")
#define MSG_MOVE_AXIS                       "\xbc\xde\xb8\xb2\xc4\xde\xb3"                                     // "пЅјпѕћпЅёпЅІпѕ„пѕћпЅі" ("Move axis")
#define MSG_BED_LEVELING                    "\xcd\xde\xaf\xc4\xde\xda\xcd\xde\xd8\xdd\xb8\xde"                 // "пѕЌпѕћпЅЇпѕ„пѕћпѕљпѕЌпѕћпѕ�пѕќпЅёпѕћ" ("Bed Leveling")
#define MSG_LEVEL_BED                       "\xcd\xde\xaf\xc4\xde\xda\xcd\xde\xd8\xdd\xb8\xde"                 // "пѕЌпѕћпЅЇпѕ„пѕћпѕљпѕЌпѕћпѕ�пѕќпЅёпѕћ" ("Level bed")
#define MSG_MOVING                          "\xb2\xc4\xde\xb3\xc1\xad\xb3"                                     // "пЅІпѕ„пѕћпЅіпѕЃпЅ­пЅі" ("Moving...")
#define MSG_FREE_XY                         "XY\xbc\xde\xb8\x20\xb6\xb2\xce\xb3"                               // "XYпЅјпѕћпЅё пЅ¶пЅІпѕЋпЅі" ("Free XY")
#define MSG_MOVE_X                          "X\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "XпЅјпѕћпЅё пЅІпѕ„пѕћпЅі" ("Move X")
#define MSG_MOVE_Y                          "Y\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "YпЅјпѕћпЅё пЅІпѕ„пѕћпЅі" ("Move Y")
#define MSG_MOVE_Z                          "Z\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "ZпЅјпѕћпЅё пЅІпѕ„пѕћпЅі" ("Move Z")
#define MSG_MOVE_E                          "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0"                             // "пЅґпЅёпЅЅпѕ„пѕ™пЅ°пѕЂпѕћпЅ°" ("Extruder")
#define MSG_MOVE_01MM                       "0.1mm \xb2\xc4\xde\xb3"                                           // "0.1mm пЅІпѕ„пѕћпЅі" ("Move 0.1mm")
#define MSG_MOVE_1MM                        "  1mm \xb2\xc4\xde\xb3"                                           // "  1mm пЅІпѕ„пѕћпЅі" ("Move 1mm")
#define MSG_MOVE_10MM                       " 10mm \xb2\xc4\xde\xb3"                                           // " 10mm пЅІпѕ„пѕћпЅі" ("Move 10mm")
#define MSG_SPEED                           "\xbf\xb8\xc4\xde"                                                 // "пЅїпЅёпѕ„пѕћ" ("Speed")
#define MSG_BED_Z                           "Z\xb5\xcc\xbe\xaf\xc4"                                            // "ZпЅµпѕЊпЅѕпЅЇпѕ„" ("Bed Z")
#define MSG_NOZZLE                          "\xc9\xbd\xde\xd9"                                                 // "пѕ‰пЅЅпѕћпѕ™" ("Nozzle")
#define MSG_BED                             "\xcd\xde\xaf\xc4\xde"                                             // "пѕЌпѕћпЅЇпѕ„пѕћ" ("Bed")
#define MSG_FAN_SPEED                       "\xcc\xa7\xdd\xbf\xb8\xc4\xde"                                     // "пѕЊпЅ§пѕќпЅїпЅёпѕ„пѕћ" ("Fan speed")
#define MSG_FLOW                            "\xc4\xbc\xad\xc2\xd8\xae\xb3"                                     // "пѕ„пЅјпЅ­пѕ‚пѕ�пЅ®пЅі" ("Flow")
#define MSG_CONTROL                         "\xbe\xb2\xb7\xde\xae"                                             // "пЅѕпЅІпЅ·пѕћпЅ®" ("Control")
#define MSG_MIN                             LCD_STR_THERMOMETER " \xbb\xb2\xc3\xb2"                            // " пЅ»пЅІпѕѓпЅІ" (" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER " \xbb\xb2\xba\xb3"                            // " пЅ»пЅІпЅєпЅі" (" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER " \xcc\xa7\xb8\xc0\xb0"                        // " пѕЊпЅ§пЅёпѕЂпЅ°" (" Fact")
#if LCD_WIDTH >= 20
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde\xbe\xb2\xb7\xde\xae"         // "пЅјпѕћпѕ„пѕћпЅіпЅµпѕќпѕ„пѕћпЅѕпЅІпЅ·пѕћпЅ®" ("Autotemp")
#else
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde"                             // "пЅјпѕћпѕ„пѕћпЅіпЅµпѕќпѕ„пѕћ" ("Autotemp")
#endif
#define MSG_ON                              "\xb5\xdd "                                                        // "пЅµпѕќ " ("On ")
#define MSG_OFF                             "\xb5\xcc "                                                        // "пЅµпѕЊ " ("Off")
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "\xbe\xdd\xc0\xb8"                                                 // "пЅѕпѕќпѕЂпЅё" ("Select")
#define MSG_JERK                            "\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                            // гѓ¤г‚Їгѓ‰ mm/s ("Jerk")
#if LCD_WIDTH >= 20
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde mm/s2"                                       // "пЅ¶пЅїпЅёпѕ„пѕћ mm/s2" ("Accel")
  #if IS_KINEMATIC
    #define MSG_VA_JERK                     "A\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "AпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Va-jerk")
    #define MSG_VB_JERK                     "B\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "BпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Vb-jerk")
    #define MSG_VC_JERK                     "C\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "CпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Vc-jerk")
  #else
    #define MSG_VA_JERK                     "X\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "XпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Vx-jerk")
    #define MSG_VB_JERK                     "Y\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "YпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Vy-jerk")
    #define MSG_VC_JERK                     "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "ZпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ mm/s" ("Vz-jerk")
  #endif
  #define MSG_VE_JERK                       "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\x20\xd4\xb8\xc4\xde"         // "пЅґпЅёпЅЅпѕ„пѕ™пЅ°пѕЂпѕћпЅ° пѕ”пЅёпѕ„пѕћ" ("Ve-jerk")
  #define MSG_VMAX                          "\xbb\xb2\xc0\xde\xb2\xb5\xb8\xd8\xbf\xb8\xc4\xde "                // "пЅ»пЅІпѕЂпѕћпЅІпЅµпЅёпѕ�пЅїпЅёпѕ„пѕћ " ("Vmax ")
  #define MSG_VMIN                          "\xbb\xb2\xbc\xae\xb3\xb5\xb8\xd8\xbf\xb8\xc4\xde"                 // "пЅ»пЅІпЅјпЅ®пЅіпЅµпЅёпѕ�пЅїпЅёпѕ„пѕћ" ("Vmin")
  #define MSG_VTRAV_MIN                     "\xbb\xb2\xbc\xae\xb3\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"             // "пЅ»пЅІпЅјпЅ®пЅіпЅІпѕ„пѕћпЅіпЅїпЅёпѕ„пѕћ" ("VTrav min")
  #define MSG_AMAX                          "\xbb\xb2\xc0\xde\xb2\xb6\xbf\xb8\xc4\xde "                        // "пЅ»пЅІпѕЂпѕћпЅІпЅ¶пЅїпЅёпѕ„пѕћ " ("Amax ")
#else
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde"                                             // "пЅ¶пЅїпЅёпѕ„пѕћ" ("Accel")
  #if IS_KINEMATIC
    #define MSG_VA_JERK                     "A\x20\xd4\xb8\xc4\xde"                                            // "AпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Va-jerk")
    #define MSG_VB_JERK                     "B\x20\xd4\xb8\xc4\xde"                                            // "BпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Vb-jerk")
    #define MSG_VC_JERK                     "C\x20\xd4\xb8\xc4\xde"                                            // "CпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Vc-jerk")
  #else
    #define MSG_VA_JERK                     "X\x20\xd4\xb8\xc4\xde"                                            // "XпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Vx-jerk")
    #define MSG_VB_JERK                     "Y\x20\xd4\xb8\xc4\xde"                                            // "YпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Vy-jerk")
    #define MSG_VC_JERK                     "Z\x20\xd4\xb8\xc4\xde"                                            // "ZпЅјпѕћпЅё пѕ”пЅёпѕ„пѕћ" ("Vz-jerk")
  #endif
  #define MSG_VE_JERK                       "E\x20\xd4\xb8\xc4\xde"                                            // "E пѕ”пЅёпѕ„пѕћ" ("Ve-jerk")
  #define MSG_VMAX                          "max\xb5\xb8\xd8\xbf\xb8\xc4\xde "                                 // "maxпЅµпЅёпѕ�пЅїпЅёпѕ„пѕћ" ("Vmax ")
  #define MSG_VMIN                          "min\xb5\xb8\xd8\xbf\xb8\xc4\xde"                                  // "minпЅµпЅёпѕ�пЅїпЅёпѕ„пѕћ" ("Vmin")
  #define MSG_VTRAV_MIN                     "min\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"                              // "minпЅІпѕ„пѕћпЅіпЅїпЅёпѕ„пѕћ" ("VTrav min")
  #define MSG_AMAX                          "max\xb6\xbf\xb8 "                                                 // "maxпЅ¶пЅїпЅё " ("Amax ")
#endif
#define MSG_A_RETRACT                       "\xcb\xb7\xba\xd0\xb6\xbf\xb8\xc4\xde"                             // "пѕ‹пЅ·пЅєпѕђпЅ¶пЅїпЅёпѕ„пѕћ" ("A-retract")
#define MSG_A_TRAVEL                        "\xb2\xc4\xde\xb3\xb6\xbf\xb8\xc4\xde"                             // "пЅІпѕ„пѕћпЅіпЅ¶пЅїпЅёпѕ„пѕћ" ("A-travel")
#if LCD_WIDTH >= 20
  #define MSG_STEPS_PER_MM                  "Steps/mm"
  #if IS_KINEMATIC
    #define MSG_ASTEPS                      "Asteps/mm"
    #define MSG_BSTEPS                      "Bsteps/mm"
    #define MSG_CSTEPS                      "Csteps/mm"
  #else
    #define MSG_ASTEPS                      "Xsteps/mm"
    #define MSG_BSTEPS                      "Ysteps/mm"
    #define MSG_CSTEPS                      "Zsteps/mm"
  #endif
  #define MSG_ESTEPS                        "Esteps/mm"
  #define MSG_E1STEPS                       "E1steps/mm"
  #define MSG_E2STEPS                       "E2steps/mm"
  #define MSG_E3STEPS                       "E3steps/mm"
  #define MSG_E4STEPS                       "E4steps/mm"
  #define MSG_E5STEPS                       "E5steps/mm"
#else
  #define MSG_STEPS_PER_MM                  "Steps"
  #if IS_KINEMATIC
    #define MSG_ASTEPS                      "Asteps"
    #define MSG_BSTEPS                      "Bsteps"
    #define MSG_CSTEPS                      "Csteps"
  #else
    #define MSG_ASTEPS                      "Xsteps"
    #define MSG_BSTEPS                      "Ysteps"
    #define MSG_CSTEPS                      "Zsteps"
  #endif
  #define MSG_ESTEPS                        "Esteps"
  #define MSG_E1STEPS                       "E1steps"
  #define MSG_E2STEPS                       "E2steps"
  #define MSG_E3STEPS                       "E3steps"
  #define MSG_E4STEPS                       "E4steps"
  #define MSG_E5STEPS                       "E5steps"
#endif
#define MSG_TEMPERATURE                     "\xb5\xdd\xc4\xde"                                                 // "пЅµпѕќпѕ„пѕћ" ("Temperature")
#define MSG_MOTION                          "\xb3\xba\xde\xb7\xbe\xaf\xc3\xb2"                                 // "пЅіпЅєпѕћпЅ·пЅѕпЅЇпѕѓпЅІ" ("Motion")
#define MSG_FILAMENT                        "\xcc\xa8\xd7\xd2\xdd\xc4"                                         // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„" ("Filament")
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#if LCD_WIDTH >= 20
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xc1\xae\xaf\xb9\xb2"                     // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пѕЃпЅ®пЅЇпЅ№пЅІ" ("Fil. Dia.")
#else
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xb9\xb2"                                 // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅ№пЅІ" ("Fil. Dia.")
#endif
#define MSG_CONTRAST                        "LCD\xba\xdd\xc4\xd7\xbd\xc4"                                      // "LCDпЅєпѕќпѕ„пѕ—пЅЅпѕ„" ("LCD contrast")
#define MSG_STORE_EEPROM                    "\xd2\xd3\xd8\xcd\xb6\xb8\xc9\xb3"                                 // "пѕ’пѕ“пѕ�пѕЌпЅ¶пЅёпѕ‰пЅі" ("Store memory")
#define MSG_LOAD_EEPROM                     "\xd2\xd3\xd8\xb6\xd7\xd6\xd0\xba\xd0"                             // "пѕ’пѕ“пѕ�пЅ¶пѕ—пѕ–пѕђпЅєпѕђ" ("Load memory")
#define MSG_RESTORE_FAILSAFE                "\xbe\xaf\xc3\xb2\xd8\xbe\xaf\xc4"                                 // "пЅѕпЅЇпѕѓпЅІпѕ�пЅѕпЅЇпѕ„" ("Restore failsafe")
#define MSG_REFRESH                         "\xd8\xcc\xda\xaf\xbc\xad"                                         // "пѕ�пѕЊпѕљпЅЇпЅјпЅ­" ("Refresh")
#define MSG_WATCH                           "\xbc\xde\xae\xb3\xce\xb3\xb6\xde\xd2\xdd"                         // "пЅјпѕћпЅ®пЅіпѕЋпЅіпЅ¶пѕћпѕ’пѕќ" ("Info screen")
#define MSG_PREPARE                         "\xbc\xde\xad\xdd\xcb\xde\xbe\xaf\xc3\xb2"                         // "пЅјпѕћпЅ­пѕќпѕ‹пѕћпЅѕпЅЇпѕѓпЅІ" ("Prepare")
#define MSG_TUNE                            "\xc1\xae\xb3\xbe\xb2"                                             // "пѕЃпЅ®пЅіпЅѕпЅІ" ("Tune")
#define MSG_PAUSE_PRINT                     "\xb2\xc1\xbc\xde\xc3\xb2\xbc"                                     // "пЅІпѕЃпЅјпѕћпѕѓпЅІпЅј" ("Pause print")
#define MSG_RESUME_PRINT                    "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                             // "пѕЊпѕџпѕ�пѕќпѕ„пЅ»пЅІпЅ¶пЅІ" ("Resume print")
#define MSG_STOP_PRINT                      "\xcc\xdf\xd8\xdd\xc4\xc3\xb2\xbc"                                 // "пѕЊпѕџпѕ�пѕќпѕ„пѕѓпЅІпЅј" ("Stop print")
#define MSG_CARD_MENU                       "SD\xb6\xb0\xc4\xde\xb6\xd7\xcc\xdf\xd8\xdd\xc4"                   // "SDпЅ¶пЅ°пѕ„пѕћпЅ¶пѕ—пѕЊпѕџпѕ�пѕќпѕ„" ("Print from SD")
#define MSG_NO_CARD                         "SD\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                   // "SDпЅ¶пЅ°пѕ„пѕћпЅ¶пѕћпЅ±пѕ�пѕЏпЅѕпѕќ" ("No SD card")
#define MSG_DWELL                           "\xb7\xad\xb3\xbc"                                                 // "пЅ·пЅ­пЅіпЅј" ("Sleep...")
#define MSG_USERWAIT                        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "пЅјпѕЉпѕћпѕ—пЅёпЅµпѕЏпѕЃпЅёпѕЂпѕћпЅ»пЅІ" ("Wait for user...")
#define MSG_PRINT_ABORTED                   "\xcc\xdf\xd8\xdd\xc4\xb6\xde\xc1\xad\xb3\xbc\xbb\xda\xcf\xbc\xc0" // "пѕЊпѕџпѕ�пѕќпѕ„пЅ¶пѕћпѕЃпЅ­пЅіпЅјпЅ»пѕљпѕЏпЅјпѕЂ" ("Print aborted")
#define MSG_NO_MOVE                         "\xb3\xba\xde\xb7\xcf\xbe\xdd"                                     // "пЅіпЅєпѕћпЅ·пѕЏпЅѕпѕќ" ("No move.")
#define MSG_KILLED                          "\xcb\xbc\xde\xae\xb3\xc3\xb2\xbc"                                 // "пѕ‹пЅјпѕћпЅ®пЅіпѕѓпЅІпЅј" ("KILLED. ")
#define MSG_STOPPED                         "\xc3\xb2\xbc\xbc\xcf\xbc\xc0"                                     // "пѕѓпЅІпЅјпЅјпѕЏпЅјпѕЂ" ("STOPPED. ")
#if LCD_WIDTH >= 20
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3 mm"                                  // "пѕ‹пЅ·пЅєпѕђпѕ�пЅ®пЅі mm" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S mm"                                 // "пѕ‹пЅ·пЅєпѕђпѕ�пЅ®пЅіS mm" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde mm/s"                            // "пѕ‹пЅ·пЅєпѕђпЅїпЅёпѕ„пѕћ mm/s" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb mm"                                  // "пѕ‰пЅЅпѕћпѕ™пѕЂпЅІпѕ‹ mm" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3 mm"                                  // "пѕЋпЅјпЅ®пЅіпѕ�пЅ®пЅі mm" ("UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S mm"                                 // "пѕЋпЅјпЅ®пЅіпѕ�пЅ®пЅіS mm" ("S UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde mm/s"                            // "пѕЋпЅјпЅ®пЅіпЅїпЅёпѕ„пѕћ mm/s" ("UnRet  V")
#else
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3"                                     // "пѕ‹пЅ·пЅєпѕђпѕ�пЅ®пЅі" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S"                                    // "пѕ‹пЅ·пЅєпѕђпѕ�пЅ®пЅіS" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde"                                 // "пѕ‹пЅ·пЅєпѕђпЅїпЅёпѕ„пѕћ" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb"                                     // "пѕ‰пЅЅпѕћпѕ™пѕЂпЅІпѕ‹" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3"                                     // "пѕЋпЅјпЅ®пЅіпѕ�пЅ®пЅі" ("UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S"                                    // "пѕЋпЅјпЅ®пЅіпѕ�пЅ®пЅіS" ("S UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde"                                 // "пѕЋпЅјпЅ®пЅіпЅїпЅёпѕ„пѕћ" ("UnRet  V")
#endif
#define MSG_AUTORETRACT                     "\xbc\xde\xc4\xde\xb3\xcb\xb7\xba\xd0"                             // "пЅјпѕћпѕ„пѕћпЅіпѕ‹пЅ·пЅєпѕђ" ("AutoRetr.")
#define MSG_FILAMENTCHANGE                  "\xcc\xa8\xd7\xd2\xdd\xc4\xba\xb3\xb6\xdd"                         // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅєпЅіпЅ¶пѕќ" ("Change filament")
#define MSG_INIT_SDCARD                     "SD\xb6\xb0\xc4\xde\xbb\xb2\xd6\xd0\xba\xd0"                       // "SDпЅ¶пЅ°пѕ„пѕћпЅ»пЅІпѕ–пѕђпЅєпѕђ" ("Init. SD card")
#define MSG_CNG_SDCARD                      "SD\xb6\xb0\xc4\xde\xba\xb3\xb6\xdd"                               // "SDпЅ¶пЅ°пѕ„пѕћпЅєпЅіпЅ¶пѕќ" ("Change SD card")
#define MSG_ZPROBE_OUT                      "Z\xcc\xdf\xdb\xb0\xcc\xde\x20\xcd\xde\xaf\xc4\xde\xb6\xde\xb2"    // "ZпѕЊпѕџпѕ›пЅ°пѕЊпѕћ пѕЌпѕћпЅЇпѕ„пѕћпЅ¶пѕћпЅІ" ("Z probe out. bed")
#if LCD_WIDTH >= 20
  #define MSG_BLTOUCH_SELFTEST              "BLTouch \xbc\xde\xba\xbc\xdd\xc0\xde\xdd"                         // "BLTouch пЅјпѕћпЅєпЅјпѕќпѕЂпѕћпѕќ" ("BLTouch Self-Test")
#else
  #define MSG_BLTOUCH_SELFTEST              "BLTouch \xbe\xd9\xcc\xc3\xbd\xc4"                                 // "BLTouch пЅѕпѕ™пѕЊпѕѓпЅЅпѕ„" ("BLTouch Self-Test")
#endif
#define MSG_BLTOUCH_RESET                   "BLTouch \xd8\xbe\xaf\xc4"                                         // "BLTouch пѕ�пЅѕпЅЇпѕ„" ("Reset BLTouch")
#define MSG_HOME                            "\xbb\xb7\xc6"                                                     // "пЅ»пЅ·пѕ†" ("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#if LCD_WIDTH >= 20
  #define MSG_FIRST                         "\xa6\xcc\xaf\xb7\xbb\xbe\xc3\xb8\xc0\xde\xbb\xb2"                 // "пЅ¦пѕЊпЅЇпЅ·пЅ»пЅѕпѕѓпЅёпѕЂпѕћпЅ»пЅІ" ("first")
#else
  #define MSG_FIRST                         "\xa6\xcc\xaf\xb7\xbb\xbe\xd6"                                     // "пЅ¦пѕЊпЅЇпЅ·пЅ»пЅѕпѕ–" ("first")
#endif
#define MSG_ZPROBE_ZOFFSET                  "Z\xb5\xcc\xbe\xaf\xc4"                                            // "ZпЅµпѕЊпЅѕпЅЇпѕ„" ("Z Offset")
#define MSG_BABYSTEP_X                      "X\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "XпЅјпѕћпЅё пѕ‹пѕћпѕ„пѕћпЅі" ("Babystep X")
#define MSG_BABYSTEP_Y                      "Y\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "YпЅјпѕћпЅё пѕ‹пѕћпѕ„пѕћпЅі" ("Babystep Y")
#define MSG_BABYSTEP_Z                      "Z\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "ZпЅјпѕћпЅё пѕ‹пѕћпѕ„пѕћпЅі" ("Babystep Z")
#if LCD_WIDTH >= 20
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1\xb7\xc9\xb3"     // "пЅІпѕ„пѕћпЅіпЅ№пѕћпѕќпЅ¶пЅІпЅ№пѕќпѕЃпЅ·пѕ‰пЅі" ("Endstop abort")
#else
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1"                 // "пЅІпѕ„пѕћпЅіпЅ№пѕћпѕќпЅ¶пЅІпЅ№пѕќпѕЃ" ("Endstop abort")
#endif
#define MSG_HEATING_FAILED_LCD              "\xb6\xc8\xc2\xbc\xaf\xca\xdf\xb2"                                 // "пЅ¶пѕ€пѕ‚пЅјпЅЇпѕЉпѕџпЅІ" ("Heating failed")
#if LCD_WIDTH >= 20
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0\xb0\xb7\xc9\xb3" // "пЅґпѕ—пЅ°:пЅјпѕћпЅ®пЅіпѕЃпЅ®пЅіпЅ»пЅ°пѕђпЅЅпѕЂпЅ°пЅ·пѕ‰пЅі" ("Err: REDUNDANT TEMP")
#else
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0"                 // "пЅґпѕ—пЅ°:пЅјпѕћпЅ®пЅіпѕЃпЅ®пЅіпЅ»пЅ°пѕђпЅЅпѕЂ" ("Err: REDUNDANT TEMP")
#endif
#define MSG_THERMAL_RUNAWAY                 "\xc8\xc2\xce\xde\xb3\xbf\xb3"                                                  // "пѕ€пѕ‚пѕЋпѕћпЅіпЅїпЅі" ("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6"                         // "пЅґпѕ—пЅ°:пЅ»пЅІпЅєпЅіпЅµпѕќпѕЃпЅ®пЅіпЅ¶" ("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"                             // "пЅґпѕ—пЅ°:пЅ»пЅІпѕѓпЅІпЅµпѕќпѕђпѕЏпѕќ" ("Err: MINTEMP")
#if LCD_WIDTH >= 20
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6" // "пЅґпѕ—пЅ°:пѕЌпѕћпЅЇпѕ„пѕћ пЅ»пЅІпЅєпЅіпЅµпѕќпѕЃпЅ®пЅіпЅ¶" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"     // "пЅґпѕ—пЅ°:пѕЌпѕћпЅЇпѕ„пѕћ пЅ»пЅІпѕѓпЅІпЅµпѕќпѕђпѕЏпѕќ" ("Err: MINTEMP BED")
#else
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd"                 // "пЅґпѕ—пЅ°:пѕЌпѕћпЅЇпѕ„пѕћ пЅ»пЅІпЅєпЅіпЅµпѕќ" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd"                 // "пЅґпѕ—пЅ°:пѕЌпѕћпЅЇпѕ„пѕћ пЅ»пЅІпѕѓпЅІпЅµпѕќ" ("Err: MINTEMP BED")
#endif
#define MSG_ERR_Z_HOMING                    MSG_HOME " " MSG_X MSG_Y " " MSG_FIRST                             // "пЅ»пЅ·пѕ† XY пЅ¦пѕЊпЅЇпЅ·пЅ»пЅѕпѕѓпЅёпѕЂпѕћпЅ»пЅІ" or "пЅ»пЅ·пѕ† XY пЅ¦пѕЊпЅЇпЅ·пЅ»пЅѕпѕ–" ("G28 Z Forbidden")
#define MSG_HALTED                          "\xcc\xdf\xd8\xdd\xc0\xb0\xca\xc3\xb2\xbc\xbc\xcf\xbc\xc0"         // "пѕЊпѕџпѕ�пѕќпѕЂпЅ°пѕЉпѕѓпЅІпЅјпЅјпѕЏпЅјпѕЂ" ("PRINTER HALTED")
#define MSG_PLEASE_RESET                    "\xd8\xbe\xaf\xc4\xbc\xc3\xb8\xc0\xde\xbb\xb2"                     // "пѕ�пЅѕпЅЇпѕ„пЅјпѕѓпЅёпѕЂпѕћпЅ»пЅІ" ("Please reset")
#define MSG_SHORT_DAY                       "d"                                                                // One character only
#define MSG_SHORT_HOUR                      "h"                                                                // One character only
#define MSG_SHORT_MINUTE                    "m"                                                                // One character only
#define MSG_HEATING                         "\xb6\xc8\xc2\xc1\xad\xb3"                                         // "пЅ¶пѕ€пѕ‚пѕЃпЅ­пЅі" ("Heating...")
#define MSG_BED_HEATING                     "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xc1\xad\xb3"                 // "пѕЌпѕћпЅЇпѕ„пѕћ пЅ¶пѕ€пѕ‚пѕЃпЅ­пЅі" ("Bed Heating...")
#define MSG_DELTA_CALIBRATE                 "\xc3\xde\xd9\xc0\x20\xba\xb3\xbe\xb2"                             // "пѕѓпѕћпѕ™пѕЂ пЅєпЅіпЅѕпЅІ" ("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               "X\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "XпЅјпѕћпЅё пЅєпЅіпЅѕпЅІ" ("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               "Y\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "YпЅјпѕћпЅё пЅєпЅіпЅѕпЅІ" ("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               "Z\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "ZпЅјпѕћпЅё пЅєпЅіпЅѕпЅІ" ("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          "\xc1\xad\xb3\xbc\xdd\x20\xba\xb3\xbe\xb2"                         // "пѕЃпЅ­пЅіпЅјпѕќ пЅєпЅіпЅѕпЅІ" ("Calibrate Center")
#define MSG_INFO_MENU                       "\xba\xc9\xcc\xdf\xd8\xdd\xc0\xb0\xc6\xc2\xb2\xc3"                 // "пЅєпѕ‰пѕЊпѕџпѕ�пѕќпѕЂпЅ°пѕ†пѕ‚пЅІпѕѓ" ("About Printer")
#define MSG_INFO_PRINTER_MENU               "\xcc\xdf\xd8\xdd\xc0\xb0\xbc\xde\xae\xb3\xce\xb3"                 // "пѕЊпѕџпѕ�пѕќпѕЂпЅ°пЅјпѕћпЅ®пЅіпѕЋпЅі" ("Printer Info")
#define MSG_INFO_STATS_MENU                 "\xcc\xdf\xd8\xdd\xc4\xbc\xde\xae\xb3\xb7\xae\xb3"                 // "пѕЊпѕџпѕ�пѕќпѕ„пЅјпѕћпЅ®пЅіпЅ·пЅ®пЅі" ("Printer Stats")
#define MSG_INFO_BOARD_MENU                 "\xbe\xb2\xb7\xde\xae\xb9\xb2\xbc\xde\xae\xb3\xce\xb3"             // "пЅѕпЅІпЅ·пѕћпЅ®пЅ№пЅІпЅјпѕћпЅ®пЅіпѕЋпЅі" ("Board Info")
#define MSG_INFO_THERMISTOR_MENU            "\xbb\xb0\xd0\xbd\xc0\xb0"                                         // "пЅ»пЅ°пѕђпЅЅпѕЂпЅ°" ("Thermistors")
#define MSG_INFO_EXTRUDERS                  "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\xbd\xb3"                     // "пЅґпЅёпЅЅпѕ„пѕ™пЅ°пѕЂпѕћпЅ°пЅЅпЅі" ("Extruders")
#define MSG_INFO_BAUDRATE                   "\xce\xde\xb0\xda\xb0\xc4"                                         // "пѕЋпѕћпЅ°пѕљпЅ°пѕ„" ("Baud")
#define MSG_INFO_PROTOCOL                   "\xcc\xdf\xdb\xc4\xba\xd9"                                         // "пѕЊпѕџпѕ›пѕ„пЅєпѕ™" ("Protocol")
#define MSG_CASE_LIGHT                      "\xb7\xae\xb3\xc0\xb2\xc5\xb2\xbc\xae\xb3\xd2\xb2"                 // "пЅ·пЅ®пЅіпѕЂпЅІпѕ…пЅІпЅјпЅ®пЅіпѕ’пЅІ" ("Case light")
#define MSG_INFO_PRINT_COUNT                "\xcc\xdf\xd8\xdd\xc4\xbd\xb3"                                     // "пѕЊпѕџпѕ�пѕќпѕ„пЅЅпЅі" ("Print Count")
#define MSG_INFO_COMPLETED_PRINTS           "\xb6\xdd\xd8\xae\xb3\xbd\xb3"                                     // "пЅ¶пѕќпѕ�пЅ®пЅіпЅЅпЅі" ("Completed")
#define MSG_INFO_PRINT_TIME                 "\xcc\xdf\xd8\xdd\xc4\xbc\xde\xb6\xdd\xd9\xb2\xb9\xb2"             // "пѕЊпѕџпѕ�пѕќпѕ„пЅјпѕћпЅ¶пѕќпѕ™пЅІпЅ№пЅІ" ("Total print time")
#define MSG_INFO_PRINT_LONGEST              "\xbb\xb2\xc1\xae\xb3\xcc\xdf\xd8\xdd\xc4\xbc\xde\xb6\xdd"         // "пЅ»пЅІпѕЃпЅ®пЅіпѕЊпѕџпѕ�пѕќпѕ„пЅјпѕћпЅ¶пѕќ" ("Longest job time")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_FILAMENT           "\xcc\xa8\xd7\xd2\xdd\xc4\xbc\xd6\xb3\xd8\xae\xb3\xd9\xb2\xb9\xb2" // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅјпѕ–пЅіпѕ�пЅ®пЅіпѕ™пЅІпЅ№пЅІ" ("Extruded total")
#else
  #define MSG_INFO_PRINT_FILAMENT           "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xbc\xd6\xb3\xd8\xae\xb3"         // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅїпЅіпЅјпѕ–пЅіпѕ�пЅ®пЅі" ("Extruded")
#endif
#define MSG_INFO_MIN_TEMP                   "\xbe\xaf\xc3\xb2\xbb\xb2\xc3\xb2\xb5\xdd"                         // "пЅѕпЅЇпѕѓпЅІпЅ»пЅІпѕѓпЅІпЅµпѕќ" ("Min Temp")
#define MSG_INFO_MAX_TEMP                   "\xbe\xaf\xc3\xb2\xbb\xb2\xba\xb3\xb5\xdd"                         // "пЅѕпЅЇпѕѓпЅІпЅ»пЅІпЅєпЅіпЅµпѕќ" ("Max Temp")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PSU                      "\xc3\xde\xdd\xb9\xde\xdd\xbc\xad\xcd\xde\xc2"                     // "пѕѓпѕћпѕќпЅ№пѕћпѕќпЅјпЅ­пѕЌпѕћпѕ‚" ("Power Supply")
#else
  #define MSG_INFO_PSU                      "\xc3\xde\xdd\xb9\xde\xdd"                                         // "пѕѓпѕћпѕќпЅ№пѕћпѕќ" ("Power Supply")
#endif
#define MSG_DRIVE_STRENGTH                  "\xd3\xb0\xc0\xb0\xb8\xc4\xde\xb3\xd8\xae\xb8"                     // "пѕ“пЅ°пѕЂпЅ°пЅёпѕ„пѕћпЅіпѕ�пЅ®пЅё" ("Drive Strength")
#if LCD_WIDTH >= 20
  #define MSG_DAC_PERCENT                   "DAC\xbc\xad\xc2\xd8\xae\xb8 %"                                    // "DACпЅјпЅ­пѕ‚пѕ�пЅ®пЅё %" ("Driver %")
#else
  #define MSG_DAC_PERCENT                   "DAC\xbc\xad\xc2\xd8\xae\xb8"                                      // "DACпЅјпЅ­пѕ‚пѕ�пЅ®пЅё" ("Driver %")
#endif
#define MSG_DAC_EEPROM_WRITE                MSG_STORE_EEPROM                                                   // "пѕ’пѕ“пѕ�пѕЌпЅ¶пЅёпѕ‰пЅі" ("DAC EEPROM Write")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                             // "пѕЊпѕџпѕ�пѕќпѕ„пЅ»пЅІпЅ¶пЅІ" ("Resume print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\xba\xb3\xb6\xdd\xa6\xb6\xb2\xbc\xbc\xcf\xbd"                     // "пЅєпЅіпЅ¶пѕќпЅ¦пЅ¶пЅІпЅјпЅјпѕЏпЅЅ" ("Wait for start")
  #define MSG_FILAMENT_CHANGE_INIT_2        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "пЅјпѕЉпѕћпѕ—пЅёпЅµпѕЏпѕЃпЅёпѕЂпѕћпЅ»пЅІ" ("of the filament")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xc7\xb7\xc0\xde\xbc\xc1\xad\xb3"         // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пѕ‡пЅ·пѕЂпѕћпЅјпѕЃпЅ­пЅі" ("Wait for")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2      "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "пЅјпѕЉпѕћпѕ—пЅёпЅµпѕЏпѕЃпЅёпѕЂпѕћпЅ»пЅІ" ("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xa6\xbf\xb3\xc6\xad\xb3\xbc,"            // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅ¦пЅїпЅіпѕ†пЅ­пЅіпЅј," ("Insert filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2      "\xb8\xd8\xaf\xb8\xbd\xd9\xc4\xbf\xde\xaf\xba\xb3\xbc\xcf\xbd"     // "пЅёпѕ�пЅЇпЅёпЅЅпѕ™пѕ„пЅїпѕћпЅЇпЅєпЅіпЅјпѕЏпЅЅ" ("and press button")
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xc3\xdd\xc1\xad\xb3"             // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅїпЅіпѕѓпѕќпѕЃпЅ­пЅі" ("Wait for")
  #define MSG_FILAMENT_CHANGE_LOAD_2        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "пЅјпѕЉпѕћпѕ—пЅёпЅµпѕЏпѕЃпЅёпѕЂпѕћпЅ»пЅІ" ("filament load")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xcc\xdf\xd8\xdd\xc4\xa6\xbb\xb2\xb6\xb2\xbc\xcf\xbd"             // "пѕЊпѕџпѕ�пѕќпѕ„пЅ¦пЅ»пЅІпЅ¶пЅІпЅјпѕЏпЅЅ" ("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2      "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "пЅјпѕЉпѕћпѕ—пЅёпЅµпѕЏпѕЃпЅёпѕЂпѕћпЅ»пЅІ" ("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\xba\xb3\xb6\xdd\xa6\xb6\xb2\xbc\xbc\xcf\xbd"                     // "пЅєпЅіпЅ¶пѕќпЅ¦пЅ¶пЅІпЅјпЅјпѕЏпЅЅ" ("Please wait...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xc7\xb7\xc0\xde\xbc\xc1\xad\xb3"         // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пѕ‡пЅ·пѕЂпѕћпЅјпѕЃпЅ­пЅі" ("Ejecting...")
  #if LCD_WIDTH >= 20
    #define MSG_FILAMENT_CHANGE_INSERT_1    "\xbf\xb3\xc6\xad\xb3\xbc\x2c\xb8\xd8\xaf\xb8\xbc\xc3\xb8\xc0\xde\xbb\xb2" // "пЅїпЅіпѕ†пЅ­пЅіпЅј,пЅёпѕ�пЅЇпЅёпЅјпѕѓпЅёпѕЂпѕћпЅ»пЅІ" ("Insert and Click")
  #else
    #define MSG_FILAMENT_CHANGE_INSERT_1    "\xbf\xb3\xc6\xad\xb3\xbc\x2c\xb8\xd8\xaf\xb8\xbe\xd6"                     // "пЅїпЅіпѕ†пЅ­пЅіпЅј,пЅёпѕ�пЅЇпЅёпЅѕпѕ–" ("Insert and Click")
  #endif
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xc3\xdd\xc1\xad\xb3"             // "пѕЊпЅЁпѕ—пѕ’пѕќпѕ„пЅїпЅіпѕѓпѕќпѕЃпЅ­пЅі" ("Loading...")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xcc\xdf\xd8\xdd\xc4\xa6\xbb\xb2\xb6\xb2\xbc\xcf\xbd"             // "пѕЊпѕџпѕ�пѕќпѕ„пЅ¦пЅ»пЅІпЅ¶пЅІпЅјпѕЏпЅЅ" ("Resuming...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_KANA_H

