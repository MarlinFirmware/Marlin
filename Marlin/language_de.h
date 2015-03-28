/**
 * German
 *
 * LCD Menu Messages
 * See also documentation/LCDLanguageFont.md
 *
 */
#ifndef LANGUAGE_DE_H
#define LANGUAGE_DE_H

#define MAPPER_C2C3
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " Bereit."
#define MSG_SD_INSERTED                     "SDKarte erkannt."
#define MSG_SD_REMOVED                      "SDKarte entfernt."
#define MSG_MAIN                            "Hauptmenü"
#define MSG_AUTOSTART                       "Autostart"
#define MSG_DISABLE_STEPPERS                "Motoren Aus" // M84
#define MSG_AUTO_HOME                       "Home" // G28
#define MSG_SET_HOME_OFFSETS                "Setze Home hier"
#define MSG_SET_ORIGIN                      "Setze Null hier" //"G92 X0 Y0 Z0" commented out in ultralcd.cpp
#define MSG_PREHEAT_PLA                     "Vorwärmen PLA"
#define MSG_PREHEAT_PLA_N                   "Vorwärmen PLA "
#define MSG_PREHEAT_PLA_ALL                 "Vorw. PLA Alle"
#define MSG_PREHEAT_PLA_BEDONLY             "Vorw. PLA Bett"
#define MSG_PREHEAT_PLA_SETTINGS            "Vorwärm. PLA Ein."
#define MSG_PREHEAT_ABS                     "Vorwärmen ABS"
#define MSG_PREHEAT_ABS_N                   "Vorwärmen ABS "
#define MSG_PREHEAT_ABS_ALL                 "Vorw. ABS Alle"
#define MSG_PREHEAT_ABS_BEDONLY             "Vorw. ABS Bett"
#define MSG_PREHEAT_ABS_SETTINGS            "Vorwärm. ABS Ein."
#define MSG_COOLDOWN                        "Abkühlen"
#define MSG_SWITCH_PS_ON                    "Netzteil Ein"
#define MSG_SWITCH_PS_OFF                   "Netzteil Aus"
#define MSG_RETRACT                         "Retract"
#define MSG_MOVE_AXIS                       "Bewegen"
#define MSG_MOVE_X                          "X"
#define MSG_MOVE_Y                          "Y"
#define MSG_MOVE_Z                          "Z"
#define MSG_MOVE_E                          "E"
#define MSG_MOVE_01MM                       " 0.1 mm"
#define MSG_MOVE_1MM                        " 1.0 mm"
#define MSG_MOVE_10MM                       "10.0 mm"
#define MSG_SPEED                           "Geschw."
#define MSG_NOZZLE                          "Düse"
#define MSG_BED                             "Bett"
#define MSG_FAN_SPEED                       "Lüftergeschw."
#define MSG_FLOW                            "Fluss"
#define MSG_CONTROL                         "Einstellungen"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Faktor"
#define MSG_AUTOTEMP                        "AutoTemp"
#define MSG_ON                              "Ein"
#define MSG_OFF                             "Aus"
#define MSG_PID_P                           "PID P"
#define MSG_PID_I                           "PID I"
#define MSG_PID_D                           "PID D"
#define MSG_PID_C                           "PID C"
#define MSG_ACC                             "A"
#define MSG_VXY_JERK                        "V xy jerk"
#define MSG_VZ_JERK                         "V z  jerk"
#define MSG_VE_JERK                         "V e  jerk"
#define MSG_VMAX                            "V max " // space by purpose
#define MSG_X                               "x"
#define MSG_Y                               "y"
#define MSG_Z                               "z"
#define MSG_E                               "e"
#define MSG_VMIN                            "V min"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "A max " // space by purpose
#define MSG_A_RETRACT                       "A Retract"
#define MSG_XSTEPS                          "X steps/mm"
#define MSG_YSTEPS                          "Y steps/mm"
#define MSG_ZSTEPS                          "Z steps/mm"
#define MSG_ESTEPS                          "E steps/mm"
#define MSG_TEMPERATURE                     "Temperatur"
#define MSG_MOTION                          "Bewegung"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm³"
#define MSG_FILAMENT_SIZE_EXTRUDER_0        "Filament D 1"
#define MSG_FILAMENT_SIZE_EXTRUDER_1        "Filament D 2"
#define MSG_FILAMENT_SIZE_EXTRUDER_2        "Filament D 3"
#define MSG_CONTRAST                        "LCD contrast"
#define MSG_STORE_EPROM                     "EPROM speichern"
#define MSG_LOAD_EPROM                      "EPROM laden"
#define MSG_RESTORE_FAILSAFE                "Standardkonfig."
#define MSG_REFRESH                         "Aktualisieren"
#define MSG_WATCH                           "Info"
#define MSG_PREPARE                         "Vorbereitung"
#define MSG_TUNE                            "Justierung"
#define MSG_PAUSE_PRINT                     "SD-Druck Pause"
#define MSG_RESUME_PRINT                    "SD-Druck Weiter"
#define MSG_STOP_PRINT                      "SD-Druck Abbruch"
#define MSG_CARD_MENU                       "SDKarte"
#define MSG_NO_CARD                         "Keine SDKarte"
#define MSG_DWELL                           "Warten..."
#define MSG_USERWAIT                        "Warte auf Nutzer."
#define MSG_RESUMING                        "Druck geht weiter"
#define MSG_PRINT_ABORTED                   "Druck abgebrochen"
#define MSG_NO_MOVE                         "Motoren Eingesch."
#define MSG_KILLED                          "KILLED."
#define MSG_STOPPED                         "ANGEHALTEN."
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Wechs. Retract mm"
#define MSG_CONTROL_RETRACTF                "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Wechs. UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Filament wechseln"
#define MSG_INIT_SDCARD                     "SDKarte erkennen"// Manually initialize the SD-card via user interface
#define MSG_CNG_SDCARD                      "SDKarte erkennen"// SD-card changed by user. For machines with no autocarddetect. Both send "M21"
#define MSG_ZPROBE_OUT                      "Sensor ausserhalb"
#define MSG_POSITION_UNKNOWN                "X/Y vor Z Homen."
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop Abbr. Ein"

#ifdef DELTA_CALIBRATION_MENU
    #define MSG_DELTA_CALIBRATE             "Delta Kalibrieren"
    #define MSG_DELTA_CALIBRATE_X           "Kalibriere X"
    #define MSG_DELTA_CALIBRATE_Y           "Kalibriere Y"
    #define MSG_DELTA_CALIBRATE_Z           "Kalibriere Z"
    #define MSG_DELTA_CALIBRATE_CENTER      "Kalibriere Mitte"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_DE_H
