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

// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " prete."
#define MSG_SD_INSERTED                     "Carte inseree"
#define MSG_SD_REMOVED                      "Carte retiree"
#define MSG_MAIN                            "Menu principal"
#define MSG_AUTOSTART                       "Demarrage auto"
#define MSG_DISABLE_STEPPERS                "Arreter moteurs"
#define MSG_AUTO_HOME                       "Origine auto."
#define MSG_LEVEL_BED_HOMING                "Origine XYZ"
#define MSG_LEVEL_BED_WAITING               "Cliquer pour commencer"
#define MSG_LEVEL_BED_DONE                  "Mise a niveau OK!"
#define MSG_LEVEL_BED_CANCEL                "Annuler"
#define MSG_SET_HOME_OFFSETS                "Regler decal. origine"
#define MSG_HOME_OFFSETS_APPLIED            "Decalages appliques"
#define MSG_SET_ORIGIN                      "Regler origine"
#define MSG_PREHEAT_1                       "Prechauffage PLA"
#define MSG_PREHEAT_1_N                     "Prechauff. PLA "
#define MSG_PREHEAT_1_ALL                   "Prech. PLA Tout"
#define MSG_PREHEAT_1_BEDONLY               "Prech. PLA Plateau"
#define MSG_PREHEAT_1_SETTINGS              "Regl. prech. PLA"
#define MSG_PREHEAT_2                       "Prechauffage ABS"
#define MSG_PREHEAT_2_N                     "Prechauff. ABS "
#define MSG_PREHEAT_2_ALL                   "Prech. ABS Tout"
#define MSG_PREHEAT_2_BEDONLY               "Prech. ABS Plateau"
#define MSG_PREHEAT_2_SETTINGS              "Regl. prech. ABS"
#define MSG_COOLDOWN                        "Refroidir"
#define MSG_SWITCH_PS_ON                    "Allumer alim."
#define MSG_SWITCH_PS_OFF                   "Eteindre alim."
#define MSG_EXTRUDE                         "Extrusion"
#define MSG_RETRACT                         "Retraction"
#define MSG_MOVE_AXIS                       "Deplacer un axe"
#define MSG_LEVEL_BED                       "Regl. Niv. Plateau"
#define MSG_MOVE_X                          "Depl. X"
#define MSG_MOVE_Y                          "Depl. Y"
#define MSG_MOVE_Z                          "Depl. Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Depl. 0.1mm"
#define MSG_MOVE_1MM                        "Depl. 1mm"
#define MSG_MOVE_10MM                       "Depl. 10mm"
#define MSG_SPEED                           " Vitesse"
#define MSG_BED_Z                           "Plateau Z"
#define MSG_NOZZLE                          "Buse"
#define MSG_BED                             "Plateau"
#define MSG_FAN_SPEED                       "Vite. ventilateur"
#define MSG_FLOW                            "Flux"
#define MSG_CONTROL                         "Controler"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Facteur"
#define MSG_AUTOTEMP                        "Temp. Auto."
#define MSG_ON                              "Marche "
#define MSG_OFF                             "Arret"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Accel"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax"
#define MSG_X                               "X"
#define MSG_Y                               "Y"
#define MSG_Z                               "Z"
#define MSG_E                               "E"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "Vdepl min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-Depl."
#define MSG_XSTEPS                          "Xpas/mm"
#define MSG_YSTEPS                          "Ypas/mm"
#define MSG_ZSTEPS                          "Zpas/mm"
#define MSG_ESTEPS                          "Epas/mm"
#define MSG_TEMPERATURE                     "Temperature"
#define MSG_MOTION                          "Mouvement"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E en mm3"
#define MSG_FILAMENT_DIAM                   "Diam. Fil."
#define MSG_CONTRAST                        "Contraste LCD"
#define MSG_STORE_EPROM                     "Sauver config"
#define MSG_LOAD_EPROM                      "Lire config"
#define MSG_RESTORE_FAILSAFE                "Restaurer defauts"
#define MSG_REFRESH                         "Actualiser"
#define MSG_WATCH                           "Surveiller"
#define MSG_PREPARE                         "Preparer"
#define MSG_TUNE                            "Regler"
#define MSG_PAUSE_PRINT                     "Interrompre impr."
#define MSG_RESUME_PRINT                    "Reprendre impr."
#define MSG_STOP_PRINT                      "Arreter impr."
#define MSG_CARD_MENU                       "Impr. depuis SD"
#define MSG_NO_CARD                         "Pas de carte"
#define MSG_DWELL                           "Repos..."
#define MSG_USERWAIT                        "Atten. de l'util."
#define MSG_RESUMING                        "Repri. de l'impr."
#define MSG_PRINT_ABORTED                   "Impr. Annulee"
#define MSG_NO_MOVE                         "Aucun mouvement."
#define MSG_KILLED                          "MORT."
#define MSG_STOPPED                         "STOPPE."
#define MSG_CONTROL_RETRACT                 "Retraction mm"
#define MSG_CONTROL_RETRACT_SWAP            "Ech. Retr. mm"
#define MSG_CONTROL_RETRACTF                "Retraction V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Ech. UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet V"
#define MSG_AUTORETRACT                     "Retract. Auto."
#define MSG_FILAMENTCHANGE                  "Changer filament"
#define MSG_INIT_SDCARD                     "Init. la carte SD"
#define MSG_CNG_SDCARD                      "Changer de carte"
#define MSG_ZPROBE_OUT                      "Z sonde exte. lit"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Decalage Z"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Butee abandon"
#define MSG_HEATING_FAILED_LCD              "Erreur de chauffe"
#define MSG_ERR_REDUNDANT_TEMP              "Err: ERREUR TEMP. REDONDANTE"
#define MSG_THERMAL_RUNAWAY                 "EMBALLEMENT THERMIQUE"
#define MSG_ERR_MAXTEMP                     "Err: TEMP. MAX"
#define MSG_ERR_MINTEMP                     "Err: TEMP. MIN"
#define MSG_ERR_MAXTEMP_BED                 "Err: TEMP. MAX PLATEAU"
#define MSG_ERR_MINTEMP_BED                 "Err: TEMP. MIN PLATEAU"
#define MSG_HEATING                         "En chauffe..."
#define MSG_HEATING_COMPLETE                "Chauffe terminee"
#define MSG_BED_HEATING                     "Plateau en chauffe..."
#define MSG_BED_DONE                        "Chauffe plateau terminee"
#define MSG_DELTA_CALIBRATE                 "Calibration Delta"
#define MSG_DELTA_CALIBRATE_X               "Calibrer X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrer Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrer Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrer centre"

#endif // LANGUAGE_FR_H
