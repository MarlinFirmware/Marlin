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
 * Basque-Euskera
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

namespace LanguageNarrow_eu {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Basque-Euskera");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" prest.");               // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Txartela sartuta");                         // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Txartela kenduta");                         // (MEDIA_TYPE_EN) Removed
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu nagusia");                             // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Itzali motoreak");                          // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Hasierara joan");                           // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("X jatorrira");                              // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Y jatorrira");                              // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Z jatorrira");                              // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("XYZ hasieraratzen");                        // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klik egin hasteko");                        // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Hurrengo Puntua");                          // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Berdintzea eginda");                        // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Etxe. offset eza.");                        // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offsetak ezarrita");                        // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Berotu ") PREHEAT_1_LABEL;                  // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Berotu ") PREHEAT_1_LABEL " ~";             // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia");  // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Guztia"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Ohea");   // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Ezarp."); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Berotu $");                                 // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Berotu $ ~");                               // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Berotu $ Amaia");                           // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Berotu $ Amaia ~");                         // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Berotu $ Guztia");                          // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Berotu $ Ohea");                            // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Berotu $ Ezarp.");                          // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Hoztu");                                    // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Energia piztu");                            // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Energia itzali");                           // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Estruitu");                                 // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Atzera eragin");                            // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Ardatzak mugitu");                          // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Proba eta Berdindu");                       // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Ohe berdinketa");                           // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Ohea berdindu");                            // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Mugitu X");                                 // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Mugitu Y");                                 // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Mugitu Z");                                 // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Mugitu @");                                 // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Estrusorea");                               // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Estrusorea *");                             // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Mugitu $mm");                               // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Mugitu $in");                               // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Mugitu $") LCD_STR_DEGREE;                  // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Abiadura");                                 // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Z Ohea");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Pita");                                     // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Pita ~");                                   // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Ohea");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Haizagailu abiadura");                      // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Haizagailu abiadura ~");                    // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Fluxua");                                   // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Fluxua ~");                                 // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Kontrola");                                 // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Auto tenperatura");                         // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Aukeratu *");                               // Select *
  LSTR MSG_ACC                            = _UxGT("Azelerazioa");                              // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("VBidaia min");                              // Min Travel Speed
  LSTR MSG_A_RETRACT                      = _UxGT("A-retrakt");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-bidaia");                                 // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Pausoak/mm");                               // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" pausoak/mm");                        // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" pausoak/mm");                        // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" pausoak/mm");                        // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ pausoak/mm");                             // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E pausoak/mm");                             // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* pausoak/mm");                             // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Tenperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Mugimendua");                               // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Harizpia");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E mm3-tan");                                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Hariz. Dia.");                              // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Hariz. Dia. *");                            // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCD kontrastea");                           // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Gorde memoria");                            // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Kargatu memoria");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Larri. berriz.");                           // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Berriz kargatu");           // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Pantaila info");                            // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Prestatu");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Doitu");                                    // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausatu inprimak.");                        // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Jarraitu inprima.");                        // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Gelditu inprima.");                         // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("SD-tik inprimatu");                         // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Ez dago SD-rik");                           // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Lo egin...");                               // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Aginduak zain...");                         // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Inprim. deusezta.");                        // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Mugimendu gabe.");                          // No Move.
  LSTR MSG_KILLED                         = _UxGT("AKABATUTA. ");                              // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("GELDITUTA. ");                              // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Atzera egin mm");                           // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Swap Atzera mm");                           // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Atzera egin V");                            // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Igo mm");                                   // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Atzera egin mm");                           // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Swap Atzera mm");                           // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Atzera egin V");                            // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Atzera egin");                              // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Aldatu harizpia");                          // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Aldatu harizpia *");                        // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Hasieratu SD-a");                           // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Hasieratu SD-a");                           // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Hasieratu USB-a");                          // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Aldatu txartela");                          // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Auto hasiera");                             // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z zunda kanpora");                          // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("BLTouch berrabia.");                        // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X Konpentsatu");                            // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y Konpentsatu");                            // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z Konpentsatu");                            // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("@ Konpentsatu");                            // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Z pausoz egokitu");                         // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Mikro-urratsa X");                          // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Mikro-urratsa Y");                          // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Mikro-urratsa Z");                          // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Mikro-urratsa @");                          // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Endstop deusezta.");                        // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Err: Beroketa");                            // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err: Tenperatura");                         // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("TENP. KONTROL EZA");                        // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: Tenp Maximoa");                        // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: Tenp Minimoa");                        // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("INPRIMA. GELDIRIK");                        // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Berrabia. Mesedez");                        // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Berotzen...");                              // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Ohea Berotzen...");                         // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibraketa");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibratu X");                              // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibratu Y");                              // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibratu Z");                              // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibratu Zentrua");                        // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Inprimagailu Inf.");                        // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Inprimagailu Inf.");                        // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Inprima. estatis.");                        // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Txartelaren Info.");                        // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistoreak");                             // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Estrusoreak");                              // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baudioak");                                 // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokoloa");                               // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Kabina Argia");                             // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Inprimatze");                               // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Burututa");                                 // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Guztira");                                  // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Luzeena");                                  // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Estrusio");                                 // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Tenp. Minimoa");                            // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Tenp. Maximoa");                            // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("Elikadura-iturria");                        // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Driver-aren potentzia");                    // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Idatzi DAC EEPROM");                        // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Inprima. jarraitu");                        // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Mesedez, itxaron..."));          // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Sartu eta click egin"));         // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Deskargatzen..."));              // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Kargatzen..."));                 // |Loading...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fakt");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Inprimagailu okerra");                      // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Arazketa Menua");                           // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Prog. Barra Proba");                        // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Mugitzen...");                              // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Askatu XY");                                // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Etxera %s lehenengo");                      // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Berotzen..."));                  // |Heating...
  LSTR MSG_BACK                           = _UxGT("Atzera");                                   // Back
  LSTR MSG_BED_TRAMMING                   = _UxGT("Ertzak berdindu");                          // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Hurrengo ertza");                           // Next Corner
  LSTR MSG_EDIT_MESH                      = _UxGT("Sarea editatu");                            // Edit Mesh
  LSTR MSG_UBL_DOING_G29                  = _UxGT("G29 exekutatzen");                          // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("UBL Tresnak");                              // UBL Tools
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Sarea eskuz sortu");                        // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Neurtu");                                   // Measure
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Ohearen tenperatura");                      // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Bed Temp");                                 // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Mutur beroaren tenp.");                     // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Hotend Temp");                              // Hotend Temp
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Sarea editatzea eginda");                   // Done Editing Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Sarea sortu");                              // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("$ sarea sortu");                            // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("$ sarea balioetsi");                        // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Sare hotza sortu");                         // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Sarearen altuera doitu");                   // Adjust Mesh Height
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Sarea balioetsi");                          // Validate Mesh
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Ohe sarea balioetsi");                      // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Sare berdinketa");                          // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("3 puntuko berdinketa");                     // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Lauki-sare berdinketa");                    // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Sarea berdindu");                           // Level Mesh
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Mapa mota");                                // Map Type
  LSTR MSG_LED_CONTROL                    = _UxGT("LED ezarpenak");                            // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Argiak");                                   // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Argi #{");                                  // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Argi aurrehautaketak");                     // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Gorria");                                   // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Laranja");                                  // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Horia");                                    // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Berdea");                                   // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Urdina");                                   // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Bioleta");                                  // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Zuria");                                    // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Lehenetsia");                               // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Argi pertsonalizatuak");                    // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Intentsitate gorria");                      // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Intentsitate berdea");                      // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Intentsitate urdina");                      // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Intentsitate zuria");                       // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Distira");                                  // Brightness
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Haiz.gehig. abiadura");                     // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Haiz.gehig. abiadura ~");                   // Extra Fan Speed ~
  LSTR MSG_JERK                           = _UxGT("Astindua");                                 // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("V") STR_A _UxGT("-astindua");               // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("V") STR_B _UxGT("-astindua");               // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("V") STR_C _UxGT("-astindua");               // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("V@-astindua");                              // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Ve-astindua");                              // Max E Jerk
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Deskargatu mm");                            // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Kargatu mm");                               // Load mm
  LSTR MSG_ADVANCE_K                      = _UxGT("K Aurrerapena");                            // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("K Aurrerapena *");                          // Advance K *
  LSTR MSG_INIT_EEPROM                    = _UxGT("EEPROM-a hasieratu");                       // Initialize EEPROM
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Inprim. geldi.");                           // Print Paused
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");                                // S UnRet V
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Harizpia kargatu");                         // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Harizpia kargatu *");                       // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Harizpia deskargatu");                      // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Harizpia deskargatu *");                    // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Erabat deskargatu");                        // Unload All
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Okertze faktorea");                         // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch AutoProba");                        // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("BLTouch igo/jaso");                         // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("BLTouch jaitsi/luzatu");                    // Deploy
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Delta ezarpenak");                          // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto Kalibraketa");                         // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Barra diagonala");                          // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Altuera");                                  // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Erradioa");                                 // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("3 puntuko berdinketa");                     // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Berdinketa lineala");                       // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Berdinketa bilinearra");                    // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Unified Bed Leveling");                     // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Sare berdinketa");                          // Mesh Leveling
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Argiaren Distira");                         // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driver %");                               // @ Driver %
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("HARIZPIA ALDATU");                          // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("HARIZPIA KARGATU");                         // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("HARIZPIA DESKARGATU");                      // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("ALDAKETA AUKERAK:");                        // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Pita: ");                                 // Nozzle:
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Hasi. huts egin du");                       // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Neurketak huts egin du");                   // Probing Failed
}

namespace LanguageWide_eu {
  using namespace LanguageNarrow_eu;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Inprim. Zenbaketa");                        // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Burututa");                                 // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Inprim. denbora");                          // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Imprimatze luzeena");                       // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Estruituta guztira");                       // Extruded Total
  #endif
}

namespace LanguageTall_eu {
  using namespace LanguageWide_eu;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_eu {
  using namespace LanguageTall_eu;
}
