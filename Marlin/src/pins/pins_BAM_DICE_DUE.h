/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * BAM&DICE Due (Arduino Mega) pin assignments
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "2PrintBeta Due supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "2PrintBeta Due"

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENABLE_PIN 66   // Pin should have a pullup/pulldown!
#define SPINDLE_DIR_PIN          67
#define SPINDLE_LASER_PWM_PIN    44   // MUST BE HARDWARE PWM

#include "pins_RAMPS.h"

//
// Temperature Sensors
//
#undef TEMP_0_PIN
#undef TEMP_1_PIN
#define TEMP_0_PIN          9   // Analog Input
#define TEMP_1_PIN         11   // Analog Input
