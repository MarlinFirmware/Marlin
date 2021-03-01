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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once

#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "Anet ET4 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Anet ET4 1.x"
#endif

//
// EEPROM
//

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
  #error "You will need to use 24V to 5V converter and remove one resistor and capacitor from the motherboard. See https://github.com/davidtgbe/Marlin/blob/bugfix-2.0.x/docs/Tutorials/bltouch-en.md for more information. Comment out this line to proceed at your own risk."
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
#define TFT_RESET_PIN                       PE6
#define TFT_CS_PIN                          PD7
#define TFT_RS_PIN                          PD13
#define TFT_INTERFACE_FSMC_8BIT

#define LCD_USE_DMA_FSMC                          // Use DMA transfers to send data to the TFT
#define FSMC_CS_PIN                   TFT_CS_PIN
#define FSMC_RS_PIN                   TFT_RS_PIN

//
// Touch Screen
// https://ldm-systems.ru/f/doc/catalog/HY-TFT-2,8/XPT2046.pdf
//
#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN                      PB2
  #define TOUCH_SCK_PIN                     PB0
  #define TOUCH_MOSI_PIN                    PE5
  #define TOUCH_MISO_PIN                    PE4
  #define TOUCH_INT_PIN                     PB1
#endif

#if ENABLED(ANET_ET5_TFT35)
  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X            17125
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y           -11307
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   -26
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   337
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION     TOUCH_PORTRAIT
  #endif
#elif ENABLED(ANET_ET4_TFT28)
  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -11838
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y             8776
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   333
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   -17
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION     TOUCH_PORTRAIT
  #endif
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
    #define SD_SCK_PIN               SDIO_CK_PIN
    #define SD_MISO_PIN              SDIO_D0_PIN
    #define SD_MOSI_PIN             SDIO_CMD_PIN
  #endif

  #ifndef SD_DETECT_PIN
    #define SD_DETECT_PIN                   PD3
  #endif

#endif
