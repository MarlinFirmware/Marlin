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
 * Aragonese
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

namespace LanguageNarrow_an {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Aragonese");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" parada.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Tarcheta mesa");                            // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Tarcheta sacada");                          // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu prencipal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Amortar motors");                           // Disable Steppers
  LSTR MSG_HOMING                         = _UxGT("Orichen");                                  // Homing
  LSTR MSG_AUTO_HOME                      = _UxGT("Levar a l'orichen");                        // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Orichen X");                                // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Orichen Y");                                // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Orichen Z");                                // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Orichen XYZ");                              // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Encetar (pretar)");                         // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Vinient punto");                            // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Nivelacion feita!");                        // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Achustar desfases");                        // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Desfase aplicau");                          // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Precalentar ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Precalentar ") PREHEAT_1_LABEL " ~";        // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Boquilla"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Boquilla ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Tot"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Base"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Conf"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Precalentar $");                            // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Precalentar $ ~");                          // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Precal. $ Boquilla");                       // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Precal. $ Boquilla ~");                     // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Precalentar $ Tot");                        // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Precalentar $ Base");                       // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Precalentar $ Conf");                       // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Enfriar");                                  // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Enchegar Fuent");                           // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Amortar Fuent");                            // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extruir");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retraer");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mover Eixes");                              // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sondar y Nivelar");                         // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Nivelar base");                             // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Nivelar base");                             // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Mover X");                                  // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Mover Y");                                  // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Mover Z");                                  // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Mover @");                                  // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extrusor");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extrusor *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Mover $mm");                                // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Mover $in");                                // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Mover $") LCD_STR_DEGREE;                   // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Velocidat");                                // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Base Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Boquilla");                                 // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Boquilla ~");                               // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Base");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Ixoriador");                                // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Ixoriador ~");                              // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Fluxo");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Fluxo ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Control");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Temperatura Auto.");                        // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Trigar *");                                 // Select *
  LSTR MSG_ACC                            = _UxGT("Aceleracion");                              // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vel. viache min");                          // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Accel");                                    // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Acel. max ") STR_A;                         // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Acel. max ") STR_B;                         // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Acel. max ") STR_C;                         // Max (STR_C) Accel
  LSTR MSG_AMAX_N                         = _UxGT("Acel. max @");                              // Max @ Accel
  LSTR MSG_AMAX_E                         = _UxGT("Acel. max E");                              // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Acel. max *");                              // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("Acel. retrac.");                            // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("Acel. Viaje");                              // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Trangos/mm");                               // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" trangos/mm");                        // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" trangos/mm");                        // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" trangos/mm");                        // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ trangos/mm");                             // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E trangos/mm");                             // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* trangos/mm");                             // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Movimiento");                               // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");                                // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");                                // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");                              // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Contraste");                                // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Alzar memoria");                            // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Cargar memoria");                           // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Restaurar memoria");                        // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Tornar a cargar");          // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Informacion");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Preparar");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Achustar");                                 // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausar impresion");                         // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Contin. impresion");                        // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Detener Impresion");                        // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Menu de SD");                               // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("No i hai tarcheta");                        // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Reposo...");                                // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Aguardand ordines");                        // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impres. cancelada");                        // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sin movimiento");                           // No Move.
  LSTR MSG_KILLED                         = _UxGT("Aturada d'emerch.");                        // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("Aturada.");                                 // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retraer mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Swap Retraer mm");                          // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retraer  F");                               // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Devantar mm");                              // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("DesRet mm");                                // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Swap DesRet mm");                           // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("DesRet F");                                 // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Retraccion auto.");                         // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Cambear filamento");                        // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Cambear filamento *");                      // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Encetan. media");                           // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Encetan. SD");                              // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Encetan. USB");                             // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Cambiar tarcheta");                         // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Inicio automatico");                        // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z fuera");                            // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset BLTouch");                            // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Desfase X");                                // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Desfase Y");                                // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Desfase Z");                                // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Desfase @");                                // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Probe Z con pas");                          // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Micropaso X");                              // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Micropaso Y");                              // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Micropaso Z");                              // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Micropaso @");                              // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Cancelado - Endstop");                      // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Error en calentar");                        // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Error temp adicional");                     // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("Error de temperatura");                     // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Error Temp Max");                           // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Error Temp Min");                           // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("IMPRESORA ATURADA");                        // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Per favor reinic.");                        // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Calentando...");                            // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Calentando base...");                       // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibracion Delta");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrar X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrar Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrar Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrar Centro");                          // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Inf. Impresora");                           // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Inf. Impresora");                           // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Estadisticas Imp.");                        // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Inf. Controlador");                         // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistors");                               // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrusors");                                // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baudios");                                  // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocolo");                                // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Luz");                                      // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Impresions");                               // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Completadas");                              // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Mas larga");                                // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extrusion");                                // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temperatura menima");                       // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temperatura maxima");                       // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("Fuente de aliment");                        // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Fuerza d'o driver");                        // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Escri. DAC EEPROM");                        // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Resumir imp.");                             // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_2_LINE("Aguardand iniciar", "d'o fil. cambear")); // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_2_LINE("Meta o filamento", "y prete lo boton")); // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_2_LINE("Aguardando a", "expulsar filament")); // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_2_LINE("Aguardando a", "cargar filamento")); // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_2_LINE("Aguardando impre.", "pa continar")); // |Resuming...
}

namespace LanguageWide_an {
  using namespace LanguageNarrow_an;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Conteo de impresion");                      // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Completadas");                              // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tiempo total d'imp.");                      // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Impresion mas larga");                      // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total d'extrusion");                        // Extruded Total
  #endif
}

namespace LanguageTall_an {
  using namespace LanguageWide_an;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Aguardand iniciar", "d'o filamento", "cambear")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Meta o filamento", "y prete lo boton", "pa continar...")); // |Insert filament|and press button|to continue
  #endif
}

namespace Language_an {
  using namespace LanguageTall_an;
}
