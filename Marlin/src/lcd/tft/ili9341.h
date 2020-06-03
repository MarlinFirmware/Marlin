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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "tft.h"

#include "../../inc/MarlinConfig.h"

#define ILI9341_CASET             0x2A // Column Address Set
#define ILI9341_RASET             0x2B // Row Address Set
#define ILI9341_RAMWR             0x2C // Memory Write

static const uint16_t ili9341_init[] = {
  DATASIZE_8BIT,
  ESC_REG(0x0010), ESC_DELAY(10),
  ESC_REG(0x0001), ESC_DELAY(200),
  ESC_REG(0x0036), 0x00E8,
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x00C5), 0x003E, 0x0028,
  ESC_REG(0x00C7), 0x0086,
  ESC_REG(0x00B1), 0x0000, 0x0018,
  ESC_REG(0x00C0), 0x0023,
  ESC_REG(0x00C1), 0x0010,
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_DELAY(100),
  ESC_END
};
