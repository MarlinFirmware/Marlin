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
  void spiInit(uint8_t spiRate, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    // Ignore SPI pin hints.

    // See avr processor documentation
    CBI(
      #ifdef PRR
        PRR
      #elif defined(PRR0)
        PRR0
      #endif
      , PRSPI
    );

    // DORD is set to 0 -> MSB transfer, else LSB

    SPCR = _BV(SPE) | _BV(MSTR) | (spiRate >> 1);
    SPSR = spiRate & 1 || spiRate == SPI_SPEED_6 ? 0 : _BV(SPI2X);
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint8_t spiRate;
    if (maxClockFreq >= 20000000) {
      spiRate = SPI_FULL_SPEED;
    }
    else if (maxClockFreq >= 5000000) {
      spiRate = SPI_HALF_SPEED;
    }
    else if (maxClockFreq >= 2500000) {
      spiRate = SPI_QUARTER_SPEED;
    }
    else if (maxClockFreq >= 1250000) {
      spiRate = SPI_EIGHTH_SPEED;
    }
    else if (maxClockFreq >= 625000) {
      spiRate = SPI_SPEED_5;
    }
    else if (maxClockFreq >= 300000) {
      spiRate = SPI_SPEED_6;
    }
    else
      spiRate = SPI_SPEED_6;
    
    spiInit(spiRate, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  void spiClose() { /* do nothing */ }

  void spiSetBitOrder(int bitOrder) {
    if (bitOrder == SPI_BITORDER_MSB) {
      SPCR &= ~_BV(DORD);
    }
    else if (bitOrder == SPI_BITORDER_LSB) {
      SPCR |= _BV(DORD);
    }
  }

  void spiSetClockMode(int clockMode) {
    // TODO.
    if (clockMode != SPI_CLKMODE_0) {
      for (;;) {}
    }
  }

  /** SPI receive a byte */
  uint8_t spiRec(uint8_t txval) {
    SPDR = txval;
    while (!TEST(SPSR, SPIF)) { /* do nothing */ }
    return SPDR;
  }

  uint16_t spiRec16(uint16_t txval) {
    bool msb = ( TEST(SPCR, DORD) == false );
    uint8_t tx_first, tx_second;
    if (msb) {
      tx_first = ( txval >> 8 );
      tx_second = ( txval & 0xFF );
    }
    else {
      tx_first = ( txval & 0xFF );
      tx_second = ( txval >> 8 );
    }
    SPDR = tx_first;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    uint16_t val = ( msb ? ( (uint16_t)SPDR << 8 ) : ( SPDR ) );
    SPDR = tx_second;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    val |= ( msb ? ( SPDR ) : ( (uint16_t)SPDR << 8 ) );
    return val;
  }

  /** SPI read data  */
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    if (nbyte-- == 0) return;
    SPDR = txval;
    for (uint16_t i = 0; i < nbyte; i++) {
      while (!TEST(SPSR, SPIF)) { /* do nothing */ }
      buf[i] = SPDR;
      SPDR = txval;
    }
    while (!TEST(SPSR, SPIF)) { /* do nothing */ }
    buf[nbyte] = SPDR;
  }

  /** SPI send a byte */
  void spiSend(uint8_t b) {
    SPDR = b;
    while (!TEST(SPSR, SPIF)) { /* do nothing */ }
  }

  void spiSend16(uint16_t v) {
    bool msb = ( TEST(SPCR, DORD) == false );
    SPDR = ( msb ? ( v >> 8 ) : ( v & 0xFF ) );
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    SPDR = ( msb ? ( v & 0xFF ) : ( v >> 8 ) );
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  }

  /** SPI send block  */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    SPDR = token;
    for (uint16_t i = 0; i < 512; i += 2) {
      while (!TEST(SPSR, SPIF)) { /* do nothing */ }
      SPDR = buf[i];
      while (!TEST(SPSR, SPIF)) { /* do nothing */ }
      SPDR = buf[i + 1];
    }
    while (!TEST(SPSR, SPIF)) { /* do nothing */ }
  }

  /** Begin SPI transaction */
  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend(buf[n]);
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend16(buf[n]);
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend16(val);
  }

#if 0
  /** begin spi transaction (TODO: merge ideas into spiInitEx) */
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
#endif

#else // SOFTWARE_SPI || FORCE_SOFT_SPI

  // ------------------------
  // Software SPI
  // ------------------------

  // nop to tune soft SPI timing
  #define nop asm volatile ("\tnop\n")

  static int _spi_bit_order = SPI_BITORDER_DEFAULT;

  void spiInit(uint8_t, const int/*=-1*/, const int/*=-1*/, const int/*=-1*/, const int/*=-1*/) { /* do nothing */ }
  void spiInitEx(uint32_t, int, int, int, int) { /* do nothing */ }
  void spiClose() { /* do nothing */ }

  void spiSetBitOrder(int bitOrder) {
    _spi_bit_order = bitOrder;
  }

  void spiSetClockMode(int clockMode) {
    // TODO.
    if (clockMode != SPI_CLKMODE_0) {
      for (;;) {}
    }
  }

  // Soft SPI receive byte
  uint8_t spiRec(uint8_t txval) {
    if (txval != 0xFF) {
      // TODO.
      for (;;) {}
    }
    uint8_t data = 0;
    // no interrupts during byte receive - about 8µs
    cli();
    // output pin high - like sending 0xFF
    WRITE(SD_MOSI_PIN, HIGH);

    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );

    LOOP_L_N(i, 8) {
      WRITE(SD_SCK_PIN, HIGH);

      nop; // adjust so SCK is nice
      nop;

      if (READ(SD_MISO_PIN))
      {
        int bitidx = ( msb ? 7-i : i );
        data |= ( 1 << bitidx );
      }

      WRITE(SD_SCK_PIN, LOW);
    }

    sei();
    return data;
  }

  uint16_t spiRec16(uint16_t txval) {
    if (txval != 0xFFFF) {
      // TODO.
      for (;;) {}
    }
    uint16_t data = 0;
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    // no interrupts during byte receive - about 8µs
    cli();
    // output pin high - like sending 0xFF
    WRITE(SD_MOSI_PIN, HIGH);

    LOOP_L_N(i, 16) {
      WRITE(SD_SCK_PIN, HIGH);

      nop; // adjust so SCK is nice
      nop;

      if (READ(SD_MISO_PIN))
      {
        int bitidx = ( msb ? 15-i : i );
        data |= ( 1 << bitidx );
      }

      WRITE(SD_SCK_PIN, LOW);
    }

    sei();
    return data;
  }

  // Soft SPI read data
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    for (uint16_t i = 0; i < nbyte; i++)
      buf[i] = spiRec(txval);
  }

  // Soft SPI send byte
  void spiSend(uint8_t data) {
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    // no interrupts during byte send - about 8µs
    cli();
    LOOP_L_N(i, 8) {
      int bitidx = ( msb ? 7-i : i );
      WRITE(SD_SCK_PIN, LOW);
      WRITE(SD_MOSI_PIN, ( data & ( 1 << bitidx )) != 0);
      WRITE(SD_SCK_PIN, HIGH);
    }

    nop; // hold SCK high for a few ns
    nop;
    nop;
    nop;

    WRITE(SD_SCK_PIN, LOW);

    sei();
  }

  void spiSend16(uint16_t v) {
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    // no interrupts during byte send - about 8µs
    cli();
    LOOP_L_N(i, 16) {
      int bitidx = ( msb ? 15-i : i );
      WRITE(SD_SCK_PIN, LOW);
      WRITE(SD_MOSI_PIN, ( v & ( 1 << bitidx )) != 0);
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

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend(buf[n]);
  }
  
  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend16(buf[n]);
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend16(val);
  }

#endif // SOFTWARE_SPI || FORCE_SOFT_SPI

#endif // __AVR__
