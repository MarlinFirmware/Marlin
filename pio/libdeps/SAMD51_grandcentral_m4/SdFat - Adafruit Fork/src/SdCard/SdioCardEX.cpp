/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "SdioCard.h"

// limit of K66 due to errata KINETIS_K_0N65N.
const uint32_t MAX_SDHC_COUNT = 0XFFFF;

// Max RU is 1024 blocks.
const uint32_t RU_MASK = 0X03FF;

bool SdioCardEX::readBlock(uint32_t lba, uint8_t* dst) {
  if (m_curState != READ_STATE || lba != m_curLba) {
    if (!syncBlocks()) {
      return false;
    }
    m_limitLba = (lba + MAX_SDHC_COUNT) & ~RU_MASK;
    if (!SdioCard::readStart(lba, m_limitLba - lba)) {
      return false;
    }
    m_curLba = lba;
    m_curState = READ_STATE;
  }
  if (!SdioCard::readData(dst)) {
    return false;
  }
  m_curLba++;
  if (m_curLba >= m_limitLba) {
    m_curState = IDLE_STATE;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCardEX::readBlocks(uint32_t lba, uint8_t* dst, size_t nb) {
  for (size_t i = 0; i < nb; i++) {
    if (!readBlock(lba + i, dst + i*512UL)) {
      return false;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCardEX::syncBlocks() {
  if (m_curState == READ_STATE) {
    m_curState = IDLE_STATE;
    if (!SdioCard::readStop()) {
      return false;
    }
  } else if (m_curState == WRITE_STATE) {
    m_curState = IDLE_STATE;
    if (!SdioCard::writeStop()) {
      return false;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCardEX::writeBlock(uint32_t lba, const uint8_t* src) {
  if (m_curState != WRITE_STATE || m_curLba != lba) {
    if (!syncBlocks()) {
      return false;
    }
    m_limitLba = (lba + MAX_SDHC_COUNT) & ~RU_MASK;
    if (!SdioCard::writeStart(lba , m_limitLba - lba)) {
      return false;
    }
    m_curLba = lba;
    m_curState = WRITE_STATE;
  }
  if (!SdioCard::writeData(src)) {
    return false;
  }
  m_curLba++;
  if (m_curLba >= m_limitLba) {
    m_curState = IDLE_STATE;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCardEX::writeBlocks(uint32_t lba, const uint8_t* src, size_t nb) {
  for (size_t i = 0; i < nb; i++) {
    if (!writeBlock(lba + i, src + i*512UL)) {
      return false;
    }
  }
  return true;
}
