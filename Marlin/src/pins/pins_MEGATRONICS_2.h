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
 * MegaTronics v2.0 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_NAME         "Megatronics v2.0"
//
// Limit Switches
//
#define X_MIN_PIN          37
#define X_MAX_PIN          40
#define Y_MIN_PIN          41
#define Y_MAX_PIN          38
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  19
#endif

//
// Steppers
//
#define X_STEP_PIN         26
#define X_DIR_PIN          27
#define X_ENABLE_PIN       25

#define Y_STEP_PIN          4   // A6
#define Y_DIR_PIN          54   // A0
#define Y_ENABLE_PIN        5

#define Z_STEP_PIN         56   // A2
#define Z_DIR_PIN          60   // A6
#define Z_ENABLE_PIN       55   // A1

#define E0_STEP_PIN        35
#define E0_DIR_PIN         36
#define E0_ENABLE_PIN      34

#define E1_STEP_PIN        29
#define E1_DIR_PIN         39
#define E1_ENABLE_PIN      28

#define E2_STEP_PIN        23   // ? schematic says 24
#define E2_DIR_PIN         24   // ? schematic says 23
#define E2_ENABLE_PIN      22

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN        4   // Analog Input
#else
  #define TEMP_0_PIN       13   // Analog Input
#endif

#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN        8   // Analog Input
#else
  #define TEMP_1_PIN       15   // Analog Input
#endif

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN      8   // Analog Input
#else
  #define TEMP_BED_PIN     14   // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        9
#define HEATER_1_PIN        8
#define HEATER_BED_PIN     10

#ifndef FAN_PIN
  #define FAN_PIN           7
#endif
#define FAN1_PIN            6

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          12
#define CASE_LIGHT_PIN      2

//
// LCD / Controller
//
#define BEEPER_PIN         64

#define LCD_PINS_RS        14
#define LCD_PINS_ENABLE    15
#define LCD_PINS_D4        30
#define LCD_PINS_D5        31
#define LCD_PINS_D6        32
#define LCD_PINS_D7        33

// Buttons are directly attached using keypad
#define BTN_EN1            61
#define BTN_EN2            59
#define BTN_ENC            43

// Buttons that are attached using shift register of reprapworld keypad  v1.1
#define SHIFT_CLK 63
#define SHIFT_LD 42
#define SHIFT_OUT 17
#define SHIFT_EN 17

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN     3   // MUST BE HARDWARE PWM
#define SPINDLE_LASER_ENA_PIN    16   // Pin should have a pullup!
#define SPINDLE_DIR_PIN          11
