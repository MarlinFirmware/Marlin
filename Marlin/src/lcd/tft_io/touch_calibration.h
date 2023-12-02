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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

#define _TOUCH_CALIBRATION_X touch_calibration.calibration.x
#define _TOUCH_CALIBRATION_Y touch_calibration.calibration.y
#define _TOUCH_OFFSET_X      touch_calibration.calibration.offset.x
#define _TOUCH_OFFSET_Y      touch_calibration.calibration.offset.y
#define _TOUCH_ORIENTATION   touch_calibration.calibration.orientation

#ifndef TOUCH_SCREEN_CALIBRATION_PRECISION
  #define TOUCH_SCREEN_CALIBRATION_PRECISION  80
#endif
#ifndef TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS
  #define TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS   2500
#endif

typedef struct __attribute__((__packed__)) TouchCal : xy_long_t {
  xy_int_t offset;
  uint8_t orientation;
  TouchCal() { set(xy_long_t({ 0, 0 }), xy_int_t({ 0, 0 }), TOUCH_ORIENTATION_NONE); }
  void set(const xy_long_t &xy, const xy_int_t &hv, const uint8_t o) {
    xy_long_t::set(xy); offset = hv; orientation = o;
  }
  void reset() {
    set(xy_long_t({ TOUCH_CALIBRATION_X, TOUCH_CALIBRATION_Y }),
        xy_int_t({ TOUCH_OFFSET_X, TOUCH_OFFSET_Y }),
        TOUCH_ORIENTATION);
  }
} touch_calibration_t;

typedef struct __attribute__((__packed__)) : xy_uint_t {
  xy_int_t raw;
} touch_calibration_point_t;

enum calibrationState : uint8_t {
  CALIBRATION_TOP_LEFT = 0x00,
  CALIBRATION_TOP_RIGHT,
  CALIBRATION_BOTTOM_RIGHT,
  CALIBRATION_BOTTOM_LEFT,
  CALIBRATION_SUCCESS,
  CALIBRATION_FAIL,
  CALIBRATION_NONE,
};

class TouchCalibration {
public:
  static calibrationState calibration_state;
  static touch_calibration_point_t calibration_points[4];
  static millis_t next_touch_update_ms;

  static bool validate_precision(int32_t a, int32_t b) { return (a > b ? (100 * b) / a : (100 * a) / b) > (TOUCH_SCREEN_CALIBRATION_PRECISION); }
  static bool validate_precision_x(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw.x, calibration_points[b].raw.x); }
  static bool validate_precision_y(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw.y, calibration_points[b].raw.y); }
  static void validate_calibration();

  static touch_calibration_t calibration;
  static uint8_t failed_count;
  static void calibration_reset() { calibration.set(xy_long_t({ TOUCH_CALIBRATION_X, TOUCH_CALIBRATION_Y }), xy_int_t({ TOUCH_OFFSET_X, TOUCH_OFFSET_Y }), TOUCH_ORIENTATION); }
  static bool need_calibration() { return !(calibration.offset.x || calibration.offset.y || calibration.x || calibration.y); }

  static calibrationState calibration_start() {
    next_touch_update_ms = millis() + 750UL;
    calibration.reset();
    calibration_state = CALIBRATION_TOP_LEFT;
    calibration_points[CALIBRATION_TOP_LEFT].set(30, 30);
    calibration_points[CALIBRATION_TOP_RIGHT].set(TFT_WIDTH - 31, 30);
    calibration_points[CALIBRATION_BOTTOM_RIGHT].set(TFT_WIDTH - 31, TFT_HEIGHT - 31);
    calibration_points[CALIBRATION_BOTTOM_LEFT].set(30, TFT_HEIGHT - 31);
    failed_count = 0;
    return calibration_state;
  }
  static void calibration_end() { calibration_state = CALIBRATION_NONE; }
  static calibrationState get_calibration_state() { return calibration_state; }
  static bool calibration_loaded() {
    if (need_calibration()) calibration_reset();
    return !need_calibration();
  }

  static bool handleTouch(const xy_int_t &point);
};

extern TouchCalibration touch_calibration;

#else // TOUCH_SCREEN_CALIBRATION

#define _TOUCH_CALIBRATION_X (TOUCH_CALIBRATION_X)
#define _TOUCH_CALIBRATION_Y (TOUCH_CALIBRATION_Y)
#define _TOUCH_OFFSET_X      (TOUCH_OFFSET_X)
#define _TOUCH_OFFSET_Y      (TOUCH_OFFSET_Y)
#define _TOUCH_ORIENTATION   (TOUCH_ORIENTATION)

#endif
