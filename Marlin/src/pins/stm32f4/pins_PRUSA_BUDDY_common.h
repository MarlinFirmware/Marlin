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

/**
 * pins_PRUSA_BUDDY_common.h
 * Prusa Buddy Board common pin definitions
 *
 * This common file is shared by Buddy Mini and xBuddy board variants.
 *
 * MCU:       STM32F407VGT6 (STM32F407VG, 100-pin LQFP)
 * Schematic: https://github.com/prusa3d/Buddy-board-MINI-PCB
 */

#pragma once

#include "env_validate.h"

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION
#endif
#if ENABLED(FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_LEVELING
#endif

//
// Servo pin
//
#define SERVO0_PIN                          PE14

//
// Trinamic Stallguard / Diag pins
// (Using the UART-based TMC2209 on this board, so diag pins used for endstops)
//
#define X_STOP_PIN                          PG3
#define Y_STOP_PIN                          PG4
#define Z_STOP_PIN                          PG5

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PG6
#endif

//
// Filament Runout
//
#define FIL_RUNOUT_PIN                      PE5
#define FIL_RUNOUT2_PIN                     PE6

//
// Steppers
//
#define X_STEP_PIN                          PG1
#define X_DIR_PIN                           PB10
#define X_ENABLE_PIN                        PG0

#define Y_STEP_PIN                          PF14
#define Y_DIR_PIN                           PF15
#define Y_ENABLE_PIN                        PF13

#define Z_STEP_PIN                          PF11
#define Z_DIR_PIN                           PF12
#define Z_ENABLE_PIN                        PC5

#define E0_STEP_PIN                         PC14
#define E0_DIR_PIN                          PC13
#define E0_ENABLE_PIN                       PC15

//
// TMC2209 UART Stepper Drivers
//
#if HAS_TMC_UART
  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                 PB2
  #endif
  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                 PE2
  #endif
  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                 PE3
  #endif
  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN                PE4
  #endif

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE            19200
  #endif
#endif

//
// UART connector (J7) - USART3 via alternate pins
// PB10 is already used for X_DIR_PIN, so use alternate USART3 pins
//
#define BOARD_UART_TX_PIN                   PD8   // USART3_TX (alternate)
#define BOARD_UART_RX_PIN                   PD9   // USART3_RX (alternate)

//
// Temperature Sensors
//
// TODO: Upstream Prusa firmware marks these as "Dummy, measured by puppy"
//   — indicating external measurement via a separate controller/daughter-board.
//   Verify whether local ADC reads are valid or if values come over I2C/CAN.
//
#define TEMP_0_PIN                          PC1   // Analog Input - THERM0 (Nozzle)
#define TEMP_1_PIN                          PC2   // Analog Input - THERM1
#define TEMP_BED_PIN                        PC0   // Analog Input - BED_THERM

//
// Heaters / Fans
//
// TODO: Upstream Prusa firmware marks HEATER_0 and HEATER_BED as
//   "Dummy, measured by puppy" — heater PWM may be driven externally.
//
#define HEATER_0_PIN                        PA1
#define HEATER_BED_PIN                      PA2

// TODO: Upstream Prusa firmware has "#undef E0_AUTO_FAN_PIN" with the note
//   "no auto fan pin - handled by dwarf". The "dwarf" appears to be a
//   daughter-board or CANbus peripheral that manages fan control.
//   Investigate dwarf interface before enabling AUTO_FAN here.
//
#ifndef FAN0_PIN
  #define FAN0_PIN                          PF7
#endif
#define FAN1_PIN                            PF6

//
// Misc. Functions
//
#define PS_ON_PIN                           PA4
#define POWER_LOSS_PIN                      PA4   // Power-loss / nAC_FAULT

//
// SD Card - SDIO
//
#define ONBOARD_SDIO
#define SD_DETECT_PIN                       PA8

//
// NeoPixel / LED
//
#define LED_PIN                             PA15  // Status LED

//
// I2C (ESP32 WiFi module on level converter)
//
#define I2C_SCL_PIN                         PB6
#define I2C_SDA_PIN                         PB7

//
// SPI Flash (onboard W25Q128 via SPI1)
//
#define SPI_FLASH
#if ENABLED(SPI_FLASH)
  #define SPI_DEVICE                 1     // SPI1
  #define SPI_FLASH_SIZE             0x1000000 // 16MB
  // SPI1 pins: PA5=SCK, PA6=MISO, PA7=MOSI
  #if !PIN_EXISTS(PS_ON)
    #define SPI_FLASH_CS_PIN                PA4   // Flash CS (shares PS_ON_PIN - mutually exclusive)
  #endif
  #define SPI_FLASH_SCK_PIN                 PA5
  #define SPI_FLASH_MISO_PIN                PA6
  #define SPI_FLASH_MOSI_PIN                PA7
#endif

//
// Ethernet (LAN8742A RMII)
//
#if HAS_ETHERNET
  #ifndef ETH_PHY_TYPE
    #define ETH_PHY_TYPE                LAN8742A
    #define ETH_PHY_ADDR             1
  #endif
  // EPHY_RST via level converter - use PF2 (free GPIO, not conflicting with TMC serial)
  #define ETH_RESET_PIN                     PF2
#endif

//
// LCD / Controller (EXP1/EXP2 connectors)
//
#if IS_NEWPANEL
  #define BEEPER_PIN                        PC7
  #define BTN_EN1                           PG11
  #define BTN_EN2                           PG10
  #define BTN_ENC                           PG9
  #ifndef ENCODER_STEPS_PER_MENU_ITEM
    #define ENCODER_STEPS_PER_MENU_ITEM 2
  #endif
#endif
