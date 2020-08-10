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
 * Galician language (ISO "gl")
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace Language_gl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 1;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Galician");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" lista.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Tarxeta inserida");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Tarxeta retirada");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("FinCarro");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu principal");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autoarranque");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Apagar motores");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Ir a orixe");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Ir orixe X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Ir orixe Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Ir orixe Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Ir orixes XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Prema pulsador");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Seguinte punto");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Nivelado feito");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Offsets na orixe");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offsets fixados");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Fixar orixe");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Prequentar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Prequentar ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Bico");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Bico ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Todo");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" Cama");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Preque. ") PREHEAT_1_LABEL _UxGT(" conf");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Prequentar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Prequentar ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Preque. ") PREHEAT_2_LABEL _UxGT(" Bico");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Preque. ") PREHEAT_2_LABEL _UxGT(" Bico ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Preque. ") PREHEAT_2_LABEL _UxGT(" Todo");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Preque. ") PREHEAT_2_LABEL _UxGT(" Cama");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Preque. ") PREHEAT_2_LABEL _UxGT(" conf");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Arrefriar");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Acender");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Apagar");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudir");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retraer");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mover eixe");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Nivelar cama");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Nivelar cama");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mover X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mover Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mover Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extrusor");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extrusor *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mover %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mover 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mover 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mover 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocidade");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Cama Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Bico");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Bico ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Cama");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Velocidade vent.");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Velocidade vent. ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Fluxo");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Fluxo ~");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Escolla");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Escolla *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Acel");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Pasos/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E pasos/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* pasos/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movemento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E en mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Diam. fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Diam. fil. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Constraste LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Gardar en memo.");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Cargar de memo.");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Cargar de firm.");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Volver a cargar");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Monitorizacion");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Preparar");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Axustar");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausar impres.");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Seguir impres.");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Deter impres.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Tarxeta SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Sen tarxeta SD");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("En repouso...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("A espera...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Impre. cancelada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sen movemento.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("PROGRAMA MORTO");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("PROGRAMA PARADO");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retraccion mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Cambio retra. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retraccion V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Alzar Z mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Recup. retra. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Cambio recup. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Recuperacion V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Retraccion auto.");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Cambiar filamen.");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Cambiar filamen. *");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Iniciando SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Cambiar SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda-Z sen cama");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Comprobar BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Iniciar BLTouch");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Offset Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Micropaso X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Micropaso Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Micropaso Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Erro fin carro");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Fallo quentando");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Erro temperatura");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Temp. excesiva");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("SISTEMA MORTO");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Debe reiniciar!");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Quentando...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Quentando cama...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibracion Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrar X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrar Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrar Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrar Centro");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Acerca de...");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Informacion");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Estadisticas");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Placa nai");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistores");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrusores");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baudios");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocolo");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Luz");
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Total traballos");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Total completos");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tempo impresion");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Traballo +longo");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total extruido");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Traballos");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completos");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tempo");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("O +longo");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extruido");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Temp");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Temp");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Fonte alime.");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Potencia motor");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Garda DAC EEPROM");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Segue traballo");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Impresora incorrecta");

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Agarde para", "iniciar troco", "de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Agarde pola", "descarga do", "filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Introduza o", "filamento e", "faga click"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Agarde pola", "carga do", "filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Agarde para", "seguir co", "traballo"));
  #else // LCD_HEIGHT < 4
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Agarde..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Descargando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Introduza e click"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Cargando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Seguindo..."));
  #endif // LCD_HEIGHT < 4
}
