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
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */

#ifndef LANGUAGE_KANA_H
#define LANGUAGE_KANA_H

// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_KANA

// 片仮名表示定義
#define WELCOME_MSG                         MACHINE_NAME " ready."
#define MSG_SD_INSERTED                     "\xb6\xb0\xc4\xde\xb6\xde\xbf\xb3\xc6\xad\xb3\xbb\xda\xcf\xbc\xc0" // "ｶｰﾄﾞｶﾞｿｳﾆｭｳｻﾚﾏｼﾀ" ("Card inserted")
#define MSG_SD_REMOVED                      "\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                     // "ｶｰﾄﾞｶﾞｱﾘﾏｾﾝ" ("Card removed")
#define MSG_LCD_ENDSTOPS                    "Endstops"                                                         // Max length 8 characters
#define MSG_MAIN                            "\xd2\xb2\xdd"                                                     // "ﾒｲﾝ" ("Main")
#define MSG_AUTOSTART                       "\xbc\xde\xc4\xde\xb3\xb6\xb2\xbc"                                 // "ｼﾞﾄﾞｳｶｲｼ" ("Autostart")
#define MSG_DISABLE_STEPPERS                "\xd3\xb0\xc0\xb0\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"             // "ﾓｰﾀｰﾃﾞﾝｹﾞﾝ ｵﾌ" ("Disable steppers")
#define MSG_DEBUG_MENU                      "\xc3\xde\xca\xde\xaf\xb8\xde\xd2\xc6\xad\xb0"                     // "ﾃﾞﾊﾞｯｸﾞﾒﾆｭｰ" ("Debug Menu")
#define MSG_PROGRESS_BAR_TEST               "\xcc\xdf\xdb\xb8\xde\xda\xbd\xca\xde\xb0\x20\xc3\xbd\xc4"         // "ﾌﾟﾛｸﾞﾚｽﾊﾞｰ ﾃｽﾄ" ("Progress Bar Test")
#define MSG_AUTO_HOME                       "\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                                 // "ｹﾞﾝﾃﾝﾌｯｷ" ("Auto home")
#define MSG_AUTO_HOME_X                     "X\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "Xｼﾞｸ ｹﾞﾝﾃﾝﾌｯｷ" ("Home X")
#define MSG_AUTO_HOME_Y                     "Y\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "Yｼﾞｸ ｹﾞﾝﾃﾝﾌｯｷ" ("Home Y")
#define MSG_AUTO_HOME_Z                     "Z\xbc\xde\xb8\x20\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7"                // "Zｼﾞｸ ｹﾞﾝﾃﾝﾌｯｷ" ("Home Z")
#define MSG_LEVEL_BED_HOMING                "\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7\xc1\xad\xb3"                     // "ｹﾞﾝﾃﾝﾌｯｷﾁｭｳ" ("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               "\xda\xcd\xde\xd8\xdd\xb8\xde\xb6\xb2\xbc"                         // "ﾚﾍﾞﾘﾝｸﾞｶｲｼ" ("Click to Begin")
#define MSG_LEVEL_BED_NEXT_POINT            "\xc2\xb7\xde\xc9\xbf\xb8\xc3\xb2\xc3\xdd\xcd"                     // "ﾂｷﾞﾉｿｸﾃｲﾃﾝﾍ" ("Next Point")
#define MSG_LEVEL_BED_DONE                  "\xda\xcd\xde\xd8\xdd\xb8\xde\xb6\xdd\xd8\xae\xb3"                 // "ﾚﾍﾞﾘﾝｸﾞｶﾝﾘｮｳ" ("Leveling Done!")
#define MSG_LEVEL_BED_CANCEL                "\xc4\xd8\xd4\xd2"                                                 // "ﾄﾘﾔﾒ" ("Cancel")
#define MSG_SET_HOME_OFFSETS                "\xb7\xbc\xde\xad\xdd\xb5\xcc\xbe\xaf\xc4\xbe\xaf\xc3\xb2"         // "ｷｼﾞｭﾝｵﾌｾｯﾄｾｯﾃｲ" ("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            "\xb5\xcc\xbe\xaf\xc4\xb6\xde\xc3\xb7\xd6\xb3\xbb\xda\xcf\xbc\xc0" // "ｵﾌｾｯﾄｶﾞﾃｷﾖｳｻﾚﾏｼﾀ" ("Offsets applied")
#define MSG_SET_ORIGIN                      "\xb7\xbc\xde\xad\xdd\xbe\xaf\xc4"                                 // "ｷｼﾞｭﾝｾｯﾄ" ("Set origin")
#define MSG_PREHEAT_1                       "PLA \xd6\xc8\xc2"                                                 // "PLA ﾖﾈﾂ" ("Preheat PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   "PLA \xbd\xcd\xde\xc3\xd6\xc8\xc2"                                 // "PLA ｽﾍﾞﾃﾖﾈﾂ" (" All")
#define MSG_PREHEAT_1_BEDONLY               "PLA \xcd\xde\xaf\xc4\xde\xd6\xc8\xc2"                             // "PLA ﾍﾞｯﾄﾞﾖﾈﾂ" (" Bed")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 "\xbe\xaf\xc3\xb2"                                   // "ｾｯﾃｲ" (" conf")
#define MSG_PREHEAT_2                       "ABS \xd6\xc8\xc2"                                                 // "ABS ﾖﾈﾂ" ("Preheat ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   "ABS \xbd\xcd\xde\xc3\xd6\xc8\xc2"                                 // "ABS ｽﾍﾞﾃﾖﾈﾂ" (" All")
#define MSG_PREHEAT_2_BEDONLY               "ABS \xcd\xde\xaf\xc4\xde\xd6\xc8\xc2"                             // "ABS ﾍﾞｯﾄﾞﾖﾈﾂ" (" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 "\xbe\xaf\xc3\xb2"                                   // "ｾｯﾃｲ" (" conf")
#define MSG_COOLDOWN                        "\xb6\xc8\xc2\xc3\xb2\xbc"                                         // "ｶﾈﾂﾃｲｼ" ("Cooldown")
#define MSG_SWITCH_PS_ON                    "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xdd"                             // "ﾃﾞﾝｹﾞﾝ ｵﾝ" ("Switch power on")
#define MSG_SWITCH_PS_OFF                   "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"                             // "ﾃﾞﾝｹﾞﾝ ｵﾌ" ("Switch power off")
#define MSG_EXTRUDE                         "\xb5\xbc\xc0\xde\xbc"                                             // "ｵｼﾀﾞｼ" ("Extrude")
#define MSG_RETRACT                         "\xcb\xb7\xba\xd0\xbe\xaf\xc3\xb2"                                 // "ﾋｷｺﾐｾｯﾃｲ" ("Retract")
#define MSG_MOVE_AXIS                       "\xbc\xde\xb8\xb2\xc4\xde\xb3"                                     // "ｼﾞｸｲﾄﾞｳ" ("Move axis")
#define MSG_LEVEL_BED                       "\xcd\xde\xaf\xc4\xde\xda\xcd\xde\xd8\xdd\xb8\xde"                 // "ﾍﾞｯﾄﾞﾚﾍﾞﾘﾝｸﾞ" ("Level bed")
#define MSG_MOVING                          "\xb2\xc4\xde\xb3\xc1\xad\xb3"                                     // "ｲﾄﾞｳﾁｭｳ" ("Moving...")
#define MSG_FREE_XY                         "XY\xbc\xde\xb8\x20\xb6\xb2\xce\xb3"                               // "XYｼﾞｸ ｶｲﾎｳ" ("Free XY")
#define MSG_MOVE_X                          "X\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "Xｼﾞｸ ｲﾄﾞｳ" ("Move X")
#define MSG_MOVE_Y                          "Y\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "Yｼﾞｸ ｲﾄﾞｳ" ("Move Y")
#define MSG_MOVE_Z                          "Z\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                                // "Zｼﾞｸ ｲﾄﾞｳ" ("Move Z")
#define MSG_MOVE_E                          "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0"                             // "ｴｸｽﾄﾙｰﾀﾞｰ" ("Extruder")
#define MSG_MOVE_01MM                       "0.1mm \xb2\xc4\xde\xb3"                                           // "0.1mm ｲﾄﾞｳ" ("Move 0.1mm")
#define MSG_MOVE_1MM                        "  1mm \xb2\xc4\xde\xb3"                                           // "  1mm ｲﾄﾞｳ" ("Move 1mm")
#define MSG_MOVE_10MM                       " 10mm \xb2\xc4\xde\xb3"                                           // " 10mm ｲﾄﾞｳ" ("Move 10mm")
#define MSG_SPEED                           "\xbf\xb8\xc4\xde"                                                 // "ｿｸﾄﾞ" ("Speed")
#define MSG_BED_Z                           "Z\xb5\xcc\xbe\xaf\xc4"                                            // "Zｵﾌｾｯﾄ" ("Bed Z")
#define MSG_NOZZLE                          "\xc9\xbd\xde\xd9"                                                 // "ﾉｽﾞﾙ" ("Nozzle")
#define MSG_BED                             "\xcd\xde\xaf\xc4\xde"                                             // "ﾍﾞｯﾄﾞ" ("Bed")
#define MSG_FAN_SPEED                       "\xcc\xa7\xdd\xbf\xb8\xc4\xde"                                     // "ﾌｧﾝｿｸﾄﾞ" ("Fan speed")
#define MSG_FLOW                            "\xc4\xbc\xad\xc2\xd8\xae\xb3"                                     // "ﾄｼｭﾂﾘｮｳ" ("Flow")
#define MSG_CONTROL                         "\xbe\xb2\xb7\xde\xae"                                             // "ｾｲｷﾞｮ" ("Control")
#define MSG_MIN                             LCD_STR_THERMOMETER " \xbb\xb2\xc3\xb2"                            // " ｻｲﾃｲ" (" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER " \xbb\xb2\xba\xb3"                            // " ｻｲｺｳ" (" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER " \xcc\xa7\xb8\xc0\xb0"                        // " ﾌｧｸﾀｰ" (" Fact")
#if LCD_WIDTH >= 20
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde\xbe\xb2\xb7\xde\xae"         // "ｼﾞﾄﾞｳｵﾝﾄﾞｾｲｷﾞｮ" ("Autotemp")
#else
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde"                             // "ｼﾞﾄﾞｳｵﾝﾄﾞ" ("Autotemp")
#endif
#define MSG_ON                              "\xb5\xdd "                                                        // "ｵﾝ " ("On ")
#define MSG_OFF                             "\xb5\xcc "                                                        // "ｵﾌ " ("Off")
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "\xbe\xdd\xc0\xb8"                                                 // "ｾﾝﾀｸ" ("Select")
#if LCD_WIDTH >= 20
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde mm/s2"                                       // "ｶｿｸﾄﾞ mm/s2" ("Accel")
  #define MSG_VX_JERK                       "X\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "Xｼﾞｸ ﾔｸﾄﾞ mm/s" ("Vx-jerk")
  #define MSG_VY_JERK                       "Y\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "Yｼﾞｸ ﾔｸﾄﾞ mm/s" ("Vy-jerk")
  #define MSG_VZ_JERK                       "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "Zｼﾞｸ ﾔｸﾄﾞ mm/s" ("Vz-jerk")
  #define MSG_VE_JERK                       "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\x20\xd4\xb8\xc4\xde"         // "ｴｸｽﾄﾙｰﾀﾞｰ ﾔｸﾄﾞ" ("Ve-jerk")
  #define MSG_VMAX                          "\xbb\xb2\xc0\xde\xb2\xb5\xb8\xd8\xbf\xb8\xc4\xde "                // "ｻｲﾀﾞｲｵｸﾘｿｸﾄﾞ " ("Vmax ")
  #define MSG_VMIN                          "\xbb\xb2\xbc\xae\xb3\xb5\xb8\xd8\xbf\xb8\xc4\xde"                 // "ｻｲｼｮｳｵｸﾘｿｸﾄﾞ" ("Vmin")
  #define MSG_VTRAV_MIN                     "\xbb\xb2\xbc\xae\xb3\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"             // "ｻｲｼｮｳｲﾄﾞｳｿｸﾄﾞ" ("VTrav min")
  #define MSG_AMAX                          "\xbb\xb2\xc0\xde\xb2\xb6\xbf\xb8\xc4\xde "                        // "ｻｲﾀﾞｲｶｿｸﾄﾞ " ("Amax ")
#else
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde"                                             // "ｶｿｸﾄﾞ" ("Accel")
  #define MSG_VX_JERK                       "X\xbc\xde\xb8\x20\xd4\xb8\xc4\xde"                                // "XYｼﾞｸ ﾔｸﾄﾞ" ("Vx-jerk")
  #define MSG_VY_JERK                       "Y\xbc\xde\xb8\x20\xd4\xb8\xc4\xde"                                // "XYｼﾞｸ ﾔｸﾄﾞ" ("Vy-jerk")
  #define MSG_VZ_JERK                       "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde"                                // "Zｼﾞｸ ﾔｸﾄﾞ" ("Vz-jerk")
  #define MSG_VE_JERK                       "E\x20\xd4\xb8\xc4\xde"                                            // "E ﾔｸﾄﾞ" ("Ve-jerk")
  #define MSG_VMAX                          "max\xb5\xb8\xd8\xbf\xb8\xc4\xde "                                 // "maxｵｸﾘｿｸﾄﾞ" ("Vmax ")
  #define MSG_VMIN                          "min\xb5\xb8\xd8\xbf\xb8\xc4\xde"                                  // "minｵｸﾘｿｸﾄﾞ" ("Vmin")
  #define MSG_VTRAV_MIN                     "min\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"                              // "minｲﾄﾞｳｿｸﾄﾞ" ("VTrav min")
  #define MSG_AMAX                          "max\xb6\xbf\xb8 "                                                 // "maxｶｿｸ " ("Amax ")
#endif
#define MSG_A_RETRACT                       "\xcb\xb7\xba\xd0\xb6\xbf\xb8\xc4\xde"                             // "ﾋｷｺﾐｶｿｸﾄﾞ" ("A-retract")
#define MSG_A_TRAVEL                        "\xb2\xc4\xde\xb3\xb6\xbf\xb8\xc4\xde"                             // "ｲﾄﾞｳｶｿｸﾄﾞ" ("A-travel")
#if LCD_WIDTH >= 20
  #define MSG_XSTEPS                          "Xsteps/mm"
  #define MSG_YSTEPS                          "Ysteps/mm"
  #define MSG_ZSTEPS                          "Zsteps/mm"
  #define MSG_ESTEPS                          "Esteps/mm"
  #define MSG_E1STEPS                         "E1steps/mm"
  #define MSG_E2STEPS                         "E2steps/mm"
  #define MSG_E3STEPS                         "E3steps/mm"
  #define MSG_E4STEPS                         "E4steps/mm"
  #define MSG_E5STEPS                         "E5steps/mm"
#else
  #define MSG_XSTEPS                          "Xsteps"
  #define MSG_YSTEPS                          "Ysteps"
  #define MSG_ZSTEPS                          "Zsteps"
  #define MSG_ESTEPS                          "Esteps"
  #define MSG_E1STEPS                         "E1steps"
  #define MSG_E2STEPS                         "E2steps"
  #define MSG_E3STEPS                         "E3steps"
  #define MSG_E4STEPS                         "E4steps"
  #define MSG_E5STEPS                         "E5steps"
#endif
#define MSG_TEMPERATURE                     "\xb5\xdd\xc4\xde"                                                 // "ｵﾝﾄﾞ" ("Temperature")
#define MSG_MOTION                          "\xb3\xba\xde\xb7\xbe\xaf\xc3\xb2"                                 // "ｳｺﾞｷｾｯﾃｲ" ("Motion")
#define MSG_FILAMENT                        "\xcc\xa8\xd7\xd2\xdd\xc4"                                         // "ﾌｨﾗﾒﾝﾄ" ("Filament")
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#if LCD_WIDTH >= 20
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xc1\xae\xaf\xb9\xb2"                     // "ﾌｨﾗﾒﾝﾄﾁｮｯｹｲ" ("Fil. Dia.")
#else
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xb9\xb2"                                 // "ﾌｨﾗﾒﾝﾄｹｲ" ("Fil. Dia.")
#endif
#define MSG_CONTRAST                        "LCD\xba\xdd\xc4\xd7\xbd\xc4"                                      // "LCDｺﾝﾄﾗｽﾄ" ("LCD contrast")
#define MSG_STORE_EEPROM                    "\xd2\xd3\xd8\xcd\xb6\xb8\xc9\xb3"                                 // "ﾒﾓﾘﾍｶｸﾉｳ" ("Store memory")
#define MSG_LOAD_EEPROM                     "\xd2\xd3\xd8\xb6\xd7\xd6\xd0\xba\xd0"                             // "ﾒﾓﾘｶﾗﾖﾐｺﾐ" ("Load memory")
#define MSG_RESTORE_FAILSAFE                "\xbe\xaf\xc3\xb2\xd8\xbe\xaf\xc4"                                 // "ｾｯﾃｲﾘｾｯﾄ" ("Restore failsafe")
#define MSG_REFRESH                         "\xd8\xcc\xda\xaf\xbc\xad"                                         // "ﾘﾌﾚｯｼｭ" ("Refresh")
#define MSG_WATCH                           "\xbc\xde\xae\xb3\xce\xb3\xb6\xde\xd2\xdd"                         // "ｼﾞｮｳﾎｳｶﾞﾒﾝ" ("Info screen")
#define MSG_PREPARE                         "\xbc\xde\xad\xdd\xcb\xde\xbe\xaf\xc3\xb2"                         // "ｼﾞｭﾝﾋﾞｾｯﾃｲ" ("Prepare")
#define MSG_TUNE                            "\xc1\xae\xb3\xbe\xb2"                                             // "ﾁｮｳｾｲ" ("Tune")
#define MSG_PAUSE_PRINT                     "\xb2\xc1\xbc\xde\xc3\xb2\xbc"                                     // "ｲﾁｼﾞﾃｲｼ" ("Pause print")
#define MSG_RESUME_PRINT                    "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                             // "ﾌﾟﾘﾝﾄｻｲｶｲ" ("Resume print")
#define MSG_STOP_PRINT                      "\xcc\xdf\xd8\xdd\xc4\xc3\xb2\xbc"                                 // "ﾌﾟﾘﾝﾄﾃｲｼ" ("Stop print")
#define MSG_CARD_MENU                       "SD\xb6\xb0\xc4\xde\xb6\xd7\xcc\xdf\xd8\xdd\xc4"                   // "SDｶｰﾄﾞｶﾗﾌﾟﾘﾝﾄ" ("Print from SD")
#define MSG_NO_CARD                         "SD\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                   // "SDｶｰﾄﾞｶﾞｱﾘﾏｾﾝ" ("No SD card")
#define MSG_DWELL                           "\xb7\xad\xb3\xbc"                                                 // "ｷｭｳｼ" ("Sleep...")
#define MSG_USERWAIT                        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("Wait for user...")
#define MSG_RESUMING                        "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                             // "ﾌﾟﾘﾝﾄｻｲｶｲ" ("Resuming print")
#define MSG_PRINT_ABORTED                   "\xcc\xdf\xd8\xdd\xc4\xb6\xde\xc1\xad\xb3\xbc\xbb\xda\xcf\xbc\xc0" // "ﾌﾟﾘﾝﾄｶﾞﾁｭｳｼｻﾚﾏｼﾀ" ("Print aborted")
#define MSG_NO_MOVE                         "\xb3\xba\xde\xb7\xcf\xbe\xdd"                                     // "ｳｺﾞｷﾏｾﾝ" ("No move.")
#define MSG_KILLED                          "\xcb\xbc\xde\xae\xb3\xc3\xb2\xbc"                                 // "ﾋｼﾞｮｳﾃｲｼ" ("KILLED. ")
#define MSG_STOPPED                         "\xc3\xb2\xbc\xbc\xcf\xbc\xc0"                                     // "ﾃｲｼｼﾏｼﾀ" ("STOPPED. ")
#if LCD_WIDTH >= 20
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3 mm"                                  // "ﾋｷｺﾐﾘｮｳ mm" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S mm"                                 // "ﾋｷｺﾐﾘｮｳS mm" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde mm/s"                            // "ﾋｷｺﾐｿｸﾄﾞ mm/s" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb mm"                                  // "ﾉｽﾞﾙﾀｲﾋ mm" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3 mm"                                  // "ﾎｼｮｳﾘｮｳ mm" ("UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S mm"                                 // "ﾎｼｮｳﾘｮｳS mm" ("S UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde mm/s"                            // "ﾎｼｮｳｿｸﾄﾞ mm/s" ("UnRet  V")
#else
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3"                                     // "ﾋｷｺﾐﾘｮｳ" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S"                                    // "ﾋｷｺﾐﾘｮｳS" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde"                                 // "ﾋｷｺﾐｿｸﾄﾞ" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb"                                     // "ﾉｽﾞﾙﾀｲﾋ" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3"                                     // "ﾎｼｮｳﾘｮｳ" ("UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S"                                    // "ﾎｼｮｳﾘｮｳS" ("S UnRet mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde"                                 // "ﾎｼｮｳｿｸﾄﾞ" ("UnRet  V")
#endif
#define MSG_AUTORETRACT                     "\xbc\xde\xc4\xde\xb3\xcb\xb7\xba\xd0"                             // "ｼﾞﾄﾞｳﾋｷｺﾐ" ("AutoRetr.")
#define MSG_FILAMENTCHANGE                  "\xcc\xa8\xd7\xd2\xdd\xc4\xba\xb3\xb6\xdd"                         // "ﾌｨﾗﾒﾝﾄｺｳｶﾝ" ("Change filament")
#define MSG_INIT_SDCARD                     "SD\xb6\xb0\xc4\xde\xbb\xb2\xd6\xd0\xba\xd0"                       // "SDｶｰﾄﾞｻｲﾖﾐｺﾐ" ("Init. SD card")
#define MSG_CNG_SDCARD                      "SD\xb6\xb0\xc4\xde\xba\xb3\xb6\xdd"                               // "SDｶｰﾄﾞｺｳｶﾝ" ("Change SD card")
#define MSG_ZPROBE_OUT                      "Z\xcc\xdf\xdb\xb0\xcc\xde\x20\xcd\xde\xaf\xc4\xde\xb6\xde\xb2"    // "Zﾌﾟﾛｰﾌﾞ ﾍﾞｯﾄﾞｶﾞｲ" ("Z probe out. bed")
#if LCD_WIDTH >= 20
  #define MSG_BLTOUCH_SELFTEST              "BLTouch \xbc\xde\xba\xbc\xdd\xc0\xde\xdd"                         // "BLTouch ｼﾞｺｼﾝﾀﾞﾝ" ("BLTouch Self-Test")
#else
  #define MSG_BLTOUCH_SELFTEST              "BLTouch \xbe\xd9\xcc\xc3\xbd\xc4"                                 // "BLTouch ｾﾙﾌﾃｽﾄ" ("BLTouch Self-Test")
#endif
#define MSG_BLTOUCH_RESET                   "BLTouch \xd8\xbe\xaf\xc4"                                         // "BLTouch ﾘｾｯﾄ" ("Reset BLTouch")
#define MSG_HOME                            "\xbb\xb7\xc6"                                                     // "ｻｷﾆ" ("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#if LCD_WIDTH >= 20
  #define MSG_FIRST                         "\xa6\xcc\xaf\xb7\xbb\xbe\xc3\xb8\xc0\xde\xbb\xb2"                 // "ｦﾌｯｷｻｾﾃｸﾀﾞｻｲ" ("first")
#else
  #define MSG_FIRST                         "\xa6\xcc\xaf\xb7\xbb\xbe\xd6"                                     // "ｦﾌｯｷｻｾﾖ" ("first")
#endif
#define MSG_ZPROBE_ZOFFSET                  "Z\xb5\xcc\xbe\xaf\xc4"                                            // "Zｵﾌｾｯﾄ" ("Z Offset")
#define MSG_BABYSTEP_X                      "X\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Xｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep X")
#define MSG_BABYSTEP_Y                      "Y\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Yｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep Y")
#define MSG_BABYSTEP_Z                      "Z\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Zｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep Z")
#if LCD_WIDTH >= 20
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1\xb7\xc9\xb3"     // "ｲﾄﾞｳｹﾞﾝｶｲｹﾝﾁｷﾉｳ" ("Endstop abort")
#else
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1"                 // "ｲﾄﾞｳｹﾞﾝｶｲｹﾝﾁ" ("Endstop abort")
#endif
#define MSG_HEATING_FAILED_LCD              "\xb6\xc8\xc2\xbc\xaf\xca\xdf\xb2"                                 // "ｶﾈﾂｼｯﾊﾟｲ" ("Heating failed")
#if LCD_WIDTH >= 20
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0\xb0\xb7\xc9\xb3" // "ｴﾗｰ:ｼﾞｮｳﾁｮｳｻｰﾐｽﾀｰｷﾉｳ" ("Err: REDUNDANT TEMP")
#else
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0"                 // "ｴﾗｰ:ｼﾞｮｳﾁｮｳｻｰﾐｽﾀ" ("Err: REDUNDANT TEMP")
#endif
#define MSG_THERMAL_RUNAWAY                 "\xc8\xc2\xce\xde\xb3\xbf\xb3"                                                  // "ﾈﾂﾎﾞｳｿｳ" ("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6"                         // "ｴﾗｰ:ｻｲｺｳｵﾝﾁｮｳｶ" ("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"                             // "ｴﾗｰ:ｻｲﾃｲｵﾝﾐﾏﾝ" ("Err: MINTEMP")
#if LCD_WIDTH >= 20
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6" // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲｺｳｵﾝﾁｮｳｶ" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"     // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲﾃｲｵﾝﾐﾏﾝ" ("Err: MINTEMP BED")
#else
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd"                 // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲｺｳｵﾝ" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd"                 // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲﾃｲｵﾝ" ("Err: MINTEMP BED")
#endif
#define MSG_ERR_Z_HOMING                    MSG_HOME " " MSG_X MSG_Y " " MSG_FIRST                             // "ｻｷﾆ XY ｦﾌｯｷｻｾﾃｸﾀﾞｻｲ" or "ｻｷﾆ XY ｦﾌｯｷｻｾﾖ" ("G28 Z Forbidden")
#define MSG_HALTED                          "\xcc\xdf\xd8\xdd\xc0\xb0\xca\xc3\xb2\xbc\xbc\xcf\xbc\xc0"         // "ﾌﾟﾘﾝﾀｰﾊﾃｲｼｼﾏｼﾀ" ("PRINTER HALTED")
#define MSG_PLEASE_RESET                    "\xd8\xbe\xaf\xc4\xbc\xc3\xb8\xc0\xde\xbb\xb2"                     // "ﾘｾｯﾄｼﾃｸﾀﾞｻｲ" ("Please reset")
#define MSG_SHORT_DAY                       "d"                                                                // One character only
#define MSG_SHORT_HOUR                      "h"                                                                // One character only
#define MSG_SHORT_MINUTE                    "m"                                                                // One character only
#define MSG_HEATING                         "\xb6\xc8\xc2\xc1\xad\xb3"                                         // "ｶﾈﾂﾁｭｳ" ("Heating...")
#define MSG_HEATING_COMPLETE                "\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"                                 // "ｶﾈﾂｶﾝﾘｮｳ" ("Heating done.")
#define MSG_BED_HEATING                     "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xc1\xad\xb3"                 // "ﾍﾞｯﾄﾞ ｶﾈﾂﾁｭｳ" ("Bed Heating.")
#define MSG_BED_DONE                        "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"         // "ﾍﾞｯﾄﾞ ｶﾈﾂｶﾝﾘｮｳ" ("Bed done.")
#define MSG_DELTA_CALIBRATE                 "\xc3\xde\xd9\xc0\x20\xba\xb3\xbe\xb2"                             // "ﾃﾞﾙﾀ ｺｳｾｲ" ("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               "X\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Xｼﾞｸ ｺｳｾｲ" ("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               "Y\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Yｼﾞｸ ｺｳｾｲ" ("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               "Z\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Zｼﾞｸ ｺｳｾｲ" ("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          "\xc1\xad\xb3\xbc\xdd\x20\xba\xb3\xbe\xb2"                         // "ﾁｭｳｼﾝ ｺｳｾｲ" ("Calibrate Center")
#define MSG_INFO_MENU                       "\xba\xc9\xcc\xdf\xd8\xdd\xc0\xb0\xc6\xc2\xb2\xc3"                 // "ｺﾉﾌﾟﾘﾝﾀｰﾆﾂｲﾃ" ("About Printer")
#define MSG_INFO_PRINTER_MENU               "\xcc\xdf\xd8\xdd\xc0\xb0\xbc\xde\xae\xb3\xce\xb3"                 // "ﾌﾟﾘﾝﾀｰｼﾞｮｳﾎｳ" ("Printer Info")
#define MSG_INFO_STATS_MENU                 "\xcc\xdf\xd8\xdd\xc4\xbc\xde\xae\xb3\xb7\xae\xb3"                 // "ﾌﾟﾘﾝﾄｼﾞｮｳｷｮｳ" ("Printer Stats")
#define MSG_INFO_BOARD_MENU                 "\xbe\xb2\xb7\xde\xae\xb9\xb2\xbc\xde\xae\xb3\xce\xb3"             // "ｾｲｷﾞｮｹｲｼﾞｮｳﾎｳ" ("Board Info")
#define MSG_INFO_THERMISTOR_MENU            "\xbb\xb0\xd0\xbd\xc0\xb0"                                         // "ｻｰﾐｽﾀｰ" ("Thermistors")
#define MSG_INFO_EXTRUDERS                  "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\xbd\xb3"                     // "ｴｸｽﾄﾙｰﾀﾞｰｽｳ" ("Extruders")
#define MSG_INFO_BAUDRATE                   "\xce\xde\xb0\xda\xb0\xc4"                                         // "ﾎﾞｰﾚｰﾄ" ("Baud")
#define MSG_INFO_PROTOCOL                   "\xcc\xdf\xdb\xc4\xba\xd9"                                         // "ﾌﾟﾛﾄｺﾙ" ("Protocol")
#define MSG_LIGHTS_ON                       "\xb7\xae\xb3\xc0\xb2\xc5\xb2\xbc\xae\xb3\xd2\xb2\x20\xb5\xdd"     // "ｷｮｳﾀｲﾅｲｼｮｳﾒｲ ｵﾝ" ("Case light on")
#define MSG_LIGHTS_OFF                      "\xb7\xae\xb3\xc0\xb2\xc5\xb2\xbc\xae\xb3\xd2\xb2\x20\xb5\xcc"     // "ｷｮｳﾀｲﾅｲｼｮｳﾒｲ ｵﾌ" ("Case light off")
#define MSG_INFO_PRINT_COUNT                "\xcc\xdf\xd8\xdd\xc4\xbd\xb3"                                     // "ﾌﾟﾘﾝﾄｽｳ" ("Print Count")
#define MSG_INFO_COMPLETED_PRINTS           "\xb6\xdd\xd8\xae\xb3\xbd\xb3"                                     // "ｶﾝﾘｮｳｽｳ" ("Completed")
#define MSG_INFO_PRINT_TIME                 "\xcc\xdf\xd8\xdd\xc4\xbc\xde\xb6\xdd\xd9\xb2\xb9\xb2"             // "ﾌﾟﾘﾝﾄｼﾞｶﾝﾙｲｹｲ" ("Total print time")
#define MSG_INFO_PRINT_LONGEST              "\xbb\xb2\xc1\xae\xb3\xcc\xdf\xd8\xdd\xc4\xbc\xde\xb6\xdd"         // "ｻｲﾁｮｳﾌﾟﾘﾝﾄｼﾞｶﾝ" ("Longest job time")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_FILAMENT           "\xcc\xa8\xd7\xd2\xdd\xc4\xbc\xd6\xb3\xd8\xae\xb3\xd9\xb2\xb9\xb2" // "ﾌｨﾗﾒﾝﾄｼﾖｳﾘｮｳﾙｲｹｲ" ("Extruded total")
#else
  #define MSG_INFO_PRINT_FILAMENT           "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xbc\xd6\xb3\xd8\xae\xb3"         // "ﾌｨﾗﾒﾝﾄｿｳｼﾖｳﾘｮｳ" ("Extruded")
#endif
#define MSG_INFO_MIN_TEMP                   "\xbe\xaf\xc3\xb2\xbb\xb2\xc3\xb2\xb5\xdd"                         // "ｾｯﾃｲｻｲﾃｲｵﾝ" ("Min Temp")
#define MSG_INFO_MAX_TEMP                   "\xbe\xaf\xc3\xb2\xbb\xb2\xba\xb3\xb5\xdd"                         // "ｾｯﾃｲｻｲｺｳｵﾝ" ("Max Temp")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PSU                      "\xc3\xde\xdd\xb9\xde\xdd\xbc\xad\xcd\xde\xc2"                     // "ﾃﾞﾝｹﾞﾝｼｭﾍﾞﾂ" ("Power Supply")
#else
  #define MSG_INFO_PSU                      "\xc3\xde\xdd\xb9\xde\xdd"                                         // "ﾃﾞﾝｹﾞﾝ" ("Power Supply")
#endif
#define MSG_DRIVE_STRENGTH                  "\xd3\xb0\xc0\xb0\xb8\xc4\xde\xb3\xd8\xae\xb8"                     // "ﾓｰﾀｰｸﾄﾞｳﾘｮｸ" ("Drive Strength")
#if LCD_WIDTH >= 20
  #define MSG_DAC_PERCENT                   "DAC\xbc\xad\xc2\xd8\xae\xb8 %"                                    // "DACｼｭﾂﾘｮｸ %" ("Driver %")
#else
  #define MSG_DAC_PERCENT                   "DAC\xbc\xad\xc2\xd8\xae\xb8"                                      // "DACｼｭﾂﾘｮｸ" ("Driver %")
#endif
#define MSG_DAC_EEPROM_WRITE                MSG_STORE_EPROM                                                    // "ﾒﾓﾘﾍｶｸﾉｳ" ("DAC EEPROM Write")
#define MSG_FILAMENT_CHANGE_HEADER          "\xcc\xa8\xd7\xd2\xdd\xc4\xba\xb3\xb6\xdd"                         // "ﾌｨﾗﾒﾝﾄｺｳｶﾝ" ("CHANGE FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "\xc4\xde\xb3\xbb\xa6\xbe\xdd\xc0\xb8\xbc\xc3\xb8\xc0\xde\xbb\xb2" // "ﾄﾞｳｻｦｾﾝﾀｸｼﾃｸﾀﾞｻｲ" ("CHANGE OPTIONS:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "\xbb\xd7\xc6\xb5\xbc\xc0\xde\xbd"                                 // "ｻﾗﾆｵｼﾀﾞｽ" ("Extrude more")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                             // "ﾌﾟﾘﾝﾄｻｲｶｲ" ("Resume print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\xba\xb3\xb6\xdd\xa6\xb6\xb2\xbc\xbc\xcf\xbd"                     // "ｺｳｶﾝｦｶｲｼｼﾏｽ" ("Wait for start")
  #define MSG_FILAMENT_CHANGE_INIT_2        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("of the filament")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xc7\xb7\xc0\xde\xbc\xc1\xad\xb3"         // "ﾌｨﾗﾒﾝﾄﾇｷﾀﾞｼﾁｭｳ" ("Wait for")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2      "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xa6\xbf\xb3\xc6\xad\xb3\xbc,"            // "ﾌｨﾗﾒﾝﾄｦｿｳﾆｭｳｼ," ("Insert filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2      "\xb8\xd8\xaf\xb8\xbd\xd9\xc4\xbf\xde\xaf\xba\xb3\xbc\xcf\xbd"     // "ｸﾘｯｸｽﾙﾄｿﾞｯｺｳｼﾏｽ" ("and press button")
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xc3\xdd\xc1\xad\xb3"             // "ﾌｨﾗﾒﾝﾄｿｳﾃﾝﾁｭｳ" ("Wait for")
  #define MSG_FILAMENT_CHANGE_LOAD_2        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("filament load")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1     "\xcc\xa8\xd7\xd2\xdd\xc4\xb5\xbc\xc0\xde\xbc\xc1\xad\xb3"         // "ﾌｨﾗﾒﾝﾄｵｼﾀﾞｼﾁｭｳ" ("Wait for")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2     "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("filament extrude")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xcc\xdf\xd8\xdd\xc4\xa6\xbb\xb2\xb6\xb2\xbc\xcf\xbd"             // "ﾌﾟﾘﾝﾄｦｻｲｶｲｼﾏｽ" ("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2      "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"             // "ｼﾊﾞﾗｸｵﾏﾁｸﾀﾞｻｲ" ("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\xba\xb3\xb6\xdd\xa6\xb6\xb2\xbc\xbc\xcf\xbd"                     // "ｺｳｶﾝｦｶｲｼｼﾏｽ" ("Please wait...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "\xcc\xa8\xd7\xd2\xdd\xc4\xc7\xb7\xc0\xde\xbc\xc1\xad\xb3"         // "ﾌｨﾗﾒﾝﾄﾇｷﾀﾞｼﾁｭｳ" ("Ejecting...")
  #if LCD_WIDTH >= 20
    #define MSG_FILAMENT_CHANGE_INSERT_1    "\xbf\xb3\xc6\xad\xb3\xbc\x2c\xb8\xd8\xaf\xb8\xbc\xc3\xb8\xc0\xde\xbb\xb2" // "ｿｳﾆｭｳｼ,ｸﾘｯｸｼﾃｸﾀﾞｻｲ" ("Insert and Click")
  #else
    #define MSG_FILAMENT_CHANGE_INSERT_1    "\xbf\xb3\xc6\xad\xb3\xbc\x2c\xb8\xd8\xaf\xb8\xbe\xd6"                     // "ｿｳﾆｭｳｼ,ｸﾘｯｸｾﾖ" ("Insert and Click")
  #endif
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\xcc\xa8\xd7\xd2\xdd\xc4\xbf\xb3\xc3\xdd\xc1\xad\xb3"             // "ﾌｨﾗﾒﾝﾄｿｳﾃﾝﾁｭｳ" ("Loading...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1     "\xcc\xa8\xd7\xd2\xdd\xc4\xb5\xbc\xc0\xde\xbc\xc1\xad\xb3"         // "ﾌｨﾗﾒﾝﾄｵｼﾀﾞｼﾁｭｳ" ("Extruding...")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xcc\xdf\xd8\xdd\xc4\xa6\xbb\xb2\xb6\xb2\xbc\xcf\xbd"             // "ﾌﾟﾘﾝﾄｦｻｲｶｲｼﾏｽ" ("Resuming...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_KANA_H
