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
 * Italian
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_IT_H
#define LANGUAGE_IT_H

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " pronto."
#define MSG_SD_INSERTED                     "SD Card inserita"
#define MSG_SD_REMOVED                      "SD Card rimossa"
#define MSG_MAIN                            "Menu principale"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Disabilita Motori"
#define MSG_AUTO_HOME                       "Auto Home"
#define MSG_LEVEL_BED_HOMING                "Home assi XYZ"
#define MSG_LEVEL_BED_WAITING               "Premi per Iniziare"
#define MSG_LEVEL_BED_DONE                  "Livel. terminato!"
#define MSG_LEVEL_BED_CANCEL                "Annulla"
#define MSG_SET_HOME_OFFSETS                "Imp. offset home"
#define MSG_HOME_OFFSETS_APPLIED            "Offset applicato"
#define MSG_SET_ORIGIN                      "Imposta Origine"
#define MSG_PREHEAT_PLA                     "Preriscalda PLA"
#define MSG_PREHEAT_PLA_N                   MSG_PREHEAT_PLA " "
#define MSG_PREHEAT_PLA_ALL                 MSG_PREHEAT_PLA " Tutto"
#define MSG_PREHEAT_PLA_BEDONLY             MSG_PREHEAT_PLA " Piatto"
#define MSG_PREHEAT_PLA_SETTINGS            MSG_PREHEAT_PLA " conf"
#define MSG_PREHEAT_ABS                     "Preriscalda ABS"
#define MSG_PREHEAT_ABS_N                   MSG_PREHEAT_ABS " "
#define MSG_PREHEAT_ABS_ALL                 MSG_PREHEAT_ABS " Tutto"
#define MSG_PREHEAT_ABS_BEDONLY             MSG_PREHEAT_ABS " Piatto"
#define MSG_PREHEAT_ABS_SETTINGS            MSG_PREHEAT_ABS " conf"
#define MSG_COOLDOWN                        "Raffredda"
#define MSG_SWITCH_PS_ON                    "Accendi aliment."
#define MSG_SWITCH_PS_OFF                   "Spegni aliment."
#define MSG_EXTRUDE                         "Estrudi"
#define MSG_RETRACT                         "Ritrai"
#define MSG_MOVE_AXIS                       "Muovi Asse"
#define MSG_LEVEL_BED                       "Livella piano"
#define MSG_MOVE_X                          "Muovi X"
#define MSG_MOVE_Y                          "Muovi Y"
#define MSG_MOVE_Z                          "Muovi Z"
#define MSG_MOVE_E                          "Estrusore"
#define MSG_MOVE_01MM                       "Muovi di 0.1mm"
#define MSG_MOVE_1MM                        "Muovi di   1mm"
#define MSG_MOVE_10MM                       "Muovi di  10mm"
#define MSG_SPEED                           "Velocità"
#define MSG_BED_Z                           "piatto Z"
#define MSG_NOZZLE                          "Ugello"
#define MSG_BED                             "Piatto"
#define MSG_FAN_SPEED                       "Velocità ventola"
#define MSG_FLOW                            "Flusso"
#define MSG_CONTROL                         "Controllo"
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
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_X                               "X"
#define MSG_Y                               "Y"
#define MSG_Z                               "Z"
#define MSG_E                               "E"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-Spostamento"
#define MSG_XSTEPS                          "Xpassi/mm"
#define MSG_YSTEPS                          "Ypassi/mm"
#define MSG_ZSTEPS                          "Zpassi/mm"
#define MSG_ESTEPS                          "Epassi/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Movimento"
#define MSG_VOLUMETRIC                      "Filamento"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Diam. filo"
#define MSG_CONTRAST                        "Contrasto LCD"
#define MSG_STORE_EPROM                     "Salva in memoria"
#define MSG_LOAD_EPROM                      "Carica da memoria"
#define MSG_RESTORE_FAILSAFE                "Ripristina imp."
#define MSG_REFRESH                         "Aggiorna"
#define MSG_WATCH                           "Guarda"
#define MSG_PREPARE                         "Prepara"
#define MSG_TUNE                            "Regola"
#define MSG_PAUSE_PRINT                     "Pausa"
#define MSG_RESUME_PRINT                    "Riprendi stampa"
#define MSG_STOP_PRINT                      "Arresta stampa"
#define MSG_CARD_MENU                       "Stampa da SD"
#define MSG_NO_CARD                         "SD non presente"
#define MSG_DWELL                           "Sospensione..."
#define MSG_USERWAIT                        "Attendi Utente.."
#define MSG_RESUMING                        "Riprendi Stampa"
#define MSG_PRINT_ABORTED                   "Stampa annullata"
#define MSG_NO_MOVE                         "Nessun Movimento"
#define MSG_KILLED                          "UCCISO. "
#define MSG_STOPPED                         "ARRESTATO. "
#define MSG_CONTROL_RETRACT                 "Ritrai mm"
#define MSG_CONTROL_RETRACT_SWAP            "Scamb. Ritrai mm"
#define MSG_CONTROL_RETRACTF                "Ritrai  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Salta mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Scamb. UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRitrai"
#define MSG_FILAMENTCHANGE                  "Cambia filamento"
#define MSG_INIT_SDCARD                     "Iniz. SD-Card"
#define MSG_CNG_SDCARD                      "Cambia SD-Card"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_YX_UNHOMED                      "Home X/Y prima di Z"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Finecorsa abort"
#define MSG_HEATING_FAILED_LCD              "Riscald. Fallito"
#define MSG_ERR_REDUNDANT_TEMP              "Err: TEMP RIDONDANTI"
#define MSG_THERMAL_RUNAWAY                 "TEMP FUORI CONTROLLO"
#define MSG_ERR_MAXTEMP                     "Err: TEMP MASSIMA"
#define MSG_ERR_MINTEMP                     "Err: TEMP MINIMA"
#define MSG_ERR_MAXTEMP_BED                 "Err: TEMP MASSIMA PIATTO"
#define MSG_ERR_MINTEMP_BED                 "Err: TEMP MINIMA PIATTO"
#define MSG_END_HOUR                        "ore"
#define MSG_END_MINUTE                      "minuti"
#define MSG_HEATING                         "Riscaldamento.."
#define MSG_HEATING_COMPLETE                "Risc. completato"
#define MSG_BED_HEATING                     "Risc. Piatto.."
#define MSG_BED_DONE                        "Piatto Pronto"

#define MSG_DELTA_CALIBRATE                 "Calibraz. Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibra X"
#define MSG_DELTA_CALIBRATE_Y               "Calibra Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibra Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibra Center"

#endif // LANGUAGE_IT_H
