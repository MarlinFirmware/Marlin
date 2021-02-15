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

#ifndef ADAFRUIT_FLASHTRANSPORT_SPI_H_
#define ADAFRUIT_FLASHTRANSPORT_SPI_H_

#include "Arduino.h"
#include "SPI.h"

class Adafruit_FlashTransport_SPI : public Adafruit_FlashTransport {
private:
  SPIClass *_spi;
  uint8_t _ss;

  // SAMD21 M0 can write up to 24 Mhz, but can only read reliably with 12 MHz
  uint32_t _clock_wr;
  uint32_t _clock_rd;

public:
  Adafruit_FlashTransport_SPI(uint8_t ss, SPIClass *spiinterface);
  Adafruit_FlashTransport_SPI(uint8_t ss, SPIClass &spiinterface);

  virtual void begin(void);

  virtual bool supportQuadMode(void) { return false; }

  virtual void setClockSpeed(uint32_t write_hz, uint32_t read_hz);

  virtual bool runCommand(uint8_t command);
  virtual bool readCommand(uint8_t command, uint8_t *response, uint32_t len);
  virtual bool writeCommand(uint8_t command, uint8_t const *data, uint32_t len);
  virtual bool eraseCommand(uint8_t command, uint32_t addr);

  virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t len);
  virtual bool writeMemory(uint32_t addr, uint8_t const *data, uint32_t len);

private:
  void fillAddress(uint8_t *buf, uint32_t addr);

  void beginTransaction(uint32_t clock_hz) {
    _spi->beginTransaction(SPISettings(clock_hz, MSBFIRST, SPI_MODE0));
    digitalWrite(_ss, LOW);
  }

  void endTransaction(void) {
    digitalWrite(_ss, HIGH);
    _spi->endTransaction();
  }
};

#endif /* ADAFRUIT_FLASHTRANSPORT_SPI_H_ */
