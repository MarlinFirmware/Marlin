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
 * MKS Robin E3D v1.1 (STM32F103RCT6) board pin assignments
 */

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "MKS Robin E3D v1.1 only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS Robin E3D V1.1"
#endif

//
// Steppers
//
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC7
#endif
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD2
#endif
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PC12
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PC11
#endif

//
// SPI pins for TMC2130 stepper drivers
// Software and hardware actually, they are connected to SPI2 bus.
//
#ifndef TMC_SPI_MOSI
  #define TMC_SPI_MOSI                      PB15
#endif
#ifndef TMC_SPI_MISO
  #define TMC_SPI_MISO                      PB14
#endif
#ifndef TMC_SPI_SCK
  #define TMC_SPI_SCK                       PB13
#endif

#include "pins_MKS_ROBIN_E3_V1_1_common.h" // ... MKS_ROBIN_E3_common
