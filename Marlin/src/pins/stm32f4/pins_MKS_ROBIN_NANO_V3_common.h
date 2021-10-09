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

//
// MKS Robin Nano V3, MKS Eagle pinmap
//

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#if EITHER(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
  #define I2C_SCL_PIN                       PB6
  #define I2C_SDA_PIN                       PB7
#endif

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
#define X_DIAG_PIN                          PA15
#define Y_DIAG_PIN                          PD2
#define Z_DIAG_PIN                          PC8
#define E0_DIAG_PIN                         PC4
#define E1_DIAG_PIN                         PE7

#define X_STOP_PIN                    X_DIAG_PIN
#define Y_STOP_PIN                    Y_DIAG_PIN
#define Z_MIN_PIN                     Z_DIAG_PIN
#define Z_MAX_PIN                    E0_DIAG_PIN

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
#define E1_STEP_PIN                         PD15
#define E1_DIR_PIN                          PA1

#if HAS_TMC_UART
  //
  // Software serial
  // No Hardware serial for steppers
  //
  #define X_SERIAL_TX_PIN                   PD5
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PD7
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PD4
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PD9
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #define E1_SERIAL_TX_PIN                  PD8
  #define E1_SERIAL_RX_PIN      E1_SERIAL_TX_PIN

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
#if HAS_TFT_LVGL_UI
  #define MT_DET_1_PIN                      PA4   // MT_DET
  #define MT_DET_2_PIN                      PE6
  #define MT_DET_PIN_STATE                  LOW
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN            MT_DET_1_PIN
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN           MT_DET_2_PIN
#endif

#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PA13  // PW_DET
#endif

//#define SUICIDE_PIN                       PB2
//#define LED_PIN                           PB2
//#define KILL_PIN                          PA2
//#define KILL_PIN_STATE                    LOW

//
// Power Supply Control
//
#if ENABLED(MKS_PWC)
  #if ENABLED(TFT_LVGL_UI)
    #undef PSU_CONTROL
    #undef MKS_PWC
    #define SUICIDE_PIN                     PB2
    #define SUICIDE_PIN_STATE               LOW
  #else
    #define PS_ON_PIN                       PB2   // PW_OFF
  #endif
  #define KILL_PIN                          PA13  // PW_DET
  #define KILL_PIN_STATE                    HIGH
#endif

// Random Info
#define USB_SERIAL              -1  // USB Serial
#define WIFI_SERIAL              3  // USART3
#define MKS_WIFI_MODULE_SERIAL   1  // USART1
#define MKS_WIFI_MODULE_SPI      2  // SPI2

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

// MKS WIFI MODULE
#if ENABLED(MKS_WIFI_MODULE)
  #define WIFI_IO0_PIN                      PC13
  #define WIFI_IO1_PIN                      PC7
  #define WIFI_RESET_PIN                    PE9
#endif

// MKS TEST
#if ENABLED(MKS_TEST)
  #define MKS_TEST_POWER_LOSS_PIN           PA13  // PW_DET
  #define MKS_TEST_PS_ON_PIN                PB2   // PW_OFF
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
  #define SD_DETECT_PIN                     PD12
#endif

#define SPI_FLASH
#if ENABLED(SPI_FLASH)
  #define HAS_SPI_FLASH                        1
  #define SPI_DEVICE                           2
  #define SPI_FLASH_SIZE               0x1000000
  #define SPI_FLASH_CS_PIN                  PB12
  #define SPI_FLASH_MOSI_PIN                PC3
  #define SPI_FLASH_MISO_PIN                PC2
  #define SPI_FLASH_SCK_PIN                 PB13
#endif

/**
 *                ------                                     ------
 * (BEEPER)  PC5 |10  9 | PE13 (BTN_ENC)   (SPI1 MISO)  PA6 |10  9 | PA5  (SPI1 SCK)
 * (LCD_EN) PD13 | 8  7 | PC6  (LCD_RS)      (BTN_EN1)  PE8 | 8  7 | PE10 (SPI1 CS)
 * (LCD_D4) PE14   6  5 | PE15 (LCD_D5)      (BTN_EN2) PE11   6  5 | PA7  (SPI1 MOSI)
 * (LCD_D6) PD11 | 4  3 | PD10 (LCD_D7)      (SPI1_RS) PE12 | 4  3 | RESET
 *           GND | 2  1 | 5V                            GND | 2  1 | 3.3V
 *                ------                                     ------
 *                 EXP1                                       EXP2
 */
#define EXP1_03_PIN                         PD10
#define EXP1_04_PIN                         PD11
#define EXP1_05_PIN                         PE15
#define EXP1_06_PIN                         PE14
#define EXP1_07_PIN                         PC6
#define EXP1_08_PIN                         PD13
#define EXP1_09_PIN                         PE13
#define EXP1_10_PIN                         PC5

#define EXP2_03_PIN                         -1    // RESET
#define EXP2_04_PIN                         PE12
#define EXP2_05_PIN                         PA7
#define EXP2_06_PIN                         PE11
#define EXP2_07_PIN                         PE10
#define EXP2_08_PIN                         PE8
#define EXP2_09_PIN                         PA5
#define EXP2_10_PIN                         PA6

//
// SPI SD Card
//
#if SD_CONNECTION_IS(LCD)
  #define ENABLE_SPI1
  #define SDSS                       EXP2_07_PIN
  #define SD_SCK_PIN                 EXP2_09_PIN
  #define SD_MISO_PIN                EXP2_10_PIN
  #define SD_MOSI_PIN                EXP2_05_PIN
  #define SD_DETECT_PIN              EXP2_04_PIN
#endif

//
// LCD / Controller
//
#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)
  #define TFT_CS_PIN                 EXP1_04_PIN
  #define TFT_SCK_PIN                EXP2_09_PIN
  #define TFT_MISO_PIN               EXP2_10_PIN
  #define TFT_MOSI_PIN               EXP2_05_PIN
  #define TFT_DC_PIN                 EXP1_03_PIN
  #define TFT_RST_PIN                EXP1_07_PIN
  #define TFT_A0_PIN                  TFT_DC_PIN

  #define TFT_RESET_PIN              EXP1_07_PIN
  #define TFT_BACKLIGHT_PIN          EXP1_08_PIN

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

  #define LCD_BACKLIGHT_PIN          EXP1_08_PIN
  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif

  #define TOUCH_CS_PIN               EXP1_06_PIN  // SPI1_NSS
  #define TOUCH_SCK_PIN              EXP2_09_PIN  // SPI1_SCK
  #define TOUCH_MISO_PIN             EXP2_10_PIN  // SPI1_MISO
  #define TOUCH_MOSI_PIN             EXP2_05_PIN  // SPI1_MOSI

  #define LCD_READ_ID                       0xD3
  #define LCD_USE_DMA_SPI

  #define TFT_BUFFER_SIZE                  14400

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

#elif HAS_WIRED_LCD

  #define LCD_PINS_ENABLE            EXP1_08_PIN
  #define LCD_PINS_RS                EXP1_07_PIN
  #define LCD_BACKLIGHT_PIN                 -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)
    //#define LCD_BACKLIGHT_PIN             -1
    //#define LCD_RESET_PIN                 -1
    #define DOGLCD_A0                EXP1_04_PIN
    #define DOGLCD_CS                EXP1_05_PIN
    //#define DOGLCD_SCK             EXP2_09_PIN
    //#define DOGLCD_MOSI            EXP2_05_PIN

    // Required for MKS_MINI_12864 with this board
    //#define MKS_LCD12864B
    //#undef SHOW_BOOTSCREEN

  #elif ENABLED(MKS_MINI_12864_V3)
    #define DOGLCD_CS                EXP1_08_PIN
    #define DOGLCD_A0                EXP1_07_PIN
    #define LCD_PINS_DC                DOGLCD_A0
    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN            EXP1_06_PIN
    #define NEOPIXEL_PIN             EXP1_05_PIN
    #define DOGLCD_MOSI              EXP2_05_PIN
    #define DOGLCD_SCK               EXP2_09_PIN
    #if SD_CONNECTION_IS(ONBOARD)
      #define FORCE_SOFT_SPI
    #endif
    //#define LCD_SCREEN_ROT_180

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4              EXP1_06_PIN
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5            EXP1_05_PIN
      #define LCD_PINS_D6            EXP1_04_PIN
      #define LCD_PINS_D7            EXP1_03_PIN
    #endif

    #define BOARD_ST7920_DELAY_1              96
    #define BOARD_ST7920_DELAY_2              48
    #define BOARD_ST7920_DELAY_3             600

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI, HAS_WIRED_LCD)
  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN
  #define BTN_ENC                    EXP1_09_PIN
#endif
