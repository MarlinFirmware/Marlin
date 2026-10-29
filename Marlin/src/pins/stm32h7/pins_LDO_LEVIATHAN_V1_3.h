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
#pragma once

/**
 * LDO Leviathan V1.3 (STM32H743ZIT6) pin assignments
 * Schematic: https://github.com/MotorDynamicsLab/Leviathan/blob/master/Schematic/Leviathan_V1.3.pdf
 *
 * The V1.3 swaps the V1.2's STM32F446ZET6 for an STM32H743ZIT6 with an
 * identical pinout, so all pin assignments are shared with the V1.2.
 */

#define BOARD_INFO_NAME "LDO Leviathan V1.3"

// On the STM32H743's 144-pin package PC2/PC3 are bonded as the dual-pad
// PC2_C/PC3_C pins, whose analog switches to the PC2/PC3 GPIOs are closed
// at reset, so they serve the same endstop inputs as on the V1.2.
#define PC2                              PC2_C
#define PC3                              PC3_C

#include "../stm32f4/pins_LDO_LEVIATHAN.h"
