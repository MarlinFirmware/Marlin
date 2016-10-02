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
 * Galician language (ISO "gl")
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_GL_H
#define LANGUAGE_GL_H

#define MAPPER_C2C3
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " lista."
#define MSG_SD_INSERTED                     "Tarxeta inserida"
#define MSG_SD_REMOVED                      "Tarxeta retirada"
#define MSG_LCD_ENDSTOPS                    "FinCarro"
#define MSG_MAIN                            "Menu principal"
#define MSG_AUTOSTART                       "Autoarranque"
#define MSG_DISABLE_STEPPERS                "Apagar motores"
#define MSG_AUTO_HOME                       "Ir a orixe"
#define MSG_AUTO_HOME_X                     "Ir orixe X"
#define MSG_AUTO_HOME_Y                     "Ir orixe Y"
#define MSG_AUTO_HOME_Z                     "Ir orixe Z"
#define MSG_LEVEL_BED_HOMING                "Ir orixes XYZ"
#define MSG_LEVEL_BED_WAITING               "Prema pulsador"
#define MSG_LEVEL_BED_NEXT_POINT            "Seguinte punto"
#define MSG_LEVEL_BED_DONE                  "Nivelado feito"
#define MSG_LEVEL_BED_CANCEL                "Cancelar"
#define MSG_SET_HOME_OFFSETS                "Offsets na orixe"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets fixados"
#define MSG_SET_ORIGIN                      "Fixar orixe"
#define MSG_PREHEAT_1                       "Prequentar PLA"
#define MSG_PREHEAT_1_N                     "Prequentar PLA "
#define MSG_PREHEAT_1_ALL                   "Preque. PLA Todo"
#define MSG_PREHEAT_1_BEDONLY               "Preque. PLA Cama"
#define MSG_PREHEAT_1_SETTINGS              "Preque. PLA conf"
#define MSG_PREHEAT_2                       "Prequentar ABS"
#define MSG_PREHEAT_2_N                     "Prequentar ABS "
#define MSG_PREHEAT_2_ALL                   "Preque. ABS Todo"
#define MSG_PREHEAT_2_BEDONLY               "Preque. ABS Cama"
#define MSG_PREHEAT_2_SETTINGS              "Preque. ABS conf"
#define MSG_COOLDOWN                        "Arrefriar"
#define MSG_SWITCH_PS_ON                    "Acender"
#define MSG_SWITCH_PS_OFF                   "Apagar"
#define MSG_EXTRUDE                         "Extrudir"
#define MSG_RETRACT                         "Retraer"
#define MSG_MOVE_AXIS                       "Mover eixe"
#define MSG_LEVEL_BED                       "Nivelar cama"
#define MSG_MOVE_X                          "Mover X"
#define MSG_MOVE_Y                          "Mover Y"
#define MSG_MOVE_Z                          "Mover Z"
#define MSG_MOVE_E                          "Extrusor"
#define MSG_MOVE_01MM                       "Mover 0.1mm"
#define MSG_MOVE_1MM                        "Mover 1mm"
#define MSG_MOVE_10MM                       "Mover 10mm"
#define MSG_SPEED                           "Velocidade"
#define MSG_BED_Z                           "Cama Z"
#define MSG_NOZZLE                          "Bico"
#define MSG_BED                             "Cama"
#define MSG_FAN_SPEED                       "Velocidade vent."
#define MSG_FLOW                            "Fluxo"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On "
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Escolla"
#define MSG_ACC                             "Acel"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_XSTEPS                          "Xpasos/mm"
#define MSG_YSTEPS                          "Ypasos/mm"
#define MSG_ZSTEPS                          "Zpasos/mm"
#define MSG_ESTEPS                          "Epasos/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Movemento"
#define MSG_VOLUMETRIC                      "Filamento"
#define MSG_VOLUMETRIC_ENABLED              "E en mm3"
#define MSG_FILAMENT_DIAM                   "Diam. fil."
#define MSG_CONTRAST                        "Constraste LCD"
#define MSG_STORE_EPROM                     "Gardar en memo."
#define MSG_LOAD_EPROM                      "Cargar de memo."
#define MSG_RESTORE_FAILSAFE                "Cargar de firm."
#define MSG_REFRESH                         "Volver a cargar"
#define MSG_WATCH                           "Monitorizacion"
#define MSG_PREPARE                         "Preparar"
#define MSG_TUNE                            "Axustar"
#define MSG_PAUSE_PRINT                     "Pausar impres."
#define MSG_RESUME_PRINT                    "Seguir impres."
#define MSG_STOP_PRINT                      "Deter impres."
#define MSG_CARD_MENU                       "Tarxeta SD"
#define MSG_NO_CARD                         "Sen tarxeta SD"
#define MSG_DWELL                           "En repouso..."
#define MSG_USERWAIT                        "A espera..."
#define MSG_RESUMING                        "Imprimindo..."
#define MSG_PRINT_ABORTED                   "Impre. cancelada"
#define MSG_NO_MOVE                         "Sen movemento."
#define MSG_KILLED                          "PROGRAMA MORTO"
#define MSG_STOPPED                         "PROGRAMA PARADO"
#define MSG_CONTROL_RETRACT                 "Retraccion mm"
#define MSG_CONTROL_RETRACT_SWAP            "Cambio retra. mm"
#define MSG_CONTROL_RETRACTF                "Retraccion V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Alzar Z mm"
#define MSG_CONTROL_RETRACT_RECOVER         "Recup. retra. mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Cambio recup. mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "Recuperacion V"
#define MSG_AUTORETRACT                     "Retraccion auto."
#define MSG_FILAMENTCHANGE                  "Cambiar filamen."
#define MSG_INIT_SDCARD                     "Iniciando SD"
#define MSG_CNG_SDCARD                      "Cambiar SD"
#define MSG_ZPROBE_OUT                      "Sonda-Z sen cama"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Offset Z"
#define MSG_BABYSTEP_X                      "Micropaso X"
#define MSG_BABYSTEP_Y                      "Micropaso Y"
#define MSG_BABYSTEP_Z                      "Micropaso Z"
#define MSG_ENDSTOP_ABORT                   "Erro fin carro"
#define MSG_HEATING_FAILED_LCD              "Fallo quentando"
#define MSG_ERR_REDUNDANT_TEMP              "Erro temperatura"
#define MSG_THERMAL_RUNAWAY                 "Temp. excesiva"
#define MSG_ERR_MAXTEMP                     "Err: temp. max."
#define MSG_ERR_MINTEMP                     "Err: temp. min."
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "Err: MINTEMP BED"
#define MSG_HALTED                          "SISTEMA MORTO"
#define MSG_PLEASE_RESET                    "Debe reiniciar!"
#define MSG_SHORT_DAY                       "d" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Quentando..."
#define MSG_HEATING_COMPLETE                "Xa esta quente"
#define MSG_BED_HEATING                     "Quentando cama"
#define MSG_BED_DONE                        "Cama esta quente"
#define MSG_DELTA_CALIBRATE                 "Calibracion Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibrar X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrar Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrar Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrar Centro"
#define MSG_INFO_MENU                       "Acerca de..."
#define MSG_INFO_PRINTER_MENU               "Informacion"
#define MSG_INFO_STATS_MENU                 "Estadisticas"
#define MSG_INFO_BOARD_MENU                 "Placa nai"
#define MSG_INFO_THERMISTOR_MENU            "Termistores"
#define MSG_INFO_EXTRUDERS                  "Extrusores"
#define MSG_INFO_BAUDRATE                   "Baudios"
#define MSG_INFO_PROTOCOL                   "Protocolo"
#if LCD_WIDTH > 19
    #define MSG_INFO_PRINT_COUNT              "Total traballos"
    #define MSG_INFO_COMPLETED_PRINTS         "Total completos"
    #define MSG_INFO_PRINT_TIME               "Tempo impresion"
    #define MSG_INFO_PRINT_LONGEST            "Traballo +longo"
    #define MSG_INFO_PRINT_FILAMENT           "Total extruido"
#else
    #define MSG_INFO_PRINT_COUNT              "Traballos"
    #define MSG_INFO_COMPLETED_PRINTS         "Completos"
    #define MSG_INFO_PRINT_TIME               "Tempo"
    #define MSG_INFO_PRINT_LONGEST            "O +longo"
    #define MSG_INFO_PRINT_FILAMENT           "Extruido"
#endif
#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Fonte alime."
#define MSG_FILAMENT_CHANGE_HEADER          "TROCO FILAMENTO"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "OPCIONS TROCO:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extruir mais"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Segue traballo"
#if LCD_HEIGHT >= 4
    #define MSG_FILAMENT_CHANGE_INIT_1          "Agarde para"
    #define MSG_FILAMENT_CHANGE_INIT_2          "iniciar troco"
    #define MSG_FILAMENT_CHANGE_INIT_3          "de filamento"
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Agarde pola"
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        "descarga do"
    #define MSG_FILAMENT_CHANGE_UNLOAD_3        "filamento"
    #define MSG_FILAMENT_CHANGE_INSERT_1        "Introduza o"
    #define MSG_FILAMENT_CHANGE_INSERT_2        "filamento e"
    #define MSG_FILAMENT_CHANGE_INSERT_3        "faga click"
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Agarde pola"
    #define MSG_FILAMENT_CHANGE_LOAD_2          "carga do"
    #define MSG_FILAMENT_CHANGE_LOAD_3          "filamento"
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Agarde pola"
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "extrusion do"
    #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "filamento"
    #define MSG_FILAMENT_CHANGE_RESUME_1        "Agarde para"
    #define MSG_FILAMENT_CHANGE_RESUME_2        "seguir co"
    #define MSG_FILAMENT_CHANGE_RESUME_3        "traballo"
#else // LCD_HEIGHT < 4
    #define MSG_FILAMENT_CHANGE_INIT_1          "Agarde..."
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Descargando..."
    #define MSG_FILAMENT_CHANGE_INSERT_1        "Introduza e click"
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Cargando..."
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extruindo..."
    #define MSG_FILAMENT_CHANGE_RESUME_1        "Seguindo..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_GL_H

