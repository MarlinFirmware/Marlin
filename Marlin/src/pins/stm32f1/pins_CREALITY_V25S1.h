/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Creality v2.4.S1 (STM32F103RE / STM32F103RC) v101 as found in the Ender 7 board pin assignments
 */

#define BOARD_INFO_NAME      "Creality v2.5.S1"
#define DEFAULT_MACHINE_NAME "Creality3D"

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  // FLASH
  //#define FLASH_EEPROM_EMULATION

  // I2C
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0 used only for display settings
  #if ENABLED(IIC_BL24CXX_EEPROM)
    #define IIC_EEPROM_SDA                  PA11
    #define IIC_EEPROM_SCL                  PA12
    #define MARLIN_EEPROM_SIZE 0x800              // 2Kb (24C16)
  #else
    #define SDCARD_EEPROM_EMULATION               // SD EEPROM until all EEPROM is BL24CXX
    #define MARLIN_EEPROM_SIZE 0x800              // 2Kb
  #endif

  // SPI
  //#define SPI_EEPROM                            // EEPROM on SPI-0
  //#define SPI_CHAN_EEPROM1  ?
  //#define SPI_EEPROM1_CS    ?

  // 2K EEPROM
  //#define SPI_EEPROM2_CS    ?

  // 32Mb FLASH
  //#define SPI_FLASH_CS      ?
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PC4
#define Y_STOP_PIN                          PC5

//
// Servos
//
#if ENABLED(BLTOUCH)
  #define Z_MIN_PROBE_PIN                     PC15   // BLTouch IN PIN
  #define SERVO0_PIN                          PC14   // BLTouch OUT PIN
  #define Z_MIN_PIN -1
#elif ENABLED(PROBE_ACTIVATION_SWITCH)
  #define Z_MIN_PIN                           PC15
  #define PROBE_TARE_PIN                      PC14
  #define PROBE_ACTIVATION_SWITCH_PIN         PB2
#else
  #define Z_MIN_PIN                           PC15
#endif

//
// Filament Runout Sensor
//
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #define FIL_RUNOUT_PIN                      PA15   // "Pulled-high"
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PC3
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB7

#define Y_ENABLE_PIN                        PC3
#define Y_STEP_PIN                          PB6
#define Y_DIR_PIN                           PB5

#define Z_ENABLE_PIN                        PC3
#define Z_STEP_PIN                          PB4
#define Z_DIR_PIN                           PB3

#define E0_ENABLE_PIN                       PC3
#define E0_STEP_PIN                         PC2
#define E0_DIR_PIN                          PB9

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PB1   // TH1
#define TEMP_BED_PIN                        PB0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB14  // HEATER1
#define HEATER_BED_PIN                      PB13  // HOT BED

#define FAN_PIN                             PB15  // FAN
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     PC13  // FAN
#endif
#define FAN_SOFT_PWM

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION                ONBOARD
#define ON_BOARD_SPI_DEVICE                    1
#define ONBOARD_SD_CS_PIN                   PC12   // SDSS
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                           // This board's SD is only seen by the printer

#define CASE_LIGHT_PIN     PA7

//
// Suicide Power
//
#define PS_ON_PIN       PA0
#define MOTOR_CIRCUIT_PIN   PA1

//
// Motor Protect
//
#define MOTOR_PROTECT_PIN   PC0

//
// WiFI Reset
//
#define RESET_WIFI_PIN      PB12
