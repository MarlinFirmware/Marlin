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

/**
 * Arduino-style SPI API for Atmel SAM3U (AT91SAM3U4E)
 *
 * Marlin core code (e.g. module/stepper/control.cpp, which drives the 4pi's
 * AD5206 digipot) includes <SPI.h> and uses a global `SPI` object. There is no
 * Arduino core here, so this provides that surface on top of the bit-banged
 * master in HAL/SAM3U/HAL_SPI.cpp.
 */

#include <stdint.h>
#include <stddef.h>

/**
 * Name collision: the CMSIS device header defines SPI as a macro for the SPI
 * peripheral's base address, which would mangle the `SPI` object below. Pull
 * that header in first (via pinmapping.h) so its include guard is set, keep
 * the base address under a name of our own, then take the macro out of the
 * way. Any later #include of sam3u4e.h is a no-op, so the macro stays gone.
 */
#include "pinmapping.h"

#ifdef SPI
  #define SAM3U_SPI_REGS ((Spi *)0x40008000U)   // The peripheral, if ever needed
  #undef SPI
#endif

#include "../../shared/HAL_SPI.h"

// Full-duplex single-byte exchange, implemented in HAL_SPI.cpp
uint8_t spiTransfer(uint8_t b);

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

class SPISettings {
public:
  SPISettings(const uint32_t clock=4000000, const uint8_t bitOrder=MSBFIRST, const uint8_t dataMode=SPI_MODE0)
    : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
  uint32_t clock() const   { return _clock; }
  uint8_t bitOrder() const { return _bitOrder; }
  uint8_t dataMode() const { return _dataMode; }
private:
  uint32_t _clock;
  uint8_t _bitOrder, _dataMode;
};

class SPIClass {
public:
  void begin() { spiBegin(); }
  void end()   {}

  void beginTransaction(const SPISettings &s) { spiBeginTransaction(s.clock(), s.bitOrder(), s.dataMode()); }
  void endTransaction() {}

  uint8_t transfer(const uint8_t data) { return spiTransfer(data); }

  void transfer(void *buf, const size_t count) {
    uint8_t * const p = (uint8_t *)buf;
    for (size_t i = 0; i < count; i++) p[i] = spiTransfer(p[i]);
  }

  void setClockDivider(const uint8_t rate) { spiInit(rate); }
  void setBitOrder(const uint8_t) {}    // Always MSB first
  void setDataMode(const uint8_t) {}    // Always mode 0
};

extern SPIClass SPI;
