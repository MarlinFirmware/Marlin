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
 * Anycubic TriGorilla Gen V3.0.6 (HC32F460KETA) - Common pin definitions
 * Used by Kobra Go and Kobra Neo.
 *
 * Schematic reference: https://github.com/ANYCUBIC-3D/Kobra_Neo
 *                      https://github.com/ANYCUBIC-3D/Kobra_Go
 */

#include "env_validate.h"

#define BOARD_NO_NATIVE_USB

//
// Crystal
//
#ifndef BOARD_XTAL_FREQUENCY
  #define BOARD_XTAL_FREQUENCY           8000000  // 8 MHz XTAL
#endif

//
// EEPROM (flash emulation — no I2C EEPROM on board)
//
#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION
  #undef NO_EEPROM_SELECTED
#endif
#define MARLIN_EEPROM_SIZE               0x2000U  // 8K

//
// Limit Switches
//
#define X_MIN_PIN                           PH2
#define Y_MIN_PIN                           PC13
#define Z_MIN_PIN                           PC14

//
// Steppers — all axes share a single enable pin
//
#define X_ENABLE_PIN                        PA15
#define X_STEP_PIN                          PA12
#define X_DIR_PIN                           PA11

#define Y_ENABLE_PIN                        PA15
#define Y_STEP_PIN                          PA9
#define Y_DIR_PIN                           PA8

#define Z_ENABLE_PIN                        PA15
#define Z_STEP_PIN                          PC7
#define Z_DIR_PIN                           PC6

#define E0_ENABLE_PIN                       PA15
#define E0_STEP_PIN                         PB15
#define E0_DIR_PIN                          PB14

//
// TMC UART — all axes share a single-wire UART on PB2
//
#define X_SERIAL_TX_PIN                     PB2
#define X_SERIAL_RX_PIN                     PB2
#define Y_SERIAL_TX_PIN                     PB2
#define Y_SERIAL_RX_PIN                     PB2
#define Z_SERIAL_TX_PIN                     PB2
#define Z_SERIAL_RX_PIN                     PB2
#define E0_SERIAL_TX_PIN                    PB2
#define E0_SERIAL_RX_PIN                    PB2

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3   // T0 (hotend)
#define TEMP_BED_PIN                        PC1   // TB

//
// Heaters
//
#define HEATER_0_PIN                        PB8   // HE0
#define HEATER_BED_PIN                      PB9   // HB

//
// Fans
//
#define FAN0_PIN                            PB5   // Part cooling
#define FAN1_PIN                            PB13  // Hotend auto-fan
#define FAN2_PIN                            PB12  // Controller fan

//
// Misc
//
#define BEEPER_PIN                          PB7
#define POWER_LOSS_PIN                      PC2
#define POWER_MONITOR_VOLTAGE_PIN           PC2
#define FIL_RUNOUT_PIN                      PC15  // Active LOW

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PB6   // Power-off control
#endif

//
// Probe — LeviQ hall-effect sensor (auto-level, no deploy/stow by default)
//
#define AUTO_LEVEL_TX_PIN                   -1
#define AUTO_LEVEL_RX_PIN                   PA1

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN      AUTO_LEVEL_RX_PIN
#endif

//
// SD Card — 4-bit SDIO (no SPI)
//
#define SD_DETECT_PIN                       PA10
#ifndef ONBOARD_SDIO
  #define ONBOARD_SDIO
#endif

//
// Display (serial LCD / DWIN) — USART4: TX=PB4, RX=PB3
// Configure via SERIAL_PORT_2 in Configuration.h.
//
