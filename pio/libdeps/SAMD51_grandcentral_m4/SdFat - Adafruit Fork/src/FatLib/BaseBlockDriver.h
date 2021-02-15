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
#ifndef BaseBlockDriver_h
#define BaseBlockDriver_h
#include "FatLibConfig.h"
/**
 * \class BaseBlockDriver
 * \brief Base block driver.
 */
class BaseBlockDriver {
 public:
    /**
   * Read a 512 byte block from an SD card.
   *
   * \param[in] block Logical block to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  virtual bool readBlock(uint32_t block, uint8_t* dst) = 0;
  /** End multi-block transfer and go to idle state.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  virtual bool syncBlocks() = 0;
  /**
   * Writes a 512 byte block to an SD card.
   *
   * \param[in] block Logical block to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  virtual bool writeBlock(uint32_t block, const uint8_t* src) = 0;
#if USE_MULTI_BLOCK_IO
  /**
   * Read multiple 512 byte blocks from an SD card.
   *
   * \param[in] block Logical block to be read.
   * \param[in] nb Number of blocks to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  virtual bool readBlocks(uint32_t block, uint8_t* dst, size_t nb) = 0;
  /**
   * Write multiple 512 byte blocks to an SD card.
   *
   * \param[in] block Logical block to be written.
   * \param[in] nb Number of blocks to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  virtual bool writeBlocks(uint32_t block, const uint8_t* src, size_t nb) = 0;
#endif  // USE_MULTI_BLOCK_IO
};
#endif  // BaseBlockDriver_h
