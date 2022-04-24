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

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Longer3D LGT KIT V1.0 board only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#if SERIAL_PORT == 1 || SERIAL_PORT_2 == 1 || SERIAL_PORT_3 == 1
  #warning "Serial 1 is originally reserved to DGUS LCD."
#endif
#if SERIAL_PORT == 2 || SERIAL_PORT_2 == 2 || SERIAL_PORT_3 == 2 || LCD_SERIAL_PORT == 2
  #warning "Serial 2 has no connector. Hardware changes may be required to use it."
#endif
#if SERIAL_PORT == 3 || SERIAL_PORT_2 == 3 || SERIAL_PORT_3 == 3 || LCD_SERIAL_PORT == 3
  #define CHANGE_Y_LIMIT_PINS
  #warning "Serial 3 is originally reserved to Y limit switches. Hardware changes are required to use it."
#endif

// Custom flags and defines for the build
//#define BOARD_CUSTOM_BUILD_FLAGS -D__FOO__

#define BOARD_INFO_NAME "LGT KIT V1.0"

//
// Servos
//
#if !MB(LONGER3D_LK1_PRO)
  #define SERVO0_PIN                           7
#endif
#define SERVO1_PIN -1
#define SERVO2_PIN -1
#define SERVO3_PIN -1

//
// Limit Switches
//
#define X_STOP_PIN                             3

#ifdef CHANGE_Y_LIMIT_PINS
  #define Y_STOP_PIN                          37
#else
  #define Y_MIN_PIN                           14
  #define Y_MAX_PIN                           15
#endif

#if !MB(LONGER3D_LK1_PRO)
  #ifdef CHANGE_Y_LIMIT_PINS
    #define Z_STOP_PIN                        35
  #else
    #define Z_MIN_PIN                         35
    #define Z_MAX_PIN                         37
  #endif
#else
  #define Z_MIN_PIN                           11
  #define Z_MAX_PIN                           37
#endif

#undef CHANGE_Y_LIMIT_PINS

//
// Steppers - No E1 pins
//
#define E1_STEP_PIN                           -1
#define E1_DIR_PIN                            -1
#define E1_ENABLE_PIN                         -1
#define E1_CS_PIN                             -1

//
// Z Probe (when not Z_MIN_PIN)
//
#define Z_MIN_PROBE_PIN                       -1

//
// Temperature Sensors
//
#define TEMP_1_PIN                            -1

//
// Průša i3 MK2 Multiplexer Support
//
#if HAS_PRUSA_MMU1
  #define E_MUX2_PIN                          -1
#endif

//
// Misc. Functions
//
#define SD_DETECT_PIN                         49
#define FIL_RUNOUT_PIN                         2

//
// Other RAMPS 1.3 pins
//
#include "pins_RAMPS_13.h" // ... RAMPS
