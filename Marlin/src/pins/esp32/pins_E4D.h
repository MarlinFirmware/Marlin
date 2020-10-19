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
 * E4d@Box  pin assignments
 * E4d@Box is a small factor 3D printer control board based on the ESP32 microcontroller for Laser, CNC and 3d printers
 * for more info check https://atbox.tech/ and join to Facebook page E4d@box.
 */

#if NOT_TARGET(ARDUINO_ARCH_ESP32)
  #error "Oops! Select an ESP32 board in 'Tools > Board.'"
#elif EXTRUDERS > 1 || E_STEPPERS > 1
  #error "E4d@box only supports one E Stepper. Comment out this line to continue."
#elif HOTENDS > 2
  #error "E4d@box currently supports only one hotend. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME       "E4D@BOX"
#define BOARD_WEBSITE_URL     "github.com/Exilaus/E4d@box"
#define DEFAULT_MACHINE_NAME  BOARD_INFO_NAME

//
// Limit Switches
//
#define X_STOP_PIN                            34
#define Y_STOP_PIN                            35
#define Z_STOP_PIN                            16

//
// Steppers
//
#define X_STEP_PIN                            12
#define X_DIR_PIN                             13
#define X_ENABLE_PIN                          17
//#define X_CS_PIN                             0

#define Y_STEP_PIN                            32
#define Y_DIR_PIN                             33
#define Y_ENABLE_PIN                X_ENABLE_PIN
//#define Y_CS_PIN                            13

#define Z_STEP_PIN                            25
#define Z_DIR_PIN                             26
#define Z_ENABLE_PIN                X_ENABLE_PIN
//#define Z_CS_PIN                             5  // SS_PIN

#define E0_STEP_PIN                           27
#define E0_DIR_PIN                            14
#define E0_ENABLE_PIN               X_ENABLE_PIN
//#define E0_CS_PIN                           21

//
// Temperature Sensors
//
#define TEMP_0_PIN                            36  // Analog Input
#define TEMP_BED_PIN                          39  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           2
#define FAN_PIN                                0
#define HEATER_BED_PIN                        15

// SPI
#define SDSS                                   5
#define I2S_STEPPER_STREAM
#define I2S_WS                                23
#define I2S_BCK                               22
#define I2S_DATA                              21
