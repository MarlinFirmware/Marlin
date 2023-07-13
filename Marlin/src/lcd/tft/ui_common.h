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
#else
  // add_control() function is used to display encoder-controlled elements
  enum TouchControlType : uint16_t {
    NONE = 0x0000,
  };
#endif

#define UI_INCL_(W, H) STRINGIFY_(ui_##W##x##H.h)
#define UI_INCL(W, H) UI_INCL_(W, H)

#include "ui_theme.h"
#include UI_INCL(TFT_WIDTH, TFT_HEIGHT)
#include "tft_font.h"
#include "tft_color.h"

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

void text_line(const uint16_t y, uint16_t color=COLOR_BACKGROUND);
void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
void menu_item(const uint8_t row, bool sel = false);

typedef void (*screenFunc_t)();
void add_control(uint16_t x, uint16_t y, TouchControlType control_type, intptr_t data, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED);
inline void add_control(uint16_t x, uint16_t y, TouchControlType control_type, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, control_type, 0, image, is_enabled, color_enabled, color_disabled); }
#if ENABLED(TOUCH_SCREEN)
  inline void add_control(uint16_t x, uint16_t y, screenFunc_t screen, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, MENU_SCREEN, (intptr_t)screen, image, is_enabled, color_enabled, color_disabled); }
#endif

#if HAS_TOUCH_SLEEP
  bool lcd_sleep_task();
#endif

#define ABSOLUTE_ZERO     -273.15

#if DISABLED(CUSTOM_STATUS_SCREEN_ITEMS_ORDER)
  enum {
    OPTITEM(HAS_EXTRUDERS, ITEM_E0)
    OPTITEM(HAS_MULTI_HOTEND, ITEM_E1)
    #if HOTENDS > 2
      ITEM_E2,
    #endif
    OPTITEM(HAS_HEATED_BED, ITEM_BED)
    OPTITEM(HAS_TEMP_CHAMBER, ITEM_CHAMBER)
    OPTITEM(HAS_TEMP_COOLER, ITEM_COOLER)
    OPTITEM(HAS_FAN, ITEM_FAN)
    ITEMS_COUNT
  };
#endif
