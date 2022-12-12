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
 * Brainwave Pro pin assignments (AT90USB1286)
 *
 * Requires hardware bundle for Arduino:
 * https://github.com/unrepentantgeek/brainwave-arduino
 * 
 * Not to be confused with the regular Brainwave controller (https://www.reprap.org/wiki/Brainwave)
 */

/**
 *  Rev B  16 JAN 2017
 *
 *  Added pointers to currently available Arduino IDE extensions that will
 *  allow this board to use the latest Marlin software
 *
 *
 *  Rev C  2 JUN 2017
 *
 *  Converted to Arduino pin numbering
 */

/**
 *  There are two Arduino IDE extensions that are compatible with this board
 *  and with the mainstream Marlin software.
 *
 *  Teensyduino - https://www.pjrc.com/teensy/teensyduino.html
 *    Select Teensy++ 2.0 in Arduino IDE from the 'Tools > Board' menu
 *
 *    Installation instructions are at the above URL.  Don't bother loading the
 *    libraries - they are not used with the Marlin software.
 *
 *  Printrboard - https://github.com/scwimbush/Printrboard-HID-Arduino-IDE-Support
 *
 *    Installation:
 *
 *       1. Go to the above URL, click on the "Clone or Download" button and then
 *          click on "Download ZIP" button.
 *       2. Unzip the file, find the "printrboard" directory and then copy it to the
 *          hardware directory in Arduino.  The Arduino hardware directory will probably
 *          be located in a path similar to this: C:\Program Files (x86)\Arduino\hardware.
 *       3. Restart Arduino.
 *       4. Select "Printrboard" from the 'Tools > Board' menu.
 *
 *  Teensyduino is the most popular option. Printrboard is used if your board doesn't have
 *  the Teensyduino bootloader on it.
 */

/**
 *  To burn the bootloader that comes with Printrboard:
 *
 *   1. Connect your programmer to the board.
 *   2. In the Arduino IDE select "Printrboard" and then select the programmer.
 *   3. In the Arduino IDE click on "burn bootloader". Don't worry about the "verify failed at 1F000" error message.
 *   4. The programmer is no longer needed. Remove it.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Brainwave Pro"

// Just a wild guess because no schematics!
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Limit Switches
//
#define X_STOP_PIN                            PinF7  // F7
#define Y_STOP_PIN                            PinC2  // C2
#define Z_STOP_PIN                            PinE4  // E4

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC1  // C1
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinE1  // E1
#define X_DIR_PIN                             PinE0  // E0
#define X_ENABLE_PIN                          PinB3  // B3

#define Y_STEP_PIN                            PinD7  // D7
#define Y_DIR_PIN                             PinD6  // D6
#define Y_ENABLE_PIN                          PinB0  // B0

#define Z_STEP_PIN                            PinD5  // D5
#define Z_DIR_PIN                             PinD4  // D4
#define Z_ENABLE_PIN                          PinE5  // E5

#define E0_STEP_PIN                           PinE3  // E3
#define E0_DIR_PIN                            PinE2  // E2
#define E0_ENABLE_PIN                         PinB5  // B5

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinD2  // F2  Analog Input
#define TEMP_1_PIN                            PinD1  // F1  Analog Input
#define TEMP_BED_PIN                          PinD0  // F0  Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB7  // B7
#define HEATER_BED_PIN                        PinB6  // B6  Bed
#ifndef FAN_PIN
  #define FAN_PIN                             PinC6  // C6  Fan, PWM3A
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0  // B0
#define SD_DETECT_PIN                         PinB4  // B4
#define LED_PIN                               PinC3  // C3
