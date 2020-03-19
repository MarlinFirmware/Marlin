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

#include "pins_BTT_SKR_MINI_E3.h"

#define BOARD_INFO_NAME "BIGTREE SKR Mini E3 V1.2"

#define NEOPIXEL_PIN       PC7  // LED driving pin

/**
 * TMC2208/TMC2209 stepper drivers
 */
#if HAS_TMC_UART
  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN  PB15
  #define X_SERIAL_RX_PIN  PB15

  #define Y_SERIAL_TX_PIN  PC6
  #define Y_SERIAL_RX_PIN  PC6

  #define Z_SERIAL_TX_PIN  PC10
  #define Z_SERIAL_RX_PIN  PC10

  #define E0_SERIAL_TX_PIN PC11
  #define E0_SERIAL_RX_PIN PC11

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif
