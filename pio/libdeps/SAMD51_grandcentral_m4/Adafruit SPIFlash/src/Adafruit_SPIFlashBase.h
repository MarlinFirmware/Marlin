/**
 * @file Adafruit_QSPI_Flash.h
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach and Dean Miller for Adafruit Industries LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef ADAFRUIT_SPIFLASHBASE_H_
#define ADAFRUIT_SPIFLASHBASE_H_

#include "Adafruit_FlashTransport.h"
#include "flash_devices.h"

// An easy to use interface for working with Flash memory.
//
// If you are managing allocation of the Flash space yourself, this is the
// class to use as it take very little RAM.
class Adafruit_SPIFlashBase {
public:
  Adafruit_SPIFlashBase();
  Adafruit_SPIFlashBase(Adafruit_FlashTransport *transport);
  ~Adafruit_SPIFlashBase() {}

  bool begin(SPIFlash_Device_t const *flash_devs = NULL, size_t count = 1);
  bool end(void);

  void setIndicator(int pin, bool state_on = true);

  uint32_t numPages(void);
  uint16_t pageSize(void);

  uint32_t size(void);

  uint8_t readStatus(void);
  uint8_t readStatus2(void);
  void waitUntilReady(void);
  bool writeEnable(void);
  bool writeDisable(void);

  uint32_t getJEDECID(void);

  uint32_t readBuffer(uint32_t address, uint8_t *buffer, uint32_t len);
  uint32_t writeBuffer(uint32_t address, uint8_t const *buffer, uint32_t len);

  bool eraseSector(uint32_t sectorNumber);
  bool eraseBlock(uint32_t blockNumber);
  bool eraseChip(void);

  // Helper
  uint8_t read8(uint32_t addr);
  uint16_t read16(uint32_t addr);
  uint32_t read32(uint32_t addr);

protected:
  Adafruit_FlashTransport *_trans;
  SPIFlash_Device_t const *_flash_dev;

  int _ind_pin;
  bool _ind_active;

  void _indicator_on(void) {
    if (_ind_pin >= 0) {
      digitalWrite(_ind_pin, _ind_active ? HIGH : LOW);
    }
  }

  void _indicator_off(void) {
    if (_ind_pin >= 0) {
      digitalWrite(_ind_pin, _ind_active ? LOW : HIGH);
    }
  }
};

// for debugging
#define SPIFLASH_DEBUG 0

#endif /* ADAFRUIT_SPIFLASHBASE_H_ */
