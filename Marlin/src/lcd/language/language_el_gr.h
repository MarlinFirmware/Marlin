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
 */

#define DISPLAY_CHARSET_ISO10646_GREEK

namespace Language_el_gr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Greek");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" έτοιμος.");
  PROGMEM Language_Str MSG_YES                             = _UxGT("ΝΑΙ");
  PROGMEM Language_Str MSG_NO                              = _UxGT("ΟΧΙ");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Πίσω");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Προχωρημένες ρυθμίσεις");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Διαμόρφωση");

  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Κάρτα εισήχθη");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Κάρτα αφαιρέθη");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Αναμονή για κάρτα");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Ματαίωση...");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = MEDIA_TYPE_EN _UxGT(" σφάλμα ανάγνωσης");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB αφαιρέθη");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Αποτυχία εκκίνησης USB");
  PROGMEM Language_Str MSG_SD_INIT_FAIL                    = _UxGT("Αποτυχία αρχικοποίησης SD");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Αρχική Οθόνη");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Αυτόματη εκκίνηση");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Απενεργοποίηση μοτέρ");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Αυτόμ. επαναφορά XYZ");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Επαναφορά X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Επαναφορά Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Επαναφορά Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Αυτόμ. επαναφορά XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Πιέστε για έναρξη");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Επόμενο σημείο");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Τέλος επιπεδοποίησης!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ορισμός μετατοπίσεων");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Εφαρμογή μετατοπίσεων");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Ορισμός προέλευσης");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" όλα");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" bed"); //SHORTEN
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" επιβεβαίωση"); //SHORTEN

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Προθέρμανση $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Προθέρμανση $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Προθέρμανση $ End");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Προθέρμανση $ End ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Προθέρμανση $ όλα");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Προθέρμανση $ bed"); //SHORTEN
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Προθέρμανση $ επιβεβαίωση"); //SHORTEN
  #endif
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Αποθέρμανση");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Ενεργοποίηση");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Απενεργοποίηση");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Εξώθηση");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Ανάσυρση");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Μετακίνηση άξονα");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Επιπεδοποίηση κλίνης");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Επιπεδοποίηση κλίνης");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Μετακίνηση X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Μετακίνηση Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Μετακίνηση Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Εξωθητής");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Εξωθητής *");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Μετακίνηση %s μμ");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Μετακίνηση 0,1 μμ");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Μετακίνηση 1 μμ");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Μετακίνηση 10 μμ");
  PROGMEM Language_Str MSG_MOVE_100MM                      = _UxGT("Μετακίνηση 100 μμ");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Ταχύτητα");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Επ. Εκτύπωσης Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Ακροφύσιο");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Ακροφύσιο ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Επ. Εκτύπωσης");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Ταχύτητα ανεμιστήρα");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Ταχύτητα ανεμιστήρα ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Ροή");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Ροή ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Έλεγχος");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Αυτορρύθμιση θερ/σίας");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Ενεργοποιημένο");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Απενεργοποιημένο");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Επιτάχυνση");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_C;
  PROGMEM Language_Str MSG_VI_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_I;
  PROGMEM Language_Str MSG_VJ_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_J;
  PROGMEM Language_Str MSG_VK_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_K;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Vαντίδραση E");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("V Μέγιστο") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("V Μέγιστο") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("V Μέγιστο") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_I                          = _UxGT("V Μέγιστο") LCD_STR_I;
  PROGMEM Language_Str MSG_VMAX_J                          = _UxGT("V Μέγιστο") LCD_STR_J;
  PROGMEM Language_Str MSG_VMAX_K                          = _UxGT("V Μέγιστο") LCD_STR_K;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("V Μέγιστο") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("V Μέγιστο *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("V Ελάχιστο");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vελάχ. μετατόπιση");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Accel");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Aμεγ ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Aμεγ ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Aμεγ ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_I                          = _UxGT("Aμεγ ") LCD_STR_I;
  PROGMEM Language_Str MSG_AMAX_J                          = _UxGT("Aμεγ ") LCD_STR_J;
  PROGMEM Language_Str MSG_AMAX_K                          = _UxGT("Aμεγ ") LCD_STR_K;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Aμεγ ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Aμεγ *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Α-ανάσυρση");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Α-μετατόπιση");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Bήματα ανά μμ");
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("Bήματα ") LCD_STR_A _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("Bήματα ") LCD_STR_B _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("Bήματα ") LCD_STR_C _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_I_STEPS                         = _UxGT("Bήματα ") LCD_STR_I _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_J_STEPS                         = _UxGT("Bήματα ") LCD_STR_J _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_K_STEPS                         = _UxGT("Bήματα ") LCD_STR_K _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Bήματα Ε ανά μμ");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("Bήματα * ανά μμ");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Θερμοκρασία");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Κίνηση");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Νήμα");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("Ε σε mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Διάμετρος νήματος");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Διάμετρος νήματος *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Αντίθεση LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Αποθήκευση σε EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Φόρτωση από EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Επαναφορά προεπιλογών");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Ανανέωση");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Οθόνη πληροφόρησης");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Προετοιμασία");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Συντονισμός");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Παύση εκτύπωσης");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Συνέχιση εκτύπωσης");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Διακοπή εκτύπωσης");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Εκτύπωση από SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Δεν βρέθηκε SD");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Αναστολή λειτουργίας");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Αναμονή για χρήστη");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Εκτύπωση διεκόπη");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Καμία κίνηση.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ΔΙΑΚΟΠΗ. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Ανάσυρση μμ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Εναλλαγή ανάσυρσης mm");  //SHORTEN
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Ανάσυρση V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Μεταπήδηση mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("S Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Unretract V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Αυτόματη ανάσυρση");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Αλλαγή νήματος");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Αλλαγή νήματος *");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Προετοιμασία SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Αλλαγή κάρτας SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Διερεύνηση Z εκτός Επ.Εκτύπωσης"); //SHORTEN
  PROGMEM Language_Str MSG_YX_UNHOMED                      = _UxGT("Επαναφορά Χ/Υ πρώτα");
  PROGMEM Language_Str MSG_XYZ_UNHOMED                     = _UxGT("Επαναφορά ΧΥΖ πρώτα");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Μετατόπιση Ζ");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Μικρό βήμα Χ");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Μικρό βήμα Υ");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Μικρό βήμα Ζ");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Ακύρωση endstop");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Αποτυχία θέρμανσης");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ΘΕΡΜΙΚΗ ΔΙΑΦΥΓΗ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("ΠΕΡΙΤΤΗ ΘΕΡΜΟΚΡΑΣΙΑ");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("ΑΝΕΠΑΡΚΗΣ ΘΕΡΜΟΚΡΑΣΙΑ");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("Εκτυπωτής διεκόπη");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Θερμαίνεται…");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Θέρμανση κλίνης");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Βαθμονόμηση Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Βαθμονόμηση X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Βαθμονόμηση Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Βαθμονόμηση Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Βαθμονόμηση κέντρου");

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Εσφαλμένος εκτυπωτής");
}
