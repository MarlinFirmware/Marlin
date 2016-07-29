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
 * MegaTronics v2.0 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Megatronics v2.0"
#define LARGE_FLASH        true

#define X_STEP_PIN 26
#define X_DIR_PIN 27
#define X_ENABLE_PIN 25
#define X_MIN_PIN 37
#define X_MAX_PIN 40

#define Y_STEP_PIN 4 // A6
#define Y_DIR_PIN 54 // A0
#define Y_ENABLE_PIN 5
#define Y_MIN_PIN 41
#define Y_MAX_PIN 38

#define Z_STEP_PIN 56 // A2
#define Z_DIR_PIN 60 // A6
#define Z_ENABLE_PIN 55 // A1
#define Z_MIN_PIN 18
#define Z_MAX_PIN 19

#define E0_STEP_PIN 35
#define E0_DIR_PIN 36
#define E0_ENABLE_PIN 34

#define E1_STEP_PIN 29
#define E1_DIR_PIN 39
#define E1_ENABLE_PIN 28

#define E2_STEP_PIN 23
#define E2_DIR_PIN 24
#define E2_ENABLE_PIN 22

#define SDSS 53
#define LED_PIN 13

#define FAN_PIN 7
#define FAN2_PIN 6
#define PS_ON_PIN 12

#define HEATER_0_PIN 9  // EXTRUDER 1
#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN 4  // ANALOG NUMBERING
#else
  #define TEMP_0_PIN 13 // ANALOG NUMBERING
#endif

#define HEATER_1_PIN 8  // EXTRUDER 2
#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN 8  // ANALOG NUMBERING
#else
  #define TEMP_1_PIN 15 // ANALOG NUMBERING
#endif

#define HEATER_BED_PIN 10 // BED
#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN 8  // ANALOG NUMBERING
#else
  #define TEMP_BED_PIN 14 // ANALOG NUMBERING
#endif

#define BEEPER_PIN 64

#define LCD_PINS_RS 14
#define LCD_PINS_ENABLE 15
#define LCD_PINS_D4 30
#define LCD_PINS_D5 31
#define LCD_PINS_D6 32
#define LCD_PINS_D7 33

// Buttons are directly attached using keypad
#define BTN_EN1 61
#define BTN_EN2 59
#define BTN_ENC 43
