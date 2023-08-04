/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * DWIN general defines and data structs for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.12.2
 * Date: 2022/08/08
 */

#if defined(__STM32F1__) || defined(STM32F1)
  #define DASH_REDRAW 1
#endif

#if DISABLED(PROBE_MANUALLY) && ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_UBL)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#define defColorBackground  RGB( 1, 12,  8)
#define defColorCursor      RGB(20, 49, 31)
#define defColorTitleBg     RGB( 0, 23, 16)
#define defColorTitleTxt    COLOR_WHITE
#define defColorText        COLOR_WHITE
#define defColorSelected    COLOR_SELECT
#define defColorSplitLine   RGB( 0, 23, 16)
#define defColorHighlight   COLOR_WHITE
#define defColorStatusBg    RGB( 0, 23, 16)
#define defColorStatusTxt   COLOR_YELLOW
#define defColorPopupBg     COLOR_BG_WINDOW
#define defColorPopupTxt    COLOR_POPUP_TEXT
#define defColorAlertBg     COLOR_BG_RED
#define defColorAlertTxt    COLOR_YELLOW
#define defColorPercentTxt  COLOR_PERCENT
#define defColorBarfill     COLOR_BARFILL
#define defColorIndicator   COLOR_WHITE
#define defColorCoordinate  COLOR_WHITE
#define defColorButton      RGB( 0, 23, 16)
#if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  #define defColorLeds      LEDColorWhite()
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  #define defCaseLightBrightness 255
#endif
#ifdef Z_AFTER_HOMING
  #define DEF_Z_AFTER_HOMING Z_AFTER_HOMING
#else
  #define DEF_Z_AFTER_HOMING 0
#endif
#define DEF_HOTENDPIDT TERN(PREHEAT_1_TEMP_BED, PREHEAT_1_TEMP_HOTEND, 195)
#define DEF_BEDPIDT TERN(PREHEAT_1_TEMP_BED, PREHEAT_1_TEMP_HOTEND, 60)
#define DEF_PIDCYCLES 5

