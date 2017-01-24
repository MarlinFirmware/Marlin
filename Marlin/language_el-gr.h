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
 * Greek (Greece)
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_EL_GR_H
#define LANGUAGE_EL_GR_H

#define MAPPER_CECF
#define DISPLAY_CHARSET_ISO10646_GREEK

#define WELCOME_MSG                         MACHINE_NAME " έτοιμο."
#define MSG_SD_INSERTED                     "Εισαγωγή κάρτας"
#define MSG_SD_REMOVED                      "Αφαίρεση κάρτας"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Βασική Οθόνη"
#define MSG_AUTOSTART                       "Αυτόματη εκκίνηση"
#define MSG_DISABLE_STEPPERS                "Απενεργοποίηση βηματιστή"
#define MSG_AUTO_HOME                       "Αυτομ. επαναφορά στο αρχικό σημείο"
#define MSG_AUTO_HOME_X                     "Αρχικό σημείο X"
#define MSG_AUTO_HOME_Y                     "Αρχικό σημείο Y"
#define MSG_AUTO_HOME_Z                     "Αρχικό σημείο Z"
#define MSG_LEVEL_BED_HOMING                "Επαναφορά στο αρχικό σημείο ΧΥΖ"
#define MSG_LEVEL_BED_WAITING               "Κάντε κλικ για να ξεκινήσετε"
#define MSG_LEVEL_BED_NEXT_POINT            "Επόμενο σημείο"
#define MSG_LEVEL_BED_DONE                  "Ολοκλήρωση επιπεδοποίησης!"
#define MSG_LEVEL_BED_CANCEL                "Ακύρωση"
#define MSG_SET_HOME_OFFSETS                "Ορισμός βασικών μετατοπίσεων"
#define MSG_HOME_OFFSETS_APPLIED            "Εφαρμόστηκαν οι μετατοπίσεις"
#define MSG_SET_ORIGIN                      "Ορισμός προέλευσης"
#define MSG_PREHEAT_1                       "Προθέρμανση PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " όλα"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " κλίνη"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " επιβεβαίωση"
#define MSG_PREHEAT_2                       "Προθέρμανση ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " όλα"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " επιβεβαίωση"
#define MSG_COOLDOWN                        "Μειωση θερμοκρασιας"
#define MSG_SWITCH_PS_ON                    "Ενεργοποίηση"
#define MSG_SWITCH_PS_OFF                   "Απενεργοποίηση"
#define MSG_EXTRUDE                         "Εξώθηση"
#define MSG_RETRACT                         "Ανάσυρση"
#define MSG_MOVE_AXIS                       "Μετακίνηση άξονα"
#define MSG_LEVEL_BED                       "Επιπεδοποίηση κλίνης"
#define MSG_MOVE_X                          "Μετακίνηση X"
#define MSG_MOVE_Y                          "Μετακίνηση Y"
#define MSG_MOVE_Z                          "Μετακίνηση Z"
#define MSG_MOVE_E                          "Εξωθητήρας"
#define MSG_MOVE_01MM                       "Μετακίνηση 0,1 μμ"
#define MSG_MOVE_1MM                        "Μετακίνηση 1 μμ"
#define MSG_MOVE_10MM                       "Μετακίνηση 10 μμ"
#define MSG_SPEED                           "Ταχύτητα"
#define MSG_BED_Z                           "Κλίνη Z"
#define MSG_NOZZLE                          "Ακροφύσιο"
#define MSG_BED                             "Κλίνη"
#define MSG_FAN_SPEED                       "Ταχύτητα ανεμιστήρα"
#define MSG_FLOW                            "Ροή"
#define MSG_CONTROL                         "Έλεγχος"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        "Αυτομ. ρύθμιση θερμοκρασίας"
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
#define MSG_VMAX                            "Vμεγ "
#define MSG_VMIN                            "Vελαχ"
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
#define MSG_RESTORE_FAILSAFE                "Επαναφορά ασφαλούς αντιγράφου"
#define MSG_REFRESH                         "Ανανέωση"
#define MSG_WATCH                           "Οθόνη πληροφόρησης"
#define MSG_PREPARE                         "Προετοιμασία"
#define MSG_TUNE                            "Συντονισμός"
#define MSG_PAUSE_PRINT                     "Παύση εκτύπωσης"
#define MSG_RESUME_PRINT                    "Συνέχιση εκτύπωσης"
#define MSG_STOP_PRINT                      "Διακοπή εκτύπωσης"
#define MSG_CARD_MENU                       "Εκτύπωση από SD"
#define MSG_NO_CARD                         "Δεν βρέθηκε SD"
#define MSG_DWELL                           "Αναστολή λειτουργίας..."
#define MSG_USERWAIT                        "Αναμονή για χρήστη…"
#define MSG_RESUMING                        "Συνεχίζεται η εκτύπωση"
#define MSG_PRINT_ABORTED                   "Διακόπτεται η εκτύπωση"
#define MSG_NO_MOVE                         "Καμία κίνηση."
#define MSG_KILLED                          "ΤΕΡΜΑΤΙΣΜΟΣ. "
#define MSG_STOPPED                         "ΔΙΑΚΟΠΗ. "
#define MSG_CONTROL_RETRACT                 "Ανάσυρση μμ"
#define MSG_CONTROL_RETRACT_SWAP            "Εναλλαγή ανάσυρσης μμ"
#define MSG_CONTROL_RETRACTF                "Ανάσυρση V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Μεταπήδηση μμ"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "Αυτόματη ανάσυρση"
#define MSG_FILAMENTCHANGE                  "Αλλαγή νήματος"
#define MSG_INIT_SDCARD                     "Προετοιμασία κάρτας SD"
#define MSG_CNG_SDCARD                      "Αλλαγή κάρτας SD"
#define MSG_ZPROBE_OUT                      "Διερεύνηση Z εκτός κλίνης"
#define MSG_YX_UNHOMED                      "Επαναφορά Χ/Υ πριν από Ζ"
#define MSG_XYZ_UNHOMED                     "Επαναφορά ΧΥΖ πρώτα"
#define MSG_ZPROBE_ZOFFSET                  "Μετατόπιση Ζ"
#define MSG_BABYSTEP_X                      "Μικρό βήμα Χ"
#define MSG_BABYSTEP_Y                      "Μικρό βήμα Υ"
#define MSG_BABYSTEP_Z                      "Μικρό βήμα Ζ"
#define MSG_ENDSTOP_ABORT                   "Ματαίωση endstop "
#define MSG_HEATING_FAILED_LCD              "Ανεπιτυχής θέρμανση"
#define MSG_ERR_REDUNDANT_TEMP              "Λάθος: ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ"
#define MSG_THERMAL_RUNAWAY                 "ΔΙΑΦΥΓΗ ΘΕΡΜΟΤΗΤΑΣ"
#define MSG_ERR_MAXTEMP                     "Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ"
#define MSG_ERR_MINTEMP                     "Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ"
#define MSG_ERR_MAXTEMP_BED                 "Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ"
#define MSG_ERR_MINTEMP_BED                 "Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ"
#define MSG_HEATING                         "Θερμαίνεται…"
#define MSG_HEATING_COMPLETE                "Η θέρμανση ολοκληρώθηκε."
#define MSG_BED_HEATING                     "Θέρμανση κλίνης."
#define MSG_BED_DONE                        "Η κλίνη ολοκληρώθηκε."
#define MSG_DELTA_CALIBRATE                 "Βαθμονόμηση Delta"
#define MSG_DELTA_CALIBRATE_X               "Βαθμονόμηση X"
#define MSG_DELTA_CALIBRATE_Y               "Βαθμονόμηση Y"
#define MSG_DELTA_CALIBRATE_Z               "Βαθμονόμηση Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Βαθμονόμηση κέντρου"

#endif // LANGUAGE_EL_GR_H
