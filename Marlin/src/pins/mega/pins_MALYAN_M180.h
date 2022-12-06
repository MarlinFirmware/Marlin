/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Malyan M180 pin assignments
 * Contributed by Timo Birnschein (timo.birnschein@microforge.de)
 * @Timo: sind diese Pin Definitionen immernoch korrekt? (Antwort an turningtides@outlook.de bitte)
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Malyan M180 v.2"
//
// Limit Switches
//
#define X_STOP_PIN                            PinL1
#define Y_STOP_PIN                            PinL3
#define Z_STOP_PIN                            PinL7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     -1
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF1
#define X_DIR_PIN                             PinF0
#define X_ENABLE_PIN                          PinF2

#define Y_STEP_PIN                            PinF5
#define Y_DIR_PIN                             PinF4
#define Y_ENABLE_PIN                          PinF6

#define Z_STEP_PIN                            PinK1
#define Z_DIR_PIN                             PinK0
#define Z_ENABLE_PIN                          PinK2

#define E0_STEP_PIN                           PinA3
#define E0_DIR_PIN                            PinA2
#define E0_ENABLE_PIN                         PinA4

#define E1_STEP_PIN                           PinA7
#define E1_DIR_PIN                            PinA6
#define E1_ENABLE_PIN                         PinG2

//
// Temperature Sensors
//
#define TEMP_BED_PIN                          PinJ0  // Analog Input

// Extruder thermocouples 0 and 1 are read out by two separate ICs using
// SPI for MAX Thermocouple
// Uses a separate SPI bus
#define TEMP_0_CS_PIN                         PinE3  // E3 - CS0
#define TEMP_0_SCK_PIN                        PinE2  // E2 - SCK
#define TEMP_0_MISO_PIN                       PinE5  // E5 - MISO
//#define TEMP_0_MOSI_PIN                    ...  // For MAX31865

#define TEMP_1_CS_PIN                         PinE4  // E4 - CS1
#define TEMP_1_SCK_PIN            TEMP_0_SCK_PIN
#define TEMP_1_MISO_PIN          TEMP_0_MISO_PIN
//#define TEMP_1_MOSI_PIN        TEMP_0_MOSI_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH3
#define HEATER_1_PIN                          PinB5
#define HEATER_BED_PIN                        PinL4

#ifndef FAN_PIN
  #define FAN_PIN                             PinH4  // M106 Sxxx command supported and tested. M107 as well.
#endif

#ifndef FAN_PIN1
  #define FAN_PIN1                            PinB6  // Currently Unsupported by Marlin
#endif
