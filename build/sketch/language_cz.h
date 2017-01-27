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
 * Czech
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 * Translated by Petr Zahradnik, Computer Laboratory
 * Blog and video blog Zahradnik se bavi
 * http://www.zahradniksebavi.cz
 *
 */
#ifndef LANGUAGE_CZ_H
#define LANGUAGE_CZ_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " pripraven."
#define MSG_SD_INSERTED                     "Karta vlozena"
#define MSG_SD_REMOVED                      "Karta vyjmuta"
#define MSG_LCD_ENDSTOPS                    "Endstopy" // maximalne 8 znaku
#define MSG_MAIN                            "Hlavni nabidka"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Uvolnit motory"
#define MSG_AUTO_HOME                       "Domovska pozice"
#define MSG_AUTO_HOME_X                     "Domu osa X"
#define MSG_AUTO_HOME_Y                     "Domu osa Y"
#define MSG_AUTO_HOME_Z                     "Domu osa Z"
#define MSG_LEVEL_BED_HOMING                "Mereni podlozky"
#define MSG_LEVEL_BED_WAITING               "Kliknutim spustte"
#define MSG_LEVEL_BED_NEXT_POINT            "Dalsi bod"
#define MSG_LEVEL_BED_DONE                  "Mereni hotovo!"
#define MSG_LEVEL_BED_CANCEL                "Storno"
#define MSG_SET_HOME_OFFSETS                "Nastavit ofsety"
#define MSG_HOME_OFFSETS_APPLIED            "Ofsety nastaveny"
#define MSG_SET_ORIGIN                      "Nastavit pocatek"
#define MSG_PREHEAT_1                       "Zahrat PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Vse"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Podloz"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Nast"
#define MSG_PREHEAT_2                       "Zahrat ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Vse"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Podloz"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Nast"
#define MSG_COOLDOWN                        "Zchladit"
#define MSG_SWITCH_PS_ON                    "Zapnout napajeni"
#define MSG_SWITCH_PS_OFF                   "Vypnout napajeni"
#define MSG_EXTRUDE                         "Vytlacit (extr.)"
#define MSG_RETRACT                         "Zatlacit (retr.)"
#define MSG_MOVE_AXIS                       "Posunout osy"
#define MSG_LEVEL_BED                       "Vyrovnat podlozku"
#define MSG_MOVE_X                          "Posunout X"
#define MSG_MOVE_Y                          "Posunout Y"
#define MSG_MOVE_Z                          "Posunout Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Posunout o 0,1mm"
#define MSG_MOVE_1MM                        "Posunout o 1mm"
#define MSG_MOVE_10MM                       "Posunout o 10mm"
#define MSG_SPEED                           "Rychlost"
#define MSG_BED_Z                           "Vyska podl."
#define MSG_NOZZLE                          "Tryska"
#define MSG_BED                             "Podlozka"
#define MSG_FAN_SPEED                       "Rychlost vent."
#define MSG_FLOW                            "Prutok"
#define MSG_CONTROL                         "Ovladani"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fakt"
#define MSG_AUTOTEMP                        "Autoteplota"
#define MSG_ON                              "Zap"
#define MSG_OFF                             "Vyp"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Vybrat"
#define MSG_ACC                             "Zrychl"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retrakt"
#define MSG_A_TRAVEL                        "A-prejezd"
#define MSG_XSTEPS                          "Xkroku/mm"
#define MSG_YSTEPS                          "Ykroku/mm"
#define MSG_ZSTEPS                          "Zkroku/mm"
#define MSG_ESTEPS                          "Ekroku/mm"
#define MSG_TEMPERATURE                     "Teplota"
#define MSG_MOTION                          "Pohyb"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E na mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Prum."
#define MSG_CONTRAST                        "Kontrast LCD"
#define MSG_STORE_EPROM                     "Ulozit nastaveni"
#define MSG_LOAD_EPROM                      "Nacist nastaveni"
#define MSG_RESTORE_FAILSAFE                "Obnovit vychozi"
#define MSG_REFRESH                         "Obnovit"
#define MSG_WATCH                           "Info obrazovka"
#define MSG_PREPARE                         "Priprava tisku"
#define MSG_TUNE                            "Doladeni tisku"
#define MSG_PAUSE_PRINT                     "Pozastavit tisk"
#define MSG_RESUME_PRINT                    "Obnovit tisk"
#define MSG_STOP_PRINT                      "Zastavit tisk"
#define MSG_CARD_MENU                       "Tisknout z SD"
#define MSG_NO_CARD                         "Zadna SD karta"
#define MSG_DWELL                           "Uspano..."
#define MSG_USERWAIT                        "Cekani na uziv..."
#define MSG_RESUMING                        "Obnovovani tisku"
#define MSG_PRINT_ABORTED                   "Tisk zrusen"
#define MSG_NO_MOVE                         "Zadny pohyb."
#define MSG_KILLED                          "PRERUSENO. "
#define MSG_STOPPED                         "ZASTAVENO. "
#define MSG_CONTROL_RETRACT                 "Retrakt mm"
#define MSG_CONTROL_RETRACT_SWAP            "Vymena Re.mm"
#define MSG_CONTROL_RETRACTF                "Retraktovat  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Zvednuti Z mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Vymenit filament"
#define MSG_INIT_SDCARD                     "Nacist SD kartu"
#define MSG_CNG_SDCARD                      "Vymenit SD kartu"
#define MSG_ZPROBE_OUT                      "Sonda Z mimo podl"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   "BLTouch Reset"
#define MSG_HOME                            "Domu"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "prvni"
#define MSG_ZPROBE_ZOFFSET                  "Z ofset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Chyba zahrivani"
#define MSG_ERR_REDUNDANT_TEMP              "REDUND. TEPLOTA"
#define MSG_THERMAL_RUNAWAY                 "TEPLOTNI SKOK"
#define MSG_ERR_MAXTEMP                     "VYSOKA TEPLOTA"
#define MSG_ERR_MINTEMP                     "NIZKA TEPLOTA"
#define MSG_ERR_MAXTEMP_BED                 "VYS. TEPL. PODL."
#define MSG_ERR_MINTEMP_BED                 "NIZ. TEPL. PODL."
#define MSG_ERR_Z_HOMING                    "G28 Z ZAKAZANO"
#define MSG_HALTED                          "TISK. ZASTAVENA"
#define MSG_PLEASE_RESET                    "Provedte reset"
#define MSG_SHORT_DAY                       "d"
#define MSG_SHORT_HOUR                      "h"
#define MSG_SHORT_MINUTE                    "m"
#define MSG_HEATING                         "Zahrivani..."
#define MSG_HEATING_COMPLETE                "Zahrati hotovo."
#define MSG_BED_HEATING                     "Zahrivani podl."
#define MSG_BED_DONE                        "Podlozka hotova."
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrace"
#define MSG_DELTA_CALIBRATE_X               "Kalibrovat X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibrovat Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibrovat Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibrovat Stred"
#define MSG_INFO_MENU                       "O tiskarne"
#define MSG_INFO_PRINTER_MENU               "Info o tiskarne"
#define MSG_INFO_STATS_MENU                 "Statistika"
#define MSG_INFO_BOARD_MENU                 "Info o desce"
#define MSG_INFO_THERMISTOR_MENU            "Termistory"
#define MSG_INFO_EXTRUDERS                  "Extrudery"
#define MSG_INFO_BAUDRATE                   "Rychlost"
#define MSG_INFO_PROTOCOL                   "Protokol"
#define MSG_LIGHTS_ON                       "Osvetleni Zap"
#define MSG_LIGHTS_OFF                      "Osvetleni Vyp"
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Pocet tisku"
  #define MSG_INFO_COMPLETED_PRINTS         "Dokonceno"
  #define MSG_INFO_PRINT_TIME               "Celkovy cas"
  #define MSG_INFO_PRINT_LONGEST            "Nejdelsi tisk"
  #define MSG_INFO_PRINT_FILAMENT           "Celkem vytlaceno"
#else
  #define MSG_INFO_PRINT_COUNT              "Tisky"
  #define MSG_INFO_COMPLETED_PRINTS         "Hotovo"
  #define MSG_INFO_PRINT_TIME               "Cas"
  #define MSG_INFO_PRINT_LONGEST            "Nejdelsi"
  #define MSG_INFO_PRINT_FILAMENT           "Vytlaceno"
#endif
#define MSG_INFO_MIN_TEMP                   "Teplota min"
#define MSG_INFO_MAX_TEMP                   "Teplota max"
#define MSG_INFO_PSU                        "Nap. zdroj"
#define MSG_DRIVE_STRENGTH                  "Buzeni motoru"
#define MSG_DAC_PERCENT                     "Motor %"
#define MSG_DAC_EEPROM_WRITE                "Ulozit do EEPROM"

#define MSG_FILAMENT_CHANGE_HEADER          "VYMENA FILAMENTU"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "CO DAL?"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Jeste vytlacit"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Obnovit tisk"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Cekejte prosim"
  #define MSG_FILAMENT_CHANGE_INIT_2          "na zahajeni"
  #define MSG_FILAMENT_CHANGE_INIT_3          "vymeny filamentu"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Cekejte prosim"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "na vysunuti"
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        "filamentu"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Vlozte filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "a stisknete"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "tlacitko..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Cekejte prosim"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "na zavedeni"
  #define MSG_FILAMENT_CHANGE_LOAD_3          "filamentu"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Cekejte prosim"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "na vytlaceni"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "filamentu"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Cekejte prosim"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "na pokracovani"
  #define MSG_FILAMENT_CHANGE_RESUME_3        "tisku"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Cekejte..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Vysouvani..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Vlozte, kliknete"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Zavadeni..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Vytlacovani..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Pokracovani..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_CZ_H
