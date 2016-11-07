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

#define WELCOME_MSG                         MACHINE_NAME " haz\xfdr."
#define MSG_SD_INSERTED                     "SD Yerle\xfeti."
#define MSG_SD_REMOVED                      "SD \xc7\xfdkar\xfdld\xfd."
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Ana"
#define MSG_AUTOSTART                       "Otoba\xfelat"
#define MSG_DISABLE_STEPPERS                "Motorlar\xfd Durdur"
#define MSG_AUTO_HOME                       "Eksenleri S\xfd\x66\xfdrla"
#define MSG_AUTO_HOME_X                     "X S\xfd\x66\xfdrla"
#define MSG_AUTO_HOME_Y                     "Y S\xfd\x66\xfdrla"
#define MSG_AUTO_HOME_Z                     "Z S\xfd\x66\xfdrla"
#define MSG_LEVEL_BED_HOMING                "XYZ S\xfd\x66\xfdrlan\xfdyor"
#define MSG_LEVEL_BED_WAITING               "Ba\xfelatmak i\xe7in t\xfdkla"
#define MSG_LEVEL_BED_NEXT_POINT            "S\xfdradaki Nokta"
#define MSG_LEVEL_BED_DONE                  "Seviyeleme Tamam!"
#define MSG_LEVEL_BED_CANCEL                "\xddptal"
#define MSG_SET_HOME_OFFSETS                "Offset Ayarla"
#define MSG_HOME_OFFSETS_APPLIED            "Offset tamam"
#define MSG_SET_ORIGIN                      "S\xfd\x66\xfdr Belirle"
#define MSG_PREHEAT_1                       "\xd6n Is\xfdnma PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " T\xfcm"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Tabla"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Ayar"
#define MSG_PREHEAT_2                       "\xd6n Is\xfdnma ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " T\xfcm"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Tabla"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Ayar"
#define MSG_COOLDOWN                        "So\xf0ut"
#define MSG_SWITCH_PS_ON                    "G\xfcc\xfc A\xe7"
#define MSG_SWITCH_PS_OFF                   "G\xfcc\xfc Kapat"
#define MSG_EXTRUDE                         "Extrude"
#define MSG_RETRACT                         "Geri \xc7ek"
#define MSG_MOVE_AXIS                       "Eksen Y\xf6net"
#define MSG_LEVEL_BED                       "Tabla Seviyele"
#define MSG_MOVE_X                          "X"
#define MSG_MOVE_Y                          "Y"
#define MSG_MOVE_Z                          "Z"
#define MSG_MOVE_E                          "Ekstruder"
#define MSG_MOVE_01MM                       "0.1mm"
#define MSG_MOVE_1MM                        "1mm"
#define MSG_MOVE_10MM                       "10mm"
#define MSG_SPEED                           "H\xfdz"
#define MSG_BED_Z                           "Tabla Z"
#define MSG_NOZZLE                          "Noz\xfcl"
#define MSG_BED                             "Tabla"
#define MSG_FAN_SPEED                       "Fan H\xfdz\xfd"
#define MSG_FLOW                            "Ak\xfd\xfe"
#define MSG_CONTROL                         "Kontrol"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " \xc7\x61rpan"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On "
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Se\xe7"
#define MSG_ACC                             "\xddvme"
#define MSG_VX_JERK                         "Vx-Jerk"
#define MSG_VY_JERK                         "Vy-Jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "S\xfd\x63\x61kl\xfdk"
#define MSG_MOTION                          "Hareket"
#define MSG_VOLUMETRIC                      "Filaman"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. \xc7\x61p"
#define MSG_CONTRAST                        "LCD Kontrast"
#define MSG_STORE_EPROM                     "Haf\xfdzaya Al"
#define MSG_LOAD_EPROM                      "Haf\xfdzadan Y\xfckle"
#define MSG_RESTORE_FAILSAFE                "Fabrika Ayarlar\xfd"
#define MSG_REFRESH                         "Yenile"
#define MSG_WATCH                           "Bilgi Ekran\xfd"
#define MSG_PREPARE                         "Haz\xfdrl\xfdk"
#define MSG_TUNE                            "Ayar"
#define MSG_PAUSE_PRINT                     "Duraklat"
#define MSG_RESUME_PRINT                    "S\xfcrd\xfcr"
#define MSG_STOP_PRINT                      "Durdur"
#define MSG_CARD_MENU                       "SD den Yazd\xfdr"
#define MSG_NO_CARD                         "SD Kart Yok"
#define MSG_DWELL                           "Uyku..."
#define MSG_USERWAIT                        "Operat\xf6r bekleniyor..."
#define MSG_RESUMING                        "Bask\xfd S\xfcrd\xfcr\xfcl\xfcyor"
#define MSG_PRINT_ABORTED                   "Bask\xfd Duruduruldu"
#define MSG_NO_MOVE                         "\xdd\xfelem yok."
#define MSG_KILLED                          "Kilitlendi. " //maybe a better turkish term needed
#define MSG_STOPPED                         "Durdu. "
#define MSG_CONTROL_RETRACT                 "Geri \xc7ek mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "Geri \xc7ekme  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Filaman De\xf0i\xfetir"
#define MSG_INIT_SDCARD                     "Init. SD"
#define MSG_CNG_SDCARD                      "SD De\xf0i\xfetir"
#define MSG_ZPROBE_OUT                      "Z Prob A\xe7\xfdk. Tabla"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   "S\xfd\x66\xfdrla BLTouch"
#define MSG_HOME                            "S\xfd\x66\xfdrla"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "\xf6nce"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Miniad\xfdm X"
#define MSG_BABYSTEP_Y                      "Miniad\xfdm Y"
#define MSG_BABYSTEP_Z                      "Miniad\xfdm Z"
#define MSG_ENDSTOP_ABORT                   "Endstop iptal"
#define MSG_HEATING_FAILED_LCD              "Is\xfdnma ba\xfe\x61\x72\xfds\xfdz"
#define MSG_ERR_REDUNDANT_TEMP              "Err: Ge\xe7ersiz S\xfd\x63akl\xfdk"
#define MSG_THERMAL_RUNAWAY                 "TERMAL PROBLEM"
#define MSG_ERR_MAXTEMP                     "Err: MAXSICAKLIK"
#define MSG_ERR_MINTEMP                     "Err: MINSICAKLIK"
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXSIC. TABLA"
#define MSG_ERR_MINTEMP_BED                 "Err: MINSIC. TABLA"
#define MSG_ERR_Z_HOMING                    "G28 Z Yap\xfdlamaz"
#define MSG_HALTED                          "YAZICI DURDURULDU"
#define MSG_PLEASE_RESET                    "L\xfctfen resetleyin"
#define MSG_SHORT_DAY                       "s" // One character only
#define MSG_SHORT_HOUR                      "S" // One character only
#define MSG_SHORT_MINUTE                    "d" // One character only
#define MSG_HEATING                         "Is\xfdn\xfdyor..."
#define MSG_HEATING_COMPLETE                "Is\xfdnma tamam."
#define MSG_BED_HEATING                     "Tabla Is\xfdn\xfdyor."
#define MSG_BED_DONE                        "Tabla haz\xfdr."
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrasyonu"
#define MSG_DELTA_CALIBRATE_X               "Ayarla X"
#define MSG_DELTA_CALIBRATE_Y               "Ayarla Y"
#define MSG_DELTA_CALIBRATE_Z               "Ayarla Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Ayarla Merkez"

#define MSG_INFO_MENU                       "Yaz\xfd\x63\xfd Hakk\xfdnda"
#define MSG_INFO_PRINTER_MENU               "Yaz\xfd\x63\xfd Bilgisi"
#define MSG_INFO_STATS_MENU                 "\xddstatistikler"
#define MSG_INFO_BOARD_MENU                 "Kontrol\xf6r Bilgisi"
#define MSG_INFO_THERMISTOR_MENU            "Termist\xf6rler"
#define MSG_INFO_EXTRUDERS                  "Ekstruderler"
#define MSG_INFO_BAUDRATE                   "\xddleti\xfeim H\xfdz\xfd"
#define MSG_INFO_PROTOCOL                   "Protokol"

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              "Bask\xfd Say\xfds\xfd"
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"
  #define MSG_INFO_PRINT_TIME               "Toplam Bask\xfd S\xfcresi"
  #define MSG_INFO_PRINT_LONGEST            "En Uzun Bask\xfd S\xfcresi"
  #define MSG_INFO_PRINT_FILAMENT           "Toplam Filaman"
#else
  #define MSG_INFO_PRINT_COUNT              "Bask\xfd"
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"
  #define MSG_INFO_PRINT_TIME               "S\xfcre"
  #define MSG_INFO_PRINT_LONGEST            "En Uzun"
  #define MSG_INFO_PRINT_FILAMENT           "Filaman"
#endif

#define MSG_INFO_MIN_TEMP                   "Min S\xfd\x63."
#define MSG_INFO_MAX_TEMP                   "Max S\xfd\x63."
#define MSG_INFO_PSU                        "G\xfc\xe7 Kayna\xf0\xfd"

#define MSG_DRIVE_STRENGTH                  "\x53\xfc\x72\xfc\x63\xfc \x47\xfc\x63\xfc"
#define MSG_DAC_PERCENT                     "\x53\xfc\x72\xfc\x63\xfc %"
#define MSG_DAC_EEPROM_WRITE                "DAC\x27\xfd EEPROM\x27\x61 Yaz"
#define MSG_FILAMENT_CHANGE_HEADER          "Filaman De\xf0i\xfetir"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Se\xe7enekler:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrude more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Bask\xfdy\xfd s\xfcrd\xfcr"
#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Ba\xfelama bekleniyor"
  #define MSG_FILAMENT_CHANGE_INIT_2          "filaman\xfdn"
  #define MSG_FILAMENT_CHANGE_INIT_3          "de\xf0i\xfeimi"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Bekleniyor"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filaman\xfdn \xe7\xfdkmas\xfd"
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        ""
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Filaman\xfd y\xfckle"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "ve devam i\xe7in"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "tu\xfea bas..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Bekleniyor"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filaman\xfdn y\xfcklenmesi"
  #define MSG_FILAMENT_CHANGE_LOAD_3          ""
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Bekleniyor"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filaman akmas\xfd"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       ""
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Bask\xfdn\xfdn s\xfcrd\xfcr\xfclmesini"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "bekle"
  #define MSG_FILAMENT_CHANGE_RESUME_3        ""
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "L\xfctfen bekleyiniz..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "\xc7\xfdkart\xfdl\xfdyor..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Y\xfckle ve bas"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Y\xfckl\xfcyor..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Ak\xfdt\xfdl\xfdyor..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "S\xfcrd\xfcr\xfcl\xfcyor..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_TR_H
