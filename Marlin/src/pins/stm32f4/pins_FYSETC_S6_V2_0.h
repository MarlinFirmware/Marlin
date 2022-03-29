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

#define BOARD_INFO_NAME "FYSETC S6 2.0"

//
// EEPROM Emulation
//
#if NO_EEPROM_SELECTED
  #undef NO_EEPROM_SELECTED
  //#define FLASH_EEPROM_EMULATION
  #define I2C_EEPROM
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PE9

#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                   PE8
  #define Y_SERIAL_TX_PIN                   PC4
  #define Z_SERIAL_TX_PIN                   PD12
  #define E0_SERIAL_TX_PIN                  PA15
  #define E1_SERIAL_TX_PIN                  PC5
  #define E2_SERIAL_TX_PIN                  PE0
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#define TMC_USE_SW_SPI
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PE14
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PE13
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PE12
  #endif
#endif

#include "pins_FYSETC_S6.h"
