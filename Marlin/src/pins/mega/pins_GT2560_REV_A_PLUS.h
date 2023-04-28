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
 * Geeetech GT2560 Revision A+ board pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Geeetech%20GT2560%20Revision%20A+/Hardware_GT2560_RevA+.pdf
 * Origin: https://www.geeetech.com/wiki/images/d/d3/Hardware_GT2560_RevA%2B.pdf
 * ATmega2560
 */

#define BOARD_INFO_NAME "GT2560 Rev.A+"

#include "pins_GT2560_REV_A.h"

#if DISABLED(BLTOUCH)
  #define SERVO0_PIN  32
#endif
