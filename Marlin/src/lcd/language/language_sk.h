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
 * Slovak
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Translated by Michal Holeš, Farma MaM
 * https://www.facebook.com/farmamam
 *
 * Substitutions are applied for the following characters when used
 * in menu items that call lcd_put_u8str_ind_P with an index:
 *
 *   $ displays an inserted C-string
 *   = displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */
#define DISPLAY_CHARSET_ISO10646_SK

namespace Language_sk {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Slovenčina");

  LSTR WELCOME_MSG                        = MACHINE_NAME _UxGT(" pripravená.");
  LSTR MSG_YES                            = _UxGT("ÁNO");
  LSTR MSG_NO                             = _UxGT("NIE");
  LSTR MSG_BACK                           = _UxGT("Naspäť");
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Ruším...");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Karta vložená");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Karta vybraná");
  LSTR MSG_MEDIA_WAITING                  = _UxGT("Čakám na kartu");
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Inicial. SD zlyhala");
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Chyba čítania karty");
  LSTR MSG_MEDIA_USB_REMOVED              = _UxGT("USB zaria. odstrán.");
  LSTR MSG_MEDIA_USB_FAILED               = _UxGT("Chyba spúšťania USB");
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Preteč. podprogramu");
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Endstopy"); // max 8 znakov
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Soft. endstopy");
  LSTR MSG_MAIN                           = _UxGT("Hlavná ponuka");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Pokročilé nastav.");
  LSTR MSG_CONFIGURATION                  = _UxGT("Konfigurácia");
  LSTR MSG_RUN_AUTO_FILES                 = _UxGT("Auto-štart");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Uvolniť motory");
  LSTR MSG_DEBUG_MENU                     = _UxGT("Ponuka ladenia");
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test uk. priebehu");
  LSTR MSG_HOMING                         = _UxGT("Parkovanie");
  LSTR MSG_AUTO_HOME                      = _UxGT("Domovská pozícia");
  LSTR MSG_AUTO_HOME_A                    = _UxGT("Domov os @");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Domov os X");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Domov os Y");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Domov os Z");
  LSTR MSG_FILAMENT_SET                   = _UxGT("Nastav. filamentu");
  LSTR MSG_FILAMENT_MAN                   = _UxGT("Správa filamentu");
  LSTR MSG_LEVBED_FL                      = _UxGT("Ľavý predný");
  LSTR MSG_LEVBED_FR                      = _UxGT("Pravý predný");
  LSTR MSG_LEVBED_C                       = _UxGT("Stred");
  LSTR MSG_LEVBED_BL                      = _UxGT("Ľavý zadný");
  LSTR MSG_LEVBED_BR                      = _UxGT("Pravý zadný");
  LSTR MSG_MANUAL_MESH                    = _UxGT("Ručná mriežka");
  LSTR MSG_AUTO_MESH                      = _UxGT("Automat. mriežka");
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto-zarovn. Z");
  LSTR MSG_ITERATION                      = _UxGT("Iterácia G34: %i");
  LSTR MSG_DECREASING_ACCURACY            = _UxGT("Klesajúca presnosť!");
  LSTR MSG_ACCURACY_ACHIEVED              = _UxGT("Dosiahnutá presnosť");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Parkovanie XYZ");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Kliknutím začnete");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Ďalší bod");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Vyrovnanie hotové!");
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Výška rovnania");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Nastav. dom. ofsety");
  LSTR MSG_HOME_OFFSET_X                  = _UxGT("X Ofset");
  LSTR MSG_HOME_OFFSET_Y                  = _UxGT("Y Ofset");
  LSTR MSG_HOME_OFFSET_Z                  = _UxGT("Z Ofset");
  LSTR MSG_HOME_OFFSET_I                  = STR_I _UxGT(" Ofset");
  LSTR MSG_HOME_OFFSET_J                  = STR_J _UxGT(" Ofset");
  LSTR MSG_HOME_OFFSET_K                  = STR_K _UxGT(" Ofset");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Ofsety nastavené");
  LSTR MSG_TRAMMING_WIZARD                = _UxGT("Spriev. vyrovn.");
  LSTR MSG_SELECT_ORIGIN                  = _UxGT("Vyberte začiatok");
  LSTR MSG_LAST_VALUE_SP                  = _UxGT("Posl. hodnota ");
  #if HAS_PREHEAT
    LSTR MSG_PREHEAT_1                    = _UxGT("Zahriať ") PREHEAT_1_LABEL;
    LSTR MSG_PREHEAT_1_H                  = _UxGT("Zahriať ") PREHEAT_1_LABEL " ~";
    LSTR MSG_PREHEAT_1_END                = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" hotend");
    LSTR MSG_PREHEAT_1_END_E              = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" hotend ~");
    LSTR MSG_PREHEAT_1_ALL                = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" všetko");
    LSTR MSG_PREHEAT_1_BEDONLY            = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" podlož");
    LSTR MSG_PREHEAT_1_SETTINGS           = _UxGT("Zahriať ") PREHEAT_1_LABEL _UxGT(" nast.");

    LSTR MSG_PREHEAT_M                    = _UxGT("Zahriať $");
    LSTR MSG_PREHEAT_M_H                  = _UxGT("Zahriať $ ~");
    LSTR MSG_PREHEAT_M_END                = _UxGT("Zahriať $ hotend");
    LSTR MSG_PREHEAT_M_END_E              = _UxGT("Zahriať $ hotend ~");
    LSTR MSG_PREHEAT_M_ALL                = _UxGT("Zahriať $ všetko");
    LSTR MSG_PREHEAT_M_BEDONLY            = _UxGT("Zahriať $ podlož");
    LSTR MSG_PREHEAT_M_SETTINGS           = _UxGT("Zahriať $ nast.");
  #endif
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Vlastná teplota");
  LSTR MSG_COOLDOWN                       = _UxGT("Schladiť");

  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frekvencia");
  LSTR MSG_LASER_MENU                     = _UxGT("Nastavenie lasera");
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Nastavenie vretena");
  LSTR MSG_LASER_POWER                    = _UxGT("Výkon lasera");
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Výkon vretena");
  LSTR MSG_LASER_TOGGLE                   = _UxGT("Prepnúť laser");
  LSTR MSG_LASER_EVAC_TOGGLE              = _UxGT("Prepnúť odsávanie");
  LSTR MSG_LASER_ASSIST_TOGGLE            = _UxGT("Prepnúť ofuk");
  LSTR MSG_LASER_PULSE_MS                 = _UxGT("Test. impulz ms");
  LSTR MSG_LASER_FIRE_PULSE               = _UxGT("Vystreliť impulz");
  LSTR MSG_FLOWMETER_FAULT                = _UxGT("Chyba chladenia");
  LSTR MSG_SPINDLE_TOGGLE                 = _UxGT("Prepnúť vreteno");
  LSTR MSG_SPINDLE_EVAC_TOGGLE            = _UxGT("Prepnúť odsávanie");
  LSTR MSG_SPINDLE_FORWARD                = _UxGT("Dopredný chod");
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Spätný chod");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Zapnúť napájanie");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Vypnúť napájanie");
  LSTR MSG_EXTRUDE                        = _UxGT("Vytlačiť (extr.)");
  LSTR MSG_RETRACT                        = _UxGT("Vytiahnuť (retr.)");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Posunúť osy");
  LSTR MSG_BED_LEVELING                   = _UxGT("Vyrovnanie podložky");
  LSTR MSG_LEVEL_BED                      = _UxGT("Vyrovnať podložku");
  LSTR MSG_BED_TRAMMING                   = _UxGT("Vyrovnať rohy");
  LSTR MSG_BED_TRAMMING_RAISE             = _UxGT("Zdvyhnite podl., kým sa nezopne sonda");
  LSTR MSG_BED_TRAMMING_IN_RANGE          = _UxGT("Rohy sú vrámci odchyl. Vyrovnajte podl.");
  LSTR MSG_BED_TRAMMING_GOOD_POINTS       = _UxGT("Dobré body: ");
  LSTR MSG_BED_TRAMMING_LAST_Z            = _UxGT("Posl. Z: ");
  LSTR MSG_NEXT_CORNER                    = _UxGT("Ďalší roh");
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor sieťe bodov");
  LSTR MSG_MESH_VIEWER                    = _UxGT("Zobraz. sieťe bodov");
  LSTR MSG_EDIT_MESH                      = _UxGT("Upraviť sieť bodov");
  LSTR MSG_MESH_VIEW                      = _UxGT("Zobraz. sieť bodov");
  LSTR MSG_EDITING_STOPPED                = _UxGT("Koniec úprav siete");
  LSTR MSG_NO_VALID_MESH                  = _UxGT("Neplatná sieť bodov");
  LSTR MSG_PROBING_POINT                  = _UxGT("Skúšam bod");
  LSTR MSG_MESH_X                         = _UxGT("Index X");
  LSTR MSG_MESH_Y                         = _UxGT("Index Y");
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Hodnota Z");
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Vlastné príkazy");
  LSTR MSG_M48_TEST                       = _UxGT("M48 Test sondy");
  LSTR MSG_M48_POINT                      = _UxGT("M48 Bod");
  LSTR MSG_M48_OUT_OF_BOUNDS              = _UxGT("Sonda mimo hraníc");
  LSTR MSG_M48_DEVIATION                  = _UxGT("Odchýlka");
  LSTR MSG_IDEX_MENU                      = _UxGT("IDEX režim");
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Ofset nástrojov");
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-parkovanie");
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplikácia");
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Zrkadlená kópia");
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Plná kontrola");
  LSTR MSG_IDEX_DUPE_GAP                  = _UxGT("Duplik. medz.-X");
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2. tryska Z");
  LSTR MSG_HOTEND_OFFSET_A                = _UxGT("2. tryska @");
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Vykonávam G29");
  LSTR MSG_UBL_TOOLS                      = _UxGT("Nástroje UBL");
  LSTR MSG_UBL_LEVEL_BED                  = _UxGT("UBL rovnanie");
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Vyrovnávam bod");
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Manuálna sieť bodov");
  LSTR MSG_UBL_MESH_WIZARD                = _UxGT("Spriev. UBL rovnan.");
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Položte a zmerajte");
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Zmerajte");
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Odstráňte a zmerajte");
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Presun na ďalší");
  LSTR MSG_UBL_ACTIVATE_MESH              = _UxGT("Aktivovať UBL");
  LSTR MSG_UBL_DEACTIVATE_MESH            = _UxGT("Deaktivovať UBL");
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Teplota podložky");
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Teplota podložky");
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Teplota hotendu");
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Teplota hotendu");
  LSTR MSG_UBL_MESH_EDIT                  = _UxGT("Úprava siete bodov");
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Upraviť vlastnú sieť");
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Doladiť sieť bodov");
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Koniec úprav siete");
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Vlastná sieť");
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Vytvoriť sieť");
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Sieť bodov ($)");
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Studená sieť bodov");
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Upraviť výšku siete");
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Výška");
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Skontrolovať sieť");
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Kontrola siete ($)");
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Kontrola vlast.siete");
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 ohrev podlž.");
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 ohrev trysky");
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Ručné čistenie...");
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Čistenie pevn. dĺž.");
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Čistenie dokončené");
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 zrušený");
  LSTR MSG_G26_LEAVING                    = _UxGT("Opúšťam G26");
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Pokračovať v sieti");
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Sieťové rovnanie");
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("3-bodové rovnanie");
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Mriežkové rovnanie");
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Vyrovnať podložku");
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Postranné body");
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Typ siete bodov");
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Exportovať sieť");
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Export do hosta");
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Export do CSV");
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Externá záloha");
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Info. o výst. UBL");
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Hustota mriežky");
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Ručné vyplnenie");
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Chytré vyplnenie");
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Vyplniť mriežku");
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Zrušiť všetko");
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Zrušiť najbližší");
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Upraviť všetky");
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Upraviť najbližší");
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Úložisko sietí");
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Pamäťový slot");
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Načítať sieť bodov");
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Uložiť sieť bodov");
  LSTR MSG_MESH_LOADED                    = _UxGT("Sieť %i načítaná");
  LSTR MSG_MESH_SAVED                     = _UxGT("Sieť %i uložená");
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Nedostatok miesta");
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Chyba: Ukladanie UBL");
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Chyba: Obnovenie UBL");
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-Ofset: ");
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Koniec kompenz. Z");
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("Postupné UBL");
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1.Studená sieť bodov");
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2.Chytré vyplnenie");
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3.Skontrolovať sieť");
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4.Upraviť všetky");
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5.Skontrolovať sieť");
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6.Upraviť všetky");
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7.Uložiť sieť bodov");

  LSTR MSG_LED_CONTROL                    = _UxGT("Nastavenie LED");
  LSTR MSG_LEDS                           = _UxGT("Svetlo");
  LSTR MSG_LED_PRESETS                    = _UxGT("Predvolby svetla");
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Červená");
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Oranžová");
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Žltá");
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Zelená");
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Modrá");
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Fialová");
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Biela");
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Obnoviť nastavenie");
  LSTR MSG_LED_CHANNEL_N                  = _UxGT("Kanál =");
  LSTR MSG_LEDS2                          = _UxGT("Svetlo #2");
  LSTR MSG_NEO2_PRESETS                   = _UxGT("Predvolby svetla #2");
  LSTR MSG_NEO2_BRIGHTNESS                = _UxGT("Jas");
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Vlastná farba");
  LSTR MSG_INTENSITY_R                    = _UxGT("Inten. červenej");
  LSTR MSG_INTENSITY_G                    = _UxGT("Inten. zelenej");
  LSTR MSG_INTENSITY_B                    = _UxGT("Inten. modrej");
  LSTR MSG_INTENSITY_W                    = _UxGT("Inten. bielej");
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Jas");

  LSTR MSG_MOVING                         = _UxGT("Posúvam...");
  LSTR MSG_FREE_XY                        = _UxGT("Uvolniť XY");
  LSTR MSG_MOVE_X                         = _UxGT("Posunúť X");
  LSTR MSG_MOVE_Y                         = _UxGT("Posunúť Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Posunúť Z");
  LSTR MSG_MOVE_I                         = _UxGT("Posunúť ") STR_I;
  LSTR MSG_MOVE_J                         = _UxGT("Posunúť ") STR_J;
  LSTR MSG_MOVE_K                         = _UxGT("Posunúť ") STR_K;
  LSTR MSG_MOVE_E                         = _UxGT("Extrudér");
  LSTR MSG_MOVE_EN                        = _UxGT("Extrudér *");
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Hotend je studený");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Posunúť o %smm");
  LSTR MSG_MOVE_01MM                      = _UxGT("Posunúť o 0,1mm");
  LSTR MSG_MOVE_1MM                       = _UxGT("Posunúť o 1mm");
  LSTR MSG_MOVE_10MM                      = _UxGT("Posunúť o 10mm");
  LSTR MSG_MOVE_100MM                     = _UxGT("Posunúť o 100mm");
  LSTR MSG_MOVE_0001IN                    = _UxGT("Posunúť o 0,001in");
  LSTR MSG_MOVE_001IN                     = _UxGT("Posunúť o 0,01in");
  LSTR MSG_MOVE_01IN                      = _UxGT("Posunúť o 0,1in");
  LSTR MSG_MOVE_1IN                       = _UxGT("Posunúť o 1,0in");
  LSTR MSG_SPEED                          = _UxGT("Rýchlosť");
  LSTR MSG_MAXSPEED                       = _UxGT("Max rýchl. (mm/s)");
  LSTR MSG_MAXSPEED_X                     = _UxGT("Max rýchl. ") STR_A;
  LSTR MSG_MAXSPEED_Y                     = _UxGT("Max rýchl. ") STR_B;
  LSTR MSG_MAXSPEED_Z                     = _UxGT("Max rýchl. ") STR_C;
  LSTR MSG_MAXSPEED_E                     = _UxGT("Max rýchl. ") STR_E;
  LSTR MSG_MAXSPEED_A                     = _UxGT("Max rýchl. @");
  LSTR MSG_BED_Z                          = _UxGT("Výška podl.");
  LSTR MSG_NOZZLE                         = _UxGT("Tryska");
  LSTR MSG_NOZZLE_N                       = _UxGT("Tryska ~");
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Tryska zaparkovaná");
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Záložná tryska");
  LSTR MSG_BED                            = _UxGT("Podložka");
  LSTR MSG_CHAMBER                        = _UxGT("Komora");
  LSTR MSG_COOLER                         = _UxGT("Chladen. lasera");
  LSTR MSG_COOLER_TOGGLE                  = _UxGT("Prepnúť chladenie");
  LSTR MSG_FLOWMETER_SAFETY               = _UxGT("Bezpeč. prietok");
  LSTR MSG_LASER                          = _UxGT("Laser");
  LSTR MSG_FAN_SPEED                      = _UxGT("Rýchlosť vent.");
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Rýchlosť vent. ~");
  LSTR MSG_STORED_FAN_N                   = _UxGT("Ulož. vent. ~");
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Rýchlosť ex. vent.");
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Rýchlosť ex. vent. ~");
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Vent. riad. jedn.");
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Voľno. rýchl.");
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Auto-režim");
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Aktív. rýchl.");
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Doba nečinnosti");
  LSTR MSG_FLOW                           = _UxGT("Prietok");
  LSTR MSG_FLOW_N                         = _UxGT("Prietok ~");
  LSTR MSG_CONTROL                        = _UxGT("Ovládanie");
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fakt");
  LSTR MSG_AUTOTEMP                       = _UxGT("Auto-teplota");
  LSTR MSG_LCD_ON                         = _UxGT("Zap");
  LSTR MSG_LCD_OFF                        = _UxGT("Vyp");
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("Kalibrácia PID");
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("Kalibrácia PID *");
  LSTR MSG_PID_CYCLE                      = _UxGT("Cykly PID");
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("Kal. PID dokončená");
  LSTR MSG_PID_BAD_EXTRUDER_NUM           = _UxGT("Auto-kal. zlyhala. Zlý extrúder.");
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Auto-kal. zlyhala. Príliš vysoká tepl.");
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Auto-kal. zlyhala! Čas vypršal.");
  LSTR MSG_SELECT                         = _UxGT("Vybrať");
  LSTR MSG_SELECT_E                       = _UxGT("Vybrať *");
  LSTR MSG_ACC                            = _UxGT("Zrýchlenie");
  LSTR MSG_JERK                           = _UxGT("Skok");
  LSTR MSG_VA_JERK                        = _UxGT("V") STR_A _UxGT("-skok");
  LSTR MSG_VB_JERK                        = _UxGT("V") STR_B _UxGT("-skok");
  LSTR MSG_VC_JERK                        = _UxGT("V") STR_C _UxGT("-skok");
  LSTR MSG_VI_JERK                        = _UxGT("V") STR_I _UxGT("-skok");
  LSTR MSG_VJ_JERK                        = _UxGT("V") STR_J _UxGT("-skok");
  LSTR MSG_VK_JERK                        = _UxGT("V") STR_K _UxGT("-skok");
  LSTR MSG_VE_JERK                        = _UxGT("Ve-skok");
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Junction Dev");
  LSTR MSG_VELOCITY                       = _UxGT("Rýchlosť");
  LSTR MSG_VMAX_A                         = _UxGT("Vmax ") STR_A;
  LSTR MSG_VMAX_B                         = _UxGT("Vmax ") STR_B;
  LSTR MSG_VMAX_C                         = _UxGT("Vmax ") STR_C;
  LSTR MSG_VMAX_I                         = _UxGT("Vmax ") STR_I;
  LSTR MSG_VMAX_J                         = _UxGT("Vmax ") STR_J;
  LSTR MSG_VMAX_K                         = _UxGT("Vmax ") STR_K;
  LSTR MSG_VMAX_E                         = _UxGT("Vmax ") STR_E;
  LSTR MSG_VMAX_EN                        = _UxGT("Vmax *");
  LSTR MSG_VMIN                           = _UxGT("Vmin");
  LSTR MSG_VTRAV_MIN                      = _UxGT("VPrej Min");
  LSTR MSG_ACCELERATION                   = _UxGT("Akcelerácia");
  LSTR MSG_AMAX_A                         = _UxGT("Amax ") STR_A;
  LSTR MSG_AMAX_B                         = _UxGT("Amax ") STR_B;
  LSTR MSG_AMAX_C                         = _UxGT("Amax ") STR_C;
  LSTR MSG_AMAX_I                         = _UxGT("Amax ") STR_I;
  LSTR MSG_AMAX_J                         = _UxGT("Amax ") STR_J;
  LSTR MSG_AMAX_K                         = _UxGT("Amax ") STR_K;
  LSTR MSG_AMAX_E                         = _UxGT("Amax ") STR_E;
  LSTR MSG_AMAX_EN                        = _UxGT("Amax *");
  LSTR MSG_A_RETRACT                      = _UxGT("A-retrakt");
  LSTR MSG_A_TRAVEL                       = _UxGT("A-prejazd");
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Max. frekvencia");
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Min. posun");
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Kroky/mm");
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" krokov/mm");
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" krokov/mm");
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" krokov/mm");
  LSTR MSG_I_STEPS                        = STR_I _UxGT(" krokov/mm");
  LSTR MSG_J_STEPS                        = STR_J _UxGT(" krokov/mm");
  LSTR MSG_K_STEPS                        = STR_K _UxGT(" krokov/mm");
  LSTR MSG_E_STEPS                        = _UxGT("Ekrokov/mm");
  LSTR MSG_EN_STEPS                       = _UxGT("*krokov/mm");
  LSTR MSG_TEMPERATURE                    = _UxGT("Teplota");
  LSTR MSG_MOTION                         = _UxGT("Pohyb");
  LSTR MSG_FILAMENT                       = _UxGT("Filament");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E v mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT               = _UxGT("E Limit v mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT_E             = _UxGT("E Limit *");
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Priem. fil.");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Priem. fil. *");
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Vysunúť mm");
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Zaviesť mm");
  LSTR MSG_ADVANCE_K                      = _UxGT("K pre posun");
  LSTR MSG_ADVANCE_K_E                    = _UxGT("K pre posun *");
  LSTR MSG_CONTRAST                       = _UxGT("Kontrast LCD");
  LSTR MSG_BRIGHTNESS                     = _UxGT("Jas LCD");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Uložiť nastavenie");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Načítať nastavenie");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Obnoviť nastavenie");
  LSTR MSG_INIT_EEPROM                    = _UxGT("Inicializ. EEPROM");
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("Chyba: EEPROM CRC");
  LSTR MSG_ERR_EEPROM_INDEX               = _UxGT("Chyba: EEPROM Index");
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("Chyba: Verzia EEPROM");
  LSTR MSG_SETTINGS_STORED                = _UxGT("Nastavenie uložené");
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Aktualizovať z SD");
  LSTR MSG_RESET_PRINTER                  = _UxGT("Reštart. tlačiar.");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH  _UxGT("Obnoviť");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info. obrazovka");
  LSTR MSG_PREPARE                        = _UxGT("Príprava tlače");
  LSTR MSG_TUNE                           = _UxGT("Doladenie tlače");
  LSTR MSG_POWER_MONITOR                  = _UxGT("Monitor napájania");
  LSTR MSG_CURRENT                        = _UxGT("Prúd");
  LSTR MSG_VOLTAGE                        = _UxGT("Napätie");
  LSTR MSG_POWER                          = _UxGT("Výkon");
  LSTR MSG_START_PRINT                    = _UxGT("Spustiť tlač");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Ďalší");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Inicial.");
  LSTR MSG_BUTTON_STOP                    = _UxGT("Zastaviť");
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Tlačiť");
  LSTR MSG_BUTTON_RESET                   = _UxGT("Vynulovať");
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignorovať");
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Zrušiť");
  LSTR MSG_BUTTON_DONE                    = _UxGT("Hotovo");
  LSTR MSG_BUTTON_BACK                    = _UxGT("Naspäť");
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Pokračovať");
  LSTR MSG_BUTTON_SKIP                    = _UxGT("Preskočiť");
  LSTR MSG_BUTTON_INFO                    = _UxGT("Informácie");
  LSTR MSG_BUTTON_LEVEL                   = _UxGT("Vyrovnať");
  LSTR MSG_BUTTON_PAUSE                   = _UxGT("Pauza");
  LSTR MSG_BUTTON_RESUME                  = _UxGT("Obnoviť");
  LSTR MSG_BUTTON_ADVANCED                = _UxGT("Pokročilé");
  LSTR MSG_PAUSING                        = _UxGT("Pozastavujem...");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pozastaviť tlač");
  LSTR MSG_ADVANCED_PAUSE                 = _UxGT("Pokročil. pauza");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Obnoviť tlač");
  LSTR MSG_HOST_START_PRINT               = _UxGT("Spustiť z hosta");
  LSTR MSG_STOP_PRINT                     = _UxGT("Zastaviť tlač");
  LSTR MSG_END_LOOPS                      = _UxGT("Koniec opak. sluč.");
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Tlačím objekt");
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Zrušiť objekt");
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Zrušiť objekt =");
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Obnova po výp. nap.");
  LSTR MSG_CONTINUE_PRINT_JOB             = _UxGT("Pokračovať v úlohe");
  LSTR MSG_MEDIA_MENU                     = _UxGT("Tlačiť z SD");
  LSTR MSG_NO_MEDIA                       = _UxGT("Žiadna SD karta");
  LSTR MSG_DWELL                          = _UxGT("Spím...");
  LSTR MSG_USERWAIT                       = _UxGT("Pokrač. kliknutím...");
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Tlač pozastavená");
  LSTR MSG_PRINTING                       = _UxGT("Tlačím...");
  LSTR MSG_STOPPING                       = _UxGT("Zastavujem...");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Tlač zrušená");
  LSTR MSG_PRINT_DONE                     = _UxGT("Tlač dokončená");
  LSTR MSG_NO_MOVE                        = _UxGT("Žiadny pohyb.");
  LSTR MSG_KILLED                         = _UxGT("PRERUŠENÉ. ");
  LSTR MSG_STOPPED                        = _UxGT("ZASTAVENÉ. ");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Retrakt mm");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Výmena Re.mm");
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Retraktovať  V");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Zdvih Z mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("S Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("S UnRet V");
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Dĺžka výmeny");
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Vymeniť naviac");
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Dĺžka vytlačenia");
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Výmena nástroja");
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Zdvihnúť Z");
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Primárna rýchl.");
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Rýchl. retrakcie");
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Parkovať hlavu");
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Rýchl. obnovenia");
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Rýchlosť vent.");
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Doba fúkania");
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto-Zap");
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto-Vyp");
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Výmena nástroja");
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Auto-výmena");
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Posl. extruder");
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Vymeniť za *");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Vymeniť filament");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Vymeniť filament *");
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Zaviesť filament");
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Zaviesť filament *");
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Vysunúť filament");
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Vysunúť filament *");
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Vysunúť všetko");
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Načítať SD kartu");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Vymeniť SD kartu");
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Odpojiť SD kartu");
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonda Z mimo podl.");
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Faktor skosenia");
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Self-Test");
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset");
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Zasunúť");
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Vysunúť");
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("Režim SW");
  LSTR MSG_BLTOUCH_SPEED_MODE             = _UxGT("Vysoká rýchl.");
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("Režim 5V");
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("Režim OD");
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Ulož. režim");
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Prepnúť do 5V");
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Prepnúť do OD");
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Zobraziť režim");
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("POZOR: Zlé nastav. môže spôsobiť poškoden. Pokračovať?");
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Inicializ. TouchMI");
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test Z ofsetu");
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Uložiť");
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Zasunúť TouchMI");
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Zasunúť sondu Z");
  LSTR MSG_MANUAL_STOW                    = _UxGT("Vysunúť sondu Z");
  LSTR MSG_HOME_FIRST                     = _UxGT("Najskôr os %s%s%s domov");
  LSTR MSG_ZPROBE_SETTINGS                = _UxGT("Nastav. sondy");
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Ofsety sondy Z");
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X ofset");
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y ofset");
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z ofset");
  LSTR MSG_MOVE_NOZZLE_TO_BED             = _UxGT("Pos. trysku k podl.");
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystep X");
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystep Y");
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystep Z");
  LSTR MSG_BABYSTEP_I                     = _UxGT("Babystep ") STR_I;
  LSTR MSG_BABYSTEP_J                     = _UxGT("Babystep ") STR_J;
  LSTR MSG_BABYSTEP_K                     = _UxGT("Babystep ") STR_K;
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Celkom");
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Zastavenie Endstop");
  LSTR MSG_HEATING_FAILED_LCD             = _UxGT("Chyba ohrevu");
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Chyba: REDUND. TEP.");
  LSTR MSG_THERMAL_RUNAWAY                = _UxGT("TEPLOTNÝ SKOK");
  LSTR MSG_TEMP_MALFUNCTION               = _UxGT("TEPLOTNÁ PORUCHA");
  LSTR MSG_THERMAL_RUNAWAY_BED            = _UxGT("TEPLOTNÝ SKOK PODL.");
  LSTR MSG_THERMAL_RUNAWAY_CHAMBER        = _UxGT("TEPLOTNÝ SKOK KOMO.");
  LSTR MSG_THERMAL_RUNAWAY_COOLER         = _UxGT("TEPLOTNÝ SKOK CHLAD.");
  LSTR MSG_COOLING_FAILED                 = _UxGT("Ochladz. zlyhalo");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Chyba: MAXTEMP");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Chyba: MINTEMP");
  LSTR MSG_HALTED                         = _UxGT("TLAČIAREŇ ZASTAVENÁ");
  LSTR MSG_PLEASE_WAIT                    = _UxGT("Čakajte prosím...");
  LSTR MSG_PLEASE_RESET                   = _UxGT("Reštartuje ju");
  LSTR MSG_PREHEATING                     = _UxGT("Zahrievanie...");
  LSTR MSG_HEATING                        = _UxGT("Ohrev...");
  LSTR MSG_COOLING                        = _UxGT("Ochladzovanie...");
  LSTR MSG_BED_HEATING                    = _UxGT("Ohrev podložky...");
  LSTR MSG_BED_COOLING                    = _UxGT("Ochladz. podložky...");
  LSTR MSG_PROBE_HEATING                  = _UxGT("Ohrev sondy...");
  LSTR MSG_PROBE_COOLING                  = _UxGT("Ochladz. sondy...");
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Ohrev komory...");
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Ochladz. komory...");
  LSTR MSG_LASER_COOLING                  = _UxGT("Ochladz. lasera...");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta kalibrácia");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibrovať X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibrovať Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibrovať Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibrovať stred");
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Delta nastavenia");
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto-kalibrácia");
  LSTR MSG_DELTA_HEIGHT_CALIBRATE         = _UxGT("Nast. výšku delty");
  LSTR MSG_DELTA_Z_OFFSET_CALIBRATE       = _UxGT("Ofset sondy Z");
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Diag. rameno");
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Výška");
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Polomer");
  LSTR MSG_INFO_MENU                      = _UxGT("O tlačiarni");
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info. o tlačiarni");
  LSTR MSG_3POINT_LEVELING                = _UxGT("3-bodové rovnanie");
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Lineárne rovnanie");
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Bilineárne rovnanie");
  LSTR MSG_UBL_LEVELING                   = _UxGT("UBL rovnanie");
  LSTR MSG_MESH_LEVELING                  = _UxGT("Mriežkové rovnanie");
  LSTR MSG_MESH_DONE                      = _UxGT("Mriežka dokončená");
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Štatistika");
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info. o doske");
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistory");
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrudéry");
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Rýchlosť");
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokol");
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Tepl. ochrana: VYP");
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Tepl. ochrana: ZAP");
  LSTR MSG_HOTEND_IDLE_TIMEOUT            = _UxGT("Vypr.čas nečinnosti");
  LSTR MSG_FAN_SPEED_FAULT                = _UxGT("Chyba rýchl. vent.");

  LSTR MSG_CASE_LIGHT                     = _UxGT("Osvetlenie");
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Jas svetla");
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Nesprávna tlačiareň");

  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2
    LSTR MSG_MEDIA_NOT_INSERTED           = _UxGT("Nie je vložená karta.");
    LSTR MSG_REMAINING_TIME               = _UxGT("Zostávajúci čas");
    LSTR MSG_PLEASE_WAIT_REBOOT           = _UxGT("Prosím čakajte do reštartu. ");
    LSTR MSG_PLEASE_PREHEAT               = _UxGT("Prosím zahrejte hotend.");
    LSTR MSG_INFO_PRINT_COUNT_RESET       = _UxGT("Vynulovať počítadlo");
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Počet tlačí");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Celkový čas");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Najdlhšia tlač");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Celkom vytlačené");
    LSTR MSG_COLORS_GET                   = _UxGT("Zvoliť farbu");
    LSTR MSG_COLORS_SELECT                = _UxGT("Zvoliť farby");
    LSTR MSG_COLORS_APPLIED               = _UxGT("Farby aplikované");
    LSTR MSG_COLORS_RED                   = _UxGT("Červená");
    LSTR MSG_COLORS_GREEN                 = _UxGT("Zelená");
    LSTR MSG_COLORS_BLUE                  = _UxGT("Modrá");
    LSTR MSG_UI_LANGUAGE                  = _UxGT("Jazyk rozhrania");
    LSTR MSG_SOUND_ENABLE                 = _UxGT("Povoliť zvuky");
    LSTR MSG_LOCKSCREEN                   = _UxGT("Uzamknúť obrazovku");
  #else
    LSTR MSG_MEDIA_NOT_INSERTED           = _UxGT("Žiadna karta");
    LSTR MSG_PLEASE_PREHEAT               = _UxGT("Prosím zahrejte");
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Tlače");
    LSTR MSG_REMAINING_TIME               = _UxGT("Zostávajúci");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Čas");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Najdlhšia");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Vytlačené");
  #endif

  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Dokončené");
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Teplota min");
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Teplota max");
  LSTR MSG_INFO_PSU                       = _UxGT("Nap. zdroj");
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Budenie motorov");
  LSTR MSG_DAC_PERCENT_A                  = STR_A _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_B                  = STR_B _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_C                  = STR_C _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_I                  = STR_I _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_J                  = STR_J _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_K                  = STR_K _UxGT(" Motor %");
  LSTR MSG_DAC_PERCENT_E                  = _UxGT("E Motor %");
  LSTR MSG_ERROR_TMC                      = _UxGT("CHYBA KOMUNIKÁ. TMC");
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Uložiť do EEPROM");
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("VÝMENA FILAMENTU");
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("PAUZA TLAČE");
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("ZAVEDENIE FILAMENTU");
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("VYSUNUTIE FILAMENTU");
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("MOŽNOSTI POKRAČ.:");
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Vytlačiť viacej");
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Obnoviť tlač");
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Tryska: ");
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Senzor filamentu");
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Vzd. mm fil. senz.");
  LSTR MSG_RUNOUT_ENABLE                  = _UxGT("Zapnúť senzor");
  LSTR MSG_FANCHECK                       = _UxGT("Kontrola rýchl.");
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Parkovanie zlyhalo");
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Kalibrácia zlyhala");

  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("VYBERTE FILAMENT");
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU2");
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Aktualizujte FW MMU!");
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU potrebuje zásah.");
  LSTR MSG_MMU2_RESUME                    = _UxGT("Obnoviť tlač");
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Obnovovanie...");
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Zaviesť filament");
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Zaviesť všetky");
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Zaviesť po trysku");
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Vysunúť filament");
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Vysunúť filament ~");
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Vyňať filament");
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Zavádzanie fil. %i...");
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Vysúvanie fil. ...");
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Vysúvanie fil. ...");
  LSTR MSG_MMU2_ALL                       = _UxGT("Všetky");
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filament ~");
  LSTR MSG_MMU2_RESET                     = _UxGT("Reštartovať MMU");
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Reštart MMU...");
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Odstráňte, kliknite");

  LSTR MSG_MIX                            = _UxGT("Mix");
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Zložka ~");
  LSTR MSG_MIXER                          = _UxGT("Mixér");
  LSTR MSG_GRADIENT                       = _UxGT("Gradient");
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Plný gradient");
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Prepnúť mix");
  LSTR MSG_CYCLE_MIX                      = _UxGT("Cyklický mix");
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Gradientný mix");
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Otočiť gradient");
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Aktívny V-tool");
  LSTR MSG_START_VTOOL                    = _UxGT("Počiat. V-tool");
  LSTR MSG_END_VTOOL                      = _UxGT("Konečn. V-tool");
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias V-tool");
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Vynulovať V-tools");
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Uložiť V-tool Mix");
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-tools vynulované");
  LSTR MSG_START_Z                        = _UxGT("Počiat.Z:");
  LSTR MSG_END_Z                          = _UxGT("Konečn.Z:");

  LSTR MSG_GAMES                          = _UxGT("Hry");
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");
  LSTR MSG_INVADERS                       = _UxGT("Nájazdníci");
  LSTR MSG_SNAKE                          = _UxGT("Had");
  LSTR MSG_MAZE                           = _UxGT("Bludisko");

  LSTR MSG_BAD_PAGE                       = _UxGT("Chyb. index stránky");
  LSTR MSG_BAD_PAGE_SPEED                 = _UxGT("Chyb. rých. stránky");

  LSTR MSG_EDIT_PASSWORD                  = _UxGT("Zmeniť heslo");
  LSTR MSG_LOGIN_REQUIRED                 = _UxGT("Vyžad. sa prihl.");
  LSTR MSG_PASSWORD_SETTINGS              = _UxGT("Nastavenie hesla");
  LSTR MSG_ENTER_DIGIT                    = _UxGT("Zvoľte číslo");
  LSTR MSG_CHANGE_PASSWORD                = _UxGT("Zmeniť heslo");
  LSTR MSG_REMOVE_PASSWORD                = _UxGT("Odstrániť heslo");
  LSTR MSG_PASSWORD_SET                   = _UxGT("Heslo je ");
  LSTR MSG_START_OVER                     = _UxGT("Začať odznova");
  LSTR MSG_REMINDER_SAVE_SETTINGS         = _UxGT("Nezabudnite uložiť!");
  LSTR MSG_PASSWORD_REMOVED               = _UxGT("Heslo odstránené");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Stlačte tlačidlo", "pre obnovu tlače"));
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parkovanie..."));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Čakajte prosím", "na spustenie", "výmeny filamentu"));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Vložte filament", "a stlačte tlačidlo", "pre pokračovanie"));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Stlačte tlačidlo", "pre ohrev trysky"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Ohrev trysky", "Čakajte prosím..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_3_LINE("Čakajte prosím", "na vysunutie", "filamentu"));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_3_LINE("Čakajte prosím", "na zavedenie", "filamentu"));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_3_LINE("Čakajte prosím", "na vytlačenie", "filamentu"));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_3_LINE("Stlačte tlačidlo", "pre dokončenie", "vytláčania filam."));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Čakajte prosím na", "obnovenie tlače..."));
  #else
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_1_LINE("Kliknite pre pokr."));
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parkovanie..."));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_1_LINE("Čakajte prosím..."));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_1_LINE("Vložte a kliknite"));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_1_LINE("Kliknite pre ohrev"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_1_LINE("Ohrev..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_1_LINE("Vysúvanie..."));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_1_LINE("Zavádzanie..."));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_1_LINE("Vytlačovanie..."));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_1_LINE("Klik. pre dokonč."));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_1_LINE("Pokračovanie..."));
  #endif
  LSTR MSG_TMC_DRIVERS                    = _UxGT("Ovládače TMC");
  LSTR MSG_TMC_CURRENT                    = _UxGT("Prúd ovládača");
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Hybridný prah");
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Bezsenzor. domov");
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Režim krokovania");
  LSTR MSG_TMC_STEALTH_ENABLED            = _UxGT("StealthChop zapnutý");
  LSTR MSG_SERVICE_RESET                  = _UxGT("Vynulovať");
  LSTR MSG_SERVICE_IN                     = _UxGT("za:");
  LSTR MSG_BACKLASH                       = _UxGT("Kompenz. vôle");
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Korekcia");
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Vyhladzovanie");

  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Vyrovnať os X");
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Auto-kalibrovať");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("Vypršal čas ohrevu, znížená teplota. Stlačte OK pre ohrev a ešte raz pre obnovu.");
  #else
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("Vypršal čas ohrevu");
  #endif
  LSTR MSG_REHEAT                         = _UxGT("Zohriať");
  LSTR MSG_REHEATING                      = _UxGT("Zohrievanie...");
  LSTR MSG_REHEATDONE                     = _UxGT("Zohrievanie dokonč.");

  LSTR MSG_PROBE_WIZARD                   = _UxGT("Sprievodca sondy Z");
  LSTR MSG_PROBE_WIZARD_PROBING           = _UxGT("Referencia Z");
  LSTR MSG_PROBE_WIZARD_MOVING            = _UxGT("Presúvam na pozíciu");

  LSTR MSG_XATC                           = _UxGT("Sprievodca X-Twist");
  LSTR MSG_XATC_DONE                      = _UxGT("Spriev. X-Twist dokonč.!");
  LSTR MSG_XATC_UPDATE_Z_OFFSET           = _UxGT("Aktual. ofset sondy Z na ");

  LSTR MSG_SOUND                          = _UxGT("Zvuk");

  LSTR MSG_TOP_LEFT                       = _UxGT("Ľavý horný");
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Ľavý dolný");
  LSTR MSG_TOP_RIGHT                      = _UxGT("Pravý horný");
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Pravý dolný");
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Kalibrácia dokončená");
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Kalibrácia zlyhala");

  LSTR MSG_DRIVER_BACKWARD                = _UxGT(" spätný chod ovl.");

  LSTR MSG_SD_CARD                        = _UxGT("SD karta");
  LSTR MSG_USB_DISK                       = _UxGT("USB disk");

  LSTR MSG_HOST_SHUTDOWN                  = _UxGT("Vypnúť hosta");
}
