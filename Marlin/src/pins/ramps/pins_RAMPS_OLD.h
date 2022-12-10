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
 * Arduino Mega with RAMPS v1.0, v1.1, v1.2 pin assignments
 * ATmega2560, ATmega1280
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "RAMPS <1.2"

// Uncomment the following line for RAMPS v1.0
//#define RAMPS_V_1_0

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinH1
#define Y_MAX_PIN                             PinH0
#define Z_MIN_PIN                             PinD3
#define Z_MAX_PIN                             PinD2

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinD2
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinA4
#define X_DIR_PIN                             PinA6
#define X_ENABLE_PIN                          PinA2

#define Y_STEP_PIN                            PinD7
#define Y_DIR_PIN                             PinG1
#define Y_ENABLE_PIN                          PinC1

#define Z_STEP_PIN                            PinL5
#define Z_DIR_PIN                             PinL3
#define Z_ENABLE_PIN                          PinL7

#define E0_STEP_PIN                           PinC5
#define E0_DIR_PIN                            PinC3
#define E0_ENABLE_PIN                         PinC7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE4  // Analog Input
#define TEMP_BED_PIN                          PinE1  // Analog Input

// SPI for MAX Thermocouple
#if DISABLED(SDSUPPORT)
  #define TEMP_0_CS_PIN                       PinK4  // Don't use 53 if using Display/SD card
#else
  #define TEMP_0_CS_PIN                       PinK4  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// Heaters / Fans
//
#if ENABLED(RAMPS_V_1_0)
  #define HEATER_0_PIN                        PinB6
  #define HEATER_BED_PIN                      -1
  #ifndef FAN_PIN
    #define FAN_PIN                           PinB5
  #endif
#else                                             // RAMPS_V_1_1 or RAMPS_V_1_2
  #define HEATER_0_PIN                        PinB4
  #define HEATER_BED_PIN                      PinH5
  #ifndef FAN_PIN
    #define FAN_PIN                           PinH6
  #endif
#endif

//
// Misc. Functions
//
#define SDPOWER_PIN                           PinL1
#define SDSS                                  PinB0
#define LED_PIN                               PinB7

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinL4  // Hardware PWM
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN                 PinG0  // Pullup or pulldown!
#define SPINDLE_LASER_PWM_PIN                 PinL4  // Hardware PWM
#define SPINDLE_DIR_PIN                       PinL6
