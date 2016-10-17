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
 * 5DPrint D8 Driver board pin assignments
 *
 * https://bitbucket.org/makible/5dprint-d8-controller-board
 */

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME "Makibox"
#define BOARD_NAME           "5DPrint D8"

#define LARGE_FLASH        true

//
// Limit Switches
//
#define X_STOP_PIN         37
#define Y_STOP_PIN         36
#define Z_STOP_PIN         39

//
// Steppers
//
#define X_STEP_PIN          0
#define X_DIR_PIN           1
#define X_ENABLE_PIN       23

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       19

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5
#define Z_ENABLE_PIN       18

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN      17

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN          25
#define X_MS2_PIN          26
#define Y_MS1_PIN           9
#define Y_MS2_PIN           8
#define Z_MS1_PIN           7
#define Z_MS2_PIN           6
#define E0_MS1_PIN          5
#define E0_MS2_PIN          4

//
// Temperature Sensors
//
#define TEMP_0_PIN          1  // Analog
#define TEMP_BED_PIN        0  // Analog

//
// Heaters / Fans
//
#define HEATER_0_PIN       21
#define HEATER_BED_PIN     20

// You may need to change FAN_PIN to 16 because Marlin isn't using fastio.h
// for the fan and Teensyduino uses a different pin mapping.
#define FAN_PIN            16

//
// SD Card
//
#define SDSS               20
