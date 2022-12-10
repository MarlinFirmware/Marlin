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
 * BAM&DICE Due (Arduino Mega) pin assignments
 * ATmega2560, ATmega1280
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "2PrintBeta Due supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "2PrintBeta Due"

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN                 PinK4  // Pullup or pulldown!
#define SPINDLE_DIR_PIN                       PinK5
#define SPINDLE_LASER_PWM_PIN                 PinL5  // Hardware PWM

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH6  // Analog Input
#define TEMP_1_PIN                            PinB5  // Analog Input

#include "pins_RAMPS.h"
