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

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_KANA

// 片仮名表示定義
#define WELCOME_MSG                         MACHINE_NAME " ready."
#define MSG_SD_INSERTED                     "\xb6\xb0\xc4\xde\x20\xbf\xb3\xc6\xad\xb3\xbb\xda\xcf\xbc\xc0" // "Card inserted"
#define MSG_SD_REMOVED                      "\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                 // "Card removed"
#define MSG_MAIN                            "\xd2\xb2\xdd"                                                 // "Main"
#define MSG_AUTOSTART                       "\xbc\xde\xc4\xde\xb3\xb6\xb2\xbc"                             // "Autostart"
#define MSG_DISABLE_STEPPERS                "\xd3\xb0\xc0\xb0\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"         // "Disable steppers"
#define MSG_AUTO_HOME                       "\xb9\xde\xdd\xc3\xdd\xc6\xb2\xc4\xde\xb3"                     // "Auto home"
#define MSG_LEVEL_BED_HOMING                "\xb9\xde\xdd\xc3\xdd\xc6\xb2\xc4\xde\xb3"                     // "Homing XYZ"
#define MSG_SET_HOME_OFFSETS                "\xb7\xbc\xde\xad\xdd\xb5\xcc\xbe\xaf\xc4\xbe\xaf\xc3\xb2"     // "Set home offsets"
#define MSG_SET_ORIGIN                      "\xb7\xbc\xde\xad\xdd\xbe\xaf\xc4"                             // "Set origin"
#define MSG_PREHEAT_PLA                     "PLA \xd6\xc8\xc2"                                             // "Preheat PLA"
#define MSG_PREHEAT_PLA_N                   MSG_PREHEAT_PLA " "
#define MSG_PREHEAT_PLA_ALL                 MSG_PREHEAT_PLA " \xbd\xcd\xde\xc3"                            // " All"
#define MSG_PREHEAT_PLA_BEDONLY             MSG_PREHEAT_PLA " \xcd\xde\xaf\xc4\xde"                        // "Bed"
#define MSG_PREHEAT_PLA_SETTINGS            MSG_PREHEAT_PLA " \xbe\xaf\xc3\xb2"                            // "conf"
#define MSG_PREHEAT_ABS                     "ABS \xd6\xc8\xc2"                                             // "Preheat ABS"
#define MSG_PREHEAT_ABS_N                   MSG_PREHEAT_ABS " "
#define MSG_PREHEAT_ABS_ALL                 MSG_PREHEAT_ABS " \xbd\xcd\xde\xc3"                            // " All"
#define MSG_PREHEAT_ABS_BEDONLY             MSG_PREHEAT_ABS " \xcd\xde\xaf\xc4\xde"                        // "Bed"
#define MSG_PREHEAT_ABS_SETTINGS            MSG_PREHEAT_ABS " \xbe\xaf\xc3\xb2"                            // "conf"
#define MSG_COOLDOWN                        "\xb6\xc8\xc2\xc3\xb2\xbc"                                     // "Cooldown"
#define MSG_SWITCH_PS_ON                    "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xdd"                         // "Switch power on"
#define MSG_SWITCH_PS_OFF                   "\xc3\xde\xdd\xb9\xde\xdd\x20\xb5\xcc"                         // "Switch power off"
#define MSG_EXTRUDE                         "\xb5\xbc\xc0\xde\xbc"                                         // "Extrude"
#define MSG_RETRACT                         "\xcb\xb7\xba\xd0\xbe\xaf\xc3\xb2"                             // "Retract"
#define MSG_MOVE_AXIS                       "\xbc\xde\xb8\xb2\xc4\xde\xb3"                                 // "Move axis"
#define MSG_LEVEL_BED                       "\xcd\xde\xaf\xc4\xde\xda\xcd\xde\xd8\xdd\xb8\xde"             // "Level bed"
#define MSG_MOVE_X                          "X\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                            // "Move X"
#define MSG_MOVE_Y                          "Y\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                            // "Move Y"
#define MSG_MOVE_Z                          "Z\xbc\xde\xb8\x20\xb2\xc4\xde\xb3"                            // "Move Z"
#define MSG_MOVE_E                          "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0"                         // "Extruder"
#define MSG_MOVE_01MM                       "0.1mm \xb2\xc4\xde\xb3"                                       // "Move 0.1mm"
#define MSG_MOVE_1MM                        "  1mm \xb2\xc4\xde\xb3"                                       // "Move 1mm"
#define MSG_MOVE_10MM                       " 10mm \xb2\xc4\xde\xb3"                                       // "Move 10mm"
#define MSG_SPEED                           "\xbd\xcb\xdf\xb0\xc4\xde"                                     // "Speed"
#define MSG_BED_Z                           "Z\xb5\xcc\xbe\xaf\xc4"                                        // "Bed Z"
#define MSG_NOZZLE                          "\xc9\xbd\xde\xd9"                                             // "Nozzle"
#define MSG_BED                             "\xcd\xde\xaf\xc4\xde"                                         // "Bed"
#define MSG_FAN_SPEED                       "\xcc\xa7\xdd\xbf\xb8\xc4\xde"                                 // "Fan speed"
#define MSG_FLOW                            "\xb5\xb8\xd8\xd8\xae\xb3"                                     // "Flow"
#define MSG_CONTROL                         "\xba\xdd\xc4\xdb\xb0\xd9"                                     // "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " \xbb\xb2\xc3\xb2"                        // " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " \xbb\xb2\xba\xb3"                        // " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " \xcc\xa7\xb8\xc0\xb0"                    // " Fact"
#define MSG_AUTOTEMP                        "\xbc\xde\xc4\xde\xb3\xb5\xdd\xc4\xde"                         // "Autotemp"
#define MSG_ON                              "\xb5\xdd "                                                    // "On "
#define MSG_OFF                             "\xb5\xcc "                                                    // "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "\xb6\xbf\xb8\xc4\xde mm/s^2"                                  // "Accel"
#define MSG_VXY_JERK                        "XY\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                      // "Vxy-jerk"
#define MSG_VZ_JERK                         "Z\xbc\xde\xb8\x20\xd4\xb8\xc4\xde mm/s"                       // "Vz-jerk"
#define MSG_VE_JERK                         "\xb4\xb8\xbd\xc4\xd9\xb0\xc0\xde\xb0\x20\xd4\xb8\xc4\xde"     // "Ve-jerk"
#define MSG_VMAX                            "\xbb\xb2\xc0\xde\xb2\xcc\xa8\xb0\xc4\xde\xda\xb0\xc4 "        // "Vmax "
#define MSG_X                               "X"                                                            // "x"
#define MSG_Y                               "Y"                                                            // "y"
#define MSG_Z                               "Z"                                                            // "z"
#define MSG_E                               "E"                                                            // "e"
#define MSG_VMIN                            "\xbb\xb2\xbc\xae\xb3\xcc\xa8\xb0\xc4\xde\xda\xb0\xc4"         // "Vmin"
#define MSG_VTRAV_MIN                       "\xbb\xb2\xbc\xae\xb3\xc4\xd7\xcd\xde\xd9\xda\xb0\xc4"         // "VTrav min"
#define MSG_AMAX                            "\xbb\xb2\xc0\xde\xb2\xb6\xbf\xb8\xc4\xde "                    // "Amax "
#define MSG_A_RETRACT                       "\xcb\xb7\xba\xd0\xb6\xbf\xb8\xc4\xde"                         // "A-retract"
#define MSG_A_TRAVEL                        "\xc4\xd7\xcd\xde\xd9\xb6\xbf\xb8\xc4\xde"                     // "A-travel"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "\xb5\xdd\xc4\xde"                                              // "Temperature"
#define MSG_MOTION                          "\xb3\xba\xde\xb7\xbe\xaf\xc3\xb2"                              // "Motion"
#define MSG_VOLUMETRIC                      "\xcc\xa8\xd7\xd2\xdd\xc4"                                      // "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "\xcc\xa8\xd7\xd2\xdd\xc4\xc1\xae\xaf\xb9\xb2"                  // "Fil. Dia."
#define MSG_CONTRAST                        "LCD\xba\xdd\xc4\xd7\xbd\xc4"                                   // "LCD contrast"
#define MSG_STORE_EPROM                     "\xd2\xd3\xd8\xcd\xb6\xb8\xc9\xb3"                              // "Store memory"
#define MSG_LOAD_EPROM                      "\xd2\xd3\xd8\xb6\xd7\xd6\xd0\xba\xd0"                          // "Load memory"
#define MSG_RESTORE_FAILSAFE                "\xbe\xaf\xc3\xb2\xd8\xbe\xaf\xc4"                              // "Restore failsafe"
#define MSG_REFRESH                         "\xd8\xcc\xda\xaf\xbc\xad"                                      // "Refresh"
#define MSG_WATCH                           "\xb2\xdd\xcc\xab"                                              // "Info screen"
#define MSG_PREPARE                         "\xbc\xde\xad\xdd\xcb\xde\xbe\xaf\xc3\xb2"                      // "Prepare"
#define MSG_TUNE                            "\xc1\xae\xb3\xbe\xb2"                                          // "Tune"
#define MSG_PAUSE_PRINT                     "\xb2\xc1\xbc\xde\xc3\xb2\xbc"                                  // "Pause print"
#define MSG_RESUME_PRINT                    "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                          // "Resume print"
#define MSG_STOP_PRINT                      "\xcc\xdf\xd8\xdd\xc4\xc3\xb2\xbc"                              // "Stop print"
#define MSG_CARD_MENU                       "SD\xb6\xb0\xc4\xde\xb6\xd7\xcc\xdf\xd8\xdd\xc4"                // "Print from SD"
#define MSG_NO_CARD                         "SD\xb6\xb0\xc4\xde\xb6\xde\xb1\xd8\xcf\xbe\xdd"                // "No SD card"
#define MSG_DWELL                           "\xbd\xd8\xb0\xcc\xdf"                                          // "Sleep..."
#define MSG_USERWAIT                        "\xbc\xca\xde\xd7\xb8\xb5\xcf\xc1\xb8\xc0\xde\xbb\xb2"          // "Wait for user..."
#define MSG_RESUMING                        "\xcc\xdf\xd8\xdd\xc4\xbb\xb2\xb6\xb2"                          // "Resuming print"
#define MSG_PRINT_ABORTED                   "\xcc\xdf\xd8\xdd\xc4\xc1\xad\xb3\xbc\xbb\xda\xcf\xbc\xc0"      // "Print aborted"
#define MSG_NO_MOVE                         "\xb3\xba\xde\xb7\xcf\xbe\xdd"                                  // "No move."
#define MSG_KILLED                          "\xcb\xbc\xde\xae\xb3\xc3\xb2\xbc"                              // "KILLED. "
#define MSG_STOPPED                         "\xc3\xb2\xbc\xbc\xcf\xbc\xc0"                                  // "STOPPED. "
#define MSG_CONTROL_RETRACT                 "\xcb\xb7\xba\xd0\xd8\xae\xb3 mm"                               // "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "\xcb\xb7\xba\xd0\xd8\xae\xb3S mm"                              // "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "\xcb\xb7\xba\xd0\xda\xb0\xc4 mm/s"                             // "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "\xc9\xbd\xde\xd9\xc0\xb2\xcb mm"                               // "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "\xd8\xb6\xca\xde\xb0 +mm"                                      // "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "\xd8\xb6\xca\xde\xb0S +mm"                                     // "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "\xd8\xb6\xca\xde\xb0\xda\xb0\xc4 mm/s"                         // "UnRet  V"
#define MSG_AUTORETRACT                     "\xbc\xde\xc4\xde\xb3\xcb\xb7\xba\xd0"                             // "AutoRetr."
#define MSG_FILAMENTCHANGE                  "\xcc\xa8\xd7\xd2\xdd\xc4\xba\xb3\xb6\xdd"                      // "Change filament"
#define MSG_INIT_SDCARD                     "SD\xb6\xb0\xc4\xde\xbb\xb2\xd6\xd0\xba\xd0"                    // "Init. SD card"
#define MSG_CNG_SDCARD                      "SD\xb6\xb0\xc4\xde\xba\xb3\xb6\xdd"                            // "Change SD card"
#define MSG_ZPROBE_OUT                      "Z\xcc\xdf\xdb\xb0\xcc\xde\x20\xcd\xde\xaf\xc4\xde\xb6\xde\xb2" // "Z probe out. bed"
#define MSG_YX_UNHOMED                      "\xb9\xde\xdd\xc3\xdd\xcaXY\xb2\xc4\xde\xb3\xba\xdeZ"           // "Home X/Y before Z"
#define MSG_ZPROBE_ZOFFSET                  "Z\xb5\xcc\xbe\xaf\xc4"                                         // "Z Offset"
#define MSG_BABYSTEP_X                      "X\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                         // "Babystep X"
#define MSG_BABYSTEP_Y                      "Y\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                         // "Babystep Y"
#define MSG_BABYSTEP_Z                      "Z\xbc\xde\xb8\x20\xcb\xde\xc4\xde\xb3"                         // "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "\xb4\xdd\xc4\xde\xbd\xc4\xaf\xcc\xdf\x20\xbb\xc4\xde\xb3"      // "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "\xb6\xc8\xc2\xbc\xaf\xca\xde\xb2"                              // "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "\xb4\xd7\xb0:\xbc\xde\xae\xb3\xc1\xae\xb3\xbb\xb0\xd0\xbd\xc0\xb0\xb7\xc9\xb3" // "Err: REDUNDANT TEMP ERROR"
#define MSG_THERMAL_RUNAWAY                 "\xc8\xc2\xce\xde\xb3\xbf\xb3"                                                  // "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6"                         // "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "\xb4\xd7\xb0:\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"                             // "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xba\xb3\xb5\xdd\xc1\xae\xb3\xb6" // "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "\xb4\xd7\xb0:\xcd\xde\xaf\xc4\xde\x20\xbb\xb2\xc3\xb2\xb5\xdd\xd0\xcf\xdd"     // "Err: MINTEMP BED"
#define MSG_END_HOUR                        "\xbc\xde\xb6\xdd"                                              // "hours"
#define MSG_END_MINUTE                      "\xcc\xdd"                                                      // "minutes"
#define MSG_HEATING                         "\xb6\xc8\xc2\xc1\xad\xb3"                                      // "Heating..."
#define MSG_HEATING_COMPLETE                "\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"                              // "Heating done."
#define MSG_BED_HEATING                     "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xc1\xad\xb3"              // "Bed Heating."
#define MSG_BED_DONE                        "\xcd\xde\xaf\xc4\xde\x20\xb6\xc8\xc2\xb6\xdd\xd8\xae\xb3"      // "Bed done."

#if ENABLED(DELTA_CALIBRATION_MENU)
  #define MSG_DELTA_CALIBRATE               "\xc3\xde\xd9\xc0\x20\xba\xb3\xbe\xb2"                          // "Delta Calibration"
  #define MSG_DELTA_CALIBRATE_X             "X\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                             // "Calibrate X"
  #define MSG_DELTA_CALIBRATE_Y             "Y\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                             // "Calibrate Y"
  #define MSG_DELTA_CALIBRATE_Z             "Z\xbc\xde\xb8\x20\xba\xb3\xbe\xb2"                             // "Calibrate Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "\xc1\xad\xb3\xbc\xdd\x20\xba\xb3\xbe\xb2"                      // "Calibrate Center"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_KANA_H
