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
 * GNU General Public License för more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Swedish
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_sv {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Swedish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Redo.");
  PROGMEM Language_Str MSG_MARLIN                          = _UxGT("Marlin");
  PROGMEM Language_Str MSG_YES                             = _UxGT("JA");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NEJ");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Bakåt");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Avbryter...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Media Instatt");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Media Borttaget");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Väntar på media");
  PROGMEM Language_Str MSG_SD_INIT_FAIL                    = _UxGT("SD init misslyckades");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Media läsningsfel");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB enhet borttagen");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB start misslyckad");
  PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW           = _UxGT("Underanrop överskriden");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Slutstop"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Mjuk slutstopp");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Huvud");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Advancerade inställningar");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Konfiguration");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Autostarta Filer");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Inaktivera Stegare");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Debug Meny");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Framstegsindikator Test");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Auto Hem");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Hem X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Hem Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Hem Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto Z-Justering");
  PROGMEM Language_Str MSG_ITERATION                       = _UxGT("G34 Iteration: %i");
  PROGMEM Language_Str MSG_DECREASING_ACCURACY             = _UxGT("Noggrannhet Minskar!");
  PROGMEM Language_Str MSG_ACCURACY_ACHIEVED               = _UxGT("Noggrannhet uppnådd");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Hemning XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klicka för att börja");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Nästa Punkt");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Nivellering Färdig!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Falna Höjd");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Sätt Hem Offset");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offset Tillämpad");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Sätt Origo");
  PROGMEM Language_Str MSG_TRAMMING_WIZARD                 = _UxGT("Justerings Wizard");
  PROGMEM Language_Str MSG_SELECT_ORIGIN                   = _UxGT("Välj Origo");
  PROGMEM Language_Str MSG_LAST_VALUE_SP                   = _UxGT("Senaste värde ");

  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Förvärmning ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Förvärmning ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Stoppa");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Stoppa ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Alla");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Bädd");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Konf");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Förvärmning $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Förvärmning $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Förvärmning $ Stoppa");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Förvärmning $ Stoppa ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Förvärmning $ Alla");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Förvärmning $ Bädd");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Förvärmning $ Donf");
  #endif

  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Förvärmning Anpassad");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Nedkylning");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frekvens");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Laser kontroll");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Spindel Kontroll");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Laser Styrka");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Spindel Styrka");
  PROGMEM Language_Str MSG_LASER_TOGGLE                    = _UxGT("Växla Laser");
  PROGMEM Language_Str MSG_LASER_PULSE_MS                  = _UxGT("Test Puls ms");
  PROGMEM Language_Str MSG_LASER_FIRE_PULSE                = _UxGT("Avfyra Puls");
  PROGMEM Language_Str MSG_SPINDLE_TOGGLE                  = _UxGT("Växla Spindel");
  PROGMEM Language_Str MSG_SPINDLE_FORWARD                 = _UxGT("Spindel Framåt");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Spindel Bakåt");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Laser Av");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Laser På");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Spindel Av");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Spindel På");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Sätt på ström");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Stäng av ström");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudera");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Dra tillbaka");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Flytta Axel");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Bädd Nivellering");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Nivellera Bädd");
  PROGMEM Language_Str MSG_BED_TRAMMING                    = _UxGT("Bädd Justering");
  PROGMEM Language_Str MSG_BED_TRAMMING_RAISE              = _UxGT("Höj Bädd tills nästa Sond Triggad");
  PROGMEM Language_Str MSG_BED_TRAMMING_IN_RANGE           = _UxGT("Alla Hörn inom Tolerans. Nivellering Bädd");
  PROGMEM Language_Str MSG_BED_TRAMMING_GOOD_POINTS        = _UxGT("Bra Punkter: ");
  PROGMEM Language_Str MSG_BED_TRAMMING_LAST_Z             = _UxGT("Senaste Z: ");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Nästa Hörn");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Nät Redigerare");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Redigera Nät");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Nätredigering Stoppad");
  PROGMEM Language_Str MSG_PROBING_POINT                   = _UxGT("Sonderingspunkt");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z Värde");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Anpassade Kommandon");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Sond Test");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Punkt");
  PROGMEM Language_Str MSG_M48_OUT_OF_BOUNDS               = _UxGT("Sond utan för gränser");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Avvikelse");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX Läge");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Verktygsoffset");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Parkera");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplicering");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Speglad Kopia");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Full Kontroll");
  PROGMEM Language_Str MSG_IDEX_DUPE_GAP                   = _UxGT("Duplicera X-Avstånd");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2:a Munstycke X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2:a Munstycke Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2:a Munstycke Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Utför G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL Verktyg");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Enad Bädd Nivellering (UBL)");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Lutningspunkt");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Manuellt skapa nät");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Placera Shim & Mät");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Mät");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Ta bort & Mät bädd");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Flyttar till nästa");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Aktivera UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Avaktivera UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Bädd Temp");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bädd Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Hetände Temp");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hetände Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Nät Redigera");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Redigera Anpassat Nät");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Finjustera Nät");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Färdig Redigera Nät");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Bygg Anpassat Nät");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Bygg Nät");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M                = _UxGT("Bygg Nät ($)");
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Bygg Kallt Nät");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Justera Nät Höjd");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Höjd Antal");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Validera Nät");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M             = _UxGT("Validera Nät ($)");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Validera Anpassat Nät");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Värma Bädd");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Värma Munstycke");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Manuel grundning...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Fastlängd Grundning");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Färdig Grundning");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Avbruten");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Nivellera G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Fortsätt Bädd Nät");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Nät Nivellering");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Punkts Nivellering");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Rutnät Nivellering");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Nivellera Nät");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Sidopunkter");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Kart Typ");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Utmatning Nät Map");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Utmatning för Värd");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Utmatning för CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Utanför skrivare Backup");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Utmatning UBL Info");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Ifyllnad Mängd");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Manuell Ifyllnad");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Smart Ifyllnad");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Ifyllnad Nät");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Ogiltigförklara Alla");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Ogiltigförklara Närmast");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Finjustera Alla");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Finjustera Närmast");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Nät Lagra");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Minnesöppning");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Ladda Bädd Nät");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Spara Bädd Nät");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Nät %i Ladda");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Nät %i Sparad");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Ingen Lagring");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Fel: UBL Sparad");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Fel: UBL Återställd");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Offset: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Offset Stoppad");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Steg-för-Steg UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. Bygg Kallt Nät");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. Smart Ifyllnad");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. Validera Nät");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. Finjustera Alla");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. Validera Nät");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. Finjustera Alla");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. Spara Bädd Nät");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED Kontroll");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Ljus");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Ljus Förinställd");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Röd");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Orange");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Gul");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Grön");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Blå");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Violet");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Vitt");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Standard");
  PROGMEM Language_Str MSG_LED_CHANNEL_N                   = _UxGT("Kanal =");
  PROGMEM Language_Str MSG_LEDS2                           = _UxGT("Ljus #2");
  PROGMEM Language_Str MSG_NEO2_PRESETS                    = _UxGT("Ljus #2 Förinställd");
  PROGMEM Language_Str MSG_NEO2_BRIGHTNESS                 = _UxGT("Ljusstyrka");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Anpassat Ljus");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Rör Intensitet");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Grön Intensitet");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Blå Intensitet");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Vit Intensitet");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Brightness");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Flyttar...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Fri XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Flytta X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Flytta Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Flytta Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extruder *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Hetände för kall");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Flytta %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Flytta 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Flytta 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Flytta 10mm");
  PROGMEM Language_Str MSG_MOVE_100MM                      = _UxGT("Flytta 100mm");
  PROGMEM Language_Str MSG_MOVE_0001IN                     = _UxGT("Flytta 0.001tum");
  PROGMEM Language_Str MSG_MOVE_001IN                      = _UxGT("Flytta 0.01tum");
  PROGMEM Language_Str MSG_MOVE_01IN                       = _UxGT("Flytta 0.1tum");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Hastighet");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bädd Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Munstycke");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Munstycke ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Munstycke Parkerad");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Munstycke Standby");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bädd");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Inkapsling");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Fläkt Hastighet");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Fläkt Hastighet ~");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Lagrad Fläkt ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Extra Fläkt Hastighet");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Extra Fläkt Hastighet ~");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Kontroller Fläkt");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Overksam Hastighet");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Auto läga");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Aktive Hastighet");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Overksam Period");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flöde");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flöde ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Kontroll");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fakt");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("På");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Av");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID Autojustera");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID Autojustera *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("PID tuning done");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Autojustera misslyckad. Dålig extruder.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Autojustera misslyckad. Temperatur för hög.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Autojustera misslyckad! Tidsgräns.");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Välj");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Välj *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Accel");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Ryck");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VI_JERK                         = _UxGT("V") LCD_STR_I _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VJ_JERK                         = _UxGT("V") LCD_STR_J _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VK_JERK                         = _UxGT("V") LCD_STR_K _UxGT("-Ryck");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-Ryck");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Knutpunkt Avv");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Hastighet");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vmax ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vmax ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vmax ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_I                          = _UxGT("Vmax ") LCD_STR_I;
  PROGMEM Language_Str MSG_VMAX_J                          = _UxGT("Vmax ") LCD_STR_J;
  PROGMEM Language_Str MSG_VMAX_K                          = _UxGT("Vmax ") LCD_STR_K;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vmax ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vmax *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vmin");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VTrav Min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Acceleration");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Amax ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Amax ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Amax ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_I                          = _UxGT("Amax ") LCD_STR_I;
  PROGMEM Language_Str MSG_AMAX_J                          = _UxGT("Amax ") LCD_STR_J;
  PROGMEM Language_Str MSG_AMAX_K                          = _UxGT("Amax ") LCD_STR_K;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Amax ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Amax *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-Dra tillbaka");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-Färdas");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Frekvens max");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Flöde min");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Steg/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_I_STEPS                         = LCD_STR_I _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_J_STEPS                         = LCD_STR_J _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_K_STEPS                         = LCD_STR_K _UxGT(" Steg/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E Steg/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* Steg/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatur");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Rörelse");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Tråd");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E i mm³");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("E Gräns i mm³");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("E Gräns *");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Tråd Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Tråd Dia. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Lossa mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Ladda mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Advancera K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Advancera K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD Kontrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Spara Inställningar");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Ladda Inställningar");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Återställ Standard");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Initiera EEPROM");
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("EEPROM CRC Fel");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("EEPROM Index Fel");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("EEPROM Version Fel");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Inställningar Lagrad");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Media Uppdatera");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Återställ Skrivare");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH _UxGT("Uppdatera");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Info Skärm");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Förbered");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Justera");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Ström övervakning");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Ström");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Spänning");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Ström");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Start Utskrift");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Nästa");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Initiera");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Stoppa");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Skriv");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Återställa");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Ignorera");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Avbryt");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Färdig");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Bakåt");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Fortsätt");
  PROGMEM Language_Str MSG_BUTTON_SKIP                     = _UxGT("Hoppa över");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Paus..");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausera Utskrift");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Återuppta Utskrift");
  PROGMEM Language_Str MSG_HOST_START_PRINT                = _UxGT("Värd Start");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Stoppa Utskrift");
  PROGMEM Language_Str MSG_END_LOOPS                       = _UxGT("Slut Upprepningsloop");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Skriver Objekt");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Avbryt Objekt");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Avbryt Objekt =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Ström Avbrott");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Skriv fråm Media");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Inget Media");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Sov...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Klick för att återuppta...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Utskrift Pausad");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Skriver...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Utskrift Avbruten");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Utskrift Färdig");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Ingen Flytt.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("DÖDAD. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("STOPPAD. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Dra tillbaka mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Byt Dra.mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Dra tillbaka V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hoppa mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Åter dra tillbaka. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Byt åter dra t. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Återdrat. V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("Byt åter dra. V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto-Dra-tillbka");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Byt Längd");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Byt Extra");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Rensa Längd");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Byt verktyg");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z Höj");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Grund Hastighet");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Återgå Hastighet");
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Parkera Huvud");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Återgår Hastighet");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("Fläkt Hastighet");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("Fläkt Tid");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Auto PÅ");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Auto AV");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Verktyg Migration");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Auto-migration");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Senast Extruder");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Migrera till *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Byt Tråd");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Byt Tråd *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Ladda Tråd");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Ladda *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Lossa Tråd");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Lossa *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Lossa All");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Bifoga Media");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Byt Media");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Släpp Media");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z Sond Utanför Bädd");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Skev Faktor");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Själv-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Återställ");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Stuva undan");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Fällut");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Läge");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Läge");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Läge");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Läge-Lägring");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Sätt BLTouch to 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Sätt BLTouch to OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Reportera Dränering");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("FARA: Dålig inställningar kan orsaka skada! Fortsätt ändå?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Initiera TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Z Offset Test");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Spara");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Fällut TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Fällut Z-Sond");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Stuva undan Z-Sond");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Hem %s%s%s Först");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Sond Offsets");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Sond X Offset");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Sond Y Offset");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Sond Z Offset");
  PROGMEM Language_Str MSG_MOVE_NOZZLE_TO_BED              = _UxGT("Flytta Munstycke till Bädd");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Småsteg X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Småsteg Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Småsteg Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Total");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Slutstopp Avbrott");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Värma Misslyckad");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Fel: REDUNDANT TEMP");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("TERMISK ÖVERDRIFT");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("BÄDD TERMISK ÖVERDRIFT");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("KAMMARE T. ÖVERDRIFT");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Fel: MAXTEMP");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Fel: MINTEMP");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("Utskrift stoppad");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Snälla Återställ");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("t"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Värmer...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Kyler...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Bädd Värmer...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Bädd Kyler...");
  PROGMEM Language_Str MSG_PROBE_HEATING                   = _UxGT("Sond Värmer...");
  PROGMEM Language_Str MSG_PROBE_COOLING                   = _UxGT("Sond Kyler...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Kammare Värmer...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Kammare Kyler...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibrering");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibrera X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibrera Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibrera Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibrera Center");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta Inställningar");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Kalibrering");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Sätt Delta Höjd");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Sond Z-offset");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag Rod");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Höjd");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Radius");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Om Skrivaren");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Skrivare Info");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-Punkt Nivellering");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Linjär Nivellering");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Bilinjär Nivellering");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Enhetlig Bädd Nivellering (UBL)");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Nät Nivellering");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Skrivar Stats");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Kort Info");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistor");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extruderare");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokoll");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Överdrift Övervakning: AV");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Överdrift Övervakning: PÅ");
  PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT             = _UxGT("Hetände Overksam Tidsgräns");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Lådljus");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Ljus ljusstyrka");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("INKORREKT SKRIVARE");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Utskriftsantal");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Färdiga");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total Utskriftstid");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Längsta Jobbtid");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extruderade Totalt");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Utskrift");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Färdig");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Längsta");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extruderad");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Temp");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Temp");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Driv Styrka");
  PROGMEM Language_Str MSG_DAC_PERCENT_A                   = LCD_STR_A _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_B                   = LCD_STR_B _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_C                   = LCD_STR_C _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_I                   = LCD_STR_I _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_J                   = LCD_STR_J _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_K                   = LCD_STR_K _UxGT(" Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Driver %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC KOPPLNINGSFEL");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM Skriv");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("TRÅDBYTE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("UTSKRIFTSPAUSERAD");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("LADDA TRÅD");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("LOSSA TRÅD");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("ÅTERGÅ VAÖ:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Rensa mer");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Fortsätt");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Munstycke: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Utskjut Sensor");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Utskjut Dist mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Hemning Misslyckad");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Sondering Misslyckad");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("VÄLJ TRÅD");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Uppdatera MMU Firmware!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU Behöver uppmärksamhet.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("MMU Återuppta");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("MMU Återupptas...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("MMU Ladda");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("MMU Ladda Alla");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("MMU Ladda till Munstycke");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("MMU Mata ut");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("MMU Mata ut ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("MMU Lossa");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Ladda Tråd %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Mata ut Tråd ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Lossa Tråd...");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Alla");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Tråd ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Återställ MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("MMU Återställer...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Ta bort, Klicka");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Mixa");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Komponent =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Mixer");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradient");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Full Gradient");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Växla Mix");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Totera Mix");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Gradient Mix");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Omvänd Gradient");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Aktive V-verktyg");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("Start V-verktyg");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT(" Slut V-verktyg");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("Alias V-verktyg");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Återställ V-verktyg");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Kommitta V-verktyg Mix");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-verktyg blev Återställda");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Start Z:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT(" Slut Z:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Spel");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Labyrint");

  PROGMEM Language_Str MSG_BAD_PAGE                        = _UxGT("Dålig sida index");
  PROGMEM Language_Str MSG_BAD_PAGE_SPEED                  = _UxGT("Dålig sida hastighet");

  PROGMEM Language_Str MSG_EDIT_PASSWORD                   = _UxGT("Redigera Lösenord");
  PROGMEM Language_Str MSG_LOGIN_REQUIRED                  = _UxGT("Login Krävs");
  PROGMEM Language_Str MSG_PASSWORD_SETTINGS               = _UxGT("Lösenordsinställningar");
  PROGMEM Language_Str MSG_ENTER_DIGIT                     = _UxGT("Ange Siffra");
  PROGMEM Language_Str MSG_CHANGE_PASSWORD                 = _UxGT("Sätt/Redigera Lösenord");
  PROGMEM Language_Str MSG_REMOVE_PASSWORD                 = _UxGT("Ta bort Lösenord");
  PROGMEM Language_Str MSG_PASSWORD_SET                    = _UxGT("Lösenord är ");
  PROGMEM Language_Str MSG_START_OVER                      = _UxGT("Börja om");
  PROGMEM Language_Str MSG_REMINDER_SAVE_SETTINGS          = _UxGT("Kom ihåg att Spara!");
  PROGMEM Language_Str MSG_PASSWORD_REMOVED                = _UxGT("Lösenord Bort taget");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Tryck på knappen", "för att fortsätta utskrift"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkera..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Vänta på", "trådbyte", "att börja"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Sätt in tråd", "och tryck på knappen", "för att fortsätta"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Tryck på knappen", "för att värma munstycke"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Munstycke värms", "Var snäll och vänta..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Väntar på", "trådlossning"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Väntar på", "trådladdning"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Väntar på", "tråd utrensning"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Klicka för att slutföra", "tråd utrensning"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Väntar på utskrift", "att återstarta..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Klick för att fortsätta"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parkera..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Vänta..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Sätt in och klicka"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Klicka för att värma"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Värmer..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Lossar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Laddar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Rensar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Klicka för att slutföra"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Återgår..."));
  #endif
  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("TMC Drivers");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Driver Ström");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hybrid Tröskelvärde");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Sensorlös Hemning");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Stegningsläge");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("Smyghack Aktiverad");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Återställ");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" in:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Backlash");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_I                      = LCD_STR_I;
  PROGMEM Language_Str MSG_BACKLASH_J                      = LCD_STR_J;
  PROGMEM Language_Str MSG_BACKLASH_K                      = LCD_STR_K;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Korrigering");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Glättning");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("Nivå X Axel");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Auto Kalibrera");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Overksam tidsgräns, temperatur minskning. Tryck ok för att återvärma och igen för att fortsätta.");
  #else
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Värmare Tidsgräns");
  #endif
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Återvärm");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Återvärmning...");

  PROGMEM Language_Str MSG_PROBE_WIZARD                    = _UxGT("Z Sond Wizard");
  PROGMEM Language_Str MSG_PROBE_WIZARD_PROBING            = _UxGT("Sondering Z Referens");
  PROGMEM Language_Str MSG_PROBE_WIZARD_MOVING             = _UxGT("Flyttar till Sonderings Pos");

  PROGMEM Language_Str MSG_SOUND                           = _UxGT("Ljud");

  PROGMEM Language_Str MSG_TOP_LEFT                        = _UxGT("Uppe Vänster");
  PROGMEM Language_Str MSG_BOTTOM_LEFT                     = _UxGT("Nere Vänster");
  PROGMEM Language_Str MSG_TOP_RIGHT                       = _UxGT("Uppe Höger");
  PROGMEM Language_Str MSG_BOTTOM_RIGHT                    = _UxGT("Nere Höger");
  PROGMEM Language_Str MSG_CALIBRATION_COMPLETED           = _UxGT("Kalibrering Färdig");
  PROGMEM Language_Str MSG_CALIBRATION_FAILED              = _UxGT("Kalibrering Misslyckad");
}
