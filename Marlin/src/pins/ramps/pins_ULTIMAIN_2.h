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
 * Ultiboard v2.0 pin assignments
 * ATmega2560
 */

/**
 * Rev B   2 JAN 2017
 *
 *  Added pin definitions for:
 *    M3, M4 & M5 spindle control commands
 *    case light
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#define BOARD_INFO_NAME         "Ultimaker 2.x"
#define DEFAULT_MACHINE_NAME    "Ultimaker"
#define DEFAULT_SOURCE_CODE_URL "github.com/Ultimaker/Marlin"

//
// Limit Switches
//
#define X_STOP_PIN                            PinA0
#define Y_STOP_PIN                            PinA4
#define Z_STOP_PIN                            PinA7

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5

#define Y_STEP_PIN                            PinC5
#define Y_DIR_PIN                             PinC3
#define Y_ENABLE_PIN                          PinC6

#define Z_STEP_PIN                            PinC2
#define Z_DIR_PIN                             PinC1
#define Z_ENABLE_PIN                          PinC3

#define E0_STEP_PIN                           PinL7
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinC0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

#define MOTOR_CURRENT_PWM_XY_PIN              PinL5
#define MOTOR_CURRENT_PWM_Z_PIN               PinL4
#define MOTOR_CURRENT_PWM_E_PIN               PinL3
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5  // Analog Input
#define TEMP_1_PIN                            PinH6  // Analog Input
#define TEMP_BED_PIN                          PinB4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinE5
#define HEATER_BED_PIN                        PinG5

#ifndef FAN_PIN
  #define FAN_PIN                             PinH4
#endif

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     PinJ6
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinG2
#define LED_PIN                               PinH5
//#define SAFETY_TRIGGERED_PIN                PinA6  // PIN to detect the safety circuit has triggered
//#define MAIN_VOLTAGE_MEASURE_PIN            PinJ1  // ANALOG PIN to measure the main voltage, with a 100k - 4k7 resitor divider.

//
// LCD / Controller
//
#define BEEPER_PIN                            PinD3

#define LCD_PINS_RS                           PinD1
#define LCD_PINS_ENABLE                       PinJ0
#define LCD_PINS_D4                           PinJ1
#define LCD_PINS_D5                           PinD0
#define LCD_PINS_D6                           PinE3
#define LCD_PINS_D7                           PinH3

// Buttons are directly attached
#define BTN_EN1                               PinG1
#define BTN_EN2                               PinG0
#define BTN_ENC                               PinD2

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER                                    // use the LED_PIN for spindle speed control or case light
  #undef LED_PIN
  #define SPINDLE_DIR_PIN                     PinH1
  #define SPINDLE_LASER_ENA_PIN               PinH0  // Pullup!
  #define SPINDLE_LASER_PWM_PIN               PinH5  // Hardware PWM
#else
  #undef LED_PIN
  #define CASE_LIGHT_PIN                      PinH5
#endif
