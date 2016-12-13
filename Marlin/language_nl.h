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

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " gereed."
#define MSG_SD_INSERTED                     "Kaart ingestoken"
#define MSG_SD_REMOVED                      "Kaart verwijderd"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Hoofdmenu"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Motoren uit"
#define MSG_AUTO_HOME                       "Auto home"
#define MSG_AUTO_HOME_X                     "Home X"
#define MSG_AUTO_HOME_Y                     "Home Y"
#define MSG_AUTO_HOME_Z                     "Home Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Klik voor begin"
#define MSG_LEVEL_BED_NEXT_POINT            "Volgende Plaats"
#define MSG_LEVEL_BED_DONE                  "Bed level kompl."
#define MSG_LEVEL_BED_CANCEL                "Bed level afbr."
#define MSG_SET_HOME_OFFSETS                "Zet home offsets"
#define MSG_HOME_OFFSETS_APPLIED            "H offset toegep."
#define MSG_SET_ORIGIN                      "Nulpunt instellen"
#define MSG_PREHEAT_1                       "PLA voorverwarmen"
#define MSG_PREHEAT_1_N                     "PLA voorverw. "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1_N "aan"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1_N "Bed"
#define MSG_PREHEAT_1_SETTINGS              "PLA verw. conf"
#define MSG_PREHEAT_2                       "ABS voorverwarmen"
#define MSG_PREHEAT_2_N                     "ABS voorverw. "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2_N "aan"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2_N "Bed"
#define MSG_PREHEAT_2_SETTINGS              "ABS verw. conf"
#define MSG_COOLDOWN                        "Afkoelen"
#define MSG_SWITCH_PS_ON                    "Stroom aan"
#define MSG_SWITCH_PS_OFF                   "Stroom uit"
#define MSG_EXTRUDE                         "Extrude"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "As verplaatsen"
#define MSG_LEVEL_BED                       "Level bed"
#define MSG_MOVE_X                          "Verplaats X"
#define MSG_MOVE_Y                          "Verplaats Y"
#define MSG_MOVE_Z                          "Verplaats Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Verplaats 0.1mm"
#define MSG_MOVE_1MM                        "Verplaats 1mm"
#define MSG_MOVE_10MM                       "Verplaats 10mm"
#define MSG_SPEED                           "Snelheid"
#define MSG_BED_Z                           "Bed Z"
#define MSG_NOZZLE                          "Nozzle"
#define MSG_BED                             "Bed"
#define MSG_FAN_SPEED                       "Fan snelheid"
#define MSG_FLOW                            "Flow"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "Aan "
#define MSG_OFF                             "Uit"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Selecteer"
#define MSG_ACC                             "Versn"
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
#define MSG_E1STEPS                         "E1steps/mm"
#define MSG_E2STEPS                         "E2steps/mm"
#define MSG_E3STEPS                         "E3steps/mm"
#define MSG_E4STEPS                         "E4steps/mm"
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
#define MSG_CONTROL_RETRACT                 "Retract mm"  //accepted English term in Dutch
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
#define MSG_BLTOUCH_SELFTEST                "BLTouch Zelf-Test"
#define MSG_BLTOUCH_RESET                   "Reset BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "Eerst"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"  //accepted English term in Dutch
#define MSG_BABYSTEP_X                      "Babystap X"
#define MSG_BABYSTEP_Y                      "Babystap Y"
#define MSG_BABYSTEP_Z                      "Babystap Z"
#define MSG_ENDSTOP_ABORT                   "Endstop afbr."
#define MSG_HEATING_FAILED_LCD              "Voorverw. fout"
#define MSG_ERR_REDUNDANT_TEMP              "Redun. temp fout"
#define MSG_THERMAL_RUNAWAY                 "Therm. wegloop"
#define MSG_ERR_MAXTEMP                     "Err: Max. temp"
#define MSG_ERR_MINTEMP                     "Err: Min. temp"
#define MSG_ERR_MAXTEMP_BED                 "Err: Max.tmp bed"
#define MSG_ERR_MINTEMP_BED                 "Err: Min.tmp bed"
#define MSG_ERR_Z_HOMING                    "Fout Z homing"
#define MSG_HALTED                          "PRINTER GESTOPT"
#define MSG_PLEASE_RESET                    "Reset A.U.B."
#define MSG_SHORT_DAY                       "d" //  One character only. Keep English standard
#define MSG_SHORT_HOUR                      "h" //  One character only
#define MSG_SHORT_MINUTE                    "m" //  One character only
#define MSG_HEATING                         "Voorwarmen..."
#define MSG_HEATING_COMPLETE                "Voorverw. kompl."
#define MSG_BED_HEATING                     "Bed voorverw."
#define MSG_BED_DONE                        "Bed is voorverw."
#define MSG_DELTA_CALIBRATE                 "Delta Calibratie"
#define MSG_DELTA_CALIBRATE_X               "Kalibreer X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibreer Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibreer Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibreer Midden"

#define MSG_INFO_STATS_MENU                 "Printer Stats"
#define MSG_INFO_BOARD_MENU                 "Board Info" //accepted English term in Dutch
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_MENU                       "Over Printer"
#define MSG_INFO_PRINTER_MENU               "Printer Info"
#define MSG_INFO_PROTOCOL                   "Protocol"
#define MSG_LIGHTS_ON                       "Case licht aan"
#define MSG_LIGHTS_OFF                      "Case licht uit"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Printed Aantal"
  #define MSG_INFO_COMPLETED_PRINTS         "Totaal Voltooid"
  #define MSG_INFO_PRINT_TIME               "Totale Printtijd"
  #define MSG_INFO_PRINT_LONGEST            "Langste Printtijd"
  #define MSG_INFO_PRINT_FILAMENT           "Totaal Extrudeert"
#else
  #define MSG_INFO_PRINT_COUNT              "Aantal"
  #define MSG_INFO_COMPLETED_PRINTS         "Voltooid"
  #define MSG_INFO_PRINT_TIME               "Printtijd "
  #define MSG_INFO_PRINT_LONGEST            "Langste"
  #define MSG_INFO_PRINT_FILAMENT           "Extrud."
#endif

#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Power Supply"  //accepted English term in Dutch

#define MSG_DRIVE_STRENGTH                  "Motorstroom"
#define MSG_DAC_PERCENT                     "Driver %"  //accepted English term in Dutch
#define MSG_DAC_EEPROM_WRITE                "DAC Opslaan"
#define MSG_FILAMENT_CHANGE_HEADER          "WISSEL FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "WISSEL OPTIES:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrudeer meer"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Hervat print"

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  // Up to 3 lines
  #define MSG_FILAMENT_CHANGE_INIT_1          "Wacht voor start"
  #define MSG_FILAMENT_CHANGE_INIT_2          "filament te"
  #define MSG_FILAMENT_CHANGE_INIT_3          "verwisselen"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Wacht voor"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament uit"
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        "te laden"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Laad filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "en druk knop"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "om verder..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Wacht voor"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filament te"
  #define MSG_FILAMENT_CHANGE_LOAD_3          "laden"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Wacht voor"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament te"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "extruderen"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Wacht voor print"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "om verder"
  #define MSG_FILAMENT_CHANGE_RESUME_3        "te gaan"
#else // LCD_HEIGHT < 4
  // Up to 2 lines
  #define MSG_FILAMENT_CHANGE_INIT_1          "Wacht voor"
  #define MSG_FILAMENT_CHANGE_INIT_2          "start..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Wacht voor"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "uitladen..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Laad filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "en druk knop"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Wacht voor"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "inladen..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Wacht voor"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "extrudering"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Wacht voor"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "printing..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_NL_H
