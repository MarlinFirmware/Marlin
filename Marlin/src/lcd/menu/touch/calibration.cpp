/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_CALIBRATION)

#include "../../ultralcd.h"
#include "calibration.h"

TouchCalibration::TouchCalibration()
{
  memset(&results[0], 0, sizeof(results));
}

void TouchCalibration::init_calibration(const uint8_t init_state) {
  calibration_state = init_state;
  #ifndef LONGER3D
  ui.encoder_direction_normal();
  ui.defer_status_screen();
  #endif
  do_calibration(results);
  #ifndef LONGER3D
  exit_calibration();
  #endif
}

void TouchCalibration::exit_calibration() {
  ui.init();
  ui.goto_previous_screen_no_defer();
}

#endif // TOUCH_CALIBRATION
