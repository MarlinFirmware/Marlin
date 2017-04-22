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

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" pripraven.")
#define MSG_SD_INSERTED                     _UxGT("Karta vlozena")
#define MSG_SD_REMOVED                      _UxGT("Karta vyjmuta")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstopy") // maximalne 8 znaku
#define MSG_MAIN                            _UxGT("Hlavni nabidka")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Uvolnit motory")
#define MSG_AUTO_HOME                       _UxGT("Domovska pozice")
#define MSG_AUTO_HOME_X                     _UxGT("Domu osa X")
#define MSG_AUTO_HOME_Y                     _UxGT("Domu osa Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Domu osa Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Mereni podlozky")
#define MSG_LEVEL_BED_WAITING               _UxGT("Kliknutim spustte")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Dalsi bod")
#define MSG_LEVEL_BED_DONE                  _UxGT("Mereni hotovo!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Storno")
#define MSG_SET_HOME_OFFSETS                _UxGT("Nastavit ofsety")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Ofsety nastaveny")
#define MSG_SET_ORIGIN                      _UxGT("Nastavit pocatek")
#define MSG_PREHEAT_1                       _UxGT("Zahrat PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Vse")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Podloz")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Nast")
#define MSG_PREHEAT_2                       _UxGT("Zahrat ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Vse")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Podloz")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Nast")
#define MSG_COOLDOWN                        _UxGT("Zchladit")
#define MSG_SWITCH_PS_ON                    _UxGT("Zapnout napajeni")
#define MSG_SWITCH_PS_OFF                   _UxGT("Vypnout napajeni")
#define MSG_EXTRUDE                         _UxGT("Vytlacit (extr.)")
#define MSG_RETRACT                         _UxGT("Zatlacit (retr.)")
#define MSG_MOVE_AXIS                       _UxGT("Posunout osy")
#define MSG_LEVEL_BED                       _UxGT("Vyrovnat podlozku")
#define MSG_MOVE_X                          _UxGT("Posunout X")
#define MSG_MOVE_Y                          _UxGT("Posunout Y")
#define MSG_MOVE_Z                          _UxGT("Posunout Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Posunout o 0,1mm")
#define MSG_MOVE_1MM                        _UxGT("Posunout o 1mm")
#define MSG_MOVE_10MM                       _UxGT("Posunout o 10mm")
#define MSG_SPEED                           _UxGT("Rychlost")
#define MSG_BED_Z                           _UxGT("Vyska podl.")
#define MSG_NOZZLE                          _UxGT("Tryska")
#define MSG_BED                             _UxGT("Podlozka")
#define MSG_FAN_SPEED                       _UxGT("Rychlost vent.")
#define MSG_FLOW                            _UxGT("Prutok")
#define MSG_CONTROL                         _UxGT("Ovladani")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fakt")
#define MSG_AUTOTEMP                        _UxGT("Autoteplota")
#define MSG_ON                              _UxGT("Zap")
#define MSG_OFF                             _UxGT("Vyp")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Vybrat")
#define MSG_ACC                             _UxGT("Zrychl")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retrakt")
#define MSG_A_TRAVEL                        _UxGT("A-prejezd")
#define MSG_XSTEPS                          _UxGT("Xkroku/mm")
#define MSG_YSTEPS                          _UxGT("Ykroku/mm")
#define MSG_ZSTEPS                          _UxGT("Zkroku/mm")
#define MSG_ESTEPS                          _UxGT("Ekroku/mm")
#define MSG_E1STEPS                         _UxGT("E1kroku/mm")
#define MSG_E2STEPS                         _UxGT("E2kroku/mm")
#define MSG_E3STEPS                         _UxGT("E3kroku/mm")
#define MSG_E4STEPS                         _UxGT("E4kroku/mm")
#define MSG_E5STEPS                         _UxGT("E5kroku/mm")
#define MSG_TEMPERATURE                     _UxGT("Teplota")
#define MSG_MOTION                          _UxGT("Pohyb")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E na mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Prum.")
#define MSG_CONTRAST                        _UxGT("Kontrast LCD")
#define MSG_STORE_EEPROM                    _UxGT("Ulozit nastaveni")
#define MSG_LOAD_EEPROM                     _UxGT("Nacist nastaveni")
#define MSG_RESTORE_FAILSAFE                _UxGT("Obnovit vychozi")
#define MSG_REFRESH                         _UxGT("Obnovit")
#define MSG_WATCH                           _UxGT("Info obrazovka")
#define MSG_PREPARE                         _UxGT("Priprava tisku")
#define MSG_TUNE                            _UxGT("Doladeni tisku")
#define MSG_PAUSE_PRINT                     _UxGT("Pozastavit tisk")
#define MSG_RESUME_PRINT                    _UxGT("Obnovit tisk")
#define MSG_STOP_PRINT                      _UxGT("Zastavit tisk")
#define MSG_CARD_MENU                       _UxGT("Tisknout z SD")
#define MSG_NO_CARD                         _UxGT("Zadna SD karta")
#define MSG_DWELL                           _UxGT("Uspano...")
#define MSG_USERWAIT                        _UxGT("Cekani na uziv...")
#define MSG_RESUMING                        _UxGT("Obnovovani tisku")
#define MSG_PRINT_ABORTED                   _UxGT("Tisk zrusen")
#define MSG_NO_MOVE                         _UxGT("Zadny pohyb.")
#define MSG_KILLED                          _UxGT("PRERUSENO. ")
#define MSG_STOPPED                         _UxGT("ZASTAVENO. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retrakt mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Vymena Re.mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraktovat  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Zvednuti Z mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Vymenit filament")
#define MSG_INIT_SDCARD                     _UxGT("Nacist SD kartu")
#define MSG_CNG_SDCARD                      _UxGT("Vymenit SD kartu")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z mimo podl")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("BLTouch Reset")
#define MSG_HOME                            _UxGT("Domu")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("prvni")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z ofset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_HEATING_FAILED_LCD              _UxGT("Chyba zahrivani")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("REDUND. TEPLOTA")
#define MSG_THERMAL_RUNAWAY                 _UxGT("TEPLOTNI SKOK")
#define MSG_ERR_MAXTEMP                     _UxGT("VYSOKA TEPLOTA")
#define MSG_ERR_MINTEMP                     _UxGT("NIZKA TEPLOTA")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("VYS. TEPL. PODL.")
#define MSG_ERR_MINTEMP_BED                 _UxGT("NIZ. TEPL. PODL.")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z ZAKAZANO")
#define MSG_HALTED                          _UxGT("TISK. ZASTAVENA")
#define MSG_PLEASE_RESET                    _UxGT("Provedte reset")
#define MSG_SHORT_DAY                       _UxGT("d")
#define MSG_SHORT_HOUR                      _UxGT("h")
#define MSG_SHORT_MINUTE                    _UxGT("m")
#define MSG_HEATING                         _UxGT("Zahrivani...")
#define MSG_HEATING_COMPLETE                _UxGT("Zahrati hotovo.")
#define MSG_BED_HEATING                     _UxGT("Zahrivani podl.")
#define MSG_BED_DONE                        _UxGT("Podlozka hotova.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrace")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibrovat X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibrovat Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibrovat Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibrovat Stred")
#define MSG_INFO_MENU                       _UxGT("O tiskarne")
#define MSG_INFO_PRINTER_MENU               _UxGT("Info o tiskarne")
#define MSG_INFO_STATS_MENU                 _UxGT("Statistika")
#define MSG_INFO_BOARD_MENU                 _UxGT("Info o desce")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistory")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrudery")
#define MSG_INFO_BAUDRATE                   _UxGT("Rychlost")
#define MSG_INFO_PROTOCOL                   _UxGT("Protokol")
#define MSG_LIGHTS_ON                       _UxGT("Osvetleni Zap")
#define MSG_LIGHTS_OFF                      _UxGT("Osvetleni Vyp")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Pocet tisku")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Dokonceno")
  #define MSG_INFO_PRINT_TIME               _UxGT("Celkovy cas")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Nejdelsi tisk")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Celkem vytlaceno")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Tisky")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Hotovo")
  #define MSG_INFO_PRINT_TIME               _UxGT("Cas")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Nejdelsi")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Vytlaceno")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Teplota min")
#define MSG_INFO_MAX_TEMP                   _UxGT("Teplota max")
#define MSG_INFO_PSU                        _UxGT("Nap. zdroj")
#define MSG_DRIVE_STRENGTH                  _UxGT("Buzeni motoru")
#define MSG_DAC_PERCENT                     _UxGT("Motor %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Ulozit do EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("VYMENA FILAMENTU")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("CO DAL?")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Jeste vytlacit")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Obnovit tisk")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Cekejte prosim")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("na zahajeni")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("vymeny filamentu")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Cekejte prosim")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("na vysunuti")
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        _UxGT("filamentu")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Vlozte filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("a stisknete")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("tlacitko...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Cekejte prosim")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("na zavedeni")
  #define MSG_FILAMENT_CHANGE_LOAD_3          _UxGT("filamentu")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Cekejte prosim")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("na vytlaceni")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       _UxGT("filamentu")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Cekejte prosim")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("na pokracovani")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("tisku")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Cekejte...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Vysouvani...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Vlozte, kliknete")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Zavadeni...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Vytlacovani...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Pokracovani...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_CZ_H
