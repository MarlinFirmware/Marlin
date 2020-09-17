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

#include "../../inc/MarlinConfigPre.h"

#include "tft.h"
#include "tft_image.h"

#if ENABLED(TOUCH_SCREEN)
  #include "touch.h"
#endif

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

#define MENU_TEXT_X_OFFSET  10
#define MENU_TEXT_Y_OFFSET   7
void menu_line(const uint8_t row, uint16_t color = COLOR_BACKGROUND);
void menu_item(const uint8_t row, bool sel = false);

#define MENU_FONT_NAME    Helvetica14
#define SYMBOLS_FONT_NAME Helvetica14_symbols

#define ABSOLUTE_ZERO     -273.15

const tImage Images[imgCount] = {
  MarlinLogo320x240x16,
  HotEnd_64x64x4,
  Bed_64x64x4,
  Bed_Heated_64x64x4,
  Chamber_64x64x4,
  Chamber_Heated_64x64x4,
  Fan0_64x64x4,
  Fan_Slow0_64x64x4,
  Fan_Slow1_64x64x4,
  Fan_Fast0_64x64x4,
  Fan_Fast1_64x64x4,
  Feedrate_32x32x4,
  Flowrate_32x32x4,
  SD_64x64x4,
  Menu_64x64x4,
  Settings_64x64x4,
  Directory_32x32x4,
  Confirm_64x64x4,
  Cancel_64x64x4,
  Increase_64x64x4,
  Decrease_64x64x4,
  Back_32x32x4,
  Up_32x32x4,
  Down_32x32x4,
  Left_32x32x4,
  Right_32x32x4,
  Refresh_32x32x4,
  Leveling_32x32x4,
  Slider8x16x4,
  Home_64x64x4,
  BtnRounded_64x52x4,
};

#if HAS_TEMP_CHAMBER && HOTENDS > 1
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_CHAMBER    3
  #define ITEM_FAN        4
  #define ITEMS_COUNT     5
  #define POS_Y           0
#elif HAS_TEMP_CHAMBER
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_CHAMBER    2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
  #define POS_Y           0
#elif HOTENDS > 1
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
  #define POS_Y           0
#else
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_FAN        2
  #define ITEMS_COUNT     3
  #define POS_Y           0
#endif
