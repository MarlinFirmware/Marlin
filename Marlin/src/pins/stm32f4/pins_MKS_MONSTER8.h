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

#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 5
  #error "MKS Monster supports up to 3 hotends and 5 E-steppers."
#elif HAS_FSMC_TFT
  #error "MKS Monster doesn't support FSMC-based TFT displays."
#endif

#define BOARD_INFO_NAME "MKS Monster8 V1.x"

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

//#define DISABLE_DEBUG

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#define I2C_EEPROM                                // Need use jumpers set i2c for EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB
#define I2C_SCL_PIN                         PB8   // I2C_SCL and CAN_RX
#define I2C_SDA_PIN                         PB9   // I2C_SDA and CAN_TX

//
// Servos
//
#define SERVO0_PIN                          PA8   // Enable BLTOUCH

//
// Limit Switches for diag signal
//
#define X_DIAG_PIN                          PA14  // Driver0 diag signal is connect to X-
#define Y_DIAG_PIN                          PA15  // Driver1 diag signal is connect to Y-
#define Z_DIAG_PIN                          PB13  // Driver2 diag signal is connect to Z-
#define E0_DIAG_PIN                         PA13  // Driver3 diag signal is connect to X+
#define E1_DIAG_PIN                         PC5   // Driver4 diag signal is connect to Y+
#define E2_DIAG_PIN                         PB12  // Driver5 diag signal is connect to Z+
#define E3_DIAG_PIN                         -1    // Driver6 diag signal is not connect
#define E4_DIAG_PIN                         -1    // Driver7 diag signal is not connect

// Limit Switches for endstop
#define X_MIN_PIN                           PA14
#define X_MAX_PIN                           PA13
#define Y_MIN_PIN                           PA15
#define Y_MAX_PIN                           PC5
#define Z_MIN_PIN                           PB13
#define Z_MAX_PIN                           PB12

//
// Steppers
// Driver 0  1  2  3  4  5  6  7
// For    X  Y  Z  E0 E1 E2 E3 E4(default pin settings)
//
//Driver0
#define X_ENABLE_PIN                        PC15
#define X_STEP_PIN                          PC14
#define X_DIR_PIN                           PC13
#ifndef X_CS_PIN
  #define X_CS_PIN                          PE6
#endif
//Driver1
#define Y_ENABLE_PIN                        PC15
#define Y_STEP_PIN                          PE5
#define Y_DIR_PIN                           PE4
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE3
#endif
//Driver2
#define Z_ENABLE_PIN                        PE2
#define Z_STEP_PIN                          PE1
#define Z_DIR_PIN                           PE0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PB7
#endif
//Driver3
#define E0_ENABLE_PIN                       PB6
#define E0_STEP_PIN                         PB5
#define E0_DIR_PIN                          PB4
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PB3
#endif
//Driver4
#define E1_ENABLE_PIN                       PD7
#define E1_STEP_PIN                         PD6
#define E1_DIR_PIN                          PD5
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PD4
#endif
//Driver5
#define E2_ENABLE_PIN                       PD3
#define E2_STEP_PIN                         PD2
#define E2_DIR_PIN                          PD1
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PD0
#endif
//Driver6
#define E3_ENABLE_PIN                       PC8
#define E3_STEP_PIN                         PC7
#define E3_DIR_PIN                          PC6
#ifndef E3_CS_PIN
  #define E3_CS_PIN                         PD15
#endif
//Driver7
#define E4_ENABLE_PIN                       PD14
#define E4_STEP_PIN                         PD13
#define E4_DIR_PIN                          PD12
#ifndef E4_CS_PIN
  #define E4_CS_PIN                         PD11
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
    #define TMC_SW_MOSI                     PE14
  #endif
  #if !defined(TMC_SW_MISO) || TMC_SW_MISO == -1
    #define TMC_SW_MISO                     PE13
  #endif
  #if !defined(TMC_SW_SCK) || TMC_SW_SCK == -1
    #define TMC_SW_SCK                      PE12
  #endif
#endif

#if HAS_TMC_UART
  //
  // Software serial
  // No Hardware serial for steppers
  //
  #define X_SERIAL_TX_PIN                   PE6
  #define X_SERIAL_RX_PIN                   PE6

  #define Y_SERIAL_TX_PIN                   PE3
  #define Y_SERIAL_RX_PIN                   PE3

  #define Z_SERIAL_TX_PIN                   PB7
  #define Z_SERIAL_RX_PIN                   PB7

  #define E0_SERIAL_TX_PIN                  PB3
  #define E0_SERIAL_RX_PIN                  PB3

  #define E1_SERIAL_TX_PIN                  PD4
  #define E1_SERIAL_RX_PIN                  PD4

  #define E2_SERIAL_TX_PIN                  PD0
  #define E2_SERIAL_RX_PIN                  PD0

  #define E3_SERIAL_TX_PIN                  PD15
  #define E3_SERIAL_RX_PIN                  PD15

  #define E4_SERIAL_TX_PIN                  PD11
  #define E4_SERIAL_RX_PIN                  PD11

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH0
#define TEMP_1_PIN                          PC2   // TH1
#define TEMP_2_PIN                          PC3   // TH2
#define TEMP_BED_PIN                        PC0   // TB

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // HE0
#define HEATER_1_PIN                        PB0   // HE1
#define HEATER_2_PIN                        PA3   // HE2
#define HEATER_BED_PIN                      PB10  // H-BED

#define FAN_PIN                             PA2   // FAN0
#define FAN1_PIN                            PA1   // FAN1
#define FAN2_PIN                            PA0   // FAN2

//
// Misc. Functions
//
#define MT_DET_1                       Y_MAX_PIN
#define MT_DET_2                       Z_MAX_PIN
#define PW_DET                         Y_MAX_PIN
#define PW_OFF                         Z_MAX_PIN

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                MT_DET_1
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN               MT_DET_2
#endif

#define POWER_LOSS_PIN                    PW_DET
#define PS_ON_PIN                         PW_OFF

// Random Info
#define USB_SERIAL                          -1    // USB Serial

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

//
// Onboard SD card
//
// detect pin doesn't work when ONBOARD and NO_SD_HOST_DRIVE disabled
#if SD_CONNECTION_IS(ONBOARD)
  #define ENABLE_SPI3
  #define SD_SS_PIN                         -1
  #define SDSS                              PC9
  #define SD_SCK_PIN                        PC10
  #define SD_MISO_PIN                       PC11
  #define SD_MOSI_PIN                       PC12
  #define SD_DETECT_PIN                     PC4
//
// LCD SD
//
#elif SD_CONNECTION_IS(LCD)
  #define ENABLE_SPI1
  #define SDSS                              PA4
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PB11
#endif

/**
 *                _____                                             _____
 *   (BEEPER)PB2 | · · | PE10(BTN_ENC)             (SPI1 MISO) PA6 | · · | PA5 (SPI1 SCK)
 *  (LCD_EN)PE11 | · · | PD10(LCD_RS)                (BTN_EN1) PE9 | · · | PA4 (SPI1 CS)
 *  (LCD_D4)PD9  | · ·   PD8(LCD_D5)                 (BTN_EN2) PE8 | · ·   PA7 (SPI1 MOSI)
 *  (LCD_D6)PE15 | · · | PE7(LCD_D7)                (SPI1_RS) PB11 | · · | RESET
 *           GND | · · | 5V                                    GND | · · | 3.3V
 *                ￣￣￣                                             ￣￣￣
 *                EXP1                                               EXP2
 */

#if ANY(TFT_COLOR_UI, TFT_CLASSIC_UI)
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

  #define TFT_CS_PIN                        PE15
  #define TFT_SCK_PIN                       PA5
  #define TFT_MISO_PIN                      PA6
  #define TFT_MOSI_PIN                      PA7
  #define TFT_DC_PIN                        PE7
  #define TFT_RST_PIN                       PD10
  #define TFT_A0_PIN                  TFT_DC_PIN

  #define TFT_RESET_PIN                     PD10
  #define TFT_BACKLIGHT_PIN                 PE11

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

  #define LCD_BACKLIGHT_PIN                 PE11
  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif

  #define TOUCH_CS_PIN                      PD9   // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define BTN_EN1                           PE9
  #define BTN_EN2                           PE8
  #define BEEPER_PIN                        PB2
  #define BTN_ENC                           PE10

  #define LCD_READ_ID                       0xD3
  #define LCD_USE_DMA_SPI

  #define TFT_BUFFER_SIZE                  14400

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                        PB2
  #define BTN_ENC                           PE10
  #define LCD_PINS_ENABLE                   PE11
  #define LCD_PINS_RS                       PD10
  #define BTN_EN1                           PE9
  #define BTN_EN2                           PE8
  #define LCD_BACKLIGHT_PIN                 -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)
    //#define LCD_BACKLIGHT_PIN             -1
    //#define LCD_RESET_PIN                 -1
    #define DOGLCD_A0                       PD11
    #define DOGLCD_CS                       PE15
    //#define DOGLCD_SCK                    PA5
    //#define DOGLCD_MOSI                   PA7

  #elif ENABLED(MKS_MINI_12864_V3)
    #define DOGLCD_CS                       PE11
    #define DOGLCD_A0                       PD10
    #define LCD_PINS_DC                DOGLCD_A0
    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   PD9
    #define NEOPIXEL_PIN                    PD8
    #define DOGLCD_MOSI                     PA7
    #define DOGLCD_SCK                      PA5
    #if SD_CONNECTION_IS(ONBOARD)
      #define FORCE_SOFT_SPI
    #endif
    //#define LCD_SCREEN_ROT_180

  #else

    #define LCD_PINS_D4                     PD9
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PD8
      #define LCD_PINS_D6                   PE15
      #define LCD_PINS_D7                   PE7
    #endif

    #define BOARD_ST7920_DELAY_1    DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2    DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3    DELAY_NS(600)

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD
