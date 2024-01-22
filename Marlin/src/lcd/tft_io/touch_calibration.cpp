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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

#include "touch_calibration.h"

#define TOUCH_CALIBRATION_MAX_RETRIES 5

#define DEBUG_OUT ENABLED(DEBUG_TOUCH_CALIBRATION)
#include "../../core/debug_out.h"

#if ENABLED(TOUCH_CALIBRATION_AUTO_SAVE)
  #include "../../module/settings.h"
#endif

TouchCalibration touch_calibration;

touch_calibration_t TouchCalibration::calibration;
calibrationState TouchCalibration::calibration_state = CALIBRATION_NONE;
touch_calibration_point_t TouchCalibration::calibration_points[4];
uint8_t TouchCalibration::failed_count;
millis_t TouchCalibration::next_touch_update_ms; // = 0;

void TouchCalibration::validate_calibration() {
  #define VALIDATE_PRECISION(XY, A, B) validate_precision_##XY(CALIBRATION_##A, CALIBRATION_##B)
  const bool landscape = VALIDATE_PRECISION(x, TOP_LEFT, BOTTOM_LEFT)
                      && VALIDATE_PRECISION(x, TOP_RIGHT, BOTTOM_RIGHT)
                      && VALIDATE_PRECISION(y, TOP_LEFT, TOP_RIGHT)
                      && VALIDATE_PRECISION(y, BOTTOM_LEFT, BOTTOM_RIGHT);
  const bool portrait = VALIDATE_PRECISION(y, TOP_LEFT, BOTTOM_LEFT)
                     && VALIDATE_PRECISION(y, TOP_RIGHT, BOTTOM_RIGHT)
                     && VALIDATE_PRECISION(x, TOP_LEFT, TOP_RIGHT)
                     && VALIDATE_PRECISION(x, BOTTOM_LEFT, BOTTOM_RIGHT);
  #undef VALIDATE_PRECISION

  #define CP(N) calibration_points[CALIBRATION_##N]
  if (landscape) {
    calibration_state = CALIBRATION_SUCCESS;
    calibration.x = ((CP(TOP_RIGHT).x - CP(TOP_LEFT).x) << 17) / (CP(BOTTOM_RIGHT).raw_x + CP(TOP_RIGHT).raw_x - CP(BOTTOM_LEFT).raw_x - CP(TOP_LEFT).raw_x);
    calibration.y = ((CP(BOTTOM_LEFT).y - CP(TOP_LEFT).y) << 17) / (CP(BOTTOM_RIGHT).raw_y - CP(TOP_RIGHT).raw_y + CP(BOTTOM_LEFT).raw_y - CP(TOP_LEFT).raw_y);
    calibration.offset_x = CP(TOP_LEFT).x - int16_t(((CP(TOP_LEFT).raw_x + CP(BOTTOM_LEFT).raw_x) * calibration.x) >> 17);
    calibration.offset_y = CP(TOP_LEFT).y - int16_t(((CP(TOP_LEFT).raw_y + CP(TOP_RIGHT).raw_y) * calibration.y) >> 17);
    calibration.orientation = TOUCH_LANDSCAPE;
  }
  else if (portrait) {
    calibration_state = CALIBRATION_SUCCESS;
    calibration.x = ((CP(TOP_RIGHT).x - CP(TOP_LEFT).x) << 17) / (CP(BOTTOM_RIGHT).raw_y + CP(TOP_RIGHT).raw_y - CP(BOTTOM_LEFT).raw_y - CP(TOP_LEFT).raw_y);
    calibration.y = ((CP(BOTTOM_LEFT).y - CP(TOP_LEFT).y) << 17) / (CP(BOTTOM_RIGHT).raw_x - CP(TOP_RIGHT).raw_x + CP(BOTTOM_LEFT).raw_x - CP(TOP_LEFT).raw_x);
    calibration.offset_x = CP(TOP_LEFT).x - int16_t(((CP(TOP_LEFT).raw_y + CP(BOTTOM_LEFT).raw_y) * calibration.x) >> 17);
    calibration.offset_y = CP(TOP_LEFT).y - int16_t(((CP(TOP_LEFT).raw_x + CP(TOP_RIGHT).raw_x) * calibration.y) >> 17);
    calibration.orientation = TOUCH_PORTRAIT;
  }
  else {
    calibration_state = CALIBRATION_FAIL;
    calibration_reset();
    if (need_calibration() && failed_count++ < TOUCH_CALIBRATION_MAX_RETRIES) calibration_state = CALIBRATION_NONE;
  }
  #undef CP

  if (calibration_state == CALIBRATION_SUCCESS) {
    SERIAL_ECHOLNPGM("Touch screen calibration completed");
    SERIAL_ECHOLN(F("#define TOUCH_"), F("CALIBRATION_X "), calibration.x);
    SERIAL_ECHOLN(F("#define TOUCH_"), F("CALIBRATION_Y "), calibration.y);
    SERIAL_ECHOLN(F("#define TOUCH_"), F("OFFSET_X "), calibration.offset_x);
    SERIAL_ECHOLN(F("#define TOUCH_"), F("OFFSET_Y "), calibration.offset_y);
    SERIAL_ECHO(F("#define TOUCH_")); SERIAL_ECHO_TERNARY(calibration.orientation == TOUCH_LANDSCAPE, "ORIENTATION ", "TOUCH_LANDSCAPE", "TOUCH_PORTRAIT", "\n");
    TERN_(TOUCH_CALIBRATION_AUTO_SAVE, settings.save());
  }
}

bool TouchCalibration::handleTouch(const uint16_t x, const uint16_t y) {
  const millis_t now = millis();

  if (next_touch_update_ms && PENDING(now, next_touch_update_ms)) return false;
  next_touch_update_ms = now + BUTTON_DELAY_MENU;

  if (calibration_state < CALIBRATION_SUCCESS) {
    calibration_points[calibration_state].raw_x = x;
    calibration_points[calibration_state].raw_y = y;
    DEBUG_ECHOLNPGM("TouchCalibration - State: ", calibration_state, ", x: ", calibration_points[calibration_state].x, ", raw_x: ", x, ", y: ", calibration_points[calibration_state].y, ", raw_y: ", y);
  }

  switch (calibration_state) {
    case CALIBRATION_TOP_LEFT: calibration_state = CALIBRATION_TOP_RIGHT; break;
    case CALIBRATION_TOP_RIGHT: calibration_state = CALIBRATION_BOTTOM_RIGHT; break;
    case CALIBRATION_BOTTOM_RIGHT: calibration_state = CALIBRATION_BOTTOM_LEFT; break;
    case CALIBRATION_BOTTOM_LEFT: validate_calibration(); break;
    default: break;
  }

  return true;
}

#endif // TOUCH_SCREEN_CALIBRATION
