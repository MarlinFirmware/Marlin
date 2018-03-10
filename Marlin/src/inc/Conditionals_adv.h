/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Conditionals_adv.h
 * Defines that depend on advanced configuration.
 */

#ifndef CONDITIONALS_ADV_H
#define CONDITIONALS_ADV_H

  #ifndef USBCON
    // Define constants and variables for buffering serial data.
    // Use only 0 or powers of 2 greater than 1
    // : [0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, ...]
    #ifndef RX_BUFFER_SIZE
      #define RX_BUFFER_SIZE 128
    #endif
    // 256 is the max TX buffer limit due to uint8_t head and tail
    // : [0, 4, 8, 16, 32, 64, 128, 256]
    #ifndef TX_BUFFER_SIZE
      #define TX_BUFFER_SIZE 32
    #endif
  #else
    // SERIAL_XON_XOFF not supported on USB-native devices
    #undef SERIAL_XON_XOFF
  #endif

#endif // CONDITIONALS_ADV_H
