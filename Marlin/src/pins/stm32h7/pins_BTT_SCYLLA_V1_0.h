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
 * BigTreeTech Scylla V1.0 (STM32H723VGT6, 25MHz HSE)
 * CNC controller with 4x onboard TMC2160 (SPI), no heaters or thermistors.
 * https://github.com/bigtreetech/Scylla
 *
 * The fourth driver (A) drives the I axis when I_DRIVER_TYPE is set,
 * otherwise it is E0.
 */

#include "env_validate.h"

#if E_STEPPERS > 1 || (HAS_I_AXIS && E_STEPPERS) || HAS_J_AXIS
  #error "BTT Scylla V1.0 only has four drivers (XYZ + A as I or E0)."
#endif

#define BOARD_INFO_NAME   "BTT Scylla V1.0"
#define BOARD_WEBSITE_URL "github.com/bigtreetech/Scylla"

//
// EEPROM
//
#if SHALL_USE_EEPROM(FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE                0x800U  // 2K
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2K
#endif

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PC1
#define Y_DIAG_PIN                          PC3
#define Z_DIAG_PIN                          PE3
#if HAS_I_AXIS
  #define I_DIAG_PIN                        PE0   // A
#else
  #define E0_DIAG_PIN                       PE0   // A
#endif

//
// Limit Switches (optoisolated)
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #define X_OTHR_PIN                        PC6   // X-MAX
#else
  #define X_MIN_PIN                         PD11  // X-MIN
  #define X_MAX_PIN                         PC6   // X-MAX
#endif
#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #define Y_OTHR_PIN                        PD14  // Y-MAX
#else
  #define Y_MIN_PIN                         PA8   // Y-MIN
  #define Y_MAX_PIN                         PD14  // Y-MAX
#endif
#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #define Z_OTHR_PIN                        PD12  // Z-MAX
#else
  #define Z_MIN_PIN                         PC7   // Z-MIN
  #define Z_MAX_PIN                         PD12  // Z-MAX
#endif
#ifdef I_STALL_SENSITIVITY
  #define I_STOP_PIN                  I_DIAG_PIN
  #define I_OTHR_PIN                        PD13  // A-MAX
#else
  #define I_MIN_PIN                         PD15  // A-MIN
  #define I_MAX_PIN                         PD13  // A-MAX
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PE15  // Probe
#endif

//
// Misc. inputs
//
//#define TOOL_SETTER_PIN                   PE7   // Tool
//#define IO_IN_PIN                         PB7   // IO-IN (5V digital input)

//
// Steppers
//
#define X_STEP_PIN                          PA0
#define X_DIR_PIN                           PA1
#define X_ENABLE_PIN                        PC0
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC15
#endif

#define Y_STEP_PIN                          PC13
#define Y_DIR_PIN                           PE6
#define Y_ENABLE_PIN                        PC2
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PC14
#endif

#define Z_STEP_PIN                          PB8
#define Z_DIR_PIN                           PB9
#define Z_ENABLE_PIN                        PE5
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PE2
#endif

// A driver: I axis if enabled, otherwise E0
#if HAS_I_AXIS
  #define I_STEP_PIN                        PD3
  #define I_DIR_PIN                         PD4
  #define I_ENABLE_PIN                      PE1
  #ifndef I_CS_PIN
    #define I_CS_PIN                        PE4
  #endif
#else
  #define E0_STEP_PIN                       PD3
  #define E0_DIR_PIN                        PD4
  #define E0_ENABLE_PIN                     PE1
  #ifndef E0_CS_PIN
    #define E0_CS_PIN                       PE4
  #endif
#endif

//
// SPI pins for TMC2160 stepper drivers
//
#ifndef TMC_SPI_MOSI
  #define TMC_SPI_MOSI                      PB5
#endif
#ifndef TMC_SPI_MISO
  #define TMC_SPI_MISO                      PB4
#endif
#ifndef TMC_SPI_SCK
  #define TMC_SPI_SCK                       PB3
#endif

//
// Integrated TMC2160 driver defaults
//
#if  (HAS_X_AXIS && !AXIS_DRIVER_TYPE_X(TMC2160)) \
  || (HAS_Y_AXIS && !AXIS_DRIVER_TYPE_Y(TMC2160)) \
  || (HAS_Z_AXIS && !AXIS_DRIVER_TYPE_Z(TMC2160)) \
  || (HAS_I_AXIS && !AXIS_DRIVER_TYPE_I(TMC2160)) \
  || (EXTRUDERS >= 1 && !AXIS_DRIVER_TYPE_E0(TMC2160))
  #error "All DRIVER TYPEs must be TMC2160 for BOARD_BTT_SCYLLA_V1_0."
#endif

// RSENSE defaults
#if HAS_X_AXIS
  static_assert(X_RSENSE == 0.05, "X_RSENSE must be 0.05 for BOARD_BTT_SCYLLA_V1_0.");
#endif
#if HAS_Y_AXIS
  static_assert(Y_RSENSE == 0.05, "Y_RSENSE must be 0.05 for BOARD_BTT_SCYLLA_V1_0.");
#endif
#if HAS_Z_AXIS
  static_assert(Z_RSENSE == 0.05, "Z_RSENSE must be 0.05 for BOARD_BTT_SCYLLA_V1_0.");
#endif
#if HAS_I_AXIS
  static_assert(I_RSENSE == 0.05, "I_RSENSE must be 0.05 for BOARD_BTT_SCYLLA_V1_0.");
#endif
#if EXTRUDERS >= 1
  static_assert(E0_RSENSE == 0.05, "E0_RSENSE must be 0.05 for BOARD_BTT_SCYLLA_V1_0.");
#endif

//
// Auxiliary outputs (optoisolated MOSFETs, V-MOS)
//
#ifndef FAN0_PIN
  #define FAN0_PIN                          PA4   // AUX0
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                          PA5   // AUX1
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN                          PA6   // AUX2
#endif

//
// Coolant
//
#define COOLANT_FLOOD_PIN                   PC4   // Cool
#define COOLANT_MIST_PIN                    PA7   // Mist

//
// Spindle / VFD
//
#if HAS_CUTTER
  #ifndef SPINDLE_LASER_ENA_PIN
    #define SPINDLE_LASER_ENA_PIN           PC5   // SP-EN
  #endif
  #ifndef SPINDLE_LASER_PWM_PIN
    #define SPINDLE_LASER_PWM_PIN           PB1   // SPD (analog 0-10V or PWM, per Speed OUT jumper)
  #endif
  #ifndef SPINDLE_DIR_PIN
    #define SPINDLE_DIR_PIN                 PB0   // SP-DIR
  #endif
#endif

//
// Relay (COM/NO/NC)
//
//#define RELAY_PIN                         PD5   // High = COM-NO

//
// SD Connection
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define ONBOARD_SDIO
  #define SDIO_CLOCK                    24000000  // 24MHz
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

//
// NeoPixel LED
//
#ifndef BOARD_NEOPIXEL_PIN
  #define BOARD_NEOPIXEL_PIN                PD6   // RGB
#endif

/**
 * Other headers (not assigned by default):
 *
 *   UART    : PD8 (TX) PD9 (RX)            (USART3)
 *   RS485   : PA9 (TX) PA10 (RX)           (USART1)
 *   I2C     : PB10 (SCL) PB11 (SDA)
 *   CAN     : PD0 (RX) PD1 (TX)            (FDCAN1)
 *   Pi-SPI  : PB13 (SCK) PB14 (MISO) PB15 (MOSI) PB12 (CS) PD10 (RST)
 *   ESP32   : PA2 (TX) PA3 (RX) PE9 (IO0) PE8 (IO4) PE10 (RST)
 *             PE11 (CS) PE12 (CLK) PE13 (MISO) PE14 (MOSI)
 */
