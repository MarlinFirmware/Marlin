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

/**
 * Gen7 v1.1, v1.2, v1.3 pin assignments
 */

 /**
 * Rev B    26 DEC 2016
 *
 * 1) added pointer to a current Arduino IDE extension
 * 2) added support for M3, M4 & M5 spindle control commands
 * 3) added case light pin definition
 *
 */

/**
 * A useable Arduino IDE extension (board manager) can be found at
 * https://github.com/Lauszus/Sanguino
 *
 * This extension has been tested on Arduino 1.6.12 & 1.8.0
 *
 * Here's the JSON path:
 * https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
 *
 * When installing select 1.0.2
 *
 * Installation instructions can be found at https://learn.sparkfun.com/pages/CustomBoardsArduino
 * Just use the above JSON URL instead of Sparkfun's JSON.
 *
 * Once installed select the Sanguino board and then select the CPU.
 *
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
  #error "Oops! Select 'Sanguino' in 'Tools > Boards' and 'ATmega644', 'ATmega644P', or 'ATmega1284P' in 'Tools > Processor.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "Gen7 v1.1 / 1.2"
#endif

#ifndef GEN7_VERSION
  #define GEN7_VERSION 12   // v1.x
#endif

//
// Limit Switches
//
#define X_MIN_PIN           7
#define Y_MIN_PIN           5
#define Z_MIN_PIN           1
#define Z_MAX_PIN           0
#define Y_MAX_PIN           2
#define X_MAX_PIN           6


//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN   0
#endif

//
// Steppers
//
#define X_STEP_PIN         19
#define X_DIR_PIN          18
#define X_ENABLE_PIN       24

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_ENABLE_PIN       24

#define Z_STEP_PIN         26
#define Z_DIR_PIN          25
#define Z_ENABLE_PIN       24

#define E0_STEP_PIN        28
#define E0_DIR_PIN         27
#define E0_ENABLE_PIN      24

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // Analog Input
#define TEMP_BED_PIN        2   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        4
#define HEATER_BED_PIN      3

#if !defined(FAN_PIN) && GEN7_VERSION < 13   // Gen7 v1.3 removed the fan pin
  #define FAN_PIN        31
#endif

//
// Misc. Functions
//
#define PS_ON_PIN          15

#if GEN7_VERSION < 13
  #define CASE_LIGHT_PIN   16   // MUST BE HARDWARE PWM
#else     // Gen7 v1.3 removed the I2C connector & signals so need to get PWM off the PC power supply header
  #define CASE_LIGHT_PIN   15   // MUST BE HARDWARE PWM
#endif

// All these generations of Gen7 supply thermistor power
// via PS_ON, so ignore bad thermistor readings
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

#define DEBUG_PIN           0

// RS485 pins
#define TX_ENABLE_PIN      12
#define RX_ENABLE_PIN      13

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN    10   // Pin should have a pullup/pulldown!
#define SPINDLE_DIR_PIN          11
#if GEN7_VERSION < 13
  #define SPINDLE_LASER_PWM_PIN  16   // MUST BE HARDWARE PWM
#else  // Gen7 v1.3 removed the I2C connector & signals so need to get PWM off the PC power supply header
  #define SPINDLE_LASER_PWM_PIN  15   // MUST BE HARDWARE PWM
#endif
