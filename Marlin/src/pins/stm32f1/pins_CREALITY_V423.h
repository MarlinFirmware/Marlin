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
 * CREALITY v4.2.3 (STM32F103) board pin assignments
 */

#define BOARD_INFO_NAME      "Creality v4.2.3"
#define DEFAULT_MACHINE_NAME "Creality3D"

//
// Heaters
//
#define HEATER_BED_PIN                      PB10  // HOT BED

#include "pins_CREALITY_V4.h"

//
// Encoder
//
#if BTN_EN1 == PB10
  #undef BTN_EN1
  #define BTN_EN1                           PA2   // Rotary Encoder
#endif
