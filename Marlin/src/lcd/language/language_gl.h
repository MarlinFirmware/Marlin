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
 * Galician
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

#if HAS_SDCARD && !HAS_USB_FLASH_DRIVE
  #define MEDIA_TYPE_GL "SD"
#elif HAS_USB_FLASH_DRIVE && !HAS_SDCARD
  #define MEDIA_TYPE_GL "FD"
#else
  #define MEDIA_TYPE_GL "SD/FD"
#endif

namespace LanguageNarrow_gl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Galician");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" lista.");               // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Tarxeta inserida");                         // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Tarxeta retirada");                         // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("FinCarro");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menú principal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Apagar motores");                           // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Ir a orixe");                               // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Ir orixe X");                               // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Ir orixe Y");                               // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Ir orixe Z");                               // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Ir orixes XYZ");                            // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Prema pulsador");                           // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Seguinte punto");                           // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Fin Nivelación!");                          // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Axustar Desfases");                         // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Desfases aplicados");                       // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Prequentar ") PREHEAT_1_LABEL;              // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Prequentar ") PREHEAT_1_LABEL " ~";         // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Bico");  // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Bico ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Todo");  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Cama");  // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" conf");  // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Prequentar $");                             // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Prequentar $ ~");                           // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Preque. $ Bico");                           // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Preque. $ Bico ~");                         // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Preque. $ Todo");                           // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Preque. $ Cama");                           // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Preque. $ conf");                           // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Arrefriar");                                // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Acender");                                  // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Apagar");                                   // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extruír");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retraer");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mover eixe");                               // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Probar e Nivelar");                         // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Nivelando Cama");                           // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Nivelar Cama");                             // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Mover X");                                  // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Mover Y");                                  // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Mover Z");                                  // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Mover @");                                  // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extrusor");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extrusor *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Mover $mm");                                // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Mover $in");                                // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Mover $") LCD_STR_DEGREE;                   // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Velocidade");                               // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Cama Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Bico");                                     // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Bico ~");                                   // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Cama");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Vel. Ventilador");                          // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vel. Ventilador ~");                        // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Fluxo");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Fluxo ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Control");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Temperatura Auto.");                        // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Escolla *");                                // Select *
  LSTR MSG_ACC                            = _UxGT("Acel");                                     // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("V-viaxe min");                              // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Aceleración");                              // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Max ") STR_A _UxGT(" Accel");               // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Max ") STR_B _UxGT(" Accel");               // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Max ") STR_C _UxGT(" Accel");               // Max (STR_C) Accel
  LSTR MSG_AMAX_E                         = _UxGT("Max E Accel");                              // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Max * Accel");                              // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("A-retrac.");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-viaxe");                                  // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Pasos/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" pasos/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" pasos/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" pasos/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ pasos/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E pasos/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* pasos/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Movemento");                                // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");                                // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E en mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diam. fil.");                               // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diam. fil. *");                             // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Constraste LCD");                           // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Gardar Configuración");                     // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Cargar Configuración");                     // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Rest. Defecto");                            // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Recargar");                 // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Información");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Preparar");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Axustar");                                  // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausar impresión");                         // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Retomar impresión");                        // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Deter impresión");                          // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Tarxeta SD");                               // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Sen tarxeta SD");                           // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("En repouso...");                            // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Prema para Retomar..");                     // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impresión Cancelada");                      // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sen movemento.");                           // No Move.
  LSTR MSG_KILLED                         = _UxGT("MORTO.");                                   // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("DETIDO.");                                  // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retraer mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Cambio retra. mm");                         // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retraer V");                                // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Alzar Z mm");                               // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Recup. retra. mm");                         // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Cambio recup. mm");                         // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Recuperacion V");                           // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retracción");                          // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Cambiar Filamento");                        // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Cambiar Filamento *");                      // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Iniciar ") MEDIA_TYPE_GL;                   // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Iniciar SD");                               // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Iniciar USB");                              // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Cambiar ") MEDIA_TYPE_GL;                   // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autoarranque");                             // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda-Z fóra Cama");                        // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Desfase Sonda X");                          // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Desfase Sonda Y");                          // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Desfase Sonda Z");                          // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Desfase Sonda @");                          // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Micropaso Sonda-Z");                        // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Micropaso X");                              // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Micropaso Y");                              // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Micropaso Z");                              // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Micropaso @");                              // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Erro FinCarro");                            // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Fallo Quentando");                          // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Erro:Temp Redundante");                     // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("FUGA TÉRMICA");                             // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Erro:TEMP MÁX");                            // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Erro:TEMP MÍN");                            // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("IMPRESORA DETIDA");                         // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Debe reiniciar!");                          // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Quentando...");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Quentando cama...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibracion Delta");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrar X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrar Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrar Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrar Centro");                          // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Acerca de...");                             // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Información");                              // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Estatísticas");                             // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Placa nai");                                // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistores");                              // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrusores");                               // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baudios");                                  // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocolo");                                // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Luz da Caixa");                             // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Impresións");                               // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Completadas");                              // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Máis Longa");                               // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extruido");                                 // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp Mín");                                 // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp Máx");                                 // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("Fonte Alimentación");                       // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Forza do Motor");                           // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Escribe DAC EEPROM");                       // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Retomar traballo");                         // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Agarde..."));                    // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Introduza e click"));            // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Descargando..."));               // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Cargando..."));                  // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Retomando..."));                 // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Acender");                                  // On
  LSTR MSG_LCD_OFF                        = _UxGT("Apagar");                                   // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("IMPRESORA INCORRECTA");                     // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menú depuración");                          // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test barra progreso");                      // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Movendo...");                               // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Libre XY");                                 // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Orixe %s Primeiro");                        // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Quentando..."));                 // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_1_LINE("Prema para quentar"));           // |Click to heat
  LSTR MSG_YES                            = _UxGT("SI");                                       // YES
  LSTR MSG_NO                             = _UxGT("NON");                                      // NO
  LSTR MSG_BACK                           = _UxGT("Atrás");                                    // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Cancelando...");                            // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Erro lectura ") MEDIA_TYPE_GL;              // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("Disp. USB retirado");                       // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("Inicio USB fallido");                       // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("FinCarro SW");                              // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Axustes avanzados");                        // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Configuración");                            // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Autoaliñar Z");                             // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Compensación Altura");                      // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Preque. Personali.");                       // Preheat Custom
  LSTR MSG_LASER_MENU                     = _UxGT("Control Láser");                            // Laser Control
  LSTR MSG_LASER_POWER                    = _UxGT("Potencia Láser");                           // Laser Power
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Control Fuso");                             // Spindle Control
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Potencia Fuso");                            // Spindle Pwr
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Inverter xiro");                            // Spindle Reverse
  LSTR MSG_BED_TRAMMING                   = _UxGT("Nivelar Cantos");                           // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Seguinte Canto");                           // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor Mallado");                           // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Editar Mallado");                           // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Ed. Mallado Detida");                       // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Punto de Proba");                           // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Índice X");                                 // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Índice Y");                                 // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valor Z");                                  // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Comandos Personaliz.");                     // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 Probar Sonda");                         // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 Punto");                                // M48 Point
  LSTR MSG_M48_DEVIATION                  = _UxGT("Desviación");                               // Deviation
  LSTR MSG_IDEX_MENU                      = _UxGT("Modo IDEX");                                // IDEX Mode
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Ferramentas Compens");                      // Tool Offsets
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Estacionar");                          // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplicación");                              // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Copia Espello");                            // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Control Total");                            // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2º Bico Z");                                // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2º Bico @");                                // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Executando G29");                           // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("Ferramentas UBL");                          // UBL Tools
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Punto de inclinación");                     // Tilting Point
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Facer Malla Manual");                       // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Colocar Calzo e Medir");                    // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Medir");                                    // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Quitar e Medir Cama");                      // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Mover ao Seguinte");                        // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Temp Cama");                                // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Temp Cama");                                // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temp Bico");                                // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temp Bico");                                // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Edit. Malla Person.");                      // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Axuste Fino da Malla");                     // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Fin Edición da Malla");                     // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Crear Malla Person.");                      // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Crear Malla");                              // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Crear Malla ($)");                          // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Validar Malla ($)");                        // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Crear Malla Fría");                         // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Axustar Altura Malla");                     // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Altura");                                   // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Validar Malla");                            // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Validar Malla perso.");                     // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Quentando Cama");                       // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Quentando Bico");                       // G26 Heating Nozzle
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Traballo manual...");                       // Manual priming...
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Traballo Lonxit Fixa");                     // Fixed Length Prime
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Fin Traballo");                             // Done Priming
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Cancelado");                            // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("Saíndo de G26");                            // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Continuar Malla");                          // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Nivelación Malla");                         // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Nivelación 3Puntos");                       // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Nivelación Grid");                          // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Nivelar Malla");                            // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Puntos Laterais");                          // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Tipo de Mapa ");                            // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Gardar Mapa");                              // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Enviar ao Host");                           // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Gardar en CSV");                            // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Backup Externo");                           // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Info do UBL");                              // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Cantidade de Recheo");                      // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Recheo Manual");                            // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Recheo Intelixente");                       // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Recheo da Malla");                          // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Invalidar todo");                           // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Invalidar cercanos");                       // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Axustar Fino Todo");                        // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Axustar Fino Cerc");                        // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Almacenamento Malla");                      // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Rañura Memoria");                           // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Cargar Malla Cama");                        // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Gardar Malla Cama");                        // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Malla %i Cargada");                         // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Malla %i Gardada");                         // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Sen Gardar");                               // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Erro: Gardadado UBL");                      // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Erro: Recuperación UBL");                   // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Desfase de Z: ");                           // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Desfase de Z Detido");                      // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL Paso a Paso");                          // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1. Crear Malla Fría");                      // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2. Recheo Intelixente");                    // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3. Validar Malla");                         // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4. Axustar Fino Todo");                     // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5. Validar Malla");                         // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6. Axustar Fino Todo");                     // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7. Gardar Malla Cama");                     // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("Control LED");                              // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Luces");                                    // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Luce #{");                                  // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Axustes Luz");                              // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Vermello");                                 // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Laranxa");                                  // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Amarelo");                                  // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Verde");                                    // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Azul");                                     // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Índigo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Violeta");                                  // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Branco");                                   // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Por defecto");                              // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Luces personalizadas");                     // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Intensidade Vermello");                     // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Intensidade Verde");                        // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Intensidade Azul");                         // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Intensidade Branco");                       // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Brillo");                                   // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Bico moi frío");                            // Hotend too cold
  LSTR MSG_CHAMBER                        = _UxGT("Cámara");                                   // Enclosure
  LSTR MSG_STORED_FAN_N                   = _UxGT("Ventilador Mem. ~");                        // Stored Fan ~
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Vel. Vent. Extra");                         // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Vel. Vent. Extra ~");                       // Extra Fan Speed ~
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("Auto-Sint. PID");                           // PID Autotune
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("Auto-Sint. PID *");                         // Autotune * PID
  LSTR MSG_JERK                           = _UxGT("Jerk");                                     // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("Max ") STR_A _UxGT(" Jerk");                // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("Max ") STR_B _UxGT(" Jerk");                // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("Max ") STR_C _UxGT(" Jerk");                // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("Max @ Jerk");                               // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Max E Jerk");                               // Max E Jerk
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Desvío Unión");                             // Junction Dev
  LSTR MSG_MAX_SPEED                      = _UxGT("Max Velocidade");                           // Max Speed (mm/s)
  LSTR MSG_VMAX_A                         = _UxGT("Max ") STR_A _UxGT(" Speed");               // Max (STR_A) Speed
  LSTR MSG_VMAX_B                         = _UxGT("Max ") STR_B _UxGT(" Speed");               // Max (STR_B) Speed
  LSTR MSG_VMAX_C                         = _UxGT("Max ") STR_C _UxGT(" Speed");               // Max (STR_C) Speed
  LSTR MSG_VMAX_N                         = _UxGT("Max @ Speed");                              // Max @ Speed
  LSTR MSG_VMAX_E                         = _UxGT("Max E Speed");                              // Max E Speed
  LSTR MSG_VMAX_EN                        = _UxGT("Max * Speed");                              // Max * Speed
  LSTR MSG_VMIN                           = _UxGT("Vmin");                                     // Min Velocity
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Descarga mm");                              // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Carga mm");                                 // Load mm
  LSTR MSG_ADVANCE_K                      = _UxGT("Avance K");                                 // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Avance K *");                               // Advance K *
  LSTR MSG_INIT_EEPROM                    = _UxGT("Inicializar EEPROM");                       // Initialize EEPROM
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Actualizar ") MEDIA_TYPE_GL;                // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Reiniciar Impresora");                      // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Comezar impresión");                        // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Seguinte");                                 // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Comezar");                                  // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("Deter");                                    // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Imprimir");                                 // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Cancelar");                                 // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("Listo");                                    // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Atrás");                                    // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Proceder");                                 // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Imprimindo Obxecto");                       // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Cancelar Obxecto");                         // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Cancelar Obxecto {");                       // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Recuperar Impresión");                      // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Impresión Pausada");                        // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Imprimindo...");                            // Printing...
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");                                // S UnRet V
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Lonxitude Retracción");                     // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Lonxitude de Purga");                       // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Cambiar Ferramenta");                       // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Levantar Z");                               // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Velocidade prim.");                         // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Vel. de Retracción");                       // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Bico Standby");                             // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Cargar Filamento");                         // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Cargar Filamento *");                       // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Descargar Filamento");                      // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Descargar Filamento *");                    // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Descargar Todo");                           // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Lanzar ") MEDIA_TYPE_GL;                    // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Factor de Desviación");                     // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Auto-Test");                                // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Recoller");                                 // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Estender");                                 // Deploy
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("Modo Software");                            // SW-Mode
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("Modo 5V");                                  // 5V-Mode
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("Modo OD");                                  // OD-Mode
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Modo Almacenar");                           // Mode-Store
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Axustar BLTouch a 5V");                     // Set BLTouch to 5V
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Axustar BLTouch a OD");                     // Set BLTouch to OD
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Modo de Informe");                          // Report Drain
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("PERIGO: Unha mala configuración pode producir daños! Proceder igualmente?"); // DANGER: Bad settings can cause damage! Proceed anyway?
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");                                  // TouchMI
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Iniciar TouchMI");                          // Init TouchMI
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test de Desfase Z");                        // Z Offset Test
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Gardar");                                   // Save
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Estender TouchMI");                         // Deploy TouchMI
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Estender Sonda Z");                         // Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Recoller Sonda Z");                         // Stow Z-Probe
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");                                    // Total
  LSTR MSG_COOLING                        = _UxGT("Arrefriando...");                           // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Enfriando Cama...");                        // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Quentando Cámara...");                      // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Arrefriando Cámara...");                    // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Configuración Delta");                      // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto Calibración");                         // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Barra Diagonal");                           // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Altura");                                   // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Radio");                                    // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("Nivelación 3puntos");                       // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Nivelación Lineal");                        // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Nivelación Bilineal");                      // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Nivelación UBL");                           // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Nivelación en Malla");                      // Mesh Leveling
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Reloxo Traballo: OFF");                     // Runaway Watch: OFF
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Reloxo Traballo: ON");                      // Runaway Watch: ON
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Brillo Luces");                             // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driver %");                               // @ Driver %
  LSTR MSG_ERROR_TMC                      = _UxGT("ERRO CONEX. TMC");                          // TMC CONNECTION ERROR
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("CAMBIAR FILAMENTO");                        // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("IMPRESIÓN PAUSADA");                        // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("CARGAR FILAMENTO");                         // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("DESCARGAR FILAMENTO");                      // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("OPCIÓN DE RETOMAR:");                       // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Purgar máis");                              // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Bico: ");                                 // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Sensor Filamento");                         // Runout Sensor
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Dist mm Sensor Fil");                       // Runout Dist mm
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Fallo ao ir á Orixe");                      // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Fallo ao Sondar");                          // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("ESCOLLE FILAMENTO");                        // CHOOSE FILAMENT
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");                                      // MMU
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Actualizar FW MMU!");                       // Update MMU Firmware!
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU Precisa Atención.");                    // MMU Needs Attention.
  LSTR MSG_MMU2_RESUME                    = _UxGT("Retomar impr.");                            // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Retomando...");                             // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Cargar Filamento");                         // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Cargar Todo");                              // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Cargar até bico");                          // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Expulsar Filamento");                       // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Expulsar Filamento ~");                     // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Descargar Filamento");                      // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Cargando Fil. %i...");                      // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Expulsando Fil. ...");                      // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Descargando Fil...");                       // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("Todo");                                     // All
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filamento ~");                              // Filament ~
  LSTR MSG_MMU2_RESET                     = _UxGT("Reiniciar MMU");                            // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Reiniciando MMU...");                       // Resetting...
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Expulsar, premer");                         // Eject Recover
  LSTR MSG_MIX                            = _UxGT("Mestura");                                  // Mix
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Compoñente {");                             // Component {
  LSTR MSG_MIXER                          = _UxGT("Mesturadora");                              // Mixer
  LSTR MSG_GRADIENT                       = _UxGT("Degradado");                                // Gradient
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Degradado Total");                          // Full Gradient
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Mestura Conmutada");                        // Toggle Mix
  LSTR MSG_CYCLE_MIX                      = _UxGT("Mestura Cíclica");                          // Cycle Mix
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Mestura de Degradado");                     // Gradient Mix
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Degradado Inverso");                        // Reverse Gradient
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Activar Ferr-V");                           // Active V-tool
  LSTR MSG_START_VTOOL                    = _UxGT("Inicio Ferr-V");                            // Start V-tool
  LSTR MSG_END_VTOOL                      = _UxGT("  Fin Ferr-V");                             // End V-tool
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias Ferr-V");                             // Alias V-tool
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Reiniciar Ferr-V");                         // Reset V-tools
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Commit mest. Ferr-V");                      // Commit V-tool Mix
  LSTR MSG_VTOOLS_RESET                   = _UxGT("Ferr-V reiniciadas");                       // V-tools Were Reset
  LSTR MSG_START_Z                        = _UxGT("Inicio Z:");                                // Start Z:
  LSTR MSG_END_Z                          = _UxGT("  Fin Z:");                                 // End Z:
  LSTR MSG_GAMES                          = _UxGT("Xogos");                                    // Games
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");                                 // Brickout
  LSTR MSG_INVADERS                       = _UxGT("Invaders");                                 // Invaders
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");                                    // Sn4k3
  LSTR MSG_MAZE                           = _UxGT("Labirinto");                                // Maze
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_1_LINE("Premer para continuar"));        // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Estacionando..."));              // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Purgando..."));                  // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_1_LINE("Prema para finalizar"));         // |Click to finish
  LSTR MSG_TMC_DRIVERS                    = _UxGT("Controladores TMC");                        // TMC Drivers
  LSTR MSG_TMC_CURRENT                    = _UxGT("Controlador Actual");                       // Driver Current
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Limiar Hibrido");                           // Hybrid Threshold
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Orixe sen Sensores");                       // Sensorless Homing
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Modo de pasos");                            // Stepping Mode
  LSTR MSG_TMC_STEALTHCHOP                = _UxGT("StealthChop");                              // StealthChop
  LSTR MSG_SERVICE_RESET                  = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_SERVICE_IN                     = _UxGT(" dentro:");                                 // in:
  LSTR MSG_BACKLASH                       = _UxGT("Reacción");                                 // Backlash
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Corrección");                               // Correction
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Suavizado");                                // Smoothing
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Desbord. Subch.");                          // Subcall Overflow
  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frecuencia");                               // Frequency
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Bico Estacionado");                         // Nozzle Parked
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Controlador Vent.");                        // Controller Fan
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Velocidade Repouso");                       // Idle Speed
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Modo Auto");                                // Auto Mode
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Velocidade Activa");                        // Active Speed
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Tempo Repouso");                            // Idle Period
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("Fin Auto-Sint PID");                        // PID tuning done
  LSTR MSG_PID_BAD_HEATER_ID              = _UxGT("Auto-Sint. fallida! Extrusor danado.");     // Autotune failed! Bad extruder.
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Auto-Sint. fallida! Temperatura moi alta."); // Autotune failed! Temperature too high.
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Auto-Sint. fallida! Tempo excedido.");      // Autotune failed! Timeout.
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Frecuencia max");                           // XY Freq Limit
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Avance min");                               // Min FR Factor
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("Erro: CRC EEPROM");                         // Err: EEPROM CRC
  LSTR MSG_ERR_EEPROM_SIZE                = _UxGT("Erro: Tamaño EEPROM");                      // Err: EEPROM Size
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("Erro: Versión EEPROM");                     // Err: EEPROM Version
  LSTR MSG_SETTINGS_STORED                = _UxGT("Config Gardada");                           // Settings Stored
  LSTR MSG_PAUSING                        = _UxGT("Pausando...");                              // Pausing...
  LSTR MSG_PRINT_DONE                     = _UxGT("Fin Impresión");                            // Print Done
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Cambio Extra");                             // Swap Extra
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Extrusor Est.");                            // Park Head
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Vel. Recuperación");                        // Recover Speed
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Vel. Ventilador");                          // Fan Speed
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Tempo Ventilador");                         // Fan Time
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto ON");                                  // Auto ON
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto OFF");                                 // Auto OFF
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Cambio Ferramenta");                        // Tool Migration
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Cambio Automático");                        // Auto-migration
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Último Extrusor");                          // Last Extruder
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Cambio a *");                               // Migrate to *
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Desfases Sonda");                           // Probe Offsets
  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Nivel Eixe X");                             // Level X Axis
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Auto Calibrar");                            // Auto Calibrate
  LSTR MSG_HEATER_TIMEOUT                 = _UxGT("Tempo exc. Quent.");                        // Heater Timeout
  LSTR MSG_REHEAT                         = _UxGT("Requentar");                                // Reheat
  LSTR MSG_REHEATING                      = _UxGT("Requentando...");                           // Reheating...
}

namespace LanguageWide_gl {
  using namespace LanguageNarrow_gl;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Total Impresións");                         // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Completadas");                              // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tempo Total Imp.");                         // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Impresión máis longa");                     // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total Extruído");                           // Extruded Total
  #endif
}

namespace LanguageTall_gl {
  using namespace LanguageWide_gl;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Agarde para", "comezar cambio", "de filamento")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Introduza o", "filamento e", "faga click")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_3_LINE("Agarde pola", "descarga do", "filamento")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_3_LINE("Agarde pola", "carga do", "filamento")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_3_LINE("Agarde a que", "se retome", "a impresión")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Quentando bico", "Agarde, por favor...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Prema o botón para", "quentar o bico")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Preme o botón para", "continuar impresión")); // |Press Button|to resume print
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Agarde para", "purgar o filamento")); // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Prema para finalizar", "a purga do filamen.")); // |Click to finish|filament purge
  #endif
}

namespace Language_gl {
  using namespace LanguageTall_gl;
}
