/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Mega controller pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Mega Controller supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Mega Controller"

//
// Servos
//
#define SERVO0_PIN         30
#define SERVO1_PIN         31
#define SERVO2_PIN         32
#define SERVO3_PIN         33

//
// Limit Switches
//
#define X_MIN_PIN          43
#define X_MAX_PIN          42
#define Y_MIN_PIN          38
#define Y_MAX_PIN          41
#define Z_MIN_PIN          40
#define Z_MAX_PIN          37

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  37
#endif

//
// Steppers
//
#define X_STEP_PIN         62   // A8
#define X_DIR_PIN          63   // A9
#define X_ENABLE_PIN       61   // A7

#define Y_STEP_PIN         65   // A11
#define Y_DIR_PIN          66   // A12
#define Y_ENABLE_PIN       64   // A10

#define Z_STEP_PIN         68   // A14
#define Z_DIR_PIN          69   // A15
#define Z_ENABLE_PIN       67   // A13

#define E0_STEP_PIN        23
#define E0_DIR_PIN         24
#define E0_ENABLE_PIN      22

#define E1_STEP_PIN        26
#define E1_DIR_PIN         27
#define E1_ENABLE_PIN      25

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN        4   // Analog Input
#else
  #define TEMP_0_PIN        0   // Analog Input
#endif

#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN        5   // Analog Input
#else
  #define TEMP_1_PIN        2   // Analog Input
#endif

#define TEMP_2_PIN          3   // Analog Input

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN      6   // Analog Input
#else
  #define TEMP_BED_PIN      1   // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       29
#define HEATER_1_PIN       34
#define HEATER_BED_PIN     28

#ifndef FAN_PIN
  #define FAN_PIN          39
#endif
#define FAN1_PIN           35
#define FAN2_PIN           36

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN FAN2_PIN
#endif

#define FAN_SOFT_PWM

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define CASE_LIGHT_PIN      2

//
// LCD / Controller
//
#if ENABLED(MINIPANEL)

  #define BEEPER_PIN       46
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0        47
  #define DOGLCD_CS        45
  #define LCD_BACKLIGHT_PIN 44   // backlight LED on PA3

  #define KILL_PIN         12
  // GLCD features
  //#define LCD_CONTRAST 190
  // Uncomment screen orientation
  //#define LCD_SCREEN_ROT_90
  //#define LCD_SCREEN_ROT_180
  //#define LCD_SCREEN_ROT_270

  #define BTN_EN1          48
  #define BTN_EN2          11
  #define BTN_ENC          10

  #define SD_DETECT_PIN    49

#endif // MINIPANEL

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN     6   // MUST BE HARDWARE PWM
#define SPINDLE_LASER_ENABLE_PIN  7   // Pin should have a pullup!
#define SPINDLE_DIR_PIN           8
