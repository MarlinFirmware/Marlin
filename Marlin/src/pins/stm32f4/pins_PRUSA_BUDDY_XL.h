/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * pins_PRUSA_BUDDY_XL.h
 * Prusa xBuddy Board pin definitions
 *
 * Board:  Prusa xBuddy Board
 * MCU:    STM32F407VGT6 (STM32F407VG, 100-pin LQFP)
 *
 * The xBuddy shares the base pinout with the Buddy Mini.
 * This file includes the common definitions and adds xBuddy-specific overrides.
 *
 * Schematic: https://github.com/prusa3d/Buddy-board-MINI-PCB
 */

#pragma once

#define BOARD_INFO_NAME              "Prusa xBuddy"
#define DEFAULT_MACHINE_NAME         "Prusa xBuddy"

// xBuddy uses the same base pinout as Buddy Mini
#include "pins_PRUSA_BUDDY_common.h"

//
// xBuddy-specific overrides (if any) go below
//
// Note: If xBuddy has differences such as a different display connector,
// additional stepper drivers, or different peripheral routing, define
// them here to override the common definitions.
