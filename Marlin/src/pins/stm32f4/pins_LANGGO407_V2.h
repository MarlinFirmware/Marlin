/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * STM32F407VET6 with RAMPS-like shield
 * 'Black' STM32F407VET6 board - https://www.stm32duino.com/viewtopic.php?t=485
 * Shield - https://github.com/jmz52/Hardware
 */
#include "env_validate.h"

#if NOT_TARGET(STM32F4, STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "Black STM32F4VET6 supports up to 2 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "F4Z_MC"

  #define CHIP_NAME ""

  #if ENABLED(R3_PRO)
    #define ROBOT_NAME "-MAX"
  #elif ENABLED(R4_PRO)
    #define ROBOT_NAME "-PRO"
  #else
    #define ROBOT_NAME ""
  #endif

  #if ENABLED(USE_TI_CHOKE)
    #define MATERIALS_NAME "-Ti"
    #define MATERIALS_NAME_TI "-Ti"
  #else
    #define MATERIALS_NAME ""
    #define MATERIALS_NAME_TI "-Ti"
  #endif
#endif

#define DEFAULT_MACHINE_NAME "HP STM32F407VE"

//#define I2C_EEPROM
#define W25QXX_SPI_EEPROM

//
// Servos
//
#ifdef BLTOUCH
  #define SERVO0_PIN                        PA4   // After ver3.0
  //#define SERVO0_PIN                      PC6
  //#define SERVO1_PIN                      PC7
#else
  //#define CALIB_PIN                       PD6   // Before ver3.0
  #define CALIB_PIN                         PA4   // After ver3.0
#endif

//
// Z Probe must be this pin
//
//#define Z_MIN_PROBE_PIN                   PD3   // Before ver3.0
#define Z_MIN_PROBE_PIN                     PD6   // After ver3.0

//
// Limit Switches
//
#define X_STOP_PIN                          PF14
//#define X_STOP_PIN                        PA15
#define Y_STOP_PIN                          PF13
//#define Y_STOP_PIN                        PD12
#define Z_STOP_PIN                          PG0
//#define Z_STOP_PIN                        PD15
#define E0_STOP_PIN                         PD3   // after ver3.0

#define Z_MIN_PROBE_PIN                     PF11

//#define LED_RED_PIN                       PG9
#define RB_LED_PIN                          PD7   // Red-Blue LED

#if ENABLED(WS2812_LED)
  #define WS2812_LED_PIN                    PD7
#endif

#define CAP_TOUCH_PIN                       PA0

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2

#define Y_ENABLE_PIN                        PF0
#define Y_STEP_PIN                          PC13
#define Y_DIR_PIN                           PE6

#define Z_ENABLE_PIN                        PF4
#define Z_STEP_PIN                          PF3
#define Z_DIR_PIN                           PF2

#define Z2_ENABLE_PIN                       PC2
#define Z2_STEP_PIN                         PC1
#define Z2_DIR_PIN                          PC0

#define E0_ENABLE_PIN                       PE0
#define E0_STEP_PIN                         PB7
#define E0_DIR_PIN                          PB6

#define E1_ENABLE_PIN                       PC5
#define E1_STEP_PIN                         PC4
#define E1_DIR_PIN                          PA1

#if HAS_TMC_UART
  //
  // TMC220x stepper drivers
  //
  #define X_SERIAL_TX_PIN                   PE5
  #define Y_SERIAL_TX_PIN                   PF1
  #define Z_SERIAL_TX_PIN                   PF9   // 此处应是PF10，但表与PF9反了
  #define Z2_SERIAL_TX_PIN                  PC3
  #define E0_SERIAL_TX_PIN                  PE1
  #define E1_SERIAL_TX_PIN                  PB0

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif
#endif

//
// Filament Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PG10
#endif

//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PG8
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PF5   // T0
#define TEMP_1_PIN                          PB1   // T1
#define TEMP_BED_PIN                        PF6   // TB

#ifndef TEMP_CHAMBER_PIN
  //#define TEMP_CHAMBER_PIN                PC3   // TC
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PD11
#define HEATER_1_PIN                        PC7
#define HEATER_BED_PIN                      PD13

#define FAN0_PIN                            PB9
//#define FAN1_PIN                          PB8

#define STEP_TIMER_NUM                         5  // Timer Index for Stepper
#define TEMP_TIMER_NUM                        14  // Timer Index for Temperature

//
// USB connect control
//
#define USB_CONNECT_PIN                     PG11
#define USB_CONNECT_INVERTING              false

#define SD_DETECT_PIN                       PA8
#define BEEPER_PIN                          PB12
#define LED_PIN                             PB2

//#define NEOPIXEL_PIN                      PD7

/**
 * Note: Alfawise screens use various TFT controllers. Supported screens
 * are based on the ILI9341, ILI9328 and ST7798V. Define init sequences for
 * other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
 *
 * If the screen stays white, disable 'LCD_RESET_PIN' to let the bootloader
 * init the screen.
 *
 * Setting an 'LCD_RESET_PIN' may cause a flicker when entering the LCD menu
 * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
 */

#define LCD_BACKLIGHT_PIN                   PG1
#define FSMC_CS_PIN                         PG12  // FSMC_NE1
#define FSMC_RS_PIN                         PF12  // A23 Register. Only one address needed
#define TFT_CS_PIN                   FSMC_CS_PIN
#define TFT_RS_PIN                   FSMC_RS_PIN
#define TFT_BACKLIGHT_PIN      LCD_BACKLIGHT_PIN

#define LCD_USE_DMA_FSMC   // Use DMA transfers to send data to the TFT
#define FSMC_DMA_DEV                        DMA1
#define FSMC_DMA_CHANNEL   DMA_CH4  // Fixme in F4

#if ENABLED(TOUCH_SCREEN)
  #define TOUCH_CS_PIN                      PG0
  #define TOUCH_SCK_PIN                     PA5
  #define TOUCH_MOSI_PIN                    PA7
  #define TOUCH_MISO_PIN                    PA6
  #define TOUCH_INT_PIN                     PF15  // PenIRQ coming from XPT2046 触发
#endif

#define DOGLCD_MOSI                         -1    // Prevent auto-define by Conditionals_post.h
#define DOGLCD_SCK                          -1

//
// OCP
//
#define MOTORS_OCP_PIN                      PF7
#define BED_OCP_PIN                         PF8
#define HOTEND_OCP_PIN                      PF10  // 应该为PF9，但表PF9、PF10的值反了，只能用这个
#define SHUTDOWN_HOTEND_PIN                 PD12
#define SHUTDOWN_BED_PIN                    PG7

//
// Onboard SD support
//
#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_CK_PIN                         PC12
#define SDIO_CMD_PIN                        PD2

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  //#define ONBOARD_SDIO                          // Use SDIO for onboard SD
  #if DISABLED(ONBOARD_SDIO)
    #define SOFTWARE_SPI                          // Use soft SPI for onboard SD
    #define SD_SS_PIN                SDIO_D3_PIN  // A4
    #define SD_SCK_PIN               SDIO_CK_PIN  // A5
    #define SD_MISO_PIN              SDIO_D0_PIN  // A6
    #define SD_MOSI_PIN             SDIO_CMD_PIN  // A7
  #endif
#endif

//
// WIFI
//

/**
 *          -----
 *      TX | 1 2 | GND      Enable PG1   // Must be high for module to run
 *  Enable | 3 4 | GPIO2    Reset  PG0   // active low, probably OK to leave floating
 *   Reset | 5 6 | GPIO0    GPIO2  PF15  // must be high (ESP3D software configures this with a pullup so OK to leave as floating)
 *    3.3V | 7 8 | RX       GPIO0  PF14  // Leave as unused (ESP3D software configures this with a pullup so OK to leave as floating)
 *          -----
 *            W1
 */
#define ESP_WIFI_MODULE_COM                    6  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN           PG2
#define ESP_WIFI_MODULE_ENABLE_PIN          PG3
#define ESP_WIFI_MODULE_GPIO0_PIN           PG5
#define ESP_WIFI_MODULE_GPIO2_PIN           PG4

//#define SHUTTER_CTRL_PIN                  PB13
