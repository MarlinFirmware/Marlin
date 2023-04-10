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

#define MARLIN_LOGO_FULL_SIZE MarlinLogo480x320x16

#include "ui_common.h"

#define TFT_STATUS_TOP_Y       4
#define TFT_TOP_LINE_Y         4

#define MENU_TEXT_X_OFFSET    16
#define MENU_TEXT_Y_OFFSET    tft_string.vcenter(MENU_ITEM_HEIGHT)

#define MENU_ITEM_ICON_X       5
#define MENU_ITEM_ICON_Y       5
#define MENU_ITEM_ICON_SPACE  42

#if HAS_UI_480x320

  #define MENU_ITEM_HEIGHT    43
  #define FONT_LINE_HEIGHT    34

  #if (TFT_FONT == NOTOSANS) || (TFT_FONT == HELVETICA)
    #define FONT_SIZE         19
  #elif TFT_FONT == UNIFONT
    #define FONT_SIZE         20
  #endif

#elif HAS_UI_480x272
  #if TFT_FONT == NOTOSANS
    #define FONT_SIZE         16
    #define FONT_LINE_HEIGHT  26
  #elif TFT_FONT == UNIFONT
    #define FONT_SIZE         10
    #define FONT_LINE_HEIGHT  24
  #elif TFT_FONT == HELVETICA
    #define FONT_SIZE         14
    #define FONT_LINE_HEIGHT  24
  #endif

  #define MENU_ITEM_HEIGHT    36
#endif

#define MENU_LINE_HEIGHT      (MENU_ITEM_HEIGHT + 2)

/**
 * Status screen - portrait layout:
 *
 * | FAN   E0   BED   .. | - 120px
 * |     Coordinates     | - FONT_LINE_HEIGHT
 * | Feedrate   Flowrate | - 32px
 * | SD         Settings | - 64px if ENABLED(TOUCHSCREEN), else 0px
 * | Print duration time | - 29px
 * |     Progress bar    | - 7px
 * |    Status message   | - FONT_LINE_HEIGHT
 *
 * Summary with touchscreen:
 * - Total height: 252px + 2 * FONT_LINE_HEIGHT (320px if FONT_LINE_HEIGHT is 34px)
 * - Rows count: 7
 * - Margins count: 8
 *
 * Summary without touchscreen:
 * - Total height: 188px + 2 * FONT_LINE_HEIGHT (256px if FONT_LINE_HEIGHT is 34px)
 * - Rows count: 6
 * - Margins count: 7
 *
 * Status screen - landscape layout:
 *
 * |          FAN   E0   BED   ...          | - 120px
 * |              Coordinates               | - FONT_LINE_HEIGHT
 * | SD    Flowrate    Feedrate    Settings | - 44px for TOUCHSCREEN, else 32px
 * |          Print duration time           | - 29px
 * |              Progress bar              | - 7px
 * |             Status message             | - FONT_LINE_HEIGHT
 *
 * Summary with touchscreen:
 * - Total height: 200px (268px if FONT_LINE_HEIGHT is 34px)
 * - Rows count: 6
 * - Margins count: 7
 *
 * Summary without touchscreen:
 * - Total height: 188px + 2 * FONT_LINE_HEIGHT (256px if FONT_LINE_HEIGHT is 34px)
 * - Rows count: 6
 * - Margins count: 7
 */
#ifdef TFT_COLOR_UI_PORTRAIT
  #if ENABLED(TOUCH_SCREEN)
    #define STATUS_TOTAL_ROWS_HEIGHT (2 * FONT_LINE_HEIGHT + 120 + 32 + 64 + 29 + 7)
    #define STATUS_MARGINS_REGIONS 8
  #else
    #define STATUS_TOTAL_ROWS_HEIGHT (2 * FONT_LINE_HEIGHT + 120 + 32 + 29 + 7)
    #define STATUS_MARGINS_REGIONS 7
  #endif

  #define FEEDRATE_X(W) ((TFT_WIDTH - 2 * (W)) / 4)
  #define FLOWRATE_X(W) ((3 * TFT_WIDTH - 2 * (W)) / 4)
  #define SETTINGS_X (3 * TFT_WIDTH / 4 - 32)
  #define SDCARD_X (TFT_WIDTH / 4 - 32)
#else
  #define STATUS_TOTAL_ROWS_HEIGHT (2 * FONT_LINE_HEIGHT + 120 + TERN(TOUCH_SCREEN, 44, 32) + 29 + 7)
  #define STATUS_MARGINS_REGIONS 7

  #define FEEDRATE_X(W) (3 * TFT_WIDTH / 8 - (W) / 2)
  #define FLOWRATE_X(W) (5 * TFT_WIDTH / 8 - (W) / 2)
  #define SETTINGS_X (TFT_WIDTH / 8 - 32)
  #define SDCARD_X (7 * TFT_WIDTH / 8 - 32)
#endif

#define STATUS_MARGIN_SIZE ((TFT_HEIGHT - STATUS_TOTAL_ROWS_HEIGHT) / STATUS_MARGINS_REGIONS)

#include "tft_font.h"
