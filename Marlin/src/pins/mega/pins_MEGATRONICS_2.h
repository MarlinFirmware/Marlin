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
 * MegaTronics v2.0 pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Megatronics v2.0"
//
// Limit Switches
//
#define X_MIN_PIN                             PinC0
#define X_MAX_PIN                             PinG1
#define Y_MIN_PIN                             PinG0
#define Y_MAX_PIN                             PinD7
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
#define X_DIR_PIN                             PinA5
#define X_ENABLE_PIN                          PinA3

#define Y_STEP_PIN                            PinG5  // A6 (??? pin mismatch!!!)
#define Y_DIR_PIN                             PinF0  // A0
#define Y_ENABLE_PIN                          PinE3

#define Z_STEP_PIN                            PinF2  // A2
#define Z_DIR_PIN                             PinF6  // A6
#define Z_ENABLE_PIN                          PinF1  // A1

#define E0_STEP_PIN                           PinC2
#define E0_DIR_PIN                            PinC1
#define E0_ENABLE_PIN                         PinC3

#define E1_STEP_PIN                           PinA7
#define E1_DIR_PIN                            PinG2
#define E1_ENABLE_PIN                         PinA6
 
#define E2_STEP_PIN                           PinA1  // ? schematic says 24 (this comment is a sign of confusion by maintainers about the internal schematic numbering VS the internal numbering of AVR Marlin FW which should not matter!)
#define E2_DIR_PIN                            PinA2  // ? schematic says 23
#define E2_ENABLE_PIN                         PinA0

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN                          PinG5  // Analog Input
#else
  #define TEMP_0_PIN                          PinB7  // Analog Input
#endif

#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN                          PinH5  // Analog Input
#else
  #define TEMP_1_PIN                          PinJ0  // Analog Input
#endif

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN                        PinH5  // Analog Input
#else
  #define TEMP_BED_PIN                        PinJ1  // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH6
#define HEATER_1_PIN                          PinH5
#define HEATER_BED_PIN                        PinB4

#ifndef FAN_PIN
  #define FAN_PIN                             PinH4
#endif
#define FAN1_PIN                              PinH3

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinB6

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinE4
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN                 PinE5  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 PinH1  // Pullup!
#define SPINDLE_DIR_PIN                       PinB5

//
// LCD / Controller
//
#define BEEPER_PIN                            PinK2

#if HAS_WIRED_LCD

  #define LCD_PINS_RS                         PinJ1
  #define LCD_PINS_ENABLE                     PinJ0
  #define LCD_PINS_D4                         PinC7
  #define LCD_PINS_D5                         PinC6
  #define LCD_PINS_D6                         PinC5
  #define LCD_PINS_D7                         PinC4

  #if IS_NEWPANEL
    // Buttons are directly attached using keypad
    #define BTN_EN1                           PinF7
    #define BTN_EN2                           PinF5
    #define BTN_ENC                           PinL6
  #else
    // Buttons attached to shift register of reprapworld keypad v1.1
    #define SHIFT_CLK_PIN                     PinK1
    #define SHIFT_LD_PIN                      PinL7
    #define SHIFT_OUT_PIN                     PinH0
    #define SHIFT_EN_PIN                      PinH0
  #endif

#endif // HAS_WIRED_LCD
