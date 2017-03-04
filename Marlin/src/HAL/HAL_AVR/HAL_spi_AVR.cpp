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
 * Originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 */
 
/**
 * Description: HAL for AVR - SPI functions
 *
 * For ARDUINO_ARCH_AVR
 */

#ifdef ARDUINO_ARCH_AVR

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../../Marlin.h"

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void spiBegin (void) {
  SET_OUTPUT(SS_PIN);
  WRITE(SS_PIN, HIGH);
  SET_OUTPUT(SCK_PIN);
  SET_INPUT(MISO_PIN);
  SET_OUTPUT(MOSI_PIN);

  #if DISABLED(SOFTWARE_SPI)
    // SS must be in output mode even it is not chip select
    SET_OUTPUT(SS_PIN);
    // set SS high - may be chip select for another SPI device
    #if SET_SPI_SS_HIGH
      WRITE(SS_PIN, HIGH);
    #endif  // SET_SPI_SS_HIGH
    // set a default rate
    spiInit(1);
  #endif  // SOFTWARE_SPI
}


//------------------------------------------------------------------------------
#if DISABLED(SOFTWARE_SPI)
  // functions for hardware SPI
  //------------------------------------------------------------------------------
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
        , PRSPI);

    SPCR = _BV(SPE) | _BV(MSTR) | (spiRate >> 1);
    SPSR = spiRate & 1 || spiRate == 6 ? 0 : _BV(SPI2X);
  }
  //------------------------------------------------------------------------------
  /** SPI receive a byte */
  uint8_t spiRec(void) {
    SPDR = 0XFF;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    return SPDR;
  }
  //------------------------------------------------------------------------------
  /** SPI read data  */
  void spiRead(uint8_t* buf, uint16_t nbyte) {
    if (nbyte-- == 0) return;
    SPDR = 0XFF;
    for (uint16_t i = 0; i < nbyte; i++) {
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      buf[i] = SPDR;
      SPDR = 0XFF;
    }
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
    buf[nbyte] = SPDR;
  }
  //------------------------------------------------------------------------------
  /** SPI send a byte */
  void spiSend(uint8_t b) {
    SPDR = b;
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  }
  //------------------------------------------------------------------------------
  /** SPI send block  */
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    SPDR = token;
    for (uint16_t i = 0; i < 512; i += 2) {
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      SPDR = buf[i];
      while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
      SPDR = buf[i + 1];
    }
    while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  }
       //------------------------------------------------------------------------------
#else  // SOFTWARE_SPI
       //------------------------------------------------------------------------------
  /** nop to tune soft SPI timing */
  #define nop asm volatile ("\tnop\n")

  /** Set SPI rate */
  void spiInit(uint8_t spiRate) {
    // nothing to do
    UNUSED(spiRate);
  }

  //------------------------------------------------------------------------------
  /** Soft SPI receive byte */
  uint8_t spiRec() {
    uint8_t data = 0;
    // no interrupts during byte receive - about 8 us
    cli();
    // output pin high - like sending 0XFF
    WRITE(MOSI_PIN, HIGH);

    for (uint8_t i = 0; i < 8; i++) {
      WRITE(SCK_PIN, HIGH);

      // adjust so SCK is nice
      nop;
      nop;

      data <<= 1;

      if (READ(MISO_PIN)) data |= 1;

      WRITE(SCK_PIN, LOW);
    }
    // enable interrupts
    sei();
    return data;
  }
  //------------------------------------------------------------------------------
  /** Soft SPI read data */
  void spiRead(uint8_t* buf, uint16_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      buf[i] = spiRec();
  }
  //------------------------------------------------------------------------------
  /** Soft SPI send byte */
  void spiSend(uint8_t data) {
    // no interrupts during byte send - about 8 us
    cli();
    for (uint8_t i = 0; i < 8; i++) {
      WRITE(SCK_PIN, LOW);

      WRITE(MOSI_PIN, data & 0X80);

      data <<= 1;

      WRITE(SCK_PIN, HIGH);
    }
    // hold SCK high for a few ns
    nop;
    nop;
    nop;
    nop;

    WRITE(SCK_PIN, LOW);
    // enable interrupts
    sei();
  }
  //------------------------------------------------------------------------------
  /** Soft SPI send block */
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    spiSend(token);
    for (uint16_t i = 0; i < 512; i++)
      spiSend(buf[i]);
  }
#endif  // SOFTWARE_SPI


#endif // ARDUINO_ARCH_AVR
