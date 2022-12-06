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
 * Arduino Mega with PICA pin assignments
 * ATmega2560
 *
 * PICA is Power, Interface, and Control Adapter and is open source hardware.
 * See https://github.com/mjrice/PICA for schematics etc.
 *
 * Applies to PICA, PICA_REVB
 */

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "PICA"
#endif

/*
// Note that these are the "pins" that correspond to the analog inputs on the arduino mega.
// These are not the same as the physical pin numbers
  AD0 = 54;   AD1 = 55;   AD2 = 56;   AD3 = 57;
  AD4 = 58;   AD5 = 59;   AD6 = 60;   AD7 = 61;
  AD8 = 62;   AD9 = 63;   AD10 = 64;  AD11 = 65;
  AD12 = 66;  AD13 = 67;  AD14 = 68;  AD15 = 69;
*/

//
// Servos
//
#define SERVO0_PIN                            PinE5
#define SERVO1_PIN                            PinG5
#define SERVO2_PIN                            PinE3

//
// Limit Switches
//
#define X_MIN_PIN                             PinJ1
#define X_MAX_PIN                             PinJ0
#define Y_MIN_PIN                             PinH1
#define Y_MAX_PIN                             PinH0
#define Z_MIN_PIN                             PinA1
#define Z_MAX_PIN                             PinA0

//
// Steppers
//
#define X_STEP_PIN                            PinF1
#define X_DIR_PIN                             PinF0
#define X_ENABLE_PIN                          PinF6

#define Y_STEP_PIN                            PinF3
#define Y_DIR_PIN                             PinF2
#define Y_ENABLE_PIN                          PinF7

#define Z_STEP_PIN                            PinF5
#define Z_DIR_PIN                             PinF4
#define Z_ENABLE_PIN                          PinK0

#define E0_STEP_PIN                           PinK5
#define E0_DIR_PIN                            PinA2
#define E0_ENABLE_PIN                         PinA4

#define E1_STEP_PIN                           PinK6
#define E1_DIR_PIN                            PinA6
#define E1_ENABLE_PIN                         PinA5

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH6  // Analog Input
#define TEMP_1_PIN                            PinB4
#define TEMP_BED_PIN                          PinB4
#define TEMP_2_PIN                            PinB5
#define TEMP_3_PIN                            PinB6

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                        PinB4  // E0
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN                        PinE4  // E1
#endif
#define HEATER_BED_PIN                        PinH5  // HEAT-BED

#ifndef FAN_PIN
  #define FAN_PIN                             PinH6
#endif
#ifndef FAN_2_PIN
  #define FAN_2_PIN                           PinH4
#endif

#define SDPOWER_PIN                           -1
#define LED_PIN                               -1
#define PS_ON_PIN                             -1
#define KILL_PIN                              -1

#define SSR_PIN                               PinH3

// SPI for MAX Thermocouple
#if DISABLED(SDSUPPORT)
  #define TEMP_0_CS_PIN                       PinK4  // Don't use 53 if using Display/SD card
#else
  #define TEMP_0_CS_PIN                       PinK4  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// SD Support
//
#define SD_DETECT_PIN                         PinL0
#define SDSS                                  PinB0

//
// LCD / Controller
//
#define BEEPER_PIN                            PinA7

#if HAS_WIRED_LCD
  #define LCD_PINS_RS                         PinC4
  #define LCD_PINS_ENABLE                     PinC7
  #define LCD_PINS_D4                         PinC2
  #define LCD_PINS_D5                         PinC5
  #define LCD_PINS_D6                         PinC0
  #define LCD_PINS_D7                         PinC1

  #define BTN_EN1                             PinL2
  #define BTN_EN2                             PinL1
  #define BTN_ENC                             PinC6

  #define LCD_SDSS                            PinB0
#endif
