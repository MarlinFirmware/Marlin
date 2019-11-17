/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

// adapted from  I2C/master/master.c example
//   https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/master_8c_source.html

#define USEDI2CDEV_M  1  // use I2C1 controller

#if USEDI2CDEV_M == 0
  #define I2CDEV_M LPC_I2C0
#elif USEDI2CDEV_M == 1
  #define I2CDEV_M LPC_I2C1
#elif USEDI2CDEV_M == 2
  #define I2CDEV_M LPC_I2C2
#else
  #error "Master I2C device not defined!"
#endif

#ifdef __cplusplus
  extern "C" {
#endif

#include <lpc17xx_i2c.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg_default.h>

uint8_t digipot_mcp4451_start(uint8_t sla);
void digipot_mcp4451_init();
uint8_t digipot_mcp4451_send_byte(uint8_t data);

#ifdef __cplusplus
  }
#endif
