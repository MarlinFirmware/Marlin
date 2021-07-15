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
 * Custom STM32F407VET6/VGT6 pick and place machine board
 * based on the BLACK_STM32F407VE6 pin mapping.
 * 
 * https://www.github.com/index-machines/index
 * Motherboard REV03
 */

/*#if !defined(STM32F4) && !defined(STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"

#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "Index REV03 supports up to 3 E-steppers."
#endif
*/

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Index Motherboard REV03"
#endif

#define DEFAULT_MACHINE_NAME "Index Pick and Place"

//#define I2C_EEPROM
#define SRAM_EEPROM_EMULATION
#define MARLIN_EEPROM_SIZE                0x2000  // 8KB

// No power supply control, or kill switch
#define PS_ON_PIN                           -1
#define KILL_PIN                            -1

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

//
// Steppers
//
#define X_STEP_PIN                          PB15
#define X_DIR_PIN                           PB14
#define X_ENABLE_PIN                        PD9
#ifndef X_CS_PIN
  #define X_CS_PIN                          PD8
#endif

#define Y_STEP_PIN                          PE15
#define Y_DIR_PIN                           PE14
#define Y_ENABLE_PIN                        PB13
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PB12
#endif

#define Z_STEP_PIN                          PE7
#define Z_DIR_PIN                           PB1
#define Z_ENABLE_PIN                        PE9
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PE8
#endif

//E0 is a surrogate for the left head stepper, or "L"
//T0
#define E0_STEP_PIN                         PC4
#define E0_DIR_PIN                          PA4
#define E0_ENABLE_PIN                       PB0
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PC5
#endif

//E1 is a surrogate for the right head stepper, or "R"
//T1
#define E1_STEP_PIN                         PE11
#define E1_DIR_PIN                          PE10
#define E1_ENABLE_PIN                       PE13
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PE12
#endif

//E2 is a surrogate for the auxiliary or conveyour stepper, or "A"
//T2
#define E2_STEP_PIN                         PC_15
#define E2_DIR_PIN                          PC_14
#define E2_ENABLE_PIN                       PA3
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PA2
#endif

//
// Temperature Sensors ( i.e. "ANALOG" input )
//
#define TEMP_0_PIN                          PC10   // T0
#define TEMP_1_PIN                          PC10   // T1
#define TEMP_2_PIN                          PC10   // T2

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   * Software serial
   */

  #define X_SERIAL_TX_PIN                  PD8
  #define X_SERIAL_RX_PIN                  PD8

  #define Y_SERIAL_TX_PIN                  PB12
  #define Y_SERIAL_RX_PIN                  PB12

  #define Z_SERIAL_TX_PIN                  PE8
  #define Z_SERIAL_RX_PIN                  PE8

  #define E0_SERIAL_TX_PIN                 PC5
  #define E0_SERIAL_RX_PIN                 PC5

  #define E1_SERIAL_TX_PIN                 PE12
  #define E1_SERIAL_RX_PIN                 PE12

  #define E2_SERIAL_TX_PIN                 PA2
  #define E2_SERIAL_RX_PIN                 PA2

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Heaters
// 
#define HEATER_0_PIN                        PC11
#define HEATER_1_PIN                        PC12
#define HEATER_2_PIN                        PC13

// "FAN" pin ( MOSFETS )
#ifndef FAN_PIN
  #define FAN_PIN                           PE2 // Mosfet 1
#endif
#define FAN1_PIN                            PE3 // Mosfet 2
#define FAN2_PIN                            PE4 // Mosfet 3
#define FAN3_PIN                            PE5 // Mosfet 4
#define FAN4_PIN                            -1 // VAC_1_SIG
#define FAN_SOFT_PWM                        // slow software PWM rather than hardware

// Neopixel Rings
#define NEOPIXEL_PIN                        PC7 
#define NEOPIXEL2_PIN                       PC8