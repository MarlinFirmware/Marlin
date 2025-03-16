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

/**
 * Adapted from Arduino Sd2Card Library
 * Copyright (c) 2009 by William Greiman
 */

/**
 * HAL for AVR - SPI functions
 */

#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

void spiBegin() {
  #if PIN_EXISTS(SD_SS)
    // Do not init HIGH for boards with pin 4 used as Fans or Heaters or otherwise, not likely to have multiple SPI devices anyway.
    #if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
      // SS must be in output mode even it is not chip select
      SET_OUTPUT(SD_SS_PIN);
    #else
      // set SS high - may be chip select for another SPI device
      OUT_WRITE(SD_SS_PIN, HIGH);
    #endif
  #endif
  SET_OUTPUT(SD_SCK_PIN);
  SET_INPUT(SD_MISO_PIN);
  SET_OUTPUT(SD_MOSI_PIN);

  IF_DISABLED(SOFTWARE_SPI, spiInit(SPI_HALF_SPEED));
}

#if NONE(SOFTWARE_SPI, FORCE_SOFT_SPI)

  // ------------------------
  // Hardware SPI
  // ------------------------

  // make sure SPCR rate is in expected bits
  #if (SPR0 != 0 || SPR1 != 1)
    #error "unexpected SPCR bits"
  #endif

  /**
   * Initialize hardware SPI
   * Set SCK rate to F_CPU/pow(2, 1 + spiRate) for spiRate [0,6]
   */
  void spiInit(uint8_t spiRate) {
    // See avr processor documentation
    CBI(
      #ifdef PRR
        PRR
      #elif defined(PRR0)
        PRR0
      #endif
      , PRSPI
    );

    SPCR = _BV(SPE) | _BV(MSTR) | (spiRate >> 1);
    SPSR = spiRate & 1 || spiRate == 6 ? 0 : _BV(SPI2X);
  }

  /** SPI receive a byte */
  uint8_t spiRec() {
    SPDR = 0xFF;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    return SPDR;
  }

  /** SPI read data  */
  void spiRead(uint8_t *buf, uint16_t nbyte) {
    if (nbyte-- == 0) return;
    SPDR = 0xFF;
    for (uint16_t i = 0; i < nbyte; i++) {
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      buf[i] = SPDR;
      SPDR = 0xFF;
    }
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    buf[nbyte] = SPDR;
  }

  /** SPI send a byte */
  void spiSend(uint8_t b) {
    SPDR = b;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  }

  /** SPI send block  */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    SPDR = token;
    for (uint16_t i = 0; i < 512; i += 2) {
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      SPDR = buf[i];
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      SPDR = buf[i + 1];
    }
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  }

  /** begin spi transaction */
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    // Based on Arduino SPI library
    // Clock settings are defined as follows. Note that this shows SPI2X
    // inverted, so the bits form increasing numbers. Also note that
    // fosc/64 appears twice
    // SPR1 SPR0 ~SPI2X Freq
    //   0    0     0   fosc/2
    //   0    0     1   fosc/4
    //   0    1     0   fosc/8
    //   0    1     1   fosc/16
    //   1    0     0   fosc/32
    //   1    0     1   fosc/64
    //   1    1     0   fosc/64
    //   1    1     1   fosc/128

    // We find the fastest clock that is less than or equal to the
    // given clock rate. The clock divider that results in clock_setting
    // is 2 ^^ (clock_div + 1). If nothing is slow enough, we'll use the
    // slowest (128 == 2 ^^ 7, so clock_div = 6).
    uint8_t clockDiv;

    // When the clock is known at compiletime, use this if-then-else
    // cascade, which the compiler knows how to completely optimize
    // away. When clock is not known, use a loop instead, which generates
    // shorter code.
    if (__builtin_constant_p(spiClock)) {
      if (spiClock >= F_CPU / 2)       clockDiv = 0;
      else if (spiClock >= F_CPU / 4)  clockDiv = 1;
      else if (spiClock >= F_CPU / 8)  clockDiv = 2;
      else if (spiClock >= F_CPU / 16) clockDiv = 3;
      else if (spiClock >= F_CPU / 32) clockDiv = 4;
      else if (spiClock >= F_CPU / 64) clockDiv = 5;
      else                             clockDiv = 6;
    }
    else {
      uint32_t clockSetting = F_CPU / 2;
      clockDiv = 0;
      while (clockDiv < 6 && spiClock < clockSetting) {
        clockSetting /= 2;
        clockDiv++;
      }
    }

    // Compensate for the duplicate fosc/64
    if (clockDiv == 6) clockDiv = 7;

    // Invert the SPI2X bit
    clockDiv ^= 0x1;

    SPCR = _BV(SPE) | _BV(MSTR) | ((bitOrder == LSBFIRST) ? _BV(DORD) : 0) |
      (dataMode << CPHA) | ((clockDiv >> 1) << SPR0);
    SPSR = clockDiv | 0x01;
  }

#else // SOFTWARE_SPI || FORCE_SOFT_SPI

  // ------------------------
  // Software SPI
  // ------------------------

  // nop to tune soft SPI timing
  #define nop asm volatile ("\tnop\n")

  void spiInit(uint8_t) { /* do nothing */ }

  // Begin SPI transaction, set clock, bit order, data mode
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) { /* do nothing */ }

  // Soft SPI receive byte
  uint8_t spiRec() {
    uint8_t data = 0;
    // no interrupts during byte receive - about 8µs
    cli();
    // output pin high - like sending 0xFF
    WRITE(SD_MOSI_PIN, HIGH);

    for (uint8_t i = 0; i < 8; ++i) {
      WRITE(SD_SCK_PIN, HIGH);

      nop; // adjust so SCK is nice
      nop;

      data <<= 1;

      if (READ(SD_MISO_PIN)) data |= 1;

      WRITE(SD_SCK_PIN, LOW);
    }

    sei();
    return data;
  }

  // Soft SPI read data
  void spiRead(uint8_t *buf, uint16_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      buf[i] = spiRec();
  }

  // Soft SPI send byte
  void spiSend(uint8_t data) {
    // no interrupts during byte send - about 8µs
    cli();
    for (uint8_t i = 0; i < 8; ++i) {
      WRITE(SD_SCK_PIN, LOW);
      WRITE(SD_MOSI_PIN, data & 0x80);
      data <<= 1;
      WRITE(SD_SCK_PIN, HIGH);
    }

    nop; // hold SCK high for a few ns
    nop;
    nop;
    nop;

    WRITE(SD_SCK_PIN, LOW);

    sei();
  }

  // Soft SPI send block
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    spiSend(token);
    for (uint16_t i = 0; i < 512; i++)
      spiSend(buf[i]);
  }

#endif // SOFTWARE_SPI || FORCE_SOFT_SPI

#endif // __AVR__
