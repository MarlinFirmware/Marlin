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

#define MARLIN_LOGO_FULL_SIZE MarlinLogo320x240x16

#include "ui_common.h"

#define TFT_STATUS_TOP_Y       0
#define TFT_TOP_LINE_Y         2

#define MENU_TEXT_X_OFFSET    10
#define MENU_TEXT_Y_OFFSET    tft_string.vcenter(MENU_ITEM_HEIGHT)

#define MENU_ITEM_ICON_X       0
#define MENU_ITEM_ICON_Y       0
#define MENU_ITEM_ICON_SPACE  32

#define MENU_ITEM_HEIGHT      32
#define MENU_LINE_HEIGHT      (MENU_ITEM_HEIGHT + 2)

#if (TFT_FONT == NOTOSANS) || (TFT_FONT == HELVETICA)
  #define FONT_SIZE           14
#elif TFT_FONT == UNIFONT
  #define FONT_SIZE           10
#endif

#include "tft_font.h"
