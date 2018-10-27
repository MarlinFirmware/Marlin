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
 * RIGIDBOARD Arduino Mega with RAMPS v1.4 pin assignments
 */

#define BOARD_NAME "RigidBoard"

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  19    // Z-MAX pin J14 End Stops
#endif

//
// MOSFET changes
//
#define RAMPS_D10_PIN       9   // EXTRUDER 1
#define MOSFET_D_PIN       12   // EXTRUDER 2 or FAN

#include "pins_RAMPS.h"

//
// Steppers
//
// RigidBot swaps E0 / E1 plugs vs RAMPS 1.3
#undef E0_STEP_PIN
#undef E0_DIR_PIN
#undef E0_ENABLE_PIN
#define E0_STEP_PIN        36
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30

#undef E1_STEP_PIN
#undef E1_DIR_PIN
#undef E1_ENABLE_PIN
#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24

#define STEPPER_RESET_PIN  41   // Stepper drivers have a reset on RigidBot

//
// Temperature Sensors
//
#undef TEMP_0_PIN
#undef TEMP_1_PIN
#undef TEMP_BED_PIN
#define TEMP_0_PIN         14   // Analog Input
#define TEMP_1_PIN         13   // Analog Input
#define TEMP_BED_PIN       15   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#undef MAX6675_SS
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       53   // Don't use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS       49   // Don't use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

//
// Heaters / Fans
//
#undef HEATER_BED_PIN
#define HEATER_BED_PIN     10

#ifndef FAN_PIN
  #define FAN_PIN           8   // Same as RAMPS_13_EEF
#endif

//
// Misc. Functions
//
#undef PS_ON_PIN
#define PS_ON_PIN          -1

//
// LCD / Controller
//
// LCD Panel options for the RigidBoard
#if ENABLED(RIGIDBOT_PANEL)

  #undef BEEPER_PIN
  #define BEEPER_PIN -1

  // Direction buttons
  #define BTN_UP           37
  #define BTN_DWN          35
  #define BTN_LFT          33
  #define BTN_RT           32

  // 'R' button
  #undef BTN_ENC
  #define BTN_ENC 31

  // Disable encoder
  #undef BTN_EN1
  #define BTN_EN1 -1
  #undef BTN_EN2
  #define BTN_EN2 -1

  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN 22

#elif ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN 22

  #undef KILL_PIN
  #define KILL_PIN 32

#endif
