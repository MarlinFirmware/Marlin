/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
#pragma once

/**
 * Greek (Greece)
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_GREEK
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" έτοιμο.")
#define MSG_MEDIA_INSERTED                  _UxGT("Εισαγωγή κάρτας")
#define MSG_MEDIA_REMOVED                   _UxGT("Αφαίρεση κάρτας")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Βασική Οθόνη")
#define MSG_AUTOSTART                       _UxGT("Αυτόματη εκκίνηση")
#define MSG_DISABLE_STEPPERS                _UxGT("Απενεργοποίηση βηματιστή")
#define MSG_AUTO_HOME                       _UxGT("Αυτομ. επαναφορά στο αρχικό σημείο")
#define MSG_AUTO_HOME_X                     _UxGT("Αρχικό σημείο X")
#define MSG_AUTO_HOME_Y                     _UxGT("Αρχικό σημείο Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Αρχικό σημείο Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Επαναφορά στο αρχικό σημείο ΧΥΖ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Κάντε κλικ για να ξεκινήσετε")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Επόμενο σημείο")
#define MSG_LEVEL_BED_DONE                  _UxGT("Ολοκλήρωση επιπεδοποίησης!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ορισμός βασικών μετατοπίσεων")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Εφαρμόστηκαν οι μετατοπίσεις")
#define MSG_SET_ORIGIN                      _UxGT("Ορισμός προέλευσης")
#define MSG_PREHEAT_1                       _UxGT("Προθέρμανση " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" όλα")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" κλίνη")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" επιβεβαίωση")
#define MSG_PREHEAT_2                       _UxGT("Προθέρμανση " PREHEAT_2_LABEL)
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" όλα")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" επιβεβαίωση")
#define MSG_COOLDOWN                        _UxGT("Μειωση θερμοκρασιας")
#define MSG_SWITCH_PS_ON                    _UxGT("Ενεργοποίηση")
#define MSG_SWITCH_PS_OFF                   _UxGT("Απενεργοποίηση")
#define MSG_EXTRUDE                         _UxGT("Εξώθηση")
#define MSG_RETRACT                         _UxGT("Ανάσυρση")
#define MSG_MOVE_AXIS                       _UxGT("Μετακίνηση άξονα")
#define MSG_BED_LEVELING                    _UxGT("Επιπεδοποίηση κλίνης")
#define MSG_LEVEL_BED                       _UxGT("Επιπεδοποίηση κλίνης")
#define MSG_MOVE_X                          _UxGT("Μετακίνηση X")
#define MSG_MOVE_Y                          _UxGT("Μετακίνηση Y")
#define MSG_MOVE_Z                          _UxGT("Μετακίνηση Z")
#define MSG_MOVE_E                          _UxGT("Εξωθητήρας")
#define MSG_MOVE_Z_DIST                     _UxGT("Μετακίνηση %s μμ")
#define MSG_MOVE_01MM                       _UxGT("Μετακίνηση 0,1 μμ")
#define MSG_MOVE_1MM                        _UxGT("Μετακίνηση 1 μμ")
#define MSG_MOVE_10MM                       _UxGT("Μετακίνηση 10 μμ")
#define MSG_SPEED                           _UxGT("Ταχύτητα")
#define MSG_BED_Z                           _UxGT("Κλίνη Z")
#define MSG_NOZZLE                          _UxGT("Ακροφύσιο")
#define MSG_BED                             _UxGT("Κλίνη")
#define MSG_FAN_SPEED                       _UxGT("Ταχύτητα ανεμιστήρα")
#define MSG_FLOW                            _UxGT("Ροή")
#define MSG_CONTROL                         _UxGT("Έλεγχος")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Αυτομ. ρύθμιση θερμοκρασίας")
#define MSG_LCD_ON                          _UxGT("Ενεργοποιημένο")
#define MSG_LCD_OFF                         _UxGT("Απενεργοποιημένο")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Επιτάχυνση")
#define MSG_JERK                            _UxGT("Vαντίδραση")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Vαντίδραση A")
  #define MSG_VB_JERK                       _UxGT("Vαντίδραση B")
  #define MSG_VC_JERK                       _UxGT("Vαντίδραση C")
#else
  #define MSG_VA_JERK                       _UxGT("Vαντίδραση X")
  #define MSG_VB_JERK                       _UxGT("Vαντίδραση Y")
  #define MSG_VC_JERK                       _UxGT("Vαντίδραση Z")
#endif
#define MSG_VE_JERK                         _UxGT("Vαντίδραση E")
#define MSG_VMAX                            _UxGT("Vμεγ ")
#define MSG_VMIN                            _UxGT("Vελαχ")
#define MSG_VTRAV_MIN                       _UxGT("Vελάχ. μετατόπιση")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Aμεγ ")
#define MSG_A_RETRACT                       _UxGT("Α-ανάσυρση")
#define MSG_A_TRAVEL                        _UxGT("Α-μετατόπιση")
#define MSG_STEPS_PER_MM                    _UxGT("Bήματα ανά μμ")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Bήματα A ανά μμ")
  #define MSG_BSTEPS                        _UxGT("Bήματα B ανά μμ")
  #define MSG_CSTEPS                        _UxGT("Bήματα C ανά μμ")
#else
  #define MSG_ASTEPS                        _UxGT("Bήματα X ανά μμ")
  #define MSG_BSTEPS                        _UxGT("Bήματα Y ανά μμ")
  #define MSG_CSTEPS                        _UxGT("Bήματα Z ανά μμ")
#endif
#define MSG_ESTEPS                          _UxGT("Bήματα Ε ανά μμ")
#define MSG_E1STEPS                         _UxGT("Bήματα Ε1 ανά μμ")
#define MSG_E2STEPS                         _UxGT("Bήματα Ε2 ανά μμ")
#define MSG_E3STEPS                         _UxGT("Bήματα Ε3 ανά μμ")
#define MSG_E4STEPS                         _UxGT("Bήματα Ε4 ανά μμ")
#define MSG_E5STEPS                         _UxGT("Bήματα Ε5 ανά μμ")
#define MSG_TEMPERATURE                     _UxGT("Θερμοκρασία")
#define MSG_MOTION                          _UxGT("Κίνηση")
#define MSG_FILAMENT                        _UxGT("Νήμα")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("Ε σε μμ3")
#define MSG_FILAMENT_DIAM                   _UxGT("Διάμετρος νήματος")
#define MSG_CONTRAST                        _UxGT("Κοντράστ LCD")
#define MSG_STORE_EEPROM                    _UxGT("Αποθήκευση")
#define MSG_LOAD_EEPROM                     _UxGT("Φόρτωση")
#define MSG_RESTORE_FAILSAFE                _UxGT("Επαναφορά ασφαλούς αντιγράφου")
#define MSG_REFRESH                         _UxGT("Ανανέωση")
#define MSG_WATCH                           _UxGT("Οθόνη πληροφόρησης")
#define MSG_PREPARE                         _UxGT("Προετοιμασία")
#define MSG_TUNE                            _UxGT("Συντονισμός")
#define MSG_PAUSE_PRINT                     _UxGT("Παύση εκτύπωσης")
#define MSG_RESUME_PRINT                    _UxGT("Συνέχιση εκτύπωσης")
#define MSG_STOP_PRINT                      _UxGT("Διακοπή εκτύπωσης")
#define MSG_MEDIA_MENU                      _UxGT("Εκτύπωση από SD")
#define MSG_NO_MEDIA                        _UxGT("Δεν βρέθηκε SD")
#define MSG_DWELL                           _UxGT("Αναστολή λειτουργίας…")
#define MSG_USERWAIT                        _UxGT("Αναμονή για χρήστη…")
#define MSG_PRINT_ABORTED                   _UxGT("Διακόπτεται η εκτύπωση")
#define MSG_NO_MOVE                         _UxGT("Καμία κίνηση.")
#define MSG_KILLED                          _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ")
#define MSG_STOPPED                         _UxGT("ΔΙΑΚΟΠΗ. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Ανάσυρση μμ")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Εναλλαγή ανάσυρσης μμ")
#define MSG_CONTROL_RETRACTF                _UxGT("Ανάσυρση V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Μεταπήδηση μμ")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("Αυτόματη ανάσυρση")
#define MSG_FILAMENTCHANGE                  _UxGT("Αλλαγή νήματος")
#define MSG_INIT_MEDIA                      _UxGT("Προετοιμασία κάρτας SD")
#define MSG_CHANGE_MEDIA                    _UxGT("Αλλαγή κάρτας SD")
#define MSG_ZPROBE_OUT                      _UxGT("Διερεύνηση Z εκτός κλίνης")
#define MSG_YX_UNHOMED                      _UxGT("Επαναφορά Χ/Υ πριν από Ζ")
#define MSG_XYZ_UNHOMED                     _UxGT("Επαναφορά ΧΥΖ πρώτα")
#define MSG_zprobe_zoffset                  _UxGT("Μετατόπιση Ζ")
#define MSG_BABYSTEP_X                      _UxGT("Μικρό βήμα Χ")
#define MSG_BABYSTEP_Y                      _UxGT("Μικρό βήμα Υ")
#define MSG_BABYSTEP_Z                      _UxGT("Μικρό βήμα Ζ")
#define MSG_ENDSTOP_ABORT                   _UxGT("Ματαίωση endstop ")
#define MSG_HEATING_FAILED_LCD              _UxGT("Ανεπιτυχής θέρμανση")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Λάθος: ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ")
#define MSG_THERMAL_RUNAWAY                 _UxGT("ΔΙΑΦΥΓΗ ΘΕΡΜΟΤΗΤΑΣ")
#define MSG_ERR_MAXTEMP                     _UxGT("Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ")
#define MSG_ERR_MINTEMP                     _UxGT("Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ")
#define MSG_HEATING                         _UxGT("Θερμαίνεται…")
#define MSG_BED_HEATING                     _UxGT("Θέρμανση κλίνης…")
#define MSG_DELTA_CALIBRATE                 _UxGT("Βαθμονόμηση Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Βαθμονόμηση X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Βαθμονόμηση Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Βαθμονόμηση Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Βαθμονόμηση κέντρου")

#define MSG_EXPECTED_PRINTER                _UxGT("Εσφαλμένος εκτυπωτής")
