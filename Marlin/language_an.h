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
 * Aragonese
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_AN_H
#define LANGUAGE_AN_H

#define MAPPER_NON
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " parada."
#define MSG_SD_INSERTED                     "Tarcheta colocada"
#define MSG_SD_REMOVED                      "Tarcheta retirada"
#define MSG_MAIN                            "Menu prencipal"
#define MSG_AUTOSTART                       " Autostart"
#define MSG_DISABLE_STEPPERS                "Amortar motors"
#define MSG_AUTO_HOME                       "Levar a l'orichen"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_SET_HOME_OFFSETS                "Set home offsets"
#define MSG_SET_ORIGIN                      "Establir zero"
#define MSG_PREHEAT_PLA                     "Precalentar PLA"
#define MSG_PREHEAT_PLA_N                   "Precalentar PLA "
#define MSG_PREHEAT_PLA_ALL                 "Precalentar PLA a"
#define MSG_PREHEAT_PLA_BEDONLY             "Prec. PLA Base"
#define MSG_PREHEAT_PLA_SETTINGS            "Achustar tem. PLA"
#define MSG_PREHEAT_ABS                     "Precalentar ABS"
#define MSG_PREHEAT_ABS_N                   "Precalentar ABS "
#define MSG_PREHEAT_ABS_ALL                 "Precalentar ABS a"
#define MSG_PREHEAT_ABS_BEDONLY             "Prec. ABS Base"
#define MSG_PREHEAT_ABS_SETTINGS            "Achustar tem. ABS"
#define MSG_COOLDOWN                        "Enfriar"
#define MSG_SWITCH_PS_ON                    "Enchegar Fuent"
#define MSG_SWITCH_PS_OFF                   "Desenchegar Fuent"
#define MSG_EXTRUDE                         "Extruir"
#define MSG_RETRACT                         "Retraer"
#define MSG_MOVE_AXIS                       "Mover Eixes"
#define MSG_MOVE_X                          "Move X"
#define MSG_MOVE_Y                          "Move Y"
#define MSG_MOVE_Z                          "Move Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Move 0.1mm"
#define MSG_MOVE_1MM                        "Move 1mm"
#define MSG_MOVE_10MM                       "Move 10mm"
#define MSG_SPEED                           "Velocidat"
#define MSG_NOZZLE                          "Nozzle"
#define MSG_BED                             "Base"
#define MSG_FAN_SPEED                       "Ixoriador"
#define MSG_FLOW                            "Fluxo"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On"
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Acel"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ves-jerk"
#define MSG_VMAX                            "Vmax"
#define MSG_X                               "x"
#define MSG_Y                               "y"
#define MSG_Z                               "z"
#define MSG_E                               "y"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax"
#define MSG_A_RETRACT                       "A-retrac."
#define MSG_XSTEPS                          "X trangos/mm"
#define MSG_YSTEPS                          "Y trangos/mm"
#define MSG_ZSTEPS                          "Z trangos/mm"
#define MSG_ESTEPS                          "E trangos/mm"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_MOTION                          "Movimiento"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "Contrast"
#define MSG_STORE_EPROM                     "Alzar Memoria"
#define MSG_LOAD_EPROM                      "Cargar Memoria"
#define MSG_RESTORE_FAILSAFE                "Rest. d'emerchen."
#define MSG_REFRESH                         "Tornar a cargar"
#define MSG_WATCH                           "Monitorizar"
#define MSG_PREPARE                         "Preparar"
#define MSG_TUNE                            "Achustar"
#define MSG_PAUSE_PRINT                     "Pausar impresion"
#define MSG_RESUME_PRINT                    "Contin. impresion"
#define MSG_STOP_PRINT                      "Detener Impresion"
#define MSG_CARD_MENU                       "Menu de SD"
#define MSG_NO_CARD                         "No i hai tarcheta"
#define MSG_DWELL                           "Reposo..."
#define MSG_USERWAIT                        "Asperan. ordines"
#define MSG_RESUMING                        "Contin. impresion"
#define MSG_PRINT_ABORTED                   "Print aborted"
#define MSG_NO_MOVE                         "Sin movimiento"
#define MSG_KILLED                          "ATURADA D'EMERCH."
#define MSG_STOPPED                         "ATURADA."
#define MSG_CONTROL_RETRACT                 "Retraer mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Retraer mm"
#define MSG_CONTROL_RETRACTF                "Retraer  F"
#define MSG_CONTROL_RETRACT_ZLIFT           "Devantar mm"
#define MSG_CONTROL_RETRACT_RECOVER         "DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Swap DesRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "DesRet F"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Cambear"
#define MSG_INIT_SDCARD                     "Encetan. tarcheta"
#define MSG_CNG_SDCARD                      "Cambiar tarcheta"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_YX_UNHOMED                      "Home X/Y before Z"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_END_HOUR                        "hours"
#define MSG_END_MINUTE                      "minutes"

#if ENABLED(DELTA_CALIBRATION_MENU)
  #define MSG_DELTA_CALIBRATE               "Delta Calibration"
  #define MSG_DELTA_CALIBRATE_X             "Calibrate X"
  #define MSG_DELTA_CALIBRATE_Y             "Calibrate Y"
  #define MSG_DELTA_CALIBRATE_Z             "Calibrate Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "Calibrate Center"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_AN_H
