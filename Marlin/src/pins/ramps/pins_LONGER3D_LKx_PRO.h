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
 * Longer3D LK1/LK4/LK5 Pro board pin assignments
 */

#ifndef BOARD_LONGER3D_LK1_PRO
  #error "BOARD_LONGER3D_LK1_PRO no longer defined. Remember to change all MB(LONGER3D_LK1_PRO) occurences."
#endif

#if NOT_TARGET(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "Longer3D LGT KIT V1.0 board only supports 1 hotend / E-stepper. Comment out this line to continue."
#endif

#if SERIAL_PORT == 1 || SERIAL_PORT_2 == 1
  #warning "Serial 1 is originally reserved to DGUS LCD."
#endif
#if SERIAL_PORT == 2 || SERIAL_PORT_2 == 2
  #warning "Serial 2 has not a connector. To get it working, hardware changes are required."
#endif
#if SERIAL_PORT == 3 || SERIAL_PORT_2 == 3
  #define CHANGE_Y_LIMIT_PINS
  #warning "Serial 3 is originally reserved to Y limit switches. To get it working, hardware changes are required."
#endif

// Custom flags and defines for the build
//#define BOARD_CUSTOM_BUILD_FLAGS -D__FOO__

#define BOARD_INFO_NAME "LGT KIT V1.0"

//
// Misc. Functions
//
#define SD_DETECT_PIN                         49
#define FIL_RUNOUT_PIN                         2

//
// Other RAMPS 1.3 pins
//
#define IS_RAMPS_EFB                              // Override autodetection. Bed will be undefined.
#include "pins_RAMPS_13.h"

//
// Servos
//
#ifdef SERVO0_PIN
  #undef SERVO0_PIN
#endif
#ifdef SERVO1_PIN
  #undef SERVO1_PIN
#endif
#ifdef SERVO2_PIN
  #undef SERVO2_PIN
#endif
#ifdef SERVO3_PIN
  #undef SERVO3_PIN
#endif

#if !MB(LONGER3D_LK1_PRO)
  #define SERVO0_PIN                           7
#endif

//
// Limit Switches
//
#ifdef X_MAX_PIN
  #undef X_MAX_PIN
#endif
#ifdef Y_MIN_PIN
  #undef Y_MIN_PIN
#endif
#ifdef Y_MAX_PIN
  #undef Y_MAX_PIN
#endif
#ifdef Z_MIN_PIN
  #undef Z_MIN_PIN
#endif
#ifdef Z_MAX_PIN
  #undef Z_MAX_PIN
#endif

#ifndef CHANGE_Y_LIMIT_PINS
  #define Y_MIN_PIN                           14
  #define Y_MAX_PIN                           15
#else
  #define Y_MIN_PIN                           37
#endif

#if !MB(LONGER3D_LK1_PRO)
  #define Z_MIN_PIN                           35
  #ifndef CHANGE_Y_LIMIT_PINS
    #define Z_MAX_PIN                         37
  #endif
#else
  #define Z_MIN_PIN                           11
  #define Z_MAX_PIN                           37
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifdef Z_MIN_PROBE_PIN
  #undef Z_MIN_PROBE_PIN
#endif

//
// Steppers
//
#ifdef E1_STEP_PIN
  #undef E1_STEP_PIN
#endif
#ifdef E1_DIR_PIN
  #undef E1_DIR_PIN
#endif
#ifdef E1_ENABLE_PIN
  #undef E1_ENABLE_PIN
#endif
#ifdef E1_CS_PIN
  #undef E1_CS_PIN
#endif

//
// Temperature Sensors
//
#ifdef TEMP_1_PIN
  #undef TEMP_1_PIN
#endif

//
// Průša i3 MK2 Multiplexer Support
//
#ifdef E_MUX2_PIN
  #undef E_MUX2_PIN
#endif

#ifdef CHANGE_Y_LIMIT_PINS
  #undef CHANGE_Y_LIMIT_PINS
#endif