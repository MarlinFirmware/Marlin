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
 * MKS Robin nano (STM32F130VET6) board pin assignments
 */

#if !defined(STM32F1) && !defined(STM32F1xx)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin nano supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS Robin Nano"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_JTAG

//
// EEPROM
//
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U) // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2KB
#endif

#define ENABLE_SPI2

//
// Limit Switches
//
#define X_STOP_PIN                          PA15
#define Y_STOP_PIN                          PA12
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4

#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3

#define E1_ENABLE_PIN                       PA3
#define E1_STEP_PIN                         PA6
#define E1_DIR_PIN                          PA1

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PC2   // TH2
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PC3
#endif
#if HOTENDS == 1
  #ifndef FAN1_PIN
    #define FAN1_PIN                        PB0
  #endif
#else
  #ifndef HEATER_1_PIN
    #define HEATER_1_PIN                    PB0
  #endif
#endif
#ifndef FAN_PIN
  #define FAN_PIN                           PB1   // FAN
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PA0
#endif

//
// Thermocouples
//
//#define MAX6675_SS_PIN                    PE5   // TC1 - CS1
//#define MAX6675_SS_PIN                    PE6   // TC2 - CS2

//
// Misc. Functions
//
#if HAS_TFT_LVGL_UI
  //#define MKSPWC
  #ifdef MKSPWC
    #define SUICIDE_PIN                     PB2   // Enable MKSPWC SUICIDE PIN
    #define SUICIDE_PIN_INVERTING          false  // Enable MKSPWC PIN STATE
    #define KILL_PIN                        PA2   // Enable MKSPWC DET PIN
    #define KILL_PIN_STATE                  true  // Enable MKSPWC PIN STATE
  #endif

  #define MT_DET_1_PIN                      PA4   // LVGL UI FILAMENT RUNOUT1 PIN
  #define MT_DET_2_PIN                      PE6   // LVGL UI FILAMENT RUNOUT2 PIN
  #define MT_DET_PIN_INVERTING             false  // LVGL UI filament RUNOUT PIN STATE

  #define WIFI_IO0_PIN                      PC13  // MKS ESP WIFI IO0 PIN
  #define WIFI_IO1_PIN                      PC7   // MKS ESP WIFI IO1 PIN
  #define WIFI_RESET_PIN                    PA5   // MKS ESP WIFI RESET PIN
#else
  //#define POWER_LOSS_PIN                  PA2   // PW_DET
  //#define PS_ON_PIN                       PB2   // PW_OFF
  #define FIL_RUNOUT_PIN                    PA4
  #define FIL_RUNOUT2_PIN                   PE6
#endif

#define SERVO0_PIN                          PA8   // Enable BLTOUCH support

//#define LED_PIN                           PB2

//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#define SDIO_SUPPORT
#define SDIO_CLOCK                       4500000  // 4.5 MHz
#define SD_DETECT_PIN                       PD12
#define ONBOARD_SD_CS_PIN                   PC11

//
// LCD / Controller
//
#define BEEPER_PIN                          PC5

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
 */

// Shared FSMC Configs
#if HAS_FSMC_TFT
  #define FSMC_CS_PIN                       PD7   // NE4
  #define FSMC_RS_PIN                       PD11  // A0

  #define TOUCH_CS_PIN                      PA7   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI

  #define LCD_RESET_PIN                     PC6   // FSMC_RST
  #define LCD_BACKLIGHT_PIN                 PD13

  #define TFT_RESET_PIN                     PC6   // FSMC_RST
  #define TFT_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          2
#endif

// LVGL Configs
#if ENABLED(TFT_LVGL_UI_FSMC)

  #ifndef XPT2046_X_CALIBRATION
    #define XPT2046_X_CALIBRATION          17880
  #endif
  #ifndef XPT2046_Y_CALIBRATION
    #define XPT2046_Y_CALIBRATION         -12234
  #endif
  #ifndef XPT2046_X_OFFSET
    #define XPT2046_X_OFFSET                 -45
  #endif
  #ifndef XPT2046_Y_OFFSET
   #define XPT2046_Y_OFFSET                  349
  #endif

// Emulated DOGM Configs
#elif ENABLED(FSMC_GRAPHICAL_TFT)

  #define DOGLCD_MOSI                       -1    // prevent redefine Conditionals_post.h
  #define DOGLCD_SCK                        -1

  #ifndef GRAPHICAL_TFT_UPSCALE
    #define GRAPHICAL_TFT_UPSCALE              3
  #endif
  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_PIXEL_OFFSET_X
    #define TFT_PIXEL_OFFSET_X                48
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif
  #ifndef TFT_PIXEL_OFFSET_Y
    #define TFT_PIXEL_OFFSET_Y                32
  #endif

  #ifndef XPT2046_X_CALIBRATION
    #define XPT2046_X_CALIBRATION          12149
  #endif
  #ifndef XPT2046_Y_CALIBRATION
    #define XPT2046_Y_CALIBRATION          -8746
  #endif
  #ifndef XPT2046_X_OFFSET
    #define XPT2046_X_OFFSET                 -35
  #endif
  #ifndef XPT2046_Y_OFFSET
    #define XPT2046_Y_OFFSET                 256
  #endif

#elif ENABLED(TFT_320x240)                        // TFT32/28
  #define TFT_RESET_PIN                     PC6
  #define TFT_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #ifndef XPT2046_X_CALIBRATION
    #define XPT2046_X_CALIBRATION         -12246
  #endif
  #ifndef XPT2046_Y_CALIBRATION
    #define XPT2046_Y_CALIBRATION           9453
  #endif
  #ifndef XPT2046_X_OFFSET
    #define XPT2046_X_OFFSET                 360
  #endif
  #ifndef XPT2046_Y_OFFSET
    #define XPT2046_Y_OFFSET                 -22
  #endif

  #define TOUCH_CS_PIN                      PA7   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI

  #define TFT_DRIVER                     ILI9341
  #define TFT_BUFFER_SIZE                  14400

  // YV for normal screen mounting
  //#define ILI9341_ORIENTATION  ILI9341_MADCTL_MY | ILI9341_MADCTL_MV
  // XV for 180° rotated screen mounting
  #define ILI9341_ORIENTATION  ILI9341_MADCTL_MX | ILI9341_MADCTL_MV

  #define ILI9341_COLOR_RGB

#elif ENABLED(TFT_480x320)
  #ifndef XPT2046_X_CALIBRATION
    #define XPT2046_X_CALIBRATION          17880
  #endif
  #ifndef XPT2046_Y_CALIBRATION
    #define XPT2046_Y_CALIBRATION         -12234
  #endif
  #ifndef XPT2046_X_OFFSET
    #define XPT2046_X_OFFSET                 -45
  #endif
  #ifndef XPT2046_Y_OFFSET
    #define XPT2046_Y_OFFSET                 349
  #endif

  #define TFT_DRIVER                     ILI9488
  #define TFT_BUFFER_SIZE                  14400
  #define ILI9488_ORIENTATION               ILI9488_MADCTL_MX | ILI9488_MADCTL_MV
#endif

#define HAS_SPI_FLASH                          1
#define SPI_FLASH_SIZE                 0x1000000  // 16MB
#if HAS_SPI_FLASH
  #define W25QXX_CS_PIN                     PB12
  #define W25QXX_MOSI_PIN                   PB15
  #define W25QXX_MISO_PIN                   PB14
  #define W25QXX_SCK_PIN                    PB13
#endif
