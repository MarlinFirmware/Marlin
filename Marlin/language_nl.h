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
 * Dutch
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_NL_H
#define LANGUAGE_NL_H

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " gereed."
#define MSG_SD_INSERTED                     "Kaart ingestoken"
#define MSG_SD_REMOVED                      "Kaart verwijderd"
#define MSG_MAIN                            "Main"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Motoren uit"
#define MSG_AUTO_HOME                       "Auto home"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
#define MSG_LEVEL_BED_CANCEL                "Cancel"
#define MSG_SET_HOME_OFFSETS                "Zet home offsets"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
#define MSG_SET_ORIGIN                      "Nulpunt instellen"
#define MSG_PREHEAT_PLA                     "PLA voorverwarmen"
#define MSG_PREHEAT_PLA_N                   "PLA voorverw. "
#define MSG_PREHEAT_PLA_ALL                 "PLA voorverw. aan"
#define MSG_PREHEAT_PLA_BEDONLY             "PLA voorverw. Bed"
#define MSG_PREHEAT_PLA_SETTINGS            "PLA verw. conf"
#define MSG_PREHEAT_ABS                     "ABS voorverwarmen"
#define MSG_PREHEAT_ABS_N                   "ABS voorverw. "
#define MSG_PREHEAT_ABS_ALL                 "ABS voorverw. aan"
#define MSG_PREHEAT_ABS_BEDONLY             "ABS voorverw. Bed"
#define MSG_PREHEAT_ABS_SETTINGS            "ABS verw. conf"
#define MSG_COOLDOWN                        "Afkoelen"
#define MSG_SWITCH_PS_ON                    "Stroom aan"
#define MSG_SWITCH_PS_OFF                   "Stroom uit"
#define MSG_EXTRUDE                         "Extrude"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "As verplaatsen"
#define MSG_MOVE_X                          "Verplaats X"
#define MSG_MOVE_Y                          "Verplaats Y"
#define MSG_MOVE_Z                          "Verplaats Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Verplaats 0.1mm"
#define MSG_MOVE_1MM                        "Verplaats 1mm"
#define MSG_MOVE_10MM                       "Verplaats 10mm"
#define MSG_SPEED                           "Snelheid"
#define MSG_NOZZLE                          "Nozzle"
#define MSG_BED                             "Bed"
#define MSG_FAN_SPEED                       "Fan snelheid"
#define MSG_FLOW                            "Flow"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "Aan "
#define MSG_OFF                             "Uit"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Versn"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_X                               "x"
#define MSG_Y                               "y"
#define MSG_Z                               "z"
#define MSG_E                               "e"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "Temperatuur"
#define MSG_MOTION                          "Beweging"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "LCD contrast"
#define MSG_STORE_EPROM                     "Geheugen opslaan"
#define MSG_LOAD_EPROM                      "Geheugen laden"
#define MSG_RESTORE_FAILSAFE                "Noodstop reset"
#define MSG_REFRESH                         "Ververs"
#define MSG_WATCH                           "Info scherm"
#define MSG_PREPARE                         "Voorbereiden"
#define MSG_TUNE                            "Afstellen"
#define MSG_PAUSE_PRINT                     "Print pauzeren"
#define MSG_RESUME_PRINT                    "Print hervatten"
#define MSG_STOP_PRINT                      "Print stoppen"
#define MSG_CARD_MENU                       "Print van SD"
#define MSG_NO_CARD                         "Geen SD kaart"
#define MSG_DWELL                           "Slapen..."
#define MSG_USERWAIT                        "Wachten..."
#define MSG_RESUMING                        "Print hervatten"
#define MSG_PRINT_ABORTED                   "Print afgebroken"
#define MSG_NO_MOVE                         "Geen beweging."
#define MSG_KILLED                          "Afgebroken. "
#define MSG_STOPPED                         "Gestopt. "
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Ruil Retract mm"
#define MSG_CONTROL_RETRACTF                "Retract  F"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Ruil UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  F"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Verv. Filament"
#define MSG_INIT_SDCARD                     "Init. SD kaart"
#define MSG_CNG_SDCARD                      "Verv. SD Kaart"
#define MSG_ZPROBE_OUT                      "Z probe uit. bed"
#define MSG_YX_UNHOMED                      "Home X/Y voor Z"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystap X"
#define MSG_BABYSTEP_Y                      "Babystap Y"
#define MSG_BABYSTEP_Z                      "Babystap Z"
#define MSG_ENDSTOP_ABORT                   "Endstop afbr."
#define MSG_END_HOUR                        "uur"
#define MSG_END_MINUTE                      "minuten"
#define MSG_BED_Z                           "Bed Z"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_HEATING_FAILED_LCD              "voorverw. fout"
#define MSG_ERR_REDUNDANT_TEMP              "Fout: Redundant temp fout"
#define MSG_THERMAL_RUNAWAY                 "Thermische wegloop"
#define MSG_ERR_MAXTEMP                     "Fout: Max. temp"
#define MSG_ERR_MINTEMP                     "Fout: Min. temp"
#define MSG_ERR_MAXTEMP_BED                 "Fout: Max. temp bed"
#define MSG_ERR_MINTEMP_BED                 "Fout: Min. temp bed"
#define MSG_HEATING                         "Voorwarmen..."
#define MSG_HEATING_COMPLETE                "voorverw. kompleet"
#define MSG_BED_HEATING                     "Bed voorverwarmen"
#define MSG_BED_DONE                        "Bed is voorverw."

#if ENABLED(DELTA_CALIBRATION_MENU)
  #define MSG_DELTA_CALIBRATE               "Delta Calibratie"
  #define MSG_DELTA_CALIBRATE_X             "Kalibreer X"
  #define MSG_DELTA_CALIBRATE_Y             "Kalibreer Y"
  #define MSG_DELTA_CALIBRATE_Z             "Kalibreer Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "Kalibreer Midden"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_NL_H
