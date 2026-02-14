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
 * Finnish
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

#define DISPLAY_CHARSET_ISO10646_1

namespace LanguageNarrow_fi {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Finnish");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" valmis.");              // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Kortti asetettu");                          // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Kortti poistettu");                         // (MEDIA_TYPE_EN) Removed
  LSTR MSG_MAIN_MENU                      = _UxGT("Palaa");                                    // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Vapauta moottorit");                        // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Aja referenssiin");                         // Auto Home
  LSTR MSG_PREHEAT_1                      = _UxGT("Esilämmitä ") PREHEAT_1_LABEL;              // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Esilämmitä ") PREHEAT_1_LABEL " ~";         // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT("Suutin");  // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT("Suutin ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT(" Kaikki"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT(" Alusta"); // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Esilämm. ") PREHEAT_1_LABEL _UxGT(" konf"); // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Esilämmitä $");                             // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Esilämmitä $ ~");                           // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Esilä. $Suutin");                           // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Esilä. $Suutin ~");                         // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Esilä. $ Kaikki");                          // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Esilä. $ Alusta");                          // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Esilämm. $ konf");                          // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Jäähdytä");                                 // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Virta päälle");                             // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Virta pois");                               // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Pursota");                                  // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Vedä takaisin");                            // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Liikuta akseleita");                        // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Tutki ja tasoita");                         // Probe and Level
  LSTR MSG_MOVE_X                         = _UxGT("Liikuta X");                                // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Liikuta Y");                                // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Liikuta Z");                                // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Liikuta @");                                // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Extruder");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Extruder *");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Liikuta $mm");                              // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Liikuta $in");                              // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Liikuta $") LCD_STR_DEGREE;                 // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Nopeus");                                   // Speed
  LSTR MSG_NOZZLE                         = _UxGT("Suutin");                                   // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("Suutin ~");                                 // Nozzle ~
  LSTR MSG_BED                            = _UxGT("Alusta");                                   // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Tuul. nopeus");                             // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Tuul. nopeus ~");                           // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Virtaus");                                  // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Virtaus ~");                                // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Kontrolli");                                // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");                                 // Autotemp
  LSTR MSG_ACC                            = _UxGT("Kiihtyv");                                  // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("VLiike min");                               // Min Travel Speed
  LSTR MSG_A_RETRACT                      = _UxGT("A-peruuta");                                // Retract Accel
  LSTR MSG_TEMPERATURE                    = _UxGT("Lämpötila");                                // Temperature
  LSTR MSG_MOTION                         = _UxGT("Liike");                                    // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Filament");                                 // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_CONTRAST                       = _UxGT("LCD kontrasti");                            // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Tallenna muistiin");                        // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Lataa muistista");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Palauta oletus");                           // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Päivitä");                  // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Seuraa");                                   // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Valmistele");                               // Prepare
  LSTR MSG_TUNE                           = _UxGT("Säädä");                                    // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Keskeytä tulostus");                        // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Jatka tulostusta");                         // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Pysäytä tulostus");                         // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Korttivalikko");                            // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Ei korttia");                               // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Nukkumassa...");                            // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Odotet. valintaa");                         // Click to Resume...
  LSTR MSG_NO_MOVE                        = _UxGT("Ei liiketta.");                             // No Move.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Vedä mm");                                  // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Va. Vedä mm");                              // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Vedä V");                                   // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Z mm");                                     // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Va. Unretr. mm");                           // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");                              // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("AutoVeto.");                                // Auto-Retract
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Automaatti");                               // Run Autofiles
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Säädä Z askeltamalla");                     // Babystep Probe Z
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibrointi");                        // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibroi X");                               // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibroi Y");                               // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibroi Z");                               // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibroi Center");                          // Calibrate Center
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");             // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");             // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Kerr");            // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Väärä tulostin");                           // INCORRECT PRINTER
}

namespace LanguageWide_fi {
  using namespace LanguageNarrow_fi;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_fi {
  using namespace LanguageWide_fi;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_fi {
  using namespace LanguageTall_fi;
}
