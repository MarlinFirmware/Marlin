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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

#include "touch_calibration.h"

TouchCalibration touch_calibration;

touch_calibration_t TouchCalibration::calibration;
calibrationState TouchCalibration::calibration_state = CALIBRATION_NONE;
touch_calibration_point_t TouchCalibration::calibration_points[4];

void TouchCalibration::validate_calibration() {
  const bool landscape = validate_precision_x(0, 1) && validate_precision_x(2, 3) && validate_precision_y(0, 2) && validate_precision_y(1, 3);
  const bool portrait = validate_precision_y(0, 1) && validate_precision_y(2, 3) && validate_precision_x(0, 2) && validate_precision_x(1, 3);

  if (landscape || portrait) {
    calibration_state = CALIBRATION_SUCCESS;
    calibration.x = ((calibration_points[2].x - calibration_points[0].x) << 17) / (calibration_points[3].raw_x + calibration_points[2].raw_x - calibration_points[1].raw_x - calibration_points[0].raw_x);
    calibration.y = ((calibration_points[1].y - calibration_points[0].y) << 17) / (calibration_points[3].raw_y - calibration_points[2].raw_y + calibration_points[1].raw_y - calibration_points[0].raw_y);
    calibration.offset_x = calibration_points[0].x - int16_t(((calibration_points[0].raw_x + calibration_points[1].raw_x) * calibration.x) >> 17);
    calibration.offset_y = calibration_points[0].y - int16_t(((calibration_points[0].raw_y + calibration_points[2].raw_y) * calibration.y) >> 17);
    calibration.orientation = landscape ? TOUCH_LANDSCAPE : TOUCH_PORTRAIT;
  }
  else {
    calibration_state = CALIBRATION_FAIL;
    calibration_reset();
  }

  if (calibration_state == CALIBRATION_SUCCESS) {
    SERIAL_ECHOLNPGM("Touch screen calibration completed");
    SERIAL_ECHOLNPAIR("TOUCH_CALIBRATION_X ", calibration.x);
    SERIAL_ECHOLNPAIR("TOUCH_CALIBRATION_Y ", calibration.y);
    SERIAL_ECHOLNPAIR("TOUCH_OFFSET_X ", calibration.offset_x);
    SERIAL_ECHOLNPAIR("TOUCH_OFFSET_Y ", calibration.offset_y);
    SERIAL_ECHOPGM("TOUCH_ORIENTATION "); if (calibration.orientation == TOUCH_LANDSCAPE) SERIAL_ECHOLNPGM("TOUCH_LANDSCAPE"); else SERIAL_ECHOLNPGM("TOUCH_PORTRAIT");
  }
}

bool TouchCalibration::handleTouch(uint16_t x, uint16_t y) {
  static millis_t next_button_update_ms = 0;
  const millis_t now = millis();
  if (PENDING(now, next_button_update_ms)) return false;
  next_button_update_ms = now + BUTTON_DELAY_MENU;

  if (calibration_state < CALIBRATION_SUCCESS) {
    calibration_points[calibration_state].raw_x = x;
    calibration_points[calibration_state].raw_y = y;
  }

  switch (calibration_state) {
    case CALIBRATION_TOP_LEFT: calibration_state = CALIBRATION_BOTTOM_LEFT; break;
    case CALIBRATION_BOTTOM_LEFT: calibration_state = CALIBRATION_TOP_RIGHT; break;
    case CALIBRATION_TOP_RIGHT: calibration_state = CALIBRATION_BOTTOM_RIGHT; break;
    case CALIBRATION_BOTTOM_RIGHT: validate_calibration(); break;
    default: break;
  }

  return true;
}

#endif // TOUCH_SCREEN_CALIBRATION
