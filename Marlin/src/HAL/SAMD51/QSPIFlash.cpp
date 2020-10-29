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

#include "../../inc/MarlinConfig.h"

#if ENABLED(QSPI_EEPROM)

#include "QSPIFlash.h"

#define INVALID_ADDR    0xFFFFFFFF
#define SECTOR_OF(a)    (a & ~(SFLASH_SECTOR_SIZE - 1))
#define OFFSET_OF(a)    (a & (SFLASH_SECTOR_SIZE - 1))

Adafruit_SPIFlashBase * QSPIFlash::_flashBase = nullptr;
uint8_t QSPIFlash::_buf[SFLASH_SECTOR_SIZE];
uint32_t QSPIFlash::_addr = INVALID_ADDR;

void QSPIFlash::begin() {
  if (_flashBase) return;

  _flashBase = new Adafruit_SPIFlashBase(new Adafruit_FlashTransport_QSPI());
  _flashBase->begin(nullptr);
}

size_t QSPIFlash::size() {
  return _flashBase->size();
}

uint8_t QSPIFlash::readByte(const uint32_t address) {
  if (SECTOR_OF(address) == _addr) return _buf[OFFSET_OF(address)];

  return _flashBase->read8(address);
}

void QSPIFlash::writeByte(const uint32_t address, const uint8_t value) {
  uint32_t const sector_addr = SECTOR_OF(address);

  // Page changes, flush old and update new cache
  if (sector_addr != _addr) {
    flush();
    _addr = sector_addr;

    // read a whole page from flash
    _flashBase->readBuffer(sector_addr, _buf, SFLASH_SECTOR_SIZE);
  }

  _buf[OFFSET_OF(address)] = value;
}

void QSPIFlash::flush() {
  if (_addr == INVALID_ADDR) return;

  _flashBase->eraseSector(_addr / SFLASH_SECTOR_SIZE);
  _flashBase->writeBuffer(_addr, _buf, SFLASH_SECTOR_SIZE);

  _addr = INVALID_ADDR;
}

#endif // QSPI_EEPROM
