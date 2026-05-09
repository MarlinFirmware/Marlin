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
 * Romanian
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

/**
 * Translation by cristyanul
 */

namespace LanguageNarrow_ro {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Romanian");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" Pregatit.");            // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Media Introdus");                           // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Media Inlaturat");                          // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Principal");                                // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Dezactivare Motoare");                      // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto Acasa");                               // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Acasa X");                                  // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Acasa Y");                                  // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Acasa Z");                                  // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Acasa XYZ");                                // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Click pentru a incepe");                    // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Urmatorul Punct");                          // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Nivelare Terminata!");                      // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Seteaza Offseturile Acasa");                // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offseturi Aplicate");                       // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Preincalzeste ") PREHEAT_1_LABEL;           // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Preincalzeste ") PREHEAT_1_LABEL " ~";      // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Capatul"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Capatul ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Tot"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Patul"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Conf"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Preincalzeste $");                          // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Preincalzeste $ ~");                        // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Preincalzeste $ Capatul");                  // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Preincalzeste $ Capatul ~");                // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Preincalzeste $ Tot");                      // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Preincalzeste $ Patul");                    // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Preincalzeste $ Conf");                     // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Racire");                                   // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Porneste");                                 // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Opreste");                                  // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extrudeaza");                               // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retracteaza");                              // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Muta Axa");                                 // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Probare si Nivelare");                      // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Nivelarea Patului");                        // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Niveleaza Patul");                          // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Move X");                                   // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Move Y");                                   // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Move Z");                                   // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Move @");                                   // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extruder");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extruder *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Move $mm");                                 // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Move $in");                                 // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Move $") LCD_STR_DEGREE;                    // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Speed");                                    // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Bed Z");                                    // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Nozzle");                                   // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Nozzle ~");                                 // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Bed");                                      // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Fan Speed");                                // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Fan Speed ~");                              // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Flow");                                     // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Flow ~");                                   // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Control");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");                                 // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Select *");                                 // Select *
  LSTR MSG_ACC                            = _UxGT("Accel");                                    // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("VTrav Min");                                // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Acceleration");                             // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Amax ") STR_A;                              // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Amax ") STR_B;                              // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Amax ") STR_C;                              // Max (STR_C) Accel
  LSTR MSG_AMAX_N                         = _UxGT("Amax @");                                   // Max @ Accel
  LSTR MSG_AMAX_E                         = _UxGT("Amax E");                                   // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Amax *");                                   // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("A-Retract");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-Travel");                                 // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Steps/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" steps/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" steps/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" steps/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ steps/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E steps/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* steps/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperature");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Motion");                                   // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");                                // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");                              // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCD Contrast");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Store Settings");                           // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Load Settings");                            // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Restore Defaults");                         // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Refresh");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info Screen");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Prepare");                                  // Prepare
  LSTR MSG_TUNE                           = _UxGT("Tune");                                     // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pause Print");                              // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Resume Print");                             // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Stop Print");                               // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Print from Media");                         // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("No Media");                                 // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Sleep...");                                 // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Click to Resume...");                       // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Print Aborted");                            // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("No Move.");                                 // No Move.
  LSTR MSG_KILLED                         = _UxGT("KILLED. ");                                 // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("STOPPED. ");                                // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retract mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Swap Re.mm");                               // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retract V");                                // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Hop mm");                                   // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("S Unretr. mm");                             // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");                              // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");                             // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Inlocuire Filament");                       // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Inlocuire Filament *");                     // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Atasare card SD");                          // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Atasare card SD");                          // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Atasare FD USB");                           // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Inlocuire Media");                          // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autostart");                                // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z Probe Past Bed");                         // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset");                                    // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Probe X Offset");                           // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Probe Y Offset");                           // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Probe Z Offset");                           // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Probe @ Offset");                           // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Pas Z ajustare");                           // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystep X");                               // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystep Y");                               // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystep Z");                               // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Babystep @");                               // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Endstop Abort");                            // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Heating Failed");                           // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err: REDUNDANT TEMP");                      // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("THERMAL RUNAWAY");                          // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: MAXTEMP");                             // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: MINTEMP");                             // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("PRINTER HALTED");                           // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Please Reset");                             // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Heating...");                               // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Bed Heating...");                           // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Calibration");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrate X");                              // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrate Y");                              // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrate Z");                              // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrate Center");                         // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Despre Imprimanta");                        // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info Imprimanta");                          // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Status Imprimanta");                        // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Informatii Placa");                         // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistoare");                              // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrudere");                                // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baud");                                     // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocol");                                 // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Case Light");                               // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Prints");                                   // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Completed");                                // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Longest");                                  // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extruded");                                 // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temperatura Minima");                       // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temperatura Maxima");                       // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("PSU");                                      // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Drive Strength");                           // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC EEPROM Write");                         // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Continue");                                 // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Va rog asteptati..."));          // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Insert and Click"));             // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Ejectare..."));                  // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Incarcare..."));                 // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Se Reia..."));                   // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("On");                                       // On
  LSTR MSG_LCD_OFF                        = _UxGT("Off");                                      // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("INCORRECT PRINTER");                        // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Meniu Debug");                              // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test Bara Progres");                        // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Moving...");                                // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Free XY");                                  // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s First");                            // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Incalzire..."));                 // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_1_LINE("Click pentru incalzire"));       // |Click to heat
  LSTR MSG_YES                            = _UxGT("DA");                                       // YES
  LSTR MSG_NO                             = _UxGT("NU");                                       // NO
  LSTR MSG_BACK                           = _UxGT("Inapoi");                                   // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Abandon...");                               // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Eroare Citire Media");                      // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("Dispozitiv USB Inlaturat");                 // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("Pornire USB Esuata");                       // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Soft Endstops");                            // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Setari Avansate");                          // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Configurare");                              // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto Aliniere-Z");                          // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Fade Inaltime");                            // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Preincalzeste Personalizat");               // Preheat Custom
  LSTR MSG_LASER_MENU                     = _UxGT("Control Laser");                            // Laser Control
  LSTR MSG_LASER_POWER                    = _UxGT("Puterea Laserului");                        // Laser Power
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Controlul Spindle");                        // Spindle Control
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Puterea Spindle");                          // Spindle Pwr
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Spindle Invers");                           // Spindle Reverse
  LSTR MSG_BED_TRAMMING                   = _UxGT("Niveleaza Colturile");                      // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Urmatorul Colt");                           // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor Mesh");                              // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Editeaza Mesh");                            // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Editarea Meshului Oprita");                 // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Punctul de Probare");                       // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Index X");                                  // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Index Y");                                  // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valoare Z");                                // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Comenzi Personalizate");                    // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 Probe Test");                           // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 Point");                                // M48 Point
  LSTR MSG_M48_DEVIATION                  = _UxGT("Deviation");                                // Deviation
  LSTR MSG_IDEX_MENU                      = _UxGT("IDEX Mode");                                // IDEX Mode
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Tool Offsets");                             // Tool Offsets
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Park");                                // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplication");                              // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Mirrored Copy");                            // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Full Control");                             // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2nd Nozzle Z");                             // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2nd Nozzle @");                             // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Doing G29");                                // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("UBL Tools");                                // UBL Tools
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Tilting Point");                            // Tilting Point
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Manually Build Mesh");                      // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Place Shim & Measure");                     // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Measure");                                  // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Remove & Measure Bed");                     // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Moving to next");                           // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Bed Temp");                                 // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Bed Temp");                                 // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Hotend Temp");                              // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Hotend Temp");                              // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Edit Custom Mesh");                         // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Fine Tuning Mesh");                         // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Done Editing Mesh");                        // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Build Custom Mesh");                        // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Build Mesh");                               // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Build Mesh ($)");                           // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Validate Mesh ($)");                        // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Build Cold Mesh");                          // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Adjust Mesh Height");                       // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Height Amount");                            // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Validate Mesh");                            // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Validate Custom Mesh");                     // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Heating Bed");                          // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Heating Nozzle");                       // G26 Heating Nozzle
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Manual priming...");                        // Manual priming...
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Fixed Length Prime");                       // Fixed Length Prime
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Done Priming");                             // Done Priming
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Canceled");                             // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("Leaving G26");                              // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Continue Bed Mesh");                        // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Mesh Leveling");                            // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("3-Point Leveling");                         // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Grid Mesh Leveling");                       // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Level Mesh");                               // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Side Points");                              // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Map Type");                                 // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Output Mesh Map");                          // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Output for Host");                          // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Output for CSV");                           // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Off Imprimanta Backup");                    // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Output UBL Info");                          // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Fill-in Amount");                           // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Manual Fill-in");                           // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Smart Fill-in");                            // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Fill-in Mesh");                             // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Invalidate All");                           // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Invalidate Closest");                       // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Fine Tune All");                            // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Fine Tune Closest");                        // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Mesh Storage");                             // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Memory Slot");                              // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Load Bed Mesh");                            // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Save Bed Mesh");                            // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Mesh %i Loaded");                           // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Mesh %i Saved");                            // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("No Storage");                               // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Err: UBL Save");                            // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Err: UBL Restore");                         // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-Offset: ");                               // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Z-Offset Stopped");                         // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("Step-By-Step UBL");                         // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1. Build Cold Mesh");                       // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2. Smart Fill-in");                         // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3. Validate Mesh");                         // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4. Fine Tune All");                         // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5. Validate Mesh");                         // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6. Fine Tune All");                         // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7. Save Bed Mesh");                         // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("LED Control");                              // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Lights");                                   // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Light #{");                                 // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Light Presets");                            // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Red");                                      // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Orange");                                   // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Yellow");                                   // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Green");                                    // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Blue");                                     // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Violet");                                   // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("White");                                    // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Default");                                  // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Custom Lights");                            // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Red Intensity");                            // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Green Intensity");                          // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Blue Intensity");                           // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("White Intensity");                          // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Brightness");                               // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Capat Prea Rece");                          // Hotend too cold
  LSTR MSG_CHAMBER                        = _UxGT("Enclosure");                                // Enclosure
  LSTR MSG_STORED_FAN_N                   = _UxGT("Stored Fan ~");                             // Stored Fan ~
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Extra Fan Speed");                          // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Extra Fan Speed ~");                        // Extra Fan Speed ~
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID Autotune");                             // PID Autotune
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID Autotune *");                           // Autotune * PID
  LSTR MSG_JERK                           = _UxGT("Jerk");                                     // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("V") STR_A _UxGT("-Jerk");                   // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("V") STR_B _UxGT("-Jerk");                   // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("V") STR_C _UxGT("-Jerk");                   // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("V@-Jerk");                                  // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Ve-Jerk");                                  // Max E Jerk
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Junction Dev");                             // Junction Dev
  LSTR MSG_MAX_SPEED                      = _UxGT("Max Speed");                                // Max Speed (mm/s)
  LSTR MSG_VMAX_A                         = _UxGT("Max Speed ") STR_A;                         // Max (STR_A) Speed
  LSTR MSG_VMAX_B                         = _UxGT("Max Speed ") STR_B;                         // Max (STR_B) Speed
  LSTR MSG_VMAX_C                         = _UxGT("Max Speed ") STR_C;                         // Max (STR_C) Speed
  LSTR MSG_VMAX_N                         = _UxGT("Max Speed @");                              // Max @ Speed
  LSTR MSG_VMAX_E                         = _UxGT("Max Speed E");                              // Max E Speed
  LSTR MSG_VMAX_EN                        = _UxGT("Max Speed *");                              // Max * Speed
  LSTR MSG_VMIN                           = _UxGT("Vmin");                                     // Min Velocity
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Unload mm");                                // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Load mm");                                  // Load mm
  LSTR MSG_ADVANCE_K                      = _UxGT("Advance K");                                // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Advance K *");                              // Advance K *
  LSTR MSG_INIT_EEPROM                    = _UxGT("Initialize EEPROM");                        // Initialize EEPROM
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Media Update");                             // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Reset Imprimanta");                         // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Start Imprimare");                          // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Urmatorul");                                // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Initiere");                                 // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("Stop");                                     // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Imprimeaza");                               // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Reseteaza");                                // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Anuleaza");                                 // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("OK");                                       // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Inapoi");                                   // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Proceed");                                  // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Printing Object");                          // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Cancel Object");                            // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Cancel Object {");                          // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Outage Recovery");                          // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Print Paused");                             // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Printing...");                              // Printing...
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");                                // S UnRet V
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Swap Length");                              // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Purge Length");                             // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Tool Inlocuire");                           // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Z Raise");                                  // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Prime Speed");                              // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Retract Speed");                            // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Nozzle Standby");                           // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Incarcare Filament");                       // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Incarcare Filament *");                     // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Scoatere Filament");                        // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Scoatere Filament *");                      // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Scoate Tot");                               // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Eliberare Media");                          // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Skew Factor");                              // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Self-Test");                                // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Stow");                                     // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Deploy");                                   // Deploy
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("SW-Mode");                                  // SW-Mode
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("5V-Mode");                                  // 5V-Mode
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("OD-Mode");                                  // OD-Mode
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Mode-Store");                               // Mode-Store
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Set BLTouch to 5V");                        // Set BLTouch to 5V
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Set BLTouch to OD");                        // Set BLTouch to OD
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Report Drain");                             // Report Drain
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("DANGER: Bad settings can cause damage! Proceed anyway?"); // DANGER: Bad settings can cause damage! Proceed anyway?
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");                                  // TouchMI
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Init TouchMI");                             // Init TouchMI
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Z Offset Test");                            // Z Offset Test
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Save");                                     // Save
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Deploy TouchMI");                           // Deploy TouchMI
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Deploy Z-Probe");                           // Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Stow Z-Probe");                             // Stow Z-Probe
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");                                    // Total
  LSTR MSG_COOLING                        = _UxGT("Cooling...");                               // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Bed Cooling...");                           // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Chamber Heating...");                       // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Chamber Cooling...");                       // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Delta Settings");                           // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto Calibration");                         // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Diag Rod");                                 // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Inaltime");                                 // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Radius");                                   // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("Nivelare in 3 puncte");                     // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Nivelare Lineara");                         // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Nivelare Bilineara");                       // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Nivelarea Patului Unificata");              // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Nivelare Mesh");                            // Mesh Leveling
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Runaway Watch: OFF");                       // Runaway Watch: OFF
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Runaway Watch: ON");                        // Runaway Watch: ON
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Light Brightness");                         // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driver %");                               // @ Driver %
  LSTR MSG_ERROR_TMC                      = _UxGT("TMC CONNECTION ERROR");                     // TMC CONNECTION ERROR
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("FILAMENT CHANGE");                          // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("PRINT PAUSED");                             // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("LOAD FILAMENT");                            // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("UNLOAD FILAMENT");                          // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("RESUME OPTIONS:");                          // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Purge more");                               // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Nozzle: ");                               // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Runout Sensor");                            // Runout Sensor
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Runout Dist mm");                           // Runout Dist mm
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Homing Failed");                            // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Probing Failed");                           // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("CHOOSE FILAMENT");                          // CHOOSE FILAMENT
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");                                      // MMU
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Update MMU Firmware!");                     // Update MMU Firmware!
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU Needs Attention.");                     // MMU Needs Attention.
  LSTR MSG_MMU2_RESUME                    = _UxGT("MMU Resume");                               // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("MMU Resuming...");                          // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("MMU Load");                                 // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("MMU Load All");                             // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("MMU Load to Nozzle");                       // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("MMU Eject");                                // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("MMU Eject ~");                              // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("MMU Unload");                               // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Loading Fil. %i...");                       // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Ejecting Fil. ...");                        // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Unloading Fil....");                        // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("All");                                      // All
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filament ~");                               // Filament ~
  LSTR MSG_MMU2_RESET                     = _UxGT("Reset MMU");                                // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("MMU Resetting...");                         // Resetting...
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Remove, click");                            // Eject Recover
  LSTR MSG_MIX                            = _UxGT("Mix");                                      // Mix
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Component {");                              // Component {
  LSTR MSG_MIXER                          = _UxGT("Mixer");                                    // Mixer
  LSTR MSG_GRADIENT                       = _UxGT("Gradient");                                 // Gradient
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Full Gradient");                            // Full Gradient
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Toggle Mix");                               // Toggle Mix
  LSTR MSG_CYCLE_MIX                      = _UxGT("Cycle Mix");                                // Cycle Mix
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Gradient Mix");                             // Gradient Mix
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Reverse Gradient");                         // Reverse Gradient
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Active V-tool");                            // Active V-tool
  LSTR MSG_START_VTOOL                    = _UxGT("Start V-tool");                             // Start V-tool
  LSTR MSG_END_VTOOL                      = _UxGT("  End V-tool");                             // End V-tool
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias V-tool");                             // Alias V-tool
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Reset V-tools");                            // Reset V-tools
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Commit V-tool Mix");                        // Commit V-tool Mix
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-tools Were Reset");                       // V-tools Were Reset
  LSTR MSG_START_Z                        = _UxGT("Start Z:");                                 // Start Z:
  LSTR MSG_END_Z                          = _UxGT("  End Z:");                                 // End Z:
  LSTR MSG_GAMES                          = _UxGT("Jocuri");                                   // Games
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");                                 // Brickout
  LSTR MSG_INVADERS                       = _UxGT("Invaders");                                 // Invaders
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");                                    // Sn4k3
  LSTR MSG_MAZE                           = _UxGT("Maze");                                     // Maze
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_1_LINE("Click to continue"));            // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Parcare..."));                   // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Curatare..."));                  // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_1_LINE("Click pentru a termina"));       // |Click to finish
  LSTR MSG_TMC_DRIVERS                    = _UxGT("TMC Drivers");                              // TMC Drivers
  LSTR MSG_TMC_CURRENT                    = _UxGT("Driver Current");                           // Driver Current
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Hybrid Threshold");                         // Hybrid Threshold
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Sensorless Homing");                        // Sensorless Homing
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Stepping Mode");                            // Stepping Mode
  LSTR MSG_TMC_STEALTHCHOP                = _UxGT("StealthChop");                              // StealthChop
  LSTR MSG_SERVICE_RESET                  = _UxGT("Reset");                                    // Reset
  LSTR MSG_SERVICE_IN                     = _UxGT(" in:");                                     // in:
  LSTR MSG_BACKLASH                       = _UxGT("Backlash");                                 // Backlash
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Corectare");                                // Correction
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Smoothing");                                // Smoothing
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Eroare:Subcall Overflow");                  // Subcall Overflow
  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frecventa");                                // Frequency
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Nozzle Parked");                            // Nozzle Parked
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Controller Fan");                           // Controller Fan
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Idle Speed");                               // Idle Speed
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Auto Mode");                                // Auto Mode
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Active Speed");                             // Active Speed
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Idle Period");                              // Idle Period
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("PID tuning done");                          // PID tuning done
  LSTR MSG_PID_BAD_HEATER_ID              = _UxGT("Autotune failed! Bad extruder.");           // Autotune failed! Bad extruder.
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Autotune failed! Temperature too high.");   // Autotune failed! Temperature too high.
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Autotune failed! Timeout.");                // Autotune failed! Timeout.
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Frequency max");                            // XY Freq Limit
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Feed min");                                 // Min FR Factor
  LSTR MSG_VOLUMETRIC_LIMIT               = _UxGT("E Limit in mm") SUPERSCRIPT_THREE;          // E Limit in mm(SUPERSCRIPT_THREE)
  LSTR MSG_VOLUMETRIC_LIMIT_E             = _UxGT("E Limit *");                                // E Limit *
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("Err: EEPROM CRC");                          // Err: EEPROM CRC
  LSTR MSG_ERR_EEPROM_SIZE                = _UxGT("Err: Mărimea EEPROM");                      // Err: EEPROM Size
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("Err: Versiunea EEPROM");                    // Err: EEPROM Version
  LSTR MSG_SETTINGS_STORED                = _UxGT("Settings Stored");                          // Settings Stored
  LSTR MSG_POWER_MONITOR                  = _UxGT("Power monitor");                            // Power monitor
  LSTR MSG_CURRENT                        = _UxGT("Intensitate");                              // Current
  LSTR MSG_VOLTAGE                        = _UxGT("Voltaj");                                   // Voltage
  LSTR MSG_POWER                          = _UxGT("Putere");                                   // Power
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignora");                                   // Ignore
  LSTR MSG_PAUSING                        = _UxGT("Pausing...");                               // Pausing...
  LSTR MSG_PRINT_DONE                     = _UxGT("Print Done");                               // Print Done
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Swap Extra");                               // Swap Extra
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Park Head");                                // Park Head
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Recover Speed");                            // Recover Speed
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Fan Speed");                                // Fan Speed
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Fan Time");                                 // Fan Time
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto Pornit");                              // Auto ON
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto Oprit");                               // Auto OFF
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Tool Migration");                           // Tool Migration
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Auto-migrare");                             // Auto-migration
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Last Extruder");                            // Last Extruder
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Migrate to *");                             // Migrate to *
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Probe Offsets");                            // Probe Offsets
  LSTR MSG_HOTEND_IDLE_TIMEOUT            = _UxGT("Hotend Idle Timeout");                      // Hotend Idle Timeout
  LSTR MSG_BAD_PAGE                       = _UxGT("Bad page index");                           // Bad page index
  LSTR MSG_BAD_PAGE_SPEED                 = _UxGT("Bad page speed");                           // Bad page speed
  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Nivelare Axa X");                           // Level X Axis
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Auto Calibrare");                           // Auto Calibrate
  LSTR MSG_HEATER_TIMEOUT                 = _UxGT("Timeout Incalzitor");                       // Heater Timeout
  LSTR MSG_REHEAT                         = _UxGT("Reincalzire");                              // Reheat
  LSTR MSG_REHEATING                      = _UxGT("Reincalzire...");                           // Reheating...
}

namespace LanguageWide_ro {
  using namespace LanguageNarrow_ro;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Total Printuri");                           // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Completat");                                // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Timp Imprimare Total");                     // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Longest Job Time");                         // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total Extrudat");                           // Extruded Total
  #endif
}

namespace LanguageTall_ro {
  using namespace LanguageWide_ro;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Astept ca", "inlocuirea filamentului", "sa inceapa")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Insert filament", "and press button", "to continue")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Wait for", "filament unload"));  // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Wait for", "filament load"));    // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Wait for print", "to resume...")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Nozzle heating", "Please wait...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Press button", "to heat nozzle")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Apasa Butonul", "pentru a reveni la print")); // |Press Button|to resume print
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Wait for", "filament purge"));   // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Click to finish", "filament purge")); // |Click to finish|filament purge
  #endif
}

namespace Language_ro {
  using namespace LanguageTall_ro;
}
