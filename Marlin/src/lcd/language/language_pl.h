/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
#pragma once

/**
 * Polish for DOGM display - includes accented characters
 */

#define DISPLAY_CHARSET_ISO10646_PL

namespace Language_pl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Polish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" gotowy.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Karta włożona");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Karta usunięta");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Kranców."); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu główne");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autostart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Wyłącz silniki");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Pozycja zerowa");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Zeruj X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Zeruj Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Zeruj Z");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Poziom. stołu");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Pozycja zerowa");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Kliknij by rozp.");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Następny punkt");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Wypoziomowano!");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Własne Polecenia");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ust. poz. zer.");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Poz. zerowa ust.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Ustaw punkt zero");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Rozgrzej ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H0                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_H1                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_H2                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_H3                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_H4                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_H5                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E0                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E1                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E2                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E3                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E4                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E5                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" wsz.");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Rozgrzej stół ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Ustaw. rozg. ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Rozgrzej ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H0                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_H1                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_H2                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_H3                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_H4                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_H5                    = _UxGT("Rozgrzej ") PREHEAT_2_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E0                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E1                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E2                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E3                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E4                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E5                = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" Dysza ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Rozgrzej ") PREHEAT_2_LABEL _UxGT(" wsz.");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Rozgrzej stół ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Ustaw. rozg. ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Chłodzenie");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Włącz zasilacz");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Wyłącz zasilacz");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Ekstruzja");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Wycofanie");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Ruch osi");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Poziom. stołu");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Przesuń w X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Przesuń w Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Przesuń w Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Ekstruzja (os E)");
  PROGMEM Language_Str MSG_MOVE_E0                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E0;
  PROGMEM Language_Str MSG_MOVE_E1                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E1;
  PROGMEM Language_Str MSG_MOVE_E2                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E2;
  PROGMEM Language_Str MSG_MOVE_E3                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E3;
  PROGMEM Language_Str MSG_MOVE_E4                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E4;
  PROGMEM Language_Str MSG_MOVE_E5                         = _UxGT("Ekstruzja (os E) ") LCD_STR_E5;
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Przesuń co %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Przesuń co .1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Przesuń co 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Przesuń co 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Predkość");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Stół Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Dysza");
  PROGMEM Language_Str MSG_NOZZLE_0                        = _UxGT("Dysza ") LCD_STR_N0;
  PROGMEM Language_Str MSG_NOZZLE_1                        = _UxGT("Dysza ") LCD_STR_N1;
  PROGMEM Language_Str MSG_NOZZLE_2                        = _UxGT("Dysza ") LCD_STR_N2;
  PROGMEM Language_Str MSG_NOZZLE_3                        = _UxGT("Dysza ") LCD_STR_N3;
  PROGMEM Language_Str MSG_NOZZLE_4                        = _UxGT("Dysza ") LCD_STR_N4;
  PROGMEM Language_Str MSG_NOZZLE_5                        = _UxGT("Dysza ") LCD_STR_N5;
  PROGMEM Language_Str MSG_BED                             = _UxGT("Stół");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Obroty wiatraka");
  PROGMEM Language_Str MSG_FAN_SPEED_1                     = _UxGT("Obroty wiatraka 1");
  PROGMEM Language_Str MSG_FAN_SPEED_2                     = _UxGT("Obroty wiatraka 2");
  PROGMEM Language_Str MSG_FAN_SPEED_3                     = _UxGT("Obroty wiatraka 3");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Przepływ");
  PROGMEM Language_Str MSG_FLOW_0                          = _UxGT("Przepływ ") LCD_STR_N0;
  PROGMEM Language_Str MSG_FLOW_1                          = _UxGT("Przepływ ") LCD_STR_N1;
  PROGMEM Language_Str MSG_FLOW_2                          = _UxGT("Przepływ ") LCD_STR_N2;
  PROGMEM Language_Str MSG_FLOW_3                          = _UxGT("Przepływ ") LCD_STR_N3;
  PROGMEM Language_Str MSG_FLOW_4                          = _UxGT("Przepływ ") LCD_STR_N4;
  PROGMEM Language_Str MSG_FLOW_5                          = _UxGT("Przepływ ") LCD_STR_N5;
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Ustawienia");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Mnożnik");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Auto. temperatura");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Wł.");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Wył.");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Przyśpieszenie");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Zryw");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Zryw V") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Zryw V") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Zryw V") LCD_STR_C;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Zryw Ve");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vskok min");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-wycofanie");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-przesuń.");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("kroki/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("kroki") LCD_MSG_A _UxGT("/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("kroki") LCD_MSG_B _UxGT("/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("kroki") LCD_MSG_C _UxGT("/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("krokiE/mm");
  PROGMEM Language_Str MSG_E0_STEPS                        = _UxGT("kroki ") LCD_STR_E0 _UxGT("/mm");
  PROGMEM Language_Str MSG_E1_STEPS                        = _UxGT("kroki ") LCD_STR_E1 _UxGT("/mm");
  PROGMEM Language_Str MSG_E2_STEPS                        = _UxGT("kroki ") LCD_STR_E2 _UxGT("/mm");
  PROGMEM Language_Str MSG_E3_STEPS                        = _UxGT("kroki ") LCD_STR_E3 _UxGT("/mm");
  PROGMEM Language_Str MSG_E4_STEPS                        = _UxGT("kroki ") LCD_STR_E4 _UxGT("/mm");
  PROGMEM Language_Str MSG_E5_STEPS                        = _UxGT("kroki ") LCD_STR_E5 _UxGT("/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Ruch");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E w mm3");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Śr. fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E0                = _UxGT("Śr. fil. ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E1                = _UxGT("Śr. fil. ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E2                = _UxGT("Śr. fil. ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E3                = _UxGT("Śr. fil. ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E4                = _UxGT("Śr. fil. ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E5                = _UxGT("Śr. fil. ") LCD_STR_E5;
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Kontrast LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Zapisz w pamięci");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Wczytaj z pamięci");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Ustaw. fabryczne");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Odswież");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Ekran główny");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Przygotuj");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Strojenie");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pauza");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Wznowienie");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Stop");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Karta SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Brak karty");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Uśpij...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Oczekiwanie...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Druk przerwany");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Brak ruchu");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("Ubity. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("Zatrzymany. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Wycofaj mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Z Wycof. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Wycofaj  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Skok Z mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Cof. wycof. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Z Cof. wyc. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Cof. wycof.  V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto. wycofanie");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Zmień filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E0               = _UxGT("Zmień filament ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E1               = _UxGT("Zmień filament ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E2               = _UxGT("Zmień filament ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E3               = _UxGT("Zmień filament ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E4               = _UxGT("Zmień filament ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E5               = _UxGT("Zmień filament ") LCD_STR_E5;
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Inicjal. karty SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Zmiana karty SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z za stołem");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Self-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reset BLTouch");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s first");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Offset Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Błąd krańcówki");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Rozgrz. nieudane");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Błąd temperatury");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Zanik temp.");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err max temp");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err min temp");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Err max temp stołu");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Err min temp stołu");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Home XY first");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("Drukarka zatrzym.");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Proszę zresetować");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("g"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Rozgrzewanie...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Rozgrzewanie stołu...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Kalibrowanie Delty");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibruj X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibruj Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibruj Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibruj środek");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("O drukarce");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info drukarki");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statystyki");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Board Info");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Thermistory");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Ekstrudery");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Predkość USB");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokół");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Oświetlenie");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Niepoprawna drukarka");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Wydrukowano");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Ukończono");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Czas druku");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najdł. druk");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Użyty fil.");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Wydrukowano");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Ukończono");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Razem");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najdł. druk");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Użyty fil.");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Temp");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Temp");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Zasilacz");

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Siła silnika");
  PROGMEM Language_Str MSG_DAC_PERCENT                     = _UxGT("Siła %");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Zapisz DAC EEPROM");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("ZMIEŃ FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("ZMIEŃ OPCJE:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Wznów drukowanie");

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_2_LINE("Czekam na ", "zmianę filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Czekam na", "wyjęcie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Włóz filament", "i naciśnij przycisk", "aby kontynuować..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Czekam na", "włożenie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Czekam na", "wznowienie druku"));
  #else
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Proszę czekać..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Wysuwanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Włóż i naciśnij prz."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Ładowanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Wznowienie..."));
  #endif
}
