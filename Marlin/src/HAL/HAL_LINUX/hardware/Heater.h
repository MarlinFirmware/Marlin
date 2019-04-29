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
#pragma once

#include "Gpio.h"

struct LowpassFilter {
  uint64_t data_delay = 0;
  uint16_t update(uint16_t value) {
    data_delay = data_delay - (data_delay >> 6) + value;
    return (uint16_t)(data_delay >> 6);
  }
};

class Heater: public Peripheral {
public:
  Heater(pin_t heater, pin_t adc);
  virtual ~Heater();
  void interrupt(GpioEvent ev);
  void update();

  pin_t heater_pin, adc_pin;
  uint16_t room_temp_raw;
  uint16_t heater_state;
  LowpassFilter pwmcap;
  double heat;
  uint64_t last;
};
