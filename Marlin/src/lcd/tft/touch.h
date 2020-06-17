/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_SCREEN)

#include "tft/xpt2046.h"
#define TOUCH_DRIVER XPT2046

// Menu Navigation
extern int8_t encoderTopLine, encoderLine, screen_items;

enum TouchControlType : uint16_t {
  MENU_SCREEN,
  MENU_ITEM,
  BACK,
  PAGE_UP,
  PAGE_DOWN,
  CLICK,
  RESUME_CONTINUE,
  SLIDER,
  INCREASE,
  DECREASE,
  CANCEL,
  CONFIRM,
  HEATER,
  FAN,
  FEEDRATE,
  FLOWRATE,
  UBL_UP,
  UBL_DOWN,
  UBL_LEFT,
  UBL_RIGHT,
};

typedef void (*screenFunc_t)();

typedef struct __attribute__((__packed__)) {
  TouchControlType type;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  int32_t data;
} touchControl_t;

#define MAX_CONTROLS        16
#define MINIMUM_HOLD_TIME   15
#define TOUCH_REPEAT_DELAY  75
#define MIN_REPEAT_DELAY    25
#define UBL_REPEAT_DELAY    125
#define FREE_MOVE_RANGE     32

class Touch {
  private:
    static TOUCH_DRIVER io;
    static int16_t x, y;

    static touchControl_t controls[MAX_CONTROLS];
    static touchControl_t *current_control;
    static uint16_t controls_count;

    static int32_t x_calibration, y_calibration;
    static int16_t x_offset, y_offset;

    static millis_t now;
    static millis_t time_to_hold;
    static millis_t repeat_delay;
    static bool wait_for_unclick;

    static inline bool get_point(int16_t *x, int16_t *y);
    static void touch(touchControl_t *control);
    static void hold(touchControl_t *control, millis_t delay = 0);

  public:
    static void init();
    static void reset() { controls_count = 0; wait_for_unclick = true; current_control = NULL; };
    static void clear() { controls_count = 0; };
    static void idle();

    static void add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int32_t data = 0);
    static void add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height,  screenFunc_t screen) { add_control(type, x, y, width, height, (int32_t)screen); }
};

extern Touch touch;

#endif // TOUCH_SCREEN
