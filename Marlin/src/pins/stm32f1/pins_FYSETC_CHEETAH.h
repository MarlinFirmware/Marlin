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

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define DEFAULT_MACHINE_NAME "3D Printer"

#define BOARD_INFO_NAME   "FYSETC Cheetah"
#define BOARD_WEBSITE_URL "fysetc.com"

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

#define DISABLE_JTAG

#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     (0x800U) // 2KB
#define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

//
// Servos
//
#define SERVO0_PIN                          PA0

//
// Limit Switches
//
#define X_STOP_PIN                          PA1
#define Y_STOP_PIN                          PB4
#define Z_STOP_PIN                          PA15

//
// Filament runout
//
#define FIL_RUNOUT_PIN                      PB5

//
// Steppers
//
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB9
#define X_ENABLE_PIN                        PA8

#define Y_STEP_PIN                          PB2
#define Y_DIR_PIN                           PB3
#define Y_ENABLE_PIN                        PB1

#define Z_STEP_PIN                          PC0
#define Z_DIR_PIN                           PC1
#define Z_ENABLE_PIN                        PC2

#define E0_STEP_PIN                         PC15
#define E0_DIR_PIN                          PC14
#define E0_ENABLE_PIN                       PC13

#define X_HARDWARE_SERIAL  MSerial2
#define Y_HARDWARE_SERIAL  MSerial2
#define Z_HARDWARE_SERIAL  MSerial2
#define E0_HARDWARE_SERIAL MSerial2

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC6
#define HEATER_BED_PIN                      PC7
#ifndef FAN_PIN
  #define FAN_PIN                           PC8
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PC5   // Analog Input
#define TEMP_0_PIN                          PC4   // Analog Input

//
// Misc. Functions
//
#define SDSS                                PA4

//
// LCD Pins
//
#if HAS_SPI_LCD

  #define BEEPER_PIN                        PC9

  #if HAS_GRAPHICAL_LCD
    #define DOGLCD_A0                       PB14
    #define DOGLCD_CS                       PB12
    #define DOGLCD_SCK                      PB13
    #define DOGLCD_MOSI                     PB15
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270

    #if EITHER(FYSETC_MINI_12864, U8GLIB_ST7920)
      #define FORCE_SOFT_SPI
    #endif
  #endif

  #define LCD_PINS_RS                       PB12  // CS -- SOFT SPI for ENDER3 LCD
  #define LCD_PINS_D4                       PB13  // SCLK
  #define LCD_PINS_ENABLE                   PB15  // DATA MOSI

  // not connected to a pin
  #define SD_DETECT_PIN                     PC3

  #ifndef RGB_LED_R_PIN
    #define RGB_LED_R_PIN                   PB0
  #endif
  #ifndef RGB_LED_G_PIN
    #define RGB_LED_G_PIN                   PB7
  #endif
  #ifndef RGB_LED_B_PIN
    #define RGB_LED_B_PIN                   PB6
  #endif

  //#define LCD_CONTRAST_INIT 190

  #if ENABLED(NEWPANEL)
    #define BTN_EN1                         PC11
    #define BTN_EN2                         PC10
    #define BTN_ENC                         PC12
  #endif

#endif
