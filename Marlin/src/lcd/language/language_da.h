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
 * Danish
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_da {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Danish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" er klar");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Kort isat");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Kort fjernet");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Slå alle steppere fra");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Auto Home"); // G28
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klik når du er klar");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Næste punkt");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Bed level er færdig!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Sæt forsk. af home");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Forsk. er nu aktiv");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Sæt origin");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Forvarm ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Forvarm ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Alle");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Bed");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" conf");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Forvarm $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Forvarm $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Forvarm $ end");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Forvarm $ end ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Forvarm $ Alle");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Forvarm $ Bed");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Forvarm $ conf");
  #endif
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Afkøl");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Slå strøm til");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Slå strøm fra");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extruder");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Flyt akser");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Juster bed");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Juster bed");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Flyt X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Flyt Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Flyt Z");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Flyt %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Flyt 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Flyt 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Flyt 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Hastighed");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Plade Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Dyse");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Dyse ~");

  PROGMEM Language_Str MSG_BED                             = _UxGT("Plade");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Blæser hastighed");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Blæser hastighed ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Kontrol");
  PROGMEM Language_Str MSG_MIN                             = _UxGT(" \002 Min");
  PROGMEM Language_Str MSG_MAX                             = _UxGT(" \002 Max");
  PROGMEM Language_Str MSG_FACTOR                          = _UxGT(" \002 Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Til");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Fra");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Vælg");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Vælg *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-retract");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-rejse");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatur");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Bevægelse");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E i mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Fil. Dia. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD kontrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Gem i EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Hent fra EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Gendan Defaults");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Genopfrisk");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Info skærm");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Forbered");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pause printet");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Forsæt printet");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Stop printet");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Print fra SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Intet SD kort");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Dvale...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Venter på bruger...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Print annulleret");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Ingen bevægelse.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("DRÆBT. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("STOPPET. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Tilbagetræk mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Skift Re.mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Tilbagetræk V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hop mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Skift Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Unretract V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto-Retract");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Skift filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Skift filament *");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Skift SD kort");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Probe udenfor plade");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Selv-Test");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s først");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Opvarmning fejlet");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Fejl: reserve temp");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Temp løber løbsk");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Fejl: Maks temp");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Fejl: Min temp");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("PRINTER STOPPET");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Reset Venligst");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // Kun et bogstav
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // Kun et bogstav
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // Kun et bogstav
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Opvarmer...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Opvarmer plade...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibrering");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibrer X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibrer Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibrer Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibrerings Center");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Om Printer");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Kort Info");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Thermistors");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Ant. Prints");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Færdige");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total print tid");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Længste print");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total Extruderet");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Prints");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Færdige");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Længste");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extruderet");
  #endif

  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Strømfors.");

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Driv Styrke");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Driv %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Driv %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Driv %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Driv %");

  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM Skriv");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Forsæt print");

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Forkert printer");

  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Vent på start", "af filament", "skift"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Vent på", "filament udskyd."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Indsæt filament", "og tryk på knap", "for at fortsætte..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Vent på", "filament indtag"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Vent på at print", "fortsætter"));
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Vent venligst..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Udskyder..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Indsæt og klik"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Indtager..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Fortsætter..."));
  #endif // LCD_HEIGHT < 4
}
