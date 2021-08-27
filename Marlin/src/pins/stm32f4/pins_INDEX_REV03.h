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

/*
By default, the extra stepper motor configuration is:
  I = Left Head
  J = Right Head
  K = Auxiliary (Conveyor belt)
*/

#define SRAM_EEPROM_EMULATION
#define MARLIN_EEPROM_SIZE                  0x2000  // 8KB

//
// Servos
//
#define SERVO0_PIN                          PB10
#define SERVO1_PIN                          PB11

//
// Limit Switches
//
#define X_MIN_PIN                           PC6
#define X_MAX_PIN                           -1
#define Y_MIN_PIN                           PD15
#define Y_MAX_PIN                           -1
#define Z_MIN_PIN                           PD14
#define Z_MAX_PIN                           -1

// None of these require limit switches by default, so we leave these commented
// here for your reference.
// #define I_MIN_PIN                           PA8
// #define I_MAX_PIN                           PA8
// #define J_MIN_PIN                           PD13
// #define J_MAX_PIN                           PD13
// #define K_MIN_PIN                           PC9
// #define K_MAX_PIN                           PC9

//
// Steppers
//
#define X_STEP_PIN                          PB15
#define X_DIR_PIN                           PB14
#define X_ENABLE_PIN                        PD9
#define X_SERIAL_TX_PIN                     PD8
#define X_SERIAL_RX_PIN                     PD8

#define Y_STEP_PIN                          PE15
#define Y_DIR_PIN                           PE14
#define Y_ENABLE_PIN                        PB13
#define Y_SERIAL_TX_PIN                     PB12
#define Y_SERIAL_RX_PIN                     PB12

#define Z_STEP_PIN                          PE7
#define Z_DIR_PIN                           PB1
#define Z_ENABLE_PIN                        PE9
#define Z_SERIAL_TX_PIN                     PE8
#define Z_SERIAL_RX_PIN                     PE8

#define I_STEP_PIN                          PC4
#define I_DIR_PIN                           PA4
#define I_ENABLE_PIN                        PB0
#define I_SERIAL_TX_PIN                     PC5
#define I_SERIAL_RX_PIN                     PC5

#define J_STEP_PIN                          PE11
#define J_DIR_PIN                           PE10
#define J_ENABLE_PIN                        PE13
#define J_SERIAL_TX_PIN                     PE12
#define J_SERIAL_RX_PIN                     PE12
#define K_SERIAL_TX_PIN                     PA2
#define K_SERIAL_RX_PIN                     PA2

#define K_STEP_PIN                          PD6
#define K_DIR_PIN                           PD7
#define K_ENABLE_PIN                        PA3

// Reduce baud rate to improve software serial reliability
#define TMC_BAUD_RATE                       19200

// Not required for this board. Fails to compile otherwise.
// PD0 is not connected on this board.
#define TEMP_0_PIN  PD0

// General use mosfets, useful for things like pumps and solenoids
#define FAN_PIN                             PE2
#define FAN1_PIN                            PE3
#define FAN2_PIN                            PE4
#define FAN3_PIN                            PE5

// Neopixel Rings
#define NEOPIXEL_PIN                        PC7
#define NEOPIXEL2_PIN                       PC8

// SPI
#define MISO_PIN                            PB4
#define MOSI_PIN                            PB5
#define SCK_PIN                             PB3

// I2C
#define I2C_SDA_PIN                         PB7
#define I2C_SCL_PIN                         PB6

/*
The index mobo rev03 has 3 aux ports. We define them here so they may be used
in other places and to make sure someone doesn't have to go look up the pinout
in the board files. Each 12 pin aux port has this pinout:

VDC    1   2    GND
3.3V   3   4    SCL  (I2C_SCL_PIN)
PWM1   5   6    SDA  (I2C_SDA_PIN)
PWM2   7   8    CIPO (MISO_PIN)
A1     9  10    COPI (MOSI_PIN)
A2     11 12    SCK  (SCK_PIN)
*/
#define INDEX_AUX1_PWM1                     PA15
#define INDEX_AUX1_PWM2                     PA5
#define INDEX_AUX1_A1                       PC0
#define INDEX_AUX1_A2                       PC1

#define INDEX_AUX2_PWM1                     PA6
#define INDEX_AUX2_PWM2                     PA7
#define INDEX_AUX2_A1                       PC2
#define INDEX_AUX2_A2                       PC3

#define INDEX_AUX3_PWM1                     PB8
#define INDEX_AUX3_PWM2                     PB9
#define INDEX_AUX3_A1                       PA0
#define INDEX_AUX3_A2                       PA1