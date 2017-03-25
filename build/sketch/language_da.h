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
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " er klar"
#define MSG_SD_INSERTED                     "Kort isat"
#define MSG_SD_REMOVED                      "Kort fjernet"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Menu"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Slå alle steppere fra"
#define MSG_AUTO_HOME                       "Auto Home" // G28
#define MSG_AUTO_HOME_X                     "Home X"
#define MSG_AUTO_HOME_Y                     "Home Y"
#define MSG_AUTO_HOME_Z                     "Home Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Klik når du er klar"
#define MSG_LEVEL_BED_NEXT_POINT            "Næste punkt"
#define MSG_LEVEL_BED_DONE                  "Bed level er færdig!"
#define MSG_LEVEL_BED_CANCEL                "Annuller bed level"
#define MSG_SET_HOME_OFFSETS                "Sæt forsk. af home"
#define MSG_HOME_OFFSETS_APPLIED            "Forsk. er nu aktiv"
#define MSG_SET_ORIGIN                      "Sæt origin"
#define MSG_PREHEAT_1                       "Forvarm PLA"
#define MSG_PREHEAT_1_N                     "Forvarm PLA "
#define MSG_PREHEAT_1_ALL                   "Forvarm PLA Alle"
#define MSG_PREHEAT_1_BEDONLY               "Forvarm PLA Bed"
#define MSG_PREHEAT_1_SETTINGS              "Forvarm PLA conf"
#define MSG_PREHEAT_2                       "Forvarm ABS"
#define MSG_PREHEAT_2_N                     "Forvarm ABS "
#define MSG_PREHEAT_2_ALL                   "Forvarm ABS Alle"
#define MSG_PREHEAT_2_BEDONLY               "Forvarm ABS Bed"
#define MSG_PREHEAT_2_SETTINGS              "Forvarm ABS conf"
#define MSG_COOLDOWN                        "Afkøl"
#define MSG_SWITCH_PS_ON                    "Slå strøm til"
#define MSG_SWITCH_PS_OFF                   "Slå strøm fra"
#define MSG_EXTRUDE                         "Extruder"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "Flyt akser"
#define MSG_LEVEL_BED                       "Juster bed"
#define MSG_MOVE_X                          "Flyt X"
#define MSG_MOVE_Y                          "Flyt Y"
#define MSG_MOVE_Z                          "Flyt Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Flyt 0.1mm"
#define MSG_MOVE_1MM                        "Flyt 1mm"
#define MSG_MOVE_10MM                       "Flyt 10mm"
#define MSG_SPEED                           "Hastighed"
#define MSG_BED_Z                           "Plade Z"
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
#define MSG_SELECT                          "Vælg"
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
#define MSG_A_TRAVEL                        "A-rejse"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "Temperatur"
#define MSG_MOTION                          "Bevægelse"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E i mm3"
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
#define MSG_CONTROL_RETRACT                 "Tilbagetræk mm"
#define MSG_CONTROL_RETRACT_SWAP            "Skift Re.mm"
#define MSG_CONTROL_RETRACTF                "Tilbagetræk V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Skift UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Skift filament"
#define MSG_INIT_SDCARD                     "Init. SD card"
#define MSG_CNG_SDCARD                      "Skift SD kort"
#define MSG_ZPROBE_OUT                      "Probe udenfor plade"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Selv-Test"
#define MSG_BLTOUCH_RESET                   "Reset BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "først"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Opvarmning fejlet"
#define MSG_ERR_REDUNDANT_TEMP              "Fejl: reserve temp"
#define MSG_THERMAL_RUNAWAY                 "Temp løber løbsk"
#define MSG_ERR_MAXTEMP                     "Fejl: Maks temp"
#define MSG_ERR_MINTEMP                     "Fejl: Min temp"
#define MSG_ERR_MAXTEMP_BED                 "Fejl: Maks Plade temp"
#define MSG_ERR_MINTEMP_BED                 "Fejl: Min Plade temp"
#define MSG_ERR_Z_HOMING                    "G28 Z Forbudt"
#define MSG_HALTED                          "PRINTER STOPPET"
#define MSG_PLEASE_RESET                    "Reset Venligst"
#define MSG_SHORT_DAY                       "d" // Kun et bogstav
#define MSG_SHORT_HOUR                      "h" // Kun et bogstav
#define MSG_SHORT_MINUTE                    "m" // Kun et bogstav
#define MSG_HEATING                         "Opvarmer..."
#define MSG_HEATING_COMPLETE                "Opvarmet"
#define MSG_BED_HEATING                     "Opvarmer plade"
#define MSG_BED_DONE                        "Plade opvarmet"
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrering"
#define MSG_DELTA_CALIBRATE_X               "Kalibrer X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibrer Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibrer Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibrerings Center"

#define MSG_INFO_MENU                       "Om Printer"
#define MSG_INFO_PRINTER_MENU               "Printer Info"
#define MSG_INFO_STATS_MENU                 "Printer Stats"
#define MSG_INFO_BOARD_MENU                 "Kort Info"
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocol"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Ant. Prints"
  #define MSG_INFO_COMPLETED_PRINTS         "Færdige"
  #define MSG_INFO_PRINT_TIME               "Total print tid"
  #define MSG_INFO_PRINT_LONGEST            "Længste print"
  #define MSG_INFO_PRINT_FILAMENT           "Total Extruderet"
#else
  #define MSG_INFO_PRINT_COUNT              "Prints"
  #define MSG_INFO_COMPLETED_PRINTS         "Færdige"
  #define MSG_INFO_PRINT_TIME               "Total"
  #define MSG_INFO_PRINT_LONGEST            "Længste"
  #define MSG_INFO_PRINT_FILAMENT           "Extruderet"
#endif

#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Strømfors."

#define MSG_DRIVE_STRENGTH                  "Driv Styrke"
#define MSG_DAC_PERCENT                     "Driv %"
#define MSG_DAC_EEPROM_WRITE                "DAC EEPROM Skriv"

#define MSG_FILAMENT_CHANGE_HEADER          "SKIFT FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Skift muligheder:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extruder mere"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Forsæt print"

#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Vent på start"
  #define MSG_FILAMENT_CHANGE_INIT_2          "af filament"
  #define MSG_FILAMENT_CHANGE_INIT_3          "skift"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Vent på"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament udskyd."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Indsæt filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "og tryk på knap"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "for at fortsætte..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Vent på"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filament indtag"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Vent på"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament extrudering"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Vent på at print"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "fortsætter"
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Vent venligst..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Udskyder..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Indsæt og klik"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Indtager..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extrudere..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Fortsætter..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_DA_H
