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

#if NOT_TARGET(__STM32F1__)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME   "FLY_MINI"
#define BOARD_WEBSITE_URL "github.com/FLYmaker"
#define DISABLE_JTAG

//
// Flash EEPROM Emulation
//
#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     0x800 // 2KB
#define EEPROM_START_ADDRESS (0x8000000 + 256 * 1024 - 2 * EEPROM_PAGE_SIZE) // 256K firmware space
#define MARLIN_EEPROM_SIZE   EEPROM_PAGE_SIZE

//
// Servos
//
#define SERVO0_PIN                          PA8

//
// Limit Switches
//
#define X_MIN_PIN                           PC12
#define X_MAX_PIN                           PC11
#define Y_MIN_PIN                           PC10
#define Y_MAX_PIN                           PA15
#define Z_MIN_PIN                           PA14
#define Z_MAX_PIN                           PA13

//
// Steppers
//
#define X_STEP_PIN                          PB1
#define X_DIR_PIN                           PB2
#define X_ENABLE_PIN                        PB10
#ifndef X_CS_PIN
  #define X_CS_PIN                          PB0
#endif

#define Y_STEP_PIN                          PA2
#define Y_DIR_PIN                           PC4
#define Y_ENABLE_PIN                        PC5
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PA7
#endif

#define Z_STEP_PIN                          PA3
#define Z_DIR_PIN                           PA5
#define Z_ENABLE_PIN                        PA6
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PA4
#endif

#define E0_STEP_PIN                         PA1
#define E0_DIR_PIN                          PC3
#define E0_ENABLE_PIN                       PA0
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PC2
#endif

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
  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PB0
  #define X_SERIAL_RX_PIN                   PB0
  #define Y_SERIAL_TX_PIN                   PA7
  #define Y_SERIAL_RX_PIN                   PA7
  #define Z_SERIAL_TX_PIN                   PA4
  #define Z_SERIAL_RX_PIN                   PA4
  #define E0_SERIAL_TX_PIN                  PC2
  #define E0_SERIAL_RX_PIN                  PC2
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC6
#define HEATER_BED_PIN                      PC7
#ifndef FAN_PIN
  #define FAN_PIN                           PC8
#endif
#define FAN1_PIN                            PC9

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PC0   // Analog Input
#define TEMP_0_PIN                          PC1   // Analog Input

//
// LCD Pins
//

//
// LCD / Controller
//
#define SPI_DEVICE                             2
#define SS_PIN                              PB12
#define SCK_PIN                             PB13
#define MISO_PIN                            PB14
#define MOSI_PIN                            PB15

#define SDSS                              SS_PIN
#define SD_DETECT_PIN                       PB11

#define BEEPER_PIN                          PC14

#define LCD_PINS_RS                         PB8
#define LCD_PINS_ENABLE                     PB9
#define LCD_PINS_D4                         PB7
#define LCD_PINS_D5                         PB6
#define LCD_PINS_D6                         PB5
#define LCD_PINS_D7                         PB4

#define BTN_EN1                             PD2
#define BTN_EN2                             PB3
#define BTN_ENC                             PC13

#if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
  #define BTN_ENC_EN                 LCD_PINS_D7  // Detect the presence of the encoder
#endif

//
// Filament runout
//

//
// ST7920 Delays
//
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1            DELAY_NS(96)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2            DELAY_NS(48)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3           DELAY_NS(715)
#endif
