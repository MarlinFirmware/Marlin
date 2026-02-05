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
 * Czech
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
 * Translated by Petr Zahradnik, Computer Laboratory
 * Blog and video blog Zahradnik se bavi
 * https://www.zahradniksebavi.cz
 */

#define DISPLAY_CHARSET_ISO10646_CZ

namespace LanguageNarrow_cz {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Czech");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" připraven.");           // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Médium vloženo");                           // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Médium vyjmuto");                           // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstopy");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Hlavní nabídka");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Uvolnit motory");                           // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Domovská pozice");                          // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Domů osa X");                               // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Domů osa Y");                               // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Domů osa Z");                               // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Měření podložky");                          // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Kliknutím spusťte");                        // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Další bod");                                // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Měření hotovo!");                           // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Nastavit ofsety");                          // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Ofsety nastaveny");                         // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Zahřát ") PREHEAT_1_LABEL;                  // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Zahřát ") PREHEAT_1_LABEL " ~";             // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Zahřát ") PREHEAT_1_LABEL _UxGT(" end");    // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Zahřát ") PREHEAT_1_LABEL _UxGT(" end ~");  // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Zahřát ") PREHEAT_1_LABEL _UxGT(" vše");    // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Zahřát ") PREHEAT_1_LABEL _UxGT(" podlož"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Zahřát ") PREHEAT_1_LABEL _UxGT(" nast");   // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Zahřát $");                                 // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Zahřát $ ~");                               // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Zahřát $ end");                             // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Zahřát $ end ~");                           // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Zahřát $ vše");                             // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Zahřát $ podlož");                          // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Zahřát $ nast");                            // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Zchladit");                                 // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Zapnout napájení");                         // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Vypnout napájení");                         // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Vytlačit (extr.)");                         // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Zatlačit (retr.)");                         // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Posunout osy");                             // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Měření a vyrovnání");                       // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Vyrovnat podložku");                        // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Vyrovnat podložku");                        // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Posunout X");                               // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Posunout Y");                               // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Posunout Z");                               // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Posunout @");                               // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extrudér");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extrudér *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Posunout o $mm");                           // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Posunout o $in");                           // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Posunout o $") LCD_STR_DEGREE;              // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Rychlost");                                 // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Výška podl.");                              // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Tryska");                                   // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Tryska ~");                                 // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Podložka");                                 // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Rychlost vent.");                           // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Rychlost vent. ~");                         // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Průtok");                                   // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Průtok ~");                                 // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Ovládaní");                                 // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Autoteplota");                              // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Vybrat *");                                 // Select *
  LSTR MSG_ACC                            = _UxGT("Zrychl");                                   // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("VTrav Min");                                // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Akcelerace");                               // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Max ") STR_A _UxGT(" Akcel");               // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Max ") STR_B _UxGT(" Akcel");               // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Max ") STR_C _UxGT(" Akcel");               // Max (STR_C) Accel
  LSTR MSG_AMAX_N                         = _UxGT("Max @ Akcel");                              // Max @ Accel
  LSTR MSG_AMAX_E                         = _UxGT("Max E Akcel");                              // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Max * Akcel");                              // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("A-retrakt");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-přejezd");                                // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Kroků/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" kroků/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" kroků/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" kroků/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ kroků/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E kroků/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* kroků/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Teplota");                                  // Temperature
  LSTR MSG_MOTION                         = _UxGT("Pohyb");                                    // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E na mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Prum.");                               // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Prum. *");                             // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Kontrast LCD");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Uložit nastavení");                         // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Načíst nastavení");                         // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Obnovit výchozí");                          // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Obnovit");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info obrazovka");                           // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Připrava tisku");                           // Prepare
  LSTR MSG_TUNE                           = _UxGT("Doladění tisku");                           // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pozastavit tisk");                          // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Obnovit tisk");                             // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Zastavit tisk");                            // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Tisknout z SD");                            // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Žádná SD karta");                           // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Uspáno...");                                // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Čekání na uživ...");                        // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Tisk zrušen");                              // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Žádný pohyb.");                             // No Move.
  LSTR MSG_KILLED                         = _UxGT("PŘERUSENO. ");                              // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("ZASTAVENO. ");                              // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retrakt mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Výměna Re.mm");                             // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retraktovat  V");                           // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Zvednuti Z mm");                            // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("S Unretr. mm");                             // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");                              // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");                             // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Vyměnit filament");                         // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Vyměnit filament *");                       // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Načíst médium");                            // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Načíst SD");                                // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Načíst USB");                               // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Vyměnit médium");                           // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autostart");                                // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z mimo podl");                        // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("BLTouch reset");                            // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X ofset");                                  // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y ofset");                                  // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z ofset");                                  // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("@ ofset");                                  // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Krok a Z offset");                          // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystep X");                               // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystep Y");                               // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystep Z");                               // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Babystep @");                               // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Endstop abort");                            // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Chyba zahřívání");                          // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("REDUND. TEPLOTA");                          // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("TEPLOTNÍ ÚNIK");                            // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("VYSOKÁ TEPLOTA");                           // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("NÍZKA TEPLOTA");                            // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("TISK. ZASTAVENA");                          // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Proveďte reset");                           // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Zahřívání...");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Zahřívání podl.");                          // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibrace");                          // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibrovat X");                             // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibrovat Y");                             // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibrovat Z");                             // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibrovat Střed");                         // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("O tiskárně");                               // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info o tiskárně");                          // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Statistika");                               // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info o desce");                             // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistory");                               // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrudéry");                                // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Rychlost");                                 // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokol");                                 // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Osvětlení");                                // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Tisky");                                    // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Hotovo");                                   // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Čas");                                      // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Nejdelší");                                 // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Vytlačeno");                                // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Teplota min");                              // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Teplota max");                              // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("Nap. zdroj");                               // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Buzení motorů");                            // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC uložit EEPROM");                        // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Obnovit tisk");                             // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Čekejte..."));                   // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Vložte, klikněte"));             // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Vysouvání..."));                 // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Zavádění..."));                  // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Pokračování..."));               // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" fakt");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Zap");                                      // On
  LSTR MSG_LCD_OFF                        = _UxGT("Vyp");                                      // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("NESPRÁVNÁ TISKÁRNA");                       // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Nabídka ladění");                           // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test uk. průběhu");                         // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Posouvání...");                             // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Uvolnit XY");                               // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Domů %s první");                            // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Nahřívání..."));                 // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_2_LINE("Klikněte pro", "nahřátí"));      // |Click to heat
  LSTR MSG_YES                            = _UxGT("ANO");                                      // YES
  LSTR MSG_NO                             = _UxGT("NE");                                       // NO
  LSTR MSG_BACK                           = _UxGT("Zpět");                                     // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Rušení...");                                // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Chyba čtení média");                        // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("USB odstraněno");                           // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("Chyba USB");                                // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Soft Endstopy");                            // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Další nastavení");                          // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Konfigurace");                              // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto srovnání Z");                          // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Výška srovnávání");                         // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Zahřát vlastní");                           // Preheat Custom
  LSTR MSG_LASER_MENU                     = _UxGT("Ovládání laseru");                          // Laser Control
  LSTR MSG_LASER_POWER                    = _UxGT("Výkon laseru");                             // Laser Power
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Vřeteno ovládání");                         // Spindle Control
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Vřeteno výkon");                            // Spindle Pwr
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Vřeteno opačně");                           // Spindle Reverse
  LSTR MSG_BED_TRAMMING                   = _UxGT("Vyrovnat rohy");                            // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Další roh");                                // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor sítě");                              // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Upravit síť bodů");                         // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Konec úprav sítě");                         // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Měření bodu");                              // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Index X");                                  // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Index Y");                                  // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Hodnota Z");                                // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Vlastní příkazy");                          // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 test sondy");                           // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 bod");                                  // M48 Point
  LSTR MSG_M48_DEVIATION                  = _UxGT("Odchylka");                                 // Deviation
  LSTR MSG_IDEX_MENU                      = _UxGT("Režim IDEX");                               // IDEX Mode
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Ofsety nástrojů");                          // Tool Offsets
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Park");                                // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplikace");                                // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Zrcadlení");                                // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Plná kontrola");                            // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2. tryska Z");                              // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2. tryska @");                              // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Provádím G29");                             // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("UBL nástroje");                             // UBL Tools
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Vyrovnání bodu");                           // Tilting Point
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Manuální síť bodů");                        // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Vložte kartu, změřte");                     // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Změřte");                                   // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Odstraňte a změřte");                       // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Přesun na další");                          // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Teplota podložky");                         // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Teplota podložky");                         // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Teplota hotendu");                          // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Teplota hotendu");                          // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Upravit vlastní síť");                      // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Doladit síť bodů");                         // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Konec úprav sítě");                         // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Vlastní síť");                              // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Vytvořit síť");                             // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Síť bodů $");                               // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Kontrola sítě $");                          // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Studená síť bodů");                         // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Upravit výšku sítě");                       // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Výška");                                    // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Zkontrolovat síť");                         // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Kontrola vlast. sítě");                     // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 zahřívání podl.");                      // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 zařívání trysky");                      // G26 Heating Nozzle
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Ruční zavedení...");                        // Manual priming...
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Pevné zavední");                            // Fixed Length Prime
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Done Priming");                             // Done Priming
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Canceled");                             // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("Leaving G26");                              // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Pokračovat v síťi");                        // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Síťové rovnání");                           // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("3-bodové rovnání");                         // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Mřížkové rovnání");                         // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Srovnat podložku");                         // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Postranní body");                           // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Typ sítě bodu");                            // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Exportovat síť");                           // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Exportovat do PC");                         // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Exportovat do CSV");                        // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Záloha do PC");                             // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Info o UBL do PC");                         // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Hustota mřížky");                           // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Ruční hustota");                            // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Chytrá hustota");                           // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Zaplnit mřížku");                           // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Zrušit všechno");                           // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Zrušit poslední");                          // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Upravit všechny");                          // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Upravit poslední");                         // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Uložiště sítí");                            // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Paměťový slot");                            // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Načíst síť bodů");                          // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Uložit síť bodů");                          // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Síť %i načtena");                           // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Síť %i uložena");                           // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Nedostatek místa");                         // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Ch.: Uložit UBL");                          // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Ch.: Obnovit UBL");                         // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-ofset: ");                                // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Konec Z-ofsetu");                           // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL Postupně");                             // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1. Studená síť bodů");                      // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2. Chytrá hustota");                        // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3. Zkontrolovat síť");                      // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4. Upravit všechny");                       // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5. Zkontrolovat síť");                      // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6. Upravit všechny");                       // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7. Uložit síť bodů");                       // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("Nastavení LED");                            // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Světla");                                   // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Světla #{");                                // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Světla Předvolby");                         // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Červená");                                  // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Oranžová");                                 // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Žlutá");                                    // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Zelená");                                   // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Modrá");                                    // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Fialová");                                  // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Bílá");                                     // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Výchozí");                                  // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Vlastní světla");                           // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Červená intenzita");                        // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Zelená intezita");                          // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Modrá intenzita");                          // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Bílá intenzita");                           // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Jas");                                      // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Hotend je studený");                        // Hotend too cold
  LSTR MSG_CHAMBER                        = _UxGT("Komora");                                   // Enclosure
  LSTR MSG_STORED_FAN_N                   = _UxGT("Ulož. vent. ~");                            // Stored Fan ~
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Rychlost ex. vent.");                       // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Rychlost ex. vent. ~");                     // Extra Fan Speed ~
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID automatika");                           // PID Autotune
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID automatika *");                         // Autotune * PID
  LSTR MSG_JERK                           = _UxGT("Jerk");                                     // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("Max ") STR_A _UxGT(" Jerk");                // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("Max ") STR_B _UxGT(" Jerk");                // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("Max ") STR_C _UxGT(" Jerk");                // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("Max @ Jerk");                               // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Max E Jerk");                               // Max E Jerk
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Odchylka spoje");                           // Junction Dev
  LSTR MSG_MAX_SPEED                      = _UxGT("Max Rychlost");                             // Max Speed (mm/s)
  LSTR MSG_VMAX_A                         = _UxGT("Max ") STR_A _UxGT(" Rychlost");            // Max (STR_A) Speed
  LSTR MSG_VMAX_B                         = _UxGT("Max ") STR_B _UxGT(" Rychlost");            // Max (STR_B) Speed
  LSTR MSG_VMAX_C                         = _UxGT("Max ") STR_C _UxGT(" Rychlost");            // Max (STR_C) Speed
  LSTR MSG_VMAX_N                         = _UxGT("Max @ Rychlost");                           // Max @ Speed
  LSTR MSG_VMAX_E                         = _UxGT("Max E Rychlost");                           // Max E Speed
  LSTR MSG_VMAX_EN                        = _UxGT("Max * Rychlost");                           // Max * Speed
  LSTR MSG_VMIN                           = _UxGT("Vmin");                                     // Min Velocity
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Vysunout mm");                              // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Zavést mm");                                // Load mm
  LSTR MSG_ADVANCE_K                      = _UxGT("K pro posun");                              // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("K pro posun *");                            // Advance K *
  LSTR MSG_INIT_EEPROM                    = _UxGT("Inic. EEPROM");                             // Initialize EEPROM
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Aktualizace z SD");                         // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Reset tiskárny");                           // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Spustit tisk");                             // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Další");                                    // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Inicializace");                             // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("Stop");                                     // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Tisk");                                     // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Reset");                                    // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Zrušit");                                   // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("Hotovo");                                   // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Zpět");                                     // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Pokračovat");                               // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Tisk objektu");                             // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Zrušit objekt");                            // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Zrušit objekt {");                          // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Obnova výpadku");                           // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Tisk pozastaven");                          // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Tisknu...");                                // Printing...
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");                                // S UnRet V
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Délka retrakce");                           // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Délka zavedení");                           // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Výměna nástroje");                          // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Zdvih Z");                                  // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Rychlost primár.");                         // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Rychlost retrak.");                         // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Tryska standby");                           // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Zavést filament");                          // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Zavést filament *");                        // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Vysunout filament");                        // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Vysunout filament *");                      // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Vysunout vše");                             // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Vysunout médium");                          // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Faktor zkosení");                           // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch self-test");                        // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("BLTouch zasunout");                         // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("BLTouch vysunout");                         // Deploy
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("SW výsun BLTouch");                         // SW-Mode
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("BLTouch 5V režim");                         // 5V-Mode
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("BLTouch OD režim");                         // OD-Mode
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Uložit režim");                             // Mode-Store
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Nastavit 5V");                              // Set BLTouch to 5V
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Nastacit OD");                              // Set BLTouch to OD
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Vypsat nastavení");                         // Report Drain
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("VAROVANÍ: Špatné nastavení může způsobit škody! Pokračovat?"); // DANGER: Bad settings can cause damage! Proceed anyway?
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");                                  // TouchMI
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Inic. TouchMI");                            // Init TouchMI
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test Z Ofsetu");                            // Z Offset Test
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Uložiy");                                   // Save
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("vysunout TouchMI");                         // Deploy TouchMI
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Vysunout Z-sondu");                         // Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Zasunout Z-sondu");                         // Stow Z-Probe
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Celkem");                                   // Total
  LSTR MSG_COOLING                        = _UxGT("Chlazení...");                              // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Chlazení podl.");                           // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Zahřívání komory...");                      // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Chlazení komory...");                       // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Delta nastavení");                          // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Autokalibrace");                            // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Diag rameno");                              // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Výška");                                    // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Poloměr");                                  // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("3-bodové rovnání");                         // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Lineárni rovnání");                         // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Bilineární rovnání");                       // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Unified Bed Leveling");                     // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Mřížkové rovnání");                         // Mesh Leveling
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Sledování úniku: VYP");                     // Runaway Watch: OFF
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Sledování úniku: ZAP");                     // Runaway Watch: ON
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Jas světla");                               // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Motor %");                                // @ Driver %
  LSTR MSG_ERROR_TMC                      = _UxGT("TMC CHYBA SPOJENÍ");                        // TMC CONNECTION ERROR
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("VÝMĚNA FILAMENTU");                         // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("TISK POZASTAVEN");                          // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("ZAVEDENÍ FILAMENTU");                       // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("VYSUNUTÍ FILAMENTU");                       // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("MOŽNOSTI OBNOVENÍ:");                       // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Vytlačit víc");                             // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Tryska: ");                               // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Senzor filamentu");                         // Runout Sensor
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Délka mm senz.fil.");                       // Runout Dist mm
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Parkování selhalo");                        // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Kalibrace selhala");                        // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("VYBERTE FILAMENT");                         // CHOOSE FILAMENT
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");                                      // MMU
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Aktual. MMU firmware!");                    // Update MMU Firmware!
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU potř. pozornost.");                     // MMU Needs Attention.
  LSTR MSG_MMU2_RESUME                    = _UxGT("Obnovit tisk");                             // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Obnovování...");                            // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Zavést filament");                          // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Zavést všechny");                           // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Zavést do trysky");                         // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Vysunout filament");                        // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Vysun. filament ~");                        // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Vytáhnout filament");                       // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Zavádění fil. %i...");                      // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Vytahování fil. ...");                      // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Vysouvání fil....");                        // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("Všechny");                                  // All
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filament ~");                               // Filament ~
  LSTR MSG_MMU2_RESET                     = _UxGT("Resetovat MMU");                            // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Resetování MMU...");                        // Resetting...
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Vytáhněte, klikněte");                      // Eject Recover
  LSTR MSG_MIX                            = _UxGT("Mix");                                      // Mix
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Komponenta {");                             // Component {
  LSTR MSG_MIXER                          = _UxGT("Mixér");                                    // Mixer
  LSTR MSG_GRADIENT                       = _UxGT("Přechod");                                  // Gradient
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Celý přechod");                             // Full Gradient
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Přepnout mix");                             // Toggle Mix
  LSTR MSG_CYCLE_MIX                      = _UxGT("Střídat mix");                              // Cycle Mix
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Přechod mix");                              // Gradient Mix
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Opačný přechod");                           // Reverse Gradient
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Aktivní V-nástr.");                         // Active V-tool
  LSTR MSG_START_VTOOL                    = _UxGT("Spustit V-nástr.");                         // Start V-tool
  LSTR MSG_END_VTOOL                      = _UxGT("Ukončit V-nástr.");                         // End V-tool
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias V-nástr.");                           // Alias V-tool
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Reset. V-nástr.");                          // Reset V-tools
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Uložit V-nás. mix");                        // Commit V-tool Mix
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-nástr. reset.");                          // V-tools Were Reset
  LSTR MSG_START_Z                        = _UxGT("Počáteční Z:");                             // Start Z:
  LSTR MSG_END_Z                          = _UxGT("  Koncové Z:");                             // End Z:
  LSTR MSG_GAMES                          = _UxGT("Hry");                                      // Games
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");                                 // Brickout
  LSTR MSG_INVADERS                       = _UxGT("Invaders");                                 // Invaders
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");                                    // Sn4k3
  LSTR MSG_MAZE                           = _UxGT("Bludiště");                                 // Maze
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_2_LINE("Stikněte tlač.", "pro obnovení")); // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Parkování..."));                 // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Vytlačování..."));               // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_2_LINE("Klikněte pro", "ukončení"));     // |Click to finish
  LSTR MSG_TMC_DRIVERS                    = _UxGT("TMC budiče");                               // TMC Drivers
  LSTR MSG_TMC_CURRENT                    = _UxGT("Proud budičů");                             // Driver Current
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Hybridní práh");                            // Hybrid Threshold
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Domů bez senzorů");                         // Sensorless Homing
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Režim kroků");                              // Stepping Mode
  LSTR MSG_TMC_STEALTHCHOP                = _UxGT("StealthChop");                              // StealthChop
  LSTR MSG_SERVICE_RESET                  = _UxGT("Reset");                                    // Reset
  LSTR MSG_SERVICE_IN                     = _UxGT(" za:");                                     // in:
  LSTR MSG_BACKLASH                       = _UxGT("Vůle");                                     // Backlash
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Korekce");                                  // Correction
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Vyhlazení");                                // Smoothing
}

namespace LanguageWide_cz {
  using namespace LanguageNarrow_cz;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_BED_HEATING                  = _UxGT("Zahřívání podložky");                       // Bed Heating...
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Počet tisků");                              // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Dokončeno");                                // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Celkový čas");                              // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Nejdelší tisk");                            // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Celkem vytlačeno");                         // Extruded Total
    LSTR MSG_PROGRESS_BAR_TEST            = _UxGT("Test ukaz. průběhu");                       // Progress Bar Test
    LSTR MSG_BED_COOLING                  = _UxGT("Chlazení podložky");                        // Bed Cooling...
    LSTR MSG_ACTIVE_VTOOL                 = _UxGT("Aktivní V-nástroj");                        // Active V-tool
    LSTR MSG_START_VTOOL                  = _UxGT("Spustit V-nástroj");                        // Start V-tool
    LSTR MSG_END_VTOOL                    = _UxGT("Ukončit V-nástroj");                        // End V-tool
    LSTR MSG_GRADIENT_ALIAS               = _UxGT("Alias V-nástroje");                         // Alias V-tool
    LSTR MSG_RESET_VTOOLS                 = _UxGT("Resetovat V-nástroj");                      // Reset V-tools
    LSTR MSG_COMMIT_VTOOL                 = _UxGT("Uložit V-nástroj mix");                     // Commit V-tool Mix
    LSTR MSG_VTOOLS_RESET                 = _UxGT("V-nástroj resetovat");                      // V-tools Were Reset
  #endif
}

namespace LanguageTall_cz {
  using namespace LanguageWide_cz;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Čekejte prosím", "na zahájení", "výměny filamentu")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Vložte filament", "a stiskněte", "tlačítko...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_3_LINE("Čekejte prosím", "na vysunuti", "filamentu")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_3_LINE("Čekejte prosím", "na zavedení", "filamentu")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_3_LINE("Čekejte prosím", "na pokračování", "tisku")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Čekejte prosím", "na nahřátí tr.")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Klikněte pro", "nahřátí trysky")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Stikněte tlačítko", "pro obnovení tisku")); // |Press Button|to resume print
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parkování..."));                 // |Parking...
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Vyčkejte na", "vytlačení"));     // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_3_LINE("Klikněte pro", "ukončení", "vytlačování")); // |Click to finish|filament purge
  #endif
}

namespace Language_cz {
  using namespace LanguageTall_cz;
}
