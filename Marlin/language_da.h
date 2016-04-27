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
 * Danish
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_DA_H
#define LANGUAGE_DA_H

#define MAPPER_C2C3
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define MSG_MAIN                            "Menu"
#define WELCOME_MSG                         MACHINE_NAME " er klar"
#define MSG_SD_INSERTED                     "Kort isat"
#define MSG_SD_REMOVED                      "Kort fjernet"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_AUTO_HOME                       "Home" // G28
#define MSG_COOLDOWN                        "Afkøl"
#define MSG_DISABLE_STEPPERS                "Slå stepper fra"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Tryk for at starte"
#define MSG_LEVEL_BED_DONE                  "Justering er færdig!"
#define MSG_LEVEL_BED_CANCEL                "Annuller"
#define MSG_SET_HOME_OFFSETS                "Sæt forskyding til udgangsposition"
#define MSG_HOME_OFFSETS_APPLIED            "Forskydninger er tilføjet"
#define MSG_SET_ORIGIN                      "Sæt origin"
#define MSG_SWITCH_PS_ON                    "Slå strøm til"
#define MSG_SWITCH_PS_OFF                   "Slå strøm fra"
#define MSG_PREHEAT_PLA                     "Forvarm PLA"
#define MSG_PREHEAT_PLA_N                   "Forvarm PLA "
#define MSG_PREHEAT_PLA_ALL                 "Forvarm PLA Alle"
#define MSG_PREHEAT_PLA_BEDONLY             "Forvarm PLA Bed"
#define MSG_PREHEAT_PLA_SETTINGS            "Forvarm PLA conf"
#define MSG_PREHEAT_ABS                     "Forvarm ABS"
#define MSG_PREHEAT_ABS_N                   "Forvarm ABS "
#define MSG_PREHEAT_ABS_ALL                 "Forvarm ABS Alle"
#define MSG_PREHEAT_ABS_BEDONLY             "Forvarm ABS Bed"
#define MSG_PREHEAT_ABS_SETTINGS            "Forvarm ABS conf"
#define MSG_EXTRUDE                         "Extruder"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "Flyt akser"
#define MSG_MOVE_X                          "Flyt X"
#define MSG_MOVE_Y                          "Flyt Y"
#define MSG_MOVE_Z                          "Flyt Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Flyt 0.1mm"
#define MSG_MOVE_1MM                        "Flyt 1mm"
#define MSG_MOVE_10MM                       "Flyt 10mm"
#define MSG_SPEED                           "Hastighed"
#define MSG_NOZZLE                          "Dyse"
#define MSG_BED                             "Plade"
#define MSG_FAN_SPEED                       "Blæser hastighed"
#define MSG_FLOW                            "Flow"
#define MSG_CONTROL                         "Kontrol"
#define MSG_MIN                             " \002 Min"
#define MSG_MAX                             " \002 Max"
#define MSG_FACTOR                          " \002 Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "Til "
#define MSG_OFF                             "Fra"
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
#define MSG_A_TRAVEL                        "A-rejse"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "Temperatur"
#define MSG_MOTION                          "Motion"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "LCD kontrast"
#define MSG_STORE_EPROM                     "Gem i EEPROM"
#define MSG_LOAD_EPROM                      "Hent fra EEPROM"
#define MSG_RESTORE_FAILSAFE                "Gendan failsafe"
#define MSG_REFRESH                         "Genopfrisk"
#define MSG_WATCH                           "Info skærm"
#define MSG_PREPARE                         "Forbered"
#define MSG_TUNE                            "Tune"
#define MSG_PAUSE_PRINT                     "Pause printet"
#define MSG_RESUME_PRINT                    "Forsæt printet"
#define MSG_STOP_PRINT                      "Stop printet"
#define MSG_CARD_MENU                       "Print fra SD"
#define MSG_NO_CARD                         "Intet SD kort"
#define MSG_DWELL                           "Dvale..."
#define MSG_USERWAIT                        "Venter på bruger..."
#define MSG_RESUMING                        "Forsætter printet"
#define MSG_PRINT_ABORTED                   "Print annulleret"
#define MSG_NO_MOVE                         "Ingen bevægelse."
#define MSG_KILLED                          "DRÆBT. "
#define MSG_STOPPED                         "STOPPET. "
#define MSG_CONTROL_RETRACT                 "Tilbagetraek mm"
#define MSG_CONTROL_RETRACT_SWAP            "Skift Re.mm"
#define MSG_CONTROL_RETRACTF                "Tilbagetræk V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Skift filament"
#define MSG_INIT_SDCARD                     "Init. SD card"
#define MSG_CNG_SDCARD                      "Skift SD kort"
#define MSG_ZPROBE_OUT                      "Probe udenfor plade"
#define MSG_YX_UNHOMED                      "Home X/Y før Z"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_END_HOUR                        "Timer"
#define MSG_END_MINUTE                      "Minutter"
#define MSG_HEATING                         "Opvarmer..."
#define MSG_HEATING_COMPLETE                "Opvarmet"
#define MSG_BED_HEATING                     "Opvarmer plade"
#define MSG_BED_DONE                        "Plade opvarmet"

#define MSG_BED_Z                           "Plade Z"
#define MSG_HEATING_FAILED_LCD              "Opvarmning mislykkedes"
#define MSG_ERR_REDUNDANT_TEMP              "Fejl: reserve temp"
#define MSG_THERMAL_RUNAWAY                 "Temp løber løbsk"
#define MSG_ERR_MAXTEMP                     "Fejl: Maks temp"
#define MSG_ERR_MINTEMP                     "Fejl: Min temp"
#define MSG_ERR_MAXTEMP_BED                 "Fejl: Maks P temp"
#define MSG_ERR_MINTEMP_BED                 "Fejl: Min P temp"

#ifdef DELTA_CALIBRATION_MENU
  #define MSG_DELTA_CALIBRATE               "Delta Kalibrering"
  #define MSG_DELTA_CALIBRATE_X             "Kalibrer X"
  #define MSG_DELTA_CALIBRATE_Y             "Kalibrer Y"
  #define MSG_DELTA_CALIBRATE_Z             "Kalibrer Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "Kalibrerings Center"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_DA_H
