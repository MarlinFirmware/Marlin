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
 * Magyar
 *
 * LCD Menü Üzenetek. Lásd még https://marlinfw.org/docs/development/lcd_language.html
 * Marlin 2.0.x bugfix Magyar fordítása. A fordítást folyamatosan javítom és frissítem.
 * A Magyar fordítást készítette: AntoszHUN
 *
 *
 *
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
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Tároló Behelyezve");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Tároló Eltávolítva");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Várakozás a tárolóra");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Tároló olvasási hiba");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB eltávolítva");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB eszköz hiba");
  PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW           = _UxGT("Túlfolyás");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Végállás"); // Maximum 8 karakter
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Szoft. Végállás");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("<Fömenü>");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("További Beállítások");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfiguráció");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autoinditás");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Motorok kikapcsolása");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Hiba Menü");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Haladás sáv teszt");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("XYZ Auto kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("X Kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Y Kezdöpont");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Z Kezdöpont");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto Z-Igazítás");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("XYZ Kezdöpont");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Kattints a kezdéshez.");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Következö Pont");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Szintezés Kész!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Szint Csökkentés");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Kezdöpont eltolás");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Eltolás beállítva.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Eredeti Be");
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
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Egyedi Elömelegítés");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Visszahütés");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frekvencia");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Lézer Vezérlés");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Lézer Ki");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Lézer Be");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Lézer Teljesítmény");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Orsó Vezérlés");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Orsó Ki");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Orsó Be");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Orsó Teljesítmény");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Orsó Hátra");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Bekapcsolás");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Kikapcsolás");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudál");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Visszahúz");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Tengelyek Mozgatása");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Ágy Szintezés");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Ágy szintezése");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Sarok szint");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Következö sarok");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Háló Szerkesztö");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Háló Szerkesztése");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Háló Szerk. Állj");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Próbapont");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z Érték");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Egyéni Parancs");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Probe Teszt");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Pont");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Eltérés");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX Mód");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Eszköz Eltolás");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Parkolás");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplikálás");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Tükrözött másolás");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Teljes felügyelet");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2. fúvóka X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2. fúvóka Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2. fúvóka Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Szintezz! G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL Eszköz");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Egységes Ágy Szint");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Döntési Pont");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Kézi Háló Építés");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Tégy alátétet és mérj");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Mérés");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Üres ágyat mérj");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Továbblépés");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("UBL Aktivál");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("UBL Deaktivál");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Ágy Höfok");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Ágy Höfok");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Fúvóka Höfok");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Fúvóka Höfok");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Háló Szerkesztés");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Egyéni Háló Szerkesztés");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Finomított Háló");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Háló Kész");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Egyéni Háló Építés");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Háló Építés");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Háló Építés ($)");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Háló Elfogadás ($)");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Hideg Háló Építés");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("AHáló Magasság Állítása");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Összmagasság");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Háló Elfogadás");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Valódi Háló Elfogadása");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Ágy Fűtés");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Fúvóka Fűtés");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Kézi alapozás...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Fix hosszúságú alap");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Alapozás Kész");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Törölve");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Kilépö G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Ágy Háló Folyt.");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Háló Szintezés");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Pontos Szintezés");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Rács Szintezés");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Háló Szint");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Oldal pontok");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Térkép Típus");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Háló Térkép Kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Host Kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("CSV Kimenet");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Nyomtató Backup Ki");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("UBL Infó Kimenet");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Kitöltési Költség");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Kézi Kitöltés");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Okos Kitöltés");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Háló Kitöltés");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Minden Érvénytelen");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Közelebbi Érvénytelen");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Mindet Finomhangolja");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Közelebbi Finomhangolása");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Háló Tárolás");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Memória Foglalat");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Ágy háló Betöltés");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Ágy háló Mentés");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Háló %i Betöltve");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Háló %i Mentve");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Nincs tároló");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Hiba: UBL Mentés");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Hiba: UBL Visszaáll");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Eltolás: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Eltolás Leállítva");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Lépésröl Lépésre UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. Hideg Háló Készítés");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. Inteligens Kitöltés");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. Háló Érvényesítés");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. Minden Finomítása");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. Háló Érvényesítés");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. Minden Finomítása");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. Ágy Háló Mentése");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED Vezérlés");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Világítás");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Beállított Színek");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Piros");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Narancs");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Sárga");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Zöld");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Kék");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigó");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Viola");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Fehér");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Alapérték");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Egyéni Szín");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Piros Intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Zöld Intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Kék Intenzitás");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Fehér Intenzitás");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Fényerö");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Mozgás...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("XY Szabad");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("X Mozgás");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Y Mozgás");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Z Mozgás");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Adagoló");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Adagoló *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("A fúvóka túl hideg");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mozgás %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mozgás 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mozgás 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mozgás 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Sebesség");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z ágy");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Fúvóka");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Fúvóka ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Fej Parkolva");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Fej Készenlétbe");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Ágy");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Burkolat");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Hütés sebesség");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Hütés sebesség =");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Tárolt Hütés =");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Extra hütés sebesség");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Extra hütés sebesség =");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Hűtésvezérlés");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Alapjárat");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Automatikus Mód");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Aktív Sebesség");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Készenlét");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Folyás");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Folyás ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Konfiguráció");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Minimum");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Maximum");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Tényezö");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Automata Höfok");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Be");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Ki");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID Hangolás");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID Hangolás *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("PID hangolás kész");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Hangolási hiba. Rossz Adagoló.");
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
  PROGMEM Language_Str MSG_VA_JERK                         = LCD_STR_A _UxGT(" Ránt. Seb.");
  PROGMEM Language_Str MSG_VB_JERK                         = LCD_STR_B _UxGT(" Ránt. Seb.");
  PROGMEM Language_Str MSG_VC_JERK                         = LCD_STR_C _UxGT(" Ránt. Seb.");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("E Ránt. Seb.");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Csomopont Eltérés");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Sebesség");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Max Sebesség ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Max Sebesség ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Max Sebesség ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Max Sebesség ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Max Sebesség *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Min Sebesség");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Min Utazó.seb.");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Gyorsulás");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Max Gyors. ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Max Gyors. ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Max Gyors. ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Max Gyors. ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Max Gyorsulás *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Visszahúzás");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Utazás");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Max Frekvencia");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Min Elötolás");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Lépés/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" lépés/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" lépés/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" lépés/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E lépés/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*lépés/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Höfok");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Mozgatások");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Nyomtatószál");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E mm³-ben");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Szál. Átm.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Szál. Átm. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Kiadás mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Betöltés mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Haladó K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Haladó K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD kontraszt");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Mentés EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Betöltés EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Alapértelmezett");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("EEPROM Inicializálás");
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("Hiba: EEPROM CRC");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("Hiba: EEPROM Index");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("Hiba: EEPROM Verzió");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Beállítások Mentve");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Tároló Frissítés");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Nyomtató Újraindítása");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Frissítés");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("<Infó Képernyö>");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Vezérlés");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Hangolás");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Nyomtatás Indítása");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Tovább");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Kezdet");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Állj");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Nyomtatás");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Újraindítás");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Mégse");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Kész");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Vissza");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Folytatás");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Szüneteltetve...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Nyomtatás Szünetelés");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Nyomtatás folytatása");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Nyomtatás leállítása");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Objektum Nyomtatása");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Objektum Törlése");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Objektum Törlése =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Kiesés Helyreáll.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Nyomtatás Tárolóról");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Nincs Tároló");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Alvás...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Katt a folytatáshoz...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Nyomtatás szünetelve");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Nyomtatás...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Nyomtatás leállítva");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Nyomtatás Kész");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Nincs mozgás.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("HALOTT! ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("MEGÁLLT! ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Visszahúzás mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Visszahúzás Cs. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Viszahúzás");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Ugrás mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Visszah.helyre mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Csere.Visszah.helyre mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("AutoVisszah.");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Visszahúzás Távolság");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Extra Csere");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Tisztítási Távolság");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Szerszámcsere");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z Emelés");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Fösebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Visszah. Sebesség");
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Fej Parkolás");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Visszahúzás Sebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("FAN Sebesség");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("FAN idö");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Auto BE");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Auto KI");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Szerszámcsere");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Automata Csere");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Utolsó Adagoló");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Csere *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Szál csere");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Szál csere *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Szál betöltés");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Szál betöltés *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Szál eltávolítás");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Szál eltávolítás *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Mindet Eltávolít");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Tároló");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Tároló csere");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Tároló Kiadása");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z szonda tálcán kivül");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Ferdeség Faktor");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Önteszt");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Visszaállítás");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Elhelyez");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Telepít");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Módok");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("BLTouch 5V Mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("BLTouch OD Mód");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Jelentés");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("VESZÉLY: A rossz beállítások kárt okozhatnak! Biztos továbblép?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Kezd TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Z Offset Teszt");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Mentés");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("TouchMI Használ");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Z-Probe Használ");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Z-Probe Elhelyezés");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Elsö %s%s%s Kell");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Szonda Eltolások");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Szonda X Eltolás");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Szonda Y Eltolás");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Szonda Z Eltolás");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Mikrolépés X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Mikrolépés Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Mikrolépés Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Teljes");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Végállás megszakítva!");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Fütés hiba!");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Hiba: SZÜKSÉGTELEN HÖFOK");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("ÁGY FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("KAMRA FÜTÉS KIMARADÁS");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Hiba: MAX Höfok");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Hiba: MIN Höfok");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Hiba: MAX ÁGY HÖFOK");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Hiba: MIN ÁGY HÖFOK");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("Hiba: MAX KAMRA HÖFOK");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("Hiba: MIN KAMRA HÖFOK");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("A NYOMTATÓ LEFAGYOTT");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Indítsd újra!");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("n"); // Csak egy karakter
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("ó"); // Csak egy karakter
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("p"); // Csak egy karakter
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Fütés...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Hütés...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Ágy fütés...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Ágy hütés...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Kamra fütés...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Kamra hütés...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibráció");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("X Kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Y Kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Z Kalibrálás");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Központ Kalibrálás");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta Beállítások");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Kalibráció");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Delta Magasság Kalib.");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Z Szonda Eltolás");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag Rúd");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Magasság");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Sugár");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("A Nyomtatóról");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Nyomtató Infó");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-Pontos Szintezés");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Lineáris Szintezés");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Bilineáris Szintezés");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Egységes Ágy Szintezés");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Háló Szintezés");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statisztikák");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Alaplap Infó");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termisztorok");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Adagolók");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Átviteli sebesség");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokoll");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Futáselemzés: KI");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Futáselemzés: BE");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Munkalámpa");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Fényerösség");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("HELYTELEN NYOMTATÓ");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Nyomtatás Számláló");
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

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Höfok");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Höfok");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Meghajtási Erö");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Meghajtó %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Meghajtó %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC CSATLAKOZÁSI HIBA");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM Írása");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("NYOMTATÓSZÁL CSERE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("NYOMTATÁS SZÜNETEL");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("SZÁL BETÖLTÉS");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("SZÁL ELTÁVOLÍTÁS");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("FOLYTATÁSI OPCIÓ:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Tisztítsd meg");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Folytatás");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Fúvóka: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Túlfutás Szenzor");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Túlfutás Táv. mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Tájolási hiba");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Szondázás hiba");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Túl hideg");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("SZÁLVÁLASZTÁS");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("MMU Szoftver Feltöltése!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU Figyelmeztetés.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Nyomtatás Folytatása");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Folytatás...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Szál Betöltése");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Összes Betöltése");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Fúvóka Betöltése");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Szál Kiadása");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Szál Kiadása ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Kiadja a szálat");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Szál betölt. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Szál kiadás....");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Szál kiadása....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Mind");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Nyomtatószál ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("MMU Újraindítás");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("MMU Újraindul...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Eltávolít, kattint");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Kever");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Összetevö =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Keverö");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Színátm.");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Teljes Színátm.");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Váltás Keverésre");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Ciklikus Keverés");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Színátm. Keverés");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Fordított Színátm.");
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

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Nyomj gombot", "nyomtatás folytatáshoz"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkolás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Várj míg", "szál csere", "indítás"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Szál behelyezés", "majd nyomj gombot", "a folytatáshoz"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Nyomj gombot", "a fúvóka fűtéséhez"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Fúvóka fűtése", "Kérlek várj..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Várj a", "szál kiadására"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Várj a", "szál betöltésére"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Várj a", "szál tisztításra"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Kattints a készre", "szál tiszta"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Várj a nyomtatóra", "majd foltyat..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Katt a folytatáshoz"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkolás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Kérlek Várj..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Behelyez majd katt"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Katt a fűtéshez"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Fűtés..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Kiadás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Betöltés..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Tisztítás..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Katt ha kész"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Folytatás..."));
  #endif
  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("TMC Meghajtók");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Meghajtó áram");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hibrid Küszöbérték");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Motoros Kezdöpont");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Léptetö Mód");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop Mód");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Újraindítás");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" be:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Holtjáték");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Korrekció");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Simítás");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("X Tengely Szint");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Önkalibrálás");
  PROGMEM Language_Str MSG_HEATER_TIMEOUT                  = _UxGT("Fűtéskimaradás");
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Újrafűt");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Újrafűtés...");
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
