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
 * Cheaptronic v1.0 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true

// X motor stepper
#define X_STEP_PIN 14
#define X_DIR_PIN 15
#define X_ENABLE_PIN 24

// X endstop
#define X_MIN_PIN 3
#define X_MAX_PIN -1

// Y motor stepper
#define Y_STEP_PIN 35
#define Y_DIR_PIN 36
#define Y_ENABLE_PIN 31

// Y endstop
#define Y_MIN_PIN 2
#define Y_MAX_PIN -1

// Z motor stepper
#define Z_STEP_PIN 40
#define Z_DIR_PIN 41
#define Z_ENABLE_PIN 37

// Z endstop
#define Z_MIN_PIN 5
#define Z_MAX_PIN -1

// Extruder 0 stepper
#define E0_STEP_PIN 26
#define E0_DIR_PIN 28
#define E0_ENABLE_PIN 25

// Extruder 1 stepper
#define E1_STEP_PIN 33
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30

#define SDPOWER -1
#define SDSS -1
#define LED_PIN -1

// FAN
#define FAN_PIN -1

#define PS_ON_PIN -1
#define KILL_PIN -1

#define HEATER_0_PIN 19 // EXTRUDER 1
#define HEATER_1_PIN 23 // EXTRUDER 2
// HeatedBad
#define HEATER_BED_PIN 22
// Cheaptronic v1.0 hasent EXTRUDER 3
#define HEATER_2_PIN -1

// Temperature sensors
#define TEMP_0_PIN 15
#define TEMP_1_PIN 14
#define TEMP_2_PIN -1
#define TEMP_BED_PIN 13

// Cheaptronic v1.0 doesn't support LCD
#define LCD_PINS_RS -1
#define LCD_PINS_ENABLE -1
#define LCD_PINS_D4 -1
#define LCD_PINS_D5 -1
#define LCD_PINS_D6 -1
#define LCD_PINS_D7 -1

// Cheaptronic v1.0 doesn't support keypad
#define BTN_EN1 -1
#define BTN_EN2 -1
#define BTN_ENC -1

#define BLEN_C 2
#define BLEN_B 1
#define BLEN_A 0

// Cheaptronic v1.0 doesn't use this
#define SD_DETECT_PIN -1
