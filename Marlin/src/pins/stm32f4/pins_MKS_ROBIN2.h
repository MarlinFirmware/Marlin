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

#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS_ROBIN2 supports up to 2 hotends / E steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS_ROBIN2"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

#define SRAM_EEPROM_EMULATION

//
// Limit Switches
//
#define X_MIN_PIN                           PG8
#define X_MAX_PIN                           PG7
#define Y_MIN_PIN                           PG6
#define Y_MAX_PIN                           PG5
#define Z_MIN_PIN                           PG4
#define Z_MAX_PIN                           PG3

//
// Servos
//
#define SERVO0_PIN                          PB0   // XS2-5
#define SERVO1_PIN                          PF7   // XS1-5
#define SERVO2_PIN                          PF8   // XS1-6

//
// Steppers
//
#define X_STEP_PIN                          PE6
#define X_DIR_PIN                           PE5
#define X_ENABLE_PIN                        PC13

#define Y_STEP_PIN                          PE3
#define Y_DIR_PIN                           PE2
#define Y_ENABLE_PIN                        PE4

#define Z_STEP_PIN                          PE0
#define Z_DIR_PIN                           PB9
#define Z_ENABLE_PIN                        PE1

#define E0_STEP_PIN                         PG10
#define E0_DIR_PIN                          PG9
#define E0_ENABLE_PIN                       PB8

#define E1_STEP_PIN                         PD3
#define E1_DIR_PIN                          PA15
#define E1_ENABLE_PIN                       PD6

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // T1 <-> E0
#define TEMP_1_PIN                          PC2   // T2 <-> E1
#define TEMP_BED_PIN                        PC0   // T0 <-> Bed

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PF3   // Heater0
#define HEATER_1_PIN                        PF2   // Heater1
#define HEATER_BED_PIN                      PF4   // Hotbed
#define FAN_PIN                             PA7   // Fan0

//
// Misc. Functions
//
#define SDSS                                -1    // PB12

#define SD_DETECT_PIN                       PF9
#define BEEPER_PIN                          PG2
