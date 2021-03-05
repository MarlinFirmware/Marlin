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

#if !HAS_LCD_MENU
  #error "Seriously? High resolution TFT screen without menu?"
#endif

#include "tft.h"
#include "tft_image.h"

#if ENABLED(TOUCH_SCREEN)
  #include "touch.h"
  extern bool draw_menu_navigation;
#endif

#if HAS_UI_320x240
  #include "ui_320x240.h"
#elif HAS_UI_480x320 || HAS_UI_480x272
  #include "ui_480x320.h"
#endif

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
void menu_item(const uint8_t row, bool sel = false);

#define ABSOLUTE_ZERO     -273.15

#if HAS_TEMP_CHAMBER && HOTENDS > 1
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_CHAMBER    3
  #define ITEM_FAN        4
  #define ITEMS_COUNT     5
#elif HAS_TEMP_CHAMBER
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_CHAMBER    2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
#elif HAS_TEMP_COOLER
  #define ITEM_COOLER     0
  #define ITEM_FAN        1
  #define ITEMS_COUNT     2
#elif HOTENDS > 1
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
#else
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_FAN        2
  #define ITEMS_COUNT     3
#endif
