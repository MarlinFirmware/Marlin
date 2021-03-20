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

#ifdef __PLAT_LINUX__

#include "Clock.h"
#include <stdio.h>
#include "../../../inc/MarlinConfig.h"

#include "Heater.h"

Heater::Heater(pin_t heater, pin_t adc) {
  heater_state = 0;
  room_temp_raw = 150;
  last = Clock::micros();
  heater_pin = heater;
  adc_pin = adc;
  heat = 0.0;
}

Heater::~Heater() {
}

void Heater::update() {
  // crude pwm read and cruder heat simulation
  auto now = Clock::micros();
  double delta = (now - last);
  if (delta > 1000 ) {
    heater_state = pwmcap.update(0xFFFF * Gpio::pin_map[heater_pin].value);
    last = now;
    heat += (heater_state - heat) * (delta / 1000000000.0);

    if (heat < room_temp_raw) heat = room_temp_raw;
    Gpio::pin_map[analogInputToDigitalPin(adc_pin)].value = 0xFFFF - (uint16_t)heat;
  }
}

void Heater::interrupt(GpioEvent ev) {
  // ununsed
}

#endif // __PLAT_LINUX__
