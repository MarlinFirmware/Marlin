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

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "SPIFlashStorage.h"

uint8_t SPIFlashStorage::::m_pageData[SPI_FLASH_PageSize];
uint32_t SPIFlashStorage::::m_currentPage;
uint16_t SPIFlashStorage::::m_pageDataUsed;
uint32_t SPIFlashStorage::::m_startAddress;

#if HAS_SPI_FLASH_COMPRESSION

  uint8_t SPIFlashStorage::m_compressedData[SPI_FLASH_PageSize];
  uint16_t SPIFlashStorage::m_compressedDataUsed;

  template <typename T>
  static uint32_t rle_compress(T *output, uint32_t outputLength, T *input, uint32_t inputLength, uint32_t& inputProcessed) {
    uint32_t count = 0, out = 0, index, i;
    T pixel;
    //32767 for uint16_t
    //127 for uint16_t
    //calculated at compile time
    constexpr T max = (0xFFFFFFFF >> (8 * (4 - sizeof(T)))) / 2;

    inputProcessed = 0;
    while (count < inputLength && out < outputLength) {
      index = count;
      pixel = input[index++];
      while (index < inputLength && index - count < max && input[index] == pixel)
        index++;
      if (index - count == 1) {
        /*
         * Failed to "replicate" the current pixel. See how many to copy.
         * Avoid a replicate run of only 2-pixels after a literal run. There
         * is no gain in this, and there is a risK of loss if the run after
         * the two identical pixels is another literal run. So search for
         * 3 identical pixels.
         */
        while (index < inputLength && index - count < max && (input[index] != input[index - 1] || (index > 1 && input[index] != input[index - 2])))
          index++;
        /*
         * Check why this run stopped. If it found two identical pixels, reset
         * the index so we can add a run. Do this twice: the previous run
         * tried to detect a replicate run of at least 3 pixels. So we may be
         * able to back up two pixels if such a replicate run was found.
         */
        while (index < inputLength && input[index] == input[index - 1])
          index--;
        // If the output buffer could overflow, stop at the remaining bytes
        NOMORE(index, count + outputLength - out - 1);
        output[out++] = (uint16_t)(count - index);
        for (i = count; i < index; i++)
          output[out++] = input[i];
      }
      else {
        // Need at least more 2 spaces
        if (out > outputLength - 2) break;
        output[out++] = (uint16_t)(index - count);
        output[out++] = pixel;
      }
      count = index;
    }
    inputProcessed = count;

    // Padding
    if (out == outputLength - 1) output[out++] = 0;

    return out;
  }

  template <typename UT, typename T>
  static uint32_t rle_uncompress(UT *output, uint32_t outputLength, UT *input, uint32_t inputLength, uint32_t &outputFilled) {
    T count;
    UT i;
    uint32_t processedBytes = 0;
    outputFilled = 0;

    while (outputLength > 0 && inputLength > 0) {
      processedBytes++;
      count = static_cast<T>(*input++);
      inputLength--;
      if (count > 0) { // Replicate run
        for (i = 0; i < count && outputLength > i; i++)
          output[i] = *input;
        outputFilled += i;
        // If copy incomplete, change the input buffer to start with remaining data in the next call
        if (i < count) {
          // Change to process the difference in the next call
          *(input - 1) = static_cast<UT>(count - i);
          return processedBytes - 1;
        }
        input++;
        inputLength--;
        processedBytes++;
      }
      else if (count < 0) { // literal run
        count = static_cast<T>(-count);
        // Copy, validating if the output have enough space
        for (i = 0; i < count && outputLength > i; i++)
          output[i] = input[i];
        outputFilled += i;
        // If copy incomplete, change the input buffer to start with remaining data in the next call
        if (i < count) {
          input[i - 1] = static_cast<UT>((count - i) * -1);
          // Back one
          return processedBytes + i - 1;
        }
        input += count;
        inputLength -= count;
        processedBytes += count;
      }
      output += count;
      outputLength -= count;
    }

    return processedBytes;
  }

#endif // HAS_SPI_FLASH_COMPRESSION

void SPIFlashStorage::beginWrite(uint32_t startAddress) {
  m_pageDataUsed = 0;
  m_currentPage = 0;
  m_startAddress = startAddress;
  #if HAS_SPI_FLASH_COMPRESSION
    // Restart the compressed buffer, keep the pointers of the uncompressed buffer
    m_compressedDataUsed = 0;
  #endif
}


void SPIFlashStorage::endWrite() {
  // Flush remaining data
  #if HAS_SPI_FLASH_COMPRESSION
    if (m_compressedDataUsed > 0) {
      flushPage();
      savePage(m_compressedData);
    }
  #else
    if (m_pageDataUsed > 0) flushPage();
  #endif
}

void SPIFlashStorage::savePage(uint8_t* buffer) {
  W25QXX.SPI_FLASH_BufferWrite(buffer, m_startAddress + (SPI_FLASH_PageSize * m_currentPage), SPI_FLASH_PageSize);

  // Test env
  // char fname[256];
  // snprintf(fname, sizeof(fname), "./pages/page-%03d.data", m_currentPage);
  // FILE *fp = fopen(fname, "wb");
  // fwrite(buffer, 1, m_compressedDataUsed, fp);
  // fclose(fp);
}

void SPIFlashStorage::loadPage(uint8_t* buffer) {
  W25QXX.SPI_FLASH_BufferRead(buffer, m_startAddress + (SPI_FLASH_PageSize * m_currentPage), SPI_FLASH_PageSize);

  // Test env
  // char fname[256];
  // memset(buffer, 0, SPI_FLASH_PageSize);
  // snprintf(fname, sizeof(fname), "./pages/page-%03d.data", m_currentPage);
  // FILE *fp = fopen(fname, "rb");
  // if (fp != NULL) {
  //     fread(buffer, 1, SPI_FLASH_PageSize, fp);
  //     fclose(fp);
  // }
}

void SPIFlashStorage::flushPage() {
  #if HAS_SPI_FLASH_COMPRESSION
    // Work com with compressed in memory
    uint32_t inputProcessed;
    uint32_t compressedSize = rle_compress<uint16_t>((uint16_t *)(m_compressedData + m_compressedDataUsed), compressedDataFree() / 2, (uint16_t *)m_pageData, m_pageDataUsed / 2, inputProcessed) * 2;
    inputProcessed *= 2;
    m_compressedDataUsed += compressedSize;

    // Space remaining in the compressed buffer?
    if (compressedDataFree() > 0) {
      // Free the uncompressed buffer
      m_pageDataUsed = 0;
      return;
    }

    // Part of the m_pageData was compressed, so ajust the pointers, freeing what was processed, shift the buffer
    // TODO: To avoid this copy, use a circular buffer
    memmove(m_pageData, m_pageData + inputProcessed, m_pageDataUsed - inputProcessed);
    m_pageDataUsed -= inputProcessed;

    // No? So flush page with compressed data!!
    uint8_t *buffer = m_compressedData;
  #else
    uint8_t *buffer = m_pageData;
  #endif

  savePage(buffer);

  #if HAS_SPI_FLASH_COMPRESSION
    // Restart the compressed buffer, keep the pointers of the uncompressed buffer
    m_compressedDataUsed = 0;
  #elif
    m_pageDataUsed = 0;
  #endif
  m_currentPage++;
}

void SPIFlashStorage::readPage() {
  #if HAS_SPI_FLASH_COMPRESSION
    if (compressedDataFree() == 0) {
      loadPage(m_compressedData);
      m_currentPage++;
      m_compressedDataUsed = 0;
    }

    // Need to uncompress data
    if (pageDataFree() == 0) {
      m_pageDataUsed = 0;
      uint32_t outpuProcessed = 0;
      uint32_t inputProcessed = rle_uncompress<uint16_t, int16_t>((uint16_t *)(m_pageData + m_pageDataUsed), pageDataFree() / 2, (uint16_t *)(m_compressedData + m_compressedDataUsed), compressedDataFree() / 2, outpuProcessed);
      inputProcessed *= 2;
      outpuProcessed *= 2;
      if (outpuProcessed < pageDataFree()) {
        m_pageDataUsed = SPI_FLASH_PageSize - outpuProcessed;
        // TODO: To avoid this copy, use a circular buffer
        memmove(m_pageData + m_pageDataUsed, m_pageData, outpuProcessed);
      }

      m_compressedDataUsed += inputProcessed;
    }
  #else
    loadPage(m_pageData);
    m_pageDataUsed = 0;
    m_currentPage++;
  #endif
}

uint16_t SPIFlashStorage::inData(uint8_t* data, uint16_t size) {
  // Don't write more than we can
  NOMORE(size, pageDataFree());
  memcpy(m_pageData + m_pageDataUsed, data, size);
  m_pageDataUsed += size;
  return size;
}

void SPIFlashStorage::writeData(uint8_t* data, uint16_t size) {
  // Flush a page if needed
  if (pageDataFree() == 0) flushPage();

  while (size > 0) {
    uint16_t written = inData(data, size);
    size -= written;
    // Need to write more? Flush page and continue!
    if (size > 0) {
      flushPage();
      data += written;
    }
  }
}

void SPIFlashStorage::beginRead(uint32_t startAddress) {
  m_startAddress = startAddress;
  m_currentPage = 0;
  // Nothing in memory now
  m_pageDataUsed = SPI_FLASH_PageSize;
  #if HAS_SPI_FLASH_COMPRESSION
    m_compressedDataUsed = sizeof(m_compressedData);
  #endif
}

uint16_t SPIFlashStorage::outData(uint8_t* data, uint16_t size) {
  // Don't read more than we have
  NOMORE(size > pageDataFree());
  memcpy(data, m_pageData + m_pageDataUsed, size);
  m_pageDataUsed += size;
  return size;
}

void SPIFlashStorage::readData(uint8_t* data, uint16_t size) {
  // Read a page if needed
  if (pageDataFree() == 0) readPage();

  while (size > 0) {
    uint16_t read = outData(data, size);
    size -= read;
    // Need to write more? Flush page and continue!
    if (size > 0) {
      readPage();
      data += read;
    }
  }
}

SPIFlashStorage SPIFlash;

#endif // HAS_TFT_LVGL_UI
