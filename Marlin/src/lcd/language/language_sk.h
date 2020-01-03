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
 * Slovak
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 * Translated by Michal Holeš, Farma MaM
 * http://www.facebook.com/farmamam
 *
 */
#define DISPLAY_CHARSET_ISO10646_SK

namespace Language_sk {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Slovak");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" pripravená.");
  PROGMEM Language_Str MSG_YES                             = _UxGT("ÁNO");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NIE");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Naspäť");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Ruším...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Karta vložená");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Karta vybraná");
  PROGMEM Language_Str MSG_MEDIA_RELEASED                  = _UxGT("Karta odpojená");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Čakám na kartu");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Chyba čítania karty");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB zaria. odstrán.");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Chyba spúšťania USB");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstopy"); // max 8 znakov
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Soft. endstopy");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Hlavná ponuka");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Pokročilé nastav.");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfigurácia");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Auto-štart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Uvolniť motory");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Ponuka ladenia");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Test uk. priebehu");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Domovská pozícia");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Domov os X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Domov os Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Domov os Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto-zarovn. Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Parkovanie XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Kliknutím spusťte");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Ďalší bod");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Meranie hotové!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Výška rovnania");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Nastaviť ofsety");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Ofsety nastavené");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Nastaviť začiatok");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Zahriať ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Zahriať ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" hotend");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" hotend ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" všetko");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" podlož");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" nast.");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Zahriať ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Zahriať ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Zahriať ") PREHEAT_2_LABEL _UxGT(" hotend");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Zahriať ") PREHEAT_2_LABEL _UxGT(" hotend ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Zahriať ") PREHEAT_2_LABEL _UxGT(" všetko");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Zahriať ") PREHEAT_2_LABEL _UxGT(" podlož");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Zahriať ") PREHEAT_2_LABEL _UxGT(" nast.");
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Vlastná teplota");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Schladiť");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Nastavenie lasera");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Vypnúť laser");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Zapnúť laser");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Výkon lasera");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Nastavenie vretena");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Vypnúť vreteno");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Zapnúť vreteno");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Výkon vretena");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Spätný chod");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Zapnúť napájanie");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Vypnúť napájanie");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Vytlačiť (extr.)");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Vytiahnuť (retr.)");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Posunúť osy");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Vyrovnanie podložky");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Vyrovnať podložku");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Vyrovnať rohy");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Ďalší roh");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Editor sieťe bodov");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Upraviť sieť bodov");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Koniec úprav siete");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Skúšam bod");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Hodnota Z");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Vlastné príkazy");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Test sondy");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Bod");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Odchýlka");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX režim");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Ofset nástrojov");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-parkovanie");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplikácia");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Zrkadlená kópia");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Plná kontrola");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2. tryska X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2. tryska Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2. tryska Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Vykonávam G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Nástroje UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("UBL rovnanie");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Vyrovnávam bod");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Manuálna sieť bodov");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Položte a zmerajte");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Zmerajte");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Odstráňte a zmerajte");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Presun na ďalší");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Aktivovať UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Deaktivovať UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Teplota podložky");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Teplota podložky");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Teplota hotendu");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Teplota hotendu");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Úprava siete bodov");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Upraviť vlastnú sieť");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Doladiť sieť bodov");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Koniec úprav siete");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Vlastná sieť");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Vytvoriť sieť");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("Sieť bodov ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("Sieť bodov ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Studená sieť bodov");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Upraviť výšku siete");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Výška");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Skontrolovať sieť");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("Kontrola siete ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("Kontrola siete ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Kontrola vlast.siete");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 ohrev podlž.");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 ohrev trysky");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Ručné čistenie...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Čistenie pevn. dĺž.");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Čistenie dokončené");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 zrušený");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Opúšťam G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Pokračovať v sieti");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Sieťové rovnanie");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-bodové rovnanie");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Mriežkové rovnanie");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Vyrovnať podložku");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Postranné body");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Typ siete bodov");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Exportovať sieť");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Exportovať do PC");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Exportovať do CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Záloha do PC");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Info. o UBL do PC");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Hustota mriežky");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Ručné vyplnenie");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Chytré vyplnenie");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Vyplniť mriežku");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Zrušiť všetko");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Zrušiť najbližší");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Upraviť všetky");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Upraviť najbližší");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Úložisko sietí");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Pamäťový slot");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Načítať sieť bodov");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Uložiť sieť bodov");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Sieť %i načítaná");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Sieť %i uložená");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Nedostatok miesta");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Chyba: Ukladanie UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Chyba: Obnovenie UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Ofset: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Koniec kompenz. Z");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Postupné UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Studená sieť bodov");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Chytré vyplnenie");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Skontrolovať sieť");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Upraviť všetky");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Skontrolovať sieť");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Upraviť všetky");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Uložiť sieť bodov");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Nastavenie LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Svetlo");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Prednastavené farby");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Červená");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Oranžová");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Žltá");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Zelená");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Modrá");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Fialová");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Biela");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Obnoviť nastavenie");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Vlastná farba");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Inten. červenej");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Inten. zelenej");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Inten. modrej");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Inten. bielej");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Jas");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Posúvam...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Uvolniť XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Posunúť X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Posunúť Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Posunúť Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extrudér");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extrudér *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Hotend je studený");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Posunúť o %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Posunúť o 0,1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Posunúť o 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Posunúť o 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Rýchlosť");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Výška podl.");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Tryska");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Tryska ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Podložka");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Komora");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Rýchlosť vent.");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Rýchlosť vent. =");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Rýchlosť ex. vent.");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Rýchlosť ex. vent. =");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Prietok");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Prietok ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Ovládanie");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fakt");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Auto-teplota");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Zap");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Vyp");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID kalibrácia");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID kalibrácia *");
  PROGMEM Language_Str MSG_PID_P                           = _UxGT("PID-P");
  PROGMEM Language_Str MSG_PID_P_E                         = _UxGT("PID-P *");
  PROGMEM Language_Str MSG_PID_I                           = _UxGT("PID-I");
  PROGMEM Language_Str MSG_PID_I_E                         = _UxGT("PID-I *");
  PROGMEM Language_Str MSG_PID_D                           = _UxGT("PID-D");
  PROGMEM Language_Str MSG_PID_D_E                         = _UxGT("PID-D *");
  PROGMEM Language_Str MSG_PID_C                           = _UxGT("PID-C");
  PROGMEM Language_Str MSG_PID_C_E                         = _UxGT("PID-C *");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Vybrať");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Vybrať *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Zrýchlenie");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Skok");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-skok");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-skok");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-skok");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-skok");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Junction Dev");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Rýchlosť");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vmax ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vmax ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vmax ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vmax ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vmax *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vmin");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VPrej Min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Akcelerácia");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Amax ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Amax ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Amax ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Amax ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Amax *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-retrakt");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-prejazd");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Kroky/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("krokov/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("krokov/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("krokov/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Ekrokov/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*krokov/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Teplota");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Pohyb");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E na mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Priem. fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Priem. fil. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Vysunúť mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Zaviesť mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K pre posun");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("K pre posun *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Kontrast LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Uložiť nastavenie");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Načítať nastavenie");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Obnoviť nastavenie");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Inicializ. EEPROM");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Aktualizovať z SD");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Reštart. tlačiar.");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Obnoviť");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Info. obrazovka");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Príprava tlače");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Doladenie tlače");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Spustiť tlač");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Ďalší");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Inicial.");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Zastaviť");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Tlačiť");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Vynulovať");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Zrušiť");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Hotovo");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Naspäť");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Pokračovať");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pozastaviť tlač");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Obnoviť tlač");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Zastaviť tlač");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Tlačím objekt");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Zrušiť objekt");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Zrušiť objekt =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Obnova po výp. nap.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Tlačiť z SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Žiadna SD karta");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Spím...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Pokrač. kliknutím...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Tlač pozastavená");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Tlačím...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Tlač zrušená");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Žiadny pohyb.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("PRERUŠENÉ. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ZASTAVENÉ. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retrakt mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Výmena Re.mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retraktovať  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Zdvih Z mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("S UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("AutoRetr.");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Dĺžka výmeny");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Dĺžka vytlačenia");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Výmena nástroja");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Zdvihnúť Z");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPD          = _UxGT("Primárna rýchl.");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPD        = _UxGT("Rýchl. retrakcie");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Záložná tryska");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Vymeniť filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Vymeniť filament *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Zaviesť filament");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Zaviesť filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Vysunúť filament");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Vysunúť filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Vysunúť všetko");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Načítať SD kartu");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Vymeniť SD kartu");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Odpojiť SD kartu");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z mimo podl.");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Faktor skosenia");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Cmd: Self-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Cmd: Reset");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Cmd: Zasunúť");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Cmd: Vysunúť");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("Cmd: Režim SW");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("Cmd: Režim 5V");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("Cmd: Režim OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Cmd: Ulož. režim");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Prepnúť do 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Prepnúť do OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Zobraziť režim");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("POZOR: Zlé nastav. môže spôsobiť poškoden. Pokračovať?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Inicializ. TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Test ofsetu Z");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Uložiť");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Zasunúť TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Zasunúť sondu Z");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Vysunúť sondu Z");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Najskôr os %s%s%s domov");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Ofset sondy Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Celkom");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Zastavenie Endstop");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Chyba ohrevu");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Chyba ohrevu podl.");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_CHAMBER      = _UxGT("Chyba ohrevu komory");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Chyba: REDUND. TEP.");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("TEPLOTNÝ SKOK");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("TEPLOTNÝ SKOK PODL.");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("TEPLOTNÝ SKOK KOMO.");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Chyba: MAXTEMP");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Chyba: MINTEMP");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Chyba: MAXTEMP PODL.");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Chyba: MINTEMP PODL.");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("Chyba: MAXTEMP KOMO.");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("Chyba: MINTEMP KOMO.");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Najskôr os XY domov");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("TLAČIAREŇ ZASTAVENÁ");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Reštartuje ju");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d");
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h");
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m");
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Ohrev...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Ochladzovanie...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Ohrev podložky...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Ochladz. podložky...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Ohrev komory...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Ochladz. komory...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta kalibrácia");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibrovať X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibrovať Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibrovať Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibrovať stred");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta nastavenia");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto-kalibrácia");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Nast. výšku delty");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Ofset sondy Z");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag. rameno");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Výška");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Polomer");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("O tlačiarni");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info. o tlačiarni");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-bodové rovnanie");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Lineárne rovnanie");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Bilineárne rovnanie");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("UBL rovnanie");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Mriežkové rovnanie");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Štatistika");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info. o doske");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistory");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrudéry");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Rýchlosť");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokol");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Tepl. ochrana: VYP");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Tepl. ochrana: ZAP");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Osvetlenie");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Jas svetla");
  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Nesprávna tlačiareň");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Počet tlačí");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Dokončené");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Celkový čas");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najdlhšia tlač");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Celkom vytlačené");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Tlače");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Hotovo");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Čas");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Najdlhšia");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Vytlačené");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Teplota min");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Teplota max");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Nap. zdroj");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Budenie motorov");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Motor %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Motor %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Motor %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Motor %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("CHYBA KOMUNIKÁ. TMC");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Uložiť do EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("VÝMENA FILAMENTU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("PAUZA TLAČE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("ZAVEDENIE FILAMENTU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("VYSUNUTIE FILAMENTU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("MOŽNOSTI POKRAČ.:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Vytlačiť viacej");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Obnoviť tlač");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Tryska: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Senzor filamentu");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Vzd. mm fil. senz.");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Parkovanie zlyhalo");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Kalibrácia zlyhala");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Príliš studený");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("VYBERTE FILAMENT");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU2");
  PROGMEM Language_Str MSG_MMU2_WRONG_FIRMWARE             = _UxGT("Aktualizujte FW MMU!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU potrebuje zásah.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Obnoviť tlač");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Obnovovanie...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Zaviesť filament");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Zaviesť všetky");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Zaviesť po trysku");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Vysunúť filament");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Vysunúť filament ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Vyňať filament");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Zavádzanie fil. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Vysúvanie fil. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Vysúvanie fil. ...");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Všetky");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filament ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Reštartovať MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Reštart MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Odstráňte, kliknite");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Mix");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Zložka ~");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Mixér");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradient");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Plný gradient");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Prepnúť mix");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Cyklický mix");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Gradientný mix");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Otočiť gradient");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Aktívny V-tool");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("Počiat. V-tool");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("Konečn. V-tool");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("Alias V-tool");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Vynulovať V-tools");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Uložiť V-tool Mix");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-tools vynulované");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Počiat.Z:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("Konečn.Z:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Hry");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Nájazdníci");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Had");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Bludisko");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Stlačte tlačidlo", "pre obnovu tlače"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_INIT              = _UxGT(MSG_1_LINE("Parkovanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Čakajte prosím", "na spustenie", "výmeny filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Vložte filament", "a stlačte tlačidlo", "pre pokračovanie"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Stlačte tlačidlo", "pre ohrev trysky"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Ohrev trysky", "Čakajte prosím..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Čakajte prosím", "na vysunutie", "filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Čakajte prosím", "na zavedenie", "filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_3_LINE("Čakajte prosím", "na vytlačenie", "filamentu"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_3_LINE("Stlačte tlačidlo", "pre dokončenie", "vytláčania filam."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Čakajte prosím na", "obnovenie tlače..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Kliknite pre pokr."));
    PROGMEM Language_Str MSG_PAUSE_PRINT_INIT              = _UxGT(MSG_1_LINE("Parkovanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Čakajte prosím..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Vložte a kliknite"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Kliknite pre ohrev"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Ohrev..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Vysúvanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Zavádzanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Vytlačovanie..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Klik. pre dokonč."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Pokračovanie..."));
  #endif
  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Ovládače TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Prúd ovládača");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hybridný prah");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Bezsenzor. domov");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Režim krokovania");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop zapnutý");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Vynulovať");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT("za:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Kompenz. vôle");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Korekcia");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Vyhladzovanie");
}
