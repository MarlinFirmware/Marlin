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
 * Greek
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_EL_H
#define LANGUAGE_EL_H

#define MAPPER_CECF
#define DISPLAY_CHARSET_ISO10646_GREEK

#define WELCOME_MSG                         MACHINE_NAME " έτοιμο."
#define MSG_SD_INSERTED                     "Εισαγωγή κάρτας"
#define MSG_SD_REMOVED                      "Αφαίρεση κάρτας"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Βασική Οθόνη"
#define MSG_AUTOSTART                       "Αυτόματη εκκίνηση"
#define MSG_DISABLE_STEPPERS                "Απενεργοποίηση Μοτέρ"
#define MSG_AUTO_HOME                       "Αυτομ. επαναφορά στο αρχικό σημείο" //SHORTEN
#define MSG_AUTO_HOME_X                     "Αρχικό σημείο X"
#define MSG_AUTO_HOME_Y                     "Αρχικό σημείο Y"
#define MSG_AUTO_HOME_Z                     "Αρχικό σημείο Z"
#define MSG_LEVEL_BED_HOMING                "Επαναφορά Επ. Εκτύπωσης" //SHORTEN
#define MSG_LEVEL_BED_WAITING               "Επιπεδοποίηση επ. Εκτύπωσης περιμενει" //SHORTEN
#define MSG_LEVEL_BED_NEXT_POINT            "Επόμενο σημείο"
#define MSG_LEVEL_BED_DONE                  "Ολοκλήρωση επιπεδοποίησης!" //SHORTEN
#define MSG_LEVEL_BED_CANCEL                "Ακύρωση"
#define MSG_SET_HOME_OFFSETS                "Ορισμός βασικών μετατοπίσεων" //SHORTEN
#define MSG_HOME_OFFSETS_APPLIED            "Εφαρμόστηκαν οι μετατοπίσεις" //SHORTEN
#define MSG_SET_ORIGIN                      "Ορισμός προέλευσης"
#define MSG_PREHEAT_1                       "Προθέρμανση PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " όλα"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " bed" //SHORTEN
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " επιβεβαίωση" //SHORTEN
#define MSG_PREHEAT_2                       "Προθέρμανση ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " όλα"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " bed" //SHORTEN
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " επιβεβαίωση" //SHORTEN
#define MSG_COOLDOWN                        "Μειωση θερμοκρασιας"
#define MSG_SWITCH_PS_ON                    "Ενεργοποίηση"
#define MSG_SWITCH_PS_OFF                   "Απενεργοποίηση"
#define MSG_EXTRUDE                         "Εξώθηση"
#define MSG_RETRACT                         "Ανάσυρση"
#define MSG_MOVE_AXIS                       "Μετακίνηση άξονα"
#define MSG_LEVEL_BED                       "Επιπεδοποίηση Επ. Εκτύπωσης" //SHORTEN
#define MSG_MOVE_X                          "Μετακίνηση X"
#define MSG_MOVE_Y                          "Μετακίνηση Y"
#define MSG_MOVE_Z                          "Μετακίνηση Z"
#define MSG_MOVE_E                          "Εξωθητήρας"
#define MSG_MOVE_01MM                       "Μετακίνηση 0,1μμ"
#define MSG_MOVE_1MM                        "Μετακίνηση 1μμ"
#define MSG_MOVE_10MM                       "Μετακίνηση 10μμ"
#define MSG_SPEED                           "Ταχύτητα"
#define MSG_BED_Z                           "Επ. Εκτύπωσης Z"
#define MSG_NOZZLE                          "Ακροφύσιο"
#define MSG_BED                             "Κλίνη"
#define MSG_FAN_SPEED                       "Ταχύτητα ανεμιστήρα"
#define MSG_FLOW                            "Ροή"
#define MSG_CONTROL                         "Έλεγχος"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Αυτομ ρύθμιση θερ/σίας" //SHORTEN
#define MSG_ON                              "Ενεργοποιημένο"
#define MSG_OFF                             "Απενεργοποιημένο"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Επιτάχυνση"
#define MSG_VX_JERK                         "Vαντίδραση x"
#define MSG_VY_JERK                         "Vαντίδραση y"
#define MSG_VZ_JERK                         "Vαντίδραση z"
#define MSG_VE_JERK                         "Vαντίδραση e"
#define MSG_VMAX                            "V Μέγιστο"
#define MSG_VMIN                            "V Ελάχιστο"
#define MSG_VTRAV_MIN                       "Vελάχ. μετατόπιση"
#define MSG_AMAX                            "Aμεγ "
#define MSG_A_RETRACT                       "Α-ανάσυρση"
#define MSG_A_TRAVEL                        "Α-μετατόπιση"
#define MSG_XSTEPS                          "Bήματα X ανά μμ"
#define MSG_YSTEPS                          "Bήματα Υ ανά μμ"
#define MSG_ZSTEPS                          "Bήματα Ζ ανά μμ"
#define MSG_ESTEPS                          "Bήματα Ε ανά μμ"
#define MSG_TEMPERATURE                     "Θερμοκρασία"
#define MSG_MOTION                          "Κίνηση"
#define MSG_VOLUMETRIC                      "Νήμα"
#define MSG_VOLUMETRIC_ENABLED              "Ε σε μμ3"
#define MSG_FILAMENT_DIAM                   "Διάμετρος νήματος"
#define MSG_CONTRAST                        "Κοντράστ LCD"
#define MSG_STORE_EPROM                     "Αποθήκευση"
#define MSG_LOAD_EPROM                      "Φόρτωση"
#define MSG_RESTORE_FAILSAFE                "Επαναφορά ασφαλούς αντιγράφου" //SHORTEN
#define MSG_REFRESH                         "Ανανέωση"
#define MSG_WATCH                           "Οθόνη πληροφόρησης"
#define MSG_PREPARE                         "Προετοιμασία"
#define MSG_TUNE                            "Συντονισμός"
#define MSG_PAUSE_PRINT                     "Παύση εκτύπωσης"
#define MSG_RESUME_PRINT                    "Συνέχιση εκτύπωσης"
#define MSG_STOP_PRINT                      "Διακοπή εκτύπωσης"
#define MSG_CARD_MENU                       "Εκτύπωση από SD"
#define MSG_NO_CARD                         "Δεν βρέθηκε SD"
#define MSG_DWELL                           "Αναστολή λειτουργίας"
#define MSG_USERWAIT                        "Αναμονή για χρήστη"
#define MSG_RESUMING                        "Συνεχίζεται η εκτύπωση" //SHORTEN
#define MSG_PRINT_ABORTED                   "Διακόπτεται η εκτύπωση" //SHORTEN
#define MSG_NO_MOVE                         "Καμία κίνηση."
#define MSG_KILLED                          "ΤΕΡΜΑΤΙΣΜΟΣ. "
#define MSG_STOPPED                         "ΔΙΑΚΟΠΗ. "
#define MSG_CONTROL_RETRACT                 "Ανάσυρση μμ"
#define MSG_CONTROL_RETRACT_SWAP            "Εναλλαγή ανάσυρσης μμ"  //SHORTEN
#define MSG_CONTROL_RETRACTF                "Ανάσυρση V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Μεταπήδηση μμ"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "Αυτόματη ανάσυρση"
#define MSG_FILAMENTCHANGE                  "Αλλαγή νήματος"
#define MSG_INIT_SDCARD                     "Προετοιμασία κάρτας SD"  //SHORTEN
#define MSG_CNG_SDCARD                      "Αλλαγή κάρτας SD"
#define MSG_ZPROBE_OUT                      "Διερεύνηση Z εκτός Επ.Εκτύπωσης" //SHORTEN
#define MSG_YX_UNHOMED                      "Επαναφορά Χ/Υ πριν από Ζ" //SHORTEN
#define MSG_XYZ_UNHOMED                     "Επαναφορά ΧΥΖ πρώτα"
#define MSG_ZPROBE_ZOFFSET                  "Μετατόπιση Ζ"
#define MSG_BABYSTEP_X                      "Μικρό βήμα Χ"
#define MSG_BABYSTEP_Y                      "Μικρό βήμα Υ"
#define MSG_BABYSTEP_Z                      "Μικρό βήμα Ζ"
#define MSG_ENDSTOP_ABORT                   "Ακύρωση endstop "
#define MSG_HEATING_FAILED_LCD              "Ανεπιτυχής θέρμανση"
#define MSG_ERR_REDUNDANT_TEMP              "ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ"
#define MSG_THERMAL_RUNAWAY                 "ΔΙΑΦΥΓΗ ΘΕΡΜΟΚΡΑΣΙΑΣ"
#define MSG_ERR_MAXTEMP                     "ΠΕΡΙΤΗ ΘΕΡΜΟΚΡΑΣΙΑ"
#define MSG_ERR_MINTEMP                     "ΜΗ ΕΠΑΡΚΗΣ ΘΕΡΜΟΚΡΑΣΙΑΣ" //SHORTEN
#define MSG_ERR_MAXTEMP_BED                 "ΜΕΓΙΣΤΗ ΘΕΡΜΟΚΡΑΣΙΑΣ ΕΠ. ΕΚΤΥΠΩΣΗΣ" //SHORTEN
#define MSG_ERR_MINTEMP_BED                 "ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΚΡΑΣΙΑΣ ΕΠ. ΕΚΤΥΠΩΣΗΣ" //SHORTEN
#define MSG_HALTED                          "H εκτύπωση διακόπηκε"
#define MSG_PLEASE_RESET                    "PLEASE RESET" //TRANSLATE
#define MSG_HEATING                         "Θερμαίνεται…"
#define MSG_HEATING_COMPLETE                "Η θέρμανση ολοκληρώθηκε." //SHORTEN
#define MSG_BED_HEATING                     "Θέρμανση ΕΠ. Εκτύπωσης" //SHORTEN
#define MSG_BED_DONE                        "Η Επ. Εκτύπωσης ολοκληρώθηκε" //SHORTEN
#define MSG_DELTA_CALIBRATE                 "Βαθμονόμηση Delta"
#define MSG_DELTA_CALIBRATE_X               "Βαθμονόμηση X"
#define MSG_DELTA_CALIBRATE_Y               "Βαθμονόμηση Y"
#define MSG_DELTA_CALIBRATE_Z               "Βαθμονόμηση Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Βαθμονόμηση κέντρου"

#define MSG_INFO_MENU                       "About Printer"
#define MSG_INFO_PRINTER_MENU               "Printer Info"
#define MSG_INFO_STATS_MENU                 "Printer Stats"
#define MSG_INFO_BOARD_MENU                 "Board Info"
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocol"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Print Count"
  #define MSG_INFO_COMPLETED_PRINTS         "Completed  "
  #define MSG_INFO_PRINT_TIME               "Total Time "
#else
  #define MSG_INFO_PRINT_COUNT              "Prints   "
  #define MSG_INFO_COMPLETED_PRINTS         "Completed"
  #define MSG_INFO_PRINT_TIME               "Duration "
#endif
#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Power Supply"

#define MSG_FILAMENT_CHANGE_HEADER          "CHANGE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrude more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Resume print"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Wait for start"
  #define MSG_FILAMENT_CHANGE_INIT_2          "of the filament"
  #define MSG_FILAMENT_CHANGE_INIT_3          "change"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Wait for"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament unload"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insert filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "and press button"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "to continue..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Wait for"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filament load"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Wait for"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament extrude"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Wait for print"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "to resume"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Please wait..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Ejecting..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insert and Click"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Loading..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extruding..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Resuming..."
#endif

#endif // LANGUAGE_EL_H
