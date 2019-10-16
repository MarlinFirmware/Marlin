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

namespace Language_el_gr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Greek (Greece)");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" έτοιμο.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Εισαγωγή κάρτας");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Αφαίρεση κάρτας");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Βασική Οθόνη");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Αυτόματη εκκίνηση");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Απενεργοποίηση βηματιστή");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Αυτομ. επαναφορά στο αρχικό σημείο");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Αρχικό σημείο X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Αρχικό σημείο Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Αρχικό σημείο Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Επαναφορά στο αρχικό σημείο ΧΥΖ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Κάντε κλικ για να ξεκινήσετε");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Επόμενο σημείο");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Ολοκλήρωση επιπεδοποίησης!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ορισμός βασικών μετατοπίσεων");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Εφαρμόστηκαν οι μετατοπίσεις");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Ορισμός προέλευσης");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H0                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_H1                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_H2                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_H3                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_H4                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_H5                    = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E0                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E1                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E2                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E3                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E4                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E5                = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" End ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" όλα");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" κλίνη");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Προθέρμανση ") PREHEAT_1_LABEL _UxGT(" επιβεβαίωση");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H0                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_H1                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_H2                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_H3                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_H4                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_H5                    = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E0                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E1                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E2                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E3                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E4                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E5                = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" End ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" όλα");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" κλίνη");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Προθέρμανση ") PREHEAT_2_LABEL _UxGT(" επιβεβαίωση");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Μειωση θερμοκρασιας");
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
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Εξωθητήρας");
  PROGMEM Language_Str MSG_MOVE_E0                         = _UxGT("Εξωθητήρας ") LCD_STR_E0;
  PROGMEM Language_Str MSG_MOVE_E1                         = _UxGT("Εξωθητήρας ") LCD_STR_E1;
  PROGMEM Language_Str MSG_MOVE_E2                         = _UxGT("Εξωθητήρας ") LCD_STR_E2;
  PROGMEM Language_Str MSG_MOVE_E3                         = _UxGT("Εξωθητήρας ") LCD_STR_E3;
  PROGMEM Language_Str MSG_MOVE_E4                         = _UxGT("Εξωθητήρας ") LCD_STR_E4;
  PROGMEM Language_Str MSG_MOVE_E5                         = _UxGT("Εξωθητήρας ") LCD_STR_E5;
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Μετακίνηση %s μμ");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Μετακίνηση 0,1 μμ");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Μετακίνηση 1 μμ");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Μετακίνηση 10 μμ");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Ταχύτητα");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Κλίνη Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Ακροφύσιο");
  PROGMEM Language_Str MSG_NOZZLE_0                        = _UxGT("Ακροφύσιο ") LCD_STR_N0;
  PROGMEM Language_Str MSG_NOZZLE_1                        = _UxGT("Ακροφύσιο ") LCD_STR_N1;
  PROGMEM Language_Str MSG_NOZZLE_2                        = _UxGT("Ακροφύσιο ") LCD_STR_N2;
  PROGMEM Language_Str MSG_NOZZLE_3                        = _UxGT("Ακροφύσιο ") LCD_STR_N3;
  PROGMEM Language_Str MSG_NOZZLE_4                        = _UxGT("Ακροφύσιο ") LCD_STR_N4;
  PROGMEM Language_Str MSG_NOZZLE_5                        = _UxGT("Ακροφύσιο ") LCD_STR_N5;
  PROGMEM Language_Str MSG_BED                             = _UxGT("Κλίνη");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Ταχύτητα ανεμιστήρα");
  PROGMEM Language_Str MSG_FAN_SPEED_1                     = _UxGT("Ταχύτητα ανεμιστήρα 1");
  PROGMEM Language_Str MSG_FAN_SPEED_2                     = _UxGT("Ταχύτητα ανεμιστήρα 2");
  PROGMEM Language_Str MSG_FAN_SPEED_3                     = _UxGT("Ταχύτητα ανεμιστήρα 3");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Ροή");
  PROGMEM Language_Str MSG_FLOW_0                          = _UxGT("Ροή ") LCD_STR_N0;
  PROGMEM Language_Str MSG_FLOW_1                          = _UxGT("Ροή ") LCD_STR_N1;
  PROGMEM Language_Str MSG_FLOW_2                          = _UxGT("Ροή ") LCD_STR_N2;
  PROGMEM Language_Str MSG_FLOW_3                          = _UxGT("Ροή ") LCD_STR_N3;
  PROGMEM Language_Str MSG_FLOW_4                          = _UxGT("Ροή ") LCD_STR_N4;
  PROGMEM Language_Str MSG_FLOW_5                          = _UxGT("Ροή ") LCD_STR_N5;
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Έλεγχος");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Αυτομ. ρύθμιση θερμοκρασίας");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Ενεργοποιημένο");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Απενεργοποιημένο");
  PROGMEM Language_Str MSG_PID_P                           = _UxGT("PID-P");
  PROGMEM Language_Str MSG_PID_P_E0                        = _UxGT("PID-P ") LCD_STR_E0;
  PROGMEM Language_Str MSG_PID_P_E1                        = _UxGT("PID-P ") LCD_STR_E1;
  PROGMEM Language_Str MSG_PID_P_E2                        = _UxGT("PID-P ") LCD_STR_E2;
  PROGMEM Language_Str MSG_PID_P_E3                        = _UxGT("PID-P ") LCD_STR_E3;
  PROGMEM Language_Str MSG_PID_P_E4                        = _UxGT("PID-P ") LCD_STR_E4;
  PROGMEM Language_Str MSG_PID_P_E5                        = _UxGT("PID-P ") LCD_STR_E5;
  PROGMEM Language_Str MSG_PID_I                           = _UxGT("PID-I");
  PROGMEM Language_Str MSG_PID_I_E0                        = _UxGT("PID-I ") LCD_STR_E0;
  PROGMEM Language_Str MSG_PID_I_E1                        = _UxGT("PID-I ") LCD_STR_E1;
  PROGMEM Language_Str MSG_PID_I_E2                        = _UxGT("PID-I ") LCD_STR_E2;
  PROGMEM Language_Str MSG_PID_I_E3                        = _UxGT("PID-I ") LCD_STR_E3;
  PROGMEM Language_Str MSG_PID_I_E4                        = _UxGT("PID-I ") LCD_STR_E4;
  PROGMEM Language_Str MSG_PID_I_E5                        = _UxGT("PID-I ") LCD_STR_E5;
  PROGMEM Language_Str MSG_PID_D                           = _UxGT("PID-D");
  PROGMEM Language_Str MSG_PID_D_E0                        = _UxGT("PID-D ") LCD_STR_E0;
  PROGMEM Language_Str MSG_PID_D_E1                        = _UxGT("PID-D ") LCD_STR_E1;
  PROGMEM Language_Str MSG_PID_D_E2                        = _UxGT("PID-D ") LCD_STR_E2;
  PROGMEM Language_Str MSG_PID_D_E3                        = _UxGT("PID-D ") LCD_STR_E3;
  PROGMEM Language_Str MSG_PID_D_E4                        = _UxGT("PID-D ") LCD_STR_E4;
  PROGMEM Language_Str MSG_PID_D_E5                        = _UxGT("PID-D ") LCD_STR_E5;
  PROGMEM Language_Str MSG_PID_C                           = _UxGT("PID-C");
  PROGMEM Language_Str MSG_PID_C_E0                        = _UxGT("PID-C ") LCD_STR_E0;
  PROGMEM Language_Str MSG_PID_C_E1                        = _UxGT("PID-C ") LCD_STR_E1;
  PROGMEM Language_Str MSG_PID_C_E2                        = _UxGT("PID-C ") LCD_STR_E2;
  PROGMEM Language_Str MSG_PID_C_E3                        = _UxGT("PID-C ") LCD_STR_E3;
  PROGMEM Language_Str MSG_PID_C_E4                        = _UxGT("PID-C ") LCD_STR_E4;
  PROGMEM Language_Str MSG_PID_C_E5                        = _UxGT("PID-C ") LCD_STR_E5;
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Επιτάχυνση");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Vαντίδραση");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Vαντίδραση ") LCD_STR_C;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Vαντίδραση E");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vμεγ ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vμεγ ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vμεγ ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vμεγ ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_E0                         = _UxGT("Vμεγ ") LCD_STR_E0;
  PROGMEM Language_Str MSG_VMAX_E1                         = _UxGT("Vμεγ ") LCD_STR_E1;
  PROGMEM Language_Str MSG_VMAX_E2                         = _UxGT("Vμεγ ") LCD_STR_E2;
  PROGMEM Language_Str MSG_VMAX_E3                         = _UxGT("Vμεγ ") LCD_STR_E3;
  PROGMEM Language_Str MSG_VMAX_E4                         = _UxGT("Vμεγ ") LCD_STR_E4;
  PROGMEM Language_Str MSG_VMAX_E5                         = _UxGT("Vμεγ ") LCD_STR_E5;
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vελαχ");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vελάχ. μετατόπιση");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Accel");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Aμεγ ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Aμεγ ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Aμεγ ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Aμεγ ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_E0                         = _UxGT("Aμεγ ") LCD_STR_E0;
  PROGMEM Language_Str MSG_AMAX_E1                         = _UxGT("Aμεγ ") LCD_STR_E1;
  PROGMEM Language_Str MSG_AMAX_E2                         = _UxGT("Aμεγ ") LCD_STR_E2;
  PROGMEM Language_Str MSG_AMAX_E3                         = _UxGT("Aμεγ ") LCD_STR_E3;
  PROGMEM Language_Str MSG_AMAX_E4                         = _UxGT("Aμεγ ") LCD_STR_E4;
  PROGMEM Language_Str MSG_AMAX_E5                         = _UxGT("Aμεγ ") LCD_STR_E5;
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Α-ανάσυρση");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Α-μετατόπιση");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Bήματα ανά μμ");
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("Bήματα ") LCD_STR_A _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("Bήματα ") LCD_STR_B _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("Bήματα ") LCD_STR_C _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Bήματα Ε ανά μμ");
  PROGMEM Language_Str MSG_E0_STEPS                        = _UxGT("Bήματα ") LCD_STR_E0 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E1_STEPS                        = _UxGT("Bήματα ") LCD_STR_E1 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E2_STEPS                        = _UxGT("Bήματα ") LCD_STR_E2 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E3_STEPS                        = _UxGT("Bήματα ") LCD_STR_E3 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E4_STEPS                        = _UxGT("Bήματα ") LCD_STR_E4 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_E5_STEPS                        = _UxGT("Bήματα ") LCD_STR_E5 _UxGT(" ανά μμ");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Θερμοκρασία");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Κίνηση");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Νήμα");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("Ε σε μμ3");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Διάμετρος νήματος");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E0                = _UxGT("Διάμετρος νήματος ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E1                = _UxGT("Διάμετρος νήματος ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E2                = _UxGT("Διάμετρος νήματος ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E3                = _UxGT("Διάμετρος νήματος ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E4                = _UxGT("Διάμετρος νήματος ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E5                = _UxGT("Διάμετρος νήματος ") LCD_STR_E5;
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Κοντράστ LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Αποθήκευση");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Φόρτωση");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Επαναφορά ασφαλούς αντιγράφου");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Ανανέωση");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Οθόνη πληροφόρησης");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Προετοιμασία");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Συντονισμός");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Παύση εκτύπωσης");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Συνέχιση εκτύπωσης");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Διακοπή εκτύπωσης");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Εκτύπωση από SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Δεν βρέθηκε SD");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Αναστολή λειτουργίας…");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Αναμονή για χρήστη…");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Διακόπτεται η εκτύπωση");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Καμία κίνηση.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ΤΕΡΜΑΤΙΣΜΟΣ. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ΔΙΑΚΟΠΗ. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Ανάσυρση μμ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Εναλλαγή ανάσυρσης μμ");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Ανάσυρση V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Μεταπήδηση μμ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("S UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet  V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Αυτόματη ανάσυρση");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Αλλαγή νήματος");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E0               = _UxGT("Αλλαγή νήματος ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E1               = _UxGT("Αλλαγή νήματος ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E2               = _UxGT("Αλλαγή νήματος ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E3               = _UxGT("Αλλαγή νήματος ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E4               = _UxGT("Αλλαγή νήματος ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E5               = _UxGT("Αλλαγή νήματος ") LCD_STR_E5;
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Προετοιμασία κάρτας SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Αλλαγή κάρτας SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Διερεύνηση Z εκτός κλίνης");
  PROGMEM Language_Str MSG_YX_UNHOMED                      = _UxGT("Επαναφορά Χ/Υ πριν από Ζ");
  PROGMEM Language_Str MSG_XYZ_UNHOMED                     = _UxGT("Επαναφορά ΧΥΖ πρώτα");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Μετατόπιση Ζ");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Μικρό βήμα Χ");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Μικρό βήμα Υ");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Μικρό βήμα Ζ");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Ματαίωση endstop ");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Ανεπιτυχής θέρμανση");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Λάθος: ΠΛΕΟΝΑΖΟΥΣΑ ΘΕΡΜΟΤΗΤΑ");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ΔΙΑΦΥΓΗ ΘΕΡΜΟΤΗΤΑΣ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Λάθος: ΜΕΓΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Λάθος: ΕΛΑΧΙΣΤΗ ΘΕΡΜΟΤΗΤΑ ΚΛΙΝΗΣ");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Θερμαίνεται…");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Θέρμανση κλίνης…");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Βαθμονόμηση Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Βαθμονόμηση X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Βαθμονόμηση Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Βαθμονόμηση Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Βαθμονόμηση κέντρου");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Εσφαλμένος εκτυπωτής");
}
