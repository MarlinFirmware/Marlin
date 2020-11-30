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
 * HAL_LPC1768/include/i2c_util.h
 */

#include "../../../inc/MarlinConfigPre.h"

#ifndef I2C_MASTER_ID
  #define I2C_MASTER_ID 1
#endif

#if I2C_MASTER_ID == 0
  #define I2CDEV_M LPC_I2C0
#elif I2C_MASTER_ID == 1
  #define I2CDEV_M LPC_I2C1
#elif I2C_MASTER_ID == 2
  #define I2CDEV_M LPC_I2C2
#else
  #error "Master I2C device not defined!"
#endif

#include <lpc17xx_i2c.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg_default.h>

#ifdef __cplusplus
  extern "C" {
#endif

void configure_i2c(const uint8_t clock_option);

#ifdef __cplusplus
  }
#endif
