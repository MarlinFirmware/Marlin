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

#if !defined(__STM32F1__) && !defined(__STM32F4__)
  #error "Oops! Select an STM32F1/4 board in 'Tools > Board.'"
#endif

/**
 * 2017 Victor Perez Marlin for stm32f1 test
 */

#define BOARD_INFO_NAME      "Chitu3D"
#define DEFAULT_MACHINE_NAME "STM32F103ZET6"

#define DISABLE_JTAG

//
// EEPROM
//

#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // SoC Flash (framework-arduinoststm32-maple/STM32F1/libraries/EEPROM/EEPROM.h)
  #define EEPROM_START_ADDRESS (0x8000000UL + (512 * 1024) - 2 * EEPROM_PAGE_SIZE)
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB, but will use 2x more (4KB)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#else
  #define MARLIN_EEPROM_SIZE 0x800U               // On SD, Limit to 2KB, require this amount of RAM
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PG10
#define Y_STOP_PIN                          PA12
#define Z_STOP_PIN                          PG9

//
// Steppers
//
#define X_ENABLE_PIN                        PC13
#define X_STEP_PIN                          PE5
#define X_DIR_PIN                           PE6

#define Y_ENABLE_PIN                        PE4
#define Y_STEP_PIN                          PE2
#define Y_DIR_PIN                           PE3

#define Z_ENABLE_PIN                        PE1
#define Z_STEP_PIN                          PB9
#define Z_DIR_PIN                           PE0

#define Z2_ENABLE_PIN                       PF3
#define Z2_STEP_PIN                         PF5
#define Z2_DIR_PIN                          PF1

#define E0_ENABLE_PIN                       PB8
#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB5

#define E1_ENABLE_PIN                       PG8
#define E1_STEP_PIN                         PC7
#define E1_DIR_PIN                          PC6

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA1   // TH1
#define TEMP_BED_PIN                        PA0   // TB1

//
// Heaters
//
#define HEATER_0_PIN                        PG12  // HEATER1
#define HEATER_BED_PIN                      PG11  // HOT BED
//#define HEATER_BED_INVERTING true

//
// Fans
//
#define CONTROLLER_FAN_PIN                  PD6   // BOARD FAN
#define FAN_PIN                             PG13  // FAN
#define FAN_PIN_2                           PG14

//
// Misc
//
#define BEEPER_PIN                          PB0
//#define LED_PIN                           PD3
//#define POWER_LOSS_PIN                    PG2   // PG4 PW_DET

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA15  // MT_DET
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF13
#endif

//
// TronXY TFT Support
//
#if ENABLED(FSMC_GRAPHICAL_TFT)
  #define FSMC_UPSCALE 3

  #define LCD_RESET_PIN                     PF11
  #define LCD_BACKLIGHT_PIN                 PD13
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #if NEED_TOUCH_PINS
    #define TOUCH_CS_PIN                    PB7   // SPI1_NSS
    #define TOUCH_SCK_PIN                   PA5   // SPI1_SCK
    #define TOUCH_MISO_PIN                  PA6   // SPI1_MISO
    #define TOUCH_MOSI_PIN                  PA7   // SPI1_MOSI

    #define BUTTON_DELAY_EDIT 50                  // (ms) Button repeat delay for edit screens
    #define BUTTON_DELAY_MENU 250                 // (ms) Button repeat delay for menus

    #ifndef XPT2046_X_CALIBRATION
      #define XPT2046_X_CALIBRATION       -12316
    #endif
    #ifndef XPT2046_Y_CALIBRATION
      #define XPT2046_Y_CALIBRATION         8981
    #endif
    #ifndef XPT2046_X_OFFSET
      #define XPT2046_X_OFFSET               340
    #endif
    #ifndef XPT2046_Y_OFFSET
      #define XPT2046_Y_OFFSET               -20
    #endif
  #endif

#elif ENABLED(TFT_480x320)
  #define TFT_RESET_PIN                     PF11
  #define TFT_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define XPT2046_X_CALIBRATION           -17181
  #define XPT2046_Y_CALIBRATION            11434
  #define XPT2046_X_OFFSET                   501
  #define XPT2046_Y_OFFSET                    -9

  #define TOUCH_CS_PIN                      PB7   // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define TFT_DRIVER                        ILI9488
  #define TFT_BUFFER_SIZE                   14400
#endif

// SPI Flash
#define SPI_FLASH_SIZE                      0x200000  // 2MB
#define HAS_SPI_FLASH                       1

// SPI 2
#define W25QXX_CS_PIN                       PB12
#define W25QXX_MOSI_PIN                     PB15
#define W25QXX_MISO_PIN                     PB14
#define W25QXX_SCK_PIN                      PB13

#if HAS_TFT_LVGL_UI
  #define HAS_SPI_FLASH_FONT                0
  #define HAS_GCODE_PREVIEW                 1
  #define HAS_GCODE_DEFAULT_VIEW_IN_FLASH   0
  #define HAS_LANG_SELECT_SCREEN            0
  #define HAS_BAK_VIEW_IN_FLASH             0
  #define HAS_LOGO_IN_FLASH                 0
  #define HAS_TOUCH_XPT2046                 1

  #define TOUCH_CS_PIN                      PB7   // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI
  //#define TOUCH_INT_PIN                   PB6

  #if ENABLED(TFT_LVGL_UI_SPI)
    #define SPI_TFT_CS_PIN          TOUCH_CS_PIN
    #define SPI_TFT_SCK_PIN        TOUCH_SCK_PIN
    #define SPI_TFT_MISO_PIN      TOUCH_MISO_PIN
    #define SPI_TFT_MOSI_PIN      TOUCH_MOSI_PIN
    #define SPI_TFT_DC_PIN                  PB6
    #define SPI_TFT_RST_PIN                 PF11
  #endif

  #define LCD_RESET_PIN                     PF11
  #define LCD_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define LCD_PIXEL_WIDTH       480
  #define LCD_PIXEL_HEIGHT      320
  #define LCD_FULL_PIXEL_WIDTH  LCD_PIXEL_WIDTH
  #define LCD_FULL_PIXEL_HEIGHT LCD_PIXEL_HEIGHT
  #define LCD_PIXEL_OFFSET_X     48
  #define LCD_PIXEL_OFFSET_Y     48

  #define XPT2046_X_CALIBRATION  -12316
  #define XPT2046_Y_CALIBRATION   8981
  #define XPT2046_X_OFFSET        340
  #define XPT2046_Y_OFFSET        -20

  #define USE_XPT2046       1
  #define XPT2046_XY_SWAP   0
  #define XPT2046_X_INV     1
  #define XPT2046_Y_INV     0

  #define XPT2046_HOR_RES   480
  #define XPT2046_VER_RES   320
  #define XPT2046_X_MIN     140
  #define XPT2046_Y_MIN     200
  #define XPT2046_X_MAX     1900
  #define XPT2046_Y_MAX     1900
  #define XPT2046_AVG       4
  #define XPT2046_INV       0
#endif

// SPI1(PA7)=LCD & SPI3(PB5)=STUFF, are not available
// so SPI2 is required.
#define ENABLE_SPI2
#define SCK_PIN                             PB13
#define MISO_PIN                            PB14
#define MOSI_PIN                            PB15
#define SS_PIN                              PB12

//
// SD Card
//
#define SDIO_SUPPORT
#define SD_DETECT_PIN                       -1    // PF0, but it isn't connected
#define SDIO_CLOCK 4500000
#define SDIO_READ_RETRIES 16
