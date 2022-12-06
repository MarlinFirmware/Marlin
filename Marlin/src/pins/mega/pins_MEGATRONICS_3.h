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
 * MegaTronics v3.0 / v3.1 / v3.2 pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#if MB(MEGATRONICS_32)
  #define BOARD_INFO_NAME "Megatronics v3.2"
#elif MB(MEGATRONICS_31)
  #define BOARD_INFO_NAME "Megatronics v3.1"
#else
  #define BOARD_INFO_NAME "Megatronics v3.0"
#endif

//
// Servos
//
#define SERVO0_PIN                            PinL3  // AUX3-6
#define SERVO1_PIN                            PinL2  // AUX3-5
#define SERVO2_PIN                            PinL1  // AUX3-4
#define SERVO3_PIN                            PinL0  // AUX3-3

//
// Limit Switches
//
#define X_MIN_PIN                             PinC0  // No INT
#define X_MAX_PIN                             PinG1  // No INT
#define Y_MIN_PIN                             PinG0  // No INT
#define Y_MAX_PIN                             PinD7  // No INT
#define Z_MIN_PIN                             PinD3  // No INT
#define Z_MAX_PIN                             PinD2  // No INT

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinD2
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF4
#define X_DIR_PIN                             PinF3
#define X_ENABLE_PIN                          PinF5

#if ENABLED(REPRAPWORLD_KEYPAD) && EXTRUDERS <= 2
  #define Y_ENABLE_PIN                        PinA1
  #define Y_STEP_PIN                          PinA0
  #define Y_DIR_PIN                           PinF6
#else
  #define Y_STEP_PIN                          PinE3
  #define Y_DIR_PIN                           PinH0
  #define Y_ENABLE_PIN                        PinG5

  #define E2_STEP_PIN                         PinA0
  #define E2_DIR_PIN                          PinF6
  #define E2_ENABLE_PIN                       PinA1
#endif

#define Z_STEP_PIN                            PinH1
#define Z_DIR_PIN                             PinB5
#define Z_ENABLE_PIN                          PinE5

#define E0_STEP_PIN                           PinA6
#define E0_DIR_PIN                            PinA5
#define E0_ENABLE_PIN                         PinA7

#define E1_STEP_PIN                           PinA3
#define E1_DIR_PIN                            PinA2
#define E1_ENABLE_PIN                         PinA4

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN                          PinB5  // Analog Input
#else
  #define TEMP_0_PIN                          PinJ0  // Analog Input
#endif
#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN                          PinB4  // Analog Input
#else
  #define TEMP_1_PIN                          PinB7  // Analog Input
#endif
#if TEMP_SENSOR_2 == -1
  #define TEMP_2_PIN                          PinH6  // Analog Input
#else
  #define TEMP_2_PIN                          PinB6  // Analog Input
#endif
#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN                        PinH5  // Analog Input
#else
  #define TEMP_BED_PIN                        PinJ1  // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinH6
#define HEATER_2_PIN                          PinH5
#define HEATER_BED_PIN                        PinB4

#ifndef FAN_PIN
  #define FAN_PIN                             PinH3
#endif
#define FAN1_PIN                              PinH4

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinB6

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinL4  // Try the keypad connector
#endif

//
// LCD / Controller
//
#define BEEPER_PIN                            PinF7

#define BTN_EN1                               PinL5
#define BTN_EN2                               PinL4
#define BTN_ENC                               PinC4

#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

  #define LCD_PINS_RS                         PinF2  // CS chip select / SS chip slave select
  #define LCD_PINS_ENABLE                     PinB2  // SID (MOSI)
  #define LCD_PINS_D4                         PinB1  // SCK (CLK) clock
  #define SD_DETECT_PIN                       PinC2

#else

  #define LCD_PINS_RS                         PinC5
  #define LCD_PINS_ENABLE                     PinC6
  #define LCD_PINS_D4                         PinJ1
  #define LCD_PINS_D5                         PinC7
  #define LCD_PINS_D6                         PinG2
  #define LCD_PINS_D7                         PinJ0

  #define SHIFT_CLK_PIN                       PinL6
  #define SHIFT_LD_PIN                        PinC2
  #define SHIFT_OUT_PIN                       PinC3
  #define SHIFT_EN_PIN                        PinL5

  #if MB(MEGATRONICS_31, MEGATRONICS_32)
    #define SD_DETECT_PIN                     PinF2
  #endif

#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if DISABLED(REPRAPWORLD_KEYPAD)                  // try to use the keypad connector first
  #define SPINDLE_LASER_PWM_PIN               PinL5  // Hardware PWM
  #define SPINDLE_LASER_ENA_PIN               PinL6  // Pullup!
  #define SPINDLE_DIR_PIN                     PinL7
#elif EXTRUDERS <= 2
  // Hijack the last extruder so that we can get the PWM signal off the Y breakout
  // Move Y to the E2 plug. This makes dual Y steppers harder
  #define SPINDLE_LASER_PWM_PIN               PinG5  // Hardware PWM
  #define SPINDLE_LASER_ENA_PIN               PinH0  // Pullup!
  #define SPINDLE_DIR_PIN                     PinE3
#endif
