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
 * Indonesian
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
 * Imported from out-csv/language_id.csv on 2026-02-27 at 14:31:51
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace LanguageNarrow_id {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Bahasa Indonesia");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" siap.");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Kartu terpasang");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Kartu dilepas");
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstop");
  LSTR MSG_MAIN_MENU                      = _UxGT("Menu Utama");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Matikan motor");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Homing XYZ");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klik untuk mulai");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Titik berikutnya");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Perataan selesai!");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Setel offset home");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offset diterapkan");
  LSTR MSG_PREHEAT_1                      = _UxGT("Panaskan ") PREHEAT_1_LABEL;
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Panaskan ") PREHEAT_1_LABEL _UxGT(" semua");
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Panaskan ") PREHEAT_1_LABEL _UxGT(" bed");
  LSTR MSG_COOLDOWN                       = _UxGT("Dinginkan");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Hidupkan daya");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Matikan daya");
  LSTR MSG_EXTRUDE                        = _UxGT("Ekstrusi");
  LSTR MSG_RETRACT                        = _UxGT("Tarik balik");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Gerak sumbu");
  LSTR MSG_LEVEL_BED                      = _UxGT("Ratakan bed");
  LSTR MSG_MOVE_X                         = _UxGT("Gerak X");
  LSTR MSG_MOVE_Y                         = _UxGT("Gerak Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Gerak Z");
  LSTR MSG_MOVE_E                         = _UxGT("Ekstruder");
  LSTR MSG_FAN_SPEED                      = _UxGT("Kecep. kipas");
  LSTR MSG_FLOW                           = _UxGT("Aliran");
  LSTR MSG_CONTROL                        = _UxGT("Kontrol");
  LSTR MSG_TEMPERATURE                    = _UxGT("Suhu");
  LSTR MSG_STOP_PRINT                     = _UxGT("Hentikan cetak");
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu debug");
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Tes bar kemajuan");
  LSTR MSG_YES                            = _UxGT("Ya");
  LSTR MSG_NO                             = _UxGT("Tidak");
  LSTR MSG_BACK                           = _UxGT("Kembali");
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Kesalahan baca media");
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Endstop lunak");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Pengaturan lanjut");
  LSTR MSG_CONFIGURATION                  = _UxGT("Konfigurasi");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Lanjut");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Mulai");
  LSTR MSG_HOME_ALL                       = _UxGT("Home semua");
  LSTR MSG_PREHEAT_2                      = _UxGT("Panaskan ") PREHEAT_2_LABEL;
  LSTR MSG_PREHEAT                        = _UxGT("Panaskan");
}

namespace LanguageWide_id {
  using namespace LanguageNarrow_id;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_id {
  using namespace LanguageWide_id;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_id {
  using namespace LanguageTall_id;
}
