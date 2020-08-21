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

#include <chrono>
#include "Gpio.h"

class LinearAxis: public Peripheral {
public:
  LinearAxis(pin_type enable, pin_type dir, pin_type step, pin_type end_min, pin_type end_max);
  virtual ~LinearAxis();
  void update();
  void interrupt(GpioEvent ev);

  pin_type enable_pin;
  pin_type dir_pin;
  pin_type step_pin;
  pin_type min_pin;
  pin_type max_pin;

  int32_t position;
  int32_t min_position;
  int32_t max_position;
  uint64_t last_update;

};
