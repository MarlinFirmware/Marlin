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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * pins_PRUSA_XBUDDY.h
 * Prusa xBuddy Board (Mk4 / Core One) pin definitions
 *
 * MCU: STM32F427ZIT6 (144-pin LQFP, Cortex-M4, 2MB flash, 192KB RAM)
 *
 * Schematic: https://www.prusa3d.com/downloads/Electronics_drawings/FDM-xBUDDY-44.pdf
 *
 * Pin assignments sourced from Prusa-Firmware-Buddy:
 *   src/common/hwio_pindef.h  (BOARD_IS_XBUDDY() block)
 *   include/device/stm32f4/device/peripherals.h
 *   src/device/stm32f4/interrupts_XBUDDY.cpp
 */

#pragma once

#define BOARD_INFO_NAME   "Prusa xBuddy"
#define DEFAULT_MACHINE_NAME "Prusa MK4"

#define ALLOW_STM32DUINO

#include "env_validate.h"

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "xBuddy supports up to 1 hotend / E-stepper."
#endif

//
// EEPROM — I2C2 (PF0 SDA, PF1 SCL)
//
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE              0x1000  // 4KB

//
// Timers
//
#define STEP_TIMER  6
#define TEMP_TIMER  14

//
// Limit Switches — sensorless homing via TMC DIAG pins
//
#define X_MIN_PIN                           PG9   // X_DIAG
#define X_MAX_PIN                           PG9
#define Y_MIN_PIN                           PE13  // Y_DIAG
#define Y_MAX_PIN                           PE13
#define Z_MIN_PIN                           -1    // virtual (loadcell); use Z_DIAG for sensorless
#define Z_MAX_PIN                           PB4   // Z_DIAG

//
// Steppers
//
#define X_STEP_PIN                          PD7
#define X_DIR_PIN                           PD6
#define X_ENABLE_PIN                        PB9   // shared X/Y enable

#define Y_STEP_PIN                          PD5
#define Y_DIR_PIN                           PD4
#define Y_ENABLE_PIN                        PB9   // shared X/Y enable

#define Z_STEP_PIN                          PD3
#define Z_DIR_PIN                           PD2
#define Z_ENABLE_PIN                        PB8

#define E0_STEP_PIN                         PD1
#define E0_DIR_PIN                          PD0
#define E0_ENABLE_PIN                       PD10

//
// TMC SPI — SPI3 (PC10 SCK, PC11 MISO, PC12 MOSI)
//
#if HAS_DRIVER(TMC2130)
  #define X_CS_PIN                          PG15  // CS_X
  #define Y_CS_PIN                          PB5   // CS_Y
  #define Z_CS_PIN                          PF15  // CS_Z
  #define E0_CS_PIN                         PF12  // CS_E

  #define TMC_USE_SW_SPI
  #define TMC_SW_SCK                        PC10
  #define TMC_SW_MISO                       PC11
  #define TMC_SW_MOSI                       PC12
#endif

//
// TMC UART — SPI3 used as TMC serial on xBuddy; addressed mode
//
#if HAS_DRIVER(TMC2209)
  #define X_HARDWARE_SERIAL                 MSerial3
  #define Y_HARDWARE_SERIAL                 MSerial3
  #define Z_HARDWARE_SERIAL                 MSerial3
  #define E0_HARDWARE_SERIAL                MSerial3

  #define X_SLAVE_ADDRESS                   1
  #define Y_SLAVE_ADDRESS                   3
  #define Z_SLAVE_ADDRESS                   2
  #define E0_SLAVE_ADDRESS                  0
#endif

//
// TMC StallGuard DIAG pins
//
#define X_DIAG_PIN                          PG9
#define Y_DIAG_PIN                          PE13
#define Z_DIAG_PIN                          PB4
#define E0_DIAG_PIN                         PD14

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // Thermistor — Hotend (ADC1_CH10)
#define TEMP_BED_PIN                        PA4   // Thermistor — Bed (ADC1_CH4)
#define TEMP_HEATBREAK_PIN                  PA6   // Thermistor — Heatbreak (ADC1_CH6)

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // Hotend heater
#define HEATER_BED_PIN                      PB0   // Bed heater

#define FAN0_PIN                            PE11  // Print cooling fan
#define FAN1_PIN                            PE9   // Heatbreak fan

//
// Misc
//
#define HEATER_ENABLE_PIN                   PG10  // Active-high heater enable (xBuddy)

//
// Serial / Communication
//
// UART3 (PD8 TX, PD9 RX): Marlin main serial (USB-CDC bridged)
// UART6 (PC6 TX, PC7 RX): MMU2
// UART8 (PE0 TX, PE1 RX): ESP Wi-Fi module
//
#define SERIAL_PORT                         -1    // USB CDC
#ifndef SERIAL_PORT_2
  #define SERIAL_PORT_2                     3     // UART3 — alternate wired serial
#endif

//
// SPI Flash — SPI5, CS=PF2
//
#ifndef SPI_FLASH_CS_PIN
  #define SPI_FLASH_CS_PIN                  PF2
#endif

//
// USB FS
//
#define USB_FS_N_PIN                        PA11
#define USB_FS_P_PIN                        PA12
