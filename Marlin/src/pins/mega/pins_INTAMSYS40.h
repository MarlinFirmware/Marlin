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
 * Intamsys Funmat HT V4.0 Mainboard
 * ATmega2560
 * 4988 Drivers Tested
 * 2208 version exists and may or may not work
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Intamsys 4.0"

//
// Servos
//
#define SERVO0_PIN                            PinB6  // Uses High Temp Present Jumper Pin

//
// Limit Switches
//
#define X_STOP_PIN                            PinA0
#define Y_STOP_PIN                            PinA4
#define Z_MIN_PIN                             PinA7
#define Z_MAX_PIN                             PinK7

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinK7
#endif

#define FIL_RUNOUT_PIN                        PinB4

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5  // 44

#define Y_STEP_PIN                            PinC5  // 33
#define Y_DIR_PIN                             PinC4  // 31, 32
#define Y_ENABLE_PIN                          PinC6  // 32

#define Z_STEP_PIN                            PinC2  // 35
#define Z_DIR_PIN                             PinC1
#define Z_ENABLE_PIN                          PinC3  // 34

#define E0_STEP_PIN                           PinL7
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinC0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

#define MOTOR_CURRENT_PWM_X_PIN               PinB5
#define MOTOR_CURRENT_PWM_Y_PIN               PinL5
#define MOTOR_CURRENT_PWM_Z_PIN               PinL4
#define MOTOR_CURRENT_PWM_E_PIN               PinL3

// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  { 1300, 1300, 1250 }

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5  // Analog Input D62
#define TEMP_BED_PIN                          PinB4  // Analog Input D64

#define TEMP_CHAMBER_PIN                      PinH6  // Analog Input D63

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4  // PWM
#define HEATER_BED_PIN                        PinG5  // PWM
#define HEATER_CHAMBER_PIN                    PinE5  // PWM
#define FAN_PIN                               PinH4  // PWM

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinG2

#if ENABLED(CASE_LIGHT_ENABLE)
  #define CASE_LIGHT_PIN                      PinH5
#endif

#if ENABLED(PSU_CONTROL)
  #define PS_ON_PIN                           PinD7  // UPS Module
#endif

//
// LCD Controller
//

#define BEEPER_PIN                            PinD3

#if HAS_WIRED_LCD
  #define LCD_PINS_RS                         PinD1
  #define LCD_PINS_ENABLE                     PinC7
  #define LCD_PINS_D4                         PinJ1
  #define LCD_PINS_D5                         PinD0
  #define LCD_PINS_D6                         PinE3
  #define LCD_PINS_D7                         PinH3
  #define BTN_EN1                             PinG1
  #define BTN_EN2                             PinG0
  #define BTN_ENC                             PinD2
#endif

///////////////////// SPARE HEADERS //////////////

/**
 * J25
 * 1 D54
 * 2 D55
 * 3 D56
 * 4 D57
 * 5 D58
 * 6 D59
 * 7 D60
 * 8 D61

Hotend High Temp Connected : D12
*/
