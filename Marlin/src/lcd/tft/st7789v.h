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

#pragma once

#include "../../inc/MarlinConfig.h"
#include "tft.h"

static const uint16_t st7789v_init[] = {
  DATASIZE_8BIT,
  ESC_REG(0x0010), ESC_DELAY(10),
  ESC_REG(0x0001), ESC_DELAY(200),
  ESC_REG(0x0011), ESC_DELAY(120),
  ESC_REG(0x0036), 0x00A0,
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x002A), 0x0000, 0x0000, 0x0001, 0x003F,
  ESC_REG(0x002B), 0x0000, 0x0000, 0x0000, 0x00EF,
  ESC_REG(0x00B2), 0x000C, 0x000C, 0x0000, 0x0033, 0x0033,
  ESC_REG(0x00B7), 0x0035,
  ESC_REG(0x00BB), 0x001F,
  ESC_REG(0x00C0), 0x002C,
  ESC_REG(0x00C2), 0x0001, 0x00C3,
  ESC_REG(0x00C4), 0x0020,
  ESC_REG(0x00C6), 0x000F,
  ESC_REG(0x00D0), 0x00A4, 0x00A1,
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_END
};
