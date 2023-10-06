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

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT SKR Mini E3 V2.0"
#endif

#define BOARD_CUSTOM_BUILD_FLAGS -DTONE_CHANNEL=4 -DTONE_TIMER=4 -DTIMER_TONE=4

// Onboard I2C EEPROM
#if NO_EEPROM_SELECTED
  #define I2C_EEPROM
  #define SOFT_I2C_EEPROM
  #define MARLIN_EEPROM_SIZE              0x1000  // 4K
  #define I2C_SDA_PIN                       PB7
  #define I2C_SCL_PIN                       PB6
  #undef NO_EEPROM_SELECTED
#endif

#define FAN_PIN                             PC6

//
// USB connect control
//
#define USB_CONNECT_PIN                     PA14

/**
 *            SKR Mini E3 V2.0
 *                 ------
 * (BEEPER)  PB5  | 1  2 | PA15 (BTN_ENC)
 * (BTN_EN1) PA9  | 3  4 | RESET
 * (BTN_EN2) PA10   5  6 | PB9  (LCD_D4)
 * (LCD_RS)  PB8  | 7  8 | PB15 (LCD_EN)
 *            GND | 9 10 | 5V
 *                 ------
 *                  EXP1
 */
#define EXP1_02_PIN                         PA15
#define EXP1_08_PIN                         PB15

#include "pins_BTT_SKR_MINI_E3_common.h"

// Release PA13/PA14 (led, usb control) from SWD pins
#define DISABLE_DEBUG

#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PA8   // LED driving pin
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PC13  // Power Supply Control
#endif

#define FAN1_PIN                            PC7

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN            FAN1_PIN
#endif

#if HAS_TMC_UART
  /**
   * TMC220x stepper drivers
   * Hardware serial communication ports
   */
  #define X_HARDWARE_SERIAL  MSerial4
  #define Y_HARDWARE_SERIAL  MSerial4
  #define Z_HARDWARE_SERIAL  MSerial4
  #define E0_HARDWARE_SERIAL MSerial4

  // Default TMC slave addresses
  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS  0
  #endif
  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS  2
  #endif
  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS  1
  #endif
  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS 3
  #endif
#endif
