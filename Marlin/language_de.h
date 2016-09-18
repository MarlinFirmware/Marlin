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
 * German
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_DE_H
#define LANGUAGE_DE_H

#define MAPPER_C2C3
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " bereit"
#define MSG_SD_INSERTED                     "SD-Karte erkannt"
#define MSG_SD_REMOVED                      "SD-Karte entfernt"
#define MSG_LCD_ENDSTOPS                    "Endstopp" // Max length 8 characters
#define MSG_MAIN                            "Hauptmenü"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Motoren deaktivieren" // M84
#define MSG_AUTO_HOME                       "Home" // G28
#define MSG_AUTO_HOME_X                     "Home X"
#define MSG_AUTO_HOME_Y                     "Home Y"
#define MSG_AUTO_HOME_Z                     "Home Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Klick für Start"
#define MSG_LEVEL_BED_NEXT_POINT            "Nächste Koordinate"
#define MSG_LEVEL_BED_DONE                  "Fertig"
#define MSG_LEVEL_BED_CANCEL                "Abbruch"
#define MSG_SET_HOME_OFFSETS                "Setze Homeversatz"
#define MSG_HOME_OFFSETS_APPLIED            "Homeversatz aktiv"
#define MSG_SET_ORIGIN                      "Setze Nullpunkt" //"G92 X0 Y0 Z0" commented out in ultralcd.cpp
#define MSG_PREHEAT_1                       "Vorwärmen PLA"
#define MSG_PREHEAT_1_N                     "Vorwärmen PLA "
#define MSG_PREHEAT_1_ALL                   "Vorw. PLA Alle"
#define MSG_PREHEAT_1_BEDONLY               "Vorw. PLA Bett"
#define MSG_PREHEAT_1_SETTINGS              "Vorw. PLA Einst."
#define MSG_PREHEAT_2                       "Vorwärmen ABS"
#define MSG_PREHEAT_2_N                     "Vorwärmen ABS "
#define MSG_PREHEAT_2_ALL                   "Vorw. ABS Alle"
#define MSG_PREHEAT_2_BEDONLY               "Vorw. ABS Bett"
#define MSG_PREHEAT_2_SETTINGS              "Vorw. ABS Einst."
#define MSG_COOLDOWN                        "Abkühlen"
#define MSG_SWITCH_PS_ON                    "Netzteil ein"
#define MSG_SWITCH_PS_OFF                   "Netzteil aus"
#define MSG_EXTRUDE                         "Extrudieren"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "Bewegen"
#define MSG_LEVEL_BED                       "Bett nivellieren"
#define MSG_MOVE_X                          "X"
#define MSG_MOVE_Y                          "Y"
#define MSG_MOVE_Z                          "Z"
#define MSG_MOVE_E                          "Extruder "
#define MSG_MOVE_01MM                       " 0,1 mm"
#define MSG_MOVE_1MM                        " 1,0 mm"
#define MSG_MOVE_10MM                       "10,0 mm"
#define MSG_SPEED                           "Geschw."
#define MSG_BED_Z                           "Bett Z"
#define MSG_NOZZLE                          "Düse"
#define MSG_BED                             "Bett"
#define MSG_FAN_SPEED                       "Lüfter"
#define MSG_FLOW                            "Durchfluss"
#define MSG_CONTROL                         "Einstellungen"
#define MSG_MIN                             LCD_STR_THERMOMETER " min"
#define MSG_MAX                             LCD_STR_THERMOMETER " max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Faktor"
#define MSG_AUTOTEMP                        "AutoTemp"
#define MSG_ON                              "Ein"
#define MSG_OFF                             "Aus"
#define MSG_PID_P                           "PID P"
#define MSG_PID_I                           "PID I"
#define MSG_PID_D                           "PID D"
#define MSG_PID_C                           "PID C"
#define MSG_SELECT                          "Auswählen"
#define MSG_ACC                             "A"
#define MSG_VXY_JERK                        "V XY Jerk"
#define MSG_VZ_JERK                         "V Z  Jerk"
#define MSG_VE_JERK                         "V E  Jerk"
#define MSG_VMAX                            "V max " // space by purpose
#define MSG_VMIN                            "V min"
#define MSG_VTRAV_MIN                       "V min Leerfahrt"
#define MSG_AMAX                            "A max " // space by purpose
#define MSG_A_RETRACT                       "A Retract"
#define MSG_A_TRAVEL                        "A Leerfahrt"
#define MSG_XSTEPS                          "X Steps/mm"
#define MSG_YSTEPS                          "Y Steps/mm"
#define MSG_ZSTEPS                          "Z Steps/mm"
#define MSG_ESTEPS                          "E Steps/mm"
#define MSG_TEMPERATURE                     "Temperatur"
#define MSG_MOTION                          "Bewegung"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm³"
#define MSG_FILAMENT_DIAM                   "D Fil."
#define MSG_CONTRAST                        "LCD Kontrast"
#define MSG_STORE_EPROM                     "EPROM speichern"
#define MSG_LOAD_EPROM                      "EPROM laden"
#define MSG_RESTORE_FAILSAFE                "Standardkonfiguration"
#define MSG_REFRESH                         "Aktualisieren"
#define MSG_WATCH                           "Info"
#define MSG_PREPARE                         "Vorbereitung"
#define MSG_TUNE                            "Justierung"
#define MSG_PAUSE_PRINT                     "SD-Druck Pause"
#define MSG_RESUME_PRINT                    "SD-Druck Fortsetzung"
#define MSG_STOP_PRINT                      "SD-Druck Abbruch"
#define MSG_CARD_MENU                       "SD-Karte"
#define MSG_NO_CARD                         "Keine SD-Karte"
#define MSG_DWELL                           "Warten..."
#define MSG_USERWAIT                        "Warte auf Nutzer"
#define MSG_RESUMING                        "Druckfortsetzung"
#define MSG_PRINT_ABORTED                   "Druck abgebrochen"
#define MSG_NO_MOVE                         "Motoren eingeschaltet"
#define MSG_KILLED                          "ABGEBROCHEN"
#define MSG_STOPPED                         "ANGEHALTEN"
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Wechs. Retract mm"
#define MSG_CONTROL_RETRACTF                "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Z-Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Wechs. UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetract"
#define MSG_FILAMENTCHANGE                  "Filament wechseln"
#define MSG_INIT_SDCARD                     "SD-Karte erkennen"  // Manually initialize the SD-card via user interface
#define MSG_CNG_SDCARD                      "SD-Karte getauscht" // SD-card changed by user. For machines with no autocarddetect. Both send "M21"
#define MSG_ZPROBE_OUT                      "Sensor ausserhalb"
#define MSG_HOME                            "Vorher"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "homen"
#define MSG_ZPROBE_ZOFFSET                  "Z Versatz"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop-Abbr. ein"
#define MSG_HEATING_FAILED_LCD              "HEIZEN FEHLGESCHLAGEN"
#define MSG_ERR_REDUNDANT_TEMP              "REDUND. TEMPERATURABWEICHUNG"
#define MSG_THERMAL_RUNAWAY                 LCD_STR_THERMOMETER " NICHT ERREICHT"
#define MSG_ERR_MAXTEMP                     LCD_STR_THERMOMETER " ÜBERSCHRITTEN"
#define MSG_ERR_MINTEMP                     LCD_STR_THERMOMETER " UNTERSCHRITTEN"
#define MSG_ERR_MAXTEMP_BED                 "BETT " LCD_STR_THERMOMETER " ÜBERSCHRITTEN"
#define MSG_ERR_MINTEMP_BED                 "BETT " LCD_STR_THERMOMETER " UNTERSCHRITTEN"
#define MSG_ERR_Z_HOMING                    "G28 Z Forbidden"
#define MSG_HALTED                          "DRUCKER STOPP"
#define MSG_PLEASE_RESET                    "Bitte Resetten"
#define MSG_SHORT_DAY                       "t" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Extr. heizt..."
#define MSG_HEATING_COMPLETE                "Extr. aufgeheizt"
#define MSG_BED_HEATING                     "Bett heizt..."
#define MSG_BED_DONE                        "Bett aufgeheizt"
#define MSG_DELTA_CALIBRATE                 "Delta kalibrieren"
#define MSG_DELTA_CALIBRATE_X               "Kalibriere X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibriere Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibriere Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibriere Mitte"

#define MSG_INFO_MENU                       "Über den Drucker"
#define MSG_INFO_PRINTER_MENU               "Drucker Info"
#define MSG_INFO_STATS_MENU                 "Drucker Stats"
#define MSG_INFO_BOARD_MENU                 "Board Info"
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protokol"

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              "Gesamte Drucke"
  #define MSG_INFO_COMPLETED_PRINTS         "Beendete Drucke"
  #define MSG_INFO_PRINT_TIME               "Gesamte Druckzeit"
  #define MSG_INFO_PRINT_LONGEST            "Längster Druckjob"
  #define MSG_INFO_PRINT_FILAMENT           "Gesamt Extrudiert"
#else
  #define MSG_INFO_PRINT_COUNT              "Drucke"
  #define MSG_INFO_COMPLETED_PRINTS         "Komplett"
  #define MSG_INFO_PRINT_TIME               "Gesamt "
  #define MSG_INFO_PRINT_LONGEST            "Längster"
  #define MSG_INFO_PRINT_FILAMENT           "Extrud."
#endif
#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Stromversorgung"

#define MSG_FILAMENT_CHANGE_HEADER          "ÄNDERE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "ÄNDERE OPTIONEN:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrude mehr"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Drucke weiter"

#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Warte auf den"
  #define MSG_FILAMENT_CHANGE_INIT_2          "Start zum "
  #define MSG_FILAMENT_CHANGE_INIT_3          "Filament wechsel"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Warte auf das"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "herrausnehmen"
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        "des Filaments"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Fädel Filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "ein und drücke"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "den Knopf..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Warte auf das"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "laden des"
  #define MSG_FILAMENT_CHANGE_LOAD_3          "Filaments"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Warte auf das"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "Extruden des"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "Filaments"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Warte auf das"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "fortfahren des"
  #define MSG_FILAMENT_CHANGE_RESUME_3        "Druckes"
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          "Bitte warten..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Auswerfen..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Laden und Klick"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Laden..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extruden..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Weitermachen..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_DE_H
