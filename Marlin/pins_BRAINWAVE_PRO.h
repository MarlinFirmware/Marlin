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
 * Brainwave Pro pin assignments (AT90USB1286)
 *
 * Requires hardware bundle for Arduino:
 * https://github.com/unrepentantgeek/brainwave-arduino
 */

/**
 *  Rev B  16 JAN 2017
 *
 *  Added pointers to currently available Arduino IDE extensions that will
 *  allow this board to use the latest Marlin software
 */

/**
 *  There are three Arduino IDE extensions that are compatible with this board
 *  and with the mainstream Marlin software.  All have been used with Arduino 1.6.12
 *
 *  Teensyduino - http://www.pjrc.com/teensy/teensyduino.html
 *    Select Teensy++ 2.0 in Arduino IDE from the 'Tools -> Boards' menu
 *
 *    Installation instructions are at the above URL.  Don't bother loading the
 *    libraries - they are not used with the Marlin software.
 *
 *  Printrboard - https://github.com/scwimbush/Printrboard-HID-Arduino-IDE-Support
 *    This is basically Teensyduino but with a bootloader that can handle image sizes
 *    larger than 64K.
 *
 *    Installation:
 *
 *       1. Go to the above URL, click on the "Clone or Download" button and then
 *          click on "Download ZIP" button.
 *       2. Unzip the file, find the "printrboard" directory and then copy it to the
 *          hardware directory in Arduino.  The Arduino hardware directory will probably
 *			be located in a path similar to this: C:\Program Files (x86)\Arduino\hardware.
 *       3. Restart Arduino.
 *       4. Select "Printrboard" from the 'Tools -> Boards' menu.
 *
 *  Marlin_AT90USB - https://github.com/Bob-the-Kuhn/Marlin_AT90USB
 *    Uses the bootloader from Printerboard above.
 *
 *    "Marlin_AT90USB" makes PWM0A available rather than the usual PWM1C. These PWMs share
 *    the same physical pin. Marlin uses TIMER1 to generate interrupts and sets it up such
 *    that PWM1A, PWM1B & PWM1C can not be used.
 *
 *    Installation:
 *
 *       1. In the Arduino IDE, under Files -> Preferences paste the following URL
 *          https://rawgit.com/Bob-the-Kuhn/Marlin_AT90USB/master/package_MARLIN_AT90USB_index.json
 *       2. Under Tools -> Board -> Boards manager, scroll to the bottom, click on MARLIN_AT90USB
 *          and then click on "Install"
 *       3. Select "AT90USB1286_TEENSYPP" from the 'Tools -> Boards' menu.
 */

/**
 *  To burn the bootloader that comes with Printrboard and Marlin_AT90USB:
 *
 *   1. Connect your programmer to the board.
 *   2. In the Arduino IDE select "Printrboard" or "AT90USB1286_TEENSYPP" and then select the programmer.
 *   3. In the Arduino IDE click on "burn bootloader". Don't worry about the "verify failed at 1F000" error message.
 *   4. The programmer is no longer needed. Remove it.
 */

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0', 'AT90USB1286_TEENSYPP', or 'Printrboard' selected from the 'Tools -> Boards' menu."
#endif

#include "fastio.h"

#if DISABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS) // use Teensyduino Teensy++2.0 pin assignments instead of Marlin alphabetical.
  #error "Uncomment '#define AT90USBxx_TEENSYPP_ASSIGNMENTS' in fastio.h for this config"
#endif

#define BOARD_NAME         "Brainwave Pro"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true

//
// Limit Switches
//
#define X_STOP_PIN         47
#define Y_STOP_PIN         18
#define Z_MAX_PIN          36

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  17
#endif

//
// Steppers
//
#define X_STEP_PIN         33
#define X_DIR_PIN          32
#define X_ENABLE_PIN       11

#define Y_STEP_PIN         31
#define Y_DIR_PIN          30
#define Y_ENABLE_PIN        8

#define Z_STEP_PIN         29
#define Z_DIR_PIN          28
#define Z_ENABLE_PIN       37

#define E0_STEP_PIN        35
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      13

//
// Temperature Sensors
//
#define TEMP_0_PIN          2   // Analog Input
#define TEMP_1_PIN          1   // Analog Input
#define TEMP_BED_PIN        0   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       15
#define HEATER_BED_PIN     14  // Bed
#define FAN_PIN            16  // Fan, PWM

//
// Misc. Functions
//
#define SDSS               20
#define SD_DETECT_PIN      12
#define LED_PIN            19
