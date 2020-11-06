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

int dac_init();
void dac_current_percent(uint8_t channel, float val);
void dac_current_raw(uint8_t channel, uint16_t val);
void dac_print_values();
void dac_commit_eeprom();
uint8_t dac_current_get_percent(AxisEnum axis);
void dac_current_set_percents(xyze_uint8_t &pct);
