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
 * Ultimaker pin assignments
 * ATmega2560, ATmega1280
 */

/**
 * Rev B   2 JAN 2017
 *
 *  Added pin definitions for:
 *    M3, M4 & M5 spindle control commands
 *    case light
 */

#include "env_validate.h"

#define BOARD_INFO_NAME         "Ultimaker"
#define DEFAULT_MACHINE_NAME    BOARD_INFO_NAME
#define DEFAULT_SOURCE_CODE_URL "github.com/Ultimaker/Marlin"

// Just a guess?
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#define SERVO0_PIN                            PinB5

//
// Limit Switches
//
#define X_MIN_PIN                             PinA0
#define X_MAX_PIN                             PinA2
#define Y_MIN_PIN                             PinA4
#define Y_MAX_PIN                             PinA6
#define Z_MIN_PIN                             PinC7
#define Z_MAX_PIN                             PinC5

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC5
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5

#define Y_STEP_PIN                            PinC6
#define Y_DIR_PIN                             PinC4
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinC0
#define Z_DIR_PIN                             PinG2
#define Z_ENABLE_PIN                          PinC2

#define E0_STEP_PIN                           PinL6
#define E0_DIR_PIN                            PinL4
#define E0_ENABLE_PIN                         PinG0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

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

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinB6
#define SUICIDE_PIN                           PinF0  // PIN that has to be turned on right after start, to keep power flowing.

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinH5
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #define BEEPER_PIN                          PinD3

  #if IS_NEWPANEL

    #define LCD_PINS_RS                       PinD1
    #define LCD_PINS_ENABLE                   PinH0
    #define LCD_PINS_D4                       PinH1
    #define LCD_PINS_D5                       PinD0
    #define LCD_PINS_D6                       PinE3
    #define LCD_PINS_D7                       PinH3

    // Buttons directly attached
    #define BTN_EN1                           PinG1
    #define BTN_EN2                           PinL7
    #define BTN_ENC                           PinD2

    #define SD_DETECT_PIN                     PinD7

  #else                                           // !IS_NEWPANEL - Old style panel with shift register

    // Buttons attached to a shift register
    #define SHIFT_CLK_PIN                     PinD7
    #define SHIFT_LD_PIN                      PinL7
    #define SHIFT_OUT_PIN                     PinG1
    #define SHIFT_EN_PIN                      PinH0

    #define LCD_PINS_RS                       PinH1
    #define LCD_PINS_ENABLE                   PinE3
    #define LCD_PINS_D4                       PinH3
    #define LCD_PINS_D5                       PinD0
    #define LCD_PINS_D6                       PinD1
    #define LCD_PINS_D7                       PinD2

    #define SD_DETECT_PIN                     -1

  #endif // !IS_NEWPANEL

#endif // HAS_WIRED_LCD

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN                 PinH6  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 PinB4  // Pullup!
#define SPINDLE_DIR_PIN                       PinB5  // use the EXP3 PWM header
