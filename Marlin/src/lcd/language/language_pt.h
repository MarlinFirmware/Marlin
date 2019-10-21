/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Portuguese
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

 #define DISPLAY_CHARSET_ISO10646_1

namespace Language_pt {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Portuguese");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" pronta.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Cartão inserido");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Cartão removido");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu principal");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Desactivar motores");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Ir para origem");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Ir para origem X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Ir para origem Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Ir para origem Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Indo para origem");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Click para iniciar");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Próximo ponto");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Pronto !");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Definir desvio");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offsets aplicados");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Definir origem");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H0                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_H1                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_H2                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_H3                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_H4                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_H5                    = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E0                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E1                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E2                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E3                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E4                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E5                = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL _UxGT(" Bico ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Pre-aq. ") PREHEAT_1_LABEL _UxGT(" Tudo");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Pre-aq. ") PREHEAT_1_LABEL _UxGT(" ") LCD_STR_THERMOMETER _UxGT("Base");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Definições ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H0                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_H1                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_H2                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_H3                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_H4                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_H5                    = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E0                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E1                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E2                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E3                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E4                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E5                = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL _UxGT(" Bico ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Pre-aq. ") PREHEAT_2_LABEL _UxGT(" Tudo");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Pre-aq. ") PREHEAT_2_LABEL _UxGT(" ") LCD_STR_THERMOMETER _UxGT("Base");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Definições ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Arrefecer");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Ligar");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Desligar");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudir");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retrair");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mover eixo");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mover X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mover Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mover Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Mover Extrusor");
  PROGMEM Language_Str MSG_MOVE_E0                         = _UxGT("Mover Extrusor ") LCD_STR_E0;
  PROGMEM Language_Str MSG_MOVE_E1                         = _UxGT("Mover Extrusor ") LCD_STR_E1;
  PROGMEM Language_Str MSG_MOVE_E2                         = _UxGT("Mover Extrusor ") LCD_STR_E2;
  PROGMEM Language_Str MSG_MOVE_E3                         = _UxGT("Mover Extrusor ") LCD_STR_E3;
  PROGMEM Language_Str MSG_MOVE_E4                         = _UxGT("Mover Extrusor ") LCD_STR_E4;
  PROGMEM Language_Str MSG_MOVE_E5                         = _UxGT("Mover Extrusor ") LCD_STR_E5;
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mover %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mover 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mover 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mover 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocidade");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Base Z");
  PROGMEM Language_Str MSG_NOZZLE                          = " " LCD_STR_THERMOMETER _UxGT(" Bico");
  PROGMEM Language_Str MSG_NOZZLE_0                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N0;
  PROGMEM Language_Str MSG_NOZZLE_1                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N1;
  PROGMEM Language_Str MSG_NOZZLE_2                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N2;
  PROGMEM Language_Str MSG_NOZZLE_3                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N3;
  PROGMEM Language_Str MSG_NOZZLE_4                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N4;
  PROGMEM Language_Str MSG_NOZZLE_5                        = " " LCD_STR_THERMOMETER _UxGT(" Bico ") LCD_STR_N5;
  PROGMEM Language_Str MSG_BED                             = " " LCD_STR_THERMOMETER _UxGT(" Base");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Vel. ventoinha");
  PROGMEM Language_Str MSG_FAN_SPEED_1                     = _UxGT("Vel. ventoinha 1");
  PROGMEM Language_Str MSG_FAN_SPEED_2                     = _UxGT("Vel. ventoinha 2");
  PROGMEM Language_Str MSG_FAN_SPEED_3                     = _UxGT("Vel. ventoinha 3");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Fluxo");
  PROGMEM Language_Str MSG_FLOW_0                          = _UxGT("Fluxo ") LCD_STR_N0;
  PROGMEM Language_Str MSG_FLOW_1                          = _UxGT("Fluxo ") LCD_STR_N1;
  PROGMEM Language_Str MSG_FLOW_2                          = _UxGT("Fluxo ") LCD_STR_N2;
  PROGMEM Language_Str MSG_FLOW_3                          = _UxGT("Fluxo ") LCD_STR_N3;
  PROGMEM Language_Str MSG_FLOW_4                          = _UxGT("Fluxo ") LCD_STR_N4;
  PROGMEM Language_Str MSG_FLOW_5                          = _UxGT("Fluxo ") LCD_STR_N5;
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Controlo");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-retracção");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-movimento");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Passo/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E passo/mm");
  PROGMEM Language_Str MSG_E0_STEPS                        = LCD_STR_E0 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E1_STEPS                        = LCD_STR_E1 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E2_STEPS                        = LCD_STR_E2 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E3_STEPS                        = LCD_STR_E3 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E4_STEPS                        = LCD_STR_E4 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_E5_STEPS                        = LCD_STR_E5 _UxGT(" passo/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movimento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E em mm3");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Diam.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E0                = _UxGT("Fil. Diam. ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E1                = _UxGT("Fil. Diam. ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E2                = _UxGT("Fil. Diam. ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E3                = _UxGT("Fil. Diam. ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E4                = _UxGT("Fil. Diam. ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E5                = _UxGT("Fil. Diam. ") LCD_STR_E5;
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contraste");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Guardar na memoria");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Carregar da memoria");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Rest. de emergen.");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH _UxGT(" Recarregar");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Monitorizar");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Preparar");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Afinar");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausar impressão");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Retomar impressão");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Parar impressão");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Imprimir do SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Sem cartão SD");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Em espera...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Á espera de ordem");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Impressão cancelada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sem movimento");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("EMERGÊNCIA. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("PARADO. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT(" Retrair mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Troca Retrair mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT(" Retrair  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT(" Levantar mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT(" DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Troca DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT(" DesRet  V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT(" AutoRetr.");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Trocar filamento");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E0               = _UxGT("Trocar filamento ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E1               = _UxGT("Trocar filamento ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E2               = _UxGT("Trocar filamento ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E3               = _UxGT("Trocar filamento ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E4               = _UxGT("Trocar filamento ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E5               = _UxGT("Trocar filamento ") LCD_STR_E5;
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Inici. cartão SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Trocar cartão SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sensor fora/base");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Desvio Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Fim de curso");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Aquecimento falhou");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: T Máxima");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: T Mínima");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Err: T Base Máxima");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Err: T Base Mínima");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Aquecendo...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Aquecendo base...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibração Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrar X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrar Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrar Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrar Centro");

  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Fim de curso");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Impressora Incorreta");
}
