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
 *  Rev B  2 JUN 2017
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

/**
 * 5DPrint D8 Driver board pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/5DPrint%20D8/5DPD8_v1.0_OS_schematics.PDF
 * Origin: https://bitbucket.org/makible/5dprint-d8-controller-board/src/master/5DPD8_v1.0_OS_schematics.PDF
 *
 * https://bitbucket.org/makible/5dprint-d8-controller-board
 */

#include "env_validate.h"

#define DEFAULT_MACHINE_NAME "Makibox"
#define BOARD_INFO_NAME      "5DPrint D8"

// Just a wild guess because schematic does not say!
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#define SERVO0_PIN                            PinF3
#define SERVO1_PIN                            PinF4
#define SERVO2_PIN                            PinF5
#define SERVO3_PIN                            PinF6

//
// Limit Switches
//
#define X_STOP_PIN                            PinE5  // E5
#define Y_STOP_PIN                            PinE4  // E4
#define Z_STOP_PIN                            PinE7  // E7

//
// Steppers
//
#define X_STEP_PIN                            PinA0  // A0
#define X_DIR_PIN                             PinA1  // A1
#define X_ENABLE_PIN                          PinC7  // C7

#define Y_STEP_PIN                            PinA2  // A2
#define Y_DIR_PIN                             PinA3  // A3
#define Y_ENABLE_PIN                          PinC3  // C3

#define Z_STEP_PIN                            PinA4  // A4
#define Z_DIR_PIN                             PinA5  // A5
#define Z_ENABLE_PIN                          PinC2  // C2

#define E0_STEP_PIN                           PinA6  // A6
#define E0_DIR_PIN                            PinA7  // A7
#define E0_ENABLE_PIN                         PinC1  // C1

//
// Digital Microstepping
//
#define X_MS1_PIN                             PinB5  // B5
#define X_MS2_PIN                             PinB6  // B6
#define Y_MS1_PIN                             PinE1  // E1
#define Y_MS2_PIN                             PinE0  // E0
#define Z_MS1_PIN                             PinD7  // D7
#define Z_MS2_PIN                             PinD6  // D6
#define E0_MS1_PIN                            PinD5  // D5
#define E0_MS2_PIN                            PinD4  // D4

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinD1  // F1  Analog Input
#define TEMP_BED_PIN                          PinD0  // F0  Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinC5  // C5
#define HEATER_BED_PIN                        PinC4  // C4

#ifndef FAN_PIN
  #define FAN_PIN                             PinC6  // C6  PWM3A
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0  // B0

//DIGIPOTS slave addresses
#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A             0x2C  // unshifted slave address for DIGIPOT 0x2C (0x58 <- 0x2C << 1)
#endif
