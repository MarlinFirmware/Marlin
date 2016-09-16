/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * RIGIDBOARD V2 Arduino Mega with RAMPS v1.4 pin assignments
 */

#include "pins_RIGIDBOARD.h"

#undef BOARD_NAME
#define BOARD_NAME "RigidBoard V2"

// I2C based DAC like on the Printrboard REVF
#define DAC_STEPPER_CURRENT
// Channels available for DAC, For Rigidboard there are 4
#define DAC_STEPPER_ORDER { 0, 1, 2, 3 }

#define DAC_STEPPER_SENSE    0.11
#define DAC_STEPPER_ADDRESS  0
#define DAC_STEPPER_MAX   5000
#define DAC_STEPPER_VREF     1 //internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN     0
#define DAC_DISABLE_PIN     42 //  set low to enable DAC
#define DAC_OR_ADDRESS    0x01
