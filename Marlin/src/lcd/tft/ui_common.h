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

typedef void (*voidFunc_t)();

void draw_heater_status(const uint16_t x, const uint16_t y, const int8_t heater);
void draw_fan_status(const uint16_t x, const uint16_t y, const bool blink);

void drawMessage(PGM_P const msg);
inline void drawMessage(FSTR_P const fmsg) { drawMessage(FTOP(fmsg)); }

void quick_feedback();

void drawCurZSelection();
void drawCurESelection();

void moveAxis(const AxisEnum axis, const int8_t direction);
inline void e_plus()  { moveAxis(E_AXIS,  1); }
inline void e_minus() { moveAxis(E_AXIS, -1); }
inline void x_minus() { moveAxis(X_AXIS, -1); }
inline void x_plus()  { moveAxis(X_AXIS,  1); }
inline void y_plus()  { moveAxis(Y_AXIS,  1); }
inline void y_minus() { moveAxis(Y_AXIS, -1); }
inline void z_plus()  { moveAxis(Z_AXIS,  1); }
inline void z_minus() { moveAxis(Z_AXIS, -1); }

void menu_line(const uint8_t row, const uint16_t color=COLOR_BACKGROUND);
void menu_item(const uint8_t row, const bool sel=false);

#if HAS_TOUCH_SLEEP
  bool lcd_sleep_task();
#endif

#define ABSOLUTE_ZERO     -273.15

#if HAS_TEMP_CHAMBER && HAS_MULTI_HOTEND
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
#elif HAS_MULTI_HOTEND
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

#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1

struct MotionAxisState {
  xy_int_t xValuePos, yValuePos, zValuePos, eValuePos, stepValuePos, zTypePos, eNamePos;
  float currentStepSize = 10.0;
  int z_selection = Z_SELECTION_Z;
  uint8_t e_selection = 0;
  bool blocked = false;
  char message[32];
};

extern MotionAxisState motionAxisState;
