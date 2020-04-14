/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef __PLAT_LINUX__

#include <random>
#include <stdio.h>
#include "Clock.h"
#include "LinearAxis.h"

LinearAxis::LinearAxis(pin_type enable, pin_type dir, pin_type step, pin_type end_min, pin_type end_max) {
  enable_pin = enable;
  dir_pin = dir;
  step_pin = step;
  min_pin = end_min;
  max_pin = end_max;

  min_position = 50;
  max_position = (200*80) + min_position;
  position = rand() % ((max_position - 40) - min_position) + (min_position + 20);
  last_update = Clock::nanos();

  Gpio::attachPeripheral(step_pin, this);

}

LinearAxis::~LinearAxis() {

}

void LinearAxis::update() {

}

void LinearAxis::interrupt(GpioEvent ev) {
  if (ev.pin_id == step_pin && !Gpio::pin_map[enable_pin].value){
    if (ev.event == GpioEvent::RISE) {
      last_update = ev.timestamp;
      position += -1 + 2 * Gpio::pin_map[dir_pin].value;
      Gpio::pin_map[min_pin].value = (position < min_position);
      //Gpio::pin_map[max_pin].value = (position > max_position);
      //if (position < min_position) printf("axis(%d) endstop : pos: %d, mm: %f, min: %d\n", step_pin, position, position / 80.0, Gpio::pin_map[min_pin].value);
    }
  }
}

#endif // __PLAT_LINUX__
