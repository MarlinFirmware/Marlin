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

/**
 * stepper_dac.h   - To set stepper current via DAC
 */

#include "dac_mcp4728.h"

class StepperDAC {
public:
  static int init();
  static void set_current_percent(const uint8_t channel, float val);
  static void set_current_value(const uint8_t channel, uint16_t val);
  static void print_values();
  static void commit_eeprom();
  static uint8_t get_current_percent(const AxisEnum axis);
  static void set_current_percents(xyze_uint8_t &pct);
};

extern StepperDAC stepper_dac;
