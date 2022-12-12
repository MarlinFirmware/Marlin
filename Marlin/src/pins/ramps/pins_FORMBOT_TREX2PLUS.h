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
 * Formbot T-Rex 2+ pin assignments
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Formbot supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "Formbot T-Rex 2+"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

// Just a wild guess because no schematics!
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#define SERVO0_PIN                         PinB5
#define SERVO1_PIN                            -1  // was 6
#define SERVO2_PIN                            -1  // was 5
#define SERVO3_PIN                            -1

//
// Limit Switches
//
#define X_MIN_PIN                          PinE5
#ifndef X_MAX_PIN
  #define X_MAX_PIN                        PinE4
#endif
#define Y_MIN_PIN                          PinJ1
#define Y_MAX_PIN                          PinJ0
#define Z_MIN_PIN                          PinD3
#define Z_MAX_PIN                          PinD2

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  PinC5
#endif

//
// Steppers
//
#define X_STEP_PIN                         PinF0
#define X_DIR_PIN                          PinF1
#define X_ENABLE_PIN                       PinD7
#ifndef X_CS_PIN
  #define X_CS_PIN                         PinB0
#endif

#define Y_STEP_PIN                         PinF6
#define Y_DIR_PIN                          PinF7
#define Y_ENABLE_PIN                       PinF2
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         PinL0
#endif

#define Z_STEP_PIN                         PinL3
#define Z_DIR_PIN                          PinL1
#define Z_ENABLE_PIN                       PinK0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         PinG1
#endif

#define E0_STEP_PIN                        PinA4
#define E0_DIR_PIN                         PinA6
#define E0_ENABLE_PIN                      PinA2
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        PinL7
#endif

#define E1_STEP_PIN                        PinC1
#define E1_DIR_PIN                         PinC3
#define E1_ENABLE_PIN                      PinC7
#ifndef E1_CS_PIN
  #define E1_CS_PIN                        PinL5
#endif

#define E2_STEP_PIN                        PinL7
#define E2_DIR_PIN                         PinL6
#define E2_ENABLE_PIN                      PinL5

//
// Temperature Sensors
//
#define TEMP_0_PIN                         PinB7  // Analog Input
#define TEMP_1_PIN                         PinJ0  // Analog Input
#define TEMP_BED_PIN                       PinE5  // Analog Input

// SPI for MAX Thermocouple
#if DISABLED(SDSUPPORT)
  #define TEMP_0_CS_PIN                    PinK4  // Don't use 53 if using Display/SD card
#else
  #define TEMP_0_CS_PIN                    PinK4  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                       PinB4
#define HEATER_1_PIN                       PinH4
#define HEATER_BED_PIN                     PinF4

#define FAN_PIN                            PinH6

#if HAS_FILAMENT_SENSOR
  #define FIL_RUNOUT_PIN                   PinG5
  //#define FIL_RUNOUT2_PIN                   -1
#else
  // Though defined as a fan pin, it is utilized as a dedicated laser pin by Formbot.
  #define FAN1_PIN                         PinG5
#endif

//
// Misc. Functions
//
#define SDSS                               PinB0
#ifndef LED_PIN
  #define LED_PIN                          PinB7  // The Formbot v 1 board has almost no unassigned pins on it.  The Board's LED
#endif                          // is a good place to get a signal to control the Max7219 LED Matrix.

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
#define FILWIDTH_PIN                       PinE3  // Analog Input

#ifndef PS_ON_PIN
  #define PS_ON_PIN                        PinB6
#endif

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                   PinH5
#endif

//
// LCD / Controller
//
// Formbot only supports REPRAP_DISCOUNT_SMART_CONTROLLER
//
#if IS_RRD_SC
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                     PinC0
  #endif
  #define BTN_EN1                          PinC6
  #define BTN_EN2                          PinC4
  #define BTN_ENC                          PinC2
  #define SD_DETECT_PIN                    PinL0

  // Allow MAX7219 to steal the KILL pin
  #if !defined(KILL_PIN) && MAX7219_CLK_PIN != 41 && MAX7219_DIN_PIN != 41 && MAX7219_LOAD_PIN != 41
    #define KILL_PIN                       PinG0
  #endif

  #define LCD_PINS_RS                      PinH1
  #define LCD_PINS_ENABLE                  PinH0
  #define LCD_PINS_D4                      PinA1
  #define LCD_PINS_D5                      PinA3
  #define LCD_PINS_D6                      PinA5
  #define LCD_PINS_D7                      PinA7
#endif

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1               200
  #define BOARD_ST7920_DELAY_2               200
  #define BOARD_ST7920_DELAY_3               200
#endif
