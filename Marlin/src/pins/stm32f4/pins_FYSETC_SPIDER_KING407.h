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

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Spider King V1.0"
#endif

//
// EEPROM Emulation
//
#if NO_EEPROM_SELECTED
  #undef NO_EEPROM_SELECTED
  //#define FLASH_EEPROM_EMULATION
  //#define SRAM_EEPROM_EMULATION
  #define I2C_EEPROM
#endif

#if ENABLED(I2C_EEPROM)
  #define I2C_EEPROM
  #define I2C_SCL_PIN                       PF1
  #define I2C_SDA_PIN                       PF0
  #define MARLIN_EEPROM_SIZE             0x1000  // 4KB
#endif

#include "pins_FYSETC_SPIDER_KING_common.h"
