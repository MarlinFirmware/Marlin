/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
// Anycubic TriGorilla F1 V1 — Kobra Max (HC32F460PETB)
// Based on https://github.com/maikramer/MarlinKobraMax
//
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Trigorilla F1 V1"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Anycubic Kobra Max"
#endif

#define BOARD_NO_NATIVE_USB

//
// Onboard crystal oscillator
// The Kobra Max board uses a 16 MHz XTAL (vs 8 MHz on the Ender 2 Pro).
//
#ifndef BOARD_XTAL_FREQUENCY
  #define BOARD_XTAL_FREQUENCY          16000000  // 16 MHz XTAL
#endif

//
// EEPROM
// The Kobra Max uses external I2C EEPROM (AT24C32 equivalent) on PB6/PB7,
// with SDCARD fallback if no chip is detected.
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM
  #undef NO_EEPROM_SELECTED
#endif
#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PB7
  #define IIC_EEPROM_SCL                    PB6
  #define MARLIN_EEPROM_SIZE              0x1000U // 4K
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE              0x1000U // 4K
#endif
//
// Limit Switches
//
#define ONBOARD_ENDSTOPPULLUPS

#define X_MIN_PIN                           PA6
#define Y_MIN_PIN                           PC5
#define Z_MIN_PIN                           PA8

#define X_MAX_PIN                           -1
#define Y_MAX_PIN                           -1
#define Z_MAX_PIN                           -1

//
// Steppers
//
#define X_ENABLE_PIN                        PC3
#define X_STEP_PIN                          PA5
#define X_DIR_PIN                           PA4

#define Y_ENABLE_PIN                X_ENABLE_PIN
#define Y_STEP_PIN                          PC4
#define Y_DIR_PIN                           PA7

#define Z_ENABLE_PIN                X_ENABLE_PIN
#define Z_STEP_PIN                          PC7
#define Z_DIR_PIN                           PC6

#define Z2_ENABLE_PIN               X_ENABLE_PIN
#define Z2_STEP_PIN                         PB1
#define Z2_DIR_PIN                          PB0

#define E0_ENABLE_PIN               X_ENABLE_PIN
#define E0_STEP_PIN                         PC14
#define E0_DIR_PIN                          PC15

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // T0
#define TEMP_BED_PIN                        PC0   // TB

//
// Heaters
//
#define HEATER_0_PIN                        PA1
#define HEATER_BED_PIN                      PA0

//
// Fans
//
#define FAN0_PIN                            PB9   // Part cooling fan
#define FAN1_PIN                            PA13  // Hotend auto-fan (E0)
#define FAN2_PIN                            PA14  // Controller fan
#define CONTROLLER_FAN_PIN              FAN2_PIN

//
// Misc
//
#define BEEPER_PIN                          PB5
#define FIL_RUNOUT_PIN                      PC13
#define LED_PIN                             -1
#define CASE_LIGHT_PIN                      PB8
#define POWER_LOSS_PIN                      PC2
#define POWER_MONITOR_VOLTAGE_PIN           PC2

//
// Z Probe (strain-gauge style, TX/RX pins used as probe I/O)
//
#define AUTO_LEVEL_TX_PIN                   PB7
#define AUTO_LEVEL_RX_PIN                   PB6
#define PROBE_TARE_PIN         AUTO_LEVEL_TX_PIN

//
// LCD Serial (USART4) — Anycubic DGUS TFT
//
#undef LCD_SERIAL_PORT
#define LCD_SERIAL_PORT                     4

#define BOARD_USART4_TX_PIN                 PB10
#define BOARD_USART4_RX_PIN                 PH2

//
// TMC UART (Software Serial via USART1 pins)
//
#define X_SERIAL_TX_PIN                     PA9
#define X_SERIAL_RX_PIN                     PA15

#define Y_SERIAL_TX_PIN                     PA9
#define Y_SERIAL_RX_PIN                     PA15

#define Z_SERIAL_TX_PIN                     PA9
#define Z_SERIAL_RX_PIN                     PA15

#define E0_SERIAL_TX_PIN                    PA9
#define E0_SERIAL_RX_PIN                    PA15

// Debug UART (USART2)
#define BOARD_USART2_TX_PIN                 PA2
#define BOARD_USART2_RX_PIN                 PA3

// Motor UART (USART3)
#define BOARD_USART3_TX_PIN                 PB4
#define BOARD_USART3_RX_PIN                 PB3

// Primary UART / TMC (USART1)
#define BOARD_USART1_TX_PIN                 PA9
#define BOARD_USART1_RX_PIN                 PA15

//
// SD Card (SDIO)
//
#define ONBOARD_SDIO
#define SD_DETECT_PIN                       PA10

#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_CK_PIN                         PC12
#define SDIO_CMD_PIN                        PD2

// Aliases for HC32 HAL SDIO
#define BOARD_SDIO_D0                       SDIO_D0_PIN
#define BOARD_SDIO_D1                       SDIO_D1_PIN
#define BOARD_SDIO_D2                       SDIO_D2_PIN
#define BOARD_SDIO_D3                       SDIO_D3_PIN
#define BOARD_SDIO_CLK                      SDIO_CK_PIN
#define BOARD_SDIO_CMD                      SDIO_CMD_PIN
#define BOARD_SDIO_DET                      SD_DETECT_PIN

//
// SPI (not used — SDIO handles SD card)
//
#define SCK_PIN                             -1
#define MISO_PIN                            -1
#define MOSI_PIN                            -1
#define SS_PIN                              -1
