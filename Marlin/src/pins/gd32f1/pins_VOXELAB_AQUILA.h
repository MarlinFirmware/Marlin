/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * FFP0173_Aquila_Main_Board_V1.0.1 (GD32F103RC / N32G455RE)
 *
 * Uses CREALITY V4 (STM32F103RE / STM32F103RC) board pin assignments
 */

#define BOARD_INFO_NAME        "Aquila v1.0.1"
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Aquila"
#endif

#define NO_MAPLE_WARNING     // Disable warning when compiling with Maple env

#include "../stm32f1/pins_CREALITY_V4.h"
