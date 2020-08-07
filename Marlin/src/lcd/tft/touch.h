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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_SCREEN)

#include "tft_color.h"
#include "tft_image.h"

#include HAL_PATH(../../HAL, tft/xpt2046.h)
#define TOUCH_DRIVER XPT2046

#ifndef TOUCH_SCREEN_CALIBRATION_PRECISION
  #define TOUCH_SCREEN_CALIBRATION_PRECISION  80
#endif

#ifndef TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS
  #define TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS   2500
#endif

#define TOUCH_ORIENTATION_NONE  0
#define TOUCH_LANDSCAPE         1
#define TOUCH_PORTRAIT          2

#if !(defined(TOUCH_CALIBRATION_X) || defined(TOUCH_CALIBRATION_Y) || defined(TOUCH_OFFSET_X) || defined(TOUCH_OFFSET_Y) || defined(TOUCH_ORIENTATION))
  #if defined(XPT2046_X_CALIBRATION) && defined(XPT2046_Y_CALIBRATION) && defined(XPT2046_X_OFFSET) && defined(XPT2046_Y_OFFSET)
    #define TOUCH_CALIBRATION_X  XPT2046_X_CALIBRATION
    #define TOUCH_CALIBRATION_Y  XPT2046_Y_CALIBRATION
    #define TOUCH_OFFSET_X       XPT2046_X_OFFSET
    #define TOUCH_OFFSET_Y       XPT2046_Y_OFFSET
    #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
  #else
    #define TOUCH_CALIBRATION_X  0
    #define TOUCH_CALIBRATION_Y  0
    #define TOUCH_OFFSET_X       0
    #define TOUCH_OFFSET_Y       0
    #define TOUCH_ORIENTATION    TOUCH_ORIENTATION_NONE
  #endif
#endif

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
};

typedef void (*screenFunc_t)();

void add_control(uint16_t x, uint16_t y, TouchControlType control_type, int32_t data, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED);
inline void add_control(uint16_t x, uint16_t y, TouchControlType control_type, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, control_type, 0, image, is_enabled, color_enabled, color_disabled); }
inline void add_control(uint16_t x, uint16_t y, screenFunc_t screen, MarlinImage image, bool is_enabled = true, uint16_t color_enabled = COLOR_CONTROL_ENABLED, uint16_t color_disabled = COLOR_CONTROL_DISABLED) { add_control(x, y, MENU_SCREEN, (int32_t)screen, image, is_enabled, color_enabled, color_disabled); }

typedef struct __attribute__((__packed__)) {
  TouchControlType type;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  int32_t data;
} touch_control_t;

typedef struct __attribute__((__packed__)) {
  int32_t x;
  int32_t y;
  int16_t offset_x;
  int16_t offset_y;
  uint8_t orientation;
} touch_calibration_t;

typedef struct __attribute__((__packed__)) {
  uint16_t x;
  uint16_t y;
  int16_t raw_x;
  int16_t raw_y;
} touch_calibration_point_t;

enum calibrationState : uint8_t {
  CALIBRATION_POINT_1 = 0x00,
  CALIBRATION_POINT_2,
  CALIBRATION_POINT_3,
  CALIBRATION_POINT_4,
  CALIBRATION_SUCCESS,
  CALIBRATION_FAIL,
  CALIBRATION_NONE,
};

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

    static touch_control_t controls[MAX_CONTROLS];
    static touch_control_t *current_control;
    static uint16_t controls_count;

    static millis_t now;
    static millis_t time_to_hold;
    static millis_t repeat_delay;
    static millis_t touch_time;
    static TouchControlType touch_control_type;

    static inline bool get_point(int16_t *x, int16_t *y);
    static void touch(touch_control_t *control);
    static void hold(touch_control_t *control, millis_t delay = 0);

    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      static calibrationState calibration_state;
      static touch_calibration_point_t calibration_points[4];

      static bool validate_precision(int32_t a, int32_t b) { return (a > b ? (100 * b) / a :  (100 * a) / b) > TOUCH_SCREEN_CALIBRATION_PRECISION; }
      static bool validate_precision_x(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw_x, calibration_points[b].raw_x); }
      static bool validate_precision_y(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw_y, calibration_points[b].raw_y); }
    #endif // TOUCH_SCREEN_CALIBRATION

  public:
    static void init();
    static void reset() { controls_count = 0; touch_time = -1; current_control = NULL; }
    static void clear() { controls_count = 0; }
    static void idle();
    static bool is_clicked() { return touch_control_type == CLICK; }

    static void add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int32_t data = 0);

    static touch_calibration_t calibration;
    static void calibration_reset() { calibration = {TOUCH_CALIBRATION_X, TOUCH_CALIBRATION_Y, TOUCH_OFFSET_X, TOUCH_OFFSET_Y, TOUCH_ORIENTATION}; }

    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      static calibrationState calibration_start() { calibration = {0, 0, 0, 0, TOUCH_ORIENTATION_NONE}; return calibration_state = CALIBRATION_POINT_1; }
      static void calibration_end() { calibration_state = CALIBRATION_NONE; }
      static calibrationState get_calibration_state() { return calibration_state; }
    #endif // TOUCH_SCREEN_CALIBRATION
};

extern Touch touch;

#endif // TOUCH_SCREEN
