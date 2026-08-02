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
 * Portuguese
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

namespace LanguageNarrow_pt {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Portuguese");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" pronta.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Cartão inserido");                          // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Cartão removido");                          // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Fim de curso");                             // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu principal");                           // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Desactivar motores");                       // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Ir para origem");                           // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Ir para origem X");                         // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Ir para origem Y");                         // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Ir para origem Z");                         // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Indo para origem");                         // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Click para iniciar");                       // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Próximo ponto");                            // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Pronto !");                                 // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Definir desvio");                           // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offsets aplicados");                        // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " ~";        // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Pre-aq. ") PREHEAT_1_LABEL _UxGT(" Tudo");  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Pre-aq. ") PREHEAT_1_LABEL " " LCD_STR_THERMOMETER _UxGT("Base"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Definições ") PREHEAT_1_LABEL;              // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Pre-aquecer $");                            // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Pre-aquecer $ ~");                          // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Pre-aquecer $ Bico");                       // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Pre-aquecer $ Bico ~");                     // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Pre-aq. $ Tudo");                           // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Pre-aq. $ ") LCD_STR_THERMOMETER _UxGT("Base"); // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Definições $");                             // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Arrefecer");                                // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Ligar");                                    // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Desligar");                                 // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Extrudir");                                 // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Retrair");                                  // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Mover eixo");                               // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Calibrar e nivelar");                       // Probe and Level
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
  LSTR MSG_NOZZLE                         = " " LCD_STR_THERMOMETER _UxGT(" Bico");            // Nozzle
  LSTR MSG_NOZZLE_N                       = " " LCD_STR_THERMOMETER _UxGT(" Bico ~");          // Nozzle ~
  LSTR MSG_BED                            = " " LCD_STR_THERMOMETER _UxGT(" Base");            // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Vel. ventoinha");                           // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vel. ventoinha ~");                         // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Fluxo");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Fluxo ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Controlo");                                 // Control
  LSTR MSG_A_RETRACT                      = _UxGT("A-retracção");                              // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-movimento");                              // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Passo/mm");                                 // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" passo/mm");                          // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" passo/mm");                          // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" passo/mm");                          // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ passo/mm");                               // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E passo/mm");                               // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* passo/mm");                               // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Movimento");                                // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");                                // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E em mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Diam.");                               // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Diam. *");                             // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Contraste");                                // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Guardar na memoria");                       // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Carregar da memoria");                      // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Rest. de emergen.");                        // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT(" Recarregar");              // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Monitorizar");                              // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Preparar");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Afinar");                                   // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausar impressão");                         // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Retomar impressão");                        // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Parar impressão");                          // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Imprimir do SD");                           // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Sem cartão SD");                            // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Em espera...");                             // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Á espera de ordem");                        // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impressão cancelada");                      // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Sem movimento");                            // No Move.
  LSTR MSG_KILLED                         = _UxGT("EMERGÊNCIA. ");                             // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("PARADO. ");                                 // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT(" Retrair mm");                              // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Troca Retrair mm");                         // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT(" Retrair  V");                              // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT(" Levantar mm");                             // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT(" DesRet mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Troca DesRet mm");                          // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT(" DesRet  V");                               // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT(" Auto-Retract");                            // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Trocar filamento");                         // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Trocar filamento *");                       // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Inici. cartão SD");                         // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Inici. cartão SD");                         // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Inici. flash USB");                         // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Trocar cartão SD");                         // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sensor fora/base");                         // Z Probe Past Bed
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Desvio X");                                 // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Desvio Y");                                 // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Desvio Z");                                 // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Desvio @");                                 // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Passo Z ajuste");                           // Babystep Probe Z
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Fim de curso");                             // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Aquecimento falhou");                       // Heating Failed
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: T Máxima");                            // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: T Mínima");                            // Err: MINTEMP
  LSTR MSG_HEATING                        = _UxGT("Aquecendo...");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Aquecendo base...");                        // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibração Delta");                         // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrar X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrar Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrar Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrar Centro");                          // Calibrate Center
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Impressora Incorreta");                     // INCORRECT PRINTER
  LSTR MSG_TOP_LEFT                       = _UxGT("Superior Esquerdo");                        // Touch Top Left
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Inferior Esquerdo");                        // Touch Bottom Left
  LSTR MSG_TOP_RIGHT                      = _UxGT("Superior Direto");                          // Touch Top Right
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Inferior Direto");                          // Touch Bottom Right
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Calibração Completa");                      // Calibration Completed
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Calibração Falhou");                        // Calibration Failed
}

namespace LanguageWide_pt {
  using namespace LanguageNarrow_pt;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_pt {
  using namespace LanguageWide_pt;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_pt {
  using namespace LanguageTall_pt;
}
