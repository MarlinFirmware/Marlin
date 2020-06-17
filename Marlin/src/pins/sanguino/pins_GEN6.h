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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Gen6 pin assignments
 */

 /**
 * Rev B    26 DEC 2016
 *
 * 1) added pointer to a current Arduino IDE extension
 * 2) added support for M3, M4 & M5 spindle control commands
 * 3) added case light pin definition
 *
 */

/**
 * A useable Arduino IDE extension (board manager) can be found at
 * https://github.com/Lauszus/Sanguino
 *
 * This extension has been tested on Arduino 1.6.12 & 1.8.0
 *
 * Here's the JSON path:
 * https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
 *
 * When installing select 1.0.2
 *
 * Installation instructions can be found at https://learn.sparkfun.com/pages/CustomBoardsArduino
 * Just use the above JSON URL instead of Sparkfun's JSON.
 *
 * Once installed select the Sanguino board and then select the CPU.
 *
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__)
  #error "Oops! Select 'Sanguino' in 'Tools > Boards' and 'ATmega644P' or 'ATmega1284P' in 'Tools > Processor.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Gen6"
#endif

//
// Limit Switches
//
#define X_STOP_PIN                            20
#define Y_STOP_PIN                            25
#define Z_STOP_PIN                            30

//
// Steppers
//
#define X_STEP_PIN                            15
#define X_DIR_PIN                             18
#define X_ENABLE_PIN                          19

#define Y_STEP_PIN                            23
#define Y_DIR_PIN                             22
#define Y_ENABLE_PIN                          24

#define Z_STEP_PIN                            27
#define Z_DIR_PIN                             28
#define Z_ENABLE_PIN                          29

#define E0_STEP_PIN                            4  // Edited @ EJE Electronics 20100715
#define E0_DIR_PIN                             2  // Edited @ EJE Electronics 20100715
#define E0_ENABLE_PIN                          3  // Added @ EJE Electronics 20100715

//
// Temperature Sensor
//
#define TEMP_0_PIN                             5  // Analog Input

//
// Heaters
//
#define HEATER_0_PIN                          14  // changed @ rkoeppl 20110410

#if !MB(GEN6)
  #define HEATER_BED_PIN                       1  // changed @ rkoeppl 20110410
  #define TEMP_BED_PIN                         0  // Analog Input
#endif

//
// Misc. Functions
//
#define SDSS                                  17
#define DEBUG_PIN                              0

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      16  // Hardware PWM
#endif

// RS485 pins
#define TX_ENABLE_PIN                         12
#define RX_ENABLE_PIN                         13

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN                  5  // Pullup or pulldown!
#define SPINDLE_LASER_PWM_PIN                 16  // Hardware PWM
#define SPINDLE_DIR_PIN                        6
