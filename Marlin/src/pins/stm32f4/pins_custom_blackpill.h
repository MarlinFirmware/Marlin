/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "custom blackpill"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "SLA Printer"
#endif

#define DISABLE_DEBUG false // DISABLE_(DEBUG|JTAG) is not supported for STM32F4.
#define ALLOW_STM32F4
#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  //#define SDCARD_EEPROM_EMULATION
#endif

//
// Servos
//
#if !defined(SERVO0_PIN) && !defined(HAS_PIN_27_BOARD)
  #define SERVO0_PIN                        PC13  // BLTouch OUT
#endif

//
// Limit Switches
//
#define Z_STOP_PIN                        PA15
#define X_STOP_PIN                        PA5
#define Y_STOP_PIN                        PA6

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                    PC15  // "Pulled-high"

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PA7   // HOT BED
#define FAN1_PIN                            PC_0   // extruder fan
#define HEATER_0_PIN                        PA1   // HEATER1

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC_5   // TH1
#define TEMP_BED_PIN                        PC4   // TB1


//
// SD Card
//
#define ONBOARD_SPI_DEVICE                  1
#define ONBOARD_SD_CS_PIN                   PA4   // SDSS

#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION ONBOARD
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                          // This board's SD is only seen by the printer



//
// Steppers
//

#define X_STEP_PIN                        PC_2
#define X_DIR_PIN                         PB9
#define X_ENABLE_PIN                      PC_3   // Shared

#define Y_STEP_PIN                        PB8
#define Y_DIR_PIN                         PB7
#define Y_ENABLE_PIN                      X_ENABLE_PIN

#define Z_STEP_PIN                        PB6
#define Z_DIR_PIN                         PB5
#define Z_ENABLE_PIN                      X_ENABLE_PIN

#define E0_STEP_PIN                       PB4
#define E0_DIR_PIN                        PB3
#define E0_ENABLE_PIN                     X_ENABLE_PIN


// Pins for documentation and sanity checks only.
// Changing these will not change the pin they are on.

// Hardware UART pins
#define UART1_TX_PIN                        PA9   // default uses CH340 RX
#define UART1_RX_PIN                        PA10  // default uses CH340 TX
#define UART2_TX_PIN                        PA2   // default uses HEATER_BED_PIN
#define UART2_RX_PIN                        PA3   // not connected
#define UART3_TX_PIN                        PB10  // default uses LCD connector
#define UART3_RX_PIN                        PB11  // default uses LCD connector
#define UART4_TX_PIN                        PC10  // default uses sdcard SDIO_D2
#define UART4_RX_PIN                        PC11  // default uses sdcard SDIO_D3
#define UART5_TX_PIN                        PC12  // default uses sdcard SDIO_CK
#define UART5_RX_PIN                        PD2   // default uses sdcard SDIO_CMD

// SDIO pins
#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_CK_PIN                         PC12
#define SDIO_CMD_PIN                        PD2