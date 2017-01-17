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
 * Catalan
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_CA_H
#define LANGUAGE_CA_H

#define MAPPER_C2C3  // because of "ó"
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " preparada."
#define MSG_SD_INSERTED                     "Targeta detectada."
#define MSG_SD_REMOVED                      "Targeta extreta."
#define MSG_LCD_ENDSTOPS                    "Endstops"
#define MSG_MAIN                            "Menú principal"
#define MSG_AUTOSTART                       "Inici automatic"
#define MSG_DISABLE_STEPPERS                "Desactiva motors"
#define MSG_AUTO_HOME                       "Ves a l'origen"
#define MSG_AUTO_HOME_X                     "X a origen"
#define MSG_AUTO_HOME_Y                     "Y a origen"
#define MSG_AUTO_HOME_Z                     "Z a origen"
#define MSG_LEVEL_BED_HOMING                "Origen XYZ"
#define MSG_LEVEL_BED_WAITING               "Prem per iniciar"
#define MSG_LEVEL_BED_NEXT_POINT            "Següent punt"
#define MSG_LEVEL_BED_DONE                  "Anivellament fet!"
#define MSG_LEVEL_BED_CANCEL                "Cancel.la"
#define MSG_SET_HOME_OFFSETS                "Ajusta decalatge"
#define MSG_HOME_OFFSETS_APPLIED            "Decalatge aplicat"
#define MSG_SET_ORIGIN                      "Estableix origen"
#define MSG_PREHEAT_1                       "Preescalfa PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 "Tot"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 "Llit"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 "Config"
#define MSG_PREHEAT_2                       "Preescalfa ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 "Tot"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 "Llit"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 "Config"
#define MSG_COOLDOWN                        "Refreda"
#define MSG_SWITCH_PS_ON                    "Switch power on"
#define MSG_SWITCH_PS_OFF                   "Switch power off"
#define MSG_EXTRUDE                         "Extrudeix"
#define MSG_RETRACT                         "Retreu"
#define MSG_MOVE_AXIS                       "Mou eixos"
#define MSG_LEVEL_BED                       "Anivella llit"
#define MSG_MOVE_X                          "Mou X"
#define MSG_MOVE_Y                          "Mou Y"
#define MSG_MOVE_Z                          "Mou Z"
#define MSG_MOVE_E                          "Extrusor"
#define MSG_MOVE_01MM                       "Mou 0.1mm"
#define MSG_MOVE_1MM                        "Mou 1mm"
#define MSG_MOVE_10MM                       "Mou 10mm"
#define MSG_SPEED                           "Velocitat"
#define MSG_BED_Z                           "Base Z"
#define MSG_NOZZLE                          "Nozzle"
#define MSG_BED                             "Llit"
#define MSG_FAN_SPEED                       "Vel. Ventilador"
#define MSG_FLOW                            "Flux"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On "
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Accel"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VViatge min"
#define MSG_AMAX                            "Accel. max "
#define MSG_A_RETRACT                       "Accel. retracc"
#define MSG_A_TRAVEL                        "Accel. Viatge"
#define MSG_XSTEPS                          "Xpassos/mm"
#define MSG_YSTEPS                          "Ypassos/mm"
#define MSG_ZSTEPS                          "Zpassos/mm"
#define MSG_ESTEPS                          "Epassos/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Moviment"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E en mm3"
#define MSG_FILAMENT_DIAM                   "Diam. Fil."
#define MSG_CONTRAST                        "Contrast de LCD"
#define MSG_STORE_EPROM                     "Desa memoria"
#define MSG_LOAD_EPROM                      "Carrega memoria"
#define MSG_RESTORE_FAILSAFE                "Restaura valors"
#define MSG_REFRESH                         "Actualitza"
#define MSG_WATCH                           "Pantalla Info."
#define MSG_PREPARE                         "Prepara"
#define MSG_TUNE                            "Ajusta"
#define MSG_PAUSE_PRINT                     "Pausa impressio"
#define MSG_RESUME_PRINT                    "Repren impressio"
#define MSG_STOP_PRINT                      "Atura impressio."
#define MSG_CARD_MENU                       "Imprimeix de SD"
#define MSG_NO_CARD                         "No hi ha targeta"
#define MSG_DWELL                           "En repos..."
#define MSG_USERWAIT                        "Esperant usuari.."
#define MSG_RESUMING                        "Reprenent imp."
#define MSG_PRINT_ABORTED                   "Imp. cancelada"
#define MSG_NO_MOVE                         "Sense moviment."
#define MSG_KILLED                          "MATAT."
#define MSG_STOPPED                         "ATURADA."
#define MSG_CONTROL_RETRACT                 "Retreu mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Retreure mm"
#define MSG_CONTROL_RETRACTF                "Retreu V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Aixeca mm"
#define MSG_CONTROL_RETRACT_RECOVER         "DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Swap DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "DesRet V"
#define MSG_AUTORETRACT                     "Auto retraccio"
#define MSG_FILAMENTCHANGE                  "Canvia filament"
#define MSG_INIT_SDCARD                     "Inicia SD"
#define MSG_CNG_SDCARD                      "Canvia SD"
#define MSG_ZPROBE_OUT                      "Sonda Z fora"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "primer"
#define MSG_ZPROBE_ZOFFSET                  "Decalatge Z"
#define MSG_BABYSTEP_X                      "Micropas X"
#define MSG_BABYSTEP_Y                      "Micropas Y"
#define MSG_BABYSTEP_Z                      "Micropas Z"
#define MSG_ENDSTOP_ABORT                   "Cancel. Endstop"
#define MSG_HEATING_FAILED_LCD              "Error al escalfar"
#define MSG_ERR_REDUNDANT_TEMP              "Err: TEMP REDUNDANT"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: TEMP MAXIMA"
#define MSG_ERR_MINTEMP                     "Err: TEMP MINIMA"
#define MSG_ERR_MAXTEMP_BED                 "Err: TEMPMAX LLIT"
#define MSG_ERR_MINTEMP_BED                 "Err: TEMPMIN LLIT"
#define MSG_ERR_Z_HOMING                    "G28 Z No permes"
#define MSG_HALTED                          "IMPRESSORA PARADA"
#define MSG_PLEASE_RESET                    "Reinicieu"
#define MSG_SHORT_DAY                       "d" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Escalfant..."
#define MSG_HEATING_COMPLETE                "Escalfament fet."
#define MSG_BED_HEATING                     "Escalfant llit"
#define MSG_BED_DONE                        "Llit fet."
#define MSG_DELTA_CALIBRATE                 "Calibratge Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibra X"
#define MSG_DELTA_CALIBRATE_Y               "Calibra Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibra Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibra el centre"

#define MSG_INFO_MENU                       "Quant a la impr."
#define MSG_INFO_PRINTER_MENU               "Info Impressora"
#define MSG_INFO_STATS_MENU                 "Estadistiques"
#define MSG_INFO_BOARD_MENU                 "Info placa"
#define MSG_INFO_THERMISTOR_MENU            "Termistors"
#define MSG_INFO_EXTRUDERS                  "Extrusors"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocol"
#define MSG_LIGHTS_ON                       "Encen el llum"
#define MSG_LIGHTS_OFF                      "Apaga el llum"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Total impressions"
  #define MSG_INFO_COMPLETED_PRINTS         "Acabades"
  #define MSG_INFO_PRINT_TIME               "Temps total imprimint"
  #define MSG_INFO_PRINT_LONGEST            "Treball mes llarg"
  #define MSG_INFO_PRINT_FILAMENT           "Total extrudit"
#else
  #define MSG_INFO_PRINT_COUNT              "Impressions"
  #define MSG_INFO_COMPLETED_PRINTS         "Acabades"
  #define MSG_INFO_PRINT_TIME               "Total"
  #define MSG_INFO_PRINT_LONGEST            "Mes llarg"
  #define MSG_INFO_PRINT_FILAMENT           "Extrudit"
#endif

#define MSG_INFO_MIN_TEMP                   "Temp. mínima"
#define MSG_INFO_MAX_TEMP                   "Temp. màxima"
#define MSG_INFO_PSU                        "Font alimentacio"

#define MSG_DRIVE_STRENGTH                  "Força motor"
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_HEADER          "CANVI DE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "OPCIONS CANVI:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrudeix mes"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Repren impressió"

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Esperant per"
  #define MSG_FILAMENT_CHANGE_INIT_2          "iniciar el canvi"
  #define MSG_FILAMENT_CHANGE_INIT_3          "de filament"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Esperant per"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "treure filament"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Poseu filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "i premeu el boto"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "per continuar..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Esperant carrega"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "de filament"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Esperant per"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "extreure filament"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Esperant per"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "reprendre"
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Espereu..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Expulsant..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insereix i prem"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Carregant..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extrudint..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Reprenent..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_CA_H
