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
 * Hinglish (Hindi-Latin)
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

/**
 * Imported from out-csv/language_hg.csv on 2026-02-27 at 14:31:47
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace LanguageNarrow_hg {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Hinglish");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" taiyaar.");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Card lagi");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Card nikal gayi");
  LSTR MSG_MAIN_MENU                      = _UxGT("Mukhya Menu");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Motor band");
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto Ghar");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Ghar X");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Ghar Y");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Ghar Z");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Start ke liye click");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Agla point");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Leveling ho gaya!");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Home offset set karo");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offsets lage");
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Preheat ") PREHEAT_1_LABEL _UxGT(" Sab");
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Preheat ") PREHEAT_1_LABEL _UxGT(" Bed");
  LSTR MSG_COOLDOWN                       = _UxGT("Thanda karo");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Power chalu");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Power band");
  LSTR MSG_EXTRUDE                        = _UxGT("Nikalna");
  LSTR MSG_RETRACT                        = _UxGT("Wapas lena");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Axis hilaao");
  LSTR MSG_LEVEL_BED                      = _UxGT("Bed Satao");
  LSTR MSG_MOVE_X                         = _UxGT("X hilaao");
  LSTR MSG_MOVE_Y                         = _UxGT("Y hilaao");
  LSTR MSG_MOVE_Z                         = _UxGT("Z hilaao");
  LSTR MSG_MOVE_E                         = _UxGT("Extruder");
  LSTR MSG_FAN_SPEED                      = _UxGT("Pankha gati");
  LSTR MSG_CONTROL                        = _UxGT("Niyantran");
  LSTR MSG_TEMPERATURE                    = _UxGT("Tapmaan");
  LSTR MSG_STOP_PRINT                     = _UxGT("Print roko");
  LSTR MSG_YES                            = _UxGT("Haan");
  LSTR MSG_NO                             = _UxGT("Nahi");
  LSTR MSG_BACK                           = _UxGT("Peeche");
  LSTR MSG_CONFIGURATION                  = _UxGT("Settings");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Aage");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Shuru");
  LSTR MSG_HOME_ALL                       = _UxGT("Sab Ghar");
}

namespace LanguageWide_hg {
  using namespace LanguageNarrow_hg;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_hg {
  using namespace LanguageWide_hg;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_hg {
  using namespace LanguageTall_hg;
}
