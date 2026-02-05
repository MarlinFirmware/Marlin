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
 * Spanish
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

#if HAS_SDCARD && !HAS_USB_FLASH_DRIVE
  #define MEDIA_TYPE_ES "SD"
#elif HAS_USB_FLASH_DRIVE && !HAS_SDCARD
  #define MEDIA_TYPE_ES "USB"
#else
  #define MEDIA_TYPE_ES "SD/FD"
#endif

namespace LanguageNarrow_es {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Spanish");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" Lista");                // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = MEDIA_TYPE_ES _UxGT(" insertado");                 // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = MEDIA_TYPE_ES _UxGT(" retirado");                  // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menú principal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Apagar motores");                           // Disable Steppers
  LSTR MSG_HOMING                         = _UxGT("Origen");                                   // Homing
  LSTR MSG_AUTO_HOME                      = _UxGT("Llevar al origen");                         // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Origen X");                                 // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Origen Y");                                 // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Origen Z");                                 // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Origen XYZ");                               // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Pulsar para comenzar");                     // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Siguiente punto");                          // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("¡Nivelación lista!");                       // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ajustar desfases");                         // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Desfase aplicada");                         // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Precal. ") PREHEAT_1_LABEL;                 // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Precal. ") PREHEAT_1_LABEL " ~";            // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Fusor"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Fusor ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Todo");  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Cama");  // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Ajuste"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Precal. $");                                // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Precal. $ ~");                              // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Precal. $ Fusor");                          // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Precal. $ Fusor ~");                        // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Precal. $ Todo");                           // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Precal. $ Cama");                           // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Precal. $ Ajuste");                         // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Enfriar");                                  // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Encender Fuente");                          // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Apagar Fuente");                            // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extruir");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retraer");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mover ejes");                               // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sondear y Nivelar");                        // Probe and Level
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
  LSTR MSG_SPEED                          = _UxGT("Velocidad");                                // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Cama Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Boquilla");                                 // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Boquilla ~");                               // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Cama");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Ventilador");                               // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Ventilador ~");                             // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Flujo");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Flujo ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Control");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Temp. Autom.");                             // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Seleccionar *");                            // Select *
  LSTR MSG_ACC                            = _UxGT("Aceleración");                              // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vel. viaje min");                           // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Acceleración");                             // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Acel. max") STR_A;                          // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Acel. max") STR_B;                          // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Acel. max") STR_C;                          // Max (STR_C) Accel
  LSTR MSG_AMAX_N                         = _UxGT("Acel. max@");                               // Max @ Accel
  LSTR MSG_AMAX_E                         = _UxGT("Acel. maxE");                               // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Acel. max *");                              // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("Acel. retrac.");                            // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("Acel. Viaje");                              // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Pasos/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" pasos/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" pasos/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" pasos/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ pasos/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E pasos/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* pasos/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Movimiento");                               // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");                                // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E en mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diámetro Fil.");                            // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diámetro Fil. *");                          // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Contraste LCD");                            // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Guardar EEPROM");                           // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Cargar EEPROM");                            // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Rest. fábrica");                            // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Recargar");                 // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Pantalla de Inf.");                         // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Preparar");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Ajustar");                                  // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausar impresión");                         // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Reanudar impresión");                       // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Detener impresión");                        // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Imprim. desde ") MEDIA_TYPE_ES;             // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = MEDIA_TYPE_ES _UxGT(" no presente");               // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Reposo...");                                // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Pulsar para Reanudar");                     // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impresión cancelada");                      // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sin movimiento");                           // No Move.
  LSTR MSG_KILLED                         = _UxGT("MUERTA");                                   // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("DETENIDA");                                 // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retraer mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Interc. Retraer mm");                       // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retraer  V");                               // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Levantar mm");                              // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("DesRet mm");                                // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Interc. DesRet mm");                        // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("DesRet V");                                 // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Retracción Auto.");                         // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Cambiar filamento");                        // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Cambiar filamento *");                      // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Iniciar ") MEDIA_TYPE_ES;                   // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Iniciar SD");                               // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Iniciar USB");                              // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Cambiar ") MEDIA_TYPE_ES;                   // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Inicio automático");                        // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z fuera cama");                       // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Desf. Sonda X");                            // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Desf. Sonda Y");                            // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Desf. Sonda Z");                            // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Desf. Sonda @");                            // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Ajuste Z al paso");                         // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Micropaso X");                              // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Micropaso Y");                              // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Micropaso Z");                              // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Micropaso @");                              // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Cancelado - Endstop");                      // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Calent. fallido");                          // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err: TEMP. REDUN.");                        // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("FUGA TÉRMICA");                             // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err:TEMP. MÁX");                            // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err:TEMP. MIN");                            // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("IMPRESORA DETENIDA");                       // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Por favor, reinicie");                      // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Calentando...");                            // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Calentando Cama...");                       // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibración Delta");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrar X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrar Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrar Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrar Centro");                          // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Info. Impresora");                          // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info. Impresora");                          // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Estadísticas Imp.");                        // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info. Controlador");                        // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistores");                              // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrusores");                               // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baudios");                                  // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocolo");                                // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Luz cabina");                               // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Impresiones");                              // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Completadas");                              // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total");                                    // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Más larga");                                // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Extruido");                                 // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp. Mínima");                             // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp. Máxima");                             // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("F. Aliment.");                              // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Fuerza de empuje");                         // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Escribe DAC EEPROM");                       // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Continuar imp.");                           // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Por Favor espere..."));          // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Inserte y Pulse"));              // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Liberando..."));                 // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Cargando..."));                  // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Reanudando..."));                // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Factor");          // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Enc");                                      // On
  LSTR MSG_LCD_OFF                        = _UxGT("Apg");                                      // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Impresora incorrecta");                     // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menú depuración");                          // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Prob. barra progreso");                     // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Moviendo...");                              // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Libre XY");                                 // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Origen %s Prim.");                          // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Calentando..."));                // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_1_LINE("Pulse para Calentar"));          // |Click to heat
  LSTR MSG_YES                            = _UxGT("SI");                                       // YES
  LSTR MSG_NO                             = _UxGT("NO");                                       // NO
  LSTR MSG_BACK                           = _UxGT("Atrás");                                    // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Cancelando...");                            // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Error lectura ") MEDIA_TYPE_ES;             // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("Disp. USB retirado");                       // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("Inicio USB fallido");                       // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Soft Endstops");                            // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Ajustes avanzados");                        // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Configuración");                            // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto alineado Z");                          // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Compen. Altura");                           // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Precal. manual");                           // Preheat Custom
  LSTR MSG_LASER_MENU                     = _UxGT("Control Láser");                            // Laser Control
  LSTR MSG_LASER_POWER                    = _UxGT("Potencia Láser");                           // Laser Power
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Control Mandrino");                         // Spindle Control
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Potencia Mandrino");                        // Spindle Pwr
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Invertir giro");                            // Spindle Reverse
  LSTR MSG_BED_TRAMMING                   = _UxGT("Recorrido Cama");                           // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Siguente Esquina");                         // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor Mallado");                           // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Editar Mallado");                           // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Ed. Mallado parada");                       // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Sondear Punto");                            // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Índice X");                                 // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Índice Y");                                 // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valor Z");                                  // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Com. Personalizados");                      // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 Probar Sonda");                         // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 Punto");                                // M48 Point
  LSTR MSG_M48_DEVIATION                  = _UxGT("Desviación");                               // Deviation
  LSTR MSG_IDEX_MENU                      = _UxGT("Modo IDEX");                                // IDEX Mode
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Desfase Herramienta");                      // Tool Offsets
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Aparcado");                            // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplicar");                                 // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Copia Reflejada");                          // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Control Total");                            // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2ª Fusor Z");                               // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2ª Fusor @");                               // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Hacer G29");                                // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("Herramientas UBL");                         // UBL Tools
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Punto de inclinación");                     // Tilting Point
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Crear Mallado man.");                       // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Colocar cuña y Medir");                     // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Medir");                                    // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Retirar y Medir Cama");                     // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Mover al Siguente");                        // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Temp. Cama");                               // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Temp. Cama perso.");                        // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temp. Fusor");                              // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temp. Fusor perso.");                       // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Edit. Mallado perso.");                     // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Ajuste fino Mallado");                      // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Term. edici. Mallado");                     // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Crear Mallado Pers.");                      // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Crear Mallado");                            // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Crear Mallado ($)");                        // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Valid. Mall. ($)");                         // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Crear Mallado Frío");                       // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Ajustar alt. Mallado");                     // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Cantidad de altura");                       // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Valid. Mallado");                           // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Valid. Mall. perso.");                      // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Calentando Cama");                      // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Calent. Boquilla");                     // G26 Heating Nozzle
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Imprimado manual...");                      // Manual priming...
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Impri. longit. fija");                      // Fixed Length Prime
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Imprimación Lista");                        // Done Priming
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Cancelado");                            // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("Dejando G26");                              // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Contin. Mallado cama");                     // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Nivelando Mallado");                        // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Nivelando 3Puntos");                        // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Niv. Mall. cuadri");                        // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Nivel de Mallado");                         // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Puntos Laterales");                         // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Tipo de mapa ");                            // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Salida Mapa mallado");                      // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Salida para el host");                      // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Salida para CSV");                          // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Cópia de seg. ext");                        // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Salida Info. UBL");                         // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Cantidad de relleno");                      // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Relleno manual");                           // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Relleno inteligente");                      // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Mallado de relleno");                       // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Invalidar todo");                           // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Invalidar proximos");                       // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Ajustar Fino Todo");                        // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Ajustar Fino proxi.");                      // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Almacen de Mallado");                       // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Huecos memoria");                           // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Cargar Mall. cama");                        // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Guardar Mall. cama");                       // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Malla %i Cargada");                         // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Malla %i Guardada");                        // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Sin guardar");                              // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Error Guardar UBL");                        // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Error Restaurar UBL");                      // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Desfase de Z: ");                           // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Desfase de Z Parado");                      // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL Paso a Paso");                          // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1.Crear Mall. Frío");                       // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2.Relleno intelig.");                       // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3.Valid. Mallado");                         // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4.Ajustar Fino Todo");                      // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5.Valid. Mallado");                         // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6.Ajustar Fino Todo");                      // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7.Guardar Mall. cama");                     // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("Control LED");                              // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Luces");                                    // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Luce #{");                                  // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Color predefinido");                        // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Rojo");                                     // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Naranja");                                  // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Amarillo");                                 // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Verde");                                    // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Azul");                                     // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Índigo");                                   // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Violeta");                                  // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Blanco");                                   // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Por defecto");                              // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Color personalizado");                      // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Intensidad Rojo");                          // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Intensidad Verde");                         // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Intensidad Azul");                          // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Intensidad Blanco");                        // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Brillo");                                   // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Hotend muy frio");                          // Hotend too cold
  LSTR MSG_CHAMBER                        = _UxGT("Recinto");                                  // Enclosure
  LSTR MSG_STORED_FAN_N                   = _UxGT("Vent. almacenado ~");                       // Stored Fan ~
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Vel. Ext. ventil.");                        // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Vel. Ext. ventil. ~");                      // Extra Fan Speed ~
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID Auto-ajuste");                          // PID Autotune
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID Auto-ajuste *");                        // Autotune * PID
  LSTR MSG_JERK                           = _UxGT("Jerk");                                     // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("Max ") STR_A _UxGT(" Jerk");                // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("Max ") STR_B _UxGT(" Jerk");                // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("Max ") STR_C _UxGT(" Jerk");                // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("Max @ Jerk");                               // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Max E Jerk");                               // Max E Jerk
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Desvi. Unión");                             // Junction Dev
  LSTR MSG_MAX_SPEED                      = _UxGT("Max Velocidad");                            // Max Speed (mm/s)
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
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Actualizar ") MEDIA_TYPE_ES;                // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Resetear Impresora");                       // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Iniciar impresión");                        // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Siguinte");                                 // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Iniciar");                                  // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("Parar");                                    // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Imprimir");                                 // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Cancelar");                                 // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("Listo");                                    // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Retroceder");                               // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Proceder");                                 // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Imprimiendo Objeto");                       // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Cancelar Objeto");                          // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Cancelar Objeto {");                        // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Rec. Fallo electrico");                     // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Impresión Pausada");                        // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Imprimiendo...");                           // Printing...
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");                                // S UnRet V
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Inter. longitud");                          // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Purgar longitud");                          // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Cambiar Herramienta");                      // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Aumentar Z");                               // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Vel. de Cebado");                           // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Vel. de retracción");                       // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Boquilla en Espera");                       // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Cargar filamento");                         // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Cargar filamento *");                       // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Descargar filamento");                      // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Descargar fil. *");                         // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Descargar todo");                           // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Lanzar ") MEDIA_TYPE_ES;                    // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Factor de desviación");                     // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Auto-Prueba");                              // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Subir pistón");                             // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Bajar pistón");                             // Deploy
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("Modo Software");                            // SW-Mode
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("Modo 5V");                                  // 5V-Mode
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("Modo OD");                                  // OD-Mode
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Modo almacenar");                           // Mode-Store
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Poner BLTouch a 5V");                       // Set BLTouch to 5V
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Poner BLTouch a OD");                       // Set BLTouch to OD
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Informe de drenaje");                       // Report Drain
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("PELIGRO: ¡Una mala configuración puede producir daños! ¿Proceder igualmente?"); // DANGER: Bad settings can cause damage! Proceed anyway?
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");                                  // TouchMI
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Iniciar TouchMI");                          // Init TouchMI
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test de desfase Z");                        // Z Offset Test
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Guardar");                                  // Save
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Subir TouchMI");                            // Deploy TouchMI
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Subir Sonda Z");                            // Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Bajar Sonda Z");                            // Stow Z-Probe
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");                                    // Total
  LSTR MSG_COOLING                        = _UxGT("Enfriando...");                             // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Enfriando Cama...");                        // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Calentando Cámara...");                     // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Enfriando Cámara...");                      // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Configuración Delta");                      // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto Calibración");                         // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Barra Diagonal");                           // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Altura");                                   // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Radio");                                    // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("Nivelando 3puntos");                        // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Nivelando Lineal");                         // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Nivelando Bilineal");                       // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Nivelando UBL");                            // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Nivelando en Mallado");                     // Mesh Leveling
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Vig. Fuga Térm.: OFF");                     // Runaway Watch: OFF
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Vig. Fuga Térm.: ON");                      // Runaway Watch: ON
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Brillo cabina");                            // Light Brightness
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driver %");                               // @ Driver %
  LSTR MSG_ERROR_TMC                      = _UxGT("ERROR CONEX. TMC");                         // TMC CONNECTION ERROR
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("CAMBIAR FILAMENTO");                        // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("IMPRESIÓN PAUSADA");                        // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("CARGAR FILAMENTO");                         // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("DESCARGAR FILAMENTO");                      // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("OPC. REINICIO:");                           // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Purgar más");                               // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Boquilla: ");                             // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Sens. filamento");                          // Runout Sensor
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Dist. filamento mm");                       // Runout Dist mm
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Ir a origen Fallado");                      // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Sondeo Fallado");                           // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("ELIJE FILAMENTO");                          // CHOOSE FILAMENT
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");                                      // MMU
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("¡Actu. MMU Firmware!");                     // Update MMU Firmware!
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU Necesita Cuidado");                     // MMU Needs Attention.
  LSTR MSG_MMU2_RESUME                    = _UxGT("Continuar imp.");                           // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Resumiendo...");                            // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Cargar Filamento");                         // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Cargar Todo");                              // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Cargar hasta boqui.");                      // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Expulsar Filamento");                       // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Expulsar Filamento ~");                     // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Descargar Filamento");                      // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Cargando Fil. %i...");                      // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Expulsando Fil. ...");                      // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Descargando Fil....");                      // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("Todo");                                     // All
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filamento ~");                              // Filament ~
  LSTR MSG_MMU2_RESET                     = _UxGT("Reiniciar MMU");                            // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Reiniciando MMU...");                       // Resetting...
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Retirar, y pulsar");                        // Eject Recover
  LSTR MSG_MIX                            = _UxGT("Mezcla");                                   // Mix
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Componente {");                             // Component {
  LSTR MSG_MIXER                          = _UxGT("Miezclador");                               // Mixer
  LSTR MSG_GRADIENT                       = _UxGT("Degradado");                                // Gradient
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Degradado Total");                          // Full Gradient
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Mezcla Conmutada");                         // Toggle Mix
  LSTR MSG_CYCLE_MIX                      = _UxGT("Mezcla Cíclica");                           // Cycle Mix
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Mezcla de Degradado");                      // Gradient Mix
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Degradado inverso");                        // Reverse Gradient
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Activar Herr.V");                           // Active V-tool
  LSTR MSG_START_VTOOL                    = _UxGT("Inicio Herr.V");                            // Start V-tool
  LSTR MSG_END_VTOOL                      = _UxGT("   Fin Herr.V");                            // End V-tool
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias Herr.V");                             // Alias V-tool
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Reiniciar Herr.V");                         // Reset V-tools
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Cometer mezc. Herr.V");                     // Commit V-tool Mix
  LSTR MSG_VTOOLS_RESET                   = _UxGT("Herr.V reiniciados");                       // V-tools Were Reset
  LSTR MSG_START_Z                        = _UxGT("Inicio Z:");                                // Start Z:
  LSTR MSG_END_Z                          = _UxGT("   Fin Z:");                                // End Z:
  LSTR MSG_GAMES                          = _UxGT("Juegos");                                   // Games
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");                                 // Brickout
  LSTR MSG_INVADERS                       = _UxGT("Invaders");                                 // Invaders
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");                                    // Sn4k3
  LSTR MSG_MAZE                           = _UxGT("Maze");                                     // Maze
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_1_LINE("Pulse para continuar"));         // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Aparcando..."));                 // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Purgando..."));                  // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_1_LINE("Pulse para finalizar"));         // |Click to finish
  LSTR MSG_TMC_DRIVERS                    = _UxGT("Controladores TMC");                        // TMC Drivers
  LSTR MSG_TMC_CURRENT                    = _UxGT("Amperaje Controlador");                     // Driver Current
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Límite Hibrido");                           // Hybrid Threshold
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Origen sin sensores");                      // Sensorless Homing
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Modo de pasos");                            // Stepping Mode
  LSTR MSG_TMC_STEALTHCHOP                = _UxGT("StealthChop");                              // StealthChop
  LSTR MSG_SERVICE_RESET                  = _UxGT("Reiniciar");                                // Reset
  LSTR MSG_SERVICE_IN                     = _UxGT(" dentro:");                                 // in:
  LSTR MSG_BACKLASH                       = _UxGT("Backlash");                                 // Backlash
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Corrección");                               // Correction
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Suavizado");                                // Smoothing
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Fallo al iniciar ") MEDIA_TYPE_ES;          // (MEDIA_TYPE_EN) Init Fail
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Desbordamiento de subllamada");             // Subcall Overflow
  LSTR MSG_ITERATION                      = _UxGT("G34 Iteración: %i");                        // G34 Iteration: %i
  LSTR MSG_DECREASING_ACCURACY            = _UxGT("¡Precisión disminuyendo!");                 // Accuracy Decreasing!
  LSTR MSG_ACCURACY_ACHIEVED              = _UxGT("Precisión conseguida");                     // Accuracy Achieved
  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frecuencia");                               // Frequency
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Boquilla Aparcada");                        // Nozzle Parked
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Controlador Vent.");                        // Controller Fan
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Velocidad en reposo");                      // Idle Speed
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Modo Auto");                                // Auto Mode
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Velocidad Activa");                         // Active Speed
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Periodo de reposo");                        // Idle Period
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("Err: EEPROM CRC");                          // Err: EEPROM CRC
  LSTR MSG_ERR_EEPROM_SIZE                = _UxGT("Err: EEPROM Tamaño");                       // Err: EEPROM Size
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("Err: Versión EEPROM");                      // Err: EEPROM Version
  LSTR MSG_PRINT_DONE                     = _UxGT("Impresión Completada");                     // Print Done
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Desf. Sonda");                              // Probe Offsets
  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Nivel Eje X");                              // Level X Axis
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Auto Calibrar");                            // Auto Calibrate
  LSTR MSG_HEATER_TIMEOUT                 = _UxGT("T. de esp. Calent.");                       // Heater Timeout
  LSTR MSG_REHEAT                         = _UxGT("Recalentar");                               // Reheat
  LSTR MSG_REHEATING                      = _UxGT("Recalentando...");                          // Reheating...
}

namespace LanguageWide_es {
  using namespace LanguageNarrow_es;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Cont. de impresión");                       // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Completadas");                              // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tiempo total de imp.");                     // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Impresión más larga");                      // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total Extruido");                           // Extruded Total
  #endif
}

namespace LanguageTall_es {
  using namespace LanguageWide_es;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Esperando para", "iniciar el cambio", "de filamento")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Inserte el filamento", "y pulse el botón", "para continuar...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Espere para", "liberar el filamento")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Espere para", "cargar el filamento")); // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Esperando impresora", "para reanudar...")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Calentando boquilla", "Espere por favor...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Pulse el botón para", "calentar la boquilla")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Pulsar el botón para", "reanudar impresión")); // |Press Button|to resume print
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Aparcando..."));                 // |Parking...
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Espere para", "purgar el filamento")); // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Pulse para finalizar", "la purga de filamen.")); // |Click to finish|filament purge
  #endif
}

namespace Language_es {
  using namespace LanguageTall_es;
}
