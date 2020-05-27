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

/* Mind the mess: with landscape screen orientation 'Horizontal' is Y and 'Vertical' is X */
#define ILI9328_HASET             0x20   /* Horizontal GRAM address register (0-255) */
#define ILI9328_VASET             0x21   /* Vertical GRAM address register (0-511)*/
#define ILI9328_WRITE_RAM         0x22   /* Write data to GRAM */

#define ILI9328_HASTART           0x50   /* Horizontal address start position (0-255) */
#define ILI9328_HAEND             0x51   /* Horizontal address end position (0-255) */
#define ILI9328_VASTART           0x52   /* Vertical address start position (0-511) */
#define ILI9328_VAEND             0x53   /* Vertical address end position (0-511) */

static const uint16_t ili9328_init[] = {
  DATASIZE_16BIT,
  ESC_REG(0x0001), 0x0100,
  ESC_REG(0x0002), 0x0400,
  ESC_REG(0x0003), 0x1038,
  ESC_REG(0x0004), 0x0000,
  ESC_REG(0x0008), 0x0202,
  ESC_REG(0x0009), 0x0000,
  ESC_REG(0x000A), 0x0000,
  ESC_REG(0x000C), 0x0000,
  ESC_REG(0x000D), 0x0000,
  ESC_REG(0x000F), 0x0000,
  ESC_REG(0x0010), 0x0000,
  ESC_REG(0x0011), 0x0007,
  ESC_REG(0x0012), 0x0000,
  ESC_REG(0x0013), 0x0000,
  ESC_REG(0x0007), 0x0001,
  ESC_DELAY(200),
  ESC_REG(0x0010), 0x1690,
  ESC_REG(0x0011), 0x0227,
  ESC_DELAY(50),
  ESC_REG(0x0012), 0x008C,
  ESC_DELAY(50),
  ESC_REG(0x0013), 0x1500,
  ESC_REG(0x0029), 0x0004,
  ESC_REG(0x002B), 0x000D,
  ESC_DELAY(50),
  ESC_REG(0x0050), 0x0000,
  ESC_REG(0x0051), 0x00EF,
  ESC_REG(0x0052), 0x0000,
  ESC_REG(0x0053), 0x013F,
  ESC_REG(0x0020), 0x0000,
  ESC_REG(0x0021), 0x0000,
  ESC_REG(0x0060), 0x2700,
  ESC_REG(0x0061), 0x0001,
  ESC_REG(0x006A), 0x0000,
  ESC_REG(0x0080), 0x0000,
  ESC_REG(0x0081), 0x0000,
  ESC_REG(0x0082), 0x0000,
  ESC_REG(0x0083), 0x0000,
  ESC_REG(0x0084), 0x0000,
  ESC_REG(0x0085), 0x0000,
  ESC_REG(0x0090), 0x0010,
  ESC_REG(0x0092), 0x0600,
  ESC_REG(0x0007), 0x0133,
  ESC_REG(0x0022),
  ESC_END
};
