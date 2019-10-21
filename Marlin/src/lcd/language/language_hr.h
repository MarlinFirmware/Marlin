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
 * Croatian (Hrvatski)
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.

namespace Language_hr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Croatian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" spreman.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("SD kartica umetnuta");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("SD kartica uklonjena");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Main");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Auto pokretanje");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Ugasi steppere");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Automatski homing");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Home-aj X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Home-aj Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Home-aj Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Home-aj XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klikni za početak");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Sljedeća točka");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Niveliranje gotovo!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Postavi home offsete");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offsets postavljeni");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Postavi ishodište");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Predgrij ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H0                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_H1                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_H2                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_H3                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_H4                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_H5                    = _UxGT("Predgrij ") PREHEAT_1_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E0                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E1                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E2                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E3                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E4                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E5                = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Dizna ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Sve");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" Bed");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Predgrij ") PREHEAT_1_LABEL _UxGT(" conf");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Predgrij ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H0                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_H1                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_H2                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_H3                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_H4                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_H5                    = _UxGT("Predgrij ") PREHEAT_2_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E0                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E1                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E2                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E3                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E4                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E5                = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Dizna ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Sve");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" Bed");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Predgrij ") PREHEAT_2_LABEL _UxGT(" conf");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Hlađenje");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Uključi napajanje");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Isključi napajanje");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Miči os");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Niveliraj bed");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Niveliraj bed");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Miči X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Miči Y");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Miči %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Miči 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Miči 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Miči 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Brzina");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bed Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Dizna");
  PROGMEM Language_Str MSG_NOZZLE_0                        = _UxGT("Dizna ") LCD_STR_N0;
  PROGMEM Language_Str MSG_NOZZLE_1                        = _UxGT("Dizna ") LCD_STR_N1;
  PROGMEM Language_Str MSG_NOZZLE_2                        = _UxGT("Dizna ") LCD_STR_N2;
  PROGMEM Language_Str MSG_NOZZLE_3                        = _UxGT("Dizna ") LCD_STR_N3;
  PROGMEM Language_Str MSG_NOZZLE_4                        = _UxGT("Dizna ") LCD_STR_N4;
  PROGMEM Language_Str MSG_NOZZLE_5                        = _UxGT("Dizna ") LCD_STR_N5;
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bed");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Brzina ventilatora");
  PROGMEM Language_Str MSG_FAN_SPEED_1                     = _UxGT("Brzina ventilatora 1");
  PROGMEM Language_Str MSG_FAN_SPEED_2                     = _UxGT("Brzina ventilatora 2");
  PROGMEM Language_Str MSG_FAN_SPEED_3                     = _UxGT("Brzina ventilatora 3");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Odaberi");
  PROGMEM Language_Str MSG_SELECT_E0                       = _UxGT("Odaberi ") LCD_STR_E0;
  PROGMEM Language_Str MSG_SELECT_E1                       = _UxGT("Odaberi ") LCD_STR_E1;
  PROGMEM Language_Str MSG_SELECT_E2                       = _UxGT("Odaberi ") LCD_STR_E2;
  PROGMEM Language_Str MSG_SELECT_E3                       = _UxGT("Odaberi ") LCD_STR_E3;
  PROGMEM Language_Str MSG_SELECT_E4                       = _UxGT("Odaberi ") LCD_STR_E4;
  PROGMEM Language_Str MSG_SELECT_E5                       = _UxGT("Odaberi ") LCD_STR_E5;
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperature");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Gibanje");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm3");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E0                = _UxGT("Fil. Dia. ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E1                = _UxGT("Fil. Dia. ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E2                = _UxGT("Fil. Dia. ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E3                = _UxGT("Fil. Dia. ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E4                = _UxGT("Fil. Dia. ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E5                = _UxGT("Fil. Dia. ") LCD_STR_E5;
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Kontrast LCD-a");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Pohrani u memoriju");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Učitaj memoriju");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Učitaj failsafe");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Osvježi");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Info screen");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Pripremi");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pauziraj print");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Nastavi print");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Zaustavi print");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Printaj s SD kartice");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Nema SD kartice");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Čekaj korisnika...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Print otkazan");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ZAUSTAVLJEN. ");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Promijeni filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E0               = _UxGT("Promijeni filament ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E1               = _UxGT("Promijeni filament ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E2               = _UxGT("Promijeni filament ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E3               = _UxGT("Promijeni filament ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E4               = _UxGT("Promijeni filament ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E5               = _UxGT("Promijeni filament ") LCD_STR_E5;
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Init. SD karticu");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Promijeni SD karticu");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Grijanje neuspješno");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Grijanje...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Grijanje Bed-a...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibracija");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibriraj X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibriraj Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibriraj Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibriraj Središte");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("O printeru");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Podaci o printeru");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statistika printera");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Podaci o elektronici");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistori");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extruderi");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokol");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Osvjetljenje");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Neispravan pisač");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Broj printova");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Završeni");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Ukupno printanja");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najduži print");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrudirano ukupno");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Printovi");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Završeni");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Ukupno");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najduži");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrudirano");
  #endif

  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Napajanje");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Nastavi print");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_2_LINE("Čekaj", "filament unload"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_2_LINE("Pričekaj", "filament load"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME          = _UxGT(MSG_1_LINE("Nastavljam..."));

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Čekaj početak", "filamenta", "promijeni"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Umetni filament", "i pritisni tipku", "za nastavak..."));
  #else
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT("Pričekaj...");
    //PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT      = _UxGT(MSG_2_LINE("?", "?"));
  #endif
}
