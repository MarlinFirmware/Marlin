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

//#define SIMULATE_ROMFONT //Comment in to see what is seen on the character based displays
#if DISABLED(SIMULATE_ROMFONT) && DISABLED(DISPLAY_CHARSET_ISO10646_1) && DISABLED(DISPLAY_CHARSET_ISO10646_5) && DISABLED(DISPLAY_CHARSET_ISO10646_KANA) && DISABLED(DISPLAY_CHARSET_ISO10646_GREEK) && DISABLED(DISPLAY_CHARSET_ISO10646_CN)
  #define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.
#endif


#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME " er klar"
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     "Kort isat"
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      "Kort fjernet"
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            "Menu"
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       "Autostart"
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                "Slå alle steppere fra"
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       "Auto Home" // G28
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     "Home X"
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     "Home Y"
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     "Home Z"
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               "Tryk for at starte bed level"
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            "Næste punkt"
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  "Bed level er færdig!"
#endif
#ifndef MSG_LEVEL_BED_CANCEL
  #define MSG_LEVEL_BED_CANCEL                "Annuller bed level"
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                "Sæt forskyding af home"
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            "Forskydninger af home pos. er tilføjet"
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      "Sæt origin"
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       "Forvarm PLA"
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     "Forvarm PLA "
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   "Forvarm PLA Alle"
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               "Forvarm PLA Bed"
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              "Forvarm PLA conf"
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       "Forvarm ABS"
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     "Forvarm ABS "
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   "Forvarm ABS Alle"
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               "Forvarm ABS Bed"
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              "Forvarm ABS conf"
#endif
#ifndef MSG_H1
  #define MSG_H1                              "1"
#endif
#ifndef MSG_H2
  #define MSG_H2                              "2"
#endif
#ifndef MSG_H3
  #define MSG_H3                              "3"
#endif
#ifndef MSG_H4
  #define MSG_H4                              "4"
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        "Afkøl"
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    "Slå strøm til"
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   "Slå strøm fra"
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         "Extruder"
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         "Retract"
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       "Flyt akser"
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       "Juster bed"
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          "Flyt X"
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          "Flyt Y"
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          "Flyt Z"
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          "Extruder"
#endif
#ifndef MSG_MOVE_E1
  #define MSG_MOVE_E1                         "1"
#endif
#ifndef MSG_MOVE_E2
  #define MSG_MOVE_E2                         "2"
#endif
#ifndef MSG_MOVE_E3
  #define MSG_MOVE_E3                         "3"
#endif
#ifndef MSG_MOVE_E4
  #define MSG_MOVE_E4                         "4"
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       "Flyt 0.1mm"
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        "Flyt 1mm"
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       "Flyt 10mm"
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           "Hastighed"
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           "Plade Z"
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          "Dyse"
#endif
#ifndef MSG_N1

  #define MSG_N1                              " 1"
#endif
#ifndef MSG_N2
  #define MSG_N2                              " 2"
#endif
#ifndef MSG_N3
  #define MSG_N3                              " 3"
#endif
#ifndef MSG_N4
  #define MSG_N4                              " 4"
#endif
#ifndef MSG_BED
  #define MSG_BED                             "Plade"
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       "Blæser hastighed"
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            "Flow"
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         "Kontrol"
#endif
#ifndef MSG_MIN
  #define MSG_MIN                             " \002 Min"
#endif
#ifndef MSG_MAX
  #define MSG_MAX                             " \002 Max"
#endif
#ifndef MSG_FACTOR
  #define MSG_FACTOR                          " \002 Fact"
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP                        "Autotemp"
#endif
#ifndef MSG_ON
  #define MSG_ON                              "Til "
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             "Fra"
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P                           "PID-P"
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I                           "PID-I"
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D                           "PID-D"
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C                           "PID-C"
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT                          "Vælg"
#endif
#ifndef MSG_E1
  #define MSG_E1                              " E1"
#endif
#ifndef MSG_E2
  #define MSG_E2                              " E2"
#endif
#ifndef MSG_E3
  #define MSG_E3                              " E3"
#endif
#ifndef MSG_E4
  #define MSG_E4                             " E4"
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             "Accel"
#endif
#ifndef MSG_VXY_JERK
  #define MSG_VXY_JERK                        "Vxy-jerk"
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK                         "Vz-jerk"
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         "Ve-jerk"
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX                            "Vmax "
#endif
#ifndef MSG_X
  #define MSG_X                               "X"
#endif
#ifndef MSG_Y
  #define MSG_Y                               "Y"
#endif
#ifndef MSG_Z
  #define MSG_Z                               "Z"
#endif
#ifndef MSG_E
  #define MSG_E                               "E"
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN                            "Vmin"
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN                       "VTrav min"
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            "Amax "
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       "A-retract"
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        "A-rejse"
#endif
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS                          "Xsteps/mm"
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS                          "Ysteps/mm"
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS                          "Zsteps/mm"
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          "Esteps/mm"
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     "Temperatur"
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          "Bevægelse"
#endif
#ifndef MSG_VOLUMETRIC
  #define MSG_VOLUMETRIC                      "Filament"
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   "Fil. Dia."
#endif
#ifndef MSG_DIAM_E1
  #define MSG_DIAM_E1                         " 1"
#endif
#ifndef MSG_DIAM_E2
  #define MSG_DIAM_E2                         " 2"
#endif
#ifndef MSG_DIAM_E3
  #define MSG_DIAM_E3                         " 3"
#endif
#ifndef MSG_DIAM_E4
  #define MSG_DIAM_E4                         " 4"
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST                        "LCD kontrast"
#endif
#ifndef MSG_STORE_EPROM
  #define MSG_STORE_EPROM                     "Gem i EEPROM"
#endif
#ifndef MSG_LOAD_EPROM
  #define MSG_LOAD_EPROM                      "Hent fra EEPROM"
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                "Gendan failsafe"
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         "Genopfrisk"
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           "Info skærm"
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         "Forbered"
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            "Tune"
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     "Pause printet"
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    "Forsæt printet"
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      "Stop printet"
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       "Print fra SD"
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         "Intet SD kort"
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           "Dvale..."
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        "Venter på bruger..."
#endif
#ifndef MSG_RESUMING
  #define MSG_RESUMING                        "Forsætter printet"
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   "Print annulleret"
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         "Ingen bevægelse."
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          "DRÆBT. "
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         "STOPPET. "
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 "Tilbagetræk mm"
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP            "Skift Re.mm"
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF                "Tilbagetræk V"
#endif
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     "AutoRetr."
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  "Skift filament"
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     "Init. SD card"
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      "Skift SD kort"
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      "Probe udenfor plade"
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           "først"
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X                      "Babystep X"
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y                      "Babystep Y"
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z                      "Babystep Z"
#endif
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   "Endstop abort"
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              "Opvarmning mislykkedes"
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              "Fejl: reserve temp"
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 "Temp løber løbsk"
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     "Fejl: Maks temp"
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     "Fejl: Min temp"
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 "Fejl: Maks Plsde temp"
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 "Fejl: Min Plade temp"
#endif
#ifndef MSG_HALTED
  #define MSG_HALTED                          "PRINTER HALTED"
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_PLEASE_RESET                    "Reset venligst"
#endif
#ifndef MSG_SHORT_DAY
  #define MSG_SHORT_DAY                       "d" // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR                      "h" // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    "m" // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         "Opvarmer..."
#endif
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE                "Opvarmet"
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     "Opvarmer plade"
#endif
#ifndef MSG_BED_DONE
  #define MSG_BED_DONE                        "Plade opvarmet"
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 "Delta Kalibrering"
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               "Kalibrer X"
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               "Kalibrer Y"
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               "Kalibrer Z"
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          "Kalibrerings Center"
#endif

#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU                       "Om Printer"
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               "Printer Info"
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU                 "Printer Stat."
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 "Board Info"
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  "Extrudere"
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   "Baud"
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   "Protocol"
#endif

#if LCD_WIDTH > 19
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              "Antal Print"
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         "Færdige"
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               "Total print tid"
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            "Længste print tid"
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           "Total extruderet"
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              "Prints"
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         "Færdige"
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               "Total"
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            "Længste"
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           "Extruderet"
  #endif
#endif

#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   "Min Temp"
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   "Max Temp"
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        "Strømforsyning"
#endif

#ifndef MSG_FILAMENT_CHANGE_HEADER
  #define MSG_FILAMENT_CHANGE_HEADER          "SKIFT FILAMENT"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   "SKIFTE MULIGHEDER:"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extruder mere"
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   "forsæt print"
#endif
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          "Vent på"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_2
    #define MSG_FILAMENT_CHANGE_INIT_2          "skift af "
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_3
    #define MSG_FILAMENT_CHANGE_INIT_3          "filament"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Vent på"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_2
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament tømning"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_3
    #define MSG_FILAMENT_CHANGE_UNLOAD_3        ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        "indsæt filament"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_2
    #define MSG_FILAMENT_CHANGE_INSERT_2        "og tryk for"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_3
    #define MSG_FILAMENT_CHANGE_INSERT_3        "at fortsætte..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Vent på"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_2
    #define MSG_FILAMENT_CHANGE_LOAD_2          "filament fødning"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_3
    #define MSG_FILAMENT_CHANGE_LOAD_3          ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Vent på"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_2
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament extrudere"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_3
    #define MSG_FILAMENT_CHANGE_EXTRUDE_3       ""
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        "Vent på at"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_2
    #define MSG_FILAMENT_CHANGE_RESUME_2        "print forsætter"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_3
    #define MSG_FILAMENT_CHANGE_RESUME_3        ""
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          "Vent venligst..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Tømmer..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        "Indsæt og klik"
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          "Føder..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extrudere..."
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        "Fortsætter..."
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_DA_H
