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
 * MegaTronics pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Megatronics"
//
// Limit Switches
//
#define X_MIN_PIN                             PinG0
#define X_MAX_PIN                             PinC0
#define Y_MIN_PIN                             PinJ1
#define Y_MAX_PIN                             PinJ0
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

#define Y_STEP_PIN                            PinF6  // A6 (TODO: wtf? pin assignment? ff.)
#define Y_DIR_PIN                             PinF7  // A7
#define Y_ENABLE_PIN                          

#define Z_STEP_PIN                            PinF0  // A0
#define Z_DIR_PIN                             PinF1  // A1
#define Z_ENABLE_PIN                          PinF2  // A2

#define E0_STEP_PIN                           PinC6
#define E0_DIR_PIN                            PinC5
#define E0_ENABLE_PIN                         PinD7

#define E1_STEP_PIN                           PinC3
#define E1_DIR_PIN                            PinC1
#define E1_ENABLE_PIN                         PinC7

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN                           PinH5  // Analog Input
#else
  #define TEMP_0_PIN                          PinB7  // Analog Input
#endif
#define TEMP_1_PIN                            PinJ0  // Analog Input
#define TEMP_BED_PIN                          PinJ1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           PinH6
#define HEATER_1_PIN                           PinH5
#define HEATER_BED_PIN                        PinB4

#ifndef FAN_PIN
  #define FAN_PIN                              PinH4  // IO pin. Buffer needed
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinB6

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                       PinE4
#endif

//
// LCD / Controller
//
#define BEEPER_PIN                            PinC4

#if HAS_WIRED_LCD && IS_NEWPANEL

  #define LCD_PINS_RS                         PinH1
  #define LCD_PINS_ENABLE                     PinH0
  #define LCD_PINS_D4                         PinA1
  #define LCD_PINS_D5                         PinA3
  #define LCD_PINS_D6                         PinA5
  #define LCD_PINS_D7                         PinA7

  // Buttons directly attached to AUX-2
  #define BTN_EN1                             PinF5
  #define BTN_EN2                             PinK2
  #define BTN_ENC                             PinL6

  #define SD_DETECT_PIN                       -1  // RAMPS doesn't use this

#endif // HAS_WIRED_LCD && IS_NEWPANEL

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN                  PinE5  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                  PinG5  // Pullup!
#define SPINDLE_DIR_PIN                       PinB5
