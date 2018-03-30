/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MegaTronics v3.0 / v3.1 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#if MB(MEGATRONICS_31)
  #define BOARD_NAME "Megatronics v3.1"
#else
  #define BOARD_NAME "Megatronics v3.0"
#endif

//
// Servos
//
#define SERVO0_PIN         46   // AUX3-6
#define SERVO1_PIN         47   // AUX3-5
#define SERVO2_PIN         48   // AUX3-4
#define SERVO3_PIN         49   // AUX3-3

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
#define X_STEP_PIN         58
#define X_DIR_PIN          57
#define X_ENABLE_PIN       59

#define Y_STEP_PIN          5
#define Y_DIR_PIN          17
#define Y_ENABLE_PIN        4

#define Z_STEP_PIN         16
#define Z_DIR_PIN          11
#define Z_ENABLE_PIN        3

#define E0_STEP_PIN        28
#define E0_DIR_PIN         27
#define E0_ENABLE_PIN      29

#define E1_STEP_PIN        25
#define E1_DIR_PIN         24
#define E1_ENABLE_PIN      26

#define E2_STEP_PIN        22
#define E2_DIR_PIN         60
#define E2_ENABLE_PIN      23

//
// Temperature Sensors
//
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN       11   // Analog Input
#else
  #define TEMP_0_PIN       15   // Analog Input
#endif
#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN       10   // Analog Input
#else
  #define TEMP_1_PIN       13   // Analog Input
#endif
#if TEMP_SENSOR_2 == -1
  #define TEMP_2_PIN        9   // Analog Input
#else
  #define TEMP_2_PIN       12   // Analog Input
#endif
#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN      8   // Analog Input
#else
  #define TEMP_BED_PIN     14   // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define HEATER_1_PIN        9
#define HEATER_2_PIN        8
#define HEATER_BED_PIN     10

#define FAN_PIN             6
#define FAN1_PIN            7

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          12
#define CASE_LIGHT_PIN     45   // Try the keypad connector

//
// LCD / Controller
//
#define BEEPER_PIN         61

#define BTN_EN1            44
#define BTN_EN2            45
#define BTN_ENC            33

#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

  #define LCD_PINS_RS      56   // CS chip select / SS chip slave select
  #define LCD_PINS_ENABLE  51   // SID (MOSI)
  #define LCD_PINS_D4      52   // SCK (CLK) clock
  #define SD_DETECT_PIN    35

#else

  #define LCD_PINS_RS      32
  #define LCD_PINS_ENABLE  31
  #define LCD_PINS_D4      14
  #define LCD_PINS_D5      30
  #define LCD_PINS_D6      39
  #define LCD_PINS_D7      15

  #define SHIFT_CLK        43
  #define SHIFT_LD         35
  #define SHIFT_OUT        34
  #define SHIFT_EN         44

  #if MB(MEGATRONICS_31)
    #define SD_DETECT_PIN  56
  #endif

#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if DISABLED(REPRAPWORLD_KEYPAD)        // try to use the keypad connector first
  #define SPINDLE_LASER_PWM_PIN    44   // MUST BE HARDWARE PWM
  #define SPINDLE_LASER_ENABLE_PIN 43   // Pin should have a pullup!
  #define SPINDLE_DIR_PIN          42
#elif EXTRUDERS <= 2
  // Hijack the last extruder so that we can get the PWM signal off the Y breakout
  // Move Y to the E2 plug. This makes dual Y steppers harder
  #undef Y_ENABLE_PIN  //  4
  #undef Y_STEP_PIN    //  5
  #undef Y_DIR_PIN     // 17
  #undef E2_ENABLE_PIN // 23
  #undef E2_STEP_PIN   // 22
  #undef E2_DIR_PIN    // 60
  #define Y_ENABLE_PIN             23
  #define Y_STEP_PIN               22
  #define Y_DIR_PIN                60
  #define SPINDLE_LASER_PWM_PIN     4   // MUST BE HARDWARE PWM
  #define SPINDLE_LASER_ENABLE_PIN 17   // Pin should have a pullup!
  #define SPINDLE_DIR_PIN           5
#endif
