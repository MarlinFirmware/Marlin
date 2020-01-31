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
 * Aragonese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace Language_an {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 1;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Aragonese");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" parada.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Tarcheta mesa");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Tarcheta sacada");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu prencipal");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Inicio automatico");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Amortar motors");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Levar a l'orichen");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Orichen X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Orichen Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Orichen Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Orichen XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Encetar (pretar)");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Vinient punto");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Nivelacion feita!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Achustar desfases");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Desfase aplicau");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Establir orichen");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Precalentar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Precalentar ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Boquilla");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Precal. ") PREHEAT_1_LABEL _UxGT(" Boquilla ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Tot");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Base");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Conf");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Precalentar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Precalentar ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Precal. ") PREHEAT_2_LABEL _UxGT(" Boquilla");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Precal. ") PREHEAT_2_LABEL _UxGT(" Boquilla ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Tot");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Base");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Conf");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Enfriar");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Enchegar Fuent");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Amortar Fuent");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extruir");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retraer");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mover Eixes");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Nivelar base");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Nivelar base");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mover X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mover Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mover Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extrusor");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extrusor *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mover %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mover 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mover 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mover 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocidat");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Base Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Boquilla");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Boquilla ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Base");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Ixoriador");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Ixoriador ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Fluxo");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Fluxo ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Control");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Temperatura Auto.");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Trigar");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Trigar *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Aceleracion");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vel. viache min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Accel");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Acel. max") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Acel. max") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Acel. max") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Acel. max") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Acel. max *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Acel. retrac.");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Acel. Viaje");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Trangos/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" trangos/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" trangos/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" trangos/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E trangos/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* trangos/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movimiento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Fil. Dia. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contraste");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Alzar memoria");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Cargar memoria");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Restaurar memoria");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH _UxGT("Tornar a cargar");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Informacion");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Preparar");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Achustar");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausar impresion");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Contin. impresion");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Detener Impresion");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Menu de SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("No i hai tarcheta");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Reposo...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Aguardand ordines");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Impres. cancelada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sin movimiento");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("Aturada d'emerch.");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("Aturada.");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retraer mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Swap Retraer mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retraer  F");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Devantar mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Swap DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("DesRet F");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Retraccion auto.");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Cambear filamento");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Cambear filamento *");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Encetan. tarcheta");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Cambiar tarcheta");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z fuera");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reset BLTouch");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Desfase Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Micropaso X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Micropaso Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Micropaso Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Cancelado - Endstop");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Error: en calentar");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Error: temperatura");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Error de temperatura");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Error: Temp Max");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Error: Temp Min");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Home XY first");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("IMPRESORA ATURADA");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Per favor reinic.");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d");
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h");
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Calentando...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Calentando base...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibracion Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrar X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrar Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrar Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrar Centro");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Inf. Impresora");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Inf. Impresora");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Estadisticas Imp.");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Inf. Controlador");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistors");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrusors");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baudios");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocolo");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Luz");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Conteo de impresion");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tiempo total d'imp.");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Impresion mas larga");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total d'extrusion");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Impresions");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Mas larga");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrusion");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temperatura menima");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temperatura maxima");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Fuente de aliment");

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Fuerza d'o driver");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Escri. DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Resumir imp.");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Aguardand iniciar", "d'o filamento", "cambear"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Meta o filamento", "y prete lo boton", "pa continar..."));
  #else // LCD_HEIGHT < 4
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_2_LINE("Aguardand iniciar", "d'o fil. cambear"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_2_LINE("Meta o filamento", "y prete lo boton"));
  #endif // LCD_HEIGHT < 4

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_2_LINE("Aguardando a", "expulsar filament"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_2_LINE("Aguardando a", "cargar filamento"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME          = _UxGT(MSG_2_LINE("Aguardando impre.", "pa continar"));
}
