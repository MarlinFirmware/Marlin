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
 * Finnish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_fi {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Finnish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" valmis.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Kortti asetettu");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Kortti poistettu");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Palaa");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Automaatti");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Vapauta moottorit");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Aja referenssiin");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Aseta origo");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Esilämmitä ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Esilämmitä ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT("Suutin");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT("Suutin ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT(" Kaikki");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Esilä. ") PREHEAT_1_LABEL _UxGT(" Alusta");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Esilämm. ") PREHEAT_1_LABEL _UxGT(" konf");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Esilämmitä ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Esilämmitä ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Esilä. ") PREHEAT_2_LABEL _UxGT("Suutin");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Esilä. ") PREHEAT_2_LABEL _UxGT("Suutin ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Esilä. ") PREHEAT_2_LABEL _UxGT(" Kaikki");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Esilä. ") PREHEAT_2_LABEL _UxGT(" Alusta");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Esilämm. ") PREHEAT_2_LABEL _UxGT(" konf");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Jäähdytä");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Virta päälle");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Virta pois");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Pursota");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Vedä takaisin");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Liikuta akseleita");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Liikuta X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Liikuta Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Liikuta Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extruder *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Liikuta %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Liikuta 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Liikuta 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Liikuta 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Nopeus");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Suutin");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Suutin ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Alusta");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Tuul. nopeus");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Tuul. nopeus =");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Virtaus");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Virtaus ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Kontrolli");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Kerr");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Kiihtyv");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VLiike min");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-peruuta");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Lämpötila");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Liike");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm³");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD kontrasti");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Tallenna muistiin");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Lataa muistista");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Palauta oletus");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Päivitä");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Seuraa");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Valmistele");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Säädä");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Keskeytä tulostus");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Jatka tulostusta");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Pysäytä tulostus");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Korttivalikko");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Ei korttia");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Nukkumassa...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Odotet. valintaa");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Ei liiketta.");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Vedä mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Va. Vedä mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Vedä V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Z mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Va. UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet  V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("AutoVeto.");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibrointi");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibroi X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibroi Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibroi Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibroi Center");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Väärä tulostin");
}
