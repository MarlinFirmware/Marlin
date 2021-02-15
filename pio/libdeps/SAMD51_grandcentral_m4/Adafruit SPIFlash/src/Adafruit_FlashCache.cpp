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

#include "Adafruit_SPIFlash.h"

#if SPIFLASH_DEBUG
#define SPICACHE_LOG(_new_addr)                                                \
  do {                                                                         \
    Serial.print(__FUNCTION__);                                                \
    Serial.print(": flush sector = ");                                         \
    Serial.print(_addr / 512);                                                 \
    Serial.print(", new sector = ");                                           \
    Serial.println(_new_addr / 512);                                           \
  } while (0)
#else
#define SPICACHE_LOG(_new_addr)
#endif

#define INVALID_ADDR 0xffffffff

static inline uint32_t sector_of(uint32_t addr) {
  return addr & ~(SFLASH_SECTOR_SIZE - 1);
}

static inline uint32_t offset_of(uint32_t addr) {
  return addr & (SFLASH_SECTOR_SIZE - 1);
}

Adafruit_FlashCache::Adafruit_FlashCache(void) { _addr = INVALID_ADDR; }

bool Adafruit_FlashCache::sync(Adafruit_SPIFlash *fl) {
  if (_addr == INVALID_ADDR)
    return true;

  fl->eraseSector(_addr / SFLASH_SECTOR_SIZE);
  fl->writeBuffer(_addr, _buf, SFLASH_SECTOR_SIZE);

  _addr = INVALID_ADDR;

  return true;
}

bool Adafruit_FlashCache::write(Adafruit_SPIFlash *fl, uint32_t address,
                                void const *src, uint32_t len) {
  uint8_t const *src8 = (uint8_t const *)src;
  uint32_t remain = len;

  // Program up to sector boundary each loop
  while (remain) {
    uint32_t const sector_addr = sector_of(address);
    uint32_t const offset = offset_of(address);

    uint32_t wr_bytes = SFLASH_SECTOR_SIZE - offset;
    wr_bytes = min(remain, wr_bytes);

    // Page changes, flush old and update new cache
    if (sector_addr != _addr) {
      SPICACHE_LOG(sector_addr);
      this->sync(fl);
      _addr = sector_addr;

      // read a whole page from flash
      fl->readBuffer(sector_addr, _buf, SFLASH_SECTOR_SIZE);
    }

    memcpy(_buf + offset, src8, wr_bytes);

    // adjust for next run
    src8 += wr_bytes;
    remain -= wr_bytes;
    address += wr_bytes;
  }

  return true;
}

bool Adafruit_FlashCache::read(Adafruit_SPIFlash *fl, uint32_t address,
                               uint8_t *buffer, uint32_t count) {
  // overwrite with cache value if available
  if ((_addr != INVALID_ADDR) &&
      !(address < _addr && address + count <= _addr) &&
      !(address >= _addr + SFLASH_SECTOR_SIZE)) {
    int32_t dst_off = _addr - address;
    int32_t src_off = 0;

    if (dst_off < 0) {
      src_off = -dst_off;
      dst_off = 0;
    }

    int32_t cache_bytes = min((int32_t)(SFLASH_SECTOR_SIZE - src_off),
                              (int32_t)(count - dst_off));

    // start to cached
    if (dst_off) {
      fl->readBuffer(address, buffer, dst_off);
    }

    // cached
    memcpy(buffer + dst_off, _buf + src_off, cache_bytes);

    // cached to end
    uint32_t copied = (uint32_t)(dst_off + cache_bytes);
    if (copied < count) {
      fl->readBuffer(address + copied, buffer + copied, count - copied);
    }
  } else {
    fl->readBuffer(address, buffer, count);
  }

  return true;
}
