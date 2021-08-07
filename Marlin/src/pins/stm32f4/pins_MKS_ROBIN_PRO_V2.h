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

#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin Nano V3 supports up to 1 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "MKS Robin PRO V2"

// Avoid conflict with TIMER_TONE
#define STEP_TIMER 10

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG

//
// Note: MKS Robin board is using SPI2 interface.
//
//#define SPI_MODULE 2

//
// Servos
//
#define SERVO0_PIN                          PA8   // Enable BLTOUCH

//
// Limit Switches
//
#define X_DIAG_PIN                          PA15
#define Y_DIAG_PIN                          PA12
#define Z_DIAG_PIN                          PA11
#define E0_DIAG_PIN                         PC4
#define E1_DIAG_PIN                         PE7

#define X_STOP_PIN                          PA15
#define Y_STOP_PIN                          PA12
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // MT_DET
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2
#ifndef X_CS_PIN
  #define X_CS_PIN                          PD5
#endif

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD7
#endif

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PD4
#endif

#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD9
#endif

#define E1_ENABLE_PIN                       PA3
#define E1_STEP_PIN                         PD15
#define E1_DIR_PIN                          PA1
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PD8
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PD14
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PD1
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PD0
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //

  #define X_SERIAL_TX_PIN                   PD5
  #define X_SERIAL_RX_PIN                   PD5

  #define Y_SERIAL_TX_PIN                   PD7
  #define Y_SERIAL_RX_PIN                   PD7

  #define Z_SERIAL_TX_PIN                   PD4
  #define Z_SERIAL_RX_PIN                   PD4

  #define E0_SERIAL_TX_PIN                  PD9
  #define E0_SERIAL_RX_PIN                  PD9

  #define E1_SERIAL_TX_PIN                  PD8
  #define E1_SERIAL_RX_PIN                  PD8

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif // HAS_TMC_UART

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PC2   // TH2
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   // HEATER1
#define HEATER_1_PIN                        PB0   // HEATER2
#define HEATER_BED_PIN                      PA0   // HOT BED

#define FAN_PIN                             PB1   // FAN

//
// Thermocouples
//
//#define TEMP_0_CS_PIN                     PE5   // TC1 - CS1
//#define TEMP_0_CS_PIN                     PE6   // TC2 - CS2

//
// Misc. Functions
//
//#define PS_ON_PIN                         PA3   // PW_OFF

//
// Power Supply Control
//
#if ENABLED(PSU_CONTROL)                          // MKSPWC
  //#define SUICIDE_PIN                     PB2   // LED
  //#define KILL_PIN                        PA2   // PW_DET
  //#define KILL_PIN_STATE                  HIGH
#else
  //#define POWER_LOSS_PIN                  PA2   // PW_DET
  //#define LED_PIN                         PB2
#endif

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION ONBOARD
#endif

//#define USE_NEW_SPI_API 1

//
// Onboard SD card
// NOT compatible with LCD
//
// detect pin doesn't work when ONBOARD and NO_SD_HOST_DRIVE disabled
#if !defined(SDCARD_CONNECTION) || SDCARD_CONNECTION == ONBOARD
  #if USE_NEW_SPI_API
    #define SD_SPI MARLIN_SPI(HardwareSPI3, PC9)
  #else
    #define ENABLE_SPI3
    #define SD_SS_PIN                       -1
    #define SDSS                            PC9
    #define SD_SCK_PIN                      PC10
    #define SD_MISO_PIN                     PC11
    #define SD_MOSI_PIN                     PC12
  #endif
  #define SD_DETECT_PIN                     PD12
#endif

/*
//
// LCD SD
//
#if SDCARD_CONNECTION == LCD
  #define ENABLE_SPI1
  #define SDSS                              PE10
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PE12
#endif
*/

//
// LCD / Controller
#define SPI_FLASH
#define HAS_SPI_FLASH 1
#define SPI_DEVICE                             2
#define SPI_FLASH_SIZE                 0x1000000
#if ENABLED(SPI_FLASH)
  #define W25QXX_CS_PIN                     PB12
  #define W25QXX_MOSI_PIN                   PB15
  #define W25QXX_MISO_PIN                   PB14
  #define W25QXX_SCK_PIN                    PB13
#endif

/**
 *                _____                                             _____
 *   (BEEPER)PC5 | · · | PE13(BTN_ENC)             (SPI1 MISO) PA6 | · · | PA5 (SPI1 SCK)
 *  (LCD_EN)PD13 | · · | PC6(LCD_RS)                 (BTN_EN1) PE8 | · · | PE10 (SPI1 CS)
 *  (LCD_D4)PE14 | · · | PE15(LCD_D5)               (BTN_EN2) PE11 | · · | PA7 (SPI1 MOSI)
 *  (LCD_D6)PD11 | · · | PD10(LCD_D7)               (SPI DET) PE12 | · · | RESET
 *           GND | · · | 5V                                    GND | · · | 3.3V
 *                ￣￣￣                                             ￣￣￣
 *                EXP1                                               EXP2
 */

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)
  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -17253
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y            11579
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   514
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   -24
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
  #endif

  #define TFT_CS_PIN                        PD11
  #define TFT_SCK_PIN                       PA5
  #define TFT_MISO_PIN                      PA6
  #define TFT_MOSI_PIN                      PA7
  #define TFT_DC_PIN                        PD10
  #define TFT_RST_PIN                       PC6
  #define TFT_A0_PIN                  TFT_DC_PIN

  #define TFT_RESET_PIN                     PC6
  #define TFT_BACKLIGHT_PIN                 PD13

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

  #define LCD_BACKLIGHT_PIN                 PD13
  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif

  #define TOUCH_CS_PIN                      PE14  // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define BTN_EN1                           PE8
  #define BTN_EN2                           PE11
  #define BEEPER_PIN                        PC5
  #define BTN_ENC                           PE13

  #define LCD_READ_ID                       0xD3
  #define LCD_USE_DMA_SPI

  //#define TFT_DRIVER                    ST7796
  #define TFT_BUFFER_SIZE                  14400

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                        PC5
  #define BTN_ENC                           PE13
  #define LCD_PINS_ENABLE                   PD13
  #define LCD_PINS_RS                       PC6
  #define BTN_EN1                           PE8
  #define BTN_EN2                           PE11
  #define LCD_BACKLIGHT_PIN                 -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)
    //#define LCD_BACKLIGHT_PIN             -1
    //#define LCD_RESET_PIN                 -1
    #define DOGLCD_A0                       PD11
    #define DOGLCD_CS                       PE15
    //#define DOGLCD_SCK                    PA5
    //#define DOGLCD_MOSI                   PA7

    // Required for MKS_MINI_12864 with this board
    //#define MKS_LCD12864B
    //#undef SHOW_BOOTSCREEN

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4                     PE14
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PE15
      #define LCD_PINS_D6                   PD11
      #define LCD_PINS_D7                   PD10
    #endif

    #ifndef ST7920_DELAY_1
    #define ST7920_DELAY_1          DELAY_NS(96)
    #endif
    #ifndef ST7920_DELAY_2
      #define ST7920_DELAY_2        DELAY_NS(48)
    #endif
    #ifndef ST7920_DELAY_3
      #define ST7920_DELAY_3       DELAY_NS(600)
    #endif

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD
