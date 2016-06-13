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
 * Portuguese (Brazil)
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_PT_BR_UTF_H
#define LANGUAGE_PT_BR_UTF_H

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " pronto."
#define MSG_SD_INSERTED                     "Cartão inserido"
#define MSG_SD_REMOVED                      "Cartão removido"
#define MSG_MAIN                            "Menu principal"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Desabi. motores"
#define MSG_AUTO_HOME                       "Ir para origen"
#define MSG_LEVEL_BED_HOMING                "Indo para origem"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
#define MSG_LEVEL_BED_CANCEL                "Cancel"
#define MSG_SET_HOME_OFFSETS                "Ajustar Jogo"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
#define MSG_SET_ORIGIN                      "Ajustar orig."
#define MSG_PREHEAT_PLA                     "Pre-aquecer PLA"
#define MSG_PREHEAT_PLA_N                   "Pre-aquecer PLA"
#define MSG_PREHEAT_PLA_ALL                 "Pre-aq.Todo PLA"
#define MSG_PREHEAT_PLA_BEDONLY             "Pre-aq. PLA " LCD_STR_THERMOMETER "Base"
#define MSG_PREHEAT_PLA_SETTINGS            "Ajustar PLA"
#define MSG_PREHEAT_ABS                     "Pre-aquecer ABS"
#define MSG_PREHEAT_ABS_N                   "Pre-aquecer ABS"
#define MSG_PREHEAT_ABS_ALL                 "Pre-aq.Todo ABS"
#define MSG_PREHEAT_ABS_BEDONLY             "Pre-aq. ABS " LCD_STR_THERMOMETER "Base"
#define MSG_PREHEAT_ABS_SETTINGS            "Ajustar ABS"
#define MSG_COOLDOWN                        "Esfriar"
#define MSG_SWITCH_PS_ON                    "Ligar"
#define MSG_SWITCH_PS_OFF                   "Desligar"
#define MSG_EXTRUDE                         "Extrudar"
#define MSG_RETRACT                         "Retrair"
#define MSG_MOVE_AXIS                       "Mover eixo"
#define MSG_MOVE_X                          "Mover X"
#define MSG_MOVE_Y                          "Mover Y"
#define MSG_MOVE_Z                          "Mover Z"
#define MSG_MOVE_E                          "Mover Extrusor"
#define MSG_MOVE_01MM                       "Mover 0.1mm"
#define MSG_MOVE_1MM                        "Mover 1mm"
#define MSG_MOVE_10MM                       "Mover 10mm"
#define MSG_SPEED                           "Velocidade"
#define MSG_BED_Z                           "Base Z"
#define MSG_NOZZLE                          LCD_STR_THERMOMETER " Bocal"
#define MSG_BED                             LCD_STR_THERMOMETER " Base"
#define MSG_FAN_SPEED                       "Vel. Ventoinha"
#define MSG_FLOW                            "Fluxo"
#define MSG_CONTROL                         "Controle"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Temp. Automática"
#define MSG_ON                              "Ligado "
#define MSG_OFF                             "Desligado"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Acc"
#define MSG_VXY_JERK                        "jogo VXY"
#define MSG_VZ_JERK                         "jogo VZ"
#define MSG_VE_JERK                         "jogo VE"
#define MSG_VMAX                            " Vmax "
#define MSG_X                               "X"
#define MSG_Y                               "Y"
#define MSG_Z                               "Z"
#define MSG_E                               "E"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "Retrair A"
#define MSG_A_TRAVEL                        "A-movimento"
#define MSG_XSTEPS                          "Passo X/mm"
#define MSG_YSTEPS                          "Passo Y/mm"
#define MSG_ZSTEPS                          "Passo Z/mm"
#define MSG_ESTEPS                          "E/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Movimento"
#define MSG_VOLUMETRIC                      "Filamento"
#define MSG_VOLUMETRIC_ENABLED              "Extr. em mm3"
#define MSG_FILAMENT_DIAM                   "Diametro Fil."
#define MSG_CONTRAST                        "Contraste"
#define MSG_STORE_EPROM                     "Salvar"
#define MSG_LOAD_EPROM                      "Ler"
#define MSG_RESTORE_FAILSAFE                "Rest. de emerg."
#define MSG_REFRESH                         LCD_STR_REFRESH " Restaurar"
#define MSG_WATCH                           "Monitorar"
#define MSG_PREPARE                         "Preparar"
#define MSG_TUNE                            "Afinar"
#define MSG_PAUSE_PRINT                     "Pausar impressão"
#define MSG_RESUME_PRINT                    "Resumir impressão"
#define MSG_STOP_PRINT                      "Parar impressão"
#define MSG_CARD_MENU                       "Imprimir do SD"
#define MSG_NO_CARD                         "Sem cartão SD"
#define MSG_DWELL                           "Repouso..."
#define MSG_USERWAIT                        "Esperando ordem"
#define MSG_RESUMING                        "Resumindo Impres."
#define MSG_PRINT_ABORTED                   "Impres. Abortada."
#define MSG_NO_MOVE                         "Sem movimento"
#define MSG_KILLED                          "PARADA DE EMERG."
#define MSG_STOPPED                         "PARADA. "
#define MSG_CONTROL_RETRACT                 "Retrair mm"
#define MSG_CONTROL_RETRACT_SWAP            "Retrair Troca mm"
#define MSG_CONTROL_RETRACTF                "Retrair  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Levantar mm"
#define MSG_CONTROL_RETRACT_RECOVER         "Des Retrair +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Des RetTroca +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "Des Retrair  V"
#define MSG_AUTORETRACT                     "Retração Autom."
#define MSG_FILAMENTCHANGE                  "Trocar Filamento"
#define MSG_INIT_SDCARD                     "Iniciar SD"
#define MSG_CNG_SDCARD                      "Trocar SD"
#define MSG_ZPROBE_OUT                      "Son. fora da mesa"
#define MSG_YX_UNHOMED                      "Pos. Desconhecida"
#define MSG_ZPROBE_ZOFFSET                  "Deslocamento no Z"
#define MSG_BABYSTEP_X                      "Passinho X"
#define MSG_BABYSTEP_Y                      "Passinho Y"
#define MSG_BABYSTEP_Z                      "Passinho Z"
#define MSG_ENDSTOP_ABORT                   "Fim de Curso"
#define MSG_HEATING_FAILED_LCD              "Aquecimento falhou"
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: T Máxima"
#define MSG_ERR_MINTEMP                     "Err: T Mínima"
#define MSG_ERR_MAXTEMP_BED                 "Err: T Base Máxima"
#define MSG_ERR_MINTEMP_BED                 "Err: T Base Mínima"
#define MSG_END_HOUR                        "Horas"
#define MSG_END_MINUTE                      "Minutos"
#define MSG_HEATING                         "Aquecendo..."
#define MSG_HEATING_COMPLETE                "Aquecida."
#define MSG_BED_HEATING                     "Aquecendo base.."
#define MSG_BED_DONE                        "Base aquecida."

#define MSG_DELTA_CALIBRATE                 "Calibrar Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibrar X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrar Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrar Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrar Centro"

#endif // LANGUAGE_PT_BR_UTF_H
