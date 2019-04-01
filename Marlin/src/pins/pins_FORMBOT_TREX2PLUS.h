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
 * Formbot pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Formbot supports up to 2 hotends / E-steppers. Comment this line to keep going."
#endif

#define DEFAULT_MACHINE_NAME "Formbot"
#define BOARD_NAME           "Formbot"

//
// Servos
//
#define SERVO0_PIN         11
#define SERVO1_PIN         -1    // was 6
#define SERVO2_PIN         -1    // was 5
#define SERVO3_PIN         -1

//
// Limit Switches
//
#define X_MIN_PIN           3
#ifndef X_MAX_PIN
  #define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  32
#endif

//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#ifndef X_CS_PIN
  #define X_CS_PIN         53
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#ifndef Y_CS_PIN
  #define Y_CS_PIN         49
#endif

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#ifndef Z_CS_PIN
  #define Z_CS_PIN         40
#endif

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#ifndef E0_CS_PIN
  #define E0_CS_PIN        42
#endif

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#ifndef E1_CS_PIN
  #define E1_CS_PIN        44
#endif

#define E2_STEP_PIN        42
#define E2_DIR_PIN         43
#define E2_ENABLE_PIN      44

//
// Temperature Sensors
//
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_BED_PIN        3   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS_PIN   66 // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS_PIN   66 // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

//
// Augmentation for auto-assigning RAMPS plugs
//
#if DISABLED(IS_RAMPS_EEB, IS_RAMPS_EEF, IS_RAMPS_EFB, IS_RAMPS_EFF, IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       10
#define HEATER_1_PIN        7
#define HEATER_BED_PIN     58

#define FAN_PIN             9

#if HAS_FILAMENT_SENSOR
  #define FIL_RUNOUT_PIN    4
  //#define FIL_RUNOUT2_PIN  -1
#else
  // Though defined as a fan pin, it is utilized as a dedicated laser pin by Formbot.
  #define FAN1_PIN          4
#endif

//
// Misc. Functions
//
#define CASE_LIGHT_PIN      8
#define SDSS               53
#ifndef ROXYs_TRex
  #define LED_PIN          13   // The Formbot v 1 board has almost no unassigned pins on it.  The Board's LED
#endif                          // is a good place to get a signal to control the Max7219 LED Matrix.

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
#define FILWIDTH_PIN        5   // Analog Input

#ifndef PS_ON_PIN
  #define PS_ON_PIN        12
#endif

//
// LCD / Controller
//
// Formbot only supports REPRAP_DISCOUNT_SMART_CONTROLLER
//
#if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
  #define LCD_PINS_RS      16
  #define LCD_PINS_ENABLE  17
  #define LCD_PINS_D4      23
  #define LCD_PINS_D5      25
  #define LCD_PINS_D6      27
  #define LCD_PINS_D7      29
  #define BTN_EN1          31
  #define BTN_EN2          33
  #define BTN_ENC          35
  #define SD_DETECT_PIN    49
  #ifndef ROXYs_TRex
    #define KILL_PIN       41
    #define BEEPER_PIN     37
  #endif
#endif
