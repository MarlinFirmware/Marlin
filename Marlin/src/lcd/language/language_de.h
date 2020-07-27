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
 * German
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */

namespace Language_de {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Deutsche");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" bereit");

  PROGMEM Language_Str MSG_YES                             = _UxGT("JA");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NEIN");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Zurück");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Abbruch...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Medium erkannt");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Medium entfernt");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Warten auf Medium");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Medium Lesefehler");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB Gerät entfernt");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB Start fehlge.");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstopp"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Software-Endstopp");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Hauptmenü");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Erw. Einstellungen");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfiguration");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Autostart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Motoren deaktivieren"); // M84 :: Max length 19 characters
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Debug-Menü");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Statusbalken-Test");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Z-Achsen ausgleichen");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klick zum Starten");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Nächste Koordinate");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Nivellieren fertig!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Ausblendhöhe");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Setze Homeversatz");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Homeversatz aktiv");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Setze Nullpunkte"); //"G92 X0 Y0 Z0" commented out in ultralcd.cpp
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = PREHEAT_1_LABEL _UxGT(" Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = PREHEAT_1_LABEL _UxGT(" Vorwärmen ~");
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = PREHEAT_1_LABEL _UxGT(" Extr. Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = PREHEAT_1_LABEL _UxGT(" Extr. Vorwärm. ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = PREHEAT_1_LABEL _UxGT(" Alles Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = PREHEAT_1_LABEL _UxGT(" Bett Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = PREHEAT_1_LABEL _UxGT(" Einstellungen");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("$ Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("$ Vorwärmen") " ~";
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("$ Extr. Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("$ Extr. Vorwärm. ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("$ Alles Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("$ Bett Vorwärmen");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("$ Einstellungen");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("benutzerdef. Heizen");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Abkühlen");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frequenz");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Laser");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Laser aus");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Laser an");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Laserleistung");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Spindel-Steuerung");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Spindel aus");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Spindel an");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Spindelleistung");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Spindelrichtung");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Netzteil ein");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Netzteil aus");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudieren");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Einzug");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Achsen bewegen");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Bett-Nivellierung");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Bett nivellieren");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Ecken nivellieren");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Nächste Ecke");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Netz Editor");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Netz bearbeiten");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Netzbearb. angeh.");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Messpunkt");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z-Wert");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Benutzer-Menü");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Berührungspunkt");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Sondentest");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Punkt");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Abweichung");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX-Modus");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Werkzeugversätze");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Autom. parken");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplizieren");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Spiegelkopie");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("vollstä. Kontrolle");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2. Düse X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2. Düse Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2. Düse Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("G29 ausführen");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL-Werkzeuge");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Netz manuell erst.");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Unterlegen & messen");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Messen");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Entfernen & messen");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Nächster Punkt...");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("UBL aktivieren");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("UBL deaktivieren");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Betttemperatur");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Betttemperatur");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Hotend-Temp.");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend-Temp.");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Netz bearbeiten");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Eigenes Netz bearb.");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Feineinstellung...");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Bearbeitung beendet");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Eigenes Netz erst.");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Netz erstellen");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("$ Netz erstellen");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("$ Netz validieren");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Netz erstellen kalt");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Netzhöhe einst.");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Höhe");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Netz validieren");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Eig. Netz validieren");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 heizt Bett");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Düse aufheizen");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Feste Länge Prime");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Priming fertig");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 abgebrochen");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("G26 verlassen");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Netzerst. forts.");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Netz-Nivellierung");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Punkt-Nivell.");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Gitternetz-Nivell.");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Netz nivellieren");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Eckpunkte");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Kartentyp");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Karte ausgeben");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Ausgabe für Host");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Ausgabe für CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Externe Sicherung");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("UBL-Info ausgeben");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Menge an Füllung");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Manuelles Füllen");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Cleveres Füllen");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Netz Füllen");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Alles annullieren");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Nächstlieg. ann.");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Feineinst. Alles");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Feineinst. Nächstl.");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Netz-Speicherplatz");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Speicherort");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Bettnetz laden");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Bettnetz speichern");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Netz %i geladen");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Netz %i gespeichert");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Kein Speicher");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Err:UBL speichern");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Err:UBL wiederherst.");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Versatz: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Versatz angehalten");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Schrittweises UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Netz erstellen kalt");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Cleveres Füllen");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Netz validieren");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Feineinst. Alles");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Netz validieren");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Feineinst. Alles");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Bettnetz speichern");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Licht-Steuerung");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Licht");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Licht-Einstellung");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Rot");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Orange");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Gelb");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Grün");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Blau");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Violett");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Weiß");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Standard");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Benutzerdefiniert");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Intensität Rot");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Intensität Grün");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Intensität Blau");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Intensität Weiß");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Helligkeit");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("In Bewegung...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Abstand XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Bewege X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Bewege Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Bewege Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Bewege Extruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Bewege Extruder *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Hotend zu kalt");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT(" %s mm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT(" 0,1   mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT(" 1,0   mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("10,0   mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Geschw.");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bett Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Düse");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Düse ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Düse geparkt");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bett");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Gehäuse");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Lüfter");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Lüfter ~");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Gespeich. Lüfter ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Geschw. Extralüfter");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Geschw. Extralüfter ~");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Lüfter Kontroller");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Lüfter Leerlauf");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Motorlast Modus");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Lüfter Motorlast");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Ausschalt Delay");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flussrate");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flussrate ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Einstellungen");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Faktor");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Auto Temperatur");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("an");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("aus");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Auswählen");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Auswählen *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Beschleunigung");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Jerk");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V ") LCD_STR_A _UxGT(" Jerk");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V ") LCD_STR_B _UxGT(" Jerk");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V ") LCD_STR_C _UxGT(" Jerk");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("V E Jerk");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Junction Dev");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Geschwindigkeit");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("V max ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("V max ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("V max ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("V max ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("V max *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("V min ");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("V min Leerfahrt");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Beschleunigung");
  PROGMEM Language_Str MSG_AMAX                            = _UxGT("A max "); // space intentional
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("A max ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("A max ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("A max ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("A max ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("A max *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A Einzug");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A Leerfahrt");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Steps/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" Steps/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" Steps/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" Steps/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = LCD_STR_E _UxGT(" Steps/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* Steps/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatur");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Bewegung");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Filamentdurchmesser");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Filamentdurchmesser *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Entladen mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Laden mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Vorschubfaktor");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Vorschubfaktor *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD-Kontrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Konfig. speichern");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Konfig. laden");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Standardwerte laden");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Werkseinstellungen");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Einstell. gespei.");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("FW Update vom Medium");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Drucker neustarten");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Aktualisieren");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Info");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Vorbereitung");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Justierung");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Starte Druck");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Weiter");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Bestätigen");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Stop");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Drucken");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Reseten");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Abbrechen");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Fertig");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Zurück");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Pause...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("SD-Druck pausieren");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("SD-Druck fortsetzen");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("SD-Druck abbrechen");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Objekt abbrechen");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Objekt abbrechen =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Wiederh. n. Stroma.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Druck vom Medium");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Kein Medium");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Warten...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Klick zum Fortsetzen");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Druck pausiert...");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Druckt...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Druck abgebrochen");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Druck fertig");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Motoren angeschaltet");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ABGEBROCHEN");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ANGEHALTEN");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Einzug mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Wechs. Einzug mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("V Einzug");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Z-Sprung mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Wechs. UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Autom. Einzug");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Einzugslänge");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Entladelänge");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Werkzeugwechsel");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z anheben");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Prime-Geschwin.");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Einzug-Geschwin.");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Düsen-Standby");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Filament wechseln");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Filament wechseln *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Filament laden");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Filament laden *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Filament entladen");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Filament entladen *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Alles entladen");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Medium initial.");  // Manually initialize the SD-card via user interface
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Medium getauscht"); // SD-card changed by user. For machines with no autocarddetect. Both send "M21"
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Medium freigeben"); // if Marlin gets confused - M22
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z-Sonde außerhalb");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Korrekturfaktor");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Selbsttest");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Zurücksetzen");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Ausfahren");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Modus");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Modus");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Modus");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Mode-Store");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Setze auf 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Setze auf OD");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Einfahren");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Modus: ");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("ACHTUNG: Falsche Einstellung - kann zu Beschädigung führen! Fortfahren?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("TouchMI initial.");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Test Z-Versatz");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Speichern");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("TouchMI ausfahren");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Z-Sonde ausfahren");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Z-Sonde einfahren");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Vorher %s%s%s homen");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Sondenversatz");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Sondenversatz X");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Sondenversatz Y");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Sondenversatz Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Total");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Abbr. mit Endstopp");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("HEIZEN ERFOLGLOS");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Bett heizen fehlge.");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_CHAMBER      = _UxGT("Geh. heizen fehlge.");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("REDUND. TEMP-ABWEI.");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = " " LCD_STR_THERMOMETER _UxGT(" NICHT ERREICHT");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("BETT") " " LCD_STR_THERMOMETER _UxGT(" NICHT ERREICHT");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("GEH.") " " LCD_STR_THERMOMETER _UxGT(" NICHT ERREICHT");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = " " LCD_STR_THERMOMETER _UxGT(" ÜBERSCHRITTEN");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = " " LCD_STR_THERMOMETER _UxGT(" UNTERSCHRITTEN");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("BETT ") LCD_STR_THERMOMETER _UxGT(" ÜBERSCHRITTEN");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("BETT ") LCD_STR_THERMOMETER _UxGT(" UNTERSCHRITTEN");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("Err:Gehäuse max Temp");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("Err:Gehäuse min Temp");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("DRUCKER GESTOPPT");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Bitte neustarten");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("t"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("heizt...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("kühlt...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Bett heizt...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Bett kühlt...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Gehäuse heizt...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Gehäuse kühlt...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta kalibrieren");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibriere X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibriere Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibriere Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibriere Mitte");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta Einst. anzeig.");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Autom. Kalibrierung");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Delta Höhe setzen");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Sondenversatz Z");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag Rod");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Höhe");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Radius");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Über den Drucker");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Drucker-Info");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-Punkt-Nivellierung");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Lineare Nivellierung");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Bilineare Nivell.");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Netz-Nivellierung");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Drucker-Statistik");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Board-Info");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Thermistoren");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extruder");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baudrate");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokoll");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Beleuchtung");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Helligkeit");

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Falscher Drucker");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Gesamte Drucke");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Komplette Drucke");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Gesamte Druckzeit");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Längste Druckzeit");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Gesamt Extrudiert");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Drucke");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Komplette");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Gesamte");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Längste");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrud.");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Temp");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Temp");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Netzteil");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Motorleistung");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Treiber %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Treiber %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Treiber %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Treiber %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC Verbindungsfehler");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Werte speichern");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("FILAMENT WECHSEL");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("DRUCK PAUSIERT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("FILAMENT LADEN");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("FILAMENT ENTLADEN");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("FORTS. OPTIONEN:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Mehr entladen");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Druck weiter");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Düse: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Runout-Sensor");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Runout-Weg mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Homing gescheitert");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Probing gescheitert");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: zu kalt");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("FILAMENT WÄHLEN");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Update MMU Firmware!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU handeln erfor.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Druck fortsetzen");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Fortfahren...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Filament laden");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Lade alle");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Düse laden");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Filament auswerfen");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Filament ~ auswerfen");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Filament entladen ");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Lade Fila. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Fila. auswerfen...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Fila. entladen...");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Alle");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filament ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("setze MMU zurück");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("MMU zurücksetzen...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Entfernen, klicken");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Mix");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Komponente ~");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Mixer");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradient"); // equal Farbverlauf
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Volle Gradient");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Mix umschalten");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Zyklus Mix");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Gradient Mix");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Umgekehrte Gradient");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Aktives V-Tool");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("V-Tool Start");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("V-Tool Ende");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("V-Tool Alias");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("V-Tools Reseten");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("V-Tool Mix sichern");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-Tools ist resetet");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Z Start:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("Z Ende:");
  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Spiele");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Maze");

  //
  // Die Filament-Change-Bildschirme können bis zu 3 Zeilen auf einem 4-Zeilen-Display anzeigen
  //                                       ...oder 2 Zeilen auf einem 3-Zeilen-Display.
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Knopf drücken um", "Druck fortzusetzen"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_2_LINE("Druck ist", "pausiert..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Warte auf den", "Start des", "Filamentwechsels..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Filament einlegen", "und Knopf drücken", "um fortzusetzen"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Knopf drücken um", "Düse aufzuheizen"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Düse heizt auf", "bitte warten..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Warte auf", "Herausnahme", "des Filaments..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Warte auf", "Laden des", "Filaments..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_3_LINE("Warte auf", "Entladen des", "Filaments..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_3_LINE("Klicke um", "die Fila-Entladung", "zu beenden"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Warte auf", "Fortsetzen des", "Drucks..."));
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Klick zum Fortsetzen"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Pausiert..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Bitte warten..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Laden und Klick"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Klick zum Heizen"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Heizen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Entladen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Laden..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Entladen..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Klick zum beenden", "der Fila-Entladung"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Fortsetzen..."));
  #endif // LCD_HEIGHT < 4

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("TMC Treiber"); // Max length 18 characters
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Treiber Strom");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hybrid threshold");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Sensorloses Homing");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Schrittmodus");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop einsch.");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Reset");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" im:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Spiel");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Korrektur");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Glätten");
  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("X Achse leveln");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Auto. Kalibiren");
  PROGMEM Language_Str MSG_HEATER_TIMEOUT                  = _UxGT("Heizung Timeout");
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Erneut aufheizen");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Erneut aufhei. ...");
}
