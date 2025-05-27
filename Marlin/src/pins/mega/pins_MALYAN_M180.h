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
#define X_STOP_PIN                            48
#define Y_STOP_PIN                            46
#define Z_STOP_PIN                            42

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     -1
#endif

//
// Steppers
//
#define X_STEP_PIN                            55
#define X_DIR_PIN                             54
#define X_ENABLE_PIN                          56

#define Y_STEP_PIN                            59
#define Y_DIR_PIN                             58
#define Y_ENABLE_PIN                          60

#define Z_STEP_PIN                            63
#define Z_DIR_PIN                             62
#define Z_ENABLE_PIN                          64

#define E0_STEP_PIN                           25
#define E0_DIR_PIN                            24
#define E0_ENABLE_PIN                         26

#define E1_STEP_PIN                           29
#define E1_DIR_PIN                            28
#define E1_ENABLE_PIN                         39

//
// Temperature Sensors
//
#define TEMP_BED_PIN                          15  // Analog Input

// Extruder thermocouples 0 and 1 are read out by two separate ICs using
// SPI for MAX Thermocouple
// Uses a separate SPI bus
#define TEMP_0_CS_PIN                          5  // E3 - CS0
#define TEMP_0_SCK_PIN                        78  // E2 - SCK
#define TEMP_0_MISO_PIN                        3  // E5 - MISO
//#define TEMP_0_MOSI_PIN                    ...  // For MAX31865

#define TEMP_1_CS_PIN                          2  // E4 - CS1
#define TEMP_1_SCK_PIN            TEMP_0_SCK_PIN
#define TEMP_1_MISO_PIN          TEMP_0_MISO_PIN
//#define TEMP_1_MOSI_PIN        TEMP_0_MOSI_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                           6
#define HEATER_1_PIN                          11
#define HEATER_BED_PIN                        45

#ifndef FAN0_PIN
  #define FAN0_PIN                             7  // M106 Sxxx command supported and tested. M107 as well.
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                            12  // Currently Unsupported by Marlin
#endif
