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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_CALIBRATION)

// menu_touchscreen.cpp
void menu_touchscreen();
void enter_touch_calibrate();

// calibration.cpp
class TouchCalibration {
protected:
  uint8_t calibration_state;
  void exit_calibration();
public:
  int16_t results[4];
  TouchCalibration();
  void init_calibration(const uint8_t init_state);
  int16_t do_calibration(int16_t results[4]);
};

extern TouchCalibration calibration;

#endif // TOUCH_CALIBRATION
