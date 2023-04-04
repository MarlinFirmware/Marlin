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

#if ENABLED(NO_LCD_MENUS)
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
#elif HAS_UI_1024x600
  #include "ui_1024x600.h"
#else
  #error "Unsupported display resolution!"
#endif

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
void menu_item(const uint8_t row, bool sel = false);

#if HAS_TOUCH_SLEEP
  bool lcd_sleep_task();
#endif

#define ABSOLUTE_ZERO     -273.15

#if HAS_EXTRUDERS
  #define ITEM_E0         0
  #if HOTENDS > 1
    #define ITEM_E1       1
  #endif
  #if HOTENDS > 2
    #define ITEM_E2       2
  #endif
#endif

#if HAS_HOTBED
  #if defined(ITEM_E2)
    #define ITEM_BED (ITEM_E2 + 1)
  #elif defined(ITEM_E1)
    #define ITEM_BED (ITEM_E1 + 1)
  #elif defined(ITEM_E0)
    #define ITEM_BED (ITEM_E0 + 1)
  #else
    #define ITEM_BED 0
  #endif
#endif

#if HAS_TEMP_CHAMBER
  #if defined(ITEM_BED)
    #define ITEM_CHAMBER (ITEM_BED + 1)
  #elif defined(ITEM_E2)
    #define ITEM_CHAMBER (ITEM_E2 + 1)
  #elif defined(ITEM_E1)
    #define ITEM_CHAMBER (ITEM_E1 + 1)
  #elif defined(ITEM_E0)
    #define ITEM_CHAMBER (ITEM_E0 + 1)
  #else
    #define ITEM_CHAMBER 0
  #endif
#endif

#if HAS_TEMP_COOLER
  #if defined(ITEM_CHAMBER)
    #define ITEM_COOLER (ITEM_CHAMBER + 1)
  #elif defined(ITEM_BED)
    #define ITEM_COOLER (ITEM_BED + 1)
  #elif defined(ITEM_E2)
    #define ITEM_COOLER (ITEM_E2 + 1)
  #elif defined(ITEM_E1)
    #define ITEM_COOLER (ITEM_E1 + 1)
  #elif defined(ITEM_E0)
    #define ITEM_COOLER (ITEM_E0 + 1)
  #else
    #define ITEM_COOLER 0
  #endif
#endif

#if HAS_FAN
  #if defined(ITEM_COOLER)
    #define ITEM_FAN (ITEM_COOLER + 1)
  #elif defined(ITEM_CHAMBER)
    #define ITEM_FAN (ITEM_CHAMBER + 1)
  #elif defined(ITEM_BED)
    #define ITEM_FAN (ITEM_BED + 1)
  #elif defined(ITEM_E2)
    #define ITEM_FAN (ITEM_E2 + 1)
  #elif defined(ITEM_E1)
    #define ITEM_FAN (ITEM_E1 + 1)
  #elif defined(ITEM_E0)
    #define ITEM_FAN (ITEM_E0 + 1)
  #else
    #define ITEM_FAN 0
  #endif
#endif

#if defined(ITEM_FAN)
  #define ITEMS_COUNT (ITEM_FAN + 1)
#elif defined(ITEM_COOLER)
  #define ITEMS_COUNT (ITEM_COOLER + 1)
#elif defined(ITEM_CHAMBER)
  #define ITEMS_COUNT (ITEM_CHAMBER + 1)
#elif defined(ITEM_BED)
  #define ITEMS_COUNT (ITEM_BED + 1)
#elif defined(ITEM_E2)
  #define ITEMS_COUNT (ITEM_E2 + 1)
#elif defined(ITEM_E1)
  #define ITEMS_COUNT (ITEM_E1 + 1)
#elif defined(ITEM_E0)
  #define ITEMS_COUNT (ITEM_E0 + 1)
#else
  #define ITEMS_COUNT 0
#endif
