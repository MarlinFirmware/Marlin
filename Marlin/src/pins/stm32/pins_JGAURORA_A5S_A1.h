/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  * 2017 Victor Perez Marlin for stm32f1 test
  * 2018 Modified by Pablo Crespo for Morpheus Board (https://github.com/pscrespo/Morpheus-STM32)
  * 2019 Modified by Roberto Mariani & Samuel Pinches for JGAurora A5S & A1 Board.
  */

/**
 * JGAurora A5S A1 Board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define BOARD_NAME "JGAurora A5S A1 board"

// #define STM32_XL_DENSITY // required, but should be set by platformio flags, not here! (why? not sure.)

//#define MCU_STM32F103ZE // not yet required

// #define I2C_EEPROM   // AT24C64
// #define E2END 0x7FFF // 64KB

// Enable EEPROM Emulation for this board

//#define FLASH_EEPROM_EMULATION 1
//#define E2END 0xFFF // 4KB
//#define E2END uint32(EEPROM_START_ADDRESS + (EEPROM_PAGE_SIZE * 2) - 1)
//#define EEPROM_CHITCHAT
//#define DEBUG_EEPROM_READWRITE

//
// Limit Switches
//
#define X_STOP_PIN         PC6
#define Y_STOP_PIN         PG8
#define Z_STOP_PIN         PG7

//
// Steppers
//
#define X_STEP_PIN         PD6
#define X_DIR_PIN          PD3
#define X_ENABLE_PIN       PG9

#define Y_STEP_PIN         PG12
#define Y_DIR_PIN          PG11
#define Y_ENABLE_PIN       PG13

#define Z_STEP_PIN         PG15
#define Z_DIR_PIN          PG14
#define Z_ENABLE_PIN       PB8

#define E0_STEP_PIN        PE2
#define E0_DIR_PIN         PB9
#define E0_ENABLE_PIN      PE3

#define E1_STEP_PIN        PE5
#define E1_DIR_PIN         PE4
#define E1_ENABLE_PIN      PE6

//
// Temperature Sensors
//
#define TEMP_0_PIN         PC2
#define TEMP_BED_PIN       PC1

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA2
#define HEATER_BED_PIN     PA3

#define FAN_PIN            PA1

#define PS_ON_PIN          PA0
#define FIL_RUNOUT_PIN     PC7

//
// LCD
//
#define LCD_BACKLIGHT_PIN  PF11
#define FSMC_CS_PIN        PD7
#define FSMC_RS_PIN        PG0

//
// SD Card
//
#define SD_DETECT_PIN      PF10

//
// Misc.
//
#define BEEPER_PIN         PC3    // use PB7 to shut up if desired
#define LED_PIN            PC13

//
// Touch support
//
#if ENABLED(TOUCH_BUTTONS)
  #define BTN_ENC          PA11   // Real pin needed to enable encoder's push button functionality used by touch screen. PA11 gives stable value.
  #define TOUCH_CS_PIN     PA4
  #define TOUCH_INT_PIN    PC4
#endif

#define NO_PAUSE_AFTER_PRINT
