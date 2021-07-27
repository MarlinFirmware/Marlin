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
 * STM32F407VET6 on Index PnP Mobo Rev03
 * Website - https://indexmachines.io/
 */

#define ALLOW_STM32DUINO
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Index Mobo Rev03"
#endif

#define DEFAULT_MACHINE_NAME "Index Pick and Place"

//#define I2C_EEPROM
#define SRAM_EEPROM_EMULATION
#define MARLIN_EEPROM_SIZE                0x2000  // 8KB

//
// Servos
//
#define SERVO0_PIN                          PB6
#define SERVO1_PIN                          PB7

//
// Limit Switches
//
#define X_MIN_PIN                           PC6
#define X_MAX_PIN                           -1
#define Y_MIN_PIN                           PD15
#define Y_MAX_PIN                           -1
#define Z_MIN_PIN                           PD14
#define Z_MAX_PIN                           -1
#define I_MIN_PIN                           -1
#define I_MAX_PIN                           -1
#define J_MIN_PIN                           -1
#define J_MAX_PIN                           -1
#define K_MIN_PIN                           -1
#define K_MAX_PIN                           -1

//
// Steppers
//
#define X_STEP_PIN                          PB15
#define X_DIR_PIN                           PB14
#define X_ENABLE_PIN                        PD9

#define Y_STEP_PIN                          PE15
#define Y_DIR_PIN                           PE14
#define Y_ENABLE_PIN                        PB13

#define Z_STEP_PIN                          PE7
#define Z_DIR_PIN                           PB1
#define Z_ENABLE_PIN                        PE9

#define I_STEP_PIN                         PC4
#define I_DIR_PIN                          PA4
#define I_ENABLE_PIN                       PB0

#define J_STEP_PIN                         PE11
#define J_DIR_PIN                          PE10
#define J_ENABLE_PIN                       PE13

#define K_STEP_PIN                         PC_15  // Not an Arduino pin, might need a new board config
#define K_DIR_PIN                          PC_14  // Not an Arduino pin, might need a new board config
#define K_ENABLE_PIN                       PA3

#define X_SERIAL_TX_PIN                    PD8
#define X_SERIAL_RX_PIN                    PD8
#define Y_SERIAL_TX_PIN                    PB12
#define Y_SERIAL_RX_PIN                    PB12
#define Z_SERIAL_TX_PIN                    PE8
#define Z_SERIAL_RX_PIN                    PE8
#define I_SERIAL_TX_PIN                    PC5
#define I_SERIAL_RX_PIN                    PC5
#define J_SERIAL_TX_PIN                    PE12
#define J_SERIAL_RX_PIN                    PE12
#define K_SERIAL_TX_PIN                    PA2
#define K_SERIAL_RX_PIN                    PA2

// Reduce baud rate to improve software serial reliability
#define TMC_BAUD_RATE                    19200

#define TEMP_0_PIN  PD0 // Not required for this board, really

#define MOSFET_A_PIN                       PE2
#define MOSFET_B_PIN                       PE3
#define MOSFET_C_PIN                       PE4
#define MOSFET_D_PIN                       PE5

// Neopixel Rings
#define NEOPIXEL_PIN                        PC7
#define NEOPIXEL2_PIN                       PC8