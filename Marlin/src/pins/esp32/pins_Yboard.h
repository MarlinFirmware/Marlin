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
 * Yboard pin assignments
 *
 * 3D printer control board based on the ESP32 microcontroller.
 * Supports 4 stepper drivers, heated bed, single hotend.
 */

#include "env_validate.h"

#if EXTRUDERS > 1 || E_STEPPERS > 1
  #error "Yboard only supports one E Stepper. Comment out this line to continue."
#elif HAS_MULTI_HOTEND
  #error "Yboard only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME       "Yboard"
#define BOARD_WEBSITE_URL     "github.com/CN-CNC/Yboard"

//
// Disable I2S stepper stream, by default
//
#undef I2S_STEPPER_STREAM
#undef I2S_WS
#undef I2S_BCK
#undef I2S_DATA

//
// Limit Switches
//
#define X_STOP_PIN                            34
#define Y_STOP_PIN                            35
#define Z_STOP_PIN                            32

//
// Steppers
//
#define X_STEP_PIN                            12
#define X_DIR_PIN                             14
#define X_ENABLE_PIN                          15

#define Y_STEP_PIN                            27
#define Y_DIR_PIN                             26
#define Y_ENABLE_PIN                X_ENABLE_PIN

#define Z_STEP_PIN                            25
#define Z_DIR_PIN                             33
#define Z_ENABLE_PIN                X_ENABLE_PIN

#define E0_STEP_PIN                           16
#define E0_DIR_PIN                            17
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Temperature Sensors
//
#define TEMP_0_PIN                            36  // Analog Input
#define TEMP_BED_PIN                          39  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           2
#define Z_MIN_PROBE_PIN                       13
#define HEATER_BED_PIN                         4

//
// MicroSD card
//
#define SD_MOSI_PIN                           23
#define SD_MISO_PIN                           19
#define SD_SCK_PIN                            18
#define SDSS                                   5


// Hardware serial pins
#define HARDWARE_SERIAL1_RX                 21
#define HARDWARE_SERIAL1_TX                 22
