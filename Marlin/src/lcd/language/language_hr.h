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
 * Croatian (Hrvatski)
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

#define DISPLAY_CHARSET_ISO10646_1

namespace LanguageNarrow_hr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Croatian");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" spreman.");             // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("SD kartica umetnuta");                      // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("SD kartica uklonjena");                     // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Main");                                     // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Ugasi steppere");                           // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Automatski homing");                        // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Home-aj X");                                // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Home-aj Y");                                // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Home-aj Z");                                // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Home-aj XYZ");                              // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klikni za početak");                        // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Sljedeća točka");                           // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Niveliranje gotovo!");                      // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Postavi home offsete");                     // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offsets postavljeni");                      // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Predgrij ") PREHEAT_1_LABEL;                // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " ~";           // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Sve");  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Bed");  // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" conf"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Predgrij $");                               // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Predgrij $ ~");                             // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Predgrij $ Dizna");                         // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Predgrij $ Dizna ~");                       // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Predgrij $ Sve");                           // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Predgrij $ Bed");                           // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Predgrij $ conf");                          // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Hlađenje");                                 // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Uključi napajanje");                        // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Isključi napajanje");                       // Switch Power Off
  LSTR MSG_MOVE_AXIS                      = _UxGT("Miči os");                                  // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Probe i Nivelacija");                       // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Niveliraj bed");                            // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Niveliraj bed");                            // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Miči X");                                   // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Miči Y");                                   // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Miči Z");                                   // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Miči @");                                   // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Miči E");                                   // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Miči *");                                   // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Miči $mm");                                 // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Miči $in");                                 // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Miči $") LCD_STR_DEGREE;                    // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Brzina");                                   // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Bed Z");                                    // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Dizna");                                    // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Dizna ~");                                  // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Bed");                                      // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Brzina ventilatora");                       // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Brzina ventilatora ~");                     // Fan Speed ~
  LSTR MSG_SELECT_E                       = _UxGT("Odaberi *");                                // Select *
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperature");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Gibanje");                                  // Motion
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");                                // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");                              // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Kontrast LCD-a");                           // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Pohrani u memoriju");                       // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Učitaj memoriju");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Učitaj Defaults");                          // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Osvježi");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info screen");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Pripremi");                                 // Prepare
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pauziraj print");                           // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Nastavi print");                            // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Zaustavi print");                           // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Printaj s SD kartice");                     // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Nema SD kartice");                          // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_USERWAIT                       = _UxGT("Čekaj korisnika...");                       // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Print otkazan");                            // Print Aborted
  LSTR MSG_STOPPED                        = _UxGT("ZAUSTAVLJEN. ");                            // STOPPED.
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Promijeni filament");                       // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Promijeni filament *");                     // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Init. SD karticu");                         // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Init. SD karticu");                         // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Init. USB pogon");                          // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Promijeni SD karticu");                     // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Auto pokretanje");                          // Run Autofiles
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Korak Z offset");                           // Babystep Probe Z
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Grijanje neuspješno");                      // Heating Failed
  LSTR MSG_HEATING                        = _UxGT("Grijanje...");                              // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Grijanje Bed-a...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibracija");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibriraj X");                             // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibriraj Y");                             // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibriraj Z");                             // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibriraj Središte");                      // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("O printeru");                               // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Podaci o printeru");                        // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Statistika printera");                      // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Podaci o elektronici");                     // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistori");                               // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extruderi");                                // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baud");                                     // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokol");                                 // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Osvjetljenje");                             // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Printovi");                                 // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Završeni");                                 // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Ukupno");                                   // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Najduži");                                  // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extrudirano");                              // Extruded
  LSTR MSG_INFO_PSU                       = _UxGT("Napajanje");                                // PSU
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Nastavi print");                            // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT("Pričekaj...");                              // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_2_LINE("Čekaj", "filament unload"));     // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_2_LINE("Pričekaj", "filament load"));    // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Nastavljam..."));                // |Resuming...
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Neispravan pisač");                         // INCORRECT PRINTER
}

namespace LanguageWide_hr {
  using namespace LanguageNarrow_hr;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Broj printova");                            // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Završeni");                                 // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Ukupno printanja");                         // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Najduži print");                            // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Extrudirano ukupno");                       // Extruded Total
  #endif
}

namespace LanguageTall_hr {
  using namespace LanguageWide_hr;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Čekaj početak", "filamenta", "promijeni")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Umetni filament", "i pritisni tipku", "za nastavak...")); // |Insert filament|and press button|to continue
  #endif
}

namespace Language_hr {
  using namespace LanguageTall_hr;
}
