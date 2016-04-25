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

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " pripraven."
#define MSG_SD_INSERTED                     "Karta vlozena"
#define MSG_SD_REMOVED                      "Karta vyjmuta"
#define MSG_MAIN                            "Hlavni nabidka"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Uvolnit motory"
#define MSG_AUTO_HOME                       "Domovska pozice"
#define MSG_LEVEL_BED_HOMING                "Mereni podlozky"
#define MSG_LEVEL_BED_WAITING               "Kliknutim spustte"
#define MSG_LEVEL_BED_DONE                  "Mereni hotovo!"
#define MSG_LEVEL_BED_CANCEL                "Storno"
#define MSG_SET_HOME_OFFSETS                "Nastavit ofsety"
#define MSG_HOME_OFFSETS_APPLIED            "Ofsety nastaveny"
#define MSG_SET_ORIGIN                      "Nastavit pocatek"
#define MSG_PREHEAT_PLA                     "Zahrat PLA"
#define MSG_PREHEAT_PLA_N                   MSG_PREHEAT_PLA " "
#define MSG_PREHEAT_PLA_ALL                 MSG_PREHEAT_PLA " Vse"
#define MSG_PREHEAT_PLA_BEDONLY             MSG_PREHEAT_PLA " Podloz"
#define MSG_PREHEAT_PLA_SETTINGS            MSG_PREHEAT_PLA " Nast"
#define MSG_PREHEAT_ABS                     "Zahrat ABS"
#define MSG_PREHEAT_ABS_N                   MSG_PREHEAT_ABS " "
#define MSG_PREHEAT_ABS_ALL                 MSG_PREHEAT_ABS " Vse"
#define MSG_PREHEAT_ABS_BEDONLY             MSG_PREHEAT_ABS " Podloz"
#define MSG_PREHEAT_ABS_SETTINGS            MSG_PREHEAT_ABS " Nast"
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
#define MSG_ACC                             "Zrychl"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_X                               "X"
#define MSG_Y                               "Y"
#define MSG_Z                               "Z"
#define MSG_E                               "E"
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
#define MSG_YX_UNHOMED                      "Domu X/Y pred Z"
#define MSG_ZPROBE_ZOFFSET                  "Z ofset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Chyba zahrivani"
#define MSG_ERR_REDUNDANT_TEMP              "Chyba: REDUNDANTNI TEPLOTA"
#define MSG_THERMAL_RUNAWAY                 "TEPLOTNI SKOK"
#define MSG_ERR_MAXTEMP                     "Chyba: VYSOKA TEPLOTA"
#define MSG_ERR_MINTEMP                     "Chyba: NIZKA TEPLOTA"
#define MSG_ERR_MAXTEMP_BED                 "Chyba: VYSOKA TEPLOTA PODL."
#define MSG_ERR_MINTEMP_BED                 "Chyba: NIZKA TEPLOTA PODL."
#define MSG_END_HOUR                        "hod"
#define MSG_END_MINUTE                      "min"
#define MSG_HEATING                         "Zahrivani..."
#define MSG_HEATING_COMPLETE                "Zahrati hotovo."
#define MSG_BED_HEATING                     "Zahrivani podl."
#define MSG_BED_DONE                        "Podlozka hotova."

#define MSG_DELTA_CALIBRATE                 "Delta Kalibrace"
#define MSG_DELTA_CALIBRATE_X               "Kalibrovat X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibrovat Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibrovat Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibrovat Stred"

#endif // LANGUAGE_CZ_H
