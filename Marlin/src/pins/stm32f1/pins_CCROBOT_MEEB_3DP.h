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

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "CCROBOT-ONLINE MEEB_3DP only supports 1 hotend / E-stepper. Comment out this line to continue."
#endif

// https://github.com/ccrobot-online/MEEB_3DP
// Pin assignments for 32-bit MEEB_3DP
#define BOARD_INFO_NAME "CCROBOT-ONLINE MEEB_3DP"
#define DEFAULT_MACHINE_NAME "STM32F103RCT6"
#define BOARD_WEBSITE_URL "ccrobot-online.com"

//
// Release PB4 from JTAG NRST role
//
#define DISABLE_JTAG

//
// EEPROM
//
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     0x800U             // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE   0x1000             // 4KB
#endif

//
// Servos
//
#define SERVO0_PIN                          PA1

//
// Limit Switches
//
#define X_STOP_PIN                          PC0
#define Y_STOP_PIN                          PC1
#define Z_STOP_PIN                          PC2

//
// Z Probe must be this pin
//
#define Z_MIN_PROBE_PIN                     PC15  // "PROBE"

//
// TMC2208 stepper drivers
//
#define X_ENABLE_PIN                        PB4
#define X_STEP_PIN                          PC12
#define X_DIR_PIN                           PC11

#define Y_ENABLE_PIN                        PC10
#define Y_STEP_PIN                          PB14
#define Y_DIR_PIN                           PB13

#define Z_ENABLE_PIN                        PB12
#define Z_STEP_PIN                          PB2
#define Z_DIR_PIN                           PB1

#define E0_ENABLE_PIN                       PB0
#define E0_STEP_PIN                         PA6
#define E0_DIR_PIN                          PA5

// Stepper drivers Serial UART
#define X_SERIAL_TX_PIN                     PB3
#define X_SERIAL_RX_PIN                     PD2
#define Y_SERIAL_TX_PIN                     PA15
#define Y_SERIAL_RX_PIN                     PC6
#define Z_SERIAL_TX_PIN                     PB11
#define Z_SERIAL_RX_PIN                     PB10
#define E0_SERIAL_TX_PIN                    PC5
#define E0_SERIAL_RX_PIN                    PC4

// Reduce baud rate to improve software serial reliability
#define TMC_BAUD_RATE 19200

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // TH0
#define TEMP_BED_PIN                        PC3   // THB

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC8   // HEATER0
#define HEATER_BED_PIN                      PC9   // HOT BED

#define FAN_PIN                             PA7   // FAN  (fan2 on board) model cool fan
#define FAN1_PIN                            PA8   // FAN  (fan0 on board) e0 cool fan
#define FAN2_PIN                            PB9   // FAN  (fan1 on board) controller cool fan

// One neopixel onboard and a connector for other neopixels
#define NEOPIXEL_PIN                        PC7   // The NEOPIXEL LED driving pin

/**
 *     1 _____ 2
 *  PB5 | · · | PB6
 *  PA2 | · · | RESET
 *  PA3 | · · | PB8
 *  PB7 | · · | PA4
 *  GND | · · | VCC5
 *     9 ----- 10
 *      LCD EXP
 */

//
// LCD / Controller
//
#if ENABLED(CR10_STOCKDISPLAY)
  #define BEEPER_PIN                        PB5
  #define BTN_EN1                           PA2
  #define BTN_EN2                           PA3
  #define BTN_ENC                           PB6

  #define LCD_PINS_RS                       PB7   // CS -- SOFT SPI for ENDER3 LCD
  #define LCD_PINS_D4                       PB8   // SCLK
  #define LCD_PINS_ENABLE                   PA4   // DATA MOSI
#endif

// Alter timing for graphical display
#if HAS_GRAPHICAL_LCD
  #define BOARD_ST7920_DELAY_1 DELAY_NS(125)
  #define BOARD_ST7920_DELAY_2 DELAY_NS(125)
  #define BOARD_ST7920_DELAY_3 DELAY_NS(125)
#endif

//
// Camera
//
#define CHDK_PIN                            PB15

#if 0

//
// SD-NAND
//
#if SD_CONNECTION_IS(ONBOARD)
  #define ENABLE_SPI1
  #define SD_DETECT_PIN                     -1
  #define SCK_PIN                           PA5
  #define MISO_PIN                          PA6
  #define MOSI_PIN                          PA7
  #define SS_PIN                            PA4
#endif

#define ON_BOARD_SPI_DEVICE 1                     // SPI1
#define ONBOARD_SD_CS_PIN                   PA4   // Chip select for SD-NAND

#endif
