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
 * Croatian (Hrvatski)
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_HR_H
#define LANGUAGE_HR_H

#define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.

#define WELCOME_MSG                         MACHINE_NAME " spreman."
#define MSG_SD_INSERTED                     "SD kartica umetnuta"
#define MSG_SD_REMOVED                      "SD kartica uklonjena"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Main"
#define MSG_AUTOSTART                       "Auto pokretanje"
#define MSG_DISABLE_STEPPERS                "Ugasi steppere"
#define MSG_AUTO_HOME                       "Automatski homing"
#define MSG_AUTO_HOME_X                     "Home-aj X"
#define MSG_AUTO_HOME_Y                     "Home-aj Y"
#define MSG_AUTO_HOME_Z                     "Home-aj Z"
#define MSG_LEVEL_BED_HOMING                "Home-aj XYZ"
#define MSG_LEVEL_BED_WAITING               "Klikni za početak"
#define MSG_LEVEL_BED_NEXT_POINT            "Sljedeća točka"
#define MSG_LEVEL_BED_DONE                  "Niveliranje gotovo!"
#define MSG_LEVEL_BED_CANCEL                "Otkaži"
#define MSG_SET_HOME_OFFSETS                "Postavi home offsete"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets postavljeni"
#define MSG_SET_ORIGIN                      "Postavi ishodište"
#define MSG_PREHEAT_1                       "Predgrij PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Sve"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " conf"
#define MSG_PREHEAT_2                       "Predgrij ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Sve"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " conf"
#define MSG_COOLDOWN                        "Hlađenje"
#define MSG_SWITCH_PS_ON                    "Uključi napajanje"
#define MSG_SWITCH_PS_OFF                   "Isključi napajanje"
#define MSG_EXTRUDE                         "Extrude"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "Miči os"
#define MSG_LEVEL_BED                       "Niveliraj bed"
#define MSG_MOVE_X                          "Miči X"
#define MSG_MOVE_Y                          "Miči Y"
#define MSG_MOVE_Z                          "Miči Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Miči 0.1mm"
#define MSG_MOVE_1MM                        "Miči 1mm"
#define MSG_MOVE_10MM                       "Miči 10mm"
#define MSG_SPEED                           "Brzina"
#define MSG_BED_Z                           "Bed Z"
#define MSG_NOZZLE                          "Dizna"
#define MSG_BED                             "Bed"
#define MSG_FAN_SPEED                       "Brzina ventilatora"
#define MSG_FLOW                            "Flow"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On "
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Odaberi"
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
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "Temperature"
#define MSG_MOTION                          "Gibanje"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "Kontrast LCD-a"
#define MSG_STORE_EPROM                     "Pohrani u memoriju"
#define MSG_LOAD_EPROM                      "Učitaj memoriju"
#define MSG_RESTORE_FAILSAFE                "Učitaj failsafe"
#define MSG_REFRESH                         "Osvježi"
#define MSG_WATCH                           "Info screen"
#define MSG_PREPARE                         "Pripremi"
#define MSG_TUNE                            "Tune"
#define MSG_PAUSE_PRINT                     "Pauziraj print"
#define MSG_RESUME_PRINT                    "Nastavi print"
#define MSG_STOP_PRINT                      "Zaustavi print"
#define MSG_CARD_MENU                       "Printaj s SD kartice"
#define MSG_NO_CARD                         "Nema SD kartice"
#define MSG_DWELL                           "Sleep..."
#define MSG_USERWAIT                        "Čekaj korisnika..."
#define MSG_RESUMING                        "Nastavljam print"
#define MSG_PRINT_ABORTED                   "Print otkazan"
#define MSG_NO_MOVE                         "No move."
#define MSG_KILLED                          "KILLED. "
#define MSG_STOPPED                         "ZAUSTAVLJEN. "
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Promijeni filament"
#define MSG_INIT_SDCARD                     "Init. SD karticu"
#define MSG_CNG_SDCARD                      "Promijeni SD karticu"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   "Reset BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Grijanje neuspješno"
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    "G28 Z Forbidden"
#define MSG_HALTED                          "PRINTER HALTED"
#define MSG_PLEASE_RESET                    "Please reset"
#define MSG_SHORT_DAY                       "d" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Grijanje..."
#define MSG_HEATING_COMPLETE                "Grijanje gotovo."
#define MSG_BED_HEATING                     "Grijanje Bed-a."
#define MSG_BED_DONE                        "Bed gotov."
#define MSG_DELTA_CALIBRATE                 "Delta Kalibracija"
#define MSG_DELTA_CALIBRATE_X               "Kalibriraj X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibriraj Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibriraj Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibriraj Središte"
#define MSG_INFO_MENU                       "O printeru"
#define MSG_INFO_PRINTER_MENU               "Podaci o printeru"
#define MSG_INFO_STATS_MENU                 "Statistika printera"
#define MSG_INFO_BOARD_MENU                 "Podaci o elektronici"
#define MSG_INFO_THERMISTOR_MENU            "Termistori"
#define MSG_INFO_EXTRUDERS                  "Extruderi"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protokol"
#define MSG_LIGHTS_ON                       "Upali osvjetljenje"
#define MSG_LIGHTS_OFF                      "Ugasi osvjetljenje"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Broj printova"
  #define MSG_INFO_COMPLETED_PRINTS         "Završeni"
  #define MSG_INFO_PRINT_TIME               "Ukupno printanja"
  #define MSG_INFO_PRINT_LONGEST            "Najduži print"
  #define MSG_INFO_PRINT_FILAMENT           "Extrudirano ukupno"
#else
  #define MSG_INFO_PRINT_COUNT              "Printovi"
  #define MSG_INFO_COMPLETED_PRINTS         "Završeni"
  #define MSG_INFO_PRINT_TIME               "Ukupno"
  #define MSG_INFO_PRINT_LONGEST            "Najduži"
  #define MSG_INFO_PRINT_FILAMENT           "Extrudirano"
#endif

#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Napajanje"
#define MSG_DRIVE_STRENGTH                  "Drive Strength"
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "DAC EEPROM Write"

#define MSG_FILAMENT_CHANGE_HEADER          "CHANGE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "CHANGE OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrudiraj više"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Nastavi print"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Čekaj početak"
  #define MSG_FILAMENT_CHANGE_INIT_2          "filamenta"
  #define MSG_FILAMENT_CHANGE_INIT_3          "promijeni"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Čekaj"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament unload"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Umetni filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "i pritisni tipku"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "za nastavak..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Pričekaj"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filament load"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Pričekaj"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament extrude"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Wait for print"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "to resume"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Pričekaj..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Ejecting..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insert and Click"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Loading..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extrudiranje..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Nastavljam..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_HR_H
