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

#define WELCOME_MSG                         MACHINE_NAME " prête."
#define MSG_SD_INSERTED                     "Carte inséree"
#define MSG_SD_REMOVED                      "Carte retirée"
#define MSG_LCD_ENDSTOPS                    "Butées" // Max length 8 characters
#define MSG_MAIN                            "Menu principal"
#define MSG_AUTOSTART                       "Démarrage auto"
#define MSG_DISABLE_STEPPERS                "Arrêter moteurs"
#define MSG_AUTO_HOME                       "Origine auto."
#define MSG_AUTO_HOME_X                     "Origine X Auto."
#define MSG_AUTO_HOME_Y                     "Origine Y Auto."
#define MSG_AUTO_HOME_Z                     "Origine Z Auto."
#define MSG_LEVEL_BED_HOMING                "Origine XYZ"
#define MSG_LEVEL_BED_WAITING               "Clic pour commencer"
#define MSG_LEVEL_BED_NEXT_POINT            "Point suivant"
#define MSG_LEVEL_BED_DONE                  "Mise à niveau OK!"
#define MSG_LEVEL_BED_CANCEL                "Annuler"
#define MSG_SET_HOME_OFFSETS                "Regl. decal. origine"
#define MSG_HOME_OFFSETS_APPLIED            "Décalages appliqués"
#define MSG_SET_ORIGIN                      "Régler origine"
#define MSG_PREHEAT_1                       "Préchauffage PLA"
#define MSG_PREHEAT_1_N                     "Préchauff. PLA "
#define MSG_PREHEAT_1_ALL                   "Préch. PLA Tout"
#define MSG_PREHEAT_1_BEDONLY               "Préch. PLA Plateau"
#define MSG_PREHEAT_1_SETTINGS              "Régl. préch. PLA"
#define MSG_PREHEAT_2                       "Préchauffage ABS"
#define MSG_PREHEAT_2_N                     "Préchauff. ABS "
#define MSG_PREHEAT_2_ALL                   "Préch. ABS Tout"
#define MSG_PREHEAT_2_BEDONLY               "Préch. ABS Plateau"
#define MSG_PREHEAT_2_SETTINGS              "Régl. préch. ABS"
#define MSG_COOLDOWN                        "Refroidir"
#define MSG_SWITCH_PS_ON                    "Allumer alim."
#define MSG_SWITCH_PS_OFF                   "Eteindre alim."
#define MSG_EXTRUDE                         "Extrusion"
#define MSG_RETRACT                         "Rétraction"
#define MSG_MOVE_AXIS                       "Déplacer un axe"
#define MSG_LEVEL_BED                       "Régl. Niv. Plateau"
#define MSG_MOVE_X                          "Dépl. X"
#define MSG_MOVE_Y                          "Dépl. Y"
#define MSG_MOVE_Z                          "Dépl. Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Dépl. 0.1mm"
#define MSG_MOVE_1MM                        "Dépl. 1mm"
#define MSG_MOVE_10MM                       "Dépl. 10mm"
#define MSG_SPEED                           " Vitesse"
#define MSG_BED_Z                           "Plateau Z"
#define MSG_NOZZLE                          "Buse"
#define MSG_BED                             "Plateau"
#define MSG_FAN_SPEED                       "Vitesse ventil."
#define MSG_FLOW                            "Flux"
#define MSG_CONTROL                         "Contrôler"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Facteur"
#define MSG_AUTOTEMP                        "Temp. Auto."
#define MSG_ON                              "Marche "
#define MSG_OFF                             "Arrêt"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Sélectionner"
#define MSG_ACC                             "Accélération"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "Vdépl min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-rétract"
#define MSG_A_TRAVEL                        "A-Dépl."
#define MSG_XSTEPS                          "Xpas/mm"
#define MSG_YSTEPS                          "Ypas/mm"
#define MSG_ZSTEPS                          "Zpas/mm"
#define MSG_ESTEPS                          "Epas/mm"
#define MSG_TEMPERATURE                     "Température"
#define MSG_MOTION                          "Mouvement"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E en mm3"
#define MSG_FILAMENT_DIAM                   "Diam. Fil."
#define MSG_CONTRAST                        "Contraste LCD"
#define MSG_STORE_EPROM                     "Sauver config"
#define MSG_LOAD_EPROM                      "Lire config"
#define MSG_RESTORE_FAILSAFE                "Restaurer défauts"
#define MSG_REFRESH                         "Actualiser"
#define MSG_WATCH                           "Surveiller"
#define MSG_PREPARE                         "Préparer"
#define MSG_TUNE                            "Régler"
#define MSG_PAUSE_PRINT                     "Interrompre impr."
#define MSG_RESUME_PRINT                    "Reprendre impr."
#define MSG_STOP_PRINT                      "Arrêter impr."
#define MSG_CARD_MENU                       "Impr. depuis SD"
#define MSG_NO_CARD                         "Pas de carte"
#define MSG_DWELL                           "Repos..."
#define MSG_USERWAIT                        "Atten. de l'util."
#define MSG_RESUMING                        "Repri. de l'impr."
#define MSG_PRINT_ABORTED                   "Impr. Annulée"
#define MSG_NO_MOVE                         "Moteurs bloqués."
#define MSG_KILLED                          "MORT."
#define MSG_STOPPED                         "STOPPE."
#define MSG_CONTROL_RETRACT                 "Rétraction mm"
#define MSG_CONTROL_RETRACT_SWAP            "Ech. Rétr. mm"
#define MSG_CONTROL_RETRACTF                "Rétraction V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Ech. UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet V"
#define MSG_AUTORETRACT                     "Rétract. Auto."
#define MSG_FILAMENTCHANGE                  "Changer filament"
#define MSG_INIT_SDCARD                     "Init. la carte SD"
#define MSG_CNG_SDCARD                      "Changer de carte"
#define MSG_ZPROBE_OUT                      "Z sonde exté. lit"
#define MSG_BLTOUCH_SELFTEST                "Autotest BLTouch"
#define MSG_BLTOUCH_RESET                   "RaZ BLTouch"
#define MSG_HOME                            "Origine"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "Premier"
#define MSG_ZPROBE_ZOFFSET                  "Decalage Z"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Abandon Butée"
#define MSG_HEATING_FAILED_LCD              "Erreur de chauffe"
#define MSG_ERR_REDUNDANT_TEMP              "Err: TEMP. REDONDANT"
#define MSG_THERMAL_RUNAWAY                 "EMBALLEMENT THERM."
#define MSG_ERR_MAXTEMP                     "Err: TEMP. MAX"
#define MSG_ERR_MINTEMP                     "Err: TEMP. MIN"
#define MSG_ERR_MAXTEMP_BED                 "Err: TEMP. MAX LIT"
#define MSG_ERR_MINTEMP_BED                 "Err: TEMP. MIN LIT"
#define MSG_ERR_Z_HOMING                    "G28 Z interdit"

#define MSG_HALTED                          "IMPR. STOPPEE"
#define MSG_PLEASE_RESET                    "RaZ. SVP"
#define MSG_SHORT_DAY                       "j" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only

#define MSG_HEATING                         "En chauffe..."
#define MSG_HEATING_COMPLETE                "Chauffe terminée"
#define MSG_BED_HEATING                     "Plateau en chauffe.."
#define MSG_BED_DONE                        "Chauffe lit terminée"
#define MSG_DELTA_CALIBRATE                 "Calibration Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibrer X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrer Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrer Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrer centre"

#define MSG_INFO_MENU                       "Infos imprimante"
#define MSG_INFO_PRINTER_MENU               "Infos imprimante"
#define MSG_INFO_STATS_MENU                 "Stats. imprimante"
#define MSG_INFO_BOARD_MENU                 "Infos carte"
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocole"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Nbre impressions"
  #define MSG_INFO_COMPLETED_PRINTS         "Terminées"
  #define MSG_INFO_PRINT_TIME               "Tps impr. total"
  #define MSG_INFO_PRINT_LONGEST            "Impr. la + longue"
  #define MSG_INFO_PRINT_FILAMENT           "Total filament"
#else
  #define MSG_INFO_PRINT_COUNT              "Impressions"
  #define MSG_INFO_COMPLETED_PRINTS         "Terminées"
  #define MSG_INFO_PRINT_TIME               "Total"
  #define MSG_INFO_PRINT_LONGEST            "+ long"
  #define MSG_INFO_PRINT_FILAMENT           "Filament"
#endif

#define MSG_INFO_MIN_TEMP                   "Temp Min"
#define MSG_INFO_MAX_TEMP                   "Temp Max"
#define MSG_INFO_PSU                        "Alimentation"
#define MSG_DRIVE_STRENGTH                  "Puiss. moteur "
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "DAC EEPROM sauv."

#define MSG_FILAMENT_CHANGE_HEADER          "CHANGER FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "CHANGER OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "+ extrusion"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Reprendre impr."

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Attente Démarrage"
  #define MSG_FILAMENT_CHANGE_INIT_2          "du filament"
  #define MSG_FILAMENT_CHANGE_INIT_3          "changer"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "attente de"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "décharger filament"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "insérer filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "et app. bouton"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "pour continuer..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "attente de"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "charger filament"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "attente de"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "extrusion fil."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "attente impression"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "pour reprendre"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Patientez..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Ejection..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insérer et click"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Chargement..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extrusion..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Reprise..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_FR_H
