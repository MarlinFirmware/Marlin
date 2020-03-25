#include "../../inc/MarlinConfig.h"

#if ALL(EEPROM_SETTINGS, ONBOARD_SPI_EEPROM)

#include "QSPIFlash.h"

#define INVALID_ADDR    0xffffffff
#define SECTOR_OF(a)    (a & ~(SFLASH_SECTOR_SIZE - 1))
#define OFFSET_OF(a)    (a & (SFLASH_SECTOR_SIZE - 1))

Adafruit_SPIFlashBase * QSPIFlash::_flashBase = nullptr;
uint8_t QSPIFlash::_buf[SFLASH_SECTOR_SIZE];
uint32_t QSPIFlash::_addr = INVALID_ADDR;

void QSPIFlash::begin() {
  if (_flashBase != nullptr) return;

  _flashBase = new Adafruit_SPIFlashBase(new Adafruit_FlashTransport_QSPI());
  _flashBase->begin(NULL);
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

#endif // EEPROM_SETTINGS, FLASH_EEPROM_EMULATION
