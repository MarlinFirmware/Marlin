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
 * Einsy-Retro pin assignments
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Einsy Retro"

//
// TMC2130 Configuration_adv defaults for EinsyRetro
//
#if !AXIS_DRIVER_TYPE_X(TMC2130) || !AXIS_DRIVER_TYPE_Y(TMC2130) || !AXIS_DRIVER_TYPE_Z(TMC2130) || !AXIS_DRIVER_TYPE_E0(TMC2130)
  #error "You must set ([XYZ]|E0)_DRIVER_TYPE to TMC2130 in Configuration.h for EinsyRetro."
#endif

// TMC2130 Diag Pins
#define X_DIAG_PIN                            PinK2
#define Y_DIAG_PIN                            PinK7
#define Z_DIAG_PIN                            PinK6
#define E0_DIAG_PIN                           PinK3

//
// Limit Switches
//
// Only use Diag Pins when SENSORLESS_HOMING is enabled for the TMC2130 drivers.
// Otherwise use a physical endstop based configuration.
//
// SERVO0_PIN and Z_MIN_PIN configuration for BLTOUCH sensor when combined with SENSORLESS_HOMING.
//

#if DISABLED(SENSORLESS_HOMING)

  #define X_MIN_PIN                           PinB6  // X-
  #define Y_MIN_PIN                           PinB5  // Y-
  #define X_MAX_PIN                           PinD4  // X+
  #define Y_MAX_PIN                           PinF3  // Y+

#else

  #if X_HOME_TO_MIN
    #define X_MIN_PIN                 X_DIAG_PIN
    #define X_MAX_PIN                         PinD4  // X+
  #else
    #define X_MIN_PIN                         PinB6  // X-
    #define X_MAX_PIN                 X_DIAG_PIN
  #endif

  #if Y_HOME_TO_MIN
    #define Y_MIN_PIN                 Y_DIAG_PIN
    #define Y_MAX_PIN                         PinF3  // Y+
  #else
    #define Y_MIN_PIN                         PinB5  // Y-
    #define Y_MAX_PIN                 Y_DIAG_PIN
  #endif

  #if ENABLED(BLTOUCH)
    #define Z_MIN_PIN                         PinB5  // Y-
    #define SERVO0_PIN                        PinB4  // Z-
  #endif

#endif

#define Z_MAX_PIN                             PinH4
#ifndef Z_MIN_PIN
  #define Z_MIN_PIN                           PinB4  // Z-
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinB4
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinC0
#define X_DIR_PIN                             PinL0
#define X_ENABLE_PIN                          PinA7
#define X_CS_PIN                              PinG0

#define Y_STEP_PIN                            PinC1
#define Y_DIR_PIN                             PinL1
#define Y_ENABLE_PIN                          PinA6
#define Y_CS_PIN                              PinG2

#define Z_STEP_PIN                            PinC2
#define Z_DIR_PIN                             PinL2
#define Z_ENABLE_PIN                          PinA5
#define Z_CS_PIN                              PinK5

#define E0_STEP_PIN                           PinC3
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinA4
#define E0_CS_PIN                             PinK4

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

#ifndef CASE_LIGHT_PIN
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
  #define E_MUX2_PIN                          PinE2  // 84 in MK2 Firmware, with BEEPER as 78
#endif

//
// LCD / Controller
//
#if ANY(HAS_WIRED_LCD, TOUCH_UI_ULTIPANEL, TOUCH_UI_FTDI_EVE)

  #define KILL_PIN                            PinC5

  #if ANY(IS_ULTIPANEL, TOUCH_UI_ULTIPANEL, TOUCH_UI_FTDI_EVE)

    #if ENABLED(CR10_STOCKDISPLAY)
      #define LCD_PINS_RS                     PinH7
      #define LCD_PINS_ENABLE                 PinG3
      #define LCD_PINS_D4                     PinG4
      #define BTN_EN1                         PinD3
      #define BTN_EN2                         PinD2
    #else
      #define LCD_PINS_RS                     PinD5
      #define LCD_PINS_ENABLE                 PinD3  // On 0.6b, use 61
      #define LCD_PINS_D4                     PinD2  // On 0.6b, use 59
      #define LCD_PINS_D5                     PinG4
      #define LCD_PINS_D6                     PinH7
      #define LCD_PINS_D7                     PinG3
      #define BTN_EN1                         PinJ1
      #define BTN_EN2                         PinJ2
    #endif

    #define BTN_ENC                           PinH6  // AUX-2
    #define BEEPER_PIN                        PinH2  // AUX-4

    #define SD_DETECT_PIN                     PinJ0

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

  #endif // IS_ULTIPANEL || TOUCH_UI_ULTIPANEL || TOUCH_UI_FTDI_EVE

#endif // HAS_WIRED_LCD || TOUCH_UI_ULTIPANEL || TOUCH_UI_FTDI_EVE

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                 0
  #define BOARD_ST7920_DELAY_2               250
  #define BOARD_ST7920_DELAY_3                 0
#endif
