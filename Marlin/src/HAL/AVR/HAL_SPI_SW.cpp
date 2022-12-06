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

#include "registers.h"

#if EITHER(SOFTWARE_SPI, FORCE_SOFT_SPI)

  // ------------------------
  // Software SPI
  // ------------------------

  // nop to tune soft SPI timing
  #define nop asm volatile ("\tnop\n")

  static void _spi_on_error(int code) {
    for (;;) {
#if ENABLED(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(400);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(400);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(400);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(400);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (int n = 0; n < code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(500);
        OUT_WRITE(BEEPER_PIN, LOW);
        if (n < code-1)
          delay(500);
      }
      delay(1000);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(800);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
#endif
    }
  }

  static pin_dev_state_t _spi_sck_devstate, _spi_miso_devstate, _spi_mosi_devstate, _spi_cs_devstate;
  static int _spi_sck_pin, _spi_miso_pin, _spi_mosi_pin, _spi_cs_pin;
  static int _spi_bit_order = SPI_BITORDER_DEFAULT;
  static bool _spi_is_running = false;

  void spiBegin() {
    #if PIN_EXISTS(SD_SS)
      // Do not init HIGH for boards with pin 4 used as Fans or Heaters or otherwise, not likely to have multiple SPI devices anyway.
      #if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
        // SS must be in output mode even if it is not chip select
        SET_OUTPUT(SD_SS_PIN);
      #else
        // set SS high - may be chip select for another SPI device
        OUT_WRITE(SD_SS_PIN, HIGH);
      #endif
    #endif
    // This could still be required because the specification says that the DDR of those pins is "User Defined".
    SET_OUTPUT(SD_SCK_PIN);
    SET_OUTPUT(SD_MOSI_PIN);
  }

  void spiInit(uint8_t spiRate, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    if (_spi_is_running)
      _spi_on_error(1);

    // In software SPI we want to power all pins in GPIO mode.
    // That is why we have to disable all peripherals that conflict with our SPI operations.
    int use_pin_sck = (hint_sck >= 0) ? hint_sck : SD_SCK_PIN;
    int use_pin_miso = (hint_miso >= 0) ? hint_miso : SD_MISO_PIN;
    int use_pin_mosi = (hint_mosi >= 0) ? hint_mosi : SD_MOSI_PIN;
    int use_pin_cs = (hint_cs >= 0) ? hint_cs : SD_SS_PIN;

    if (use_pin_sck >= 0)
      _spi_sck_devstate = _ATmega_savePinAlternate((unsigned int)use_pin_sck);
    if (use_pin_miso >= 0)
      _spi_miso_devstate = _ATmega_savePinAlternate((unsigned int)use_pin_miso);
    if (use_pin_mosi >= 0)
      _spi_mosi_devstate = _ATmega_savePinAlternate((unsigned int)use_pin_mosi);
    if (use_pin_cs >= 0)
      _spi_cs_devstate = _ATmega_savePinAlternate((unsigned int)use_pin_cs);

    _spi_sck_pin = use_pin_sck;
    _spi_miso_pin = use_pin_miso;
    _spi_mosi_pin = use_pin_mosi;
    _spi_cs_pin = use_pin_cs;

    _ATmega_pinMode(_spi_sck_pin, OUTPUT);
    _ATmega_pinMode(_spi_miso_pin, INPUT);
    _ATmega_pinMode(_spi_mosi_pin, OUTPUT);

    if (use_pin_cs >= 0)
      _ATmega_digitalWrite(use_pin_cs, LOW);

    _spi_is_running = true;
  }
  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    spiInit(0, hint_sck, hint_miso, hint_mosi, hint_cs);
  }
  void spiClose() {
    if (_spi_is_running == false)
      _spi_on_error(2);

    if (_spi_cs_pin >= 0)
      _ATmega_digitalWrite(_spi_cs_pin, HIGH);

    // Restore pin device states.
    // Has to be done in reverse order.
    if (_spi_cs_pin >= 0)
      _ATmega_restorePinAlternate(_spi_cs_pin, _spi_cs_devstate);
    if (_spi_mosi_pin >= 0)
      _ATmega_restorePinAlternate(_spi_mosi_pin, _spi_mosi_devstate);
    if (_spi_miso_pin >= 0)
      _ATmega_restorePinAlternate(_spi_miso_pin, _spi_miso_devstate);
    if (_spi_sck_pin >= 0)
      _ATmega_restorePinAlternate(_spi_sck_pin, _spi_sck_devstate);

    _spi_is_running = false;
  }

  void spiSetBitOrder(int bitOrder) {
    _spi_bit_order = bitOrder;
  }

  void spiSetClockMode(int clockMode) {
    // TODO.
    if (clockMode != SPI_CLKMODE_0) {
      for (;;) {}
    }
  }

  void spiEstablish() { /* do nothing */ }

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
    _ATmega_digitalWrite(_spi_mosi_pin, HIGH);

    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );

    LOOP_L_N(i, 8) {
      _ATmega_digitalWrite(_spi_sck_pin, HIGH);

      nop; // adjust so SCK is nice
      nop;

      if (_ATmega_digitalRead(_spi_miso_pin) == HIGH)
      {
        int bitidx = ( msb ? 7-i : i );
        data |= ( 1 << bitidx );
      }

      _ATmega_digitalWrite(_spi_sck_pin, LOW);
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
    _ATmega_digitalWrite(_spi_mosi_pin, HIGH);

    LOOP_L_N(i, 16) {
      _ATmega_digitalWrite(_spi_sck_pin, HIGH);

      nop; // adjust so SCK is nice
      nop;

      if (_ATmega_digitalRead(_spi_miso_pin) == HIGH)
      {
        int bitidx = ( msb ? 15-i : i );
        data |= ( 1 << bitidx );
      }

      _ATmega_digitalWrite(_spi_sck_pin, LOW);
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
      _ATmega_digitalWrite(_spi_sck_pin, LOW);
      _ATmega_digitalWrite(_spi_mosi_pin, ( data & ( 1 << bitidx )) != 0);
      _ATmega_digitalWrite(_spi_sck_pin, HIGH);
    }

    nop; // hold SCK high for a few ns
    nop;
    nop;
    nop;

    _ATmega_digitalWrite(_spi_sck_pin, LOW);

    sei();
  }

  void spiSend16(uint16_t v) {
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    // no interrupts during byte send - about 8µs
    cli();
    LOOP_L_N(i, 16) {
      int bitidx = ( msb ? 15-i : i );
      _ATmega_digitalWrite(_spi_sck_pin, LOW);
      _ATmega_digitalWrite(_spi_mosi_pin, ( v & ( 1 << bitidx )) != 0);
      _ATmega_digitalWrite(_spi_sck_pin, HIGH);
    }

    nop; // hold SCK high for a few ns
    nop;
    nop;
    nop;

    _ATmega_digitalWrite(_spi_sck_pin, LOW);

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
