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
 * Mega controller pin assignments
 * ATmega2560
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Mega Controller supports up to 2 hotends / E steppers."
#endif

#include "env_validate.h"

#define BOARD_INFO_NAME "Mega Controller"

//
// Servos
//
#define SERVO0_PIN                            PinC7
#define SERVO1_PIN                            PinC6
#define SERVO2_PIN                            PinC5
#define SERVO3_PIN                            PinC4

//
// Limit Switches
//
#define X_MIN_PIN                             PinL6
#define X_MAX_PIN                             PinL7
#define Y_MIN_PIN                             PinD7
#define Y_MAX_PIN                             PinG0
#define Z_MIN_PIN                             PinG1
#define Z_MAX_PIN                             PinC0

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC0
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinK0  // A8
#define X_DIR_PIN                             PinK1  // A9
#define X_ENABLE_PIN                          PinF7  // A7

#define Y_STEP_PIN                            PinK3  // A11
#define Y_DIR_PIN                             PinK4  // A12
#define Y_ENABLE_PIN                          PinK2  // A10

#define Z_STEP_PIN                            PinK6  // A14
#define Z_DIR_PIN                             PinK7  // A15
#define Z_ENABLE_PIN                          PinK5  // A13

#define E0_STEP_PIN                           PinA1
#define E0_DIR_PIN                            PinA2
#define E0_ENABLE_PIN                         PinA0

#define E1_STEP_PIN                           PinA4
#define E1_DIR_PIN                            PinA5
#define E1_ENABLE_PIN                         PinA3

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN                           PinG5  // Analog Input
#else
  #define TEMP_0_PIN                           PinE0  // Analog Input
#endif

#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN                           PinE3  // Analog Input
#else
  #define TEMP_1_PIN                           PinE4  // Analog Input
#endif

#define TEMP_2_PIN                             PinE5  // Analog Input

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN                         PinH3  // Analog Input
#else
  #define TEMP_BED_PIN                         PinE1  // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinA7
#define HEATER_1_PIN                          PinC3
#define HEATER_BED_PIN                        PinA6

#ifndef FAN_PIN
  #define FAN_PIN                             PinG2
#endif
#define FAN1_PIN                              PinC2
#define FAN2_PIN                              PinC1

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN            FAN2_PIN
#endif

#define FAN_SOFT_PWM_REQUIRED

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinE4
#endif

//
// LCD / Controller
//
#if ENABLED(MINIPANEL)

  #define BEEPER_PIN                          PinL3

  #define DOGLCD_A0                           PinL2
  #define DOGLCD_CS                           PinL4
  #define LCD_BACKLIGHT_PIN                   PinL5  // backlight LED on PA3

  #define KILL_PIN                            PinB6

  #define BTN_EN1                             PinL1
  #define BTN_EN2                             PinB5
  #define BTN_ENC                             PinB4

  #define SD_DETECT_PIN                       PinL0

  //#define LCD_SCREEN_ROTATE                180  // 0, 90, 180, 270

#endif // MINIPANEL

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN                 PinH3  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 PinH4  // Pullup!
#define SPINDLE_DIR_PIN                       PinH5
