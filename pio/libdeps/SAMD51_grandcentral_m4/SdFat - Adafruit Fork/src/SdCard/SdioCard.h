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
#ifndef SdioCard_h
#define SdioCard_h
#include "SysCall.h"
#include "BlockDriver.h"
/**
 * \class SdioCard
 * \brief Raw SDIO access to SD and SDHC flash memory cards.
 */
class SdioCard : public BaseBlockDriver {
 public:
  /** Initialize the SD card.
   * \return true for success else false.
   */
  bool begin();
  /**
   * Determine the size of an SD flash memory card.
   *
   * \return The number of 512 byte sectors in the card
   *         or zero if an error occurs.
   */
  uint32_t cardCapacity();
  /** \return Card size in sectors or zero if an error occurs. */
  uint32_t cardSize() {return cardCapacity();}
  /** Erase a range of blocks.
   *
   * \param[in] firstBlock The address of the first block in the range.
   * \param[in] lastBlock The address of the last block in the range.
   *
   * \note This function requests the SD card to do a flash erase for a
   * range of blocks.  The data on the card after an erase operation is
   * either 0 or 1, depends on the card vendor.  The card must support
   * single block erase.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool erase(uint32_t firstBlock, uint32_t lastBlock);
  /**
   * \return code for the last error. See SdInfo.h for a list of error codes.
   */
  uint8_t errorCode();
  /** \return error data for last error. */
  uint32_t errorData();
  /** \return error line for last error. Tmp function for debug. */
  uint32_t errorLine();
  /**
   * Check for busy with CMD13.
   *
   * \return true if busy else false.
   */
  bool isBusy();
  /** \return the SD clock frequency in kHz. */
  uint32_t kHzSdClk();
  /**
   * Read a 512 byte block from an SD card.
   *
   * \param[in] lba Logical block to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readBlock(uint32_t lba, uint8_t* dst);
  /**
   * Read multiple 512 byte blocks from an SD card.
   *
   * \param[in] lba Logical block to be read.
   * \param[in] nb Number of blocks to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readBlocks(uint32_t lba, uint8_t* dst, size_t nb);
  /**
   * Read a card's CID register. The CID contains card identification
   * information such as Manufacturer ID, Product name, Product serial
   * number and Manufacturing date.
   *
   * \param[out] cid pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool readCID(void* cid);
  /**
   * Read a card's CSD register. The CSD contains Card-Specific Data that
   * provides information regarding access to the card's contents.
   *
   * \param[out] csd pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool readCSD(void* csd);
  /** Read one data block in a multiple block read sequence
   *
   * \param[out] dst Pointer to the location for the data to be read.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readData(uint8_t *dst);
  /** Read OCR register.
   *
   * \param[out] ocr Value of OCR register.
   * \return true for success else false.
   */
  bool readOCR(uint32_t* ocr);
  /** Start a read multiple blocks sequence.
   *
   * \param[in] lba Address of first block in sequence.
   *
   * \note This function is used with readData() and readStop() for optimized
   * multiple block reads.  SPI chipSelect must be low for the entire sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStart(uint32_t lba);
  /** Start a read multiple blocks sequence.
   *
   * \param[in] lba Address of first block in sequence.
   * \param[in] count Maximum block count.
   * \note This function is used with readData() and readStop() for optimized
   * multiple block reads.  SPI chipSelect must be low for the entire sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStart(uint32_t lba, uint32_t count);
  /** End a read multiple blocks sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStop();
  /** \return success if sync successful. Not for user apps. */
  bool syncBlocks();
  /** Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  uint8_t type();
  /**
   * Writes a 512 byte block to an SD card.
   *
   * \param[in] lba Logical block to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeBlock(uint32_t lba, const uint8_t* src);
  /**
   * Write multiple 512 byte blocks to an SD card.
   *
   * \param[in] lba Logical block to be written.
   * \param[in] nb Number of blocks to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeBlocks(uint32_t lba, const uint8_t* src, size_t nb);
  /** Write one data block in a multiple block write sequence.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeData(const uint8_t* src);
  /** Start a write multiple blocks sequence.
   *
   * \param[in] lba Address of first block in sequence.
   *
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple block writes.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStart(uint32_t lba);
  /** Start a write multiple blocks sequence.
   *
   * \param[in] lba Address of first block in sequence.
   * \param[in] count Maximum block count.
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple block writes.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStart(uint32_t lba, uint32_t count);

  /** End a write multiple blocks sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStop();
};
//==============================================================================
/**
 * \class SdioCardEX
 * \brief Extended SD I/O block driver.
 */
class SdioCardEX : public SdioCard {
 public:
  /** Initialize the SD card
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool begin() {
    m_curState = IDLE_STATE;
    return SdioCard::begin();
  }
  /** Erase a range of blocks.
   *
   * \param[in] firstBlock The address of the first block in the range.
   * \param[in] lastBlock The address of the last block in the range.
   *
   * \note This function requests the SD card to do a flash erase for a
   * range of blocks.  The data on the card after an erase operation is
   * either 0 or 1, depends on the card vendor.  The card must support
   * single block erase.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool erase(uint32_t firstBlock, uint32_t lastBlock) {
    return syncBlocks() && SdioCard::erase(firstBlock, lastBlock);
  }
  /**
   * Read a 512 byte block from an SD card.
   *
   * \param[in] block Logical block to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readBlock(uint32_t block, uint8_t* dst);
  /** End multi-block transfer and go to idle state.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool syncBlocks();
  /**
   * Writes a 512 byte block to an SD card.
   *
   * \param[in] block Logical block to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeBlock(uint32_t block, const uint8_t* src);
  /**
   * Read multiple 512 byte blocks from an SD card.
   *
   * \param[in] block Logical block to be read.
   * \param[in] nb Number of blocks to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readBlocks(uint32_t block, uint8_t* dst, size_t nb);
  /**
   * Write multiple 512 byte blocks to an SD card.
   *
   * \param[in] block Logical block to be written.
   * \param[in] nb Number of blocks to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeBlocks(uint32_t block, const uint8_t* src, size_t nb);

 private:
  static const uint32_t IDLE_STATE = 0;
  static const uint32_t READ_STATE = 1;
  static const uint32_t WRITE_STATE = 2;
  uint32_t m_curLba;
  uint32_t m_limitLba;
  uint8_t m_curState;
};
#endif  // SdioCard_h
