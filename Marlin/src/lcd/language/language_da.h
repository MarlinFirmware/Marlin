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
 * Danish
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

namespace LanguageNarrow_da {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Danish");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" er klar");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Kort isat");                                // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Kort fjernet");                             // (MEDIA_TYPE_EN) Removed
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu");                                     // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Slå alle steppere fra");                    // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto Home");                                // Auto Home
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klik når du er klar");                      // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Næste punkt");                              // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Bed level er færdig!");                     // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Sæt forsk. af home");                       // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Forsk. er nu aktiv");                       // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Forvarm ") PREHEAT_1_LABEL;                 // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Forvarm ") PREHEAT_1_LABEL " ~";            // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end");   // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Alle");  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Bed");   // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" conf");  // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Forvarm $");                                // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Forvarm $ ~");                              // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Forvarm $ end");                            // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Forvarm $ end ~");                          // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Forvarm $ Alle");                           // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Forvarm $ Bed");                            // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Forvarm $ conf");                           // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Afkøl");                                    // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Slå strøm til");                            // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Slå strøm fra");                            // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extruder");                                 // Extrude
  LSTR MSG_MOVE_AXIS                      = _UxGT("Flyt akser");                               // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Probe og nivellér");                        // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Juster bed");                               // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Juster bed");                               // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Flyt X");                                   // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Flyt Y");                                   // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Flyt Z");                                   // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Flyt @");                                   // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Flyt E");                                   // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Flyt *");                                   // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Flyt $mm");                                 // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Flyt $in");                                 // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Flyt $") LCD_STR_DEGREE;                    // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Hastighed");                                // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Plade Z");                                  // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Dyse");                                     // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Dyse ~");                                   // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Plade");                                    // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Blæser hastighed");                         // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Blæser hastighed ~");                       // Fan Speed ~
  LSTR MSG_CONTROL                        = _UxGT("Kontrol");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");                                 // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Vælg *");                                   // Select *
  LSTR MSG_A_RETRACT                      = _UxGT("A-retract");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-rejse");                                  // Travel Accel
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatur");                               // Temperature
  LSTR MSG_MOTION                         = _UxGT("Bevægelse");                                // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E i mm") SUPERSCRIPT_THREE;                 // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");                                // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");                              // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCD kontrast");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Gem i EEPROM");                             // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Hent fra EEPROM");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Gendan Defaults");                          // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Genopfrisk");               // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info skærm");                               // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Forbered");                                 // Prepare
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pause printet");                            // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Forsæt printet");                           // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Stop printet");                             // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Print fra SD");                             // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Intet SD kort");                            // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Dvale...");                                 // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Venter på bruger...");                      // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Print annulleret");                         // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Ingen bevægelse.");                         // No Move.
  LSTR MSG_KILLED                         = _UxGT("DRÆBT. ");                                  // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("STOPPET. ");                                // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Tilbagetræk mm");                           // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Skift Re.mm");                              // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Tilbagetræk V");                            // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Hop mm");                                   // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Skift Unretr. mm");                         // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");                              // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");                             // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Skift filament");                           // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Skift filament *");                         // Change * Filament
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Skift SD kort");                            // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Probe udenfor plade");                      // Z Probe Past Bed
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Justér Z trin");                            // Babystep Probe Z
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Opvarmning fejlet");                        // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Fejl: reserve temp");                       // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("Temp løber løbsk");                         // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Fejl: Maks temp");                          // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Fejl: Min temp");                           // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("PRINTER STOPPET");                          // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Reset Venligst");                           // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Opvarmer...");                              // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Opvarmer plade...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibrering");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibrer X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibrer Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibrer Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibrerings Center");                      // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Om Printer");                               // About Printer
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Kort Info");                                // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Thermistors");                              // Thermistors
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Prints");                                   // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Færdige");                                  // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Længste");                                  // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extruderet");                               // Extruded
  LSTR MSG_INFO_PSU                       = _UxGT("Strømfors.");                               // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Driv Styrke");                              // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC EEPROM Skriv");                         // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Forsæt print");                             // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Vent venligst..."));             // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Indsæt og klik"));               // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Udskyder..."));                  // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Indtager..."));                  // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Fortsætter..."));                // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Til");                                      // On
  LSTR MSG_LCD_OFF                        = _UxGT("Fra");                                      // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Forkert printer");                          // INCORRECT PRINTER
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s først");                            // Home %s First
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch Selv-Test");                        // Self-Test
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driv %");                                 // @ Driver %
}

namespace LanguageWide_da {
  using namespace LanguageNarrow_da;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Ant. Prints");                              // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Færdige");                                  // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Total print tid");                          // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Længste print");                            // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total Extruderet");                         // Extruded Total
  #endif
}

namespace LanguageTall_da {
  using namespace LanguageWide_da;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Vent på start", "af filament", "skift")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Indsæt filament", "og tryk på knap", "for at fortsætte...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Vent på", "filament udskyd."));  // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Vent på", "filament indtag"));   // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Vent på at print", "fortsætter")); // |Wait for print|to resume...
  #endif
}

namespace Language_da {
  using namespace LanguageTall_da;
}
