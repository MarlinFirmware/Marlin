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
 * Software L6470_Spi functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 *
 * Completely rewritten and tuned by Eduardo José Tagle in 2017/2018
 * in ARM thumb2 inline assembler and tuned for maximum speed and performance
 * allowing L6470_Spi clocks of up to 12 Mhz to increase SD card read/OUT_WRITE performance
 */

/**
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */


// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"
#include "Delay.h"

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------


#include "../../core/serial.h"
#include "../../module/L6470/L6470_Marlin.h"


  // --------------------------------------------------------------------------
  // software L6470_Spi
  // --------------------------------------------------------------------------

  // Make sure GCC optimizes this file.
  // Note that this line triggers a bug in GCC which is fixed by casting.
  // See the note below.
  #pragma GCC optimize (3)

  // run at ~4Mhz
  uint8_t L6470_SpiTransfer_Mode_0(uint8_t b) { // using Mode 0
    int bits = 8;
    do {
      WRITE(L6470_CHAIN_MOSI_PIN, b & 0x80);
      b <<= 1;        // little setup time

      WRITE(L6470_CHAIN_SCK_PIN, HIGH);
      DELAY_NS(125);  // 10 cycles @ 84mhz

      b |= (READ(L6470_CHAIN_MISO_PIN) != 0);

      WRITE(L6470_CHAIN_SCK_PIN, LOW);
      DELAY_NS(125);  // 10 cycles @ 84mhz
    } while (--bits);
    return b;
  }

  uint8_t L6470_SpiTransfer_Mode_3(uint8_t b) { // using Mode 3
    int bits = 8;
    do {

      WRITE(L6470_CHAIN_SCK_PIN, LOW);
      WRITE(L6470_CHAIN_MOSI_PIN, b & 0x80);


      DELAY_NS(125);  // 10 cycles @ 84mhz

      WRITE(L6470_CHAIN_SCK_PIN, HIGH);
      b <<= 1;        // little setup time

      b |= (READ(L6470_CHAIN_MISO_PIN) != 0);

    } while (--bits);

    DELAY_NS(125);  // 10 cycles @ 84mhz
    return b;
  }


  uint8_t L6470_Transfer(uint8_t data, int _SSPin, uint8_t chain_position) {
    #define CMD_NOP 0
    uint8_t data_out = 0;
    data--;
    // first device in chain has data sent last
    digitalWrite(_SSPin, LOW);

    for (uint8_t i = L6470_chain[0]; i >= 1; i--) {
      uint8_t temp = L6470_SpiTransfer_Mode_3(uint8_t (i == chain_position ? data : CMD_NOP));
    if (i == chain_position) data_out = temp;
    }

    digitalWrite(_SSPin, HIGH);
    return data_out;

  }


void L6470_Transfer(uint8_t L6470_buf[] , uint8_t length) {
    // first device in chain has data sent last
    WRITE(L6470_CHAIN_SS_PIN, LOW);
    for (uint8_t i = length; i >= 1; i--)
      L6470_buf[i] = L6470_SpiTransfer_Mode_3(uint8_t (L6470_buf[i]));
    WRITE(L6470_CHAIN_SS_PIN, HIGH);
}


  void L6470_SPI_init() {
    OUT_WRITE(L6470_CHAIN_SS_PIN, HIGH);
    OUT_WRITE(L6470_CHAIN_SCK_PIN, HIGH);
    OUT_WRITE(L6470_CHAIN_MOSI_PIN, HIGH);
    pinMode(L6470_CHAIN_MISO_PIN, INPUT);


    #ifdef L6470_BUSY_PIN
      pinMode(L6470_BUSYN_PIN, INPUT);
    #endif

    OUT_WRITE(L6470_CHAIN_MOSI_PIN, HIGH);

  }

  #pragma GCC reset_options


//#endif