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
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 */

/**
 *
 * For TARGET_LPC1768
 */

#ifdef TARGET_LPC1768

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../../MarlinConfig.h"

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(SOFTWARE_SPI)
  // --------------------------------------------------------------------------
  // software SPI
  // --------------------------------------------------------------------------
  // bitbanging transfer
  // run at ~100KHz (necessary for init)
  static uint8_t spiTransfer(uint8_t b) { // using Mode 0
    for (int bits = 0; bits < 8; bits++) {
      if (b & 0x80) {
        WRITE(MOSI_PIN, HIGH);
      }
      else {
        WRITE(MOSI_PIN, LOW);
      }
      b <<= 1;

      WRITE(SCK_PIN, HIGH);
      delayMicroseconds(3U);

      if (READ(MISO_PIN)) {
        b |= 1;
      }
      WRITE(SCK_PIN, LOW);
      delayMicroseconds(3U);
    }
    return b;
  }

  void spiBegin() {
    SET_OUTPUT(SS_PIN);
    WRITE(SS_PIN, HIGH);
    SET_OUTPUT(SCK_PIN);
    SET_INPUT(MISO_PIN);
    SET_OUTPUT(MOSI_PIN);
  }

  void spiInit(uint8_t spiRate) {
    UNUSED(spiRate);
    WRITE(SS_PIN, HIGH);
    WRITE(MOSI_PIN, HIGH);
    WRITE(SCK_PIN, LOW);
  }

  uint8_t spiRec() {
    WRITE(SS_PIN, LOW);
    uint8_t b = spiTransfer(0xff);
    WRITE(SS_PIN, HIGH);
    return b;
  }

  void spiRead(uint8_t*buf, uint16_t nbyte) {
    if (nbyte == 0) return;
    WRITE(SS_PIN, LOW);
    for (int i = 0; i < nbyte; i++) {
      buf[i] = spiTransfer(0xff);
    }
    WRITE(SS_PIN, HIGH);
  }

  void spiSend(uint8_t b) {
    WRITE(SS_PIN, LOW);
    uint8_t response = spiTransfer(b);
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

  static void spiSend(const uint8_t* buf, size_t n) {
    uint8_t response;
    if (n == 0) return;
    WRITE(SS_PIN, LOW);
    for (uint16_t i = 0; i < n; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    uint8_t response;

    WRITE(SS_PIN, LOW);
    response = spiTransfer(token);

    for (uint16_t i = 0; i < 512; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }
#else
  void spiBegin() {
  }

  void spiInit(uint8_t spiRate) {
  }

  void spiSend(byte b) {
  }

  void spiSend(const uint8_t* buf, size_t n) {
  }

  void spiSend(uint32_t chan, byte b) {
  }

  void spiSend(uint32_t chan, const uint8_t* buf, size_t n) {

  }

  // Read single byte from SPI
  uint8_t spiRec() {
    return 0;
  }

  uint8_t spiRec(uint32_t chan) {
    return 0;
  }

  // Read from SPI into buffer
  void spiRead(uint8_t*buf, uint16_t nbyte) {
  }

  // Write from buffer to SPI
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
  }
#endif // ENABLED(SOFTWARE_SPI)

#endif // TARGET_LPC1768
