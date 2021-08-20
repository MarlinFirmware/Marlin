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
 * Hungarian / Magyar
 *
 * LCD Menu Messages. See also https://marlinfw.org/docs/development/lcd_language.html
 * Hungarian translation by AntoszHUN. I am constantly improving and updating the translation.
 * Translation last updated: 07/07/2021 - 11:20
 *
 * LCD Menü Üzenetek. Lásd még https://marlinfw.org/docs/development/lcd_language.html
 * A Magyar fordítást készítette: AntoszHUN. A fordítást folyamatosan javítom és frissítem.
 * A Fordítás utolsó frissítése: 2021.07.07. - 11:20
 */

namespace Language_hu {
  using namespace Language_en; // A fordítás az örökölt Amerikai Angol (English) karakterláncokat használja.

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Magyar");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Kész.");
  PROGMEM Language_Str MSG_MARLIN                          = _UxGT("Marlin");
  PROGMEM Language_Str MSG_YES                             = _UxGT("IGEN");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NEM");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Vissza");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Megszakítás...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Tároló behelyezve");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Tároló eltávolítva");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Várakozás a tárolóra");
  PROGMEM Language_Str MSG_SD_INIT_FAIL                    = _UxGT("SD-kártya hiba");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Tároló olvasási hiba");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB eltávolítva");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB eszköz hiba");
  PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW           = _UxGT("Túlfolyás");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Végállás"); // Maximum 8 karakter
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Szoft. végállás");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("<Fömenü>");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("További beállítások");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfiguráció");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Fájl auto. futtatás");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Motorok kikapcsolása");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Hiba Menü");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Haladás sáv teszt");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("X-Y-Z auto kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("X kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Y kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Z kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_I                     = _UxGT("Kezdö ") LCD_STR_I;
  PROGMEM Language_Str MSG_AUTO_HOME_J                     = _UxGT("Kezdö ") LCD_STR_J;
  PROGMEM Language_Str MSG_AUTO_HOME_K                     = _UxGT("Kezdö ") LCD_STR_K;
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto Z-igazítás");
  PROGMEM Language_Str MSG_ITERATION                       = _UxGT("G34 Ismétlés: %i");
  PROGMEM Language_Str MSG_DECREASING_ACCURACY             = _UxGT("Pontosság csökken!");
  PROGMEM Language_Str MSG_ACCURACY_ACHIEVED               = _UxGT("Pontosság elérve");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("X-Y-Z kezdöpont");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Kattints a kezdéshez.");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Következö pont");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Szintezés kész!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Szint csökkentés");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Kezdöpont eltolás");
  PROGMEM Language_Str MSG_HOME_OFFSET_X                   = _UxGT("X Kezdö eltol.");
  PROGMEM Language_Str MSG_HOME_OFFSET_Y                   = _UxGT("Y Kezdö eltol.");
  PROGMEM Language_Str MSG_HOME_OFFSET_Z                   = _UxGT("Z Kezdö eltol.");
  PROGMEM Language_Str MSG_HOME_OFFSET_I                   = _UxGT("Kezdö eltol. ") LCD_STR_I;
  PROGMEM Language_Str MSG_HOME_OFFSET_J                   = _UxGT("Kezdö eltol. ") LCD_STR_J;
  PROGMEM Language_Str MSG_HOME_OFFSET_K                   = _UxGT("Kezdö eltol. ") LCD_STR_K;
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Eltolás beállítva.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Eredeti Be");
  PROGMEM Language_Str MSG_TRAMMING_WIZARD                 = _UxGT("Elektromos varázsló");
  PROGMEM Language_Str MSG_SELECT_ORIGIN                   = _UxGT("Eredeti választása");
  PROGMEM Language_Str MSG_LAST_VALUE_SP                   = _UxGT("Utolsó érték ");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Fütés ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Fütés ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Fütés ") PREHEAT_1_LABEL _UxGT(" Fej");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Fütés ") PREHEAT_1_LABEL _UxGT(" Fej ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Fütés ") PREHEAT_1_LABEL _UxGT(" Mind");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Fütés ") PREHEAT_1_LABEL _UxGT(" Ágy");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Fütés ") PREHEAT_1_LABEL _UxGT(" Beáll");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Fütés $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Fütés $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Fütés $ Fej");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Fütés $ Fej ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Fütés $ Mind");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Fütés $ Ágy");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Fütés $ Beáll");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Egyedi elömelegítés");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Visszahütés");

  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frekvencia");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Lézer vezérlés");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Orsó vezérlés");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Lézer telj.");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Orsó telj.");
  PROGMEM Language_Str MSG_LASER_TOGGLE                    = _UxGT("Lézer váltás");
  PROGMEM Language_Str MSG_LASER_EVAC_TOGGLE               = _UxGT("Hütés váltás");
  PROGMEM Language_Str MSG_LASER_ASSIST_TOGGLE             = _UxGT("Levegö segéd");
  PROGMEM Language_Str MSG_LASER_PULSE_MS                  = _UxGT("Impulzus teszt ms");
  PROGMEM Language_Str MSG_LASER_FIRE_PULSE                = _UxGT("Tüz impulzus");
  PROGMEM Language_Str MSG_FLOWMETER_FAULT                 = _UxGT("Áramlási hiba");
  PROGMEM Language_Str MSG_SPINDLE_TOGGLE                  = _UxGT("Orsóváltás");
  PROGMEM Language_Str MSG_SPINDLE_EVAC_TOGGLE             = _UxGT("Vákuum váltás");
  PROGMEM Language_Str MSG_SPINDLE_FORWARD                 = _UxGT("Orsó elöre");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Orsó hátra");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Bekapcsolás");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Kikapcsolás");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Adagol");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Visszahúz");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Tengelyek mozgatása");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Ágy szintezés");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Ágy szintezése");
  PROGMEM Language_Str MSG_BED_TRAMMING                    = _UxGT("Sarok szint");
  PROGMEM Language_Str MSG_BED_TRAMMING_RAISE              = _UxGT("Ágy emelése a szonda váltásig");
  PROGMEM Language_Str MSG_BED_TRAMMING_IN_RANGE           = _UxGT("Minden sarok tolerancián belül. Szint jó.");
  PROGMEM Language_Str MSG_BED_TRAMMING_GOOD_POINTS        = _UxGT("Jó pontok: ");
  PROGMEM Language_Str MSG_BED_TRAMMING_LAST_Z             = _UxGT("Utolsó Z: ");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Következö sarok");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Háló szerkesztö");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Háló szerkesztése");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Háló szerk. állj");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Próbapont");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z érték");
  PROGMEM Language_Str MSG_CUSTOM_COMMANDS                 = _UxGT("Egyéni parancs");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Szonda teszt");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Pont");
  PROGMEM Language_Str MSG_M48_OUT_OF_BOUNDS               = _UxGT("Szonda határon kívül");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Eltérés");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX mód");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Eszköz eltolás");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Automata parkolás");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplikálás");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Tükrözött másolás");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Teljes felügyelet");
  PROGMEM Language_Str MSG_IDEX_DUPE_GAP                   = _UxGT("X-hézag másolása");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2. fej X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2. fej Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2. fej Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Szintezz! G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL eszköz");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Egységes ágy szint");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Döntési pont");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Kézi háló építés");
  PROGMEM Language_Str MSG_UBL_MESH_WIZARD                 = _UxGT("UBL Háló varázsló");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Tégy alátétet és mérj");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Mérés");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Üres ágyat mérj");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Továbblépés");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("UBL aktívál");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("UBL deaktívál");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Ágy höfok");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Egyéni ágy höfok");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Fejhöfok");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Egyéni fejhöfok");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Háló szerkesztés");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Egyéni háló szerkesztés");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Finomított háló");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Háló kész");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Egyéni háló építés");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Háló építés");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M                = _UxGT("Háló építés ($)");
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Hideg háló építés");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Háló magasság állítás");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Magasság összege");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Háló elfogadás");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M             = _UxGT("Háló elfogadás ($)");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Valódi háló elfogadása");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Ágy fütés");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Fej fütés");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Kézi alapozás...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Fix hosszúságú alap");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Alapozás kész");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Törölve");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Kilépö G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Ágy háló folyt.");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Háló szintezés");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Pontos szintezés");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Rács szintezés");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Háló szint");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Oldal pontok");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Térkép típus");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Háló térkép kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Host kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("CSV kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Nyomtató bizt.mentés");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("UBL infó kimenet");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Kitöltési költség");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Kézi kitöltés");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Okos kitöltés");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Háló kitöltés");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Minden érvénytelen");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Közelebbi érvénytelen");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Mindet finomhangolja");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Közelebbi finomhangolása");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Háló tárolás");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Memória foglalat");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Ágy háló betöltés");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Ágy háló mentés");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Háló %i betöltve");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Háló %i mentve");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Nincs tároló");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Hiba: UBL mentés");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Hiba: UBL visszaáll.");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-eltolás: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-eltolás leállítva");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Lépésröl lépésre UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. Hideg háló készítés");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. Inteligens kitöltés");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. Háló érvényesítés");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. Minden finomítása");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. Háló érvényesítés");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. Minden finomítása");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. Ágy háló mentése");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED vezérlés");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Világítás");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Beállított színek");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Piros");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Narancs");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Sárga");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Zöld");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Kék");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigó");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Viola");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Fehér");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Alapérték");
  PROGMEM Language_Str MSG_LED_CHANNEL_N                   = _UxGT("Csatorna =");
  PROGMEM Language_Str MSG_LEDS2                           = _UxGT("LED-ek #2");
  PROGMEM Language_Str MSG_NEO2_PRESETS                    = _UxGT("Fény #2 megadott");
  PROGMEM Language_Str MSG_NEO2_BRIGHTNESS                 = _UxGT("Fényerö");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Egyéni szín");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Piros intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Zöld intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Kék intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Fehér intenzitás");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Fényerö");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Mozgás...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("XY szabad");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("X mozgás");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Y mozgás");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Z mozgás");
  PROGMEM Language_Str MSG_MOVE_I                          = _UxGT("Mozgás ") LCD_STR_I;
  PROGMEM Language_Str MSG_MOVE_J                          = _UxGT("Mozgás ") LCD_STR_J;
  PROGMEM Language_Str MSG_MOVE_K                          = _UxGT("Mozgás ") LCD_STR_K;
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Adagoló");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Adagoló *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("A fej túl hideg");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Mozgás %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mozgás 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mozgás 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mozgás 10mm");
  PROGMEM Language_Str MSG_MOVE_100MM                      = _UxGT("Mozgás 100mm");
  PROGMEM Language_Str MSG_MOVE_0001IN                     = _UxGT("Mozgás 0.025mm");
  PROGMEM Language_Str MSG_MOVE_001IN                      = _UxGT("Mozgás 0.254mm");
  PROGMEM Language_Str MSG_MOVE_01IN                       = _UxGT("Mozgás 2.54mm");
  PROGMEM Language_Str MSG_MOVE_1IN                        = _UxGT("Mozgáá 25.4mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Sebesség");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z ágy");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Fej");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Fej ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Fej parkolva");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Fej készenlétbe");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Ágy");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Burkolat");
  PROGMEM Language_Str MSG_COOLER                          = _UxGT("Lézer hütövíz");
  PROGMEM Language_Str MSG_COOLER_TOGGLE                   = _UxGT("Hütö kapcsoló");
  PROGMEM Language_Str MSG_FLOWMETER_SAFETY                = _UxGT("Áramlásbiztonság");
  PROGMEM Language_Str MSG_LASER                           = _UxGT("Lézer");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Hütés sebesség");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Hütés sebesség =");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Tárolt hütés =");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Extra hütés sebesség");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Extra hütés sebesség =");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Hütésvezérlés");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Alapjárat");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Automatikus mód");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Aktív sebesség");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Készenlét");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Folyás");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Folyás ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Konfiguráció");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Minimum");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Maximum");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Tényezö");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Automata höfok");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Be");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Ki");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID hangolás");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID hangolás *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("PID hangolás kész");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Hangolási hiba. Rossz adagoló.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Hangolási hiba. Magas hömérséklet.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Hangolási hiba! Idötúllépés.");
  PROGMEM Language_Str MSG_PID_P                           = _UxGT("PID-P");
  PROGMEM Language_Str MSG_PID_P_E                         = _UxGT("PID-P *");
  PROGMEM Language_Str MSG_PID_I                           = _UxGT("PID-I");
  PROGMEM Language_Str MSG_PID_I_E                         = _UxGT("PID-I *");
  PROGMEM Language_Str MSG_PID_D                           = _UxGT("PID-D");
  PROGMEM Language_Str MSG_PID_D_E                         = _UxGT("PID-D *");
  PROGMEM Language_Str MSG_PID_C                           = _UxGT("PID-C");
  PROGMEM Language_Str MSG_PID_C_E                         = _UxGT("PID-C *");
  PROGMEM Language_Str MSG_PID_F                           = _UxGT("PID-F");
  PROGMEM Language_Str MSG_PID_F_E                         = _UxGT("PID-F *");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Kiválaszt");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Kiválaszt *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Gyorsítás");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Rántás");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Seb.") LCD_STR_A _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Seb.") LCD_STR_B _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Seb.") LCD_STR_C _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VI_JERK                         = _UxGT("Seb.") LCD_STR_I _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VJ_JERK                         = _UxGT("Seb.") LCD_STR_J _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VK_JERK                         = _UxGT("Seb.") LCD_STR_K _UxGT("-Rántás");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("E ránt. seb.");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Csomopont eltérés");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Sebesség");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Max Seb. ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Max Seb. ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Max Seb. ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_I                          = _UxGT("Max Seb. ") LCD_STR_I;
  PROGMEM Language_Str MSG_VMAX_J                          = _UxGT("Max Seb. ") LCD_STR_J;
  PROGMEM Language_Str MSG_VMAX_K                          = _UxGT("Max Seb. ") LCD_STR_K;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Max Seb. ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Max sebesség *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Min sebesség");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Min utazó.seb.");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Gyorsulás");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Max gyors. ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Max gyors. ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Max gyors. ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_I                          = _UxGT("Max gyors. ") LCD_STR_I;
  PROGMEM Language_Str MSG_AMAX_J                          = _UxGT("Max gyors. ") LCD_STR_J;
  PROGMEM Language_Str MSG_AMAX_K                          = _UxGT("Max gyors. ") LCD_STR_K;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Max gyors. ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Max gyorsulás *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Visszahúzás");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Utazás");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Max frekvencia");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Min elötolás");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Lépés/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_I_STEPS                         = LCD_STR_I _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_J_STEPS                         = LCD_STR_J _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_K_STEPS                         = LCD_STR_K _UxGT(" Lépés/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E lépés/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*Lépés/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Höfok");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Mozgatások");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Nyomtatószál");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E mm³-ben");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("E Limit mm³-ben");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("E Limit *");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Szál. átm.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Szál. átm. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Kiadás mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Betöltés mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Haladó K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Haladó K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD kontraszt");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Mentés EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Betöltés EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Alapértelmezett");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("EEPROM inicializálás");
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("Hiba: EEPROM CRC");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("Hiba: EEPROM index");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("Hiba: EEPROM verzió");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Beállítások mentve");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Tároló frissítés");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Nyomtató újraindítása");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Frissítés");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("<Infó képernyö>");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Vezérlés");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Hangolás");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Teljesítménymonitor");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Jelenlegi");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Feszültség");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Energia");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Nyomtatás indítása");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Tovább");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Kezdet");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Állj");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Nyomtatás");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Újraindítás");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Mellöz");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Mégse");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Kész");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Vissza");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Folytatás");
  PROGMEM Language_Str MSG_BUTTON_SKIP                     = _UxGT("Kihagy");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Szüneteltetve...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Nyomtatás szünetelés");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Nyomtatás folytatása");
  PROGMEM Language_Str MSG_HOST_START_PRINT                = _UxGT("Hoszt indítás");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Nyomtatás leállítása");
  PROGMEM Language_Str MSG_END_LOOPS                       = _UxGT("Hurok ismétlés vége");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Objektum nyomtatása");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Objektum törlése");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Objektum törlése =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Kiesés helyreáll.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Nyomtatás tárolóról");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Nincs tároló");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Alvás...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Katt a folytatáshoz...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Nyomtatás szünetelve");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Nyomtatás...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Nyomtatás leállítva");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Nyomtatás kész");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Nincs mozgás.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("HALOTT! ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("MEGÁLLT! ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Visszahúzás mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Visszahúzás cs. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Viszahúzás");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Ugrás mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Visszah.helyre mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Csere.visszah.helyre mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Visszahúzás V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S Vissza.h V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto visszah.");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Visszahúzás távolság");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Extra csere");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Tisztítási távolság");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Szerszámcsere");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z emelés");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Fösebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Visszah. sebesség");
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Fej parkolás");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Visszav.visszah. sebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("FAN sebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("FAN idö");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Auto BE");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Auto KI");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Szerszámcsere");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Automata csere");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Utolsó adagoló");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Csere *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Szálcsere");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Szálcsere *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Szál betöltés");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Szál betöltés *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Szál eltávolítás");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Szál eltávolítás *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Mindet eltávolít");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Tároló");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Tároló csere");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Tároló Kiadása");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z szonda tálcán kivül");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Ferdeség faktor");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Önteszt");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Visszaállítás");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Elhelyez");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Telepít");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Módok");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("BLTouch 5V mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("BLTouch OD mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Jelentés");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("VESZÉLY: A rossz beállítások kárt okozhatnak! Biztos továbblép?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Kezd TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Z eltolás teszt");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Mentés");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("TouchMI használ");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Z-Szonda telepítés");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Z-Szonda elhelyezés");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Elsö %s%s%s kell");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Szonda eltolások");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("X szonda eltolás");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Y szonda eltolás");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Z szonda eltolás");
  PROGMEM Language_Str MSG_MOVE_NOZZLE_TO_BED              = _UxGT("Fej az ágyhoz");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Mikrolépés X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Mikrolépés Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Mikrolépés Z");
  PROGMEM Language_Str MSG_BABYSTEP_I                      = _UxGT("Mikrolépés ") LCD_STR_I;
  PROGMEM Language_Str MSG_BABYSTEP_J                      = _UxGT("Mikrolépés ") LCD_STR_J;
  PROGMEM Language_Str MSG_BABYSTEP_K                      = _UxGT("Mikrolépés ") LCD_STR_K;
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Teljes");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Végállás megszakítva!");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Fütés hiba!");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Hiba: SZÜKSÉGTELEN HÖFOK");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("ÁGY FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("KAMRA FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_COOLER          = _UxGT("Hütés kimaradás");
  PROGMEM Language_Str MSG_COOLING_FAILED                  = _UxGT("Hütés sikertelen");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Hiba: MAX höfok");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Hiba: MIN höfok");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("A NYOMTATÓ LEÁLLT");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Indítsd újra!");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("n"); // Csak egy karakter
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("ó"); // Csak egy karakter
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("p"); // Csak egy karakter
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Fütés...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Hütés...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Ágy fütés...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Ágy hütés...");
  PROGMEM Language_Str MSG_PROBE_HEATING                   = _UxGT("Szonda fütése...");
  PROGMEM Language_Str MSG_PROBE_COOLING                   = _UxGT("Szonda hütése...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Kamra fütés...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Kamra hütés...");
  PROGMEM Language_Str MSG_LASER_COOLING                   = _UxGT("Lézer hütés...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta kalibráció");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("X kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Y kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Z kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Központ kalibrálás");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta beállítások");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto kalibráció");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Delta magasság kalib.");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Z eltolás");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag rúd");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Magasság");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Sugár");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("A Nyomtatóról");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Nyomtató infó");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-Pontos szintezés");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Lineáris szintezés");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Bilineáris szintezés");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Egységes ágy szintezés");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Háló szintezés");
  PROGMEM Language_Str MSG_MESH_DONE                       = _UxGT("Háló szintezés kész");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statisztikák");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Alaplap infó");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termisztorok");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Adagolók");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Átviteli sebesség");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokol");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Futáselemzés: KI");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Futáselemzés: BE");
  PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT             = _UxGT("Fej üresjárati idök.");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Munkalámpa");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Fényerösség");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("HELYTELEN NYOMTATÓ");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Nyomtatás számláló");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Befejezett");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Összes nyomtatási idö");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Leghosszabb munkaidö");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Összes anyag");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Nyomtatások");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Befejezett");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Összes");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Leghosszabb");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Kiadott");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min höfok");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max höfok");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Meghajtási erö");
  PROGMEM Language_Str MSG_DAC_PERCENT_A                   = LCD_STR_A _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_B                   = LCD_STR_B _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_C                   = LCD_STR_C _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_I                   = LCD_STR_I _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_J                   = LCD_STR_J _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_K                   = LCD_STR_K _UxGT(" Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E meghajtó %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC CSATLAKOZÁSI HIBA");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM írása");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("NYOMTATÓSZÁL CSERE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("NYOMTATÁS SZÜNETEL");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("SZÁL BETÖLTÉS");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("SZÁL ELTÁVOLÍTÁS");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("FOLYTATÁSI OPCIÓ:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Tisztítsd meg");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Folytatás");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Fej: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Túlfutás szenzor");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Túlfutás táv. mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Tájolási hiba");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Szondázás hiba");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("SZÁLVÁLASZTÁS");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("MMU szoftver feltöltése!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU figyelmeztetés.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Nyomtatás folytatása");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Folytatás...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Szál betöltése");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Összes betöltése");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Fej betöltése");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Szál kidobás");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Szál kidobás ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Kiadja a szálat");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Szál betölt. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Szál kidobás. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Szál kiadása....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Mind");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Nyomtatószál ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("MMU újraindítás");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("MMU újraindul...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Kidob, kattint");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Kever");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Összetevö =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Keverö");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Színátm.");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Teljes színátm.");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Váltás keverésre");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Ciklikus keverés");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Színátm. keverés");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Fordított színátm.");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Aktív V-szerszám");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("Kezdés V-szerszám");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("  Vége V-szerszám");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("Ál V-szerszám");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Újra V-szerszám");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Gyors V-szerszám Kev.");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-szersz. visszaáll.");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Kezdés Z:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("  Vége Z:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Játékok");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Maze");

  PROGMEM Language_Str MSG_BAD_PAGE                        = _UxGT("Rossz oldalindex");
  PROGMEM Language_Str MSG_BAD_PAGE_SPEED                  = _UxGT("Rossz oldalsebesség");

  PROGMEM Language_Str MSG_EDIT_PASSWORD                   = _UxGT("Jelszó szerkesztése");
  PROGMEM Language_Str MSG_LOGIN_REQUIRED                  = _UxGT("Belépés szükséges");
  PROGMEM Language_Str MSG_PASSWORD_SETTINGS               = _UxGT("Jelszóbeállítások");
  PROGMEM Language_Str MSG_ENTER_DIGIT                     = _UxGT("Írja be a számokat");
  PROGMEM Language_Str MSG_CHANGE_PASSWORD                 = _UxGT("Jelszó Beáll/Szerk");
  PROGMEM Language_Str MSG_REMOVE_PASSWORD                 = _UxGT("Jelszó törlése");
  PROGMEM Language_Str MSG_PASSWORD_SET                    = _UxGT("A jelszó ");
  PROGMEM Language_Str MSG_START_OVER                      = _UxGT("Újrakezdés");
  PROGMEM Language_Str MSG_REMINDER_SAVE_SETTINGS          = _UxGT("Mentsd el!");
  PROGMEM Language_Str MSG_PASSWORD_REMOVED                = _UxGT("Jelszó törölve");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Nyomj gombot", "nyomtatás folytatáshoz"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkolás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Várj míg", "szálcsere", "indítás"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Szál behelyezés", "majd nyomj gombot", "a folytatáshoz"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Nyomj gombot", "a fej fütéséhez"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Fej fütése", "Kérlek várj..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Várj a", "szál kiadására"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Várj a", "szál betöltésére"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Várj a", "szál tisztításra"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Kattints a készre", "szál tiszta"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Várj a nyomtatóra", "majd folytat..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Katt a folytatáshoz"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkolás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Kérlek várj..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Behelyez majd katt"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Katt a fütéshez"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Fütés..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Kiadás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Betöltés..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Tisztítás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Katt ha kész"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Folytatás..."));
  #endif
  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("TMC meghajtók");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Meghajtó áram");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hibrid küszöbérték");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Motoros kezdöpont");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Léptetö mód");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop mód");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Újraindítás");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" be:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Holtjáték");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_I                      = LCD_STR_I;
  PROGMEM Language_Str MSG_BACKLASH_J                      = LCD_STR_J;
  PROGMEM Language_Str MSG_BACKLASH_K                      = LCD_STR_K;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Korrekció");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Simítás");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("X tengely szint");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Önkalibrálás");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Tétlenségi idökorlát, a hömérséklet csökkent. Nyomd meg az OK gombot az ismételt felfütéshez, és újra a folytatáshoz.");
  #else
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Fütés idökorlátja");
  #endif
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Újrafüt");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Újrafütés...");

  PROGMEM Language_Str MSG_PROBE_WIZARD                    = _UxGT("Z szonda varázsló");
  PROGMEM Language_Str MSG_PROBE_WIZARD_PROBING            = _UxGT("Z referencia mérés");
  PROGMEM Language_Str MSG_PROBE_WIZARD_MOVING             = _UxGT("Menj a próba pontra");

  PROGMEM Language_Str MSG_SOUND                           = _UxGT("Hang");

  PROGMEM Language_Str MSG_TOP_LEFT                        = _UxGT("Bal felsö");
  PROGMEM Language_Str MSG_BOTTOM_LEFT                     = _UxGT("Bal alsó");
  PROGMEM Language_Str MSG_TOP_RIGHT                       = _UxGT("Jobb felsö");
  PROGMEM Language_Str MSG_BOTTOM_RIGHT                    = _UxGT("Jobb alsó");
  PROGMEM Language_Str MSG_CALIBRATION_COMPLETED           = _UxGT("Kalibrálás befejezve");
  PROGMEM Language_Str MSG_CALIBRATION_FAILED              = _UxGT("Kalibrálási hiba");

  PROGMEM Language_Str MSG_DRIVER_BACKWARD                 = _UxGT(" meghajtók hátra");

  PROGMEM Language_Str MSG_SD_CARD                         = _UxGT("SD Kártya");
  PROGMEM Language_Str MSG_USB_DISK                        = _UxGT("USB Lemez");
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
