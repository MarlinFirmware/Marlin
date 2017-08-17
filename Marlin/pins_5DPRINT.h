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
  #error Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu.
#endif

#define DEFAULT_MACHINE_NAME "Makibox"

#define LARGE_FLASH        true

#define X_STEP_PIN          0
#define X_DIR_PIN           1
#define X_ENABLE_PIN       23
#define X_STOP_PIN         37

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       19
#define Y_STOP_PIN         36

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5
#define Z_ENABLE_PIN       18
#define Z_STOP_PIN         39

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN      17

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     20  // Bed
// You may need to change FAN_PIN to 16 because Marlin isn't using fastio.h
// for the fan and Teensyduino uses a different pin mapping.
#define FAN_PIN            16  // Fan

#define TEMP_0_PIN          1  // Extruder / Analog pin numbering
#define TEMP_BED_PIN        0  // Bed / Analog pin numbering

#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1

#define SDPOWER            -1
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1

// The SDSS pin uses a different pin mapping from file Sd2PinMap.h
#define SDSS               20

#if DISABLED(SDSUPPORT)
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN           9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

// Microstepping pins
// Note that the pin mapping is not from fastio.h
// See Sd2PinMap.h for the pin configurations

#undef X_MS1_PIN
#undef X_MS2_PIN
#undef Y_MS1_PIN
#undef Y_MS2_PIN
#undef Z_MS1_PIN
#undef Z_MS2_PIN
#undef E0_MS1_PIN
#undef E0_MS2_PIN
#define X_MS1_PIN 25
#define X_MS2_PIN 26
#define Y_MS1_PIN 9
#define Y_MS2_PIN 8
#define Z_MS1_PIN 7
#define Z_MS2_PIN 6
#define E0_MS1_PIN 5
#define E0_MS2_PIN 4

