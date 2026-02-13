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
 * Portuguese (Brazil)
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

namespace LanguageNarrow_pt_br {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Portuguese (BR)");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" pronto.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Cartão inserido");                          // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Cartão removido");                          // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Fins de curso");                            // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu principal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Desabilit. motores");                       // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Ir a origem XYZ");                          // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Ir na origem X");                           // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Ir na origem Y");                           // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Ir na origem Z");                           // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Indo para origem");                         // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Clique para Iniciar");                      // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Próximo Ponto");                            // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Fim nivelação!");                           // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Compensar origem");                         // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Alteração aplicada");                       // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " ~";        // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Extrusora ") PREHEAT_1_LABEL;               // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Extrusora ") PREHEAT_1_LABEL " ~";          // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Pre-aq.Todo ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Pre-aq.Mesa ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Ajustar ") PREHEAT_1_LABEL;                 // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Pre-aquecer $");                            // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Pre-aquecer $ ~");                          // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Extrusora $");                              // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Extrusora $ ~");                            // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Pre-aq.Todo $");                            // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Pre-aq.Mesa $");                            // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Ajustar $");                                // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Esfriar");                                  // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Ligar");                                    // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Desligar");                                 // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extrusar");                                 // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retrair");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mover eixo");                               // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sondar e Nivelar");                         // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Nivelação Mesa");                           // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Nivelar Mesa");                             // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Mover X");                                  // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Mover Y");                                  // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Mover Z");                                  // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Mover @");                                  // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Mover Extrusor");                           // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Mover Extrusor *");                         // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Mover $mm");                                // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Mover $in");                                // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Mover $") LCD_STR_DEGREE;                   // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Velocidade");                               // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Base Z");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Bocal");                                    // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Bocal ~");                                  // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Mesa");                                     // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Vel. Ventoinha");                           // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vel. Ventoinha ~");                         // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Vazão");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Vazão ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Controle");                                 // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Temp. Automática");                         // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("Selecionar *");                             // Select *
  LSTR MSG_ACC                            = _UxGT("Acel.");                                    // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("VDeslocamento min");                        // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Aceleração");                               // Acceleration
  LSTR MSG_A_RETRACT                      = _UxGT("Retrair A");                                // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("Movimento A");                              // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Passo/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = _UxGT("Passo ") STR_A _UxGT("/mm");                // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = _UxGT("Passo ") STR_B _UxGT("/mm");                // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = _UxGT("Passo ") STR_C _UxGT("/mm");                // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("Passo @/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E/mm");                                     // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("*/mm");                                     // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Movimento");                                // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");                                // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("Extrusão em mm") SUPERSCRIPT_THREE;         // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diâmetro Fil.");                            // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diâmetro Fil. *");                          // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Contraste");                                // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Salvar Configuração");                      // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Ler Configuração");                         // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Restauro seguro");                          // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Atualização");              // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Informações");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Preparar");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Ajustar");                                  // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausar impressão");                         // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Continuar impressão");                      // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Parar impressão");                          // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Imprimir do SD");                           // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Sem cartão SD");                            // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Dormindo...");                              // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Clique para retomar");                      // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impressão Abortada");                       // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sem movimento");                            // No Move.
  LSTR MSG_KILLED                         = _UxGT("PARADA DE EMERGÊNCIA");                     // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("PAROU. ");                                  // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retrair mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Retrair Troca mm");                         // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retrair V");                                // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Saltar mm");                                // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Des-Retrair mm");                           // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Des-RetTroca mm");                          // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Des-Retrair  V");                           // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Retração Automática");                      // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Trocar Filamento");                         // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Trocar Filamento *");                       // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Iniciar cartão SD");                        // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Iniciar cartão SD");                        // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Iniciar flash USB");                        // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Trocar SD");                                // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Início automático");                        // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda fora da mesa");                       // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reiniciar BLTouch");                        // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Compensar Sonda em X");                     // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Compensar Sonda em Y");                     // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Compensar Sonda em Z");                     // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Compensar Sonda em @");                     // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Passo Z ajuste");                           // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Passinho X");                               // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Passinho Y");                               // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Passinho Z");                               // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Passinho @");                               // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Abortar Fim de Curso");                     // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Aquecimento falhou");                       // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Erro:Temp Redundante");                     // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("ESCAPE TÉRMICO");                           // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Erro:Temp Máxima");                         // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Erro:Temp Mínima");                         // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("IMPRESSORA PAROU");                         // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("Favor resetar");                            // Please Reset
  LSTR MSG_HEATING                        = _UxGT("Aquecendo...");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Aquecendo mesa...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibrar Delta");                           // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrar X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrar Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrar Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrar Centro");                          // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("Sobre");                                    // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Impressora");                               // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Estatísticas");                             // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info. da Placa");                           // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistores");                              // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrusoras");                               // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Taxa de Transmissão");                      // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocolo");                                // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("Luz da Impressora");                        // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Qtd de Impressões");                        // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Realizadas");                               // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Tempo de Impressão");                       // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Maior trabalho");                           // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("T. Extrusão");                              // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp Mín");                                 // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp Máx");                                 // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("PSU");                                      // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Força do Motor");                           // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Escrever EEPROM DAC");                      // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Continuar Impressão");                      // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_1_LINE("Aguarde..."));                   // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_1_LINE("Insira e Clique"));              // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_1_LINE("Ejetando..."));                  // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_1_LINE("Carregando..."));                // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_1_LINE("Continuando..."));               // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Máx");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fator");           // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Ligado");                                   // On
  LSTR MSG_LCD_OFF                        = _UxGT("Desligado");                                // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Impressora Incorreta");                     // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu Debug");                               // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Testar Barra Progres");                     // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("Movendo...");                               // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("Liberar XY");                               // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s Primeiro");                         // Home %s First
  LSTR MSG_FILAMENT_CHANGE_HEATING        = _UxGT(MSG_1_LINE("Aquecendo..."));                 // |Heating...
  LSTR MSG_FILAMENT_CHANGE_HEAT           = _UxGT(MSG_1_LINE("Clique para Aquecer"));          // |Click to heat
  LSTR MSG_YES                            = _UxGT("SIM");                                      // YES
  LSTR MSG_NO                             = _UxGT("NÃO");                                      // NO
  LSTR MSG_BACK                           = _UxGT("Voltar");                                   // Back
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Abortando...");                             // Aborting...
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Erro de leitura");                          // (MEDIA_TYPE_EN) read error
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("USB removido");                             // USB device removed
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("USB falhou");                               // USB start failed
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Soft Fins curso");                          // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Config. Avançada");                         // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("Configuração");                             // Configuration
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto alinhar Z");                           // Auto Z-Align
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Suavizar altura");                          // Fade Height
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Customizar Pre-aq.");                       // Preheat Custom
  LSTR MSG_BED_TRAMMING                   = _UxGT("Nivelar Cantos");                           // Bed Tramming
  LSTR MSG_NEXT_CORNER                    = _UxGT("Próximo Canto");                            // Next Corner
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor de Malha");                          // Mesh Editor
  LSTR MSG_EDIT_MESH                      = _UxGT("Editar Malha");                             // Edit Mesh
  LSTR MSG_EDITING_STOPPED                = _UxGT("Fim da Edição");                            // Mesh Editing Stopped
  LSTR MSG_PROBING_POINT                  = _UxGT("Sondando ponto");                           // Probing Point
  LSTR MSG_MESH_X                         = _UxGT("Índice X");                                 // Index X
  LSTR MSG_MESH_Y                         = _UxGT("Índice Y");                                 // Index Y
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valor Z");                                  // Z Value
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Comando customizado");                      // Custom Commands
  LSTR MSG_M48_TEST                       = _UxGT("M48 Teste de sonda");                       // M48 Probe Test
  LSTR MSG_M48_POINT                      = _UxGT("M48 Ponto");                                // M48 Point
  LSTR MSG_IDEX_MENU                      = _UxGT("Modo IDEX");                                // IDEX Mode
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Estacionar");                          // Auto-Park
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplicação");                               // Duplication
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Cópia espelhada");                          // Mirrored Copy
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Controle Total");                           // Full Control
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2o bico Z");                                // 2nd Nozzle Z
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2o bico @");                                // 2nd Nozzle @
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Executando G29");                           // Doing G29
  LSTR MSG_UBL_TOOLS                      = _UxGT("Ferramentas UBL");                          // UBL Tools
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Fazer malha manual");                       // Manually Build Mesh
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Calçar e calibrar");                        // Place Shim & Measure
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Medir");                                    // Measure
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Remover e calibrar");                       // Remove & Measure Bed
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Movendo para Próximo");                     // Moving to next
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Temp. Mesa");                               // Bed Temp
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Temp. Mesa");                               // Bed Temp
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temp. Extrusora");                          // Hotend Temp
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temp. Extrusora");                          // Hotend Temp
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Editar Malha Custom");                      // Edit Custom Mesh
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Ajuste Fino da Malha");                     // Fine Tuning Mesh
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Fim da Edição");                            // Done Editing Mesh
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Montar Malha Custom");                      // Build Custom Mesh
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Montar ");                                  // Build Mesh
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Montar $");                                 // Build Mesh ($)
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Checar $");                                 // Validate Mesh ($)
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Montar Malha fria");                        // Build Cold Mesh
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Ajustar Altura");                           // Adjust Mesh Height
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Quant. de Altura");                         // Height Amount
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Validar Malha");                            // Validate Mesh
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Validar Malha Custom");                     // Validate Custom Mesh
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Aquecendo Mesa");                       // G26 Heating Bed
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Aquecendo Ext.");                       // G26 Heating Nozzle
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Cancelado");                            // G26 Canceled
  LSTR MSG_G26_LEAVING                    = _UxGT("G26 Saindo");                               // Leaving G26
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Continuar Malha");                          // Continue Bed Mesh
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Nivelação da Malha");                       // Mesh Leveling
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Nivelação 3 pontos");                       // 3-Point Leveling
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Nivelação Grid");                           // Grid Mesh Leveling
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Nivelar Malha");                            // Level Mesh
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Cantos");                                   // Side Points
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Tipo de Mapa");                             // Map Type
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Salvar Mapa da Malha");                     // Output Mesh Map
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Enviar Para Host");                         // Output for Host
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Salvar Malha CSV");                         // Output for CSV
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Salvar Backup");                            // Off Printer Backup
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Informação do UBL");                        // Output UBL Info
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Qtd de Enchimento");                        // Fill-in Amount
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Enchimento Manual");                        // Manual Fill-in
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Enchimento Smart");                         // Smart Fill-in
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Preencher malha");                          // Fill-in Mesh
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Invalidar tudo");                           // Invalidate All
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Invalidar próximo");                        // Invalidate Closest
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Ajuste Fino de Todos");                     // Fine Tune All
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Ajustar Mais Próximo");                     // Fine Tune Closest
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Armazenamento Malha");                      // Mesh Storage
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Slot de Memória");                          // Memory Slot
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Ler Malha");                                // Load Bed Mesh
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Salvar Malha");                             // Save Bed Mesh
  LSTR MSG_MESH_LOADED                    = _UxGT("Malha %i carregada");                       // Mesh %i Loaded
  LSTR MSG_MESH_SAVED                     = _UxGT("Malha %i salva");                           // Mesh %i Saved
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Sem armazenamento");                        // No Storage
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Erro ao salvar UBL");                       // UBL Save Error
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Erro no restauro UBL");                     // UBL Restore Error
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Compensação Z: ");                          // Z-Offset:
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Compensação Z parou");                      // Z-Offset Stopped
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL passo a passo");                        // Step-By-Step UBL
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1.Montar Malha fria");                      // 1. Build Cold Mesh
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2.Enchimento Smart");                       // 2. Smart Fill-in
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3.Validar Malha");                          // 3. Validate Mesh
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4.Ajuste Fino de Todos");                   // 4. Fine Tune All
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5.Validar Malha");                          // 5. Validate Mesh
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6.Ajuste Fino de Todos");                   // 6. Fine Tune All
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7.Salvar Malha");                           // 7. Save Bed Mesh
  LSTR MSG_LED_CONTROL                    = _UxGT("Controle do LED");                          // LED Control
  LSTR MSG_LIGHTS                         = _UxGT("Luz");                                      // Lights
  LSTR MSG_LIGHT_N                        = _UxGT("Luz #{");                                   // Light #{
  LSTR MSG_LED_PRESETS                    = _UxGT("Configuração da Luz");                      // Light Presets
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Luz Vermelha");                             // Red
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Luz Laranja");                              // Orange
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Luz Amarela");                              // Yellow
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Luz Verde");                                // Green
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Luz Azul");                                 // Blue
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Luz Indigo");                               // Indigo
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Luz Violeta");                              // Violet
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Luz Branca");                               // White
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Luz Padrão");                               // Default
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Luz Customizada");                          // Custom Lights
  LSTR MSG_INTENSITY_R                    = _UxGT("Intensidade Vermelho");                     // Red Intensity
  LSTR MSG_INTENSITY_G                    = _UxGT("Intensidade Verde");                        // Green Intensity
  LSTR MSG_INTENSITY_B                    = _UxGT("Intensidade Azul");                         // Blue Intensity
  LSTR MSG_INTENSITY_W                    = _UxGT("Intensidade Branco");                       // White Intensity
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Brilho");                                   // Brightness
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Extrus. mto fria");                         // Hotend too cold
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("+Vel. Ventoinha");                          // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("+Vel. Ventoinha ~");                        // Extra Fan Speed ~
  LSTR MSG_JERK                           = _UxGT("Arrancada");                                // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("arrancada V") STR_A;                        // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("arrancada V") STR_B;                        // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("arrancada V") STR_C;                        // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("arrancada V@");                             // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("arrancada VE");                             // Max E Jerk
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Desv. Junção");                             // Junction Dev
  LSTR MSG_MAX_SPEED                      = _UxGT("Velocidade");                               // Max Speed (mm/s)
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Descarr. mm");                              // Unload mm
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Carregar mm");                              // Load mm
  LSTR MSG_ADVANCE_K                      = _UxGT("Avanço K");                                 // Advance K
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Avanço K *");                               // Advance K *
  LSTR MSG_INIT_EEPROM                    = _UxGT("Iniciar EEPROM");                           // Initialize EEPROM
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Atualiz. SD");                              // (MEDIA_TYPE_EN) Update
  LSTR MSG_RESET_PRINTER                  = _UxGT("Resetar Impressora");                       // Reset Printer
  LSTR MSG_START_PRINT                    = _UxGT("Iniciar Impressão");                        // Start Print
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Prox.");                                    // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("Iniciar");                                  // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("Parar");                                    // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Imprimir");                                 // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("Resetar");                                  // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Cancelar");                                 // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("Pronto");                                   // Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("Voltar");                                   // Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Continuar");                                // Proceed
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Imprimindo objeto");                        // Print Obj
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Cancelar Objeto");                          // Cancel Obj
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Cancelar Objeto {");                        // Cancel Obj {
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Recuperar Impressão");                      // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Impressão Pausada");                        // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("Imprimindo...");                            // Printing...
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("Des-RetTroca V");                           // S UnRet V
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Distancia Retração");                       // Swap Length
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Distancia Purga");                          // Purge Length
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Mudar Ferramenta");                         // Tool Change
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Levantar Z");                               // Z Raise
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Preparar Veloc.");                          // Prime Speed
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Veloc. Retração");                          // Retract Speed
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Standby bico");                             // Nozzle Standby
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Carregar Filamento");                       // Load Filament
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Carregar Filamento *");                     // Load * Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Descarreg. Filamento");                     // Unload Filament
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Descarreg. Filamento *");                   // Unload * Filament
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Descarregar Todos");                        // Unload All
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Liberar SD");                               // Release (MEDIA_TYPE_EN)
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Fator de Cisalho");                         // Skew Factor
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");                                  // BLTouch
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Testar BLTouch");                           // Self-Test
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Recolher BLTouch");                         // Stow
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Estender BLTouch");                         // Deploy
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Estender Sonda-Z");                         // Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Recolher Sonda-Z");                         // Stow Z-Probe
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");                                    // Total
  LSTR MSG_COOLING                        = _UxGT("Resfriando...");                            // Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT("Esfriando mesa...");                        // Bed Cooling...
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Aquecendo Câmara...");                      // Chamber Heating...
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Esfriando Câmara...");                      // Chamber Cooling...
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Configuração Delta");                       // Delta Settings
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto-Calibração");                          // Auto Calibration
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Haste Diagonal");                           // Diag Rod
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Altura");                                   // Height
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Raio");                                     // Radius
  LSTR MSG_3POINT_LEVELING                = _UxGT("Nivelamento 3 pontos");                     // 3-Point Leveling
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Nivelamento Linear");                       // Linear Leveling
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Nivelamento Bilinear");                     // Bilinear Leveling
  LSTR MSG_UBL_LEVELING                   = _UxGT("Nivelamento UBL");                          // Unified Bed Leveling
  LSTR MSG_MESH_LEVELING                  = _UxGT("Nivelamento da Malha");                     // Mesh Leveling
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Intensidade Brilho");                       // Light Brightness
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("TROCA DE FILAMENTO");                       // FILAMENT CHANGE
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("IMPRESSÃO PAUSADA");                        // PRINT PAUSED
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("CARREGAR FILAMENTO");                       // LOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("DESCARREG. FILAMENTO");                     // UNLOAD FILAMENT
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("Config. de Retomada");                      // RESUME OPTIONS:
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Purgar mais");                              // Purge more
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Bocal: ");                                // Nozzle:
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Sensor filamento");                         // Runout Sensor
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Falha ao ir à origem");                     // Homing Failed
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Falha ao sondar");                          // Probing Failed
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("ESCOLHER FILAMENTO");                       // CHOOSE FILAMENT
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");                                      // MMU
  LSTR MSG_MMU2_RESUME                    = _UxGT("Continuar Impressão");                      // Resume
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Continuando...");                           // MMU Resuming...
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Carregar Filamento");                       // Load
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Carregar Todos");                           // Load All
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Carregar para bocal");                      // Load to Nozzle
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Ejetar Filamento");                         // Eject
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Ejetar Filamento ~");                       // Eject ~
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Liberar Filamento");                        // Unload
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Carregando Fil. %i...");                    // Filament %i Load...
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Ejetando Fil. ...");                        // Filament %i Eject...
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Carregando Fil....");                       // Filament %i Unload...
  LSTR MSG_MMU2_ALL                       = _UxGT("Todos");                                    // All
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filamento ~");                              // Filament ~
  LSTR MSG_MMU2_RESET                     = _UxGT("Resetar MMU");                              // Reset MMU
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Resetando MMU...");                         // Resetting...
  LSTR MSG_GAMES                          = _UxGT("Jogos");                                    // Games
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");                                 // Brickout
  LSTR MSG_INVADERS                       = _UxGT("Invaders");                                 // Invaders
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");                                    // Sn4k3
  LSTR MSG_MAZE                           = _UxGT("Labirinto");                                // Maze
  LSTR MSG_ADVANCED_PAUSE_WAITING         = _UxGT(MSG_1_LINE("Clique p. continuar"));          // |Click to continue
  LSTR MSG_PAUSE_PRINT_PARKING            = _UxGT(MSG_1_LINE("Estacionando..."));              // |Parking...
  LSTR MSG_FILAMENT_CHANGE_PURGE          = _UxGT(MSG_1_LINE("Purgando..."));                  // |Purging...
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE     = _UxGT(MSG_1_LINE("Clique p. finalizar"));          // |Click to finish
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Compensar Sonda");                          // Probe Offsets
  LSTR MSG_TOP_LEFT                       = _UxGT("Superior Esquerdo");                        // Touch Top Left
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Inferior Esquerdo");                        // Touch Bottom Left
  LSTR MSG_TOP_RIGHT                      = _UxGT("Superior Direto");                          // Touch Top Right
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Inferior Direto");                          // Touch Bottom Right
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Calibração Completa");                      // Calibration Completed
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Calibração Falhou");                        // Calibration Failed
}

namespace LanguageWide_pt_br {
  using namespace LanguageNarrow_pt_br;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Total de Impressões");                      // Print Count
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Realizadas");                               // Completed
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tempo de Impressão");                       // Print Time
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Trabalho Mais longo");                      // Longest Job Time
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total de Extrusão");                        // Extruded Total
  #endif
}

namespace LanguageTall_pt_br {
  using namespace LanguageWide_pt_br;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Esperando o", "inicio da", "troca de filamento")); // |Wait for|filament change|to start
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Coloque filamento", "pressione o botão", "para continuar...")); // |Insert filament|and press button|to continue
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Esperando", "remoção de filamento")); // |Wait for|filament unload
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Esperando", "filamento"));       // |Wait for|filament load
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Esperando impressão", "continuar")); // |Wait for print|to resume...
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Aquecendo o bocal", "Aguarde...")); // |Nozzle heating|Please wait...
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Pressione o botão", "p/ aquecer o bocal")); // |Press button|to heat nozzle
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Aperte o botão para", "continuar impressão")); // |Press Button|to resume print
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Espere pela", "purga de filamento")); // |Wait for|filament purge
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Clique para finaliz.", "purga de filamento")); // |Click to finish|filament purge
  #endif
}

namespace Language_pt_br {
  using namespace LanguageTall_pt_br;
}
