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
 * Minitronics v1.0/1.1 pin assignments
 * ATmega1281
 */

/**
 * Rev B   2 JAN 2017
 *
 *  Added pin definitions for M3, M4 & M5 spindle control commands
 */

#if NOT_TARGET(__AVR_ATmega1281__)
  #error "Oops! Select 'Minitronics' in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "Minitronics supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "Minitronics v1.0/1.1"
//
// Limit Switches
//
#define X_MIN_PIN                             PinE3
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinE4
#define Y_MAX_PIN                             PinE7
#define Z_MIN_PIN                             PinB4
#define Z_MAX_PIN                             -1

//
// Steppers
//
#define X_STEP_PIN                            PinF2
#define X_DIR_PIN                             PinF1
#define X_ENABLE_PIN                          PinF3

#define Y_STEP_PIN                            PinA1  // A6
#define Y_DIR_PIN                             PinA2  // A0
#define Y_ENABLE_PIN                          PinA0

#define Z_STEP_PIN                            PinA4  // A2
#define Z_DIR_PIN                             PinA5  // A6
#define Z_ENABLE_PIN                          PinA3  // A1

#define E0_STEP_PIN                           PinA7
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinG2

#define E1_STEP_PIN                           PinC6
#define E1_DIR_PIN                            PinC5
#define E1_ENABLE_PIN                         PinC7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB5  // Analog Input
#define TEMP_1_PIN                            PinB4  // Analog Input
#define TEMP_BED_PIN                          PinB4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB5  // EXTRUDER 1
#define HEATER_1_PIN                          PinB6  // EXTRUDER 2
#define HEATER_BED_PIN                        PinE5  // BED

#ifndef FAN_PIN
  #define FAN_PIN                             PinB7
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinF0

//
// LCD / Controller
//
#define BEEPER_PIN                            -1

#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

  #define LCD_PINS_RS                         PinE7  // CS chip select /SS chip slave select
  #define LCD_PINS_ENABLE                     PinB2  // SID (MOSI)
  #define LCD_PINS_D4                         PinB1  // SCK (CLK) clock

  #define BTN_EN1                             PinD1
  #define BTN_EN2                             PinD0
  #define BTN_ENC                             PinG0

  #define SD_DETECT_PIN                       PinC0

#else

  #define LCD_PINS_RS                         -1
  #define LCD_PINS_ENABLE                     -1

  // Buttons are directly attached using keypad
  #define BTN_EN1                             -1
  #define BTN_EN2                             -1
  #define BTN_ENC                             -1

  #define SD_DETECT_PIN                       -1  // Minitronics doesn't use this
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER                                    // assumes we're only doing CNC work (no 3D printing)
  #undef HEATER_BED_PIN
  #undef TEMP_BED_PIN                             // need to free up some pins but also need to
  #undef TEMP_0_PIN                               // re-assign them (to unused pins) because Marlin
  #undef TEMP_1_PIN                               // requires the presence of certain pins or else it
  #define HEATER_BED_PIN                      PinG5  // won't compile
  #define TEMP_BED_PIN                        PinF4
  #define TEMP_0_PIN                          PinF5
  #define SPINDLE_LASER_ENA_PIN               PinF6  // using A6 because it already has a pullup
  #define SPINDLE_LASER_PWM_PIN               PinE5  // WARNING - LED & resistor pull up to +12/+24V stepper voltage
  #define SPINDLE_DIR_PIN                     PinF7
#endif
