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
 * Greek
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_GREEK

namespace Language_el {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Greek (Greece)");

  LSTR WELCOME_MSG                        = MACHINE_NAME _UxGT(" έτοιμος.");
  LSTR MSG_YES                            = _UxGT("ΝΑΙ");
  LSTR MSG_NO                             = _UxGT("ΟΧΙ");
  LSTR MSG_BACK                           = _UxGT("Πίσω");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Προχωρημένες ρυθμίσεις");
  LSTR MSG_CONFIGURATION                  = _UxGT("Διαμόρφωση");

  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Κάρτα εισήχθη");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Κάρτα αφαιρέθη");
  LSTR MSG_MEDIA_WAITING                  = _UxGT("Αναμονή για κάρτα");
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Ματαίωση...");
  LSTR MSG_MEDIA_READ_ERROR               = MEDIA_TYPE_EN _UxGT(" σφάλμα ανάγνωσης");
  LSTR MSG_MEDIA_USB_REMOVED              = _UxGT("USB αφαιρέθη");
  LSTR MSG_MEDIA_USB_FAILED               = _UxGT("Αποτυχία εκκίνησης USB");
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Αποτυχία αρχικοποίησης SD");
  LSTR MSG_MAIN                           = _UxGT("Αρχική Οθόνη");
  LSTR MSG_RUN_AUTO_FILES                 = _UxGT("Αυτόματη εκκίνηση");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Απενεργοποίηση μοτέρ");
  LSTR MSG_AUTO_HOME                      = _UxGT("Αυτόμ. επαναφορά XYZ");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Επαναφορά X");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Επαναφορά Y");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Επαναφορά Z");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Αυτόμ. επαναφορά XYZ");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Πιέστε για έναρξη");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Επόμενο σημείο");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Τέλος επιπεδοποίησης!");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ορισμός μετατοπίσεων");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Εφαρμογή μετατοπίσεων");
  LSTR MSG_SET_ORIGIN                     = _UxGT("Ορισμός προέλευσης");
  #if HAS_PREHEAT
    LSTR MSG_PREHEAT_1                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL;
    LSTR MSG_PREHEAT_1_H                  = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " ~";
    LSTR MSG_PREHEAT_1_END                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End");
    LSTR MSG_PREHEAT_1_END_E              = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ~");
    LSTR MSG_PREHEAT_1_ALL                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" όλα");
    LSTR MSG_PREHEAT_1_BEDONLY            = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" bed"); //SHORTEN
    LSTR MSG_PREHEAT_1_SETTINGS           = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" επιβεβαίωση"); //SHORTEN

    LSTR MSG_PREHEAT_M                    = _UxGT("Προθέρμανση $");
    LSTR MSG_PREHEAT_M_H                  = _UxGT("Προθέρμανση $ ~");
    LSTR MSG_PREHEAT_M_END                = _UxGT("Προθέρμανση $ End");
    LSTR MSG_PREHEAT_M_END_E              = _UxGT("Προθέρμανση $ End ~");
    LSTR MSG_PREHEAT_M_ALL                = _UxGT("Προθέρμανση $ όλα");
    LSTR MSG_PREHEAT_M_BEDONLY            = _UxGT("Προθέρμανση $ bed"); //SHORTEN
    LSTR MSG_PREHEAT_M_SETTINGS           = _UxGT("Προθέρμανση $ επιβεβαίωση"); //SHORTEN
  #endif
  LSTR MSG_COOLDOWN                       = _UxGT("Αποθέρμανση");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Ενεργοποίηση");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Απενεργοποίηση");
  LSTR MSG_EXTRUDE                        = _UxGT("Εξώθηση");
  LSTR MSG_RETRACT                        = _UxGT("Ανάσυρση");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Μετακίνηση άξονα");
  LSTR MSG_BED_LEVELING                   = _UxGT("Επιπεδοποίηση κλίνης");
  LSTR MSG_LEVEL_BED                      = _UxGT("Επιπεδοποίηση κλίνης");
  LSTR MSG_MOVE_X                         = _UxGT("Μετακίνηση X");
  LSTR MSG_MOVE_Y                         = _UxGT("Μετακίνηση Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Μετακίνηση Z");
  LSTR MSG_MOVE_E                         = _UxGT("Εξωθητής");
  LSTR MSG_MOVE_EN                        = _UxGT("Εξωθητής *");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Μετακίνηση %s μμ");
  LSTR MSG_MOVE_01MM                      = _UxGT("Μετακίνηση 0,1 μμ");
  LSTR MSG_MOVE_1MM                       = _UxGT("Μετακίνηση 1 μμ");
  LSTR MSG_MOVE_10MM                      = _UxGT("Μετακίνηση 10 μμ");
  LSTR MSG_MOVE_100MM                     = _UxGT("Μετακίνηση 100 μμ");
  LSTR MSG_SPEED                          = _UxGT("Ταχύτητα");
  LSTR MSG_BED_Z                          = _UxGT("Επ. Εκτύπωσης Z");
  LSTR MSG_NOZZLE                         = _UxGT("Ακροφύσιο");
  LSTR MSG_NOZZLE_N                       = _UxGT("Ακροφύσιο ~");
  LSTR MSG_BED                            = _UxGT("Επ. Εκτύπωσης");
  LSTR MSG_FAN_SPEED                      = _UxGT("Ταχύτητα ανεμιστήρα");
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Ταχύτητα ανεμιστήρα ~");
  LSTR MSG_FLOW                           = _UxGT("Ροή");
  LSTR MSG_FLOW_N                         = _UxGT("Ροή ~");
  LSTR MSG_CONTROL                        = _UxGT("Έλεγχος");
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  LSTR MSG_AUTOTEMP                       = _UxGT("Αυτορρύθμιση θερ/σίας");
  LSTR MSG_LCD_ON                         = _UxGT("Ενεργοποιημένο");
  LSTR MSG_LCD_OFF                        = _UxGT("Απενεργοποιημένο");
  LSTR MSG_ACC                            = _UxGT("Επιτάχυνση");
  LSTR MSG_VA_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_A;
  LSTR MSG_VB_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_B;
  LSTR MSG_VC_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_C;
  LSTR MSG_VI_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_I;
  LSTR MSG_VJ_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_J;
  LSTR MSG_VK_JERK                        = _UxGT("Vαντίδραση ") LCD_STR_K;
  LSTR MSG_VE_JERK                        = _UxGT("Vαντίδραση E");
  LSTR MSG_VMAX_A                         = _UxGT("V Μέγιστο") LCD_STR_A;
  LSTR MSG_VMAX_B                         = _UxGT("V Μέγιστο") LCD_STR_B;
  LSTR MSG_VMAX_C                         = _UxGT("V Μέγιστο") LCD_STR_C;
  LSTR MSG_VMAX_I                         = _UxGT("V Μέγιστο") LCD_STR_I;
  LSTR MSG_VMAX_J                         = _UxGT("V Μέγιστο") LCD_STR_J;
  LSTR MSG_VMAX_K                         = _UxGT("V Μέγιστο") LCD_STR_K;
  LSTR MSG_VMAX_E                         = _UxGT("V Μέγιστο") LCD_STR_E;
  LSTR MSG_VMAX_EN                        = _UxGT("V Μέγιστο *");
  LSTR MSG_VMIN                           = _UxGT("V Ελάχιστο");
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vελάχ. μετατόπιση");
  LSTR MSG_ACCELERATION                   = _UxGT("Accel");
  LSTR MSG_AMAX_A                         = _UxGT("Aμεγ ") LCD_STR_A;
  LSTR MSG_AMAX_B                         = _UxGT("Aμεγ ") LCD_STR_B;
  LSTR MSG_AMAX_C                         = _UxGT("Aμεγ ") LCD_STR_C;
  LSTR MSG_AMAX_I                         = _UxGT("Aμεγ ") LCD_STR_I;
  LSTR MSG_AMAX_J                         = _UxGT("Aμεγ ") LCD_STR_J;
  LSTR MSG_AMAX_K                         = _UxGT("Aμεγ ") LCD_STR_K;
  LSTR MSG_AMAX_E                         = _UxGT("Aμεγ ") LCD_STR_E;
  LSTR MSG_AMAX_EN                        = _UxGT("Aμεγ *");
  LSTR MSG_A_RETRACT                      = _UxGT("Α-ανάσυρση");
  LSTR MSG_A_TRAVEL                       = _UxGT("Α-μετατόπιση");
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Bήματα ανά μμ");
  LSTR MSG_A_STEPS                        = _UxGT("Bήματα ") LCD_STR_A _UxGT(" ανά μμ");
  LSTR MSG_B_STEPS                        = _UxGT("Bήματα ") LCD_STR_B _UxGT(" ανά μμ");
  LSTR MSG_C_STEPS                        = _UxGT("Bήματα ") LCD_STR_C _UxGT(" ανά μμ");
  LSTR MSG_I_STEPS                        = _UxGT("Bήματα ") LCD_STR_I _UxGT(" ανά μμ");
  LSTR MSG_J_STEPS                        = _UxGT("Bήματα ") LCD_STR_J _UxGT(" ανά μμ");
  LSTR MSG_K_STEPS                        = _UxGT("Bήματα ") LCD_STR_K _UxGT(" ανά μμ");
  LSTR MSG_E_STEPS                        = _UxGT("Bήματα Ε ανά μμ");
  LSTR MSG_EN_STEPS                       = _UxGT("Bήματα * ανά μμ");
  LSTR MSG_TEMPERATURE                    = _UxGT("Θερμοκρασία");
  LSTR MSG_MOTION                         = _UxGT("Κίνηση");
  LSTR MSG_FILAMENT                       = _UxGT("Νήμα");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("Ε σε mm") SUPERSCRIPT_THREE;
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Διάμετρος νήματος");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Διάμετρος νήματος *");
  LSTR MSG_CONTRAST                       = _UxGT("Αντίθεση LCD");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Αποθήκευση σε EEPROM");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Φόρτωση από EEPROM");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Επαναφορά προεπιλογών");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH  _UxGT("Ανανέωση");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Οθόνη πληροφόρησης");
  LSTR MSG_PREPARE                        = _UxGT("Προετοιμασία");
  LSTR MSG_TUNE                           = _UxGT("Συντονισμός");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Παύση εκτύπωσης");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Συνέχιση εκτύπωσης");
  LSTR MSG_STOP_PRINT                     = _UxGT("Διακοπή εκτύπωσης");
  LSTR MSG_MEDIA_MENU                     = _UxGT("Εκτύπωση από SD");
  LSTR MSG_NO_MEDIA                       = _UxGT("Δεν βρέθηκε SD");
  LSTR MSG_DWELL                          = _UxGT("Αναστολή λειτουργίας");
  LSTR MSG_USERWAIT                       = _UxGT("Αναμονή για χρήστη");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Εκτύπωση διεκόπη");
  LSTR MSG_NO_MOVE                        = _UxGT("Καμία κίνηση.");
  LSTR MSG_KILLED                         = _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ");
  LSTR MSG_STOPPED                        = _UxGT("ΔΙΑΚΟΠΗ. ");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Ανάσυρση μμ");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Εναλλαγή ανάσυρσης mm");  //SHORTEN
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Ανάσυρση V");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Μεταπήδηση mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("S Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");
  LSTR MSG_AUTORETRACT                    = _UxGT("Αυτόματη ανάσυρση");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Αλλαγή νήματος");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Αλλαγή νήματος *");
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Προετοιμασία SD");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Αλλαγή κάρτας SD");
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Διερεύνηση Z εκτός Επ.Εκτύπωσης"); //SHORTEN
  LSTR MSG_YX_UNHOMED                     = _UxGT("Επαναφορά Χ/Υ πρώτα");
  LSTR MSG_XYZ_UNHOMED                    = _UxGT("Επαναφορά ΧΥΖ πρώτα");
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Μετατόπιση Ζ");
  LSTR MSG_BABYSTEP_X                     = _UxGT("Μικρό βήμα Χ");
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Μικρό βήμα Υ");
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Μικρό βήμα Ζ");
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Ακύρωση endstop");
  LSTR MSG_HEATING_FAILED_LCD             = _UxGT("Αποτυχία θέρμανσης");
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ");
  LSTR MSG_THERMAL_RUNAWAY                = _UxGT("ΘΕΡΜΙΚΗ ΔΙΑΦΥΓΗ");
  LSTR MSG_TEMP_MALFUNCTION               = _UxGT("ΘΕΡΜΙΚΗ ΔΥΣΛΕΙΤΟΥΡΓΙΑ");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("ΠΕΡΙΤΤΗ ΘΕΡΜΟΚΡΑΣΙΑ");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("ΑΝΕΠΑΡΚΗΣ ΘΕΡΜΟΚΡΑΣΙΑ");
  LSTR MSG_HALTED                         = _UxGT("Εκτυπωτής διεκόπη");
  LSTR MSG_HEATING                        = _UxGT("Θερμαίνεται…");
  LSTR MSG_BED_HEATING                    = _UxGT("Θέρμανση κλίνης");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Βαθμονόμηση Delta");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Βαθμονόμηση X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Βαθμονόμηση Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Βαθμονόμηση Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Βαθμονόμηση κέντρου");

  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Εσφαλμένος εκτυπωτής");
}
