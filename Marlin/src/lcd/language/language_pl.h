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
 * Polish
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

#define DISPLAY_CHARSET_ISO10646_PL

namespace LanguageNarrow_pl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Polski");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" gotowy.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Karta włożona");                            // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Karta usunięta");                           // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Krańców.");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu główne");                              // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Wyłącz silniki");                           // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Pozycja zerowa");                           // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Zeruj X");                                  // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Zeruj Y");                                  // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Zeruj Z");                                  // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Pozycja zerowa");                           // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Kliknij by rozp.");                         // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Następny punkt");                           // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Wypoziomowano!");                           // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ust. poz. zer.");                           // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Poz. zerowa ust.");                         // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Rozgrzej ") PREHEAT_1_LABEL;                // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Rozgrzej ") PREHEAT_1_LABEL " ~";           // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" Dysza ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" wsz."); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" stół"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Rozgrzej ") PREHEAT_1_LABEL _UxGT(" ustaw."); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Rozgrzej $");                               // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Rozgrzej $ ~");                             // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Rozgrzej $ Dysza");                         // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Rozgrzej $ Dysza ~");                       // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Rozgrzej $ wsz.");                          // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Rozgrzej $ stół");                          // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Rozgrzej $ ustaw.");                        // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Chłodzenie");                               // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Włącz zasilacz");                           // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Wyłącz zasilacz");                          // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Ekstruzja");                                // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Wycofanie");                                // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Ruch osi");                                 // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sonda/poziomowanie");                       // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Poziomowanie stołu");                       // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Wypoziomuj stół");                          // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Przesuń w X");                              // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Przesuń w Y");                              // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Przesuń w Z");                              // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Przesuń w @");                              // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Ekstruzja (os E)");                         // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Ekstruzja (os E) *");                       // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Przesuń co $mm");                           // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Przesuń co $in");                           // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Przesuń co $") LCD_STR_DEGREE;              // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Predkość");                                 // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Stół Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Dysza");                                    // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Dysza ~");                                  // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Stół");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Obroty wentylatora");                       // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Obroty wentylatora ~");                     // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Przepływ");                                 // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Przepływ ~");                               // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Ustawienia");                               // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Auto. temperatura");                        // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Wybierz *");                                // Select *
  LSTR MSG_ACC                            = _UxGT("Przyspieszenie");                           // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vskok min");                                // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Przyspieszenie (A)");                       // Acceleration
  LSTR MSG_A_RETRACT                      = _UxGT("A-wycofanie");                              // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-przesuń.");                               // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("kroki/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" kroki/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" kroki/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" kroki/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ kroki/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E kroki/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* kroki/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Ruch");                                     // Motion
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E w mm") SUPERSCRIPT_THREE;                 // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Śr. fil.");                                 // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Śr. fil. *");                               // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Kontrast LCD");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Zapisz w pamięci");                         // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Wczytaj z pamięci");                        // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Ustaw. fabryczne");                         // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Odswież");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Ekran główny");                             // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Przygotuj");                                // Prepare
  LSTR MSG_TUNE                           = _UxGT("Strojenie");                                // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Wstrzymaj druk");                           // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Wznowienie");                               // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Stop");                                     // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Karta SD");                                 // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Brak karty");                               // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Uśpij...");                                 // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Oczekiwanie...");                           // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Druk przerwany");                           // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Brak ruchu");                               // No Move.
  LSTR MSG_KILLED                         = _UxGT("Ubity. ");                                  // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("Zatrzymany. ");                             // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Wycofaj mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Z Wycof. mm");                              // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Wycofaj  V");                               // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Skok Z mm");                                // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Cof. wycof. mm");                           // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Z Cof. wyc. mm");                           // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Cof. wycof.  V");                           // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto. wycofanie");                          // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Zmień filament");                           // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Zmień filament *");                         // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Inicjal. karty SD");                        // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Inicjal. karty SD");                        // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Inicjal. dysk flash");                      // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Zmiana karty SD");                          // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autostart");                                // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z za stołem");                        // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset BLTouch");                            // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Offset X");                                 // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Offset Y");                                 // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Offset Z");                                 // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Offset @");                                 // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Krok Z offsetu");                           // Babystep Probe Z
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Błąd krańcówki");                           // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Rozgrz. nieudane");                         // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Błąd temperatury");                         // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("ZANIK TEMPERATURY");                        // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Błąd: MAXTEMP");                            // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Błąd: MINTEMP");                            // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("Drukarka zatrzym.");                        // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Proszę zresetować");                        // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Rozgrzewanie...");                          // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Rozgrzewanie stołu...");                    // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Kalibrowanie Delty");                       // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibruj X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibruj Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibruj Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibruj środek");                          // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("O drukarce");                               // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info drukarki");                            // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Statystyki");                               // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info płyty");                               // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistory");                               // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Ekstrudery");                               // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Predkość USB");                             // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokół");                                 // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Oświetlenie obudowy");                      // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Wydrukowano");                              // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Ukończono");                                // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Razem");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Najdł. druk");                              // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Użyty fil.");                               // Extruded
  LSTR MSG_INFO_PSU                       = _UxGT("Zasilacz");                                 // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Siła silnika");                             // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Zapisz DAC EEPROM");                        // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Kontynuuj");                                // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Proszę czekać..."));             // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Włóż i kliknij"));               // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Wysuwanie..."));                 // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Wsuwanie..."));                  // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Wznawianie..."));                // |Resuming...
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Mnożnik");         // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Wł.");                                      // On
  LSTR MSG_LCD_OFF                        = _UxGT("Wył.");                                     // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Niepoprawna drukarka");                     // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu Debugowania");                         // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Testowy pasek postępu");                    // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Ruch...");                                  // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Swobodne XY");                              // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Najpierw Home %s");                         // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Nagrzewanie..."));               // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_1_LINE("Kliknij by nagrzać"));           // |Click to heat
  LSTR MSG_YES                            = _UxGT("TAK");                                      // YES
  LSTR MSG_NO                             = _UxGT("NIE");                                      // NO
  LSTR MSG_BACK                           = _UxGT("Wstecz");                                   // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Przerywanie...");                           // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Bład odczytu karty");                       // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("Urządzenie USB usunięte");                  // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("Błąd uruchomienia USB");                    // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Progr. Krańcówki");                         // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Ustawienie zaawansowane");                  // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Konfiguracja");                             // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Autowyrównanie Z");                         // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Wys. zanikania");                           // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Rozgrzej własne ust.");                     // Preheat Custom
  LSTR MSG_LASER_MENU                     = _UxGT("Sterowanie Lasera");                        // Laser Control
  LSTR MSG_LASER_POWER                    = _UxGT("Zasilanie Lasera");                         // Laser Power
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Sterowanie wrzeciona");                     // Spindle Control
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Zasilanie wrzeciona");                      // Spindle Pwr
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Rewers wrzeciona");                         // Spindle Reverse
  LSTR MSG_BED_TRAMMING                   = _UxGT("Narożniki poziomowania");                   // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Nastepny narożnik");                        // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Edytor siatki");                            // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Edycja siatki");                            // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Edycja siatki zatrzymana");                 // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Punkt pomiarowy");                          // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Indeks X");                                 // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Indeks Y");                                 // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Wartość Z");                                // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Własne Polecenia");                         // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 Test sondy");                           // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 Punky");                                // M48 Point
  LSTR MSG_M48_DEVIATION                  = _UxGT("Odchylenie");                               // Deviation
  LSTR MSG_IDEX_MENU                      = _UxGT("Tryb IDEX");                                // IDEX Mode
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Przesunięcie narzędzia");                   // Tool Offsets
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Parkowanie");                          // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplikowanie");                             // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Kopia lustrzana");                          // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Pełne sterowanie");                         // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2ga dysza Z");                              // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2ga dysza @");                              // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Wykonywanie G29");                          // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("Narzędzia UBL");                            // UBL Tools
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Punkt pochylenia");                         // Tilting Point
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Ręczne Budowanie Siatki");                  // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Umieść podkładkę i zmierz");                // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Zmierz");                                   // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Usuń & Zmierz Stół");                       // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Przesuwanie do następnego");                // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Temperatura stołu");                        // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Temperatura stołu");                        // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temperatura dyszy");                        // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temperatura dyszy");                        // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Edycja własnej siatki");                    // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Dostrajanie siatki");                       // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Koniec edycji siati");                      // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Buduj własna siatkę");                      // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Buduj siatkę");                             // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Buduj siatkę ($)");                         // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Sprawdzenie siatki ($)");                   // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Buduj siatkę na zimno");                    // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Dostrojenie wysokości siatki");             // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Wartość wysokości");                        // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Sprawdzenie siatki");                       // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Sprawdzenie własnej siatki");               // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Nagrzewanie stołu");                    // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Nagrzewanie dyszy");                    // G26 Heating Nozzle
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Napełnianie ręczne...");                    // Manual priming...
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Napełnij kreśloną długością");              // Fixed Length Prime
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Napełnianie zakończone");                   // Done Priming
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Przewane");                             // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("Opuszczanie G26");                          // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Kontynuuj tworzenie siatki");               // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Poziomowanie siatką");                      // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Poziomowaie 3-punktowe");                   // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Poziomowaie według siatki");                // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Poziomuj siatkę");                          // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Punkty boczne");                            // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Rodzaj mapy");                              // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Wyslij mapę siatki");                       // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Wyslij do Hosta");                          // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Wyslij do CSV");                            // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Kopia poza drukarką");                      // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Wyslij info UBL");                          // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Stopień wypełnienia");                      // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Ręczne wypełnienie");                       // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Inteligentne wypełnienie");                 // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Wypełnienie siatki");                       // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Unieważnij wszystko");                      // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Unieważnij najbliższy");                    // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Dostrajaj wszystko");                       // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Dostrajaj najbliższy");                     // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Przechowywanie siatki");                    // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Slot Pamięci");                             // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Załaduj siatkę stołu");                     // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Zapisz siatkę stołu");                      // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Siatka %i załadowana");                     // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Siatka %i zapisana");                       // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Brak magazynu");                            // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Błąd: Zapis UBL");                          // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Bład: Odczyt UBL");                         // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Przesunięcie Z: ");                         // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Przesunięcie Z zatrzymane");                // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL Krok po kroku");                        // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1. Tworzenie zimnej siatki");               // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2. Inteligentne wypełnienie");              // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3. Sprawdzenie siatki");                    // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4. Dostrojenie wszystkiego");               // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5. Sprawdzenie siatki");                    // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6. Dostrojenie wszystkiego");               // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7. Zapis siatki stołu");                    // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("Sterowanie LED");                           // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Światła");                                  // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Światło #{");                               // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Ustawienia świateł");                       // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Czerwony");                                 // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Pomarańczowy");                             // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Zółty");                                    // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Zielony");                                  // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Niebieski");                                // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indygo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Fioletowy");                                // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Biały");                                    // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Domyślny");                                 // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Własne światła");                           // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Czerwony");                                 // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Zielony");                                  // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Niebieski");                                // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Biały");                                    // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Jasność");                                  // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Dysza za zimna");                           // Hotend too cold
  LSTR MSG_CHAMBER                        = _UxGT("Obudowa");                                  // Enclosure
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Obroty dodatkowego wentylatora");           // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Obroty dodatkowego wentylatora ~");         // Extra Fan Speed ~
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID Autostrojenie");                        // PID Autotune
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID Autostrojenie *");                      // Autotune * PID
  LSTR MSG_JERK                           = _UxGT("Zryw");                                     // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("Zryw V") STR_A;                             // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("Zryw V") STR_B;                             // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("Zryw V") STR_C;                             // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("Zryw V@");                                  // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Zryw Ve");                                  // Max E Jerk
  LSTR MSG_MAX_SPEED                      = _UxGT("Prędkość (V)");                             // Max Speed (mm/s)
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Wysuń mm");                                 // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Wsuń mm");                                  // Load mm
  LSTR MSG_INIT_EEPROM                    = _UxGT("Initializuj EEPROM");                       // Initialize EEPROM
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Uaktualnij kartę");                         // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Resetuj drukarkę");                         // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Start wydruku");                            // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Następny");                                 // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Inic.");                                    // Init
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Drukuj");                                   // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Resetuj");                                  // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Przerwij");                                 // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("Gotowe");                                   // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Wstecz");                                   // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Kontynuuj");                                // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Drukowanie obiektu");                       // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Anunuj obiekt");                            // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Anunuj obiekt {");                          // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Odzyskiwanie po awarii");                   // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Druk wstrzymany");                          // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Drukowanie...");                            // Printing...
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Długość zmiany");                           // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Długość oczyszczania");                     // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Zmiana narzędzia");                         // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Podniesienie Z");                           // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Prędkość napełniania");                     // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Prędkość wycofania");                       // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Dysza w oczekiwaniu");                      // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Wsuń Filament");                            // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Wsuń Filament *");                          // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Wysuń Filament");                           // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Wysuń Filament *");                         // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Wysuń wszystkie");                          // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Zwolnienie karty");                         // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Współczynik skrzywienia");                  // Skew Factor
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch Self-Test");                        // Self-Test
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("UWAGA: Złe ustawienia mogą uszkodzić drukarkę. Kontynuować?"); // DANGER: Bad settings can cause damage! Proceed anyway?
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Łącznie");                                  // Total
  LSTR MSG_COOLING                        = _UxGT("Chłodzenie...");                            // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Chłodzenie stołu...");                      // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Rozgrzewanie komory...");                   // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Chłodzenie komory...");                     // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Ustawienia delty");                         // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto kalibrowanie");                        // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Ukośne ramię");                             // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Wysokość");                                 // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Promień");                                  // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("Poziomowanie 3-punktowe");                  // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Poziomowanie liniowe");                     // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Poziomowanie biliniowe");                   // Bilinear Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Poziomowanie siatką");                      // Mesh Leveling
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Zegar pracy: OFF");                         // Runaway Watch: OFF
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Zegar pracy: ON");                          // Runaway Watch: ON
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Jasność oświetlenia");                      // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Siła %");                                 // @ Driver %
  LSTR MSG_ERROR_TMC                      = _UxGT("TMC BŁĄD POŁĄCZENIA");                      // TMC CONNECTION ERROR
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("ZMIEŃ FILAMENT");                           // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("WYDRUK WSTRZYMANY");                        // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("WSUŃ FILAMENT");                            // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("WYSUŃ FILAMENT");                           // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("OPCJE WZNOWIENIA:");                        // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Oczyść więcej");                            // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Dysza: ");                                // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Czujnik filamentu");                        // Runout Sensor
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Dystans do czujnika mm");                   // Runout Dist mm
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Zerowanie nieudane");                       // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Sondowanie nieudane");                      // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("WYBIERZ FILAMENT");                         // CHOOSE FILAMENT
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Uaktualnij firmware MMU!");                 // Update MMU Firmware!
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU wymaga uwagi.");                        // MMU Needs Attention.
  LSTR MSG_MMU2_RESUME                    = _UxGT("Wznów wydruk");                             // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Wznawianie...");                            // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Wsuń filament");                            // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Wsuń wszystko");                            // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Wsuń do dyszy");                            // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Wysuń filament");                           // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Wysuń filament ~");                         // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Wysuń filament");                           // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Wsuwanie fil. %i...");                      // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Wysuwanie fil. ...");                       // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Wysuwanie fil....");                        // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("Wszystko");                                 // All
  LSTR MSG_MMU2_RESET                     = _UxGT("Resetuj MMU");                              // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Resetowanie MMU...");                       // Resetting...
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Usuń, kliknij");                            // Eject Recover
  LSTR MSG_MIX                            = _UxGT("Miks");                                     // Mix
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Komponent {");                              // Component {
  LSTR MSG_MIXER                          = _UxGT("Mikser");                                   // Mixer
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Pełny gradient");                           // Full Gradient
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Przełacz miks");                            // Toggle Mix
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Odwrotny gradient");                        // Reverse Gradient
  LSTR MSG_GAMES                          = _UxGT("Gry");                                      // Games
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_1_LINE("Kliknij by kontynuować"));       // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Parkowanie..."));                // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Oczyszczanie..."));              // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_1_LINE("Kliknij by zakończyć"));         // |Click to finish
  LSTR MSG_TMC_DRIVERS                    = _UxGT("Sterowniki TMC");                           // TMC Drivers
  LSTR MSG_TMC_CURRENT                    = _UxGT("Prąd sterownika");                          // Driver Current
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Zerowanie bezczujnikowe");                  // Sensorless Homing
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Korekcja");                                 // Correction
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Wygładzanie");                              // Smoothing
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Błąd inicializacji karty");                 // (MEDIA_TYPE_EN) Init Fail
  LSTR MSG_DECREASING_ACCURACY            = _UxGT("Spadek dokładności!");                      // Accuracy Decreasing!
  LSTR MSG_ACCURACY_ACHIEVED              = _UxGT("Osiągnięto dokładność");                    // Accuracy Achieved
  LSTR MSG_SELECT_ORIGIN                  = _UxGT("Wybierz punkt zero");                       // Select Origin
  LSTR MSG_LAST_VALUE_SP                  = _UxGT("Poprzednia wartość ");                      // Last value
  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Częstotliwość");                            // Frequency
  LSTR MSG_LED_CHANNEL_N                  = _UxGT("Kanał {");                                  // Channel {
  LSTR MSG_NEO2_BRIGHTNESS                = _UxGT("Jasność");                                  // Brightness
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Wentylator kontrolera");                    // Controller Fan
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("Strojenie PID zakończone");                 // PID tuning done
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Częstotliwość max");                        // XY Freq Limit
  LSTR MSG_CURRENT                        = _UxGT("Natężenie");                                // Current
  LSTR MSG_VOLTAGE                        = _UxGT("Napięcie");                                 // Voltage
  LSTR MSG_POWER                          = _UxGT("Moc");                                      // Power
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignoruj");                                  // Ignore
  LSTR MSG_BUTTON_SKIP                    = _UxGT("Pomiń");                                    // Skip
  LSTR MSG_PAUSING                        = _UxGT("Wstrzymywanie...");                         // Pausing...
  LSTR MSG_PRINT_DONE                     = _UxGT("Druk zakończony");                          // Print Done
  LSTR MSG_MOVE_NOZZLE_TO_BED             = _UxGT("Przesuń dyszę do stołu");                   // Move Nozzle to Bed
  LSTR MSG_EDIT_PASSWORD                  = _UxGT("Zmień hasło");                              // Edit Password
  LSTR MSG_LOGIN_REQUIRED                 = _UxGT("Wymagane zalogowanie");                     // Login Required
  LSTR MSG_PASSWORD_SETTINGS              = _UxGT("Ustawienia hasła");                         // Password Settings
  LSTR MSG_ENTER_DIGIT                    = _UxGT("Wprowadź cyfrę");                           // Enter Digit
  LSTR MSG_CHANGE_PASSWORD                = _UxGT("Ustaw/zmień hasło");                        // Set/Edit Password
  LSTR MSG_REMOVE_PASSWORD                = _UxGT("Usuń hasło");                               // Remove Password
  LSTR MSG_PASSWORD_SET                   = _UxGT("Hasło to ");                                // Password is
  LSTR MSG_START_OVER                     = _UxGT("Od nowa");                                  // Start Over
  LSTR MSG_REMINDER_SAVE_SETTINGS         = _UxGT("Pamiętaj by zapisać!");                     // Remember to Save!
  LSTR MSG_PASSWORD_REMOVED               = _UxGT("Hasło usunięte");                           // Password Removed
  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Wypoziomuj oś X");                          // Level X Axis
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Autokalibracja");                           // Auto Calibrate
  LSTR MSG_SOUND                          = _UxGT("Dźwięk");                                   // Sound
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Kalibracja zakończona");                    // Calibration Completed
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Kalibracja nie powiodła się");              // Calibration Failed
  LSTR MSG_SHORT_HOUR                     = _UxGT("g");                                        // h
}

namespace LanguageWide_pl {
  using namespace LanguageNarrow_pl;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Wydrukowano");                              // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Ukończono");                                // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Czas druku");                               // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Najdł. druk");                              // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Użyty fil.");                               // Extruded Total
  #endif
}

namespace LanguageTall_pl {
  using namespace LanguageWide_pl;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Czekam na", "zmianę filamentu", "by wystartować")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Włóż filament", "i naciśnij przycisk", "by kontynuować")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Czekam na", "wyjęcie filamentu")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Czekam na", "włożenie filamentu")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Czekam na", "wznowienie wydruku...")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Nagrzewanie dyszy", "Proszę czekać...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Naciśnij przycisk", "by nagrzać dyszę")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Nacisnik przycisk", "by wznowić drukowanie")); // |Press Button|to resume print
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Czekam na", "oczyszczenie filamentu")); // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Kliknij by zakończyć", "oczyszczanie filamentu")); // |Click to finish|filament purge
  #endif
}

namespace Language_pl {
  using namespace LanguageTall_pl;
}
