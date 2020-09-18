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

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'RAMBo' in 'Tools > Board' or the Mega2560 environment in PlatformIO."
#endif

#if MB(MINIRAMBO_10A)
  #define BOARD_INFO_NAME "Mini RAMBo 1.0a"
#else
  #define BOARD_INFO_NAME "Mini RAMBo"
#endif

//
// Limit Switches
//
#define X_MIN_PIN                             12
#define X_MAX_PIN                             30
#define Y_MIN_PIN                             11
#define Y_MAX_PIN                             24
#define Z_MIN_PIN                             10
#define Z_MAX_PIN                             23

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     23
#endif

//
// Steppers
//
#define X_STEP_PIN                            37
#define X_DIR_PIN                             48
#define X_ENABLE_PIN                          29

#define Y_STEP_PIN                            36
#define Y_DIR_PIN                             49
#define Y_ENABLE_PIN                          28

#define Z_STEP_PIN                            35
#define Z_DIR_PIN                             47
#define Z_ENABLE_PIN                          27

#define E0_STEP_PIN                           34
#define E0_DIR_PIN                            43
#define E0_ENABLE_PIN                         26

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN                             40
#define X_MS2_PIN                             41
#define Y_MS1_PIN                             69
#define Y_MS2_PIN                             39
#define Z_MS1_PIN                             68
#define Z_MS2_PIN                             67
#define E0_MS1_PIN                            65
#define E0_MS2_PIN                            66

#define MOTOR_CURRENT_PWM_XY_PIN              46
#define MOTOR_CURRENT_PWM_Z_PIN               45
#define MOTOR_CURRENT_PWM_E_PIN               44
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

//
// Temperature Sensors
//
#define TEMP_0_PIN                             0  // Analog Input
#define TEMP_1_PIN                             1  // Analog Input
#define TEMP_BED_PIN                           2  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           3
#define HEATER_1_PIN                           7
#if !MB(MINIRAMBO_10A)
  #define HEATER_2_PIN                         6
#endif
#define HEATER_BED_PIN                         4

#ifndef FAN_PIN
  #define FAN_PIN                              8
#endif
#define FAN1_PIN                               6

//
// Misc. Functions
//
#define SDSS                                  53
#define LED_PIN                               13
#if !MB(MINIRAMBO_10A)
  #define CASE_LIGHT_PIN                       9
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
// use P1 connector for spindle pins
#define SPINDLE_LASER_PWM_PIN                  9  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 18  // Pullup!
#define SPINDLE_DIR_PIN                       19

//
// Průša i3 MK2 Multiplexer Support
//
#define E_MUX0_PIN                            17
#define E_MUX1_PIN                            16
#if !MB(MINIRAMBO_10A)
  #define E_MUX2_PIN                          78  // 84 in MK2 Firmware, with BEEPER as 78
#endif

//
// LCD / Controller
//
#if HAS_SPI_LCD || TOUCH_UI_ULTIPANEL

  #if !MB(MINIRAMBO_10A)
    #define KILL_PIN                          32
  #endif

  #if ENABLED(ULTIPANEL) || TOUCH_UI_ULTIPANEL

    #if MB(MINIRAMBO_10A)

      #define BEEPER_PIN                      78

      #define BTN_EN1                         80
      #define BTN_EN2                         73
      #define BTN_ENC                         21

      #define LCD_PINS_RS                     38
      #define LCD_PINS_ENABLE                  5
      #define LCD_PINS_D4                     14
      #define LCD_PINS_D5                     15
      #define LCD_PINS_D6                     32
      #define LCD_PINS_D7                     31

      #define SD_DETECT_PIN                   72

    #else                                         // !MINIRAMBO_10A

      // AUX-4
      #define BEEPER_PIN                      84

      // AUX-2
      #define BTN_EN1                         14
      #define BTN_EN2                         72
      #define BTN_ENC                          9

      #define LCD_PINS_RS                     82
      #define LCD_PINS_ENABLE                 18
      #define LCD_PINS_D4                     19
      #define LCD_PINS_D5                     70
      #define LCD_PINS_D6                     85
      #define LCD_PINS_D7                     71

      #define SD_DETECT_PIN                   15

    #endif // !MINIRAMBO_10A

  #endif // ULTIPANEL || TOUCH_UI_ULTIPANEL

#endif // HAS_SPI_LCD || TOUCH_UI_ULTIPANEL
