/**
 * @file QSPIFlash.h
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
 *
 * Derived from Adafruit_SPIFlash class with no SdFat references
 */
#pragma once

#include <Adafruit_SPIFlashBase.h>

// This class extends Adafruit_SPIFlashBase by adding caching support.
//
// This class will use 4096 Bytes of RAM as a block cache.
class QSPIFlash {
  public:
    static void begin();
    static size_t size();
    static uint8_t readByte(const uint32_t address);
    static void writeByte(const uint32_t address, const uint8_t v);
    static void flush();

  private:
    static Adafruit_SPIFlashBase * _flashBase;
    static uint8_t _buf[SFLASH_SECTOR_SIZE];
    static uint32_t _addr;
};

extern QSPIFlash qspi;
