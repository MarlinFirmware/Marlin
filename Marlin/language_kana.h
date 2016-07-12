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
#define MSG_PREHEAT_PLA                     "PLA \xd6\xc8\xc2"                                                 // "PLA ﾖﾈﾂ" ("Preheat PLA")
#define MSG_PREHEAT_PLA_N                   MSG_PREHEAT_PLA " "
#define MSG_PREHEAT_PLA_ALL                 MSG_PREHEAT_PLA " \xbd\xcd\xde\xc3"                                // " ｽﾍﾞﾃ" (" All")
#define MSG_PREHEAT_PLA_BEDONLY             MSG_PREHEAT_PLA " \xcd\xde\xaf\xc4\xde"                            // " ﾍﾞｯﾄﾞ" (" Bed")
#define MSG_PREHEAT_PLA_SETTINGS            MSG_PREHEAT_PLA " \xbe\xaf\xc3\xb2"                                // " ｾｯﾃｲ" (" conf")
#define MSG_PREHEAT_ABS                     "ABS \xd6\xc8\xc2"                                                 // "ABS ﾖﾈﾂ" ("Preheat ABS")
#define MSG_PREHEAT_ABS_N                   MSG_PREHEAT_ABS " "
#define MSG_PREHEAT_ABS_ALL                 MSG_PREHEAT_ABS " \xbd\xcd\xde\xc3"                                // " ｽﾍﾞﾃ" (" All")
#define MSG_PREHEAT_ABS_BEDONLY             MSG_PREHEAT_ABS " \xcd\xde\xaf\xc4\xde"                            // " ﾍﾞｯﾄﾞ" (" Bed")
#define MSG_PREHEAT_ABS_SETTINGS            MSG_PREHEAT_ABS " \xbe\xaf\xc3\xb2"                                // " ｾｯﾃｲ" (" conf")
#define MSG_COOLDOWN                        "\xb6\xc8\xc2\xc3\xb2\xbc"                                         // "ｶﾈﾂﾃｲｼ" ("Cooldown")
#define MSG_SWITCH_PS_ON                    "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xdd"                             // "ﾃﾞﾝｹﾞﾝ ｵﾝ" ("Switch power on")
#define MSG_SWITCH_PS_OFF                   "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"                             // "ﾃﾞﾝｹﾞﾝ ｵﾌ" ("Switch power off")
#define MSG_EXTRUDE                         "\xb5\xbc\xc0\xde\xbc"                                             // "ｵｼﾀﾞｼ" ("Extrude")
#define MSG_RETRACT                         "\xcb\xb7\xba\xd0\xbe\xaf\xc3\xb2"                                 // "ﾋｷｺﾐｾｯﾃｲ" ("Retract")
#define MSG_MOVE_AXIS                       "\xbc\xde\xb8\xb2\xc4\xde\xb3"                                     // "ｼﾞｸｲﾄﾞｳ" ("Move axis")
#define MSG_LEVEL_BED                       "\xcd\xde\xaf\xc4\xde\xda\xcd\xde\xd8\xdd\xb8\xde"                 // "ﾍﾞｯﾄﾞﾚﾍﾞﾘﾝｸﾞ" ("Level bed")
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
#if LCD_WIDTH < 20
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde"                             // "ｼﾞﾄﾞｳｵﾝﾄﾞ" ("Autotemp")
#else
  #define MSG_AUTOTEMP                      "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde\xbe\xb2\xb7\xde\xae"         // "ｼﾞﾄﾞｳｵﾝﾄﾞｾｲｷﾞｮ" ("Autotemp")
#endif
#define MSG_ON                              "\xb5\xdd "                                                        // "ｵﾝ " ("On ")
#define MSG_OFF                             "\xb5\xcc "                                                        // "ｵﾌ " ("Off")
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#if LCD_WIDTH < 20
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde"                                             // "ｶｿｸﾄﾞ" ("Accel")
  #define MSG_VXY_JERK                      "XY\xbc\xde\xb8\x20\xd4\xb8\xc4\xde"                               // "XYｼﾞｸ ﾔｸﾄﾞ" ("Vxy-jerk")
  #define MSG_VZ_JERK                       "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde"                                // "Zｼﾞｸ ﾔｸﾄﾞ" ("Vz-jerk")
  #define MSG_VE_JERK                       "E\x20\xd4\xb8\xc4\xde"                                            // "E ﾔｸﾄﾞ" ("Ve-jerk")
  #define MSG_VMAX                          "max\xb5\xb8\xd8\xbf\xb8\xc4\xde "                                 // "maxｵｸﾘｿｸﾄﾞ" ("Vmax ")
#else
  #define MSG_ACC                           "\xb6\xbf\xb8\xc4\xde mm/s2"                                       // "ｶｿｸﾄﾞ mm/s2" ("Accel")
  #define MSG_VXY_JERK                      "XY\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                          // "XYｼﾞｸ ﾔｸﾄﾞ mm/s" ("Vxy-jerk")
  #define MSG_VZ_JERK                       "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                           // "Zｼﾞｸ ﾔｸﾄﾞ mm/s" ("Vz-jerk")
  #define MSG_VE_JERK                       "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\x20\xd4\xb8\xc4\xde"         // "ｴｸｽﾄﾙｰﾀﾞｰ ﾔｸﾄﾞ" ("Ve-jerk")
  #define MSG_VMAX                          "\xbb\xb2\xc0\xde\xb2\xb5\xb8\xd8\xbf\xb8\xc4\xde "                // "ｻｲﾀﾞｲｵｸﾘｿｸﾄﾞ " ("Vmax ")
#endif
#define MSG_X                               "X"
#define MSG_Y                               "Y"
#define MSG_Z                               "Z"
#define MSG_E                               "E"
#if LCD_WIDTH < 20
  #define MSG_VMIN                          "min\xb5\xb8\xd8\xbf\xb8\xc4\xde"                                  // "minｵｸﾘｿｸﾄﾞ" ("Vmin")
  #define MSG_VTRAV_MIN                     "min\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"                              // "minｲﾄﾞｳｿｸﾄﾞ" ("VTrav min")
  #define MSG_AMAX                          "max\xb6\xbf\xb8 "                                                 // "maxｶｿｸ " ("Amax ")
#else
  #define MSG_VMIN                          "\xbb\xb2\xbc\xae\xb3\xb5\xb8\xd8\xbf\xb8\xc4\xde"                 // "ｻｲｼｮｳｵｸﾘｿｸﾄﾞ" ("Vmin")
  #define MSG_VTRAV_MIN                     "\xbb\xb2\xbc\xae\xb3\xb2\xc4\xde\xb3\xbf\xb8\xc4\xde"             // "ｻｲｼｮｳｲﾄﾞｳｿｸﾄﾞ" ("VTrav min")
  #define MSG_AMAX                          "\xbb\xb2\xc0\xde\xb2\xb6\xbf\xb8\xc4\xde "                        // "ｻｲﾀﾞｲｶｿｸﾄﾞ " ("Amax ")
#endif
#define MSG_A_RETRACT                       "\xcb\xb7\xba\xd0\xb6\xbf\xb8\xc4\xde"                             // "ﾋｷｺﾐｶｿｸﾄﾞ" ("A-retract")
#define MSG_A_TRAVEL                        "\xb2\xc4\xde\xb3\xb6\xbf\xb8\xc4\xde"                             // "ｲﾄﾞｳｶｿｸﾄﾞ" ("A-travel")
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "\xb5\xdd\xc4\xde"                                                 // "ｵﾝﾄﾞ" ("Temperature")
#define MSG_MOTION                          "\xb3\xba\xde\xb7\xbe\xaf\xc3\xb2"                                 // "ｳｺﾞｷｾｯﾃｲ" ("Motion")
#define MSG_VOLUMETRIC                      "\xcc\xa8\xd7\xd2\xdd\xc4"                                         // "ﾌｨﾗﾒﾝﾄ" ("Filament")
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#if LCD_WIDTH < 20
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xb9\xb2"                                 // "ﾌｨﾗﾒﾝﾄｹｲ" ("Fil. Dia.")
#else
  #define MSG_FILAMENT_DIAM                 "\xcc\xa8\xd7\xd2\xdd\xc4\xc1\xae\xaf\xb9\xb2"                     // "ﾌｨﾗﾒﾝﾄﾁｮｯｹｲ" ("Fil. Dia.")
#endif
#define MSG_CONTRAST                        "LCD\xba\xdd\xc4\xd7\xbd\xc4"                                      // "LCDｺﾝﾄﾗｽﾄ" ("LCD contrast")
#define MSG_STORE_EPROM                     "\xd2\xd3\xd8\xcd\xb6\xb8\xc9\xb3"                                 // "ﾒﾓﾘﾍｶｸﾉｳ" ("Store memory")
#define MSG_LOAD_EPROM                      "\xd2\xd3\xd8\xb6\xd7\xd6\xd0\xba\xd0"                             // "ﾒﾓﾘｶﾗﾖﾐｺﾐ" ("Load memory")
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
#if LCD_WIDTH < 20
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3"                                     // "ﾋｷｺﾐﾘｮｳ" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S"                                    // "ﾋｷｺﾐﾘｮｳS" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde"                                 // "ﾋｷｺﾐｿｸﾄﾞ" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb"                                     // "ﾉｽﾞﾙﾀｲﾋ" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3"                                     // "ﾎｼｮｳﾘｮｳ" ("UnRet +mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S"                                    // "ﾎｼｮｳﾘｮｳS" ("S UnRet+mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde"                                 // "ﾎｼｮｳｿｸﾄﾞ" ("UnRet  V")
#else
  #define MSG_CONTROL_RETRACT               "\xcb\xb7\xba\xd0\xd8\xae\xb3 mm"                                  // "ﾋｷｺﾐﾘｮｳ mm" ("Retract mm")
  #define MSG_CONTROL_RETRACT_SWAP          "\xcb\xb7\xba\xd0\xd8\xae\xb3S mm"                                 // "ﾋｷｺﾐﾘｮｳS mm" ("Swap Re.mm")
  #define MSG_CONTROL_RETRACTF              "\xcb\xb7\xba\xd0\xbf\xb8\xc4\xde mm/s"                            // "ﾋｷｺﾐｿｸﾄﾞ mm/s" ("Retract  V")
  #define MSG_CONTROL_RETRACT_ZLIFT         "\xc9\xbd\xde\xd9\xc0\xb2\xcb mm"                                  // "ﾉｽﾞﾙﾀｲﾋ mm" ("Hop mm")
  #define MSG_CONTROL_RETRACT_RECOVER       "\xce\xbc\xae\xb3\xd8\xae\xb3 mm"                                  // "ﾎｼｮｳﾘｮｳ mm" ("UnRet +mm")
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP  "\xce\xbc\xae\xb3\xd8\xae\xb3S mm"                                 // "ﾎｼｮｳﾘｮｳS mm" ("S UnRet+mm")
  #define MSG_CONTROL_RETRACT_RECOVERF      "\xce\xbc\xae\xb3\xbf\xb8\xc4\xde mm/s"                            // "ﾎｼｮｳｿｸﾄﾞ mm/s" ("UnRet  V")
#endif
#define MSG_AUTORETRACT                     "\xbc\xde\xc4\xde\xb3\xcb\xb7\xba\xd0"                             // "ｼﾞﾄﾞｳﾋｷｺﾐ" ("AutoRetr.")
#define MSG_FILAMENTCHANGE                  "\xcc\xa8\xd7\xd2\xdd\xc4\xba\xb3\xb6\xdd"                         // "ﾌｨﾗﾒﾝﾄｺｳｶﾝ" ("Change filament")
#define MSG_INIT_SDCARD                     "SD\xb6\xb0\xc4\xde\xbb\xb2\xd6\xd0\xba\xd0"                       // "SDｶｰﾄﾞｻｲﾖﾐｺﾐ" ("Init. SD card")
#define MSG_CNG_SDCARD                      "SD\xb6\xb0\xc4\xde\xba\xb3\xb6\xdd"                               // "SDｶｰﾄﾞｺｳｶﾝ" ("Change SD card")
#define MSG_ZPROBE_OUT                      "Z\xcc\xdf\xdb\xb0\xcc\xde\x20\xcd\xde\xaf\xc4\xde\xb6\xde\xb2"    // "Zﾌﾟﾛｰﾌﾞ ﾍﾞｯﾄﾞｶﾞｲ" ("Z probe out. bed")

#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
/*
#if LCD_WIDTH < 20
  #define MSG_YX_UNHOMED                    "\xbb\xb7\xc6X/Y\xa6\xcc\xaf\xb7\xbb\xbe\xd6"                                   // "ｻｷﾆX/Yｦﾌｯｷｻｾﾖ" ("Home X/Y before Z")
  #define MSG_XYZ_UNHOMED                   "\xbb\xb7\xc6\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7\xa6\xbe\xd6"                      // "ｻｷﾆｹﾞﾝﾃﾝﾌｯｷｦｾﾖ" ("Home XYZ first")
#else
  #define MSG_YX_UNHOMED                    "\xbb\xb7\xc6X/Y\xa6\xcc\xaf\xb7\xbb\xbe\xc3\xb8\xc0\xde\xbb\xb2"               // "ｻｷﾆX/Yｦﾌｯｷｻｾﾃｸﾀﾞｻｲ" ("Home X/Y before Z")
  #define MSG_XYZ_UNHOMED                   "\xbb\xb7\xc6\xb9\xde\xdd\xc3\xdd\xcc\xaf\xb7\xa6\xbc\xc3\xb8\xc0\xde\xbb\xb2"  // "ｻｷﾆｹﾞﾝﾃﾝﾌｯｷｦｼﾃｸﾀﾞｻｲ" ("Home XYZ first")
#endif
*/
#define MSG_ZPROBE_ZOFFSET                  "Z\xb5\xcc\xbe\xaf\xc4"                                            // "Zｵﾌｾｯﾄ" ("Z Offset")
#define MSG_BABYSTEP_X                      "X\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Xｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep X")
#define MSG_BABYSTEP_Y                      "Y\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Yｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep Y")
#define MSG_BABYSTEP_Z                      "Z\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                            // "Zｼﾞｸ ﾋﾞﾄﾞｳ" ("Babystep Z")
#if LCD_WIDTH < 20
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1"                 // "ｲﾄﾞｳｹﾞﾝｶｲｹﾝﾁ" ("Endstop abort")
#else
  #define MSG_ENDSTOP_ABORT                 "\xb2\xc4\xde\xb3\xb9\xde\xdd\xb6\xb2\xb9\xdd\xc1\xb7\xc9\xb3"     // "ｲﾄﾞｳｹﾞﾝｶｲｹﾝﾁｷﾉｳ" ("Endstop abort")
#endif
#define MSG_HEATING_FAILED_LCD              "\xb6\xc8\xc2\xbc\xaf\xca\xdf\xb2"                                 // "ｶﾈﾂｼｯﾊﾟｲ" ("Heating failed")
#if LCD_WIDTH < 20
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0"                 // "ｴﾗｰ:ｼﾞｮｳﾁｮｳｻｰﾐｽﾀ" ("Err: REDUNDANT TEMP")
#else
  #define MSG_ERR_REDUNDANT_TEMP            "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0\xb0\xb7\xc9\xb3" // "ｴﾗｰ:ｼﾞｮｳﾁｮｳｻｰﾐｽﾀｰｷﾉｳ" ("Err: REDUNDANT TEMP")
#endif
#define MSG_THERMAL_RUNAWAY                 "\xc8\xc2\xce\xde\xb3\xbf\xb3"                                                  // "ﾈﾂﾎﾞｳｿｳ" ("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6"                         // "ｴﾗｰ:ｻｲｺｳｵﾝﾁｮｳｶ" ("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"                             // "ｴﾗｰ:ｻｲﾃｲｵﾝﾐﾏﾝ" ("Err: MINTEMP")
#if LCD_WIDTH < 20
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd"                 // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲｺｳｵﾝ" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd"                 // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲﾃｲｵﾝ" ("Err: MINTEMP BED")
#else
  #define MSG_ERR_MAXTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6" // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲｺｳｵﾝﾁｮｳｶ" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"     // "ｴﾗｰ:ﾍﾞｯﾄﾞ ｻｲﾃｲｵﾝﾐﾏﾝ" ("Err: MINTEMP BED")
#endif
#define MSG_END_HOUR                        "\xbc\xde\xb6\xdd"                                                 // "ｼﾞｶﾝ" ("hours")
#define MSG_END_MINUTE                      "\xcc\xdd"                                                         // "ﾌﾝ" ("minutes")
#define MSG_HEATING                         "\xb6\xc8\xc2\xc1\xad\xb3"                                         // "ｶﾈﾂﾁｭｳ" ("Heating...")
#define MSG_HEATING_COMPLETE                "\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"                                 // "ｶﾈﾂｶﾝﾘｮｳ" ("Heating done.")
#define MSG_BED_HEATING                     "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xc1\xad\xb3"                 // "ﾍﾞｯﾄﾞ ｶﾈﾂﾁｭｳ" ("Bed Heating.")
#define MSG_BED_DONE                        "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"         // "ﾍﾞｯﾄﾞ ｶﾈﾂｶﾝﾘｮｳ" ("Bed done.")
#define MSG_DELTA_CALIBRATE                 "\xc3\xde\xd9\xc0\x20\xba\xb3\xbe\xb2"                             // "ﾃﾞﾙﾀ ｺｳｾｲ" ("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               "X\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Xｼﾞｸ ｺｳｾｲ" ("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               "Y\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Yｼﾞｸ ｺｳｾｲ" ("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               "Z\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                                // "Zｼﾞｸ ｺｳｾｲ" ("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          "\xc1\xad\xb3\xbc\xdd\x20\xba\xb3\xbe\xb2"                         // "ﾁｭｳｼﾝ ｺｳｾｲ" ("Calibrate Center")

#endif // LANGUAGE_KANA_H
