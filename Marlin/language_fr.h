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
 * French
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_FR_H
#define LANGUAGE_FR_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" prete.")
#define MSG_SD_INSERTED                     _UxGT("Carte inseree")
#define MSG_SD_REMOVED                      _UxGT("Carte retiree")
#define MSG_LCD_ENDSTOPS                    _UxGT("Butées") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu principal")
#define MSG_AUTOSTART                       _UxGT("Demarrage auto")
#define MSG_DISABLE_STEPPERS                _UxGT("Arreter moteurs")
#define MSG_AUTO_HOME                       _UxGT("Origine auto.")
#define MSG_AUTO_HOME_X                     _UxGT("Origine X Auto.")
#define MSG_AUTO_HOME_Y                     _UxGT("Origine Y Auto.")
#define MSG_AUTO_HOME_Z                     _UxGT("Origine Z Auto.")
#define MSG_LEVEL_BED_HOMING                _UxGT("Origine XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Clic pour commencer")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Point suivant")
#define MSG_LEVEL_BED_DONE                  _UxGT("Mise a niveau OK!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Annuler")
#define MSG_SET_HOME_OFFSETS                _UxGT("Regl. decal. origine")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Decalages appliques")
#define MSG_SET_ORIGIN                      _UxGT("Regler origine")
#define MSG_PREHEAT_1                       _UxGT("Prechauffage PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Prechauff. PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Prech. PLA Tout")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Prech. PLA Plateau")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Regl. prech. PLA")
#define MSG_PREHEAT_2                       _UxGT("Prechauffage ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Prechauff. ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Prech. ABS Tout")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Prech. ABS Plateau")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Regl. prech. ABS")
#define MSG_COOLDOWN                        _UxGT("Refroidir")
#define MSG_SWITCH_PS_ON                    _UxGT("Allumer alim.")
#define MSG_SWITCH_PS_OFF                   _UxGT("Eteindre alim.")
#define MSG_EXTRUDE                         _UxGT("Extrusion")
#define MSG_RETRACT                         _UxGT("Retraction")
#define MSG_MOVE_AXIS                       _UxGT("Deplacer un axe")
#define MSG_LEVEL_BED                       _UxGT("Regl. Niv. Plateau")
#define MSG_MOVE_X                          _UxGT("Depl. X")
#define MSG_MOVE_Y                          _UxGT("Depl. Y")
#define MSG_MOVE_Z                          _UxGT("Depl. Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Depl. 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Depl. 1mm")
#define MSG_MOVE_10MM                       _UxGT("Depl. 10mm")
#define MSG_SPEED                           _UxGT(" Vitesse")
#define MSG_BED_Z                           _UxGT("Plateau Z")
#define MSG_NOZZLE                          _UxGT("Buse")
#define MSG_BED                             _UxGT("Plateau")
#define MSG_FAN_SPEED                       _UxGT("Vitesse ventil.")
#define MSG_FLOW                            _UxGT("Flux")
#define MSG_CONTROL                         _UxGT("Controler")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Facteur")
#define MSG_AUTOTEMP                        _UxGT("Temp. Auto.")
#define MSG_ON                              _UxGT("Marche ")
#define MSG_OFF                             _UxGT("Arret")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Selectionner")
#define MSG_ACC                             _UxGT("Acceleration")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("Vdepl min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retract")
#define MSG_A_TRAVEL                        _UxGT("A-Depl.")
#define MSG_XSTEPS                          _UxGT("Xpas/mm")
#define MSG_YSTEPS                          _UxGT("Ypas/mm")
#define MSG_ZSTEPS                          _UxGT("Zpas/mm")
#define MSG_ESTEPS                          _UxGT("Epas/mm")
#define MSG_E1STEPS                         _UxGT("E1pas/mm")
#define MSG_E2STEPS                         _UxGT("E2pas/mm")
#define MSG_E3STEPS                         _UxGT("E3pas/mm")
#define MSG_E4STEPS                         _UxGT("E4pas/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperature")
#define MSG_MOTION                          _UxGT("Mouvement")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E en mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Diam. Fil.")
#define MSG_CONTRAST                        _UxGT("Contraste LCD")
#define MSG_STORE_EPROM                     _UxGT("Sauver config")
#define MSG_LOAD_EPROM                      _UxGT("Lire config")
#define MSG_RESTORE_FAILSAFE                _UxGT("Restaurer defauts")
#define MSG_REFRESH                         _UxGT("Actualiser")
#define MSG_WATCH                           _UxGT("Surveiller")
#define MSG_PREPARE                         _UxGT("Preparer")
#define MSG_TUNE                            _UxGT("Regler")
#define MSG_PAUSE_PRINT                     _UxGT("Interrompre impr.")
#define MSG_RESUME_PRINT                    _UxGT("Reprendre impr.")
#define MSG_STOP_PRINT                      _UxGT("Arreter impr.")
#define MSG_CARD_MENU                       _UxGT("Impr. depuis SD")
#define MSG_NO_CARD                         _UxGT("Pas de carte")
#define MSG_DWELL                           _UxGT("Repos...")
#define MSG_USERWAIT                        _UxGT("Atten. de l'util.")
#define MSG_RESUMING                        _UxGT("Repri. de l'impr.")
#define MSG_PRINT_ABORTED                   _UxGT("Impr. Annulee")
#define MSG_NO_MOVE                         _UxGT("Moteurs bloques.")
#define MSG_KILLED                          _UxGT("MORT.")
#define MSG_STOPPED                         _UxGT("STOPPE.")
#define MSG_CONTROL_RETRACT                 _UxGT("Retraction mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Ech. Retr. mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraction V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Ech. UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet V")
#define MSG_AUTORETRACT                     _UxGT("Retract. Auto.")
#define MSG_FILAMENTCHANGE                  _UxGT("Changer filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. la carte SD")
#define MSG_CNG_SDCARD                      _UxGT("Changer de carte")
#define MSG_ZPROBE_OUT                      _UxGT("Z sonde exte. lit")
#define MSG_BLTOUCH_SELFTEST                _UxGT("Autotest BLTouch")
#define MSG_BLTOUCH_RESET                   _UxGT("RaZ BLTouch")
#define MSG_HOME                            _UxGT("Origine")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("Premier")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Decalage Z")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Butee abandon")
#define MSG_HEATING_FAILED_LCD              _UxGT("Erreur de chauffe")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: TEMP. REDONDANT")
#define MSG_THERMAL_RUNAWAY                 _UxGT("EMBALLEMENT THERM.")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: TEMP. MAX")
#define MSG_ERR_MINTEMP                     _UxGT("Err: TEMP. MIN")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: TEMP. MAX LIT")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: TEMP. MIN LIT")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z interdit")

#define MSG_HALTED                          _UxGT("IMPR. STOPPEE")
#define MSG_PLEASE_RESET                    _UxGT("RaZ. SVP")
#define MSG_SHORT_DAY                       _UxGT("j") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only

#define MSG_HEATING                         _UxGT("En chauffe...")
#define MSG_HEATING_COMPLETE                _UxGT("Chauffe terminee")
#define MSG_BED_HEATING                     _UxGT("Plateau en chauffe..")
#define MSG_BED_DONE                        _UxGT("Chauffe lit terminee")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibration Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrer X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrer Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrer Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrer centre")

#define MSG_INFO_MENU                       _UxGT("Infos imprimante")
#define MSG_INFO_PRINTER_MENU               _UxGT("Infos imprimante")
#define MSG_INFO_STATS_MENU                 _UxGT("Stats. imprimante")
#define MSG_INFO_BOARD_MENU                 _UxGT("Infos carte")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocole")
#define MSG_LIGHTS_ON                       _UxGT("Allumer boitier")
#define MSG_LIGHTS_OFF                      _UxGT("Eteindre boitier")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Nbre impressions")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Terminees")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tps impr. total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Impr. la + longue")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total filament")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Impressions")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Terminees")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("+ long")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Filament")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Temp Min")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temp Max")
#define MSG_INFO_PSU                        _UxGT("Alimentation")
#define MSG_DRIVE_STRENGTH                  _UxGT("Puiss. moteur ")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM sauv.")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("CHANGER FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("CHANGER OPTIONS:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("+ extrusion")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Reprendre impr.")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Attente Demarrage")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("du filament")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("changer")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("attente de")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("decharger filament")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("inserer filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("et app. bouton")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("pour continuer...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("attente de")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("charger filament")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("attente de")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("extrusion fil.")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("attente impression")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("pour reprendre")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Patientez...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejection...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Inserer et click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Chargement...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extrusion...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Reprise...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_FR_H
