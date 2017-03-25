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

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " pronto."
#define MSG_SD_INSERTED                     "SD Card inserita"
#define MSG_SD_REMOVED                      "SD Card rimossa"
#define MSG_LCD_ENDSTOPS                    "Endstop"
#define MSG_MAIN                            "Menu principale"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Disabilita Motori"
#define MSG_AUTO_HOME                       "Auto Home"
#define MSG_AUTO_HOME_X                     "Home asse X"
#define MSG_AUTO_HOME_Y                     "Home asse Y"
#define MSG_AUTO_HOME_Z                     "Home asse Z"
#define MSG_LEVEL_BED_HOMING                "Home assi XYZ"
#define MSG_LEVEL_BED_WAITING               "Premi per iniziare"
#define MSG_LEVEL_BED_NEXT_POINT            "Punto successivo"
#define MSG_LEVEL_BED_DONE                  "Livel. terminato!"
#define MSG_LEVEL_BED_CANCEL                "Annulla"
#define MSG_SET_HOME_OFFSETS                "Imp. offset home"
#define MSG_HOME_OFFSETS_APPLIED            "Offset applicato"
#define MSG_SET_ORIGIN                      "Imposta Origine"
#define MSG_PREHEAT_1                       "Preriscalda PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Tutto"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Piatto"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " conf"
#define MSG_PREHEAT_2                       "Preriscalda ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Tutto"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Piatto"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " conf"
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
#if ENABLED(DOGLCD)
  #define MSG_SPEED                         "Velocità"
#else
  #define MSG_SPEED                         "Velocita"
#endif
#define MSG_BED_Z                           "piatto Z"
#define MSG_NOZZLE                          "Ugello"
#define MSG_BED                             "Piatto"
#if ENABLED(DOGLCD)
  #define MSG_FAN_SPEED                     "Velocità ventola"
#else
  #define MSG_FAN_SPEED                     "Velocita ventola"
#endif
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
#define MSG_SELECT                          "Seleziona"
#define MSG_ACC                             "Accel"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
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
#define MSG_BLTOUCH_SELFTEST                "Autotest BLTouch"
#define MSG_BLTOUCH_RESET                   "Resetta BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "prima"
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
#define MSG_ERR_Z_HOMING                    "G28 Z Vietato"
#define MSG_HALTED                          "STAMPANTE FERMATA"
#define MSG_PLEASE_RESET                    "Riavviare prego"
#define MSG_SHORT_DAY                       "g" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Riscaldamento.."
#define MSG_HEATING_COMPLETE                "Risc. completato"
#define MSG_BED_HEATING                     "Risc. Piatto.."
#define MSG_BED_DONE                        "Piatto Pronto"
#define MSG_DELTA_CALIBRATE                 "Calibraz. Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibra X"
#define MSG_DELTA_CALIBRATE_Y               "Calibra Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibra Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibra Center"
#define MSG_INFO_MENU                       "Riguardo stampante"
#define MSG_INFO_PRINTER_MENU               "Info. stampante"
#define MSG_INFO_STATS_MENU                 "Statistiche"
#define MSG_INFO_BOARD_MENU                 "Info. scheda"
#define MSG_INFO_THERMISTOR_MENU            "Termistori"
#define MSG_INFO_EXTRUDERS                  "Estrusori"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocollo"
#define MSG_LIGHTS_ON                       "Luci Case on"
#define MSG_LIGHTS_OFF                      "Luci Case off"
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Contat. stampa"
  #define MSG_INFO_COMPLETED_PRINTS         "Completati"
  #define MSG_INFO_PRINT_TIME               "Tempo totale"
  #if ENABLED(DOGLCD)
    #define MSG_INFO_PRINT_LONGEST          "Lavoro più lungo"
  #else
    #define MSG_INFO_PRINT_LONGEST          "Lavoro piu lungo"
  #endif
  #define MSG_INFO_PRINT_FILAMENT           "Totale estruso"
#else
  #define MSG_INFO_PRINT_COUNT              "Stampe"
  #define MSG_INFO_COMPLETED_PRINTS         "Completati"
  #define MSG_INFO_PRINT_TIME               "Durata"
  #if ENABLED(DOGLCD)
    #define MSG_INFO_PRINT_LONGEST          "Più lungo"
  #else
    #define MSG_INFO_PRINT_LONGEST          "Piu lungo"
  #endif
  #define MSG_INFO_PRINT_FILAMENT           "Estruso"
#endif
#define MSG_INFO_MIN_TEMP                   "Temp min"
#define MSG_INFO_MAX_TEMP                   "Temp max"
#define MSG_INFO_PSU                        "Alimentatore"

#define MSG_DRIVE_STRENGTH                  "Potenza Drive"
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "Scrivi DAC EEPROM"

#define MSG_FILAMENT_CHANGE_HEADER          "CAMBIA FILAMENTO"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "CAMBIA OPZIONI:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Estrusione"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Riprendi stampa"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Attendere avvio"
  #define MSG_FILAMENT_CHANGE_INIT_2          "del cambio"
  #define MSG_FILAMENT_CHANGE_INIT_3          "di filamento"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Attendere"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "l'espulsione"
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        "del filamento"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Inserisci il"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "filamento e"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "premi per cont"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Attendere"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "il caricamento"
  #define MSG_FILAMENT_CHANGE_LOAD_3          "del filamento"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Attendere"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "l'estrusione"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "del filamento"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Attendere"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "la ripresa"
  #define MSG_FILAMENT_CHANGE_RESUME_3        "della stampa"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Attendere..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Espulsione..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Inserisci e premi"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Caricamento..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Estrusione..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Ripresa..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_IT_H
