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
 * Basque-Euskera
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

namespace Language_eu {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 1;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Basque-Euskera");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" prest.");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Atzera");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Txartela sartuta");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Txartela kenduta");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu nagusia");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Auto hasiera");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Itzali motoreak");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Arazketa Menua");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Prog. Barra Proba");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Hasierara joan");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("X jatorrira");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Y jatorrira");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Z jatorrira");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("XYZ hasieraratzen");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Klik egin hasteko");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Hurrengo Puntua");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Berdintzea eginda");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Etxe. offset eza.");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offsetak ezarrita");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Hasiera ipini");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Berotu ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H0                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_H1                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_H2                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_H3                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_H4                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_H5                    = _UxGT("Berotu ") PREHEAT_1_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E0                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E1                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E2                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E3                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E4                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_1_END_E5                = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Amaia ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Guztia");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Ohea");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Berotu ") PREHEAT_1_LABEL _UxGT(" Ezarp.");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Berotu ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H0                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_H1                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_H2                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_H3                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_H4                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_H5                    = _UxGT("Berotu ") PREHEAT_2_LABEL " " LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E0                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N0;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E1                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N1;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E2                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N2;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E3                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N3;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E4                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N4;
  PROGMEM Language_Str MSG_PREHEAT_2_END_E5                = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Amaia ") LCD_STR_N5;
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Guztia");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Ohea");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Berotu ") PREHEAT_2_LABEL _UxGT(" Ezarp.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Hoztu");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Energia piztu");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Energia itzali");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Estruitu");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Atzera eragin");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Ardatzak mugitu");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Ohe berdinketa");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Ohea berdindu");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Ertzak berdindu");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Hurrengo ertza");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Sarea editatu");

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("G29 exekutatzen");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL Tresnak");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Sarea eskuz sortu");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Neurtu");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("UBL aktibatu");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("UBL desaktibatu");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Ohearen tenperatura");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Mutur beroaren tenp.");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Sarea editatu");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Sarea editatzea eginda");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Sarea sortu");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = PREHEAT_1_LABEL _UxGT(" sarea sortu");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = PREHEAT_2_LABEL _UxGT(" sarea sortu");
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Sare hotza sortu");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Sarearen altuera doitu");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Sarea balioetsi");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = PREHEAT_1_LABEL _UxGT(" sarea balioetsi");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = PREHEAT_2_LABEL _UxGT(" sarea balioetsi");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Ohe sarea balioetsi");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Sare berdinketa");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3 puntuko berdinketa");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Lauki-sare berdinketa");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Sarea berdindu");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Mapa mota");
  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED ezarpenak");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Argiak");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Argi aurrehautaketak");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Gorria");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Laranja");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Horia");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Berdea");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Urdina");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Bioleta");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Zuria");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Lehenetsia");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Argi pertsonalizatuak");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Intentsitate gorria");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Intentsitate berdea");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Intentsitate urdina");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Intentsitate zuria");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Distira");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Mugitzen...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Askatu XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mugitu X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mugitu Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mugitu Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Estrusorea");
  PROGMEM Language_Str MSG_MOVE_E0                         = _UxGT("Estrusorea ") LCD_STR_E0;
  PROGMEM Language_Str MSG_MOVE_E1                         = _UxGT("Estrusorea ") LCD_STR_E1;
  PROGMEM Language_Str MSG_MOVE_E2                         = _UxGT("Estrusorea ") LCD_STR_E2;
  PROGMEM Language_Str MSG_MOVE_E3                         = _UxGT("Estrusorea ") LCD_STR_E3;
  PROGMEM Language_Str MSG_MOVE_E4                         = _UxGT("Estrusorea ") LCD_STR_E4;
  PROGMEM Language_Str MSG_MOVE_E5                         = _UxGT("Estrusorea ") LCD_STR_E5;
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mugitu %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mugitu 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mugitu 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mugitu 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Abiadura");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z Ohea");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Pita");
  PROGMEM Language_Str MSG_NOZZLE_0                        = _UxGT("Pita ") LCD_STR_N0;
  PROGMEM Language_Str MSG_NOZZLE_1                        = _UxGT("Pita ") LCD_STR_N1;
  PROGMEM Language_Str MSG_NOZZLE_2                        = _UxGT("Pita ") LCD_STR_N2;
  PROGMEM Language_Str MSG_NOZZLE_3                        = _UxGT("Pita ") LCD_STR_N3;
  PROGMEM Language_Str MSG_NOZZLE_4                        = _UxGT("Pita ") LCD_STR_N4;
  PROGMEM Language_Str MSG_NOZZLE_5                        = _UxGT("Pita ") LCD_STR_N5;
  PROGMEM Language_Str MSG_BED                             = _UxGT("Ohea");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Haizagailu abiadura");
  PROGMEM Language_Str MSG_FAN_SPEED_1                     = _UxGT("Haizagailu abiadura 1");
  PROGMEM Language_Str MSG_FAN_SPEED_2                     = _UxGT("Haizagailu abiadura 2");
  PROGMEM Language_Str MSG_FAN_SPEED_3                     = _UxGT("Haizagailu abiadura 3");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Haiz.gehig. abiadura");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_1               = _UxGT("Haiz.gehig. abiadura 1");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_2               = _UxGT("Haiz.gehig. abiadura 2");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_3               = _UxGT("Haiz.gehig. abiadura 3");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Fluxua");
  PROGMEM Language_Str MSG_FLOW_0                          = _UxGT("Fluxua ") LCD_STR_N0;
  PROGMEM Language_Str MSG_FLOW_1                          = _UxGT("Fluxua ") LCD_STR_N1;
  PROGMEM Language_Str MSG_FLOW_2                          = _UxGT("Fluxua ") LCD_STR_N2;
  PROGMEM Language_Str MSG_FLOW_3                          = _UxGT("Fluxua ") LCD_STR_N3;
  PROGMEM Language_Str MSG_FLOW_4                          = _UxGT("Fluxua ") LCD_STR_N4;
  PROGMEM Language_Str MSG_FLOW_5                          = _UxGT("Fluxua ") LCD_STR_N5;
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Kontrola");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fakt");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Auto tenperatura");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Aukeratu");
  PROGMEM Language_Str MSG_SELECT_E0                       = _UxGT("Aukeratu ") LCD_STR_E0;
  PROGMEM Language_Str MSG_SELECT_E1                       = _UxGT("Aukeratu ") LCD_STR_E1;
  PROGMEM Language_Str MSG_SELECT_E2                       = _UxGT("Aukeratu ") LCD_STR_E2;
  PROGMEM Language_Str MSG_SELECT_E3                       = _UxGT("Aukeratu ") LCD_STR_E3;
  PROGMEM Language_Str MSG_SELECT_E4                       = _UxGT("Aukeratu ") LCD_STR_E4;
  PROGMEM Language_Str MSG_SELECT_E5                       = _UxGT("Aukeratu ") LCD_STR_E5;
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Azelerazioa");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Astindua");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-astindua");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-astindua");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-astindua");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-astindua");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VBidaia min");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-retrakt");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-bidaia");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Pausoak/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E pausoak/mm");
  PROGMEM Language_Str MSG_E0_STEPS                        = LCD_STR_E0 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E1_STEPS                        = LCD_STR_E1 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E2_STEPS                        = LCD_STR_E2 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E3_STEPS                        = LCD_STR_E3 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E4_STEPS                        = LCD_STR_E4 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_E5_STEPS                        = LCD_STR_E5 _UxGT(" pausoak/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Tenperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Mugimendua");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Harizpia");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E mm3-tan");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Hariz. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E0                = _UxGT("Hariz. Dia. ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E1                = _UxGT("Hariz. Dia. ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E2                = _UxGT("Hariz. Dia. ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E3                = _UxGT("Hariz. Dia. ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E4                = _UxGT("Hariz. Dia. ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E5                = _UxGT("Hariz. Dia. ") LCD_STR_E5;
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Deskargatu mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Kargatu mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K Aurrerapena");
  PROGMEM Language_Str MSG_ADVANCE_K_E0                    = _UxGT("K Aurrerapena ") LCD_STR_E0;
  PROGMEM Language_Str MSG_ADVANCE_K_E1                    = _UxGT("K Aurrerapena ") LCD_STR_E1;
  PROGMEM Language_Str MSG_ADVANCE_K_E2                    = _UxGT("K Aurrerapena ") LCD_STR_E2;
  PROGMEM Language_Str MSG_ADVANCE_K_E3                    = _UxGT("K Aurrerapena ") LCD_STR_E3;
  PROGMEM Language_Str MSG_ADVANCE_K_E4                    = _UxGT("K Aurrerapena ") LCD_STR_E4;
  PROGMEM Language_Str MSG_ADVANCE_K_E5                    = _UxGT("K Aurrerapena ") LCD_STR_E5;
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD kontrastea");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Gorde memoria");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Kargatu memoria");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Larri. berriz.");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("EEPROM-a hasieratu");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Berriz kargatu");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Pantaila info");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Prestatu");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Doitu");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausatu inprimak.");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Jarraitu inprima.");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Gelditu inprima.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("SD-tik inprimatu");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Ez dago SD-rik");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Lo egin...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Aginduak zain...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Inprim. geldi.");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Inprim. deusezta.");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Mugimendu gabe.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("AKABATUTA. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("GELDITUTA. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Atzera egin mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Swap Atzera mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Atzera egin V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Igo mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Atzera egin mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Swap Atzera mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Atzera egin V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Atzera egin");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Aldatu harizpia");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E0               = _UxGT("Aldatu harizpia ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E1               = _UxGT("Aldatu harizpia ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E2               = _UxGT("Aldatu harizpia ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E3               = _UxGT("Aldatu harizpia ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E4               = _UxGT("Aldatu harizpia ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E5               = _UxGT("Aldatu harizpia ") LCD_STR_E5;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E0                 = _UxGT("Harizpia kargatu ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E1                 = _UxGT("Harizpia kargatu ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E2                 = _UxGT("Harizpia kargatu ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E3                 = _UxGT("Harizpia kargatu ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E4                 = _UxGT("Harizpia kargatu ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTLOAD_E5                 = _UxGT("Harizpia kargatu ") LCD_STR_E5;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E0               = _UxGT("Harizpia deskargatu ") LCD_STR_E0;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E1               = _UxGT("Harizpia deskargatu ") LCD_STR_E1;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E2               = _UxGT("Harizpia deskargatu ") LCD_STR_E2;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E3               = _UxGT("Harizpia deskargatu ") LCD_STR_E3;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E4               = _UxGT("Harizpia deskargatu ") LCD_STR_E4;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E5               = _UxGT("Harizpia deskargatu ") LCD_STR_E5;
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Erabat deskargatu");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Hasieratu SD-a");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Aldatu txartela");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z zunda kanpora");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Okertze faktorea");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch AutoProba");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("BLTouch berrabia.");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("BLTouch jaitsi/luzatu");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("BLTouch igo/jaso");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Etxera %s%s%s lehenengo");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Z Konpentsatu");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Mikro-urratsa X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Mikro-urratsa Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Mikro-urratsa Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Endstop deusezta.");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Err: Beroketa");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Err: Tenperatura");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("TENP. KONTROL EZA");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: Tenp Maximoa");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: Tenp Minimoa");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Err: Ohe Tenp Max");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Err: Ohe Tenp Min");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Etxera XY lehenengo");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("INPRIMA. GELDIRIK");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Berrabia. Mesedez");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Berotzen...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Ohea Berotzen...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibraketa");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Kalibratu X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Kalibratu Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Kalibratu Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Kalibratu Zentrua");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta ezarpenak");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Kalibraketa");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Delta Alt. Ezar.");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Barra diagonala");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Altuera");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Erradioa");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Inprimagailu Inf.");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Inprimagailu Inf.");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3 puntuko berdinketa");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Berdinketa lineala");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Berdinketa bilinearra");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Sare berdinketa");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Inprima. estatis.");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Txartelaren Info.");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistoreak");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Estrusoreak");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baudioak");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokoloa");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Kabina Argia");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS
  = ;
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Inprim. Zenbaketa");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Burututa");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Inprim. denbora");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Imprimatze luzeena");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Estruituta guztira");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Inprimatze");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Burututa");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Guztira");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Luzeena");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Estrusio");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Tenp. Minimoa");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Tenp. Maximoa");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Elikadura-iturria");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Driver-aren potentzia");
  PROGMEM Language_Str MSG_DAC_PERCENT                     = _UxGT("Driver %");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Idatzi DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("HARIZPIA ALDATU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("HARIZPIA KARGATU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("HARIZPIA DESKARGATU");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("ALDAKETA AUKERAK:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Inprima. jarraitu");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Pita: ");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Hasi. huts egin du");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Neurketak huts egin du");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: hotzegi");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Inprimagailu okerra");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT            = _UxGT(MSG_1_LINE("Mesedez, itxaron..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_1_LINE("Deskargatzen..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT          = _UxGT(MSG_1_LINE("Sartu eta click egin"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING         = _UxGT(MSG_1_LINE("Berotzen..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_1_LINE("Kargatzen..."));
}
