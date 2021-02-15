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
#include "SdSpiCard.h"
bool SdSpiCardEX::readBlock(uint32_t block, uint8_t* dst) {
  if (m_curState != READ_STATE || block != m_curBlock) {
    if (!syncBlocks()) {
      return false;
    }
    if (!SdSpiCard::readStart(block)) {
      return false;
    }
    m_curBlock = block;
    m_curState = READ_STATE;
  }
  if (!SdSpiCard::readData(dst)) {
    return false;
  }
  m_curBlock++;
  return true;
}
//-----------------------------------------------------------------------------
bool SdSpiCardEX::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
  for (size_t i = 0; i < nb; i++) {
    if (!readBlock(block + i, dst + i*512UL)) {
      return false;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdSpiCardEX::syncBlocks() {
  if (m_curState == READ_STATE) {
    m_curState = IDLE_STATE;
    if (!SdSpiCard::readStop()) {
      return false;
    }
  } else if (m_curState == WRITE_STATE) {
    m_curState = IDLE_STATE;
    if (!SdSpiCard::writeStop()) {
      return false;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdSpiCardEX::writeBlock(uint32_t block, const uint8_t* src) {
  if (m_curState != WRITE_STATE || m_curBlock != block) {
    if (!syncBlocks()) {
      return false;
    }
    if (!SdSpiCard::writeStart(block)) {
      return false;
    }
    m_curBlock = block;
    m_curState = WRITE_STATE;
  }
  if (!SdSpiCard::writeData(src)) {
    return false;
  }
  m_curBlock++;
  return true;
}
//-----------------------------------------------------------------------------
bool SdSpiCardEX::writeBlocks(uint32_t block,
                                  const uint8_t* src, size_t nb) {
  for (size_t i = 0; i < nb; i++) {
    if (!writeBlock(block + i, src + i*512UL)) {
      return false;
    }
  }
  return true;
}
