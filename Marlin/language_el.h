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

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" έτοιμο.")
#define MSG_SD_INSERTED                     _UxGT("Εισαγωγή κάρτας")
#define MSG_SD_REMOVED                      _UxGT("Αφαίρεση κάρτας")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Βασική Οθόνη")
#define MSG_AUTOSTART                       _UxGT("Αυτόματη εκκίνηση")
#define MSG_DISABLE_STEPPERS                _UxGT("Απενεργοποίηση Μοτέρ")
#define MSG_AUTO_HOME                       _UxGT("Αυτομ. επαναφορά στο αρχικό σημείο") //SHORTEN
#define MSG_AUTO_HOME_X                     _UxGT("Αρχικό σημείο X")
#define MSG_AUTO_HOME_Y                     _UxGT("Αρχικό σημείο Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Αρχικό σημείο Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Επαναφορά Επ. Εκτύπωσης") //SHORTEN
#define MSG_LEVEL_BED_WAITING               _UxGT("Επιπεδοποίηση επ. Εκτύπωσης περιμενει") //SHORTEN
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Επόμενο σημείο")
#define MSG_LEVEL_BED_DONE                  _UxGT("Ολοκλήρωση επιπεδοποίησης!") //SHORTEN
#define MSG_SET_HOME_OFFSETS                _UxGT("Ορισμός βασικών μετατοπίσεων") //SHORTEN
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Εφαρμόστηκαν οι μετατοπίσεις") //SHORTEN
#define MSG_SET_ORIGIN                      _UxGT("Ορισμός προέλευσης")
#define MSG_PREHEAT_1                       _UxGT("Προθέρμανση PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" όλα")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" bed") //SHORTEN
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" επιβεβαίωση") //SHORTEN
#define MSG_PREHEAT_2                       _UxGT("Προθέρμανση ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" όλα")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" bed") //SHORTEN
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" επιβεβαίωση") //SHORTEN
#define MSG_COOLDOWN                        _UxGT("Μειωση θερμοκρασιας")
#define MSG_SWITCH_PS_ON                    _UxGT("Ενεργοποίηση")
#define MSG_SWITCH_PS_OFF                   _UxGT("Απενεργοποίηση")
#define MSG_EXTRUDE                         _UxGT("Εξώθηση")
#define MSG_RETRACT                         _UxGT("Ανάσυρση")
#define MSG_MOVE_AXIS                       _UxGT("Μετακίνηση άξονα")
#define MSG_BED_LEVELING                    _UxGT("Επιπεδοποίηση Επ. Εκτύπωσης") //SHORTEN
#define MSG_LEVEL_BED                       _UxGT("Επιπεδοποίηση Επ. Εκτύπωσης") //SHORTEN
#define MSG_MOVE_X                          _UxGT("Μετακίνηση X")
#define MSG_MOVE_Y                          _UxGT("Μετακίνηση Y")
#define MSG_MOVE_Z                          _UxGT("Μετακίνηση Z")
#define MSG_MOVE_E                          _UxGT("Εξωθητήρας")
#define MSG_MOVE_01MM                       _UxGT("Μετακίνηση 0,1μμ")
#define MSG_MOVE_1MM                        _UxGT("Μετακίνηση 1μμ")
#define MSG_MOVE_10MM                       _UxGT("Μετακίνηση 10μμ")
#define MSG_SPEED                           _UxGT("Ταχύτητα")
#define MSG_BED_Z                           _UxGT("Επ. Εκτύπωσης Z")
#define MSG_NOZZLE                          _UxGT("Ακροφύσιο")
#define MSG_BED                             _UxGT("Κλίνη")
#define MSG_FAN_SPEED                       _UxGT("Ταχύτητα ανεμιστήρα")
#define MSG_FLOW                            _UxGT("Ροή")
#define MSG_CONTROL                         _UxGT("Έλεγχος")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Αυτομ ρύθμιση θερ/σίας") //SHORTEN
#define MSG_ON                              _UxGT("Ενεργοποιημένο")
#define MSG_OFF                             _UxGT("Απενεργοποιημένο")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Επιτάχυνση")
#define MSG_JERK                            _UxGT("Jerk")
#define MSG_VX_JERK                         _UxGT("Vαντίδραση x")
#define MSG_VY_JERK                         _UxGT("Vαντίδραση y")
#define MSG_VZ_JERK                         _UxGT("Vαντίδραση z")
#define MSG_VE_JERK                         _UxGT("Vαντίδραση e")
#define MSG_VMAX                            _UxGT("V Μέγιστο")
#define MSG_VMIN                            _UxGT("V Ελάχιστο")
#define MSG_VTRAV_MIN                       _UxGT("Vελάχ. μετατόπιση")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Aμεγ ")
#define MSG_A_RETRACT                       _UxGT("Α-ανάσυρση")
#define MSG_A_TRAVEL                        _UxGT("Α-μετατόπιση")
#define MSG_STEPS_PER_MM                    _UxGT("Bήματα ανά μμ")
#define MSG_XSTEPS                          _UxGT("Bήματα X ανά μμ")
#define MSG_YSTEPS                          _UxGT("Bήματα Υ ανά μμ")
#define MSG_ZSTEPS                          _UxGT("Bήματα Ζ ανά μμ")
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
#define MSG_RESTORE_FAILSAFE                _UxGT("Επαναφορά ασφαλούς αντιγράφου") //SHORTEN
#define MSG_REFRESH                         _UxGT("Ανανέωση")
#define MSG_WATCH                           _UxGT("Οθόνη πληροφόρησης")
#define MSG_PREPARE                         _UxGT("Προετοιμασία")
#define MSG_TUNE                            _UxGT("Συντονισμός")
#define MSG_PAUSE_PRINT                     _UxGT("Παύση εκτύπωσης")
#define MSG_RESUME_PRINT                    _UxGT("Συνέχιση εκτύπωσης")
#define MSG_STOP_PRINT                      _UxGT("Διακοπή εκτύπωσης")
#define MSG_CARD_MENU                       _UxGT("Εκτύπωση από SD")
#define MSG_NO_CARD                         _UxGT("Δεν βρέθηκε SD")
#define MSG_DWELL                           _UxGT("Αναστολή λειτουργίας")
#define MSG_USERWAIT                        _UxGT("Αναμονή για χρήστη")
#define MSG_RESUMING                        _UxGT("Συνεχίζεται η εκτύπωση") //SHORTEN
#define MSG_PRINT_ABORTED                   _UxGT("Διακόπτεται η εκτύπωση") //SHORTEN
#define MSG_NO_MOVE                         _UxGT("Καμία κίνηση.")
#define MSG_KILLED                          _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ")
#define MSG_STOPPED                         _UxGT("ΔΙΑΚΟΠΗ. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Ανάσυρση μμ")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Εναλλαγή ανάσυρσης μμ")  //SHORTEN
#define MSG_CONTROL_RETRACTF                _UxGT("Ανάσυρση V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Μεταπήδηση μμ")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("Αυτόματη ανάσυρση")
#define MSG_FILAMENTCHANGE                  _UxGT("Αλλαγή νήματος")
#define MSG_INIT_SDCARD                     _UxGT("Προετοιμασία κάρτας SD")  //SHORTEN
#define MSG_CNG_SDCARD                      _UxGT("Αλλαγή κάρτας SD")
#define MSG_ZPROBE_OUT                      _UxGT("Διερεύνηση Z εκτός Επ.Εκτύπωσης") //SHORTEN
#define MSG_YX_UNHOMED                      _UxGT("Επαναφορά Χ/Υ πριν από Ζ") //SHORTEN
#define MSG_XYZ_UNHOMED                     _UxGT("Επαναφορά ΧΥΖ πρώτα")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Μετατόπιση Ζ")
#define MSG_BABYSTEP_X                      _UxGT("Μικρό βήμα Χ")
#define MSG_BABYSTEP_Y                      _UxGT("Μικρό βήμα Υ")
#define MSG_BABYSTEP_Z                      _UxGT("Μικρό βήμα Ζ")
#define MSG_ENDSTOP_ABORT                   _UxGT("Ακύρωση endstop ")
#define MSG_HEATING_FAILED_LCD              _UxGT("Ανεπιτυχής θέρμανση")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ")
#define MSG_THERMAL_RUNAWAY                 _UxGT("ΔΙΑΦΥΓΗ ΘΕΡΜΟΚΡΑΣΙΑΣ")
#define MSG_ERR_MAXTEMP                     _UxGT("ΠΕΡΙΤΗ ΘΕΡΜΟΚΡΑΣΙΑ")
#define MSG_ERR_MINTEMP                     _UxGT("ΜΗ ΕΠΑΡΚΗΣ ΘΕΡΜΟΚΡΑΣΙΑΣ") //SHORTEN
#define MSG_ERR_MAXTEMP_BED                 _UxGT("ΜΕΓΙΣΤΗ ΘΕΡΜΟΚΡΑΣΙΑΣ ΕΠ. ΕΚΤΥΠΩΣΗΣ") //SHORTEN
#define MSG_ERR_MINTEMP_BED                 _UxGT("ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΚΡΑΣΙΑΣ ΕΠ. ΕΚΤΥΠΩΣΗΣ") //SHORTEN
#define MSG_HALTED                          _UxGT("H εκτύπωση διακόπηκε")
#define MSG_PLEASE_RESET                    _UxGT("PLEASE RESET") //TRANSLATE
#define MSG_HEATING                         _UxGT("Θερμαίνεται…")
#define MSG_HEATING_COMPLETE                _UxGT("Η θέρμανση ολοκληρώθηκε.") //SHORTEN
#define MSG_BED_HEATING                     _UxGT("Θέρμανση ΕΠ. Εκτύπωσης") //SHORTEN
#define MSG_BED_DONE                        _UxGT("Η Επ. Εκτύπωσης ολοκληρώθηκε") //SHORTEN
#define MSG_DELTA_CALIBRATE                 _UxGT("Βαθμονόμηση Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Βαθμονόμηση X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Βαθμονόμηση Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Βαθμονόμηση Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Βαθμονόμηση κέντρου")

#define MSG_INFO_MENU                       _UxGT("About Printer")
#define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Print Count")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed  ")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total Time ")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Prints   ")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #define MSG_INFO_PRINT_TIME               _UxGT("Duration ")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Power Supply")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("CHANGE FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extrude more")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resume print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wait for start")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("of the filament")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("change")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("and press button")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("to continue...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("filament extrude")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Please wait...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejecting...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert and Click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extruding...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Resuming...")
#endif

#endif // LANGUAGE_EL_H
