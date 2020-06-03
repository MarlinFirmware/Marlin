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

#define SKR_MINI_E3_V2

// Onboard I2C EEPROM
#if NO_EEPROM_SELECTED
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE 0x1000                 // 4KB
  #undef NO_EEPROM_SELECTED
#endif

#include "pins_BTT_SKR_MINI_E3.h"

#define BOARD_INFO_NAME "BIGTREE SKR Mini E3 V2.0"

// Release PA13/PA14 (led, usb control) from SWD pins
#define DISABLE_DEBUG

#define NEOPIXEL_PIN                       PA8   // LED driving pin

#define PS_ON_PIN                          PC13  // Power Supply Control

#define FAN1_PIN                           PC7

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN               FAN1_PIN
#endif

/**
 * TMC220x stepper drivers
 * Hardware serial communication ports.
 */
#if HAS_TMC_UART
  #define X_HARDWARE_SERIAL  Serial4
  #define Y_HARDWARE_SERIAL  Serial4
  #define Z_HARDWARE_SERIAL  Serial4
  #define E0_HARDWARE_SERIAL Serial4
#endif
