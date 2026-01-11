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

namespace LanguageNarrow_sv {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Swedish");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" redo.");
  LSTR MSG_YES                            = _UxGT("JA");
  LSTR MSG_NO                             = _UxGT("NEJ");
  LSTR MSG_HIGH                           = _UxGT("Hög");
  LSTR MSG_LOW                            = _UxGT("låg");
  LSTR MSG_BACK                           = _UxGT("Bakåt");
  LSTR MSG_ERROR                          = _UxGT("Fel");

  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Avbryter...");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Minneskort isatt");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Minneskort avlägsnat");
  LSTR MSG_MEDIA_WAITING                  = _UxGT("Väntar på minneskort");
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Misslyckad läsning av minneskort");
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Läsningsfel minneskort");
  LSTR MSG_USB_FD_DEVICE_REMOVED          = _UxGT("USB-minne avlägsnat");
  LSTR MSG_USB_FD_USB_FAILED              = _UxGT("USB-start misslyckad");
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Underanrop överskriden");

  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Ändlägesbrytare"); // Max length 8 characters
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Mjukvarugränslägen");
  LSTR MSG_MAIN_MENU                      = _UxGT("Huvudmeny");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Advancerade inställningar");
  LSTR MSG_TOOLBAR_SETUP                  = _UxGT("Inställningar verktygsfält");
  LSTR MSG_OPTION_DISABLED                = _UxGT("Alternativ avaktiverad");
  LSTR MSG_CONFIGURATION                  = _UxGT("Konfiguration");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Inaktivera stegmotorer");
  LSTR MSG_DEBUG_MENU                     = _UxGT("Debugmeny");
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Förloppindikator test");
  LSTR MSG_HOMING                         = _UxGT("Hemkörning");
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto hem");
  LSTR MSG_AUTO_HOME_A                    = _UxGT("Hem @");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Hem X");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Hem Y");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Hem Z");
  LSTR MSG_Z_AFTER_HOME                   = _UxGT("Z efter hemkörning");
  LSTR MSG_FILAMENT_SET                   = _UxGT("Trådinställningar");
  LSTR MSG_FILAMENT_MAN                   = _UxGT("Trådhantering");
  LSTR MSG_MANUAL_LEVELING                = _UxGT("Manuell höjdjustering");
  LSTR MSG_LEVBED_FL                      = _UxGT("Övre vänster");
  LSTR MSG_LEVBED_FR                      = _UxGT("Övre höger");
  LSTR MSG_LEVBED_C                       = _UxGT("Mittpunkt");
  LSTR MSG_LEVBED_BL                      = _UxGT("Nedre vänster");
  LSTR MSG_LEVBED_BR                      = _UxGT("Nedre höger");
  LSTR MSG_MANUAL_MESH                    = _UxGT("Manellt meshmät");
  LSTR MSG_AUTO_MESH                      = _UxGT("Automatiskt meshnät");

  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Auto Z-Justering");
  LSTR MSG_ITERATION                      = _UxGT("G34 upprepning: %i");
  LSTR MSG_DECREASING_ACCURACY            = _UxGT("Noggrannhet minskar!");
  LSTR MSG_ACCURACY_ACHIEVED              = _UxGT("Noggrannhet uppnådd");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Hemkörning XYZ");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klicka för att starta");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Nästa punkt");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Höjdjustering klar!");
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Tona höjden");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Ange offset för hemmaläge");
  LSTR MSG_HOME_OFFSET_X                  = _UxGT("Hem offset X");
  LSTR MSG_HOME_OFFSET_Y                  = _UxGT("Hem offset Y");
  LSTR MSG_HOME_OFFSET_Z                  = _UxGT("Hem offset Z");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offset tillämpad");
  LSTR MSG_ERR_M428_TOO_FAR               = _UxGT("MIN/MAX för långt");
  LSTR MSG_TRAMMING_WIZARD                = _UxGT("Justerings guide");
  LSTR MSG_SELECT_ORIGIN                  = _UxGT("Välj origo");
  LSTR MSG_LAST_VALUE_SP                  = _UxGT("Senaste värde ");

  LSTR MSG_PREHEAT_1                      = _UxGT("Förvärmning ") PREHEAT_1_LABEL;
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Förvärmning ") PREHEAT_1_LABEL " ~";
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Stoppa");
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Stoppa ~");
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Alla");
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Bädd");
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Förvärmning ") PREHEAT_1_LABEL _UxGT(" Konf");

  LSTR MSG_PREHEAT_M                      = _UxGT("Förvärmning $");
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Förvärmning $ ~");
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Förvärmning $ Stoppa");
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Förvärmning $ Stoppa ~");
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Förvärmning $ Alla");
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Förvärmning $ Bädd");
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Förvärmning $ Donf");

  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Anpassad förvärmning");
  LSTR MSG_COOLDOWN                       = _UxGT("Nedkylning");
  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frekvens");
  LSTR MSG_LASER_MENU                     = _UxGT("Laserkontroll");
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Spindelkontroll");
  LSTR MSG_LASER_POWER                    = _UxGT("Laserstyrka");
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Spindelstyrka");
  LSTR MSG_LASER_TOGGLE                   = _UxGT("Växla laser");
  LSTR MSG_LASER_PULSE_MS                 = _UxGT("Testpuls ms");
  LSTR MSG_LASER_FIRE_PULSE               = _UxGT("Aktivera puls");
  LSTR MSG_SPINDLE_TOGGLE                 = _UxGT("Växla spindel");
  LSTR MSG_SPINDLE_FORWARD                = _UxGT("Spindel framåt");
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Spindel bakåt");
  LSTR MSG_LASER_OFF                      = _UxGT("Laser från");
  LSTR MSG_LASER_ON                       = _UxGT("Laser till");
  LSTR MSG_SPINDLE_OFF                    = _UxGT("Spindel från");
  LSTR MSG_SPINDLE_ON                     = _UxGT("Spindel till");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Spänning till");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Spänning från");
  LSTR MSG_EXTRUDE                        = _UxGT("Extrudera");
  LSTR MSG_RETRACT                        = _UxGT("Sug tillbaka");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Förflytta axel");
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Sond och Nivellera");
  LSTR MSG_BED_LEVELING                   = _UxGT("Höjdjustering av bädd");
  LSTR MSG_LEVEL_BED                      = _UxGT("Höjdjustera bädd");
  LSTR MSG_BED_TRAMMING                   = _UxGT("Justering av bädd");
  LSTR MSG_BED_TRAMMING_RAISE             = _UxGT("Höj bädden tills proben triggas");
  LSTR MSG_BED_TRAMMING_IN_RANGE          = _UxGT("Alla hörn inom tolerans. Bädd höjdjusterad");
  LSTR MSG_BED_TRAMMING_GOOD_POINTS       = _UxGT("Bra punkter: ");
  LSTR MSG_BED_TRAMMING_LAST_Z            = _UxGT("Senaste Z: ");
  LSTR MSG_NEXT_CORNER                    = _UxGT("Nästa hörn");
  LSTR MSG_MESH_EDITOR                    = _UxGT("Meshnätredigerare");
  LSTR MSG_EDIT_MESH                      = _UxGT("Redigera meshnät");
  LSTR MSG_EDITING_STOPPED                = _UxGT("Meshnätredigering stoppad");
  LSTR MSG_PROBING_POINT                  = _UxGT("Höjdmätpunkt");
  LSTR MSG_MESH_X                         = _UxGT("Index X");
  LSTR MSG_MESH_Y                         = _UxGT("Index Y");
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Z-värde");
  LSTR MSG_USER_MENU                      = _UxGT("Anpassade kommandon");
  LSTR MSG_M48_TEST                       = _UxGT("M48 probtest");
  LSTR MSG_M48_POINT                      = _UxGT("M48 punkt");
  LSTR MSG_M48_OUT_OF_BOUNDS              = _UxGT("Mätprob utanför tolerans");
  LSTR MSG_M48_DEVIATION                  = _UxGT("Avvikelse");
  LSTR MSG_IDEX_MENU                      = _UxGT("IDEX läge");
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Verktygsoffset");
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Autoparkera");
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplicering");
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Speglad kopia");
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Full kontroll");
  LSTR MSG_IDEX_DUPE_GAP                  = _UxGT("Duplicera X-avstånd");
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2:a munstycke Z");
  LSTR MSG_HOTEND_OFFSET_N                = _UxGT("2:a munstycke @");
  LSTR MSG_UBL_DOING_G29                  = _UxGT("Utför G29");
  LSTR MSG_UBL_TOOLS                      = _UxGT("UBL verktyg");
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Lutningspunkt");
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Skapa manuellt meshnät");
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Placera shims & mät");
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Mät");
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Ta bort & mät bädd");
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Går vidare till nästa");
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Bäddtemperatur");
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Bäddtemperatur");
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temperatur munstycke");
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temperatur munstycke");
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Redigera anpassat meshnät");
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Finjustera meshnät");
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Redigering meshnät slutförd");
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Bygg anpassat meshnät");
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Bygg meshnät");
  LSTR MSG_UBL_BUILD_MESH_M               = _UxGT("Bygg meshnät ($)");
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Bygg meshnät kallt");
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Justera höjd på meshnät");
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Höjdantal");
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Validera meshnät");
  LSTR MSG_UBL_VALIDATE_MESH_M            = _UxGT("Validera meshnät ($)");
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Validera anpassat meshnät");
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 Värm bädd");
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 Värm munstycke");
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Manuell grundinställning...");
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Fast längd grundinställning");
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Grundinställning klar");
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Avbruten");
  LSTR MSG_G26_LEAVING                    = _UxGT("Höjdjustera G26");
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Fortsätt bädd meshnät");
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Meshnät höjdjustering");
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("3-punkts höjdjustering");
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Höjdjustering av rutnät");
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Höjdjustera meshnät");
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Sidopunkter");
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Karttyp");
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Utmatning meshnätkarta");
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Utmatning för värd");
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Utmatning för CSV");
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Utanför skrivare backup");
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Utmatning UBL info");
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Ifyllnadsmängd");
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Manuell ifyllnad");
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Smart ifyllnad");
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Ifyllnad meshnät");
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Ogiltigförklara alla");
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Ogiltigförklara närmast");
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Finjustera alla");
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Finjustera närmast");
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Meshnätlagring");
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Minnesfack");
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Ladda bädd meshnät");
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Spara bädd meshnät");
  LSTR MSG_MESH_LOADED                    = _UxGT("Meshnät %i laddad");
  LSTR MSG_MESH_SAVED                     = _UxGT("Meshnät %i sparad");
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Inget lagringsutrymme");
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Fel när UBL skulle sparas");
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Fel när UBL skulle återställas");
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-offset: ");
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Z-offset stoppad");
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("Steg för steg UBL");
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1. Bygg meshnät kallt");
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2. Smart ifyllnad");
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3. Validera meshnät");
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4. Finjustera alla");
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5. Validera meshnät");
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6. Finjustera alla");
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7. Spara bädd meshnät");

  LSTR MSG_LED_CONTROL                    = _UxGT("LED-kontroll");
  LSTR MSG_LIGHTS                         = _UxGT("Ljus");
  LSTR MSG_LIGHT_N                        = _UxGT("Ljus #{");
  LSTR MSG_LED_PRESETS                    = _UxGT("Förinställt ljus");
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Rött");
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Orange");
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Gult");
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Grönt");
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Blått");
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Lila");
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Vitt");
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Standard");
  LSTR MSG_LED_CHANNEL_N                  = _UxGT("Kanal {");
  LSTR MSG_NEO2_PRESETS                   = _UxGT("Ljus #2 förinställd");
  LSTR MSG_NEO2_BRIGHTNESS                = _UxGT("Ljusstyrka");
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Anpassat ljus");
  LSTR MSG_INTENSITY_R                    = _UxGT("Ljusstyrka rött");
  LSTR MSG_INTENSITY_G                    = _UxGT("Ljusstyrka grönt");
  LSTR MSG_INTENSITY_B                    = _UxGT("Ljusstyrka blått");
  LSTR MSG_INTENSITY_W                    = _UxGT("Ljusstyrka vitt");
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Ljusstyrka");

  LSTR MSG_MOVING                         = _UxGT("Förflyttar...");
  LSTR MSG_FREE_XY                        = _UxGT("Fri XY");
  LSTR MSG_MOVE_X                         = _UxGT("Förflytta X");
  LSTR MSG_MOVE_Y                         = _UxGT("Förflytta Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Förflytta Z");
  LSTR MSG_MOVE_N                         = _UxGT("Förflytta @");
  LSTR MSG_MOVE_E                         = _UxGT("Kör extruder");
  LSTR MSG_MOVE_EN                        = _UxGT("Extruder *");
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Ej uppnådd temperatur munstycke");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Förflytta $mm");
  LSTR MSG_MOVE_N_IN                      = _UxGT("Flytta $in");
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Flytta $") LCD_STR_DEGREE;
  LSTR MSG_SPEED                          = _UxGT("Hastighet");
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Bädd Z");
  LSTR MSG_NOZZLE                         = _UxGT("Munstycke");
  LSTR MSG_NOZZLE_N                       = _UxGT("Munstycke ~");
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Munstycke parkerat");
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Munstycke standby");
  LSTR MSG_BED                            = _UxGT("Bädd");
  LSTR MSG_CHAMBER                        = _UxGT("Inkapsling");
  LSTR MSG_FAN_SPEED                      = _UxGT("Fläktvarvtal");
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Fläktvarvtal ~");
  LSTR MSG_STORED_FAN_N                   = _UxGT("Sparad fläkt ~");
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Varvtal extrafläkt");
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Varvtal extrafläkt ~");
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Fläktstyrning");
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Tomgångsvarvtal fläkt");
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Autoläge");
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Aktivt varvtal");
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Tomgångsperiod");
  LSTR MSG_FLOW                           = _UxGT("Flöde");
  LSTR MSG_FLOW_N                         = _UxGT("Flöde ~");
  LSTR MSG_CONTROL                        = _UxGT("Styrning");
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fakt");
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemperatur");
  LSTR MSG_LCD_ON                         = _UxGT("Till");
  LSTR MSG_LCD_OFF                        = _UxGT("Från");
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID automappning");
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID automappning *");
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("PID-mappninging klar");
  LSTR MSG_PID_BAD_HEATER_ID              = _UxGT("Automappning misslyckad! Kass extruder.");
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Automappning misslyckad! Temperatur för hög.");
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Automappning misslyckad! Tidsgräns överskriden.");
  LSTR MSG_SELECT_E                       = _UxGT("Välj *");
  LSTR MSG_ACC                            = _UxGT("Accel");
  LSTR MSG_JERK                           = _UxGT("Ryck");
  LSTR MSG_VA_JERK                        = _UxGT("V") STR_A _UxGT("-Ryck");
  LSTR MSG_VB_JERK                        = _UxGT("V") STR_B _UxGT("-Ryck");
  LSTR MSG_VC_JERK                        = _UxGT("V") STR_C _UxGT("-Ryck");
  LSTR MSG_VN_JERK                        = _UxGT("V@-Ryck");
  LSTR MSG_VE_JERK                        = _UxGT("Ve-Ryck");
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Knutpunkt avv");
  LSTR MSG_MAX_SPEED                      = _UxGT("Toppfart (mm/s)");
  LSTR MSG_VMAX_A                         = _UxGT("Vmax ") STR_A;
  LSTR MSG_VMAX_B                         = _UxGT("Vmax ") STR_B;
  LSTR MSG_VMAX_C                         = _UxGT("Vmax ") STR_C;
  LSTR MSG_VMAX_N                         = _UxGT("Toppfart @");
  LSTR MSG_VMAX_E                         = _UxGT("Toppfart E");
  LSTR MSG_VMAX_EN                        = _UxGT("Toppfart *");
  LSTR MSG_VMIN                           = _UxGT("Lägsta hastighet");
  LSTR MSG_VTRAV_MIN                      = _UxGT("Lägsta förflyttningshastighet");
  LSTR MSG_ACCELERATION                   = _UxGT("Acceleration");
  LSTR MSG_AMAX_A                         = _UxGT("Amax ") STR_A;
  LSTR MSG_AMAX_B                         = _UxGT("Amax ") STR_B;
  LSTR MSG_AMAX_C                         = _UxGT("Amax ") STR_C;
  LSTR MSG_AMAX_N                         = _UxGT("Högsta acceleration @");
  LSTR MSG_AMAX_E                         = _UxGT("Högsta acceleration E");
  LSTR MSG_AMAX_EN                        = _UxGT("Högsta acceleration *");
  LSTR MSG_A_RETRACT                      = _UxGT("Sugacceleration");
  LSTR MSG_A_TRAVEL                       = _UxGT("Förflyttningsacceleration");
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Frekvensbegränsning XY");
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Min frekvensfaktor");
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Steg/mm");
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" Steg/mm");
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" Steg/mm");
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" Steg/mm");
  LSTR MSG_N_STEPS                        = _UxGT("@ steg/mm");
  LSTR MSG_E_STEPS                        = _UxGT("E steg/mm");
  LSTR MSG_EN_STEPS                       = _UxGT("* steg/mm");
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatur");
  LSTR MSG_MOTION                         = _UxGT("Rörelse");
  LSTR MSG_FILAMENT                       = _UxGT("Tråd");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E i mm³");
  LSTR MSG_VOLUMETRIC_LIMIT               = _UxGT("E-gräns i mm³");
  LSTR MSG_VOLUMETRIC_LIMIT_E             = _UxGT("E-gräns *");
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Tråddiameter.");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Tråddiameter. *");
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Frigör mm");
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Ladda mm");
  LSTR MSG_ADVANCE_K                      = _UxGT("Avancera K");
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Avancera K *");
  LSTR MSG_CONTRAST                       = _UxGT("Kontrast LCD");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Spara inställningar");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Ladda inställningar");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Återställ till grundinställning");
  LSTR MSG_INIT_EEPROM                    = _UxGT("Initiera EEPROM");
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("EEPROM CRC fel");
  LSTR MSG_ERR_EEPROM_SIZE                = _UxGT("EEPROM-storlek fel");
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("EEPROM-version fel");
  LSTR MSG_SETTINGS_STORED                = _UxGT("Inställningar sparade");
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Uppdatera minneskort");
  LSTR MSG_RESET_PRINTER                  = _UxGT("Starta om 3D-skrivare");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Uppdatera");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Infoskärm");
  LSTR MSG_PREPARE                        = _UxGT("Förbered");
  LSTR MSG_TUNE                           = _UxGT("Justera");
  LSTR MSG_POWER_MONITOR                  = _UxGT("Strömövervakning");
  LSTR MSG_CURRENT                        = _UxGT("Ström");
  LSTR MSG_VOLTAGE                        = _UxGT("Spänning");
  LSTR MSG_POWER                          = _UxGT("Effekt");
  LSTR MSG_START_PRINT                    = _UxGT("Starta utskrift");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Nästa");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Initiera");
  LSTR MSG_BUTTON_STOP                    = _UxGT("Stoppa");
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Skriv");
  LSTR MSG_BUTTON_RESET                   = _UxGT("Återställ");
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignorera");
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Avbryt");
  LSTR MSG_BUTTON_DONE                    = _UxGT("Färdig");
  LSTR MSG_BUTTON_BACK                    = _UxGT("Bakåt");
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Fortsätt");
  LSTR MSG_BUTTON_SKIP                    = _UxGT("Hoppa över");
  LSTR MSG_PAUSING                        = _UxGT("Paus..");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausa utskrift");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Återuppta utskrift");
  LSTR MSG_HOST_START_PRINT               = _UxGT("Värd start");
  LSTR MSG_STOP_PRINT                     = _UxGT("Stoppa utskrift");
  LSTR MSG_END_LOOPS                      = _UxGT("Slut upprepningsloop");
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Skriver objekt");
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Avbryt objekt");
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Avbryt objekt {");
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Strömavbrott");
  LSTR MSG_MEDIA_MENU                     = _UxGT("Skriv från minneskort");
  LSTR MSG_NO_MEDIA                       = _UxGT("Inget minneskort");
  LSTR MSG_DWELL                          = _UxGT("Sov...");
  LSTR MSG_USERWAIT                       = _UxGT("Klicka för att återuppta...");
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Utskrift pausad");
  LSTR MSG_PRINTING                       = _UxGT("Skriver...");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Utskrift avbruten");
  LSTR MSG_PRINT_DONE                     = _UxGT("Utskrift slutförd");
  LSTR MSG_NO_MOVE                        = _UxGT("Ingen förflyttning.");
  LSTR MSG_KILLED                         = _UxGT("DÖDAD. ");
  LSTR MSG_STOPPED                        = _UxGT("STOPPAD. ");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Sug tillbaka mm");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Byt sug.mm");
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Sug tillbaka V");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Hoppa mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Åter sug tillbaka. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Byt åter sug t. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Återsugt. V");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("Byt åter sug. V");
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto sug tillbka");
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Byt längd");
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Byt extra");
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Rensa längd");
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Verktygsbyte");
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Z höj");
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Grundhastighet");
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Återgångshastighet");
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Parkera huvud");
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Återgångshastighet");
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Fläktvarvtal");
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Fläkttid");
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto till");
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto från");
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Verktyg migration");
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Auto migration");
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Senaste extruder");
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Migrera till *");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Byt tråd");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Byt tråd *");
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Ladda tråd");
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Ladda * tråd");
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Lossa tråd");
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Lossa * tråd");
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Lossa allt");

  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Applicera minneskort");
  LSTR MSG_ATTACH_SD                      = _UxGT("Bifoga SD-kort");
  LSTR MSG_ATTACH_USB                     = _UxGT("Bifoga USB-minne");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Byt minneskort");
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Mata ut minneskort");
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Autostarta filer");

  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z-prob utanför bädd");
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Skevhetsfaktor");
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Självtest");
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Återställ");
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Fäll in");
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Fäll ut");
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("SW-läge");
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("5V-läge");
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("OD-läge");
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Läge-lagring");
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Sätt BLTouch till 5V");
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Sätt BLTouch till OD");
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Reportera tömning");
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("VARNING: Felaktiga inställningar kan orsaka skada! Fortsätt ändå?");
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Initiera TouchMI");
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test Z-offset");
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Spara");
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Fäll ut TouchMI");
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Fäll ut Z-prob");
  LSTR MSG_MANUAL_STOW                    = _UxGT("Fäll in Z-prob");
  LSTR MSG_HOME_FIRST                     = _UxGT("Hem %s först");
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Offsets mätprob");
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X-offset mätprob");
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y-offset mätprob");
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z-offset mätprob");
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("Sond @ Offset");
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Steg Z justering");
  LSTR MSG_MOVE_NOZZLE_TO_BED             = _UxGT("Förflytta munstycke till bädd");
  LSTR MSG_BABYSTEP_X                     = _UxGT("Microsteg X");
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Microsteg Y");
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Microsteg Z");
  LSTR MSG_BABYSTEP_N                     = _UxGT("Microsteg @");
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Ändlägesstopp avbrott");
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("Värmning misslyckad");
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Fel: REDUNDANT TEMP");
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("TERMISK ÖVERDRIFT");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Fel: MAXTEMP");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Fel: MINTEMP");
  LSTR MSG_HALTED                         = _UxGT("Utskrift stoppad");
  LSTR MSG_PLEASE_RESET                   = _UxGT("Vänligen återställ");
  LSTR MSG_SHORT_HOUR                     = _UxGT("t"); // One character only
  LSTR MSG_HEATING                        = _UxGT("Värmer...");
  LSTR MSG_COOLING                        = _UxGT("Avkylning...");
  LSTR MSG_BED_HEATING                    = _UxGT("Värmer bädden...");
  LSTR MSG_BED_COOLING                    = _UxGT("Kyler bädden...");
  LSTR MSG_PROBE_HEATING                  = _UxGT("Värmer munstycke...");
  LSTR MSG_PROBE_COOLING                  = _UxGT("Kyler munstycke...");
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Värmer kammare...");
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Kyler kammare...");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Deltakalibrering");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibrera X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibrera Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibrera Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibrera mittläge");
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Deltainställningar");
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Autokalibrering");
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Diag Rod");
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Höjd");
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Radie");
  LSTR MSG_INFO_MENU                      = _UxGT("Om skrivaren");
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Skrivarinformation");
  LSTR MSG_3POINT_LEVELING                = _UxGT("3-Punkts nivåjustering");
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Linjär nivåjustering");
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Bilinjär nivåjustering");
  LSTR MSG_UBL_LEVELING                   = _UxGT("Enhetlig bädd nivåjustering (UBL)");
  LSTR MSG_MESH_LEVELING                  = _UxGT("Meshnät nivåjustering");
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Skrivarstatistik");
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Styrkortsinfo");
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistorer");
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extruderare");
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Datahastighet");
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protokoll");
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Övertempsövervakning: från");
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Övertempsövervakning: till");
  LSTR MSG_HOTEND_IDLE_TIMEOUT            = _UxGT("Tidsgräns värmare munstycke");

  LSTR MSG_CASE_LIGHT                     = _UxGT("Kapslingsljus");
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Ljusstyrka belysning");
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("DEFEKT SKRIVARE");

  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("Utskrift");
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Slutförda");
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("Total utskriftstid");
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("Längsta utskriftstid");
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("Total extrudering");

  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Min temperatur");
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Max temperatur");
  LSTR MSG_INFO_PSU                       = _UxGT("PSU");
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Drivstyrka");
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ Driver %");
  LSTR MSG_ERROR_TMC                      = _UxGT("TMC KOPPLINGSFEL");
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC EEPROM skriv");
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("TRÅDBYTE");
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("UTSKRIFT PAUSAD");
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("LADDA TRÅD");
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("LOSSA TRÅD");
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("FORTSÄTT ALTERNATIV:");
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Rensa mer");
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Fortsätt");
  LSTR MSG_FILAMENT_CHANGE_PURGE_CONTINUE = _UxGT("Rengör eller fortsätt?");
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Munstycke: ");
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Trådkontrollgivare");
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Förbrukad trådlängd mm");
  LSTR MSG_RUNOUT_ENABLE                  = _UxGT("Aktivera trådövervakning");
  LSTR MSG_RUNOUT_ACTIVE                  = _UxGT("Trådövervakning i drift");
  LSTR MSG_INVERT_EXTRUDER                = _UxGT("Invertera extruder");
  LSTR MSG_EXTRUDER_MIN_TEMP              = _UxGT("Extruder Minimumtemp.");
  LSTR MSG_FANCHECK                       = _UxGT("Fläktvarvövervakning");
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Hemkörning misslyckad");
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Probning misslyckad");

  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("VÄLJ TRÅD");
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Uppdatera MMU firmware!");
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU behöver kontrolleras.");
  LSTR MSG_MMU2_RESUME                    = _UxGT("Återuppta MMU");
  LSTR MSG_MMU2_RESUMING                  = _UxGT("MMU Återupptas...");
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Ladda MMU");
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("MMU ladda alla");
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("MMU ladda till munstycke");
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Mata ut MMU");
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("MMU Mata ut ~");
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Lossa MMU");
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Ladda tråd %i...");
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Mata ut tråd ...");
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Lossa tråd...");
  LSTR MSG_MMU2_ALL                       = _UxGT("Alla");
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Tråd ~");
  LSTR MSG_MMU2_RESET                     = _UxGT("Återställ MMU");
  LSTR MSG_MMU2_RESETTING                 = _UxGT("MMU återställs...");
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Ta bort, klicka");
  LSTR MSG_MMU2_REMOVE_AND_CLICK          = _UxGT("Avlägsna och klicka...");

  LSTR MSG_MIX                            = _UxGT("Mixa");
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Komponent {");
  LSTR MSG_MIXER                          = _UxGT("Mixer");
  LSTR MSG_GRADIENT                       = _UxGT("Gradient");
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Full gradient");
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Växla mix");
  LSTR MSG_CYCLE_MIX                      = _UxGT("Rotera mix");
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Gradient mix");
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Omvänd gradient");
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Aktiva V-verktyg");
  LSTR MSG_START_VTOOL                    = _UxGT("Starta V-verktyg");
  LSTR MSG_END_VTOOL                      = _UxGT(" Avsluta V-verktyg");
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias V-verktyg");
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Återställ V-verktyg");
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Anslut V-verktyg mix");
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-verktyg blev återställda");
  LSTR MSG_START_Z                        = _UxGT("Start Z:");
  LSTR MSG_END_Z                          = _UxGT(" Slut Z:");

  LSTR MSG_GAMES                          = _UxGT("Spel");
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");
  LSTR MSG_INVADERS                       = _UxGT("Invaders");
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");
  LSTR MSG_MAZE                           = _UxGT("Labyrint");

  LSTR MSG_BAD_PAGE                       = _UxGT("Dålig sida index");
  LSTR MSG_BAD_PAGE_SPEED                 = _UxGT("Dålig sida hastighet");

  LSTR MSG_EDIT_PASSWORD                  = _UxGT("Redigera Lösenord");
  LSTR MSG_LOGIN_REQUIRED                 = _UxGT("Inloggning krävs");
  LSTR MSG_PASSWORD_SETTINGS              = _UxGT("Lösenordsinställningar");
  LSTR MSG_ENTER_DIGIT                    = _UxGT("Ange siffra");
  LSTR MSG_CHANGE_PASSWORD                = _UxGT("Ange/ändra lösenord");
  LSTR MSG_REMOVE_PASSWORD                = _UxGT("Ta bort lösenord");
  LSTR MSG_PASSWORD_SET                   = _UxGT("Lösenordet är ");
  LSTR MSG_START_OVER                     = _UxGT("Börja om");
  LSTR MSG_REMINDER_SAVE_SETTINGS         = _UxGT("Glöm inte att spara!");
  LSTR MSG_PASSWORD_REMOVED               = _UxGT("Lösenord borttaget");

  //
  // Filament Change screens show up to 2 lines on a 3-line display
  //
  LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_1_LINE("Klick för att fortsätta"));
  LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parkera..."));
  LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_1_LINE("Vänta..."));
  LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_1_LINE("Sätt in och klicka"));
  LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_1_LINE("Klicka för att värma"));
  LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_1_LINE("Värmer..."));
  LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_1_LINE("Lossar..."));
  LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_1_LINE("Laddar..."));
  LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_1_LINE("Rensar..."));
  LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_1_LINE("Klicka för att slutföra"));
  LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_1_LINE("Återgår..."));

  LSTR MSG_TMC_DRIVERS                    = _UxGT("TMC-Drivare");
  LSTR MSG_TMC_CURRENT                    = _UxGT("Strömdrivare");
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Tröskelvärde hybrid");
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Givarfri hemkörning");
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Stegningsläge");
  LSTR MSG_TMC_STEALTHCHOP                = _UxGT("Smyghack");

  LSTR MSG_SERVICE_RESET                  = _UxGT("Återställ");
  LSTR MSG_SERVICE_IN                     = _UxGT(" in:");
  LSTR MSG_BACKLASH                       = _UxGT("Backslag");
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Korrigering");
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Släta ut");

  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Nivå X-axel");
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Autokalibrera");
  LSTR MSG_FTDI_HEATER_TIMEOUT            = _UxGT("Overksam tidsgräns, temperatur minskning. Tryck ok för att återvärma och igen för att fortsätta.");
  LSTR MSG_HEATER_TIMEOUT                 = _UxGT("Värmare Tidsgräns");
  LSTR MSG_REHEAT                         = _UxGT("Återvärm");
  LSTR MSG_REHEATING                      = _UxGT("Återvärmning...");

  LSTR MSG_PROBE_WIZARD                   = _UxGT("Z Sond Wizard");
  LSTR MSG_PROBE_WIZARD_PROBING           = _UxGT("Sondering Z Referens");
  LSTR MSG_PROBE_WIZARD_MOVING            = _UxGT("Flyttar till Sonderings Pos");

  LSTR MSG_SOUND                          = _UxGT("Ljud");

  LSTR MSG_TOP_LEFT                       = _UxGT("Uppe Vänster");
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Nere Vänster");
  LSTR MSG_TOP_RIGHT                      = _UxGT("Uppe Höger");
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Nere Höger");
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Kalibrering Färdig");
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Kalibrering misslyckad");
}

namespace LanguageWide_sv {
  using namespace LanguageNarrow_sv;
  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Utskriftsantal");
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Färdiga");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Total Utskriftstid");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Längsta Jobbtid");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Extruderade Totalt");
  #endif
}

namespace LanguageTall_sv {
  using namespace LanguageWide_sv;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Tryck på knappen", "för att fortsätta utskrift"));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Vänta på", "trådbyte", "att börja"));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Sätt in tråd", "och tryck på knappen", "för att fortsätta"));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Tryck på knappen", "för att värma munstycke"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Munstycke värms", "Var snäll och vänta..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Väntar på", "trådlossning"));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Väntar på", "trådladdning"));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Väntar på", "tråd utrensning"));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Klicka för att slutföra", "tråd utrensning"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Väntar på utskrift", "att återstarta..."));
  #endif
}

namespace Language_sv {
  using namespace LanguageTall_sv;
}
