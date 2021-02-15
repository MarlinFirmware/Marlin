/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 hathach for Adafruit Industries
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

#include "Adafruit_FlashTransport.h"

Adafruit_FlashTransport_SPI::Adafruit_FlashTransport_SPI(
    uint8_t ss, SPIClass *spiinterface) {
  _cmd_read = SFLASH_CMD_READ;
  _addr_len = 3; // work with most device if not set
  _ss = ss;
  _spi = spiinterface;
  _clock_wr = _clock_rd = 4000000;
}

Adafruit_FlashTransport_SPI::Adafruit_FlashTransport_SPI(uint8_t ss,
                                                         SPIClass &spiinterface)
    : Adafruit_FlashTransport_SPI(ss, &spiinterface) {}

void Adafruit_FlashTransport_SPI::begin(void) {
  pinMode(_ss, OUTPUT);
  digitalWrite(_ss, HIGH);

  _spi->begin();
}

void Adafruit_FlashTransport_SPI::setClockSpeed(uint32_t write_hz,
                                                uint32_t read_hz) {
  _clock_wr = write_hz;
  _clock_rd = read_hz;
}

bool Adafruit_FlashTransport_SPI::runCommand(uint8_t command) {
  beginTransaction(_clock_wr);

  _spi->transfer(command);

  endTransaction();

  return true;
}

bool Adafruit_FlashTransport_SPI::readCommand(uint8_t command,
                                              uint8_t *response, uint32_t len) {
  beginTransaction(_clock_rd);

  _spi->transfer(command);
  while (len--) {
    *response++ = _spi->transfer(0xFF);
  }

  endTransaction();

  return true;
}

bool Adafruit_FlashTransport_SPI::writeCommand(uint8_t command,
                                               uint8_t const *data,
                                               uint32_t len) {
  beginTransaction(_clock_wr);

  _spi->transfer(command);
  while (len--) {
    (void)_spi->transfer(*data++);
  }

  endTransaction();

  return true;
}

bool Adafruit_FlashTransport_SPI::eraseCommand(uint8_t command, uint32_t addr) {
  beginTransaction(_clock_wr);

  uint8_t cmd_with_addr[5] = {command};
  fillAddress(cmd_with_addr + 1, addr);

  _spi->transfer(cmd_with_addr, 1 + _addr_len);

  endTransaction();

  return true;
}

void Adafruit_FlashTransport_SPI::fillAddress(uint8_t *buf, uint32_t addr) {
  switch (_addr_len) {
  case 4:
    *buf++ = (addr >> 24) & 0xFF;
    //__attribute__((fallthrough)); ESP32 doesn't support this attribute yet
    // fall through

  case 3:
    *buf++ = (addr >> 16) & 0xFF;
    //__attribute__((fallthrough)); ESP32 doesn't support this attribute yet
    // fall through

  case 2:
  default:
    *buf++ = (addr >> 8) & 0xFF;
    *buf++ = addr & 0xFF;
  }
}

bool Adafruit_FlashTransport_SPI::readMemory(uint32_t addr, uint8_t *data,
                                             uint32_t len) {
  beginTransaction(_clock_rd);

  uint8_t cmd_with_addr[6] = {_cmd_read};
  fillAddress(cmd_with_addr + 1, addr);

  // Fast Read has 1 extra dummy byte
  uint8_t const cmd_len =
      1 + _addr_len + (SFLASH_CMD_FAST_READ == _cmd_read ? 1 : 0);

  _spi->transfer(cmd_with_addr, cmd_len);

  // Use SPI DMA if available for best performance
#if defined(ARDUINO_NRF52_ADAFRUIT) && defined(NRF52840_XXAA)
  _spi->transfer(NULL, data, len);
#elif defined(ARDUINO_ARCH_SAMD) && defined(_ADAFRUIT_ZERODMA_H_)
  _spi->transfer(NULL, data, len, true);
#else
  _spi->transfer(data, len);
#endif

  endTransaction();

  return true;
}

bool Adafruit_FlashTransport_SPI::writeMemory(uint32_t addr,
                                              uint8_t const *data,
                                              uint32_t len) {
  beginTransaction(_clock_wr);

  uint8_t cmd_with_addr[5] = {SFLASH_CMD_PAGE_PROGRAM};
  fillAddress(cmd_with_addr + 1, addr);

  _spi->transfer(cmd_with_addr, 1 + _addr_len);

  // Use SPI DMA if available for best performance
#if defined(ARDUINO_NRF52_ADAFRUIT) && defined(NRF52840_XXAA)
  _spi->transfer(data, NULL, len);
#elif defined(ARDUINO_ARCH_SAMD) && defined(_ADAFRUIT_ZERODMA_H_)
  _spi->transfer(data, NULL, len, true);
#else
  while (len--) {
    _spi->transfer(*data++);
  }
#endif

  endTransaction();

  return true;
}
