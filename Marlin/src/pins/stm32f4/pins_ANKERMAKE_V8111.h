/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * AnkerMake M5 (V8111) mainboard
 * STM32F407VGT6 or GD32F407VGT6. The stock firmware sits behind a read-protected
 * 256K bootloader, so Marlin is flashed over SWD at 0x08000000 (no bootloader).
 *
 * Pins from AnkerMake's Marlin release V3.0.75:
 *   https://github.com/ankermake/AnkerMake-Marlin
 *
 * The MCU is driven by the printer's Linux host on USART1 (and USART3).
 * The toolhead "nozzle board" (strain gauge probe, and on newer boards the
 * hotend heater and thermistor) is a separate MCU on UART4.
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "AnkerMake V8111 only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "AnkerMake V8111"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "AnkerMake M5"
#endif

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION
  #undef NO_EEPROM_SELECTED
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

//
// Power switches, set early so drivers, heaters and the nozzle board are powered.
// The second bed MOSFET (BED_CTRL2) enables the bed heater with the opposite
// state on boards that pull OCI_IN (PC6) low.
//
#define MOTOR_EN_PIN                        PA7   // Stepper driver power
#define HEATER_EN_PIN                       PC2   // Toolhead heater power
#define NOZZLE_BOARD_PWR_PIN                PC5   // Nozzle board power
#define HEATER_BED_CTRL2_PIN                PB12  // Second bed MOSFET
#define OCI_IN_PIN                          PC6   // Over-current latch / board revision

#define BOARD_PREINIT() do{ \
  OUT_WRITE(MOTOR_EN_PIN, HIGH); \
  OUT_WRITE(HEATER_EN_PIN, HIGH); \
  OUT_WRITE(NOZZLE_BOARD_PWR_PIN, HIGH); \
  SET_INPUT_PULLDOWN(OCI_IN_PIN); \
  DELAY_US(10000); \
  OUT_WRITE(HEATER_BED_CTRL2_PIN, READ(OCI_IN_PIN) ? HIGH : LOW); \
}while(0)

//
// Limit Switches
//
#define X_DIAG_PIN                          PC14
#define Y_DIAG_PIN                          PE10
#define Z_DIAG_PIN                          PD14
#define Z2_DIAG_PIN                         PB10

#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
#else
  #define X_STOP_PIN                        PC15
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
#else
  #define Y_STOP_PIN                        PE11
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
#else
  #define Z_STOP_PIN                        PE1
#endif

#if ENABLED(Z_MULTI_ENDSTOPS)
  #ifdef Z2_STALL_SENSITIVITY
    #define Z2_STOP_PIN              Z2_DIAG_PIN
  #else
    #define Z2_STOP_PIN                     PB3
  #endif
#endif

//
// Z Probe (nozzle board strain gauge)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB0
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE6

#define Y_ENABLE_PIN                        PE7
#define Y_STEP_PIN                          PB2
#define Y_DIR_PIN                           PE8

#define Z_ENABLE_PIN                        PD3
#define Z_STEP_PIN                          PC9
#define Z_DIR_PIN                           PB7

#define Z2_ENABLE_PIN                       PD2
#define Z2_STEP_PIN                         PC8
#define Z2_DIR_PIN                          PB5

#define E0_ENABLE_PIN                       PA8
#define E0_STEP_PIN                         PD15
#define E0_DIR_PIN                          PA15

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // TH0
#define TEMP_BED_PIN                        PA4   // TB

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PE5   // Heater0 (power gate to the nozzle board on newer boards)
#define HEATER_BED_PIN                      PB15  // Hotbed
#define HEATER_0_INVERTING
#define HEATER_BED_INVERTING
#define BOARD_OPENDRAIN_MOSFETS

#ifndef FAN0_PIN
  #define FAN0_PIN                          PE12  // Fan0
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                          PE13  // Fan1
#endif
#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN                PB14
#endif

//
// TMC2209 stepper drivers
//
#if HAS_TMC_UART
  /**
   * Software serial, one wire per driver
   */
  #define X_SERIAL_TX_PIN                   PC13
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PE9
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PD7
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define Z2_SERIAL_TX_PIN                  PE0
  #define Z2_SERIAL_RX_PIN      Z2_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PC12
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif

#endif // HAS_TMC_UART
