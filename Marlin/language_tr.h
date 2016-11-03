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
 * English
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_TR_H
#define LANGUAGE_TR_H

#define DISPLAY_CHARSET_ISO10646_TR

//#define SIMULATE_ROMFONT //Comment in to see what is seen on the character based displays
//#if DISABLED(SIMULATE_ROMFONT) && DISABLED(DISPLAY_CHARSET_ISO10646_1) && DISABLED(DISPLAY_CHARSET_ISO10646_5) && DISABLED(DISPLAY_CHARSET_ISO10646_KANA) && DISABLED(DISPLAY_CHARSET_ISO10646_GREEK) && DISABLED(DISPLAY_CHARSET_ISO10646_CN) && DISABLED(DISPLAY_CHARSET_ISO10646_TR)
//  #define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.
//#endif

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME " haz\xfdr."
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     "SD Yerle\xfeti."
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      "SD \xc7\xfdkar\xfdld\xfd."
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            "Ana"
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       "Otoba\xfelat"
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                "Motorlar\xfd Durdur"
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       "Eksenleri S\xfd\x66\xfdrla"
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     "X S\xfd\x66\xfdrla"
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     "Y S\xfd\x66\xfdrla"
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     "Z S\xfd\x66\xfdrla"
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                "XYZ S\xfd\x66\xfdrlan\xfdyor"
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               "Ba\xfelatmak i\xe7in t\xfdkla"
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            "S\xfdradaki Nokta"
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  "Seviyeleme Tamam!"
#endif
#ifndef MSG_LEVEL_BED_CANCEL
  #define MSG_LEVEL_BED_CANCEL                "\xddptal"
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                "Offset Ayarla"
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            "Offset tamam"
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      "S\xfd\x66\xfdr Belirle"
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       "\xd6n Is\xfdnma PLA"
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " T\xfcm"
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Tabla"
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Ayar"
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       "\xd6n Is\xfdnma ABS"
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " T\xfcm"
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Tabla"
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Ayar"
#endif
#ifndef MSG_H1
  #define MSG_H1                              "1"
#endif
#ifndef MSG_H2
  #define MSG_H2                              "2"
#endif
#ifndef MSG_H3
  #define MSG_H3                              "3"
#endif
#ifndef MSG_H4
  #define MSG_H4                              "4"
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        "So\xf0ut"
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    "G\xfcc\xfc A\xe7"
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   "G\xfcc\xfc Kapat"
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         "Extrude"
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         "Geri \xc7ek"
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       "Eksen Y\xf6net"
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       "Tabla Seviyele"
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          "X"
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          "Y"
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          "Z"
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          "Ekstruder"
#endif
#ifndef MSG_MOVE_E1
  #define MSG_MOVE_E1                         "1"
#endif
#ifndef MSG_MOVE_E2
  #define MSG_MOVE_E2                         "2"
#endif
#ifndef MSG_MOVE_E3
  #define MSG_MOVE_E3                         "3"
#endif
#ifndef MSG_MOVE_E4
  #define MSG_MOVE_E4                         "4"
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       "0.1mm"
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        "1mm"
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       "10mm"
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           "H\xfdz"
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           "Tabla Z"
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          "Noz\xfcl"
#endif
#ifndef MSG_N1
  #define MSG_N1                              " 1"
#endif
#ifndef MSG_N2
  #define MSG_N2                              " 2"
#endif
#ifndef MSG_N3
  #define MSG_N3                              " 3"
#endif
#ifndef MSG_N4
  #define MSG_N4                              " 4"
#endif
#ifndef MSG_BED
  #define MSG_BED                             "Tabla"
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       "Fan H\xfdz\xfd"
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            "Ak\xfd\xfe"
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         "Kontrol"
#endif
#ifndef MSG_MIN
  #define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#endif
#ifndef MSG_MAX
  #define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#endif
#ifndef MSG_FACTOR
  #define MSG_FACTOR                          " " LCD_STR_THERMOMETER " \xc7\x61rpan"
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP                        "Autotemp"
#endif
#ifndef MSG_ON
  #define MSG_ON                              "On "
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             "Off"
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P                           "PID-P"
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I                           "PID-I"
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D                           "PID-D"
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C                           "PID-C"
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT                          "Se\xe7"
#endif
#ifndef MSG_E1
  #define MSG_E1                              " E1"
#endif
#ifndef MSG_E2
  #define MSG_E2                              " E2"
#endif
#ifndef MSG_E3
  #define MSG_E3                              " E3"
#endif
#ifndef MSG_E4
  #define MSG_E4                              " E4"
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             "\xddvme"
#endif
#ifndef MSG_VXY_JERK
  #define MSG_VXY_JERK                        "Vxy-jerk"
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK                         "Vz-jerk"
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         "Ve-jerk"
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX                            "Vmax "
#endif
#ifndef MSG_X
  #define MSG_X                               "X"
#endif
#ifndef MSG_Y
  #define MSG_Y                               "Y"
#endif
#ifndef MSG_Z
  #define MSG_Z                               "Z"
#endif
#ifndef MSG_E
  #define MSG_E                               "E"
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN                            "Vmin"
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN                       "VTrav min"
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            "Amax "
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       "A-retract"
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        "A-travel"
#endif
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS                          "Xsteps/mm"
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS                          "Ysteps/mm"
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS                          "Zsteps/mm"
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          "Esteps/mm"
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     "S\xfd\x63\x61kl\xfdk"
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          "Hareket"
#endif
#ifndef MSG_VOLUMETRIC
  #define MSG_VOLUMETRIC                      "Filaman"
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   "Fil. \xc7\x61p"
#endif
#ifndef MSG_DIAM_E1
  #define MSG_DIAM_E1                         " 1"
#endif
#ifndef MSG_DIAM_E2
  #define MSG_DIAM_E2                         " 2"
#endif
#ifndef MSG_DIAM_E3
  #define MSG_DIAM_E3                         " 3"
#endif
#ifndef MSG_DIAM_E4
  #define MSG_DIAM_E4                         " 4"
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST                        "LCD Kontrast"
#endif
#ifndef MSG_STORE_EPROM
  #define MSG_STORE_EPROM                     "Haf\xfdzaya Al"
#endif
#ifndef MSG_LOAD_EPROM
  #define MSG_LOAD_EPROM                      "Haf\xfdzadan Y\xfckle"
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                "Fabrika Ayarlar\xfd"
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         "Yenile"
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           "Bilgi Ekran\xfd"
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         "Haz\xfdrl\xfdk"
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            "Ayar"
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     "Duraklat"
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    "S\xfcrd\xfcr"
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      "Durdur"
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       "SD den Yazd\xfdr"
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         "SD Kart Yok"
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           "Uyku..."
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        "Operat\xf6r bekleniyor..."
#endif
#ifndef MSG_RESUMING
  #define MSG_RESUMING                        "Bask\xfd S\xfcrd\xfcr\xfcl\xfcyor"
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   "Bask\xfd Duruduruldu"
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         "\xdd\xfelem yok."
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          "Kilitlendi. " //maybe a better turkish term needed
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         "Durdu. "
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 "Geri \xc7ek mm"
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF                "Geri \xc7ekme  V"
#endif
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     "AutoRetr."
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  "Filaman De\xf0i\xfetir"
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     "Init. SD"
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      "SD De\xf0i\xfetir"
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      "Z Prob A\xe7\xfdk. Tabla"
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            "S\xfd\x66\xfdrla"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           "\xf6nce"
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X                      "Miniad\xfdm X"
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y                      "Miniad\xfdm Y"
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z                      "Miniad\xfdm Z"
#endif
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   "Endstop iptal"
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              "Is\xfdnma ba\xfe\x61\x72\xfds\xfdz"
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              "Err: Ge\xe7ersiz S\xfd\x63akl\xfdk"
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 "TERMAL PROBLEM"
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     "Err: MAXSICAKLIK"
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     "Err: MINSICAKLIK"
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 "Err: MAXSIC. TABLA"
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 "Err: MINSIC. TABLA"
#endif
#ifndef MSG_HALTED
  #define MSG_HALTED                          "YAZICI DURDURULDU"
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_PLEASE_RESET                    "L\xfctfen resetleyin"
#endif
#ifndef MSG_SHORT_DAY
  #define MSG_SHORT_DAY                       "s" // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR                      "S" // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    "d" // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         "Is\xfdn\xfdyor..."
#endif
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE                "Is\xfdnma tamam."
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     "Tabla Is\xfdn\xfdyor."
#endif
#ifndef MSG_BED_DONE
  #define MSG_BED_DONE                        "Tabla haz\xfdr."
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 "Delta Kalibrasyonu"
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               "Ayarla X"
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               "Ayarla Y"
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               "Ayarla Z"
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          "Ayarla Merkez"
#endif

#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU                       "Yaz\xfd\x63\xfd Hakk\xfdnda"
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               "Yaz\xfd\x63\xfd Bilgisi"
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU                 "\xddstatistikler"
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 "Kontrol\xf6r Bilgisi"
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            "Termist\xf6rler"
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  "Ekstruderler"
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   "\xddleti\xfeim H\xfdz\xfd"
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   "Protokol"
#endif

#if LCD_WIDTH > 19
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              "Bask\xfd Say\xfds\xfd"
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               "Toplam Bask\xfd S\xfcresi"
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            "En Uzun Bask\xfd S\xfcresi"
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           "Toplam Filaman"
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              "Bask\xfd"
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               "S\xfcre"
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            "En Uzun"
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           "Filaman"
  #endif
#endif

#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   "Min S\xfd\x63."
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   "Max S\xfd\x63."
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        "G\xfc\xe7 Kayna\xf0\xfd"
#endif

#ifndef MSG_FILAMENT_CHANGE_HEADER
  #define MSG_FILAMENT_CHANGE_HEADER          "Filaman De\xf0i\xfetir"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Se\xe7enekler:"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrude more"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Bask\xfdy\xfd s\xfcrd\xfcr"
#endif
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          "Ba\xfelama bekleniyor"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_2
    #define MSG_FILAMENT_CHANGE_INIT_2          "filaman\xfdn"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_3
    #define MSG_FILAMENT_CHANGE_INIT_3          "de\xf0i\xfeimi"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Bekleniyor"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_2
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filaman\xfdn \xe7\xfdkmas\xfd"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_3
    #define MSG_FILAMENT_CHANGE_UNLOAD_3        ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        "Filaman\xfd y\xfckle"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_2
    #define MSG_FILAMENT_CHANGE_INSERT_2        "ve devam i\xe7in"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_3
    #define MSG_FILAMENT_CHANGE_INSERT_3        "tu\xfea bas..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Bekleniyor"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_2
    #define MSG_FILAMENT_CHANGE_LOAD_2          "filaman\xfdn y\xfcklenmesi"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_3
    #define MSG_FILAMENT_CHANGE_LOAD_3          ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Bekleniyor"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_2
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filaman akmas\xfd"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_3
    #define MSG_FILAMENT_CHANGE_EXTRUDE_3       ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        "Bask\xfdn\xfdn s\xfcrd\xfcr\xfclmesini"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_2
    #define MSG_FILAMENT_CHANGE_RESUME_2        "bekle"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_3
    #define MSG_FILAMENT_CHANGE_RESUME_3        ""
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          "L\xfctfen bekleyiniz..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "\xc7\xfdkart\xfdl\xfdyor..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        "Y\xfckle ve bas"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Y\xfckl\xfcyor..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Ak\xfdt\xfdl\xfdyor..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        "S\xfcrd\xfcr\xfcl\xfcyor..."
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_EN_H
