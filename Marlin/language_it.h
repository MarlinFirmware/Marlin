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

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" pronto.")
#define MSG_BACK                            _UxGT("Indietro")
#define MSG_SD_INSERTED                     _UxGT("SD Card inserita")
#define MSG_SD_REMOVED                      _UxGT("SD Card rimossa")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstop")
#define MSG_MAIN                            _UxGT("Menu principale")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Disabilita Motori")
#define MSG_DEBUG_MENU                      _UxGT("Menu di debug")
#define MSG_PROGRESS_BAR_TEST               _UxGT("Test barra avanzamento")
#define MSG_AUTO_HOME                       _UxGT("Auto Home")
#define MSG_AUTO_HOME_X                     _UxGT("Home asse X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home asse Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home asse Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Home assi XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Premi per iniziare")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Punto successivo")
#define MSG_LEVEL_BED_DONE                  _UxGT("Livel. terminato!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Annulla")
#define MSG_SET_HOME_OFFSETS                _UxGT("Imp. offset home")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offset applicato")
#define MSG_SET_ORIGIN                      _UxGT("Imposta Origine")
#define MSG_PREHEAT_1                       _UxGT("Preriscalda PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Tutto")
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" Fine")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Piatto")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" conf")
#define MSG_PREHEAT_2                       _UxGT("Preriscalda ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Tutto")
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" Fine")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Piatto")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" conf")
#define MSG_COOLDOWN                        _UxGT("Raffredda")
#define MSG_SWITCH_PS_ON                    _UxGT("Accendi aliment.")
#define MSG_SWITCH_PS_OFF                   _UxGT("Spegni aliment.")
#define MSG_EXTRUDE                         _UxGT("Estrudi")
#define MSG_RETRACT                         _UxGT("Ritrai")
#define MSG_MOVE_AXIS                       _UxGT("Muovi Asse")
#define MSG_LEVEL_BED                       _UxGT("Livella piano")
#define MSG_MOVING                          _UxGT("In movimento...")
#define MSG_FREE_XY                         _UxGT("XY liberi")
#define MSG_MOVE_X                          _UxGT("Muovi X")
#define MSG_MOVE_Y                          _UxGT("Muovi Y")
#define MSG_MOVE_Z                          _UxGT("Muovi Z")
#define MSG_MOVE_E                          _UxGT("Estrusore")
#define MSG_MOVE_01MM                       _UxGT("Muovi di 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Muovi di   1mm")
#define MSG_MOVE_10MM                       _UxGT("Muovi di  10mm")
#if ENABLED(DOGLCD)
  #define MSG_SPEED                         _UxGT("Velocità")
#else
  #define MSG_SPEED                         _UxGT("Velocita")
#endif
#define MSG_BED_Z                           _UxGT("piatto Z")
#define MSG_NOZZLE                          _UxGT("Ugello")
#define MSG_BED                             _UxGT("Piatto")
#if ENABLED(DOGLCD)
  #define MSG_FAN_SPEED                     _UxGT("Velocità ventola")
#else
  #define MSG_FAN_SPEED                     _UxGT("Velocita ventola")
#endif
#define MSG_FLOW                            _UxGT("Flusso")
#define MSG_CONTROL                         _UxGT("Controllo")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("On ")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Seleziona")
#define MSG_ACC                             _UxGT("Accel")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retract")
#define MSG_A_TRAVEL                        _UxGT("A-Spostamento")
#define MSG_XSTEPS                          _UxGT("Xpassi/mm")
#define MSG_YSTEPS                          _UxGT("Ypassi/mm")
#define MSG_ZSTEPS                          _UxGT("Zpassi/mm")
#define MSG_ESTEPS                          _UxGT("Epassi/mm")
#define MSG_E1STEPS                         _UxGT("E1passi/mm")
#define MSG_E2STEPS                         _UxGT("E2passi/mm")
#define MSG_E3STEPS                         _UxGT("E3passi/mm")
#define MSG_E4STEPS                         _UxGT("E4passi/mm")
#define MSG_E5STEPS                         _UxGT("E5passi/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Diam. filo")
#define MSG_ADVANCE_K                       _UxGT("K avanzato")
#define MSG_CONTRAST                        _UxGT("Contrasto LCD")
#define MSG_STORE_EEPROM                    _UxGT("Salva in memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Carica da memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Ripristina imp.")
#define MSG_REFRESH                         _UxGT("Aggiorna")
#define MSG_WATCH                           _UxGT("Guarda")
#define MSG_PREPARE                         _UxGT("Prepara")
#define MSG_TUNE                            _UxGT("Regola")
#define MSG_PAUSE_PRINT                     _UxGT("Pausa")
#define MSG_RESUME_PRINT                    _UxGT("Riprendi stampa")
#define MSG_STOP_PRINT                      _UxGT("Arresta stampa")
#define MSG_CARD_MENU                       _UxGT("Stampa da SD")
#define MSG_NO_CARD                         _UxGT("SD non presente")
#define MSG_DWELL                           _UxGT("Sospensione...")
#define MSG_USERWAIT                        _UxGT("Attendi Utente..")
#define MSG_RESUMING                        _UxGT("Riprendi Stampa")
#define MSG_PRINT_ABORTED                   _UxGT("Stampa annullata")
#define MSG_NO_MOVE                         _UxGT("Nessun Movimento")
#define MSG_KILLED                          _UxGT("UCCISO. ")
#define MSG_STOPPED                         _UxGT("ARRESTATO. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Ritrai mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Scamb. Ritrai mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Ritrai  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Salta mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Scamb. UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRitrai")
#define MSG_FILAMENTCHANGE                  _UxGT("Cambia filamento")
#define MSG_INIT_SDCARD                     _UxGT("Iniz. SD-Card")
#define MSG_CNG_SDCARD                      _UxGT("Cambia SD-Card")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#define MSG_BLTOUCH_SELFTEST                _UxGT("Autotest BLTouch")
#define MSG_BLTOUCH_RESET                   _UxGT("Resetta BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("prima")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Finecorsa abort")
#define MSG_HEATING_FAILED_LCD              _UxGT("Riscald. Fallito")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: TEMP RIDONDANTI")
#define MSG_THERMAL_RUNAWAY                 _UxGT("TEMP FUORI CONTROLLO")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: TEMP MASSIMA")
#define MSG_ERR_MINTEMP                     _UxGT("Err: TEMP MINIMA")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: TEMP MASSIMA PIATTO")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: TEMP MINIMA PIATTO")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Vietato")
#define MSG_HALTED                          _UxGT("STAMPANTE FERMATA")
#define MSG_PLEASE_RESET                    _UxGT("Riavviare prego")
#define MSG_SHORT_DAY                       _UxGT("g") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Riscaldamento..")
#define MSG_HEATING_COMPLETE                _UxGT("Risc. completato")
#define MSG_BED_HEATING                     _UxGT("Risc. Piatto..")
#define MSG_BED_DONE                        _UxGT("Piatto Pronto")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibraz. Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibra X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibra Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibra Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibra Center")
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto calibrazione")
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Imp. altezza Delta")
#define MSG_INFO_MENU                       _UxGT("Riguardo stampante")
#define MSG_INFO_PRINTER_MENU               _UxGT("Info. stampante")
#define MSG_INFO_STATS_MENU                 _UxGT("Statistiche")
#define MSG_INFO_BOARD_MENU                 _UxGT("Info. scheda")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistori")
#define MSG_INFO_EXTRUDERS                  _UxGT("Estrusori")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocollo")
#define MSG_LIGHTS_ON                       _UxGT("Luci Case on")
#define MSG_LIGHTS_OFF                      _UxGT("Luci Case off")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Contat. stampa")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completati")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tempo totale")
  #if ENABLED(DOGLCD)
    #define MSG_INFO_PRINT_LONGEST          _UxGT("Lavoro più lungo")
  #else
    #define MSG_INFO_PRINT_LONGEST          _UxGT("Lavoro piu lungo")
  #endif
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Totale estruso")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Stampe")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completati")
  #define MSG_INFO_PRINT_TIME               _UxGT("Durata")
  #if ENABLED(DOGLCD)
    #define MSG_INFO_PRINT_LONGEST          _UxGT("Più lungo")
  #else
    #define MSG_INFO_PRINT_LONGEST          _UxGT("Piu lungo")
  #endif
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Estruso")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Temp min")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temp max")
#define MSG_INFO_PSU                        _UxGT("Alimentatore")

#define MSG_DRIVE_STRENGTH                  _UxGT("Potenza Drive")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Scrivi DAC EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("CAMBIA FILAMENTO")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("CAMBIA OPZIONI:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Estrusione")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Riprendi stampa")
#if ENABLED(DOGLCD)
  #define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Temp minima è ")
#else
  #define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Temp minima e ")
#endif
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Ugello: ")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Attendere avvio")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("del cambio")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("di filamento")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Attendere")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("l'espulsione")
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        _UxGT("del filamento")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Inserisci il")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("filamento e")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("premi per cont")
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Premi per")
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("riscald ugello.")
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Riscald. ugello")
  #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Attendere...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Attendere")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("il caricamento")
  #define MSG_FILAMENT_CHANGE_LOAD_3          _UxGT("del filamento")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Attendere")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("l'estrusione")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       _UxGT("del filamento")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Attendere")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("la ripresa")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("della stampa")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Attendere...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Espulsione...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Inserisci e premi")
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Riscaldamento...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Caricamento...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Estrusione...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Ripresa...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_IT_H
