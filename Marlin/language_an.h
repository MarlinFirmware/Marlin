/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Aragonese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_AN_H
#define LANGUAGE_AN_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" parada.")
#define MSG_SD_INSERTED                     _UxGT("Tarcheta mesa")
#define MSG_SD_REMOVED                      _UxGT("Tarcheta sacada")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu prencipal")
#define MSG_AUTOSTART                       _UxGT("Inicio automatico")
#define MSG_DISABLE_STEPPERS                _UxGT("Amortar motors")
#define MSG_AUTO_HOME                       _UxGT("Levar a l'orichen")
#define MSG_AUTO_HOME_X                     _UxGT("Orichen X")
#define MSG_AUTO_HOME_Y                     _UxGT("Orichen Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Orichen Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Orichen XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Encetar (pretar)")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Vinient punto")
#define MSG_LEVEL_BED_DONE                  _UxGT("Nivelacion feita!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Achustar desfases")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Desfase aplicau")
#define MSG_SET_ORIGIN                      _UxGT("Establir orichen")
#define MSG_PREHEAT_1                       _UxGT("Precalentar PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Tot")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Base")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Conf")
#define MSG_PREHEAT_2                       _UxGT("Precalentar ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Tot")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Base")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Conf")
#define MSG_COOLDOWN                        _UxGT("Enfriar")
#define MSG_SWITCH_PS_ON                    _UxGT("Enchegar Fuent")
#define MSG_SWITCH_PS_OFF                   _UxGT("Amortar Fuent")
#define MSG_EXTRUDE                         _UxGT("Extruir")
#define MSG_RETRACT                         _UxGT("Retraer")
#define MSG_MOVE_AXIS                       _UxGT("Mover Eixes")
#define MSG_BED_LEVELING                    _UxGT("Nivelar base")
#define MSG_LEVEL_BED                       _UxGT("Nivelar base")
#define MSG_MOVE_X                          _UxGT("Mover X")
#define MSG_MOVE_Y                          _UxGT("Mover Y")
#define MSG_MOVE_Z                          _UxGT("Mover Z")
#define MSG_MOVE_E                          _UxGT("Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Mover 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mover 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mover 10mm")
#define MSG_SPEED                           _UxGT("Velocidat")
#define MSG_BED_Z                           _UxGT("Base Z")
#define MSG_NOZZLE                          _UxGT("Boquilla")
#define MSG_BED                             _UxGT("Base")
#define MSG_FAN_SPEED                       _UxGT("Ixoriador")
#define MSG_FLOW                            _UxGT("Fluxo")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Temperatura Auto.")
#define MSG_ON                              _UxGT("On")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Trigar")
#define MSG_ACC                             _UxGT("Aceleracion")
#define MSG_JERK                            _UxGT("Jerk")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("Vel. viache min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Acel. max")
#define MSG_A_RETRACT                       _UxGT("Acel. retrac.")
#define MSG_A_TRAVEL                        _UxGT("Acel. Viaje")
#define MSG_STEPS_PER_MM                    _UxGT("Trangos/mm")
#define MSG_XSTEPS                          _UxGT("X trangos/mm")
#define MSG_YSTEPS                          _UxGT("Y trangos/mm")
#define MSG_ZSTEPS                          _UxGT("Z trangos/mm")
#define MSG_ESTEPS                          _UxGT("E trangos/mm")
#define MSG_E1STEPS                         _UxGT("E1 trangos/mm")
#define MSG_E2STEPS                         _UxGT("E2 trangos/mm")
#define MSG_E3STEPS                         _UxGT("E3 trangos/mm")
#define MSG_E4STEPS                         _UxGT("E4 trangos/mm")
#define MSG_E5STEPS                         _UxGT("E5 trangos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimiento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("Contraste")
#define MSG_STORE_EEPROM                    _UxGT("Alzar memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Cargar memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Restaurar memoria")
#define MSG_REFRESH                         _UxGT("Tornar a cargar")
#define MSG_WATCH                           _UxGT("Informacion")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Achustar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impresion")
#define MSG_RESUME_PRINT                    _UxGT("Contin. impresion")
#define MSG_STOP_PRINT                      _UxGT("Detener Impresion")
#define MSG_CARD_MENU                       _UxGT("Menu de SD")
#define MSG_NO_CARD                         _UxGT("No i hai tarcheta")
#define MSG_DWELL                           _UxGT("Reposo...")
#define MSG_USERWAIT                        _UxGT("Aguardand ordines")
#define MSG_RESUMING                        _UxGT("Contin. impresion")
#define MSG_PRINT_ABORTED                   _UxGT("Impres. cancelada")
#define MSG_NO_MOVE                         _UxGT("Sin movimiento")
#define MSG_KILLED                          _UxGT("Aturada d'emerch.")
#define MSG_STOPPED                         _UxGT("Aturada.")
#define MSG_CONTROL_RETRACT                 _UxGT("Retraer mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Retraer mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraer  F")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Devantar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("DesRet F")
#define MSG_AUTORETRACT                     _UxGT("Retraccion auto.")
#define MSG_FILAMENTCHANGE                  _UxGT("Cambear filamento")
#define MSG_INIT_SDCARD                     _UxGT("Encetan. tarcheta")
#define MSG_CNG_SDCARD                      _UxGT("Cambiar tarcheta")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z fuera")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Auto-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Desfase Z")
#define MSG_BABYSTEP_X                      _UxGT("Micropaso X")
#define MSG_BABYSTEP_Y                      _UxGT("Micropaso Y")
#define MSG_BABYSTEP_Z                      _UxGT("Micropaso Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Cancelado - Endstop")
#define MSG_HEATING_FAILED_LCD              _UxGT("Error: en calentar")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Error: temperatura")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Error de temperatura")
#define MSG_ERR_MAXTEMP                     _UxGT("Error: Temp Maxima")
#define MSG_ERR_MINTEMP                     _UxGT("Error: Temp Menima")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Error: Temp Max base")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Error: Temp Min base")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z vedau")
#define MSG_HALTED                          _UxGT("IMPRESORA ATURADA")
#define MSG_PLEASE_RESET                    _UxGT("Per favor reinic.")
#define MSG_SHORT_DAY                       _UxGT("d")
#define MSG_SHORT_HOUR                      _UxGT("h")
#define MSG_SHORT_MINUTE                    _UxGT("m")
#define MSG_HEATING                         _UxGT("Calentando...")
#define MSG_HEATING_COMPLETE                _UxGT("Calentamiento listo")
#define MSG_BED_HEATING                     _UxGT("Calentando base...")
#define MSG_BED_DONE                        _UxGT("Base calient")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibracion Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrar X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrar Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrar Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrar Centro")

#define MSG_INFO_MENU                       _UxGT("Inf. Impresora")
#define MSG_INFO_PRINTER_MENU               _UxGT("Inf. Impresora")
#define MSG_INFO_STATS_MENU                 _UxGT("Estadisticas Imp.")
#define MSG_INFO_BOARD_MENU                 _UxGT("Inf. Controlador")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrusors")
#define MSG_INFO_BAUDRATE                   _UxGT("Baudios")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocolo")
#define MSG_CASE_LIGHT                      _UxGT("Luz")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Conteo de impresion")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tiempo total d'imp.")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Impresion mas larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total d'extrusion")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Impresions")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Mas larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrusion")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Temperatura menima")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temperatura maxima")
#define MSG_INFO_PSU                        _UxGT("Fuente de aliment")

#define MSG_DRIVE_STRENGTH                  _UxGT("Fuerza d'o driver")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Escri. DAC EEPROM")
#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("PRINT PAUSED")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("RESUME OPTIONS:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extruir mas")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resumir imp.")

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Aguardand iniciar")

#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Meta o filamento")
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("y prete lo boton")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("d'o filamento")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("cambear")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("pa continar...")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("d'o fil. cambear")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Meta o filamento")
#endif // LCD_HEIGHT < 4

#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Aguardando a")
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("expulsar filament")
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Aguardando a")
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("cargar filamento")
#define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Aguardando a")
#define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("extruir filamento")
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Aguardando impre.")
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("pa continar")

#endif // LANGUAGE_AN_H
