/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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
#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

// ------------------------
// Public functions
// ------------------------

  // If properly ported, use fast HW SPI implementation originally found in STM32 tft_spi.cpp
#if !ENABLED(SOFTWARE_SPI) && (!(defined(STM32F1xx) || defined(STM32F4xx)) || ENABLED(HALSPI_HW_GENERIC))

  #include <SPI.h>

  /**
   * VGPV SPI speed start and PCLK2/2, by default 108/2 = 54Mhz
   */

  static SPISettings spiConfig;

  /**
   * @brief  Begin SPI port setup
   *
   * @return Nothing
   *
   * @details Only configures SS pin since stm32duino creates and initialize the SPI object
   */
  void spiBegin() {
    #if PIN_EXISTS(SD_SS)
      OUT_WRITE(SD_SS_PIN, HIGH);
    #endif
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    _spi_clock = maxClockFreq;
    spiConfig = SPISettings(maxClockFreq, MSBFIRST, SPI_MODE0);

    SPI.setMISO((hint_miso != -1) ? hint_miso : SD_MISO_PIN);
    SPI.setMOSI((hint_mosi != -1) ? hint_mosi : SD_MOSI_PIN);
    SPI.setSCLK((hint_sck != -1) ? hint_sck : SD_SCK_PIN);

    SPI.begin();
    SPI.beginTransaction(spiConfig);
  }

  // Configure SPI for specified SPI speed
  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Ignore chip-select because the software manages it already.
    
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

  void spiClose() {
    // Terminates SPI activity.
    SPI.endTransaction();
    SPI.end();
  }

  void spiSetBitOrder(int bitOrder) {
    if (bitOrder == SPI_BITORDER_MSB) {
      SPI.setBitOrder(MSBFIRST);
    }
    else if (bitOrder == SPI_BITORDER_LSB) {
      SPI.setBitOrder(LSBFIRST);
    }
  }

  void spiSetClockMode(int clockMode) {
    if (clockMode == SPI_CLKMODE_0)
      SPI.setDataMode(SPI_MODE0);
    else if (clockMode == SPI_CLKMODE_1)
      SPI.setDataMode(SPI_MODE1);
    else if (clockMode == SPI_CLKMODE_2)
      SPI.setDataMode(SPI_MODE2);
    else if (clockMode == SPI_CLKMODE_3)
      SPI.setDataMode(SPI_MODE3);
  }

  /**
   * @brief  Receives a single byte from the SPI port.
   *
   * @return Byte received
   *
   * @details
   */
  uint8_t spiRec(uint8_t txval) {
    uint8_t returnByte = SPI.transfer(txval);
    return returnByte;
  }

  uint16_t spiRec16(uint16_t txval) {
    return SPI.transfer16(txval);
  }

  /**
   * @brief  Receive a number of bytes from the SPI port to a buffer
   *
   * @param  buf   Pointer to starting address of buffer to write to.
   * @param  nbyte Number of bytes to receive.
   * @return Nothing
   *
   * @details Uses DMA
   */
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    if (nbyte == 0) return;
    memset(buf, txval, nbyte);
    SPI.transfer(buf, nbyte);
  }

  /**
   * @brief  Send a single byte on SPI port
   *
   * @param  b Byte to send
   *
   * @details
   */
  void spiSend(uint8_t b) {
    SPI.transfer(b);
  }

  void spiSend16(uint16_t v) {
    SPI.transfer16(v);
  }

  void spiWrite(const uint8_t *buf, uint16_t numbytes) {
    void *inout_buf = malloc(numbytes);
    if (inout_buf == nullptr)
      _spi_on_error();
    memcpy(inout_buf, buf, numbytes);
    // Generic transfer, non-DMA.
    SPI.transfer(inout_buf, numbytes);
    free(inout_buf);
  }

  void spiWrite16(const uint16_t *buf, uint16_t numtx) {
    for (uint32_t n = 0; n < numbytes; n++) {
      SPI.transfer16(buf[n]);
    }
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      SPI.transfer(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      SPI.transfer16(val);
  }

  /**
   * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
   *
   * @param  buf   Pointer with buffer start address
   * @return Nothing
   *
   * @details Use DMA
   */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    uint8_t rxBuf[512];
    SPI.transfer(token);
    SPI.transfer((uint8_t*)buf, rxBuf, 512);
  }

#endif // !FAST HW SPI, SOFTWARE_SPI

#endif // HAL_STM32
