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
 * Polish for HD44780 display - no accented characters
 */

#ifndef LANGUAGE_PL_HD44780_H
#define LANGUAGE_PL_HD44780_H

#define NOT_EXTENDED_ISO10646_1_5X7

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" gotowy.")
#define MSG_SD_INSERTED                     _UxGT("Karta wlozona")
#define MSG_SD_REMOVED                      _UxGT("Karta usunieta")
#define MSG_LCD_ENDSTOPS                    _UxGT("Krancow.") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu glowne")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Wylacz silniki")
#define MSG_AUTO_HOME                       _UxGT("Pozycja zerowa")
#define MSG_AUTO_HOME_X                     _UxGT("Zeruj X")
#define MSG_AUTO_HOME_Y                     _UxGT("Zeruj Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Zeruj Z")
#define MSG_TMC_Z_CALIBRATION               _UxGT("Kalibruj Z")
#define MSG_LEVEL_BED                       _UxGT("Poziom. stolu")
#define MSG_LEVEL_BED_HOMING                _UxGT("Pozycja zerowa")
#define MSG_LEVEL_BED_WAITING               _UxGT("Kliknij by rozp.")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Nastepny punkt")
#define MSG_LEVEL_BED_DONE                  _UxGT("Wypoziomowano!")
#define MSG_USER_MENU                       _UxGT("Wlasne Polecenia")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ust. poz. zer.")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Poz. zerowa ust.")
#define MSG_SET_ORIGIN                      _UxGT("Ustaw punkt zero")
#define MSG_PREHEAT_1                       _UxGT("Rozgrzej PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" wsz.")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Rozgrzej stol PLA")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Ustaw. rozg. PLA")
#define MSG_PREHEAT_2                       _UxGT("Rozgrzej ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" wsz.")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Rozgrzej stol ABS")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Ustaw. rozg. ABS")
#define MSG_COOLDOWN                        _UxGT("Chlodzenie")
#define MSG_SWITCH_PS_ON                    _UxGT("Wlacz zasilacz")
#define MSG_SWITCH_PS_OFF                   _UxGT("Wylacz zasilacz")
#define MSG_EXTRUDE                         _UxGT("Ekstruzja")
#define MSG_RETRACT                         _UxGT("Wycofanie")
#define MSG_MOVE_AXIS                       _UxGT("Ruch osi")
#define MSG_BED_LEVELING                    _UxGT("Poziom. stolu")
#define MSG_MOVE_X                          _UxGT("Przesun w X")
#define MSG_MOVE_Y                          _UxGT("Przesun w Y")
#define MSG_MOVE_Z                          _UxGT("Przesun w Z")
#define MSG_MOVE_E                          _UxGT("Ekstruzja (os E)")
#define MSG_MOVE_01MM                       _UxGT("Przesun co .1mm")
#define MSG_MOVE_1MM                        _UxGT("Przesun co 1mm")
#define MSG_MOVE_10MM                       _UxGT("Przesun co 10mm")
#define MSG_SPEED                           _UxGT("Predkosc")
#define MSG_BED_Z                           _UxGT("Stol Z")
#define MSG_NOZZLE                          _UxGT("Dysza")
#define MSG_BED                             _UxGT("Stol")
#define MSG_FAN_SPEED                       _UxGT("Obroty wiatraka")
#define MSG_FLOW                            _UxGT("Przeplyw")
#define MSG_CONTROL                         _UxGT("Ustawienia")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Mnoznik")
#define MSG_AUTOTEMP                        _UxGT("Auto. temperatura")
#define MSG_ON                              _UxGT("Wl. ")
#define MSG_OFF                             _UxGT("Wyl.")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Select")
#define MSG_ACC                             _UxGT("Przyspieszenie")
#define MSG_JERK                            _UxGT("Zryw")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Zryw Va")
  #define MSG_VB_JERK                       _UxGT("Zryw Vb")
  #define MSG_VC_JERK                       _UxGT("Zryw Vc")
#else
  #define MSG_VA_JERK                       _UxGT("Zryw Vx")
  #define MSG_VB_JERK                       _UxGT("Zryw Vy")
  #define MSG_VC_JERK                       _UxGT("Zryw Vz")
#endif
#define MSG_VE_JERK                         _UxGT("Zryw Ve")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("Vskok min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Amax")
#define MSG_A_RETRACT                       _UxGT("A-wycofanie")
#define MSG_A_TRAVEL                        _UxGT("A-przesun.")
#define MSG_STEPS_PER_MM                    _UxGT("kroki/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("krokiA/mm")
  #define MSG_BSTEPS                        _UxGT("krokiB/mm")
  #define MSG_CSTEPS                        _UxGT("krokiC/mm")
#else
  #define MSG_ASTEPS                        _UxGT("krokiX/mm")
  #define MSG_BSTEPS                        _UxGT("krokiY/mm")
  #define MSG_CSTEPS                        _UxGT("krokiZ/mm")
#endif
#define MSG_ESTEPS                          _UxGT("krokiE/mm")
#define MSG_E1STEPS                         _UxGT("krokiE1/mm")
#define MSG_E2STEPS                         _UxGT("krokiE2/mm")
#define MSG_E3STEPS                         _UxGT("krokiE3/mm")
#define MSG_E4STEPS                         _UxGT("krokiE4/mm")
#define MSG_E5STEPS                         _UxGT("krokiE5/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Ruch")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E w mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Sr. fil.")
#define MSG_CONTRAST                        _UxGT("Kontrast LCD")
#define MSG_STORE_EEPROM                    _UxGT("Zapisz w pamieci")
#define MSG_LOAD_EEPROM                     _UxGT("Wczytaj z pamieci")
#define MSG_RESTORE_FAILSAFE                _UxGT("Ustaw. fabryczne")
#define MSG_REFRESH                         _UxGT("Odswiez")
#define MSG_WATCH                           _UxGT("Ekran glowny")
#define MSG_PREPARE                         _UxGT("Przygotuj")
#define MSG_TUNE                            _UxGT("Strojenie")
#define MSG_PAUSE_PRINT                     _UxGT("Pauza")
#define MSG_RESUME_PRINT                    _UxGT("Wznowienie")
#define MSG_STOP_PRINT                      _UxGT("Stop")
#define MSG_CARD_MENU                       _UxGT("Karta SD")
#define MSG_NO_CARD                         _UxGT("Brak karty")
#define MSG_DWELL                           _UxGT("Uspij...")
#define MSG_USERWAIT                        _UxGT("Oczekiwanie...")
#define MSG_PRINT_ABORTED                   _UxGT("Druk przerwany")
#define MSG_NO_MOVE                         _UxGT("Brak ruchu")
#define MSG_KILLED                          _UxGT("Ubity. ")
#define MSG_STOPPED                         _UxGT("Zatrzymany. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Wycofaj mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Z Wycof. mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Wycofaj  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Skok Z mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Cof. wycof. mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Z Cof. wyc. mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Cof. wycof.  V")
#define MSG_AUTORETRACT                     _UxGT("Auto. wycofanie")
#define MSG_FILAMENTCHANGE                  _UxGT("Zmien filament")
#define MSG_INIT_SDCARD                     _UxGT("Inicjal. karty SD")
#define MSG_CNG_SDCARD                      _UxGT("Zmiana karty SD")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z za stolem")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Offset Z")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Blad krancowki")
#define MSG_HEATING_FAILED_LCD              _UxGT("Rozgrz. nieudane")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Blad temperatury")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Zanik temp.")
#define MSG_ERR_MAXTEMP                     _UxGT("Err max temp")
#define MSG_ERR_MINTEMP                     _UxGT("Err min temp")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err max temp stolu")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err min temp stolu")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("Drukarka zatrzym.")
#define MSG_PLEASE_RESET                    _UxGT("Prosze zresetowac")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("g") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Rozgrzewanie...")
#define MSG_BED_HEATING                     _UxGT("Rozgrzewanie stolu...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Kalibrowanie Delty")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibruj X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibruj Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibruj Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibruj srodek")

#define MSG_INFO_MENU                       _UxGT("O drukarce")
#define MSG_INFO_PRINTER_MENU               _UxGT("Info drukarki")
#define MSG_INFO_STATS_MENU                 _UxGT("Statystyki")
#define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistory")
#define MSG_INFO_EXTRUDERS                  _UxGT("Ekstrudery")
#define MSG_INFO_BAUDRATE                   _UxGT("Predkosc USB")
#define MSG_INFO_PROTOCOL                   _UxGT("Protokol")
#define MSG_CASE_LIGHT                      _UxGT("Oswietlenie")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukonczono")
  #define MSG_INFO_PRINT_TIME               _UxGT("Czas druku")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najdl. druk")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Uzyty fil.")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukonczono")
  #define MSG_INFO_PRINT_TIME               _UxGT("Razem")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najdl. druk")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Uzyty fil.")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Zasilacz")

#define MSG_DRIVE_STRENGTH                  _UxGT("Sila silnika")
#define MSG_DAC_PERCENT                     _UxGT("Sila %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Zapisz DAC EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("ZMIEN FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("ZMIEN OPCJE:")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Wznow drukowanie")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Czekam na ")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("zmiane filamentu")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("wyjecie filamentu")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Wloz filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i nacisnij przycisk")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("aby kontynuowac...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("wlozenie filamentu")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("wznowienie druku")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Prosze czekac...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wysuwanie...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Wloz i nacisnij prz.")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Ladowanie...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wznowienie...")
#endif // LCD_HEIGHT < 4

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Czekam na ")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("zmiane filamentu")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("wyjecie filamentu")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Wloz filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i nacisnij przycisk")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("aby kontynuowac...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("wlozenie filamentu")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("wznowienie druku")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Prosze czekac...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wysuwanie...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Wloz i nacisnij prz.")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Ladowanie...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wznowienie...")
#endif // LCD_HEIGHT < 4

#endif  // LANGUAGE_PL_HD44780_H
