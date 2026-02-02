/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Greek (Greece)
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

#define DISPLAY_CHARSET_ISO10646_GREEK

namespace LanguageNarrow_el_gr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Greek (Greece)");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" έτοιμο.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Εισαγωγή κάρτας");                          // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Αφαίρεση κάρτας");                          // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstops");                                 // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("Βασική Οθόνη");                             // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Απενεργοποίηση βηματιστή");                 // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Αυτομ. επαναφορά στο αρχικό σημείο");       // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Αρχικό σημείο X");                          // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Αρχικό σημείο Y");                          // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Αρχικό σημείο Z");                          // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Επαναφορά στο αρχικό σημείο ΧΥZ");          // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Κάντε κλικ για να ξεκινήσετε");             // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Επόμενο σημείο");                           // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Ολοκλήρωση επιπεδοποίησης!");               // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ορισμός βασικών μετατοπίσεων");             // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Εφαρμόστηκαν οι μετατοπίσεις");             // Offsets Applied
  LSTR MSG_PREHEAT_1                      = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL;             // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " ~";        // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" όλα"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" κλίνη"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" επιβεβαίωση"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Προθέρμανση $");                            // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Προθέρμανση $ ~");                          // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Προθέρμανση $ End");                        // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Προθέρμανση $ End ~");                      // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Προθέρμανση $ όλα");                        // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Προθέρμανση $ κλίνη");                      // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Προθέρμανση $ επιβεβαίωση");                // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Μειωση θερμοκρασιας");                      // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Ενεργοποίηση");                             // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Απενεργοποίηση");                           // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Εξώθηση");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Ανάσυρση");                                 // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Μετακίνηση άξονα");                         // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Ανιχνευτής/ισοπέδωση");                     // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Επιπεδοποίηση κλίνης");                     // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Επιπεδοποίηση κλίνης");                     // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Μετακίνηση X");                             // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Μετακίνηση Y");                             // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Μετακίνηση Z");                             // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Μετακίνηση @");                             // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Εξωθητήρας");                               // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Εξωθητήρας *");                             // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Μετακίνηση $μμ");                           // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Μετακίνηση $in");                           // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Μετακίνηση $") LCD_STR_DEGREE;              // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Ταχύτητα");                                 // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Κλίνη Z");                                  // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("Ακροφύσιο");                                // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Ακροφύσιο ~");                              // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Κλίνη");                                    // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Ταχύτητα ανεμιστήρα");                      // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Ταχύτητα ανεμιστήρα ~");                    // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Ροή");                                      // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Ροή ~");                                    // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Έλεγχος");                                  // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Αυτομ. ρύθμιση θερμοκρασίας");              // Autotemp
  LSTR MSG_ACC                            = _UxGT("Επιτάχυνση");                               // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vελάχ. μετατόπιση");                        // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("Accel");                                    // Acceleration
  LSTR MSG_AMAX_A                         = _UxGT("Aμεγ ") STR_A;                              // Max (STR_A) Accel
  LSTR MSG_AMAX_B                         = _UxGT("Aμεγ ") STR_B;                              // Max (STR_B) Accel
  LSTR MSG_AMAX_C                         = _UxGT("Aμεγ ") STR_C;                              // Max (STR_C) Accel
  LSTR MSG_AMAX_N                         = _UxGT("Aμεγ @");                                   // Max @ Accel
  LSTR MSG_AMAX_E                         = _UxGT("Aμεγ E");                                   // Max E Accel
  LSTR MSG_AMAX_EN                        = _UxGT("Aμεγ *");                                   // Max * Accel
  LSTR MSG_A_RETRACT                      = _UxGT("Α-ανάσυρση");                               // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("Α-μετατόπιση");                             // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Bήματα ανά μμ");                            // Steps/mm
  LSTR MSG_A_STEPS                        = _UxGT("Bήματα ") STR_A _UxGT(" ανά μμ");           // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = _UxGT("Bήματα ") STR_B _UxGT(" ανά μμ");           // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = _UxGT("Bήματα ") STR_C _UxGT(" ανά μμ");           // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("Bήματα @ ανά μμ");                          // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("Bήματα Ε ανά μμ");                          // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("Bήματα * ανά μμ");                          // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Θερμοκρασία");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Κίνηση");                                   // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Νήμα");                                     // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("Ε σε μμ") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Διάμετρος νήματος");                        // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Διάμετρος νήματος *");                      // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("Κοντράστ LCD");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Αποθήκευση");                               // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Φόρτωση");                                  // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Επαναφορά ασφαλούς αντιγράφου");            // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Ανανέωση");                 // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Οθόνη πληροφόρησης");                       // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Προετοιμασία");                             // Prepare
  LSTR MSG_TUNE                           = _UxGT("Συντονισμός");                              // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Παύση εκτύπωσης");                          // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Συνέχιση εκτύπωσης");                       // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Διακοπή εκτύπωσης");                        // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Εκτύπωση από SD");                          // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Δεν βρέθηκε SD");                           // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Αναστολή λειτουργίας…");                    // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Αναμονή για χρήστη…");                      // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Διακόπτεται η εκτύπωση");                   // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Καμία κίνηση.");                            // No Move.
  LSTR MSG_KILLED                         = _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ");                            // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("ΔΙΑΚΟΠΗ. ");                                // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Ανάσυρση μμ");                              // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Εναλλαγή ανάσυρσης μμ");                    // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Ανάσυρση V");                               // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Μεταπήδηση μμ");                            // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("S Unretr. mm");                             // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");                              // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Αυτόματη ανάσυρση");                        // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Αλλαγή νήματος");                           // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Αλλαγή νήματος *");                         // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Προετοιμασία SD");                          // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Προετοιμασία SD");                          // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Προετοιμασία USB");                         // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Αλλαγή κάρτας SD");                         // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Αυτόματη εκκίνηση");                        // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Διερεύνηση Z εκτός κλίνης");                // Z Probe Past Bed
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Μετατόπιση X");                             // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Μετατόπιση Y");                             // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Μετατόπιση Z");                             // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Μετατόπιση @");                             // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Βήμα Z ρύθμιση");                           // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Μικρό βήμα Χ");                             // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Μικρό βήμα Υ");                             // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Μικρό βήμα Z");                             // Babystep Z
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Ματαίωση endstop ");                        // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Ανεπιτυχής θέρμανση");                      // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Λάθος: ΠΛΕΟΝΑZΟΥΣΑ ΘΕΡΜΟΤΗΤΑ");             // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("ΔΙΑΦΥΓΗ ΘΕΡΜΟΤΗΤΑΣ");                       // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ");                 // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ");                // Err: MINTEMP
  LSTR MSG_HEATING                        = _UxGT("Θερμαίνεται…");                             // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("Θέρμανση κλίνης…");                         // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Βαθμονόμηση Delta");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Βαθμονόμηση X");                            // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Βαθμονόμηση Y");                            // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Βαθμονόμηση Z");                            // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Βαθμονόμηση κέντρου");                      // Calibrate Center
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Ενεργοποιημένο");                           // On
  LSTR MSG_LCD_OFF                        = _UxGT("Απενεργοποιημένο");                         // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Εσφαλμένος εκτυπωτής");                     // INCORRECT PRINTER
  LSTR MSG_JERK                           = _UxGT("Vαντίδραση");                               // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("Vαντίδραση ") STR_A;                        // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("Vαντίδραση ") STR_B;                        // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("Vαντίδραση ") STR_C;                        // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("Vαντίδραση @");                             // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("Vαντίδραση E");                             // Max E Jerk
  LSTR MSG_VMAX_A                         = _UxGT("Vμεγ ") STR_A;                              // Max (STR_A) Speed
  LSTR MSG_VMAX_B                         = _UxGT("Vμεγ ") STR_B;                              // Max (STR_B) Speed
  LSTR MSG_VMAX_C                         = _UxGT("Vμεγ ") STR_C;                              // Max (STR_C) Speed
  LSTR MSG_VMAX_N                         = _UxGT("Vμεγ @");                                   // Max @ Speed
  LSTR MSG_VMAX_E                         = _UxGT("Vμεγ E");                                   // Max E Speed
  LSTR MSG_VMAX_EN                        = _UxGT("Vμεγ *");                                   // Max * Speed
  LSTR MSG_VMIN                           = _UxGT("Vελαχ");                                    // Min Velocity
}

namespace LanguageWide_el_gr {
  using namespace LanguageNarrow_el_gr;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_el_gr {
  using namespace LanguageWide_el_gr;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_el_gr {
  using namespace LanguageTall_el_gr;
}
