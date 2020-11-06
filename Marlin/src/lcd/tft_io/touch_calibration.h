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

#include "../../inc/MarlinConfig.h"

#ifndef TOUCH_SCREEN_CALIBRATION_PRECISION
  #define TOUCH_SCREEN_CALIBRATION_PRECISION  80
#endif

#ifndef TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS
  #define TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS   2500
#endif

#define TOUCH_ORIENTATION_NONE  0
#define TOUCH_LANDSCAPE         1
#define TOUCH_PORTRAIT          2

#ifndef TOUCH_ORIENTATION
  #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

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

enum calibrationState {
  CALIBRATION_TOP_LEFT = 0x00,
  CALIBRATION_BOTTOM_LEFT,
  CALIBRATION_TOP_RIGHT,
  CALIBRATION_BOTTOM_RIGHT,
  CALIBRATION_SUCCESS,
  CALIBRATION_FAIL,
  CALIBRATION_NONE,
};

class TouchCalibration {
public:
  static calibrationState calibration_state;
  static touch_calibration_point_t calibration_points[4];

  static bool validate_precision(int32_t a, int32_t b);
  static bool validate_precision_x(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw_x, calibration_points[b].raw_x); }
  static bool validate_precision_y(uint8_t a, uint8_t b) { return validate_precision(calibration_points[a].raw_y, calibration_points[b].raw_y); }
  static void validate_calibration();

  static touch_calibration_t calibration;
  static void calibration_reset() { calibration = {TOUCH_CALIBRATION_X, TOUCH_CALIBRATION_Y, TOUCH_OFFSET_X, TOUCH_OFFSET_Y, TOUCH_ORIENTATION}; }

  static calibrationState calibration_start();
  static void calibration_end() { calibration_state = CALIBRATION_NONE; }
  static calibrationState get_calibration_state() { return calibration_state; }

  static bool handleTouch(uint16_t x, uint16_t y);
};

extern TouchCalibration touch_calibration;

#endif
