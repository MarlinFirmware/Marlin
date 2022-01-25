/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Arduino library for MicroChip MCP3426 I2C A/D converter.
 * https://www.microchip.com/en-us/product/MCP3426
 */

#include <stdint.h>
#include <Wire.h>

// Address of MCP342X chip
#define MCP342X_ADC_I2C_ADDRESS 104

class MCP3426 {
  public:
    int16_t ReadValue(uint8_t channel, uint8_t gain);
    bool Error;
};

extern MCP3426 mcp3426;
