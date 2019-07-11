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

/**
 * Arduino Mega with RAMPS v1.0, v1.1, v1.2 pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_NAME "RAMPS <1.2"

// Uncomment the following line for RAMPS v1.0
//#define RAMPS_V_1_0

//
// Limit Switches
//
#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MIN_PIN          16
#define Y_MAX_PIN          17
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  19
#endif

//
// Steppers
//
#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24

#define Y_STEP_PIN         38
#define Y_DIR_PIN          40
#define Y_ENABLE_PIN       36

#define Z_STEP_PIN         44
#define Z_DIR_PIN          46
#define Z_ENABLE_PIN       42

#define E0_STEP_PIN        32
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30

//
// Temperature Sensors
//
#define TEMP_0_PIN          2   // Analog Input
#define TEMP_BED_PIN        1   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS_PIN   66   // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS_PIN   66   // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

//
// Heaters / Fans
//
#if ENABLED(RAMPS_V_1_0)
  #define HEATER_0_PIN     12
  #define HEATER_BED_PIN   -1
  #ifndef FAN_PIN
    #define FAN_PIN        11
  #endif
#else // RAMPS_V_1_1 or RAMPS_V_1_2
  #define HEATER_0_PIN     10
  #define HEATER_BED_PIN    8
  #ifndef FAN_PIN
    #define FAN_PIN         9
  #endif
#endif

//
// Misc. Functions
//
#define SDPOWER            48
#define SDSS               53
#define LED_PIN            13
#define CASE_LIGHT_PIN     45   // MUST BE HARDWARE PWM

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN    41   // Pin should have a pullup/pulldown!
#define SPINDLE_LASER_PWM_PIN    45   // MUST BE HARDWARE PWM
#define SPINDLE_DIR_PIN          43
