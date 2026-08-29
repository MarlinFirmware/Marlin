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

//
// EEPROM
//
#if SHALL_USE_EEPROM(I2C_EEPROM)
  #define I2C_EEPROM                              // Onboard I2C EEPROM
  #define MARLIN_EEPROM_SIZE             0x1000U  // 4K
#endif

#define Z_STEP_PIN                          PC14
#define Z_DIR_PIN                           PC15

#define BTN_ENC_EN                          -1

#include "pins_MKS_ROBIN_E3_common.h"
