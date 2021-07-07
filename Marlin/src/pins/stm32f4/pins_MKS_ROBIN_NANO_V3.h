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
  #error "MKS Robin Nano V3 supports up to 2 hotends / E-steppers."
#elif HAS_FSMC_TFT
  #error "MKS Robin Nano V3 doesn't support FSMC-based TFT displays."
#endif

#define BOARD_INFO_NAME "MKS Robin Nano V3"

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB
#define I2C_SCL_PIN                         PB6
#define I2C_SDA_PIN                         PB7

//
// Release PB4 (Z_DIR_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG

//
// Servos
//
#define SERVO0_PIN                          PA8   // Enable BLTOUCH

//
// Limit Switches
//
#define X_DIAG_PIN                          PD15
#define Y_DIAG_PIN                          PD2
#define Z_DIAG_PIN                          PC8
#define E0_DIAG_PIN                         PC4
#define E1_DIAG_PIN                         PE7

//
#define X_STOP_PIN                          PA15
#define Y_STOP_PIN                          PD2
#define Z_MIN_PIN                           PC8
#define Z_MAX_PIN                           PC4

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
// This board only supports SW SPI for stepper drivers
//
#if HAS_TMC_SPI
  #define TMC_USE_SW_SPI
#endif
#if ENABLED(TMC_USE_SW_SPI)
  #if !defined(TMC_SW_MOSI) || TMC_SW_MOSI == -1
    #define TMC_SW_MOSI                     PD14
  #endif
  #if !defined(TMC_SW_MISO) || TMC_SW_MISO == -1
    #define TMC_SW_MISO                     PD1
  #endif
  #if !defined(TMC_SW_SCK) || TMC_SW_SCK == -1
    #define TMC_SW_SCK                      PD0
  #endif
#endif

#if HAS_TMC_UART
  //
  // Software serial
  // No Hardware serial for steppers
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
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PA2   // TH2
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PE5   // HEATER1
#define HEATER_1_PIN                        PB0   // HEATER2
#define HEATER_BED_PIN                      PA0   // HOT BED

#define FAN_PIN                             PC14  // FAN
#define FAN1_PIN                            PB1   // FAN1

//
// Thermocouples
//
//#define TEMP_0_CS_PIN             HEATER_0_PIN  // TC1 - CS1
//#define TEMP_0_CS_PIN             HEATER_1_PIN  // TC2 - CS2

//
// Misc. Functions
//
#define MT_DET_1                            PA4
#define MT_DET_2                            PE6
#define PW_DET                              PA13
#define PW_OFF                              PB2

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                MT_DET_1
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN               MT_DET_2
#endif

#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                  PW_DET
#endif
#define PS_ON_PIN                         PW_OFF

//
// Enable MKSPWC support
//
//#define SUICIDE_PIN                       PB2
//#define KILL_PIN                          PA2
//#define KILL_PIN_INVERTING                true

//#define LED_PIN                           PB2

// Random Info
#define USB_SERIAL              -1  // USB Serial
#define WIFI_SERIAL              3  // USART3
#define MKS_WIFI_MODULE_SERIAL   1  // USART1
#define MKS_WIFI_MODULE_SPI      2  // SPI2

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

//
// Onboard SD card
//
// detect pin dont work when ONBOARD and NO_SD_HOST_DRIVE disabled
#if SD_CONNECTION_IS(ONBOARD)
  #define ENABLE_SPI3
  #define SD_SS_PIN                         -1
  #define SDSS                              PC9
  #define SD_SCK_PIN                        PC10
  #define SD_MISO_PIN                       PC11
  #define SD_MOSI_PIN                       PC12
  #define SD_DETECT_PIN                     PD12
#endif

//
// LCD SD
//
#if SD_CONNECTION_IS(LCD)
  #define ENABLE_SPI1
  #define SDSS                              PE10
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PE12
#endif

//
// LCD / Controller
#define SPI_FLASH
#define HAS_SPI_FLASH 1
#define SPI_DEVICE                             2
#define SPI_FLASH_SIZE                 0x1000000
#if ENABLED(SPI_FLASH)
  #define W25QXX_CS_PIN                     PB12
  #define W25QXX_MOSI_PIN                   PC3
  #define W25QXX_MISO_PIN                   PC2
  #define W25QXX_SCK_PIN                    PB13
#endif

/**
 *                _____                                             _____
 *   (BEEPER)PC5 | · · | PE13(BTN_ENC)             (SPI1 MISO) PA6 | · · | PA5 (SPI1 SCK)
 *  (LCD_EN)PD13 | · · | PC6(LCD_RS)                 (BTN_EN1) PE8 | · · | PE10 (SPI1 CS)
 *  (LCD_D4)PE14 | · ·   PE15(LCD_D5)               (BTN_EN2) PE11 | · ·   PA7 (SPI1 MOSI)
 *  (LCD_D6)PD11 | · · | PD10(LCD_D7)               (SPI1_RS) PE12 | · · | RESET
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

  #else // !MKS_MINI_12864

    #define LCD_PINS_D4                     PE14
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PE15
      #define LCD_PINS_D6                   PD11
      #define LCD_PINS_D7                   PD10
    #endif

    #define BOARD_ST7920_DELAY_1    DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2    DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3    DELAY_NS(600)

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD
