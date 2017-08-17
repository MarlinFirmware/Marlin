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
 * Gen3 Monolithic Electronics pin assignments
 */

#ifndef __AVR_ATmega644P__
  #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define DEBUG_PIN 0

// x axis
#define X_STEP_PIN 15
#define X_DIR_PIN 18
#define X_MIN_PIN 20
// Alex Checar #define X_STOP_PIN         20
#define X_ENABLE_PIN 24 // actually uses Y_enable_pin
#define X_MAX_PIN -1

// y axis
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_MIN_PIN 25
// Alex Checar #define Y_STOP_PIN         25
#define Y_ENABLE_PIN 24 // shared with X_enable_pin
#define Y_MAX_PIN -1

// z axis
#define Z_STEP_PIN 27
#define Z_DIR_PIN 28
#define Z_MIN_PIN 30
// Alex Checar #define Z_STOP_PIN         30
#define Z_ENABLE_PIN 29
#define Z_MAX_PIN -1

// extruder pins
#define E0_STEP_PIN         12
#define E0_DIR_PIN          17
#define E0_ENABLE_PIN       3

#define HEATER_0_PIN 16
#define TEMP_0_PIN 0

#define FAN_PIN -1

// bed pins
#define HEATER_BED_PIN -1
#define TEMP_BED_PIN -1


#define SDSS     -1
#define SDPOWER          -1
#define LED_PIN          -1

// pin for controlling the PSU.
#define PS_ON_PIN       14  // Alex, Do this work on the card?

// Alex extras from Gen3+
#define KILL_PIN           -1
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define HEATER_2_PIN       -1
