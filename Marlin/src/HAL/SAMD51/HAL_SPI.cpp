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
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
 */

/**
 * Hardware and software SPI implementations are included in this file.
 *
 * Control of the slave select pin(s) is handled by the calling routines and
 * SAMD51 let hardware SPI handling to remove SS from its logic.
 */

#ifdef __SAMD51__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if EITHER(SOFTWARE_SPI, FORCE_SOFT_SPI)

  // ------------------------
  // Software SPI
  // ------------------------
  #error "Software SPI not supported for SAMD51. Use Hardware SPI."

#else // !SOFTWARE_SPI

  #ifdef ADAFRUIT_GRAND_CENTRAL_M4
    #if SD_CONNECTION_IS(ONBOARD)
      #define sdSPI SDCARD_SPI
    #else
      #define sdSPI SPI
    #endif
  #endif

  static SPISettings spiConfig;

  static uint32_t _spi_clock;
  static int _spi_bitOrder;
  static int _spi_clockMode;

  // ------------------------
  // Hardware SPI
  // https://github.com/arduino/ArduinoCore-samd/blob/master/libraries/SPI/SPI.h
  // ------------------------
  void spiBegin() {
  }

  void spiInitEx(uint32_t clock, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    _spi_clock = clock;
    _spi_bitOrder = MSBFIRST;
    _spi_clockMode = SPI_MODE0;
    spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
    sdSPI.begin();
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Ignore all pin hints.
    if (spiRate == SPI_SPEED_DEFAULT) spiRate = SPI_HALF_SPEED;

    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:      clock = 8000000; break;
      case SPI_HALF_SPEED:      clock = 4000000; break;
      case SPI_QUARTER_SPEED:   clock = 2000000; break;
      case SPI_EIGHTH_SPEED:    clock = 1000000; break;
      case SPI_SIXTEENTH_SPEED: clock =  500000; break;
      case SPI_SPEED_5:         clock =  250000; break;
      case SPI_SPEED_6:         clock =  125000; break;
      default:                  clock = 4000000; break; // Default from the SPI library
    }
    spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  void spiClose() { sdSPI.end(); }

  void spiSetBitOrder(int bitOrder) {
    if (bitOrder == SPI_BITORDER_MSB) {
      _spi_bitOrder = MSBFIRST;
    }
    else if (bitOrder == SPI_BITORDER_LSB) {
      _spi_bitOrder = LSBFIRST;
    }
    else return;

    spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clockMode);
  }

  void spiSetClockMode(int clockMode) {
    if (clockMode == SPI_CLKMODE_0)
      _spi_clockMode = SPI_MODE0;
    else if (clockMode == SPI_CLKMODE_1)
      _spi_clockMode = SPI_MODE1;
    else if (clockMode == SPI_CLKMODE_2)
      _spi_clockMode = SPI_MODE2;
    else if (clockMode == SPI_CLKMODE_3)
      _spi_clockMode = SPI_MODE3;
    else return;

    spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clockMode);
  }

  void spiEstablish() { /* do nothing */ }

  /**
   * @brief  Receives a single byte from the SPI port.
   *
   * @return Byte received
   *
   * @details
   */
  uint8_t spiRec(uint8_t txval) {
    sdSPI.beginTransaction(spiConfig);
    uint8_t returnByte = sdSPI.transfer(txval);
    sdSPI.endTransaction();
    return returnByte;
  }

  uint16_t spiRec16(uint16_t txval) {
    sdSPI.beginTransaction(spiConfig);
    uint16_t res = sdSPI.transfer16(txval);
    sdSPI.endTransaction();
    return res;

  /**
   * @brief  Receives a number of bytes from the SPI port to a buffer
   *
   * @param  buf   Pointer to starting address of buffer to write to.
   * @param  nbyte Number of bytes to receive.
   * @return Nothing
   */
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    if (nbyte == 0) return;
    memset(buf, txval, nbyte);
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(buf, nbyte);
    sdSPI.endTransaction();
  }

  /**
   * @brief  Sends a single byte on SPI port
   *
   * @param  b Byte to send
   *
   * @details
   */
  void spiSend(uint8_t b) {
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(b);
    sdSPI.endTransaction();
  }

  void spiSend16(uint16_t v) {
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer16(v);
    sdSPI.endTransaction();
  }

  /**
   * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
   *
   * @param  buf   Pointer with buffer start address
   * @return Nothing
   *
   * @details Uses DMA
   */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(token);
    sdSPI.transfer(buf, nullptr, 512);
    sdSPI.endTransaction();
  }

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    sdSPI.beginTransaction(spiConfig);
    for (uint16_t n = 0; n < cnt; n++)
      sdSPI.transfer(buf[n]);
    sdSPI.endTransaction();
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    sdSPI.beginTransaction(spiConfig);
    for (uint16_t n = 0; n < cnt; n++)
      sdSPI.transfer16(buf[n]);
    sdSPI.endTransaction();
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    sdSPI.beginTransaction(spiConfig);
    for (uint16_t n = 0; n < repcnt; n++)
      sdSPI.transfer(val);
    sdSPI.endTransaction();
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    sdSPI.beginTransaction(spiConfig);
    for (uint16_t n = 0; n < repcnt; n++)
      sdSPI.transfer16(val);
    sdSPI.endTransaction();
  }
#endif // !SOFTWARE_SPI

#endif // __SAMD51__
