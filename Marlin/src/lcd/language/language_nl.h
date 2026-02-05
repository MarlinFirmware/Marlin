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
 * Dutch
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
#define NOT_EXTENDED_ISO10646_1_5X7

namespace LanguageNarrow_nl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Dutch");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" gereed.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Kaart ingestoken");                         // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Kaart verwijderd");                         // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Hoofdmenu");                                // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Motoren uit");                              // Disable Steppers
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klik voor begin");                          // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Volgende Plaats");                          // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Bed level kompl.");                         // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Zet home offsets");                         // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("H offset toegep.");                         // Offsets Applied
  LSTR MSG_PREHEAT_1                      = PREHEAT_1_LABEL _UxGT(" voorverwarmen");           // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = PREHEAT_1_LABEL _UxGT(" voorverw. ~");             // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = PREHEAT_1_LABEL _UxGT(" voorverw. Einde");         // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = PREHEAT_1_LABEL _UxGT(" voorverw. Einde ~");       // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = PREHEAT_1_LABEL _UxGT(" voorverw. aan");           // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = PREHEAT_1_LABEL _UxGT(" voorverw. Bed");           // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = PREHEAT_1_LABEL _UxGT(" verw. conf");              // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("$ voorverwarmen");                          // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("$ voorverw. ~");                            // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("$ voorverw. Einde");                        // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("$ voorverw. Einde ~");                      // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("$ voorverw. aan");                          // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("$ voorverw. Bed");                          // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("$ verw. conf");                             // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Afkoelen");                                 // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Stroom aan");                               // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Stroom uit");                               // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extrude");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retract");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("As verplaatsen");                           // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Z probe & Level");                          // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Bed Leveling");                             // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Level bed");                                // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Verplaats X");                              // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Verplaats Y");                              // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Verplaats Z");                              // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Verplaats @");                              // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extruder");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extruder *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Verplaats $mm");                            // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Verplaats $in");                            // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Verplaats $") LCD_STR_DEGREE;               // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Snelheid");                                 // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Bed Z");                                    // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Nozzle");                                   // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Nozzle ~");                                 // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Bed");                                      // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Fan snelheid");                             // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Fan snelheid ~");                           // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Flow");                                     // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Flow ~");                                   // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Control");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");                                 // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Selecteer *");                              // Select *
  LSTR MSG_ACC                            = _UxGT("Versn");                                    // Accel
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatuur");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Beweging");                                 // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");                                // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");                              // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCD contrast");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Geheugen opslaan");                         // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Geheugen laden");                           // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Noodstop reset");                           // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Ververs");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info scherm");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Voorbereiden");                             // Prepare
  LSTR MSG_TUNE                           = _UxGT("Afstellen");                                // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Print pauzeren");                           // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Print hervatten");                          // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Print stoppen");                            // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Print van SD");                             // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Geen SD kaart");                            // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Slapen...");                                // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Wachten...");                               // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Print afgebroken");                         // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Geen beweging.");                           // No Move.
  LSTR MSG_KILLED                         = _UxGT("Afgebroken. ");                             // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("Gestopt. ");                                // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retract mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Ruil Retract mm");                          // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retract  F");                               // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Hop mm");                                   // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Ruil Unretr. mm");                          // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretr. FR");                               // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");                             // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Verv. Filament");                           // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Verv. Filament *");                         // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Init. SD kaart");                           // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Init. SD kaart");                           // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Init. USB stick");                          // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Verv. SD Kaart");                           // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autostart");                                // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z probe uit. bed");                         // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset BLTouch");                            // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X Offset");                                 // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y Offset");                                 // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z Offset");                                 // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("@ Offset");                                 // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Babystep Probe Z");                         // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystap X");                               // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystap Y");                               // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystap Z");                               // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Babystap @");                               // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Endstop afbr.");                            // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Voorverw. fout");                           // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Redun. temp fout");                         // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("Therm. wegloop");                           // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: Max. temp");                           // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: Min. temp");                           // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("PRINTER GESTOPT");                          // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Reset A.U.B.");                             // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Voorwarmen...");                            // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Bed voorverw...");                          // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Calibratie");                         // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibreer X");                              // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibreer Y");                              // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibreer Z");                              // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibreer Midden");                         // Calibrate Center
  LSTR MSG_CASE_LIGHT                     = _UxGT("Case licht");                               // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Aantal");                                   // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Voltooid");                                 // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Printtijd ");                               // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Langste");                                  // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extrud.");                                  // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Min Temp");                                 // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Max Temp");                                 // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("PSU");                                      // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Motorstroom");                              // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC Opslaan");                              // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Hervat print");                             // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_2_LINE("Wacht voor", "start..."));       // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_2_LINE("Laad filament", "en druk knop")); // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_2_LINE("Wacht voor", "uitladen..."));    // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_2_LINE("Wacht voor", "inladen..."));     // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_2_LINE("Wacht voor", "printing..."));    // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Aan");                                      // On
  LSTR MSG_LCD_OFF                        = _UxGT("Uit");                                      // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Onjuiste printer");                         // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Debug Menu");                               // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Vooruitgang Test");                         // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Verplaatsen...");                           // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Vrij XY");                                  // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s Eerst");                            // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Verwarmen..."));                 // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_2_LINE("Klik knop om...", "verw. nozzle.")); // |Click to heat
  LSTR MSG_BACK                           = _UxGT("Terug");                                    // Back
  LSTR MSG_ADVANCE_K                      = _UxGT("Advance K");                                // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Advance K *");                              // Advance K *
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch Zelf-Test");                        // Self-Test
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Delta conf");                               // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto Calibratie");                          // Auto Calibration
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT(" Nozzle: ");                                // Nozzle:
}

namespace LanguageWide_nl {
  using namespace LanguageNarrow_nl;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Printed Aantal");                           // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Totaal Voltooid");                          // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Totale Printtijd");                         // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Langste Printtijd");                        // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Totaal Extrudeert");                        // Extruded Total
  #endif
}

namespace LanguageTall_nl {
  using namespace LanguageWide_nl;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Wacht voor start", "filament te", "verwisselen")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Laad filament", "en druk knop", "om verder...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_3_LINE("Wacht voor", "filament uit", "te laden")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_3_LINE("Wacht voor", "filament te", "laden")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_3_LINE("Wacht voor print", "om verder", "te gaan")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Nozzle verw.", "Wacht a.u.b.")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Klik knop om...", "verw. nozzle.")); // |Press button|to heat nozzle
  #endif
}

namespace Language_nl {
  using namespace LanguageTall_nl;
}
