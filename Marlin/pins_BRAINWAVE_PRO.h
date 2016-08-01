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
 * Brainwave Pro pin assignments (AT90USB1286)
 *
 * Requires hardware bundle for Arduino:
 * https://github.com/unrepentantgeek/brainwave-arduino
 */

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Brainwave Pro' selected from the 'Tools -> Boards' menu."
#endif

#include "fastio.h"

#ifndef AT90USBxx_TEENSYPP_ASSIGNMENTS  // use Teensyduino Teensy++2.0 pin assignments instead of Marlin alphabetical.
  #error "Uncomment #define AT90USBxx_TEENSYPP_ASSIGNMENTS in fastio.h for this config"
#endif

#define BOARD_NAME         "Brainwave Pro"

#define LARGE_FLASH        true

#define X_STEP_PIN         33
#define X_DIR_PIN          32
#define X_ENABLE_PIN       11
#define X_STOP_PIN         47

#define Y_STEP_PIN         31
#define Y_DIR_PIN          30
#define Y_ENABLE_PIN        8
#define Y_STOP_PIN         18

#define Z_STEP_PIN         29
#define Z_DIR_PIN          28
#define Z_ENABLE_PIN       37
#define Z_MAX_PIN          36
#define Z_MIN_PROBE_PIN    17  // Bed Z probe

#define E0_STEP_PIN        35
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      13

#define HEATER_0_PIN       15
#define HEATER_BED_PIN     14  // Bed
#define FAN_PIN            16  // Fan, PWM

#define TEMP_0_PIN          2  // Extruder / Analog pin numbering
#define TEMP_1_PIN          1  // Spare / Analog pin numbering
#define TEMP_BED_PIN        0  // Bed / Analog pin numbering

#define SDSS               20
#define LED_PIN            19
#define SD_DETECT_PIN      12
