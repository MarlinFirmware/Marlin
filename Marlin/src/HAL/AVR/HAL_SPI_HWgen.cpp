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
 * HAL for AVR - SPI functions
 */

#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

#if NONE(SOFTWARE_SPI, FORCE_SOFT_SPI) && ENABLED(HALSPI_HW_GENERIC)

#include <SPI.h>

#include "registers.h"

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
    // (we only support master SPI)
    SET_OUTPUT(SD_SCK_PIN);
    SET_INPUT(SD_MISO_PIN);
    SET_OUTPUT(SD_MOSI_PIN);
  }

  static SPISettings _spi_config;
  static uint32_t _spi_maxClockFreq;
  static int _spi_clockMode;
  static int _spi_bitOrder;
  static bool _spi_transaction_is_running;
  static int _spi_cs_pin;

  void spiInitEx(uint32_t maxClockFreq, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    SPI.begin();

    _spi_maxClockFreq = maxClockFreq;
    _spi_clockMode = SPI_MODE0;
    _spi_bitOrder = MSBFIRST;
    _spi_config = SPISettings(_spi_maxClockFreq, _spi_bitOrder, _spi_clockMode);

    _spi_cs_pin = hint_cs;

    _spi_transaction_is_running = false;
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use Marlin datarates
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

  void spiClose() {
    if (_spi_transaction_is_running) {
      if (_spi_cs_pin >= 0)
        _ATmega_digitalWrite(_spi_cs_pin, HIGH);

      SPI.endTransaction();

      _spi_transaction_is_running = false;
    }

    SPI.end();
  }

  void spiSetBitOrder(int bitOrder) {
    int new_bitOrder;

    if (bitOrder == SPI_BITORDER_MSB) {
      new_bitOrder = MSBFIRST;
    }
    else if (bitOrder == SPI_BITORDER_LSB) {
      new_bitOrder = LSBFIRST;
    }
    else return;

    if (_spi_bitOrder == new_bitOrder) return;

    _spi_bitOrder = new_bitOrder;
    _spi_config = SPISettings(_spi_maxClockFreq, _spi_bitOrder, _spi_clockMode);

    if (_spi_transaction_is_running) {
      SPI.endTransaction();
      SPI.beginTransaction(_spi_config);
    }
  }

  void spiSetClockMode(int clockMode) {
    int new_clockMode;

    if (clockMode == SPI_CLKMODE_0) {
      new_clockMode = SPI_MODE0;
    }
    else if (clockMode == SPI_CLKMODE_1) {
      new_clockMode = SPI_MODE1;
    }
    else if (clockMode == SPI_CLKMODE_2) {
      new_clockMode = SPI_MODE2;
    }
    else if (clockMode == SPI_CLKMODE_3) {
      new_clockMode = SPI_MODE3;
    }
    else return;

    if (new_clockMode == _spi_clockMode) return;

    _spi_clockMode = new_clockMode;
    _spi_config = SPISettings(_spi_maxClockFreq, _spi_bitOrder, _spi_clockMode);

    if (_spi_transaction_is_running) {
      SPI.endTransaction();
      SPI.beginTransaction(_spi_config);
    }
  }

  static void _maybe_start_transaction() {
    if (_spi_transaction_is_running) return;

    SPI.beginTransaction(_spi_config);

    if (_spi_cs_pin >= 0) {
      _ATmega_pinMode(_spi_cs_pin, OUTPUT);
      _ATmega_digitalWrite(_spi_cs_pin, LOW);
    }

    _spi_transaction_is_running = true;
  }

  void spiEstablish() {
    _maybe_start_transaction();
  }

  void spiSend(uint8_t txval) {
    _maybe_start_transaction();
    (void)SPI.transfer(txval);
  }

  void spiSend16(uint8_t txval) {
    _maybe_start_transaction();
    (void)SPI.transfer16(txval);
  }

  uint8_t spiRec(uint8_t txval) {
    _maybe_start_transaction();
    return SPI.transfer(txval);
  }

  uint16_t spiRec16(uint16_t txval) {
    _maybe_start_transaction();
    return SPI.transfer16(txval);
  }

  void spiRead(uint8_t *buf, uint16_t rdcnt, uint8_t txval) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < rdcnt; n++) {
      buf[n] = SPI.transfer(txval);
    }
  }

  void spiSendBlock(uint8_t token, const uint8_t *block) {
    _maybe_start_transaction();
    (void)SPI.transfer(token);
    for (uint16_t n = 0; n < 512; n++) {
      (void)SPI.transfer(block[n]);
    }
  }

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < cnt; n++) {
      (void)SPI.transfer(buf[n]);
    }
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < cnt; n++) {
      (void)SPI.transfer16(buf[n]);
    }
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < repcnt; n++) {
      (void)SPI.transfer(val);
    }
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    _maybe_start_transaction();
    for (uint16_t n = 0; n < repcnt; n++) {
      (void)SPI.transfer16(val);
    }
  }

#endif

#endif //__AVR__
