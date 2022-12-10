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
 * Mini-RAMBo pin assignments
 */

#include "env_validate.h"

#if MB(MINIRAMBO_10A)
  #define BOARD_INFO_NAME "Mini RAMBo 1.0a"
#else
  #define BOARD_INFO_NAME "Mini RAMBo"
#endif

//
// Limit Switches
//
#define X_MIN_PIN                             PinB6
#define X_MAX_PIN                             PinC7
#define Y_MIN_PIN                             PinB5
#define Y_MAX_PIN                             PinA2
#define Z_MIN_PIN                             PinB4
#define Z_MAX_PIN                             PinA1

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinA1
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

// Microstepping pins
#define X_MS1_PIN                             PinG1
#define X_MS2_PIN                             PinG0
#define Y_MS1_PIN                             PinK7
#define Y_MS2_PIN                             PinG2
#define Z_MS1_PIN                             PinK6
#define Z_MS2_PIN                             PinK5
#define E0_MS1_PIN                            PinK3
#define E0_MS2_PIN                            PinK4

#define MOTOR_CURRENT_PWM_XY_PIN              PinL3
#define MOTOR_CURRENT_PWM_Z_PIN               PinL4
#define MOTOR_CURRENT_PWM_E_PIN               PinL5
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_1_PIN                            PinE1  // Analog Input
#define TEMP_BED_PIN                          PinE4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE5
#define HEATER_1_PIN                          PinH4
#if !MB(MINIRAMBO_10A)
  #define HEATER_2_PIN                        PinH3
#endif
#define HEATER_BED_PIN                        PinG5

#ifndef FAN_PIN
  #define FAN_PIN                             PinH5
#endif
#define FAN1_PIN                              PinH3

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#if !MB(MINIRAMBO_10A)
  #define CASE_LIGHT_PIN                      PinH6
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
// use P1 connector for spindle pins
#define SPINDLE_LASER_PWM_PIN                 PinH6  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 PinD3  // Pullup!
#define SPINDLE_DIR_PIN                       PinD2

//
// Průša i3 MK2 Multiplexer Support
//
#if HAS_PRUSA_MMU1
  #define E_MUX0_PIN                          PinH0
  #define E_MUX1_PIN                          PinH1
  #if !MB(MINIRAMBO_10A)
    #define E_MUX2_PIN                        PinE2  // 84 in MK2 Firmware, with BEEPER as 78
  #endif
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD || TOUCH_UI_ULTIPANEL

  #if !MB(MINIRAMBO_10A)
    #define KILL_PIN                          PinC5
  #endif

  #if IS_ULTIPANEL || TOUCH_UI_ULTIPANEL

    #if MB(MINIRAMBO_10A)

      #define BEEPER_PIN                      PinE2

      #define BTN_EN1                         PinE7
      #define BTN_EN2                         PinJ3
      #define BTN_ENC                         PinD0

      #define LCD_PINS_RS                     PinD7
      #define LCD_PINS_ENABLE                 PinE3
      #define LCD_PINS_D4                     PinJ1
      #define LCD_PINS_D5                     PinJ0
      #define LCD_PINS_D6                     PinC5
      #define LCD_PINS_D7                     PinC6

      #define SD_DETECT_PIN                   PinJ2

    #else                                         // !MINIRAMBO_10A

      // AUX-4
      #define BEEPER_PIN                      PinH2

      // AUX-2
      #define BTN_EN1                         PinJ1
      #define BTN_EN2                         PinJ2
      #define BTN_ENC                         PinH6

      #define LCD_PINS_RS                     PinD5
      #define LCD_PINS_ENABLE                 PinD3
      #define LCD_PINS_D4                     PinD2
      #define LCD_PINS_D5                     PinG4
      #define LCD_PINS_D6                     PinH7
      #define LCD_PINS_D7                     PinG3

      #define SD_DETECT_PIN                   PinJ0

    #endif // !MINIRAMBO_10A

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

  #endif // IS_ULTIPANEL || TOUCH_UI_ULTIPANEL

#endif // HAS_WIRED_LCD || TOUCH_UI_ULTIPANEL

#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                 0
  #define BOARD_ST7920_DELAY_2               250
  #define BOARD_ST7920_DELAY_3                 0
#endif
