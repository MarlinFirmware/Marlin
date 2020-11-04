/***********
 * spi.cpp *
 ***********/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "ftdi_basic.h"

#ifdef FTDI_BASIC

/********************************* SPI Functions *********************************/

namespace FTDI {

  #ifndef CLCD_USE_SOFT_SPI
    #ifdef CLCD_SPI_BUS
      SPIClass EVE_SPI(CLCD_SPI_BUS);
    #endif
    #ifndef CLCD_HW_SPI_SPEED
      #define CLCD_HW_SPI_SPEED 8000000 >> SPI_SPEED
    #endif
    SPISettings SPI::spi_settings(CLCD_HW_SPI_SPEED, MSBFIRST, SPI_MODE0);
  #endif

  void SPI::spi_init() {
    SET_OUTPUT(CLCD_MOD_RESET); // Module Reset (a.k.a. PD, not SPI)
    WRITE(CLCD_MOD_RESET, 0); // start with module in power-down

    SET_OUTPUT(CLCD_SPI_CS);
    WRITE(CLCD_SPI_CS, 1);

    #ifdef CLCD_SPI_EXTRA_CS
      SET_OUTPUT(CLCD_SPI_EXTRA_CS);
      WRITE(CLCD_SPI_EXTRA_CS, 1);
    #endif

    #ifdef SPI_FLASH_SS
      SET_OUTPUT(SPI_FLASH_SS);
      WRITE(SPI_FLASH_SS, 1);
    #endif

    #ifdef CLCD_USE_SOFT_SPI
      SET_OUTPUT(CLCD_SOFT_SPI_MOSI);
      WRITE(CLCD_SOFT_SPI_MOSI, 1);

      SET_OUTPUT(CLCD_SOFT_SPI_SCLK);
      WRITE(CLCD_SOFT_SPI_SCLK, 0);

      SET_INPUT_PULLUP(CLCD_SOFT_SPI_MISO);
    #else
      SPI_OBJ.begin();
    #endif
  }

  #ifdef CLCD_USE_SOFT_SPI
    uint8_t SPI::_soft_spi_xfer(uint8_t spiOutByte) {
      uint8_t spiIndex  = 0x80;
      uint8_t spiInByte = 0;
      uint8_t k;

      noInterrupts();
      for (k = 0; k < 8; k++) {  // Output and Read each bit of spiOutByte and spiInByte
        WRITE(CLCD_SOFT_SPI_MOSI, (spiOutByte & spiIndex) ? 1 : 0); // Output MOSI Bit
        WRITE(CLCD_SOFT_SPI_SCLK, 1);   // Pulse Clock
        if (READ(CLCD_SOFT_SPI_MISO)) spiInByte |= spiIndex; // MISO changes on the falling edge of clock, so sample it before
        WRITE(CLCD_SOFT_SPI_SCLK, 0);
        spiIndex >>= 1;
      }
      interrupts();
      return spiInByte;
    }
  #endif

  #ifdef CLCD_USE_SOFT_SPI
    void SPI::_soft_spi_send(uint8_t spiOutByte) {
      uint8_t k, spiIndex  = 0x80;

      noInterrupts();
      for (k = 0; k < 8; k++) {         // Output each bit of spiOutByte
        WRITE(CLCD_SOFT_SPI_MOSI, (spiOutByte & spiIndex) ? 1 : 0); // Output MOSI Bit
        WRITE(CLCD_SOFT_SPI_SCLK, 1);   // Pulse Clock
        WRITE(CLCD_SOFT_SPI_SCLK, 0);
        spiIndex >>= 1;
      }
      interrupts();
    }
  #endif

  void SPI::spi_read_bulk(void *data, uint16_t len) {
    uint8_t* p = (uint8_t *)data;
    while (len--) *p++ = spi_recv();
  }

  bool SPI::spi_verify_bulk(const void *data, uint16_t len) {
    const uint8_t* p = (const uint8_t *)data;
    while (len--) if (*p++ != spi_recv()) return false;
    return true;
  }

  // CLCD SPI - Chip Select
  void SPI::spi_ftdi_select() {
    #ifndef CLCD_USE_SOFT_SPI
      SPI_OBJ.beginTransaction(spi_settings);
    #endif
    WRITE(CLCD_SPI_CS, 0);
    #ifdef CLCD_SPI_EXTRA_CS
      WRITE(CLCD_SPI_EXTRA_CS, 0);
    #endif
    delayMicroseconds(1);
  }

  // CLCD SPI - Chip Deselect
  void SPI::spi_ftdi_deselect() {
    WRITE(CLCD_SPI_CS, 1);
    #ifdef CLCD_SPI_EXTRA_CS
      WRITE(CLCD_SPI_EXTRA_CS, 1);
    #endif
    #ifndef CLCD_USE_SOFT_SPI
      SPI_OBJ.endTransaction();
    #endif
  }

  #ifdef SPI_FLASH_SS
  // Serial SPI Flash SPI - Chip Select
  void SPI::spi_flash_select() {
    #ifndef CLCD_USE_SOFT_SPI
      SPI_OBJ.beginTransaction(spi_settings);
    #endif
    WRITE(SPI_FLASH_SS, 0);
    delayMicroseconds(1);
  }

  // Serial SPI Flash SPI - Chip Deselect
  void SPI::spi_flash_deselect() {
    WRITE(SPI_FLASH_SS, 1);
    #ifndef CLCD_USE_SOFT_SPI
      SPI_OBJ.endTransaction();
    #endif
  }
  #endif

  // Not really a SPI signal...
  void SPI::ftdi_reset() {
    WRITE(CLCD_MOD_RESET, 0);
    delay(6); /* minimum time for power-down is 5ms */
    WRITE(CLCD_MOD_RESET, 1);
    delay(21); /* minimum time to allow from rising PD_N to first access is 20ms */
  }

  // Not really a SPI signal...
  void SPI::test_pulse() {
    #ifdef CLCD_AUX_0
      WRITE(CLCD_AUX_0, 1);
      delayMicroseconds(10);
      WRITE(CLCD_AUX_0, 0);
    #endif
  }
}
#endif // FTDI_BASIC
