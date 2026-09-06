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
 * Catalan
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

namespace LanguageNarrow_ca {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Catalan");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" preparada.");           // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Targeta detectada.");                       // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Targeta extreta.");                         // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menú principal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Desactiva motors");                         // Disable Steppers
  LSTR MSG_HOMING                         = _UxGT("Origen");                                   // Homing
  LSTR MSG_AUTO_HOME                      = _UxGT("Ves a l'origen");                           // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("X a origen");                               // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Y a origen");                               // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Z a origen");                               // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Origen XYZ");                               // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Premeu per iniciar");                       // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Següent punt");                             // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Anivellament fet!");                        // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ajusta decalatge");                         // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Decalatge aplicat");                        // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Preescalfa ") PREHEAT_1_LABEL;              // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Preescalfa ") PREHEAT_1_LABEL " ~";         // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" End"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" End ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Tot"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Llit"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Conf."); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Preescalfa $");                             // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Preescalfa $ ~");                           // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Preescalfa $ End");                         // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Preescalfa $ End ~");                       // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Preescalfa $ Tot");                         // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Preescalfa $ Llit");                        // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Preescalfa $ Conf.");                       // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Refreda");                                  // Cooldown
  LSTR MSG_EXTRUDE                        = _UxGT("Extrudeix");                                // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retreu");                                   // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mou eixos");                                // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sonda i anivella");                         // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Anivella llit");                            // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Anivella llit");                            // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Mou X");                                    // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Mou Y");                                    // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Mou Z");                                    // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Mou @");                                    // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extrusor");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extrusor *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Mou $mm");                                  // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Mou $in");                                  // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Mou $") LCD_STR_DEGREE;                     // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Velocitat");                                // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Llit Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Nozzle");                                   // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Nozzle ~");                                 // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Llit");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Vel. Ventilador");                          // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vel. Ventilador ~");                        // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Flux");                                     // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Flux ~");                                   // Flow ~
  LSTR MSG_VTRAV_MIN                      = _UxGT("VViatge min");                              // Min Travel Speed
  LSTR MSG_A_RETRACT                      = _UxGT("Accel. retracc");                           // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("Accel. Viatge");                            // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Passos/mm");                                // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" passos/mm");                         // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" passos/mm");                         // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" passos/mm");                         // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("E passos/mm");                              // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("@ passos/mm");                              // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* passos/mm");                              // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Moviment");                                 // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E en mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diam. Fil.");                               // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diam. Fil. *");                             // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Contrast de LCD");                          // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Desa memoria");                             // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Carrega memoria");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Restaura valors");                          // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Actualitza");               // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Pantalla Info.");                           // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Prepara");                                  // Prepare
  LSTR MSG_TUNE                           = _UxGT("Ajusta");                                   // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausa impressio");                          // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Repren impressio");                         // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Atura impressio.");                         // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Imprimeix de SD");                          // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("No hi ha targeta");                         // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("En repos...");                              // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Esperant usuari..");                        // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Imp. cancelada");                           // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sense moviment.");                          // No Move.
  LSTR MSG_KILLED                         = _UxGT("MATAT.");                                   // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("ATURADA.");                                 // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retreu mm");                                // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Swap Retreure mm");                         // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retreu V");                                 // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Aixeca mm");                                // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("DesRet +mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Swap DesRet +mm");                          // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("DesRet V");                                 // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto retraccio");                           // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Canvia filament");                          // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Canvia filament *");                        // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Inicialitza SD");                           // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Inicialitza SD");                           // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Inicialitza USB");                          // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Canvia SD");                                // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Inici automatic");                          // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z fora");                             // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reinicia BLTouch");                         // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Decalatge X");                              // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Decalatge Y");                              // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Decalatge Z");                              // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Decalatge @");                              // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Pas a pas el Z");                           // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Micropas X");                               // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Micropas Y");                               // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Micropas Z");                               // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Micropas @");                               // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Cancel. Endstop");                          // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Error al escalfar");                        // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err: TEMP REDUNDANT");                      // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("THERMAL RUNAWAY");                          // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: TEMP MAXIMA");                         // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: TEMP MINIMA");                         // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("IMPRESSORA PARADA");                        // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Reinicieu");                                // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Escalfant...");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Escalfant llit...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibratge Delta");                         // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibra X");                                // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibra Y");                                // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibra Z");                                // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibra el centre");                        // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Quant a la impr.");                         // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info Impressora");                          // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Estadistiques");                            // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info placa");                               // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistors");                               // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrusors");                                // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baud");                                     // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocol");                                 // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Llum");                                     // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Impressions");                              // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Acabades");                                 // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Mes llarg");                                // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extrudit");                                 // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp. mínima");                             // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp. màxima");                             // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("Font alimentacio");                         // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Força motor");                              // Drive Strength
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Repren impressió");                         // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Espereu..."));                   // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Insereix i prem"));              // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Expulsant..."));                 // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Carregant..."));                 // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Reprenent..."));                 // |Resuming...
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Impressora incorrecta");                    // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu de depuracio");                        // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test barra progres");                       // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Movent..");                                 // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("XY lliures");                               // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s primer");                           // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Escalfant..."));                 // |Heating...
}

namespace LanguageWide_ca {
  using namespace LanguageNarrow_ca;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Total impressions");                        // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Acabades");                                 // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Temps imprimint");                          // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Treball mes llarg");                        // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total extrudit");                           // Extruded Total
  #endif
}

namespace LanguageTall_ca {
  using namespace LanguageWide_ca;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Esperant per", "iniciar el canvi", "de filament")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Poseu filament", "i premeu el boto", "per continuar...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Esperant per", "treure filament")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Esperant carrega", "de filament")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Esperant per", "reprendre"));    // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Escalfant nozzle", "Espereu...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Premeu boto per", "escalfar nozzle.")); // |Press button|to heat nozzle
  #endif
}

namespace Language_ca {
  using namespace LanguageTall_ca;
}
