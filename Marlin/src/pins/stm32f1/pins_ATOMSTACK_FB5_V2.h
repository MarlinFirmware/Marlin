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
 * AtomStack FB5 V2.0 (STM32F103RCT6) common board pin assignments
 */

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "AtomStack FB5 V2.0"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "ATOMSTACK Cambrian"
#endif

//
// EEPROM
//
#if ANY(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2K
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2K
#endif

//
// Servos
//
#define SERVO0_PIN                          PA3

//
// Limit Switches
//
#define X_STOP_PIN                          PA4
#define Y_STOP_PIN                          PA5
#define Z_STOP_PIN                          PC6

//
// Z Probe
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB1
#endif

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE) && !defined(PROBE_ENABLE_PIN)
  #define PROBE_ENABLE_PIN            SERVO0_PIN
#endif

//
// Steppers
//
#define X_STEP_PIN                          PC0
#define X_DIR_PIN                           PB2
#define X_ENABLE_PIN                        PC13

#define Y_STEP_PIN                          PC2
#define Y_DIR_PIN                           PB9
#define Y_ENABLE_PIN                        PB12

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB7
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB6
#endif
#define Z_ENABLE_PIN                        PB8

#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB3
#define E0_ENABLE_PIN                       PB5

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  MSerial1
  //#define Y_HARDWARE_SERIAL  MSerial1
  //#define Z_HARDWARE_SERIAL  MSerial1
  //#define E0_HARDWARE_SERIAL MSerial1

  #define X_SERIAL_TX_PIN                   PC7
  #define Y_SERIAL_TX_PIN                   PD2
  #define Z_SERIAL_TX_PIN                   PC12
  #define E0_SERIAL_TX_PIN                  PC11

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif

#endif // HAS_TMC_UART

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC9
#define HEATER_BED_PIN                      PC8

#define FAN0_PIN                            PA8

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // TH1
#define TEMP_BED_PIN                        PA1   // TB

//
// Filament Runout Sensor
//
//#define FIL_RUNOUT_PIN                    PB10  // MT_DET (Mechanical Limit Detector)
#define FIL_RUNOUT_PIN                      PC15  // E0-STOP

//
// Power-loss Detection
//
#ifndef POWER_LOSS_PIN
  //#define POWER_LOSS_PIN                  PC12  // Power Loss Detection: PWR-DET
#endif

//
// USB connect control
//
#ifndef USB_CONNECT_PIN
  #define USB_CONNECT_PIN                   PB11
  #define USB_CONNECT_INVERTING             false
#endif

/**
 *   AtomStack FB5 V2.0
 *        ------
 *    5V | 1  2 | 5V
 *   GND | 3  4 | GND
 *    NC | 5  6 | PA9  (TX1)
 *    NC | 7  8 | PA10 (RX1)
 *        ------
 *         AUX1
 */

/**
  * DWG80480C043_02WTCZ12 4.3 Inch Smart LCD [800x480] pinout
  *            ---
  *        5V | 1 |
  *        NC | 2 |
  * (TX1) PA9 | 3 |
  * (RX1)PA10 | 4 |
  *        NC | 5 |
  *       GND | 6 |
  *            ---
  *        DWIN (plug)
  */

// LED driving pin
#ifndef BOARD_NEOPIXEL_PIN
  #define BOARD_NEOPIXEL_PIN                PA2
#endif

//
// SD Card
//
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SPI_DEVICE                     2  // Maple
#define ONBOARD_SD_CS_PIN              SD_SS_PIN
#define SD_DETECT_PIN                       PC10
#define NO_SD_HOST_DRIVE

// TODO: This is the only way to set SPI for SD on STM32 (for now)
#define ENABLE_SPI2
#define SD_SCK_PIN                          PB13
#define SD_MISO_PIN                         PB14
#define SD_MOSI_PIN                         PB15
#define SD_SS_PIN                           PA15
