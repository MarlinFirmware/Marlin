/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Arduino Mega with RAMPS BTT v1.6+
 * Schematic: https://github.com/bigtreetech/ramps-1.6/blob/master/Ramps1.6/hardware/R6%2B-SCH.pdf
 *
 * Differences from RAMPS v1.4:
 *  - SPI Stepper drivers use AVR hardware SPI pins
 *  - SPI Stepper CS pins are different
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "RAMPS BTT 1.6+"

#if HAS_TMC_SPI && DISABLED(TMC_USE_SW_SPI)
  #define TMC_SPI_MOSI                      MOSI  // 51
  #define TMC_SPI_MISO                      MISO  // 50
  #define TMC_SPI_SCK                        SCK  // 52
#endif

#define X_CS_PIN                              63  // A0
#define Y_CS_PIN                              40
#define Z_CS_PIN                              42
#define E0_CS_PIN                             65  // A11
#define E1_CS_PIN                             66  // A12

#include "pins_RAMPS.h"
