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

/************************************************
 * Rambo pin assignments MODIFIED FOR Scoovo X9H
 ************************************************/

#include "env_target.h"

#define BOARD_INFO_NAME "Scoovo X9H"

//
// Servos
//
#define SERVO0_PIN                            PinA0  // Motor header MX1
#define SERVO1_PIN                            PinA1  // Motor header MX2
#define SERVO2_PIN                            PinA2  // Motor header MX3
#define SERVO3_PIN                            PinE3  // PWM header pin 5

//
// Limit Switches
//
#define X_MIN_PIN                             PinB6
#define X_MAX_PIN                             PinA2
#define Y_MIN_PIN                             PinB5
#define Y_MAX_PIN                             PinA1
#define Z_MIN_PIN                             PinB4
#define Z_MAX_PIN                             PinC7

//
// Z Probe (when not Z_MIN_IN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC7
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinC0
#define X_DIR_PIN                             PinL1
#define X_ENABLE_PIN                          PinA7

#define Y_STEP_PIN                            PinC1
#define Y_DIR_PIN                             PinL0
#define Y_ENABLE_PIN                          PinA6

#define Z_STEP_PIN                            PinC2
#define Z_DIR_PIN                             PinL2
#define Z_ENABLE_PIN                          PinA5

#define E0_STEP_PIN                           PinC3
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinA4

#define E1_STEP_PIN                           PinC4
#define E1_DIR_PIN                            PinL7
#define E1_ENABLE_PIN                         PinA3

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN                             PinG1
#define X_MS2_PIN                             PinG0
#define Y_MS1_PIN                             PinK7
#define Y_MS2_PIN                             PinG2
#define Z_MS1_PIN                             PinK6
#define Z_MS2_PIN                             PinK5
#define E0_MS1_PIN                            PinK3
#define E0_MS2_PIN                            PinK4
#define E1_MS1_PIN                            PinK1
#define E1_MS2_PIN                            PinK2

#define DIGIPOTSS_PIN                         PinD7
#define DIGIPOT_CHANNELS { 4, 5, 3, 0, 1 }        // X Y Z E0 E1 digipot channels to stepper driver mapping

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_BED_PIN                          PinH4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH6
#define HEATER_1_PIN                          PinH4
#define HEATER_BED_PIN                        PinE5

#ifndef FAN_PIN
  #define FAN_PIN                             PinH5
#endif
#define FAN1_PIN                              PinH3
#define FAN2_PIN                              PinE4

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinG5

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinE5  // Analog Input
#endif

//
// LCD / Controller
//
#define LCD_PINS_RS                           PinG4  // Ext2_5
#define LCD_PINS_ENABLE                       PinG3  // Ext2_7
#define LCD_PINS_D4                           PinJ2  // Ext2_9 ?
#define LCD_PINS_D5                           PinJ3  // Ext2_11 ?
#define LCD_PINS_D6                           PinJ7  // Ext2_13
#define LCD_PINS_D7                           PinJ4  // Ext2_15 ?
#define BEEPER_PIN                            -1

#define BTN_HOME                              PinE7  // Ext_16
#define BTN_CENTER                            PinD4  // Ext_14
#define BTN_ENC                       BTN_CENTER
#define BTN_RIGHT                             PinD5  // Ext_12
#define BTN_LEFT                              PinD6  // Ext_10
#define BTN_UP                                PinH2  // Ext2_8
#define BTN_DOWN                              PinH7  // Ext2_6

#define HOME_PIN                        BTN_HOME

#if EITHER(VIKI2, miniVIKI)
  #define BEEPER_PIN                          PinL5
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0                           PinG4
  #define DOGLCD_CS                           PinG3

  #define SD_DETECT_PIN                       -1  // Pin 72 if using easy adapter board

  #define STAT_LED_RED_PIN                    PinA0
  #define STAT_LED_BLUE_PIN                   PinC5

  #define LCD_SCREEN_ROTATE                  180  // 0, 90, 180, 270
#endif
