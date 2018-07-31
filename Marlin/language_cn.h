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
 * Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_CN_H
#define LANGUAGE_CN_H

#define DISPLAY_CHARSET_ISO10646_CN
#define CHARSIZE 2

#define WELCOME_MSG                         "\xa4\xa5\xa6\xa7"
#define MSG_SD_INSERTED                     "\xa8\xa9\xaa\xab"
#define MSG_SD_REMOVED                      "\xa8\xa9\xac\xad"
#define MSG_MAIN                            "\xae\xaf\xb0"
#define MSG_AUTOSTART                       "\xb1\xb2\xb3\xb4"
#define MSG_DISABLE_STEPPERS                "\xb5\xb6\xb7\xb8\xb9\xba"
#define MSG_AUTO_HOME                       "\xbb\xbc\xbd"
#define MSG_TMC_Z_CALIBRATION               "Calibrate Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
#define MSG_SET_HOME_OFFSETS                "\xbe\xbf\xbb\xbc\xbd\xc0\xc1"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
#define MSG_SET_ORIGIN                      "\xbe\xbf\xbc\xbd"
#define MSG_PREHEAT_1                       "\xc3\xc4 PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " \xc5\xc6"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " \xc4\xc7"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " \xbe\xbf"
#define MSG_PREHEAT_2                       "\xc3\xc4 ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " \xc5\xc6"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " \xbe\xc6"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " \xbe\xbf"
#define MSG_COOLDOWN                        "\xc8\xc9"
#define MSG_SWITCH_PS_ON                    "\xb9\xcb\xca\xb3"
#define MSG_SWITCH_PS_OFF                   "\xb9\xcb\xb5\xb6"
#define MSG_EXTRUDE                         "\xcc\xad"
#define MSG_RETRACT                         "\xbb\xcd"
#define MSG_MOVE_AXIS                       "\xc1\xb2\xce"
#define MSG_BED_LEVELING                    "\xcf\xe0\xc4\xc7"
#define MSG_LEVEL_BED                       "\xcf\xe0\xc4\xc7"
#define MSG_MOVE_X                          "\xc1\xb2 X"
#define MSG_MOVE_Y                          "\xc1\xb2 Y"
#define MSG_MOVE_Z                          "\xc1\xb2 Z"
#define MSG_MOVE_E                          "\xcc\xad\xba"
#define MSG_MOVE_01MM                       "\xc1\xb2 0.1mm"
#define MSG_MOVE_1MM                        "\xc1\xb2 1mm"
#define MSG_MOVE_10MM                       "\xc1\xb2 10mm"
#define MSG_SPEED                           "\xd1\xd2"
#define MSG_NOZZLE                          "\xd3\xd4"
#define MSG_BED                             "\xc4\xc7"
#define MSG_FAN_SPEED                       "\xd5\xd6\xd1\xd2"
#define MSG_FLOW                            "\xcc\xad\xd1\xd2"
#define MSG_CONTROL                         "\xd8\xd9"
#define MSG_MIN                             LCD_STR_THERMOMETER " \xda\xdb"
#define MSG_MAX                             LCD_STR_THERMOMETER " \xda\xdc"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " \xdd\xde"
#define MSG_AUTOTEMP                        "\xb1\xb2\xd8\xc9"
#define MSG_ON                              "\xb3 "  // intentional space to shift wide symbol to the left
#define MSG_OFF                             "\xb5 "  // intentional space to shift wide symbol to the left
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Accel"
#define MSG_JERK                            "Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       "Va-jerk"
  #define MSG_VB_JERK                       "Vb-jerk"
  #define MSG_VC_JERK                       "Vc-jerk"
#else
  #define MSG_VA_JERK                       "Vx-jerk"
  #define MSG_VB_JERK                       "Vy-jerk"
  #define MSG_VC_JERK                       "Vz-jerk"
#endif
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_STEPS_PER_MM                    "Steps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_E1STEPS                         "E1steps/mm"
#define MSG_E2STEPS                         "E2steps/mm"
#define MSG_E3STEPS                         "E3steps/mm"
#define MSG_E4STEPS                         "E4steps/mm"
#define MSG_E5STEPS                         "E5steps/mm"
#define MSG_TEMPERATURE                     "\xc9\xd2"
#define MSG_MOTION                          "\xdf\xb2"
#define MSG_FILAMENT                        "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "LCD contrast"
#define MSG_STORE_EEPROM                    "Store memory"
#define MSG_LOAD_EEPROM                     "Load memory"
#define MSG_RESTORE_FAILSAFE                "Restore failsafe"
#define MSG_REFRESH                         "Refresh"
#define MSG_WATCH                           "\xec\xed\xee\xef"
#define MSG_PREPARE                         "\xa4\xa5"
#define MSG_TUNE                            "\xcf\xf0"
#define MSG_PAUSE_PRINT                     "\xf1\xf2\xca\xf3"
#define MSG_RESUME_PRINT                    "\xf4\xf5\xca\xf3"
#define MSG_STOP_PRINT                      "\xf2\xf6\xca\xf3"
#define MSG_CARD_MENU                       "\xaf\xb0"
#define MSG_NO_CARD                         "\xf9\xa8"
#define MSG_DWELL                           "Sleep..."
#define MSG_USERWAIT                        "Wait for user..."
#define MSG_PRINT_ABORTED                   "Print aborted"
#define MSG_NO_MOVE                         "No move."
#define MSG_KILLED                          "KILLED. "
#define MSG_STOPPED                         "STOPPED. "
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Change filament"
#define MSG_INIT_SDCARD                     "Init. SD card"
#define MSG_CNG_SDCARD                      "Change SD card"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXTEMP BED"
#define MSG_DELTA_CALIBRATE                 "Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               "Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrate Center"

#endif // LANGUAGE_CN_H
