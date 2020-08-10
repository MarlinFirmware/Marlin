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

/**
 * Pin assignments for the MKS RUMBA32
 *
 * https://aus3d.com.au/rumba32
 * https://github.com/Aus3D/RUMBA32
 *
 * The MKS and Aus3D versions have the same pinout but the MKS version
 * has some added resistors and LEDs.  The resistors needed for the
 * TMC2208/9 UART interface are among the additions.  Also added were
 * connectors and resistors dedicated to the TMC2130 sensorless homing
 * interface.
 */

#define BOARD_INFO_NAME "Aus3D RUMBA32"

#define RUMBA32_V1_0
//#define I2C_EEPROM

#include "pins_RUMBA32_common.h"
