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
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_AN_H
#define LANGUAGE_AN_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " parada."
#define MSG_SD_INSERTED                     "Tarcheta mesa"
#define MSG_SD_REMOVED                      "Tarcheta sacada"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Menu prencipal"
#define MSG_AUTOSTART                       "Inicio automatico"
#define MSG_DISABLE_STEPPERS                "Amortar motors"
#define MSG_AUTO_HOME                       "Levar a l'orichen"
#define MSG_AUTO_HOME_X                     "Orichen X"
#define MSG_AUTO_HOME_Y                     "Orichen Y"
#define MSG_AUTO_HOME_Z                     "Orichen Z"
#define MSG_LEVEL_BED_HOMING                "Orichen XYZ"
#define MSG_LEVEL_BED_WAITING               "Encetar (pretar)"
#define MSG_LEVEL_BED_NEXT_POINT            "Vinient punto"
#define MSG_LEVEL_BED_DONE                  "Nivelacion feita!"
#define MSG_LEVEL_BED_CANCEL                "Cancelar"
#define MSG_SET_HOME_OFFSETS                "Achustar desfases"
#define MSG_HOME_OFFSETS_APPLIED            "Desfase aplicau"
#define MSG_SET_ORIGIN                      "Establir orichen"
#define MSG_PREHEAT_1                       "Precalentar PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Tot"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Base"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Conf"
#define MSG_PREHEAT_2                       "Precalentar ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Tot"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Base"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Conf"
#define MSG_COOLDOWN                        "Enfriar"
#define MSG_SWITCH_PS_ON                    "Enchegar Fuent"
#define MSG_SWITCH_PS_OFF                   "Amortar Fuent"
#define MSG_EXTRUDE                         "Extruir"
#define MSG_RETRACT                         "Retraer"
#define MSG_MOVE_AXIS                       "Mover Eixes"
#define MSG_LEVEL_BED                       "Nivelar base"
#define MSG_MOVE_X                          "Mover X"
#define MSG_MOVE_Y                          "Mover Y"
#define MSG_MOVE_Z                          "Mover Z"
#define MSG_MOVE_E                          "Extrusor"
#define MSG_MOVE_01MM                       "Mover 0.1mm"
#define MSG_MOVE_1MM                        "Mover 1mm"
#define MSG_MOVE_10MM                       "Mover 10mm"
#define MSG_SPEED                           "Velocidat"
#define MSG_BED_Z                           "Base Z"
#define MSG_NOZZLE                          "Boquilla"
#define MSG_BED                             "Base"
#define MSG_FAN_SPEED                       "Ixoriador"
#define MSG_FLOW                            "Fluxo"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Temperatura Auto."
#define MSG_ON                              "On"
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Trigar"
#define MSG_ACC                             "Aceleracion"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "Vel. viache min"
#define MSG_AMAX                            "Acel. max"
#define MSG_A_RETRACT                       "Acel. retrac."
#define MSG_A_TRAVEL                        "Acel. Viaje"
#define MSG_XSTEPS                          "X trangos/mm"
#define MSG_YSTEPS                          "Y trangos/mm"
#define MSG_ZSTEPS                          "Z trangos/mm"
#define MSG_ESTEPS                          "E trangos/mm"
#define MSG_E1STEPS                         "E1 trangos/mm"
#define MSG_E2STEPS                         "E2 trangos/mm"
#define MSG_E3STEPS                         "E3 trangos/mm"
#define MSG_E4STEPS                         "E4 trangos/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Movimiento"
#define MSG_VOLUMETRIC                      "Filamento"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "Contraste"
#define MSG_STORE_EPROM                     "Alzar memoria"
#define MSG_LOAD_EPROM                      "Cargar memoria"
#define MSG_RESTORE_FAILSAFE                "Restaurar memoria"
#define MSG_REFRESH                         "Tornar a cargar"
#define MSG_WATCH                           "Informacion"
#define MSG_PREPARE                         "Preparar"
#define MSG_TUNE                            "Achustar"
#define MSG_PAUSE_PRINT                     "Pausar impresion"
#define MSG_RESUME_PRINT                    "Contin. impresion"
#define MSG_STOP_PRINT                      "Detener Impresion"
#define MSG_CARD_MENU                       "Menu de SD"
#define MSG_NO_CARD                         "No i hai tarcheta"
#define MSG_DWELL                           "Reposo..."
#define MSG_USERWAIT                        "Aguardand ordines"
#define MSG_RESUMING                        "Contin. impresion"
#define MSG_PRINT_ABORTED                   "Impres. cancelada"
#define MSG_NO_MOVE                         "Sin movimiento"
#define MSG_KILLED                          "Aturada d'emerch."
#define MSG_STOPPED                         "Aturada."
#define MSG_CONTROL_RETRACT                 "Retraer mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Retraer mm"
#define MSG_CONTROL_RETRACTF                "Retraer  F"
#define MSG_CONTROL_RETRACT_ZLIFT           "Devantar mm"
#define MSG_CONTROL_RETRACT_RECOVER         "DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Swap DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "DesRet F"
#define MSG_AUTORETRACT                     "Retraccion auto."
#define MSG_FILAMENTCHANGE                  "Cambear filamento"
#define MSG_INIT_SDCARD                     "Encetan. tarcheta"
#define MSG_CNG_SDCARD                      "Cambiar tarcheta"
#define MSG_ZPROBE_OUT                      "Sonda Z fuera"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Auto-Test"
#define MSG_BLTOUCH_RESET                   "Reset BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Desfase Z"
#define MSG_BABYSTEP_X                      "Micropaso X"
#define MSG_BABYSTEP_Y                      "Micropaso Y"
#define MSG_BABYSTEP_Z                      "Micropaso Z"
#define MSG_ENDSTOP_ABORT                   "Cancelado - Endstop"
#define MSG_HEATING_FAILED_LCD              "Error: en calentar"
#define MSG_ERR_REDUNDANT_TEMP              "Error: temperatura"
#define MSG_THERMAL_RUNAWAY                 "Error de temperatura"
#define MSG_ERR_MAXTEMP                     "Error: Temp Maxima"
#define MSG_ERR_MINTEMP                     "Error: Temp Menima"
#define MSG_ERR_MAXTEMP_BED                 "Error: Temp Max base"
#define MSG_ERR_MINTEMP_BED                 "Error: Temp Min base"
#define MSG_ERR_Z_HOMING                    "G28 Z vedau"
#define MSG_HALTED                          "IMPRESORA ATURADA"
#define MSG_PLEASE_RESET                    "Per favor reinic."
#define MSG_SHORT_DAY                       "d"
#define MSG_SHORT_HOUR                      "h"
#define MSG_SHORT_MINUTE                    "m"
#define MSG_HEATING                         "Calentando..."
#define MSG_HEATING_COMPLETE                "Calentamiento listo"
#define MSG_BED_HEATING                     "Calentando base..."
#define MSG_BED_DONE                        "Base calient"
#define MSG_DELTA_CALIBRATE                 "Calibracion Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibrar X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrar Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrar Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrar Centro"

#define MSG_INFO_MENU                       "Inf. Impresora"
#define MSG_INFO_PRINTER_MENU               "Inf. Impresora"
#define MSG_INFO_STATS_MENU                 "Estadisticas Imp."
#define MSG_INFO_BOARD_MENU                 "Inf. Controlador"
#define MSG_INFO_THERMISTOR_MENU            "Termistors"
#define MSG_INFO_EXTRUDERS                  "Extrusors"
#define MSG_INFO_BAUDRATE                   "Baudios"
#define MSG_INFO_PROTOCOL                   "Protocolo"
#define MSG_LIGHTS_ON                       "Enchegar luz"
#define MSG_LIGHTS_OFF                      "Desenchegar luz"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Conteo de impresion"
  #define MSG_INFO_COMPLETED_PRINTS         "Completadas"
  #define MSG_INFO_PRINT_TIME               "Tiempo total d'imp."
  #define MSG_INFO_PRINT_LONGEST            "Impresion mas larga"
  #define MSG_INFO_PRINT_FILAMENT           "Total d'extrusion"
#else
  #define MSG_INFO_PRINT_COUNT              "Impresions"
  #define MSG_INFO_COMPLETED_PRINTS         "Completadas"
  #define MSG_INFO_PRINT_TIME               "Total"
  #define MSG_INFO_PRINT_LONGEST            "Mas larga"
  #define MSG_INFO_PRINT_FILAMENT           "Extrusion"
#endif

#define MSG_INFO_MIN_TEMP                   "Temperatura menima"
#define MSG_INFO_MAX_TEMP                   "Temperatura maxima"
#define MSG_INFO_PSU                        "Fuente de aliment"

#define MSG_DRIVE_STRENGTH                  "Fuerza d'o driver"
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "Escri. DAC EEPROM"
#define MSG_FILAMENT_CHANGE_HEADER          "Cambear filamento"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Opcion de cambio:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extruir mas"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Resumir imp."

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#define MSG_FILAMENT_CHANGE_INIT_1          "Aguardand iniciar"

#define MSG_FILAMENT_CHANGE_INSERT_1        "Meta o filamento"
#define MSG_FILAMENT_CHANGE_INSERT_2        "y prete lo boton"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2          "d'o filamento"
  #define MSG_FILAMENT_CHANGE_INIT_3          "cambear"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "pa continar..."
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2          "d'o fil. cambear"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Meta o filamento"
#endif // LCD_HEIGHT < 4

#define MSG_FILAMENT_CHANGE_UNLOAD_1        "Aguardando a"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        "expulsar filament"
#define MSG_FILAMENT_CHANGE_LOAD_1          "Aguardando a"
#define MSG_FILAMENT_CHANGE_LOAD_2          "cargar filamento"
#define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Aguardando a"
#define MSG_FILAMENT_CHANGE_EXTRUDE_2       "extruir filamento"
#define MSG_FILAMENT_CHANGE_RESUME_1        "Aguardando impre."
#define MSG_FILAMENT_CHANGE_RESUME_2        "pa continar"

#endif // LANGUAGE_AN_H
