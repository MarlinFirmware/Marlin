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

#include "pins_BTT_SKR_MINI_E3.h"

#define BOARD_INFO_NAME "BIGTREE SKR Mini E3"

/**
 * TMC2209 stepper drivers
 * Hardware serial communication ports.
 */
#if HAS_DRIVER(TMC2209)
  #define X_HARDWARE_SERIAL  Serial4
  #define Y_HARDWARE_SERIAL  Serial4
  #define Z_HARDWARE_SERIAL  Serial4
  #define E0_HARDWARE_SERIAL Serial4
#endif
