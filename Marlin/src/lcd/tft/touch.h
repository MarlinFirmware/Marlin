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

#include "tft_color.h"
#include "tft_image.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../tft_io/touch_calibration.h"
#endif

#include HAL_PATH(../../HAL, tft/xpt2046.h)
#define TOUCH_DRIVER XPT2046

// Menu Navigation
extern int8_t encoderTopLine, encoderLine, screen_items;

enum TouchControlType : uint16_t {
  NONE = 0x0000,
  CALIBRATE,
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
  UBL,
  MOVE_AXIS,
  BUTTON,
};

typedef void (*screenFunc_t)();

void add_control(uint16_t x, uint16_t y, TouchControlType control_type, intptr_t data, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED);
inline void add_control(uint16_t x, uint16_t y, TouchControlType control_type, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, control_type, 0, image, is_enabled, color_enabled, color_disabled); }
inline void add_control(uint16_t x, uint16_t y, screenFunc_t screen, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, MENU_SCREEN, (intptr_t)screen, image, is_enabled, color_enabled, color_disabled); }

typedef struct __attribute__((__packed__)) {
  TouchControlType type;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  intptr_t data;
} touch_control_t;

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
    static bool enabled;

    static touch_control_t controls[MAX_CONTROLS];
    static touch_control_t *current_control;
    static uint16_t controls_count;

    static millis_t last_touch_ms, time_to_hold, repeat_delay, touch_time;
    static TouchControlType touch_control_type;

    static inline bool get_point(int16_t *x, int16_t *y);
    static void touch(touch_control_t *control);
    static void hold(touch_control_t *control, millis_t delay = 0);

  public:
    static void init();
    static void reset() { controls_count = 0; touch_time = 0; current_control = NULL; }
    static void clear() { controls_count = 0; }
    static void idle();
    static bool is_clicked() {
      if (touch_control_type == CLICK) {
        touch_control_type = NONE;
        return true;
      }
      return false;
    }
    static void disable() { enabled = false; }
    static void enable() { enabled = true; }

    static void add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, intptr_t data = 0);
};

extern Touch touch;
