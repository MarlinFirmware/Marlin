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
#pragma once

#include "HAL.h"
#include <SPI.h>

extern "C" {
  #include <utility/spi_com.h>
}

/**
 * Marlin currently requires 3 SPI classes:
 *
 * SPIClass:
 *  This class is normally provided by frameworks and has a semi-default interface.
 *  This is needed because some libraries reference it globally.
 *
 * SPISettings:
 *  Container for SPI configs for SPIClass. As above, libraries may reference it globally.
 *
 * These two classes are often provided by frameworks so we cannot extend them to add
 * useful methods for Marlin.
 *
 * MarlinSPI:
 *  Provides the default SPIClass interface plus some Marlin goodies such as a simplified
 *  interface for SPI DMA transfer.
 *
 */

#define DATA_SIZE_8BIT SPI_DATASIZE_8BIT
#define DATA_SIZE_16BIT SPI_DATASIZE_16BIT

class MarlinSPI {
public:
  MarlinSPI() : MarlinSPI(NC, NC, NC, NC) {}

  MarlinSPI(pin_t mosi, pin_t miso, pin_t sclk, pin_t ssel = (pin_t)NC) : _mosiPin(mosi), _misoPin(miso), _sckPin(sclk), _ssPin(ssel) {
    _spi.pin_miso = digitalPinToPinName(_misoPin);
    _spi.pin_mosi = digitalPinToPinName(_mosiPin);
    _spi.pin_sclk = digitalPinToPinName(_sckPin);
    _spi.pin_ssel = digitalPinToPinName(_ssPin);
    _dataSize = DATA_SIZE_8BIT;
    _bitOrder = MSBFIRST;
    _dataMode = SPI_MODE_0;
    _spi.handle.State = HAL_SPI_STATE_RESET;
    setClockDivider(SPI_SPEED_CLOCK_DIV2_MHZ);
  }

  void begin(void);
  void end(void) {}

  byte transfer(uint8_t _data);
  uint8_t dmaTransfer(const void *transmitBuf, void *receiveBuf, uint16_t length);
  uint8_t dmaSend(const void * transmitBuf, uint16_t length, bool minc = true);

  /* These methods are deprecated and kept for compatibility.
   * Use SPISettings with SPI.beginTransaction() to configure SPI parameters.
   */
  void setBitOrder(BitOrder _order) { _bitOrder = _order; }

  void setDataMode(uint8_t _mode) {
    switch (_mode) {
      case SPI_MODE0: _dataMode = SPI_MODE_0; break;
      case SPI_MODE1: _dataMode = SPI_MODE_1; break;
      case SPI_MODE2: _dataMode = SPI_MODE_2; break;
      case SPI_MODE3: _dataMode = SPI_MODE_3; break;
    }
  }

  void setClockDivider(uint8_t _div);

private:
  void setupDma(SPI_HandleTypeDef &_spiHandle, DMA_HandleTypeDef &_dmaHandle, uint32_t direction, bool minc = false);

  spi_t _spi;
  DMA_HandleTypeDef _dmaTx;
  DMA_HandleTypeDef _dmaRx;
  BitOrder _bitOrder;
  spi_mode_e _dataMode;
  uint8_t _clockDivider;
  uint32_t _speed;
  uint32_t _dataSize;
  pin_t _mosiPin;
  pin_t _misoPin;
  pin_t _sckPin;
  pin_t _ssPin;
};
