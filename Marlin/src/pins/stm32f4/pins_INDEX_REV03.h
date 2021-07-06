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
  #error "Index V1.1 supports up to 3 E-steppers."
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
#define SERVO0_PIN                          PE13
#define SERVO1_PIN                          PE14

//
// Limit Switches
//
#define X_MIN_PIN                           PE8
#define X_MAX_PIN                           -1
#define Y_MIN_PIN                           PE9
#define Y_MAX_PIN                           -1
#define Z_MIN_PIN                           PE10
#define Z_MAX_PIN                           -1

//
// Steppers
//
#define X_STEP_PIN                          PB3
#define X_DIR_PIN                           PD7
#define X_ENABLE_PIN                        PB5
#ifndef X_CS_PIN
  #define X_CS_PIN                          PB4
#endif

#define Y_STEP_PIN                          PD0
#define Y_DIR_PIN                           PC12
#define Y_ENABLE_PIN                        PD2
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD1
#endif

#define Z_STEP_PIN                          PE5
#define Z_DIR_PIN                           PE4
#define Z_ENABLE_PIN                        PC13
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PE6
#endif

//E0 is a surrogate for the Rotation stepper, or "R"
//T0
#define E0_STEP_PIN                         PE1
#define E0_DIR_PIN                          PE0
#define E0_ENABLE_PIN                       PE3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PE2
#endif

//E1 is a surrogate for the paste extrusion stepper, or "P"
//T1
#define E1_STEP_PIN                         PB7
#define E1_DIR_PIN                          PB6
#define E1_ENABLE_PIN                       PB9
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PB8
#endif

//E2 is a surrogate for the "C" stepper
//T2
#define E2_STEP_PIN                         PD4
#define E2_DIR_PIN                          PD3
#define E2_ENABLE_PIN                       PD6
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PD5
#endif

//
// Temperature Sensors ( i.e. "ANALOG" input )
//
#define TEMP_0_PIN                          PC2   // T0
#define TEMP_1_PIN                          PC3   // T1
#define TEMP_2_PIN                          PC3   // T2

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   * Software serial
   */

  #define X_SERIAL_TX_PIN                  PB4
  #define X_SERIAL_RX_PIN                  PB4

  #define Y_SERIAL_TX_PIN                  PD1
  #define Y_SERIAL_RX_PIN                  PD1

  #define Z_SERIAL_TX_PIN                  PE6
  #define Z_SERIAL_RX_PIN                  PE6

  #define E0_SERIAL_TX_PIN                 PE2
  #define E0_SERIAL_RX_PIN                 PE2

  #define E1_SERIAL_TX_PIN                 PB8
  #define E1_SERIAL_RX_PIN                 PB8

  #define E2_SERIAL_TX_PIN                 PD5
  #define E2_SERIAL_RX_PIN                 PD5

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Heaters
// 
#define HEATER_0_PIN                        PA1
#define HEATER_1_PIN                        PA1
#define HEATER_2_PIN                        PA1

// "FAN" pin ( MOSFETS )
#ifndef FAN_PIN
  #define FAN_PIN                           PC6 // Mosfet 1
#endif
#define FAN1_PIN                            PD15 // Mosfet 2
#define FAN2_PIN                            PD14 // Mosfet 3
#define FAN3_PIN                            PD13 // Mosfet 4
#define FAN4_PIN                            PC1 // VAC_1_SIG
#define FAN_SOFT_PWM                        // slow software PWM rather than hardware

// Neopixel Rings
#define NEOPIXEL_PIN                        PE11 
#define NEOPIXEL2_PIN                       PE12