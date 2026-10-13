/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Klingon (tlhIngan Hol) in pIqaD script
 *
 * pIqaD is encoded in the ConScript Unicode Registry Private Use Area (U+F8D0-U+F8FF),
 * so it can only be displayed by TFT_COLOR_UI, which includes a Klingon glyph font.
 * Each string is followed by its Latin transliteration and the English original.
 * Strings not translated here fall back to English.
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

namespace LanguageNarrow_tlh {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 3;
  LSTR LANGUAGE                           = _UxGT(" ");                                           // tlhIngan Hol | Klingon

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" .");           // (MACHINE_NAME_SUBST) ghuS. | (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("  ");                     // De' Hal 'elta' | (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("  ");                  // De' Hal teqlu'ta' | (MEDIA_TYPE_EN) Removed
  LSTR MSG_MEDIA_NOT_INSERTED             = _UxGT("  ");                  // De' Hal tu'lu'be' | No Media
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("");                             // veHmey | Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("");                        // HIDjolev'a' | Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT(" ");                      // QuQmey chu'Ha' | Disable Steppers
  LSTR MSG_HOMING                         = _UxGT(" ");                      // juHDaq cheghtaH | Homing
  LSTR MSG_AUTO_HOME                      = _UxGT(" ");                         // juHDaq chegh | Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("X  ");                       // X juHDaq chegh | Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Y  ");                       // Y juHDaq chegh | Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Z  ");                       // Z juHDaq chegh | Home Z
  LSTR MSG_HOME_ALL                       = _UxGT("  ");                     // Hoch juHDaq chegh | Home All
  LSTR MSG_LEVEL_BED                      = _UxGT(" ");                      // QongDaq beQmoH | Level Bed
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("  ");                     // nuD 'ej beQmoH | Probe and Level
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT(" ");                            // ngab jen | Fade Height
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Z  ");                       // Z nuDwI' chu' | Deploy Z-Probe
  LSTR MSG_MANUAL_STOW                    = _UxGT("Z  ");                       // Z nuDwI' pol | Stow Z-Probe
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT(" X ");                       // nuDwI' X chuq | Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT(" Y ");                       // nuDwI' Y chuq | Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT(" Z ");                       // nuDwI' Z chuq | Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT(" @ ");                       // nuDwI' @ chuq | Probe @ Offset
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("!");                            // Qapla'! | Leveling Done!
  LSTR MSG_PREHEAT_1                      = _UxGT(" ") PREHEAT_1_LABEL;            // tujmoH (PREHEAT_1_LABEL) | Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT(" ") PREHEAT_1_LABEL _UxGT(" ~"); // tujmoH (PREHEAT_1_LABEL) ~ | Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT(" ") PREHEAT_1_LABEL _UxGT(" "); // tujmoH (PREHEAT_1_LABEL) chuyDaH | Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT(" ") PREHEAT_1_LABEL _UxGT("  ~"); // tujmoH (PREHEAT_1_LABEL) chuyDaH ~ | Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT(" ") PREHEAT_1_LABEL _UxGT(" "); // tujmoH (PREHEAT_1_LABEL) Hoch | Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT(" ") PREHEAT_1_LABEL _UxGT(" "); // tujmoH (PREHEAT_1_LABEL) QongDaq | Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT(" ") PREHEAT_1_LABEL _UxGT(" "); // tujmoH (PREHEAT_1_LABEL) SeH | Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT(" $");                           // tujmoH $ | Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT(" $ ~");                         // tujmoH $ ~ | Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT(" $ ");                    // tujmoH $ chuyDaH | Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT(" $  ~");                  // tujmoH $ chuyDaH ~ | Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT(" $ ");                       // tujmoH $ Hoch | Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT(" $ ");                    // tujmoH $ QongDaq | Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT(" $ ");                       // tujmoH $ SeH | Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("");                             // bIrmoH | Cooldown
  LSTR MSG_EXTRUDE                        = _UxGT("");                                // nob | Extrude
  LSTR MSG_RETRACT                        = _UxGT("");                                // tlhap | Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT(" ");                            // tlhegh vIH | Move Axis
  LSTR MSG_MOVE_N                         = _UxGT("@ ");                              // @ vIH | Move @
  LSTR MSG_MOVE_E                         = _UxGT(" ");                         // chuyDaH vIH | Move Extruder
  LSTR MSG_MOVE_N_MM                      = _UxGT("$mm ");                            // $mm vIH | Move $mm
  LSTR MSG_SPEED                          = _UxGT("");                                 // Do | Speed
  LSTR MSG_NOZZLE                         = _UxGT("");                             // chuyDaH | Nozzle
  LSTR MSG_BED                            = _UxGT("");                             // QongDaq | Bed
  LSTR MSG_FAN_SPEED                      = _UxGT(" ");                             // SuS Do | Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT(" ~ ");                           // SuS ~ Do | Fan ~ Speed
  LSTR MSG_TEMPERATURE                    = _UxGT("");                                // tuj | Temperature
  LSTR MSG_MOTION                         = _UxGT("");                                // vIH | Motion
  LSTR MSG_FILAMENT                       = _UxGT("");                               // SIrgh | Filament
  LSTR MSG_STORE_EEPROM                   = _UxGT(" ");                      // qawHaqDaq pol | Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT(" ");                         // qawHaq laD | Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT(" ");                         // motlhDaq chegh | Restore Defaults
  LSTR MSG_SETTINGS_STORED                = _UxGT(" ");                         // SeH polta' | Settings Stored
  LSTR MSG_INFO_SCREEN                    = _UxGT(" ");                          // De' HaSta | Info Screen
  LSTR MSG_PREPARE                        = _UxGT("");                             // ghuSmoH | Prepare
  LSTR MSG_TUNE                           = _UxGT("");                                // choH | Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT(" ");                         // chenmoH yev | Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT(" ");                         // chenmoH taH | Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT(" ");                         // chenmoH mev | Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("  ");                  // De' Halvo' chenmoH | (MEDIA_TYPE_EN) Print
  LSTR MSG_MEDIA_MENU_SD                  = _UxGT("SD  ");                      // SD Halvo' wIv | Select from SD
  LSTR MSG_MEDIA_MENU_USB                 = _UxGT("USB  ");                     // USB Halvo' wIv | Select from USB
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("...");                          // mevtaH... | Aborting...
  LSTR MSG_USERWAIT                       = _UxGT("  ...");                // taHmeH leQ yIchu'... | Click to Resume...
  LSTR MSG_PRINTING                       = _UxGT("...");                       // chenmoHtaH... | Printing...
  LSTR MSG_PRINT_PAUSED                   = _UxGT(" ");                      // chenmoH yevta' | Print Paused
  LSTR MSG_PRINT_ABORTED                  = _UxGT(" ");                   // chenmoH qIllu'ta' | Print Aborted
  LSTR MSG_PRINT_DONE                     = _UxGT(" ");                         // chenmoH rIn | Print Done
  LSTR MSG_KILLED                         = _UxGT(". ");                        // HoHlu'ta'.  | KILLED. 
  LSTR MSG_STOPPED                        = _UxGT(". ");                           // mevta'.  | STOPPED. 
  LSTR MSG_PRINTER_KILLED                 = _UxGT(" !");                     // QuQ HoHlu'ta'! | Printer killed!
  LSTR MSG_HALTED                         = _UxGT(" ");                         // QuQ mevta' | PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("");                           // yItaghqa' | Please Reset
  LSTR MSG_FILAMENTCHANGE                 = _UxGT(" ");                           // SIrgh choH | Change Filament
  LSTR MSG_FILAMENTLOAD                   = _UxGT(" ");                        // SIrgh 'elmoH | Load Filament
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT(" ");                           // SIrgh teq | Unload Filament
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT(" ");                      // tujmoH Qapbe' | Heating Failed
  LSTR MSG_ERR_MAXTEMP                    = _UxGT(": MAXTEMP");                       // Qagh: MAXTEMP | Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT(": MINTEMP");                       // Qagh: MINTEMP | Err: MINTEMP
  LSTR MSG_HEATING                        = _UxGT("...");                       // tujmoHtaH... | Heating...
  LSTR MSG_BED_HEATING                    = _UxGT(" ...");                // QongDaq tujmoHtaH... | Bed Heating...
  LSTR MSG_COOLING                        = _UxGT("...");                       // bIrmoHtaH... | Cooling...
  LSTR MSG_BED_COOLING                    = _UxGT(" ...");                // QongDaq bIrmoHtaH... | Bed Cooling...
  LSTR MSG_INFO_MENU                      = _UxGT(" ");                            // QuQ De' | About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT(" ");                            // QuQ De' | Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("");                             // mI'mey | Stats
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT(" ");                         // chenmoH mI' | Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("");                                // rIn | Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT(" ");                         // chenmoH poH | Total
  LSTR MSG_YES                            = _UxGT("");                              // HIja' | YES
  LSTR MSG_NO                             = _UxGT("");                              // ghobe' | NO
  LSTR MSG_BACK                           = _UxGT("");                                // chegh | Back
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT(" ");                            // SeH Qatlh | Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("");                                // SeH | Configuration
  LSTR MSG_LIGHTS                         = _UxGT("");                       // wovmoHwI'mey | Lights
  LSTR MSG_CASE_LIGHT                     = _UxGT("");                          // wovmoHwI' | Case Light
  LSTR MSG_CANCEL_OBJECT                  = _UxGT(" ");                            // Doch qIl | Cancel Obj
  LSTR MSG_BUTTON_NEXT                    = _UxGT("");                                // veb | Next
  LSTR MSG_BUTTON_STOP                    = _UxGT("");                                // mev | Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("");                             // chenmoH | Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("");                             // taghqa' | Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("");                                // qIl | Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("");                                // rIn | Done
  LSTR MSG_BUTTON_BACK                    = _UxGT("");                                // chegh | Back
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("");                                // ghoS | Proceed
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("");                             // buSHa' | Ignore
  LSTR MSG_BUTTON_CONFIRM                 = _UxGT("");                              // HIja' | Confirm
  LSTR MSG_BUTTON_CONTINUE                = _UxGT("");                                // taH | Continue
  LSTR MSG_BUTTON_INFO                    = _UxGT("");                                // De' | Info
  LSTR MSG_BUTTON_LEVEL                   = _UxGT("");                             // beQmoH | Level
  LSTR MSG_BUTTON_PAUSE                   = _UxGT("");                                // yev | Pause
  LSTR MSG_BUTTON_RESUME                  = _UxGT("");                                // taH | Resume
  LSTR MSG_BUTTON_ADVANCED                = _UxGT("");                                // Qatlh | Advanced
  LSTR MSG_BUTTON_SAVE                    = _UxGT("");                                // pol | Save
  LSTR MSG_BUTTON_PURGE                   = _UxGT("");                            // Say'moH | Purge
  LSTR MSG_UI_LANGUAGE                    = _UxGT("");                                // Hol | UI Language
}

namespace LanguageWide_tlh {
  using namespace LanguageNarrow_tlh;
}

namespace LanguageTall_tlh {
  using namespace LanguageWide_tlh;
}

namespace Language_tlh {
  using namespace LanguageTall_tlh;
}
