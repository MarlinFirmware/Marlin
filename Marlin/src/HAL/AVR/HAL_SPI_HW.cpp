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

#if NONE(SOFTWARE_SPI, FORCE_SOFT_SPI)

#ifndef AVR_CHIPOSCILLATOR_FREQ
#error Missing AVR crystal oscillator frequency! Consult manufacturer schematics for further details (XTAL1/XTAL2 pins as guidance)
#endif

  // ------------------------
  // Hardware SPI
  // ------------------------

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

  static bool _spi_is_running = false;
  static int _spi_cs_pin;
  static bool _spi_transaction_is_active;
  static bool _spi_dirty_tx;

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

    // By default we disable the SPI peripheral.
    _PRR0._PRSPI = true;
  }

  // Returns the clock frequency as output by the System Clock Prescaler.
  inline uint32_t _GetSystemClockFrequency() {
    // See which clock is selected.
    const ATmega_lfuse lfuse = AVR_LFUSE_VALUE;

    uint32_t baseclk;

    switch(lfuse._CKSEL) {
    case 15: case 14: case 13: case 12: case 11: case 10: case 9: case 8:
    case 7: case 6: case 5: case 4:
      baseclk = AVR_CHIPOSCILLATOR_FREQ;
      break;
    case 3:
      // Internal 128kHz RC Oscillator.
      baseclk = 128000;
      break;
    case 2:
      // Calibrated Internal RC Oscillator.
      baseclk = 8000000;
      break;
    case 1:
    case 0:
      _spi_on_error(3);
      break;
    }

    // Divide the system clock.
    uint8_t clkps_po2 = _CLKPR._CLKPS;

    return ( baseclk >> clkps_po2 );
  }

  inline void _spiConfigBitOrder(SPCR_reg_t& __SPCR, int mode) {
    if (mode == SPI_BITORDER_LSB) {
      __SPCR._DORD = 1;
    }
    else if (mode == SPI_BITORDER_MSB) {
      __SPCR._DORD = 0;
    }
  }

  inline void _spiConfigClockMode(SPCR_reg_t& __SPCR, int mode) {
    if (mode == SPI_CLKMODE_0) {
      __SPCR._CPOL = 0;
      __SPCR._CPHA = 0;
    }
    else if (mode == SPI_CLKMODE_1) {
      __SPCR._CPOL = 0;
      __SPCR._CPHA = 1;
    }
    else if (mode == SPI_CLKMODE_2) {
      __SPCR._CPOL = 1;
      __SPCR._CPHA = 0;
    }
    else if (mode == SPI_CLKMODE_3) {
      __SPCR._CPOL = 1;
      __SPCR._CPHA = 1;
    }
  }

  /**
   * Initialize hardware SPI transaction
   */
  void spiInitEx(uint32_t maxClockFreq, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    if (_spi_is_running)
      _spi_on_error(1);

    // In hardware SPI mode we can only use the pre-determined SPI pins for MISO, MOSI and SCK, thus ignore the first three pin hints.
    // But for the chip-select pin, we either have to go HW mode if it is the peripheral SPI pin or we can go SW if it is a GPIO.
    if ( _ATmega_getPinFunctions(hint_cs).hasFunc(eATmegaPinFunc::SPI_CS) ) {
      // HW SPI_CS
      _spi_cs_pin = -1;
    }
    else {
      // SW SPI_CS
      _spi_cs_pin = hint_cs;
    }

    // Clear the power-reduction.
    _PRR0._PRSPI = false;

    // Calculate the required division to run the SPI clock below maxClockFreq.
    uint32_t sysclk = _GetSystemClockFrequency();

    SPCR_reg_t __SPCR;
    __SPCR._SPIE = false;
    __SPCR._SPE = true;
    _spiConfigBitOrder(__SPCR, SPI_BITORDER_DEFAULT);
    __SPCR._MSTR = true;
    _spiConfigClockMode(__SPCR, SPI_CLKMODE_DEFAULT);

    if ((sysclk / 2) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 0;
    }
    else if ((sysclk / 4) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 0;
    }
    else if ((sysclk / 8) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 1;
    }
    else if ((sysclk / 16) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 1;
    }
    else if ((sysclk / 32) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 2;
    }
    else if ((sysclk / 64) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 2;
    }
    else {
      // Cannot go below it.
      _SPSR._SPI2X = false;
      __SPCR._SPR = 3;
    }

    // Write initial configuration.
    _SPCR = __SPCR;

    _spi_is_running = true;
    _spi_transaction_is_active = false;
    _spi_dirty_tx = false;
  }

  static void _maybe_start_transaction() {
    if (_spi_transaction_is_active) return;

    if (_spi_cs_pin >= 0)
      _ATmega_digitalWrite(_spi_cs_pin, LOW);

    _spi_transaction_is_active = true;
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:    clock = 20000000; break; // 13.9mhz=20000000  6.75mhz=10000000  3.38mhz=5000000  .833mhz=1000000
      case SPI_HALF_SPEED:    clock =  5000000; break;
      case SPI_QUARTER_SPEED: clock =  2500000; break;
      case SPI_EIGHTH_SPEED:  clock =  1250000; break;
      case SPI_SPEED_5:       clock =   625000; break;
      case SPI_SPEED_6:       clock =   300000; break;
      default:
        clock = 4000000; // Default from the SPI library
    }
    spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  inline void _spi_finish_tx() {
    if (_spi_dirty_tx == false) return;

    while (_SPSR._SPIF == false) { /* do nothing */ }

    _spi_dirty_tx = false;
  }

  void spiClose() {
    if (_spi_is_running == false)
      _spi_on_error(2);

    _spi_finish_tx();

    if (_spi_transaction_is_active) {
      if (_spi_cs_pin >= 0)
        _ATmega_digitalWrite(_spi_cs_pin, HIGH);
      
      _spi_transaction_is_active = false;
    }

    // Disable the peripheral again.
    _PRR0._PRSPI = true;

    _spi_is_running = false;
  }

  void spiSetBitOrder(int bitOrder) {
    _spiConfigBitOrder(_SPCR, bitOrder);
  }

  void spiSetClockMode(int clockMode) {
    _spiConfigClockMode(_SPCR, clockMode);
  }

  void spiEstablish() {
    _maybe_start_transaction();
  }

  /** SPI receive a byte */
  uint8_t spiRec(uint8_t txval) {
    _maybe_start_transaction();
    _spi_finish_tx();
    _SPDR = txval;
    while (_SPSR._SPIF == false) { /* wait until data has been received */ }
    return _SPDR;
  }

  inline void _split_txbytes(uint16_t txval, uint8_t& tx_first, uint8_t& tx_second, bool msb) {
    if (msb) {
      tx_first = ( txval >> 8 );
      tx_second = ( txval & 0xFF );
    }
    else {
      tx_first = ( txval & 0xFF );
      tx_second = ( txval >> 8 );
    }
  }

  inline uint16_t _fuse_txbytes(uint8_t rx_first, uint8_t rx_second, bool msb) {
    if (msb) {
      return ( (uint16_t)rx_first << 8 ) | ( (uint16_t)rx_second );
    }
    else {
      return ( (uint16_t)rx_first ) | ( (uint16_t)rx_second << 8 );
    }
  }

  uint16_t spiRec16(uint16_t txval) {
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(txval, tx_first, tx_second, msb);
    _spi_finish_tx();
    _SPDR = tx_first;
    while (_SPSR._SPIF == false) { /* Intentionally left empty */ }
    uint8_t rx_first = _SPDR;
    _SPDR = tx_second;
    while (_SPSR._SPIF == false) { /* Intentionally left empty */ }
    uint8_t rx_second = _SPDR;
    return _fuse_txbytes(rx_first, rx_second, msb);
  }

  /** SPI read data  */
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    if (nbyte == 0) return;
    _maybe_start_transaction();
    nbyte--;
    _spi_finish_tx();
    _SPDR = txval;
    for (uint16_t i = 0; i < nbyte; i++) {
      while (_SPSR._SPIF == false) { /* do nothing */ }
      buf[i] = _SPDR;
      _SPDR = txval;
    }
    while (_SPSR._SPIF == false) { /* do nothing */ }
    buf[nbyte] = _SPDR;
  }

  inline void _spiSendByte(uint8_t byte) {
    _spi_finish_tx();
    _SPDR = byte;
    _spi_dirty_tx = true;
  }

  /** SPI send a byte */
  void spiSend(uint8_t b) {
    _maybe_start_transaction();
    _spiSendByte(b);
  }

  void spiSend16(uint16_t v) {
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(v, tx_first, tx_second, msb);
    _spiSendByte(tx_first);
    _spiSendByte(tx_second);
  }

  /** SPI send block  */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    _maybe_start_transaction();
    _spiSendByte(token);
    for (uint16_t i = 0; i < 512; i++) {
      _spiSendByte(buf[i]);
    }
  }

  /** Begin SPI transaction */
  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < cnt; n++)
      _spiSendByte(n);
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    for (uint16_t n = 0; n < cnt; n++) {
      uint8_t tx_first, tx_second;
      _split_txbytes(buf[n], tx_first, tx_second, msb);
      _spiSendByte(tx_first);
      _spiSendByte(tx_second);
    }
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < repcnt; n++)
      _spiSendByte(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(val, tx_first, tx_second, msb);
    for (uint16_t n = 0; n < repcnt; n++) {
      _spiSendByte(tx_first);
      _spiSendByte(tx_second);
    }
  }

#endif

#endif