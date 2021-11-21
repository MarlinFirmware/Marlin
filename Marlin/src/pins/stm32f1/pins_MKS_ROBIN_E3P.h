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
 * MKS Robin nano (STM32F103VET6) board pin assignments
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "MKS Robin E3P only supports one hotend / E-stepper. Comment out this line to continue."
#elif HAS_FSMC_TFT
  #error "MKS Robin E3P doesn't support FSMC-based TFT displays."
#endif

#define BOARD_INFO_NAME "MKS Robin E3P"

#define BOARD_NO_NATIVE_USB
#define MKS_HARDWARE_TEST_ONLY_E0

// Avoid conflict with TIMER_SERVO when using the STM32 HAL
#define TEMP_TIMER                             5

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// EEPROM
//
//#define FLASH_EEPROM_EMULATION
//#define SDCARD_EEPROM_EMULATION

#if EITHER(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM                              // EEPROM on I2C-0
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Note: MKS Robin board is using SPI2 interface.
//
#define SPI_DEVICE                             2

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
  //#define X_HARDWARE_SERIAL  Serial
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

  #define X_SERIAL_TX_PIN                   PD5
  #define X_SERIAL_RX_PIN                   PD5

  #define Y_SERIAL_TX_PIN                   PD7
  #define Y_SERIAL_RX_PIN                   PD7

  #define Z_SERIAL_TX_PIN                   PD4
  #define Z_SERIAL_RX_PIN                   PD4

  #define E0_SERIAL_TX_PIN                  PD9
  #define E0_SERIAL_RX_PIN                  PD9

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif // HAS_TMC_UART

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   // HEATER1
#define HEATER_BED_PIN                      PA0   // HOT BED

#define FAN_PIN                             PB1   // FAN

//
// Misc. Functions
//
#if HAS_TFT_LVGL_UI
  #if ENABLED(MKS_PWC)
    #define SUICIDE_PIN                     PB2
    #define SUICIDE_PIN_STATE               LOW
    #define KILL_PIN                        PA2
    #define KILL_PIN_STATE                  HIGH
  #endif

  #define MT_DET_1_PIN                      PA4
  #define MT_DET_PIN_STATE                  LOW

  #define WIFI_IO0_PIN                      PC13
  #define WIFI_IO1_PIN                      PC7
  #define WIFI_RESET_PIN                    PE9

  #if ENABLED(MKS_TEST)
    #define MKS_TEST_POWER_LOSS_PIN         PA2   // PW_DET
    #define MKS_TEST_PS_ON_PIN              PB0   // PW_OFF
  #endif
#else
  //#define POWER_LOSS_PIN                  PA2   // PW_DET
  //#define PS_ON_PIN                       PB2   // PW_OFF
  #define FIL_RUNOUT_PIN                    PA4
#endif

//#define LED_PIN                           PB2

//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT
  #define SDIO_CLOCK                     4500000  // 4.5 MHz
  #define SD_DETECT_PIN                     PD12
  #define ONBOARD_SD_CS_PIN                 PC11
#elif SD_CONNECTION_IS(LCD)
  #define ENABLE_SPI1
  #define SDSS                              PE10
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PE12
#endif

//
// LCD / Controller
//

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
 */

#if HAS_SPI_TFT

  // Shared SPI TFT

  #define LCD_BACKLIGHT_PIN                 PD13

  #define TOUCH_CS_PIN                      PE14  // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define BTN_EN1                           PE8
  #define BTN_EN2                           PE11
  #define BTN_ENC                           PE13

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

  #define LCD_USE_DMA_SPI

#endif

#if ENABLED(TFT_CLASSIC_UI)
  // Emulated DOGM SPI
  #define LCD_PINS_ENABLE                   PD13
  #define LCD_PINS_RS                       PC6
  #define BTN_ENC                           PE13
  #define BTN_EN1                           PE8
  #define BTN_EN2                           PE11
#elif ENABLED(TFT_COLOR_UI)
  #define TFT_BUFFER_SIZE                  14400
#endif

#if HAS_WIRED_LCD && !HAS_SPI_TFT
  #define BEEPER_PIN                        PC5
  #define BTN_ENC                           PE13
  #define LCD_PINS_ENABLE                   PD13
  #define LCD_PINS_RS                       PC6
  #define BTN_EN1                           PE8
  #define BTN_EN2                           PE11
  #define LCD_BACKLIGHT_PIN                 -1

  #if ENABLED(MKS_MINI_12864)

    // MKS MINI12864 and MKS LCD12864B
    // If using MKS LCD12864A (Need to remove RPK2 resistor)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                       PD11
    #define DOGLCD_CS                       PE15
    #define DOGLCD_SCK                      PA5
    #define DOGLCD_MOSI                     PA7

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)
      #define PIN_SPI_SCK                   PA5
      #define PIN_TFT_MISO                  PA6
      #define PIN_TFT_MOSI                  PA7
      #define TFTGLCD_CS                    PE8
    #endif

    #ifndef BEEPER_PIN
      #define BEEPER_PIN                    -1
    #endif

  #elif ENABLED(FYSETC_MINI_12864_2_1)
    #define DOGLCD_CS                       PD13
    #define DOGLCD_A0                       PC6
    #define LCD_PINS_DC                DOGLCD_A0
    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   PE14
    #define NEOPIXEL_PIN                    PE15
    #define DOGLCD_MOSI                     PA7
    #define DOGLCD_SCK                      PA5
    #if SD_CONNECTION_IS(ONBOARD)
      #define FORCE_SOFT_SPI
    #endif
    //#define LCD_SCREEN_ROT_180

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4                     PE14
    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   PE15
      #define LCD_PINS_D6                   PD11
      #define LCD_PINS_D7                   PD10

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

    #define BOARD_ST7920_DELAY_1             125
    #define BOARD_ST7920_DELAY_2             125
    #define BOARD_ST7920_DELAY_3             125

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD && !HAS_SPI_TFT

#define HAS_SPI_FLASH                          1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE               0x1000000  // 16MB
  #define SPI_FLASH_CS_PIN                  PB12
  #define SPI_FLASH_MOSI_PIN                PB15
  #define SPI_FLASH_MISO_PIN                PB14
  #define SPI_FLASH_SCK_PIN                 PB13
#endif

#ifndef BEEPER_PIN
  #define BEEPER_PIN                        PC5
#endif

#if ENABLED(SPEAKER) && BEEPER_PIN == PC5
  #error "MKS Robin nano default BEEPER_PIN is not a SPEAKER."
#endif
