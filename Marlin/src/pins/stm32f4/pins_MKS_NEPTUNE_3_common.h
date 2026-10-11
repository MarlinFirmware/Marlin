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

//
// MKS Neptune 3 / ZNP Robin Nano_DW V2.2 common pins
//

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#if SHALL_USE_EEPROM(I2C_EEPROM)
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE             0x1000U  // 4KB
  #define I2C_SCL_PIN                       PB6
  #define I2C_SDA_PIN                       PB7
#endif

//
// Limit Switches
//
#define Y_DIAG_PIN                          PB8
#define Z_DIAG_PIN                          PC13

#define X_STOP_PIN                    X_DIAG_PIN
#define Y_STOP_PIN                    Y_DIAG_PIN
#define Z_STOP_PIN                    Z_DIAG_PIN

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PB4
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PD2
#define X_STEP_PIN                          PC12
#define X_DIR_PIN                           PB3

#define Y_ENABLE_PIN                        PC10
#define Y_STEP_PIN                          PC11
#define Y_DIR_PIN                           PA15

#define Z_ENABLE_PIN                        PC8
#define Z_STEP_PIN                          PC7
#define Z_DIR_PIN                           PC9

#define E0_ENABLE_PIN                       PC6
#define E0_STEP_PIN                         PB10
#define E0_DIR_PIN                          PB1

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters
//
#define HEATER_0_PIN                        PA6   // HEATER1
#define HEATER_BED_PIN                      PA5   // HOT BED

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define ENABLE_SPI3
  #define SD_SS_PIN                         PB12
  #define SD_SCK_PIN                        PB13
  #define SD_MISO_PIN                       PB14
  #define SD_MOSI_PIN                       PB15
  #define SD_SPI_SPEED            SPI_HALF_SPEED
  #if ENABLED(NO_SD_HOST_DRIVE)
    #define SD_DETECT_PIN                   PC3   // Detect pin doesn't work with SD Host Drive
  #endif
#endif
