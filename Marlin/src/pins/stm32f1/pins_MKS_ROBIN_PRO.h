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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * MKS Robin pro (STM32F103ZET6) board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "MKS Robin pro supports up to 3 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS Robin pro"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// Note: MKS Robin board is using SPI2 interface.
//
//#define SPI_MODULE 2
#define ENABLE_SPI2

//
// Servos
//
#define SERVO0_PIN                          PA8   // BLTOUCH

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
//#define MAX6675_SS_PIN                    PE5   // TC1 - CS1
//#define MAX6675_SS_PIN                    PF11  // TC2 - CS2

#define POWER_LOSS_PIN                      PA2   // PW_DET
#define PS_ON_PIN                           PG11  // PW_OFF
#define FIL_RUNOUT_PIN                      PA4   // MT_DET1
//#define FIL_RUNOUT_PIN                    PE6   // MT_DET2
//#define FIL_RUNOUT_PIN                    PG14  // MT_DET3

//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(LCD)
  #define ENABLE_SPI2
  #define SD_DETECT_PIN                     PG3
  #define SCK_PIN                           PB13
  #define MISO_PIN                          PB14
  #define MOSI_PIN                          PB15
  #define SS_PIN                            PG6
#elif SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT
  #define SD_DETECT_PIN                     PD12
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
 */
#if ENABLED(FSMC_GRAPHICAL_TFT)
  #define FSMC_CS_PIN                       PD7   // NE4
  #define FSMC_RS_PIN                       PD11  // A0

  #define LCD_RESET_PIN                     PF6
  #define LCD_BACKLIGHT_PIN                 PD13

  #if ENABLED(TOUCH_BUTTONS)
    #define TOUCH_CS_PIN                    PA7
  #else
    #define BEEPER_PIN                      PC5
    #define BTN_ENC                         PG2
    #define BTN_EN1                         PG5
    #define BTN_EN2                         PG4
  #endif

#elif HAS_SPI_LCD

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
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PF15
      #define LCD_PINS_D6                   PF12
      #define LCD_PINS_D7                   PF13
    #endif

  #endif // !MKS_MINI_12864 && !ENDER2_STOCKDISPLAY
#endif

#ifndef BOARD_ST7920_DELAY_1
  #define BOARD_ST7920_DELAY_1     DELAY_NS(125)
#endif
#ifndef BOARD_ST7920_DELAY_2
  #define BOARD_ST7920_DELAY_2     DELAY_NS(125)
#endif
#ifndef BOARD_ST7920_DELAY_3
  #define BOARD_ST7920_DELAY_3     DELAY_NS(125)
#endif
