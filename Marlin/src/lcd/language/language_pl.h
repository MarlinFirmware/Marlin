/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Polish - includes accented characters
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_PL

namespace Language_pl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Polish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" gotowy.");
  PROGMEM Language_Str MSG_YES                             = _UxGT("TAK");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NIE");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Wstecz");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Przerywanie...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Karta włożona");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Karta usunięta");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Oczekiwanie na kartę");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Bład odczytu karty");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("Urządzenie USB usunięte");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Błąd uruchomienia USB");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Krańców."); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Progr. Krańcówki");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu główne");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Ustawienie zaawansowane");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfiguracja");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autostart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Wyłącz silniki");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menu Debugowania");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Testowy pasek postępu");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Pozycja zerowa");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Zeruj X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Zeruj Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Zeruj Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Autowyrównanie Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Pozycja zerowa");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Kliknij by rozp.");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Następny punkt");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Wypoziomowano!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Wys. zanikania");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ust. poz. zer.");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Poz. zerowa ust.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Ustaw punkt zero");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Rozgrzej ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" wsz.");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" stół");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" ustaw.");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Rozgrzej $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Rozgrzej $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Rozgrzej $ Dysza");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Rozgrzej $ Dysza ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Rozgrzej $ wsz.");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Rozgrzej $ stół");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Rozgrzej $ ustaw.");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Rozgrzej własne ust.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Chłodzenie");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Sterowanie Lasera");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Wyłącz Laser");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Włącz Laser");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Zasilanie Lasera");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Sterowanie wrzeciona");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Wyłącz wrzeciono");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Włącz wrzeciono");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Zasilanie wrzeciona");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Rewers wrzeciona");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Włącz zasilacz");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Wyłącz zasilacz");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Ekstruzja");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Wycofanie");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Ruch osi");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Poziomowanie stołu");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Wypoziomuj stół");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Narożniki poziomowania");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Nastepny narożnik");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Edytor siatki");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Edycja siatki");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Edycja siatki zatrzymana");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Punkt pomiarowy");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Indeks X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Indeks Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Wartość Z");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Własne Polecenia");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Test sondy");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Punky");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Odchylenie");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Tryb IDEX");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Przesunięcie narzędzia");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Parkowanie");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplikowanie");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Kopia lustrzana");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Pełne sterowanie");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2ga dysza X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2ga dysza Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2ga dysza Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Wykonywanie G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Narzędzia UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Punkt pochylenia");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Ręczne Budowanie Siatki");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Umieść podkładkę i zmierz");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Zmierz");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Usuń & Zmierz Stół");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Przesuwanie do następnego");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Aktywacja UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Dezaktywacja UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Temperatura stołu");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Temperatura stołu");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Temperatura dyszy");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Temperatura dyszy");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Edycja siatki");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Edycja własnej siatki");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Dostrajanie siatki");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Koniec edycji siati");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Buduj własna siatkę");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Buduj siatkę");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Buduj siatkę ($)");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Sprawdzenie siatki ($)");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Buduj siatkę na zimno");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Dostrojenie wysokości siatki");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Wartość wysokości");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Sprawdzenie siatki");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Sprawdzenie własnej siatki");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Nagrzewanie stołu");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Nagrzewanie dyszy");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Napełnianie ręczne...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Napełnij kreśloną długością");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Napełnianie zakończone");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Przewane");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Opuszczanie G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Kontynuuj tworzenie siatki");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Poziomowanie siatką");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Poziomowaie 3-punktowe");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Poziomowaie według siatki");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Poziomuj siatkę");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Punkty boczne");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Rodzaj mapy");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Wyslij mapę siatki");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Wyslij do Hosta");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Wyslij do CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Kopia poza drukarką");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Wyslij info UBL");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Stopień wypełnienia");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Ręczne wypełnienie");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Inteligentne wypełnienie");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Wypełnienie siatki");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Unieważnij wszystko");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Unieważnij najbliższy");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Dostrajaj wszystko");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Dostrajaj najbliższy");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Przechowywanie siatki");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Slot Pamięci");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Załaduj siatkę stołu");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Zapisz siatkę stołu");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Siatka %i załadowana");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Siatka %i zapisana");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Brak magazynu");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Błąd: Zapis UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Bład: Odczyt UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Przesunięcie Z: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Przesunięcie Z zatrzymane");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL Krok po kroku");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. Tworzenie zimnej siatki");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. Inteligentne wypełnienie");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. Sprawdzenie siatki");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. Dostrojenie wszystkiego");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. Sprawdzenie siatki");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. Dostrojenie wszystkiego");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. Zapis siatki stołu");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Sterowanie LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Światła");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Ustawienia świateł");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Czerwony");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Pomarańczowy");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Zółty");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Zielony");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Niebieski");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indygo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Fioletowy");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Biały");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Domyślny");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Własne światła");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Czerwony");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Zielony");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Niebieski");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Biały");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Jasność");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Ruch...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Swobodne XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Przesuń w X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Przesuń w Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Przesuń w Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Ekstruzja (os E)");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Ekstruzja (os E) *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Dysza za zimna");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Przesuń co %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Przesuń co .1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Przesuń co 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Przesuń co 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Predkość");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Stół Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Dysza");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Dysza ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Stół");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Obudowa");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Obroty wiatraka");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Obroty wiatraka ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Obroty dodatkowego wiatraka");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Obroty dodatkowego wiatraka ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Przepływ");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Przepływ ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Ustawienia");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Mnożnik");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Auto. temperatura");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Wł.");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Wył.");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID Autostrojenie");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID Autostrojenie *");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Wybierz");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Wybierz *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Przyspieszenie");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Zryw");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Zryw V") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Zryw V") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Zryw V") LCD_STR_C;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Zryw Ve");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Junction Dev");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Prędkość (V)");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vmax ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vmax ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vmax ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vmax ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vmax *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vmin");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vskok min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Przyspieszenie (A)");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Amax ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Amax ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Amax ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Amax ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Amax *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-wycofanie");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-przesuń.");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("kroki/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("kroki") LCD_STR_A _UxGT("/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("kroki") LCD_STR_B _UxGT("/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("kroki") LCD_STR_C _UxGT("/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("krokiE/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("kroki */mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Ruch");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E w mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Śr. fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Śr. fil. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Wysuń mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Wsuń mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Advance K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Advance K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Kontrast LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Zapisz w pamięci");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Wczytaj z pamięci");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Ustaw. fabryczne");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Initializuj EEPROM");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Uaktualnij kartę");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Resetuj drukarkę");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Odswież");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Ekran główny");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Przygotuj");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Strojenie");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Start wydruku");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Następny");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Inic.");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Stop");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Drukuj");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Resetuj");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Przerwij");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Gotowe");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Wstrzymaj druk");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Wznowienie");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Stop");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Odzyskiwanie po awarii");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Karta SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Brak karty");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Uśpij...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Oczekiwanie...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Druk wstrzymany");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Drukowanie...");
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
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Długość zmiany");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Długość oczyszczania");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Zmiana narzędzia");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Podniesienie Z");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Prędkość napełniania");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Prędkość wycofania");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Dysza w oczekiwaniu");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Zmień filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Zmień filament *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Wsuń Filament");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Wsuń Filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Wysuń Filament");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Wysuń Filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Wysuń wszystkie");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Inicjal. karty SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Zmiana karty SD");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Zwolnienie karty");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z za stołem");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Współczynik skrzywienia");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Self-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reset BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Stow");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Deploy");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Mode-Store");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Set BLTouch to 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Set BLTouch to OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Report Drain");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("UWAGA: Złe ustawienia mogą uszkodzić drukarkę. Kontynuować?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Init TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Z Offset Test");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Save");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Deploy TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Deploy Z-Probe");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Stow Z-Probe");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Najpierw Home %s%s%s");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Offset Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Łącznie");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Błąd krańcówki");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Rozgrz. nieudane");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Rozgrz. stołu nieudane");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_CHAMBER      = _UxGT("Rozgrz. komory nieudane");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Błąd temperatury");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ZANIK TEMPERATURY");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("ZANIK TEMP. STOŁU");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("ZANIK TEMP.KOMORY");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Błąd: MAXTEMP");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Błąd: MINTEMP");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Błąd: MAXTEMP STÓŁ");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Błąd: MINTEMP STÓŁ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("Błąd: MAXTEMP KOMORA");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("Błąd: MINTEMP KOMORA");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("Drukarka zatrzym.");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Proszę zresetować");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("g"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Rozgrzewanie...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Chłodzenie...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Rozgrzewanie stołu...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Chłodzenie stołu...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Rozgrzewanie komory...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Chłodzenie komory...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Kalibrowanie Delty");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibruj X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibruj Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibruj Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibruj środek");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Ustawienia delty");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto kalibrowanie");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Ustaw wysokość delty");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Przesun. Z sondy");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Ukośne ramię");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Wysokość");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Promień");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("O drukarce");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info drukarki");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Poziomowanie 3-punktowe");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Poziomowanie liniowe");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Poziomowanie biliniowe");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Poziomowanie siatką");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statystyki");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info płyty");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistory");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Ekstrudery");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Predkość USB");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokół");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Zegar pracy: OFF");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Zegar pracy: ON");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Oświetlenie obudowy");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Jasność oświetlenia");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Niepoprawna drukarka");

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
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Siła %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Siła %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Siła %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Siła %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC BŁĄD POŁĄCZENIA");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Zapisz DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("ZMIEŃ FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("WYDRUK WSTRZYMANY");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("WSUŃ FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("WYSUŃ FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("OPCJE WZNOWIENIA:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Oczyść więcej");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Kontynuuj");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Dysza: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Czujnik filamentu");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Dystans do czujnika mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Zerowanie nieudane");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Sondowanie nieudane");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: za zimne");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("WYBIERZ FILAMENT");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Uaktualnij firmware MMU!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU wymaga uwagi.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Wznów wydruk");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Wznawianie...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Wsuń filament");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Wsuń wszystko");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Wsuń do dyszy");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Wysuń filament");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Wysuń filament ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Wysuń filament");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Wsuwanie fil. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Wysuwanie fil. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Wysuwanie fil....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Wszystko");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filament ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Resetuj MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Resetowanie MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Usuń, kliknij");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Miks");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Komponent =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Mikser");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradient");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Pełny gradient");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Przełacz miks");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Cycle Mix");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Gradient Mix");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Odwrotny gradient");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Nacisnik przycisk", "by wznowić drukowanie"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkowanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Czekam na", "zmianę filamentu", "by wystartować"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Włóż filament", "i naciśnij przycisk", "by kontynuować"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Naciśnij przycisk", "by nagrzać dyszę"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Nagrzewanie dyszy", "Proszę czekać..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Czekam na", "wyjęcie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Czekam na", "włożenie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Czekam na", "oczyszczenie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Kliknij by zakończyć", "oczyszczanie filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Czekam na", "wznowienie wydruku..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Kliknij by kontynuować"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkowanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Proszę czekać..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Włóż i kliknij"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Kliknij by nagrzać"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Nagrzewanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Wysuwanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Wsuwanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Oczyszczanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Kliknij by zakończyć"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Wznawianie..."));
  #endif
}
