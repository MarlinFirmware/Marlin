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
 * MKS Robin pro (STM32F103ZET6) board pin assignments
 */

#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "MKS Robin pro supports up to 3 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS Robin pro"

#define BOARD_NO_NATIVE_USB

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

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
#define X_MIN_PIN                           PA15
#define X_MAX_PIN                           PG7
#define Y_MIN_PIN                           PA12
#define Y_MAX_PIN                           PG8
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2
#ifndef X_CS_PIN
  #define X_CS_PIN                          PF8
#endif

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PF3
#endif

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PF6
#endif

#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PG15
#endif

#define E1_ENABLE_PIN                       PA3
#define E1_STEP_PIN                         PA6
#define E1_DIR_PIN                          PA1
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PG10
#endif

#define E2_ENABLE_PIN                       PF0
#define E2_STEP_PIN                         PF2
#define E2_DIR_PIN                          PF1
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PG9
#endif
//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PB15
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PB14
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PB13
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  MSerial1
  //#define Y_HARDWARE_SERIAL  MSerial1
  //#define Z_HARDWARE_SERIAL  MSerial1
  //#define E0_HARDWARE_SERIAL MSerial1
  //#define E1_HARDWARE_SERIAL MSerial1
  //#define E2_HARDWARE_SERIAL MSerial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PF7
  #define X_SERIAL_RX_PIN                   PF8

  #define Y_SERIAL_TX_PIN                   PF4
  #define Y_SERIAL_RX_PIN                   PF3

  #define Z_SERIAL_TX_PIN                   PF5
  #define Z_SERIAL_RX_PIN                   PF6

  #define E0_SERIAL_TX_PIN                  PG13
  #define E0_SERIAL_RX_PIN                  PG15

  #define E1_SERIAL_TX_PIN                  PG12
  #define E1_SERIAL_RX_PIN                  PG10

  #define E2_SERIAL_TX_PIN                  PC13
  #define E2_SERIAL_RX_PIN                  PG9
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PC2   // TH2
#define TEMP_2_PIN                          PC3   // TH3
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PF10  // +HE0-
#define HEATER_1_PIN                        PB0   // +HE1-
#define HEATER_2_PIN                        PF9   // +HE2-
#define HEATER_BED_PIN                      PA0   // +HOT-BED-
#define FAN_PIN                             PB1   // +FAN-

/**
 * Note: MKS Robin Pro board is using SPI2 interface. Make sure your stm32duino library is configured accordingly
 */
//#define TEMP_0_CS_PIN                     PE5   // TC1 - CS1
//#define TEMP_0_CS_PIN                     PF11  // TC2 - CS2

#define POWER_LOSS_PIN                      PA2   // PW_DET
#define FIL_RUNOUT_PIN                      PA4   // MT_DET1
#define FIL_RUNOUT2_PIN                     PE6   // MT_DET2
#define FIL_RUNOUT3_PIN                     PG14  // MT_DET3

//
// Power Supply Control
//
#if ENABLED(PSU_CONTROL)                          // MKSPWC
  #if HAS_TFT_LVGL_UI
    #error "PSU_CONTROL cannot be used with TFT_LVGL_UI. Disable PSU_CONTROL to continue."
  #endif
  #ifndef PS_ON_PIN
    #define PS_ON_PIN                       PG11  // SUICIDE
  #endif
  #ifndef KILL_PIN
    #define KILL_PIN                        PA2
    #define KILL_PIN_STATE                  HIGH
  #endif
#else
  #define SUICIDE_PIN                       PG11
  #define SUICIDE_PIN_INVERTING            false
#endif

//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(LCD)
  #define SD_DETECT_PIN                     PG3
  #define SD_SCK_PIN                        PB13
  #define SD_MISO_PIN                       PB14
  #define SD_MOSI_PIN                       PB15
  #define SD_SS_PIN                         PG6
#elif SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT
  #define SD_DETECT_PIN                     PD12
  #define ONBOARD_SD_CS_PIN                 PC11
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

//
// TFT with FSMC interface
//
#if HAS_FSMC_TFT
  /**
   * Note: MKS Robin TFT screens use various TFT controllers.
   * If the screen stays white, disable 'LCD_RESET_PIN'
   * to let the bootloader init the screen.
   */
  #define TFT_RESET_PIN            LCD_RESET_PIN
  #define TFT_BACKLIGHT_PIN    LCD_BACKLIGHT_PIN

  #define FSMC_CS_PIN                       PD7   // NE4
  #define FSMC_RS_PIN                       PD11  // A0
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5
  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define TFT_CS_PIN                 FSMC_CS_PIN
  #define TFT_RS_PIN                 FSMC_RS_PIN

  #define LCD_RESET_PIN                     PF6
  #define LCD_BACKLIGHT_PIN                 PD13

  #define TFT_BUFFER_SIZE                  14400

  #if NEED_TOUCH_PINS
    #define TOUCH_BUTTONS_HW_SPI
    #define TOUCH_BUTTONS_HW_SPI_DEVICE        2
    #define TOUCH_CS_PIN                    PA7   // SPI2_NSS
    #define TOUCH_SCK_PIN                   PB13  // SPI2_SCK
    #define TOUCH_MISO_PIN                  PB14  // SPI2_MISO
    #define TOUCH_MOSI_PIN                  PB15  // SPI2_MOSI
  #else
    #define BEEPER_PIN                      PC5
    #define BTN_ENC                         PG2
    #define BTN_EN1                         PG5
    #define BTN_EN2                         PG4
  #endif

#elif IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS                      PG5
  #endif

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                        PC5
  #define BTN_ENC                           PG2
  #define LCD_PINS_ENABLE                   PG0
  #define LCD_PINS_RS                       PG1
  #define BTN_EN1                           PG5
  #define BTN_EN2                           PG4

  // MKS MINI12864 and MKS LCD12864B. If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                       PF12
    #define DOGLCD_CS                       PF15
    #define DOGLCD_SCK                      PB13
    #define DOGLCD_MOSI                     PB15

  #else                                           // !MKS_MINI_12864 && !ENDER2_STOCKDISPLAY

    #define LCD_PINS_D4                     PF14
    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   PF15
      #define LCD_PINS_D6                   PF12
      #define LCD_PINS_D7                   PF13

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif // !MKS_MINI_12864 && !ENDER2_STOCKDISPLAY

#endif

#ifndef BOARD_ST7920_DELAY_1
  #define BOARD_ST7920_DELAY_1              DELAY_NS(125)
#endif
#ifndef BOARD_ST7920_DELAY_2
  #define BOARD_ST7920_DELAY_2              DELAY_NS(125)
#endif
#ifndef BOARD_ST7920_DELAY_3
  #define BOARD_ST7920_DELAY_3              DELAY_NS(125)
#endif

#define HAS_SPI_FLASH                          1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE               0x1000000  // 16MB
  #define W25QXX_CS_PIN                     PB12  // Flash chip-select
  #define W25QXX_MOSI_PIN                   PB15
  #define W25QXX_MISO_PIN                   PB14
  #define W25QXX_SCK_PIN                    PB13
#endif
