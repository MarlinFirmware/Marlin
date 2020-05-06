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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Dutch
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace Language_nl {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 1;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Dutch");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" gereed.");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Terug");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Kaart ingestoken");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Kaart verwijderd");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Hoofdmenu");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autostart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Motoren uit");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Debug Menu"); // accepted English terms
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Vooruitgang Test");

  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klik voor begin");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Volgende Plaats");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Bed level kompl.");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Zet home offsets");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("H offset toegep.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Nulpunt instellen");
  PROGMEM Language_Str MSG_PREHEAT_1                       = PREHEAT_1_LABEL _UxGT(" voorverwarmen");
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = PREHEAT_1_LABEL _UxGT(" voorverw. ~");
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = PREHEAT_1_LABEL _UxGT(" voorverw. Einde");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = PREHEAT_1_LABEL _UxGT(" voorverw. Einde ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = PREHEAT_1_LABEL _UxGT(" voorverw. aan");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = PREHEAT_1_LABEL _UxGT(" voorverw. Bed");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = PREHEAT_1_LABEL _UxGT(" verw. conf");
  PROGMEM Language_Str MSG_PREHEAT_2                       = PREHEAT_2_LABEL _UxGT(" voorverwarmen");
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = PREHEAT_2_LABEL _UxGT(" voorverw. ~");
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = PREHEAT_2_LABEL _UxGT(" voorverw. Einde");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = PREHEAT_2_LABEL _UxGT(" voorverw. Einde ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = PREHEAT_2_LABEL _UxGT(" voorverw. aan");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = PREHEAT_2_LABEL _UxGT(" voorverw. Bed");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = PREHEAT_2_LABEL _UxGT(" verw. conf");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Afkoelen");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Stroom aan");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Stroom uit");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrude");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retract");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("As verplaatsen");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Bed Leveling");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Level bed");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Verplaatsen...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Vrij XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Verplaats X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Verplaats Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Verplaats Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extruder *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Verplaats %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Verplaats 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Verplaats 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Verplaats 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Snelheid");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bed Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Nozzle");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Nozzle ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bed");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Fan snelheid");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Fan snelheid ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flow");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flow ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Control");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Aan");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Uit");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Selecteer");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Selecteer *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Versn");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatuur");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Beweging");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Advance K");        // accepted english dutch
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Advance K *"); // accepted english dutch
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Fil. Dia. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD contrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Geheugen opslaan");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Geheugen laden");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Noodstop reset");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Ververs");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Info scherm");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Voorbereiden");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Afstellen");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Print pauzeren");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Print hervatten");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Print stoppen");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Print van SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Geen SD kaart");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Slapen...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Wachten...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Print afgebroken");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Geen beweging.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("Afgebroken. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("Gestopt. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retract mm");  // accepted English term in Dutch
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Ruil Retract mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retract  F");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hop mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Ruil UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet  F");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("AutoRetr.");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Verv. Filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Verv. Filament *");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Init. SD kaart");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Verv. SD Kaart");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z probe uit. bed");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Zelf-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reset BLTouch");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s Eerst");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Z Offset");  // accepted English term in Dutch
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystap X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystap Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystap Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Endstop afbr.");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Voorverw. fout");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Redun. temp fout");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Therm. wegloop");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: Max. temp");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: Min. temp");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Err: Max.tmp bed");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Err: Min.tmp bed");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Home XY Eerst");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("PRINTER GESTOPT");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Reset A.U.B.");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); //  One character only. Keep English standard
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); //  One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); //  One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Voorwarmen...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Bed voorverw...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Calibratie");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibreer X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibreer Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibreer Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibreer Midden");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Calibratie");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Zet Delta Hoogte");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Case licht");

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Onjuiste printer");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Printed Aantal");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Totaal Voltooid");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Totale Printtijd");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Langste Printtijd");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Totaal Extrudeert");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Aantal");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Voltooid");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Printtijd ");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Langste");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrud.");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Temp");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Temp");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");  // accepted English term in Dutch

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Motorstroom");

  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC Opslaan");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Hervat print");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT(" Nozzle: "); // accepted English term
  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    // Up to 3 lines
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Wacht voor start", "filament te", "verwisselen"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Wacht voor", "filament uit", "te laden"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Klik knop om...", "verw. nozzle.")); //nozzle accepted English term
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Nozzle verw.", "Wacht a.u.b."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Laad filament", "en druk knop", "om verder..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Wacht voor", "filament te", "laden"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Wacht voor print", "om verder", "te gaan"));
  #else
    // Up to 2 lines
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_2_LINE("Wacht voor", "start..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Wacht voor", "uitladen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Klik knop om...", "verw. nozzle.")); //nozzle accepted English term
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Verwarmen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_2_LINE("Laad filament", "en druk knop"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Wacht voor", "inladen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Wacht voor", "printing..."));
  #endif
}
