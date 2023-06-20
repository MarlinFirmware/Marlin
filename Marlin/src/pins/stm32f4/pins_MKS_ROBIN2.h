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
/**
 * MKS Robin (STM32F407ZET6) board pin assignments
 * https://github.com/makerbase-mks/MKS-Robin2/tree/master/Hardware
 */

#define ALLOW_STM32DUINO
#include "env_validate.h"
#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS_ROBIN2 supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "MKS_ROBIN2"
// Avoid conflict with TIMER_TONE
#define STEP_TIMER 10

#define HAS_OTG_USB_HOST_SUPPORT                  // USB Flash Drive support

//#define USES_DIAG_JUMPERS
//
// Release PE4 (Y_ENABLE_PIN) from JTAG NRST role
//
//#define DISABLE_JTAG



//
// EEPROM
//



    #define I2C_EEPROM // Onboard I2C EEPROM
    //#define FLASH_EEPROM_EMULATION
    //#define SRAM_EEPROM_EMULATION



#if ENABLED(I2C_EEPROM)
  #define MARLIN_EEPROM_SIZE                0xFFFF    // 64K (AT24C512
  #define I2C_SCL_PIN                         PB6
  #define I2C_SDA_PIN                         PB7
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
    #define EEPROM_PAGE_SIZE     (0x8000U) // 2K
    #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
    #define MARLIN_EEPROM_SIZE (EEPROM_PAGE_SIZE)

#endif
#define SPI_DEVICE                             1



//
// Servos
//
//
#define SERVO0_PIN                          PA6   // Enable BLTOUCH
//#define SERVO1_PIN                          PF7   // XS1 - 6
//#define SERVO2_PIN                          PB0   // XS2 - 5
//#define SERVO3_PIN                          PA4   // XS2 - 6
//
// Limit Switches
//
#define X_MIN_PIN                           PG8
#define X_MAX_PIN                           PG7
#define Y_MIN_PIN                           PG6
#define Y_MAX_PIN                           PG5
#define Z_MIN_PIN                           PG4
#define Z_MAX_PIN                           PG3

//
// Steppers
//
#define X_STEP_PIN                          PE6
#define X_DIR_PIN                           PE5
#define X_ENABLE_PIN                        PC13

#define Y_STEP_PIN                          PE3
#define Y_DIR_PIN                           PE2
#define Y_ENABLE_PIN                        PE4

#define Z_STEP_PIN                          PE0
#define Z_DIR_PIN                           PB9
#define Z_ENABLE_PIN                        PE1

#define E0_STEP_PIN                         PG10
#define E0_DIR_PIN                          PG9
#define E0_ENABLE_PIN                       PB8

#define E1_STEP_PIN                         PD3
#define E1_DIR_PIN                          PA15
#define E1_ENABLE_PIN                       PD6

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // T1 <-> E0
#define TEMP_1_PIN                          PC2   // T2 <-> E1
#define TEMP_BED_PIN                        PC0   // T0 <-> Bed

//
// Heaters
//
#define HEATER_0_PIN                        PF3   // Heater0
#define HEATER_1_PIN                        PF2   // Heater1
#define HEATER_BED_PIN                      PF4   // Hotbed

//
//Fans
//
#define FAN0_PIN                             PA7   // Fan0

// Thermocouples
//
//#define TEMP_0_CS_PIN                     PF14   // TC1 - CS1
//#define TEMP_1_CS_PIN                     PF15   // TC2 - CS2

//
// Filament Runout Sensor
//
//
// Power loss detection
//
#define POWER_LOSS_PIN                      PA2   // PW_DET

//
// Power supply control
//
#define PS_ON_PIN                           PA3   // PW_OFF

//
// Piezzoelectric speaker
//
#define BEEPER_PIN                          PG2

//
// Activity LED
//
//#define LED_PIN                             PB2


#if HAS_TFT_LVGL_UI
  #define MT_DET_1_PIN                      PF11
  #define MT_DET_2_PIN                      PF13
  #define MT_DET_PIN_STATE                  LOW
//
// MKS Wi-Fi module
//
  #define WIFI_IO0_PIN                      PC6
  #define WIFI_IO1_PIN                      PB12
  #define WIFI_RESET_PIN                    PG1
#else
  //#define POWER_LOSS_PIN                  PA2   // PW_DET
  //#define PS_ON_PIN                       PB2   // PW_OFF
  #define FIL_RUNOUT_PIN                    PF11
  #define FIL_RUNOUT2_PIN                   PF13
#endif
/*
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PF11
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF13
#endif
*/



//
// TFT with FSMC interface
//
#if HAS_FSMC_TFT
/**
   * Note: MKS Robin TFT screens use various TFT controllers
   * Supported screens are based on the ILI9341, ST7789V and ILI9328 (320x240)
      * Define init sequences for other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
   *
   * If the screen stays white, disable 'TFT_RESET_PIN'
   * to let the bootloader init the screen.
   *
   * Setting an 'TFT_RESET_PIN' may cause a flicker when entering the LCD menu
   * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
   */
  #define TFT_RESET_PIN                   PD13 // FSMC_RST
  #define TFT_BACKLIGHT_PIN               PD12
  #define DOGLCD_MOSI                       -1    // Prevent auto-define by Conditionals_post.h
  #define DOGLCD_SCK                        -1

  #define LCD_USE_DMA_FSMC       // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                     PG12
  #define FSMC_RS_PIN                     PF12
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define TFT_CS_PIN                 FSMC_CS_PIN
  #define TFT_RS_PIN                 FSMC_RS_PIN


  #define TOUCH_CS_PIN                  PD11 // SPI1_NSS
  #define TOUCH_SCK_PIN                 PB3 // SPI1_SCK
  #define TOUCH_MISO_PIN                PB4 // SPI1_MISO
  #define TOUCH_MOSI_PIN                PB5 // SPI1_MOSI

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE       1
  #define TFT_BUFFER_SIZE                  14400
#endif





//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif
#define SDIO_SUPPORT
#if ENABLED(SDIO_SUPPORT)
  #define SD_DETECT_PIN                     PF9//SD_CD
  #define ONBOARD_SD_CS_PIN                   PC11
  //#define SDSS               -1//PB12
//#define SDIO_CLOCK                       4500000
//#define SDIO_READ_RETRIES                     16
#else

   // SD as custom software SPI (SDIO pins)
  #define SD_SCK_PIN                        PC12
  #define SD_MISO_PIN                       PC8
  #define SD_MOSI_PIN                       PD2
  #define SD_SS_PIN                         -1
  #define ONBOARD_SD_CS_PIN                 PC11

   #define SD_DETECT_PIN                     PF9//SD_CD
#endif



//
// W25Q128 128Mb (16MB) SPI flash
//
#define SPI_FLASH
#if ENABLED(SPI_FLASH)
  #define SPI_FLASH_SIZE                0x1000000  // 16MB
  #define SPI_FLASH_CS_PIN                  PG15
  #define SPI_FLASH_MOSI_PIN                PB5
  #define SPI_FLASH_MISO_PIN                PB4
  #define SPI_FLASH_SCK_PIN                 PB3
#endif

//
// Misc. Functions
//
