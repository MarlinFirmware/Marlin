/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#pragma once

#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "Anet ET4 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "Anet ET4"

//
// EEPROM
//

/**
 * Status: Working.
 * Hardware: AT24C04C (ATMLH744 04CM) 4 Kb => http://ww1.microchip.com/downloads/en/DeviceDoc/AT24C04C-AT24C08C-I2C-Compatible-%20Two-Wire-Serial-EEPROM-4-Kbit-8-Kbit-20006127A.pdf
 */

// Use one of these or SDCard-based Emulation will be used
#if NO_EEPROM_SELECTED
  //#define SRAM_EEPROM_EMULATION                 // Use BackSRAM-based EEPROM emulation
  #define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
  //#define IIC_BL24CXX_EEPROM                    // Use I2C EEPROM onboard IC (AT24C04C, Size 4KB, PageSize 16B)
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#elif ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PB11
  #define IIC_EEPROM_SCL                    PB10
  #define EEPROM_DEVICE_ADDRESS             0xA0
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PC13
#define Y_STOP_PIN                          PE12
#define Z_STOP_PIN                          PE11

//
// Z Probe
//
#if ENABLED(BLTOUCH)
  #define SERVO0_PIN                        PC3
#elif !defined(Z_MIN_PROBE_PIN)
  #define Z_MIN_PROBE_PIN                   PC3
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA2
#endif

//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PA8
#endif

//
// LED PIN
//
#define LED_PIN                             PD12

//
// Steppers
//
#define X_STEP_PIN                          PB6
#define X_DIR_PIN                           PB5
#define X_ENABLE_PIN                        PB7

#define Y_STEP_PIN                          PB3
#define Y_DIR_PIN                           PD6
#define Y_ENABLE_PIN                        PB4

#define Z_STEP_PIN                          PA12
#define Z_DIR_PIN                           PA11
#define Z_ENABLE_PIN                        PA15

#define E0_STEP_PIN                         PB9
#define E0_DIR_PIN                          PB8
#define E0_ENABLE_PIN                       PE0

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA1
#define TEMP_BED_PIN                        PA4

//
// Heaters
//
#define HEATER_0_PIN                        PA0
#define HEATER_BED_PIN                      PE2

//
// Fans
//
#define FAN_PIN                             PE3   // Layer fan
#define FAN1_PIN                            PE1   // Hotend fan

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN               FAN1_PIN
#endif

//
// LCD / Controller
//

/**
 * Status: Working. Merged FSMC/DMA implementation for stm32f4 from jmz52 fork.
 * Hardware: IC ST7789V | STP320240_0280E2T (40P/1,5): ST7789 (YT280S008)  => https://a.aliexpress.com/_dV4Bghv
 * Notes:
 *  - Defined PINS: CSX, DCX, WRX, RESX, RDX, DB[8:15]
 *  - FSMC/DMA and 8080-8 interface
 */

#ifndef TFT_DRIVER
  #define TFT_DRIVER                        AUTO
#endif
#define TFT_RESET_PIN                       PE6
#define TFT_CS_PIN                          PD7
#define TFT_RS_PIN                          PD13

//
// Touch Screen
//
/**
 * Status: Working. Merged implementation from jmz52 fork.
 * Hardware: TOUCH: XPT2046 => https://ldm-systems.ru/f/doc/catalog/HY-TFT-2,8/XPT2046.pdf
 */

#if ENABLED(TOUCH_SCREEN)
  #define TOUCH_CS_PIN                      PB2
  #define TOUCH_SCK_PIN                     PB0
  #define TOUCH_MOSI_PIN                    PE5
  #define TOUCH_MISO_PIN                    PE4
  #define TOUCH_INT_PIN                     PB1
#endif

//
// SD Card
//
//#define SDIO_SUPPORT

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION         CUSTOM_CABLE
#endif

#if ENABLED(SDSUPPORT)

  #define SDIO_D0_PIN                       PC8
  #define SDIO_D1_PIN                       PC9
  #define SDIO_D2_PIN                       PC10
  #define SDIO_D3_PIN                       PC11
  #define SDIO_CK_PIN                       PC12
  #define SDIO_CMD_PIN                      PD2

  #if DISABLED(SDIO_SUPPORT)
    #define SOFTWARE_SPI
    #define SDSS                     SDIO_D3_PIN
    #define SCK_PIN                  SDIO_CK_PIN
    #define MISO_PIN                 SDIO_D0_PIN
    #define MOSI_PIN                SDIO_CMD_PIN
  #endif

  #ifndef SD_DETECT_PIN
    #define SD_DETECT_PIN                   PD3
  #endif

#endif
