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
 * Leapfrog Xeed Driver board pin assignments
 * ATmega2560
 *
 * This board is used by other Leapfrog printers in addition to the Xeed,
 * such as the Creatr HS and Bolt. The pin assignments vary wildly between
 * printer models. As such this file is currently specific to the Xeed.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Leapfrog Xeed 2015"

//
// Limit Switches
//
#define X_STOP_PIN                            PinL2  // 'X Min'
#define Y_STOP_PIN                            PinL1  // 'Y Min'
#define Z_STOP_PIN                            PinL0  // 'Z Min'

//
// Steppers
// The Xeed utilizes three Z-axis motors, which use the X, Y, and Z stepper connectors
// on the board. The X and Y steppers use external drivers, attached to signal-level
// Y-axis and X-axis connectors on the board, which map to distinct CPU pins from
// the on-board X/Y stepper drivers.
//

// X-axis signal-level connector
#define X_STEP_PIN                            PinK3
#define X_DIR_PIN                             PinK2
#define X_ENABLE_PIN                          PinK4  // Not actually used on Xeed, could be repurposed

// Y-axis signal-level connector
#define Y_STEP_PIN                            PinA1
#define Y_DIR_PIN                             PinA0
#define Y_ENABLE_PIN                          PinA2  // Not actually used on Xeed, could be repurposed

// ZMOT connector (Front Right Z Motor)
#define Z_STEP_PIN                            PinC6
#define Z_DIR_PIN                             PinC5
#define Z_ENABLE_PIN                          PinC7

// XMOT connector (Rear Z Motor)
#define Z2_STEP_PIN                           PinA6
#define Z2_DIR_PIN                            PinK1
#define Z2_ENABLE_PIN                         PinA7

// YMOT connector (Front Left Z Motor)
#define Z3_STEP_PIN                           PinJ1
#define Z3_DIR_PIN                            PinJ0
#define Z3_ENABLE_PIN                         PinG2

// EMOT2 connector
#define E0_STEP_PIN                           PinC0
#define E0_DIR_PIN                            PinG1
#define E0_ENABLE_PIN                         PinC1

// EMOT connector
#define E1_STEP_PIN                           PinC3
#define E1_DIR_PIN                            PinC2
#define E1_ENABLE_PIN                         PinC4

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                        PinL7  // ROT2 Connector
#define FIL_RUNOUT2_PIN                       PinL5  // ROT1 Connector

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinJ0  // T3 Connector
#define TEMP_1_PIN                            PinB7  // T1 Connector
#define TEMP_BED_PIN                          PinJ1  // BED Connector (Between T1 and T3)

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH5  // Misc Connector, pins 3 and 4 (Out2)
#define HEATER_1_PIN                          PinH6  // Misc Connector, pins 5 and 6 (Out3)
#define HEATER_BED_PIN                        PinH3  // Misc Connector, pins 9(-) and 10(+) (OutA)

#define FAN_PIN                               PinB4  // Misc Connector, pins 7(-) and 8 (+) (Out4)

#define LED_PIN                               PinB7

#define SOL1_PIN                              PinH4  // Misc Connector, pins 1(-) and 2(+) (Out1)

// Door Closed Sensor
//#define DOOR_PIN                            PinL4  // HM1 Connector
