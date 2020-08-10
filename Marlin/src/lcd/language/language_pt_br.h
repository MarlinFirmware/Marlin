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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Portuguese (Brazil)
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
namespace Language_pt_br {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Portuguese (BR)");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" pronto.");

  PROGMEM Language_Str MSG_BACK                            = _UxGT("Voltar");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Cartão inserido");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Cartão removido");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Fins de curso");
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Soft Fins curso");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu principal");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Config. Avançada");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Configuração");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Início automático");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Desabilit. motores");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menu Debug");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Testar Barra Progres");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Ir a origem XYZ");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Ir na origem X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Ir na origem Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Ir na origem Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto alinhar Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Indo para origem");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Clique para Iniciar");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Próximo Ponto");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Fim nivelação!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Suavizar altura");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Compensar origem");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Alteração aplicada");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Ajustar Origem");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Pre-aquecer ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Extrusora ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Extrusora ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Pre-aq.Todo ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Pre-aq.Mesa ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Ajustar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Pre-aquecer ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Extrusora ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Extrusora ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Pre-aq.Todo ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Pre-aq.Mesa ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Ajustar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Customizar Pre-aq.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Esfriar");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Ligar");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Desligar");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrusar");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retrair");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mover eixo");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Nivelação Mesa");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Nivelar Mesa");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Nivelar Cantos");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Próximo Canto");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Editar Malha");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Fim da Edição");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Índice X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Índice Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Valor Z");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Comando customizado");

  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Modo IDEX");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Estacionar");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplicação");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Cópia espelhada");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Controle Total");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2o bico X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2o bico Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2o bico Z");

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Executando G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Ferramentas UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Nivel. Mesa Unif.");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Fazer malha manual");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Calçar e calibrar");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Medir");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Remover e calibrar");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Movendo para Próximo");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Ativar UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Desativar UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Temp. Mesa");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Temp. Extrusora");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Editar Malha");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Editar Malha Custom");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Ajuste Fino da Malha");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Fim da Edição");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Montar Malha Custom");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Montar ");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("Montar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("Montar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Montar Malha fria");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Ajustar Altura");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Quant. de Altura");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Validar Malha");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("Checar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("Checar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Validar Malha Custom");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Continuar Malha");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Nivelação da Malha");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Nivelação 3 pontos");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Nivelação Grid");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Nivelar Malha");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Cantos");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Tipo de Mapa");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Salvar Mapa da Malha");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Enviar Para Host");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Salvar Malha CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Salvar Backup");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Informação do UBL");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Qtd de Enchimento");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Enchimento Manual");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Enchimento Smart");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Preencher malha");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Invalidar tudo");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Invalidar próximo");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Ajuste Fino de Todos");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Ajustar Mais Próximo");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Armazenamento Malha");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Slot de Memória");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Ler Malha");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Salvar Malha");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Malha %i carregada");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Malha %i salva");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Sem armazenamento");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Erro ao salvar UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Erro no restauro UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Compensação Z parou");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL passo a passo");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Montar Malha fria");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Enchimento Smart");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Validar Malha");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Ajuste Fino de Todos");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Validar Malha");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Ajuste Fino de Todos");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Salvar Malha");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Controle do LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Luz");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Configuração da Luz");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Luz Vermelha");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Luz Laranja");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Luz Amarela");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Luz Verde");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Luz Azul");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Luz Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Luz Violeta");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Luz Branca");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Luz Padrão");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Luz Customizada");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Intensidade Vermelho");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Intensidade Verde");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Intensidade Azul");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Intensidade Branco");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Brilho");
  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Movendo...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Liberar XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mover X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mover Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mover Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Mover Extrusor");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Mover Extrusor *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Extrus. mto fria");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mover %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mover 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mover 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mover 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocidade");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Base Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Bocal");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Bocal ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Mesa");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Vel. Ventoinha");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Vel. Ventoinha ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("+Vel. Ventoinha");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("+Vel. Ventoinha ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Vazão");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Vazão ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Controle");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Máx");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fator");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Temp. Automática");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Ligado");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Desligado");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Selecionar");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Selecionar *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Acel.");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Arrancada");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("arrancada V") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("arrancada V") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("arrancada V") LCD_STR_C;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("arrancada VE");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Desv. Junção");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Velocidade");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VDeslocamento min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Aceleração");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Retrair A");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Movimento A");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Passo/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("Passo ") LCD_STR_A _UxGT("/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("Passo ") LCD_STR_B _UxGT("/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("Passo ") LCD_STR_C _UxGT("/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movimento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("Extrusão em mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Diâmetro Fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Diâmetro Fil. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Descarr. mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Carregar mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Avanço K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Avanço K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contraste");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Salvar Configuração");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Ler Configuração");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Restauro seguro");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Iniciar EEPROM");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Atualiz. SD");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Resetar Impressora");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Atualização");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Informações");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Preparar");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Ajustar");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausar impressão");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Resumir impressão");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Parar impressão");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Recuperar Impressão");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Imprimir do SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Sem cartão SD");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Dormindo...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Clique para retomar");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Impressão Pausada");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Imprimindo...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Impressão Abortada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sem movimento");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("PARADA DE EMERGÊNCIA");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("PAROU. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retrair mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Retrair Troca mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retrair V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Saltar mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Des-Retrair mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Des-RetTroca mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Des-Retrair  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("Des-RetTroca V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Retração Automática");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Distancia Retração");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Mudar Ferramenta");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Levantar Z");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPD          = _UxGT("Preparar Veloc.");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPD        = _UxGT("Veloc. Retração");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Trocar Filamento");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Trocar Filamento *");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Carregar Filamento *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Descarreg. Filamento *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Descarregar Todos");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Iniciar SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Trocar SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda fora da mesa");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Fator de Cisalho");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Testar BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reiniciar BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Estender BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Recolher BLTouch");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Estender Sonda-Z");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Recolher Sonda-Z");

  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s Primeiro");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Compensar Sonda em Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Passinho X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Passinho Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Passinho Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Abortar Fim de Curso");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Aquecimento falhou");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Aquecer mesa falhou");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Erro:Temp Redundante");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ESCAPE TÉRMICO");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("ESCAPE TÉRMICO MESA");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Erro:Temp Máxima");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Erro:Temp Mínima");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Erro:Temp Mesa Máx");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Erro:Temp Mesa Mín");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Home XY Primeiro");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("IMPRESSORA PAROU");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Favor resetar");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d");
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h");
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Aquecendo...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Resfriando...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Aquecendo mesa...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Esfriando mesa...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibrar Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrar X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrar Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrar Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrar Centro");

  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Configuração Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto-Calibração");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Calibrar Altura");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Desloc. Sonda Z");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Haste Diagonal");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Altura");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Raio");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Sobre");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Impressora");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Nivelamento 3 pontos");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Nivelamento Linear");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Nivelamento Bilinear");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Nivelamento UBL");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Nivelamento da Malha");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Estatísticas");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info. da Placa");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistores");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrusoras");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Taxa de Transmissão");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocolo");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Luz da Impressora");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Intensidade Brilho");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Impressora Incorreta");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Total de Impressões");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Realizadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tempo de Impressão");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Trabalho Mais longo");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total de Extrusão");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Qtd de Impressões");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Realizadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tempo de Impressão");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Maior trabalho");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("T. Extrusão");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temp Mín");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temp Máx");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Força do Motor");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Escrever EEPROM DAC");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("IMPRESSÃO PAUSADA");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("CARREGAR FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("DESCARREG. FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("Config. de Retomada");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Purgar mais");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Continuar Impressão");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Bocal: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Sensor filamento");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Falha ao ir à origem");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Falha ao sondar");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Muito frio");

  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Aperte o botão para", "continuar impressão"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Esperando o", "inicio da", "troca de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Coloque filamento", "pressione o botão", "para continuar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Pressione o botão", "p/ aquecer o bocal"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Aquecendo o bocal", "Aguarde..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Esperando", "remoção de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Esperando", "filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Espere pela", "purga de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Clique para finaliz.", "purga de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Esperando impressão", "continuar"));
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Clique p. continuar"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Aguarde..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Insira e Clique"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Aquecendo..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Ejetando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Carregando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Purgando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Clique p. finalizar"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Continuando..."));
  #endif
}
