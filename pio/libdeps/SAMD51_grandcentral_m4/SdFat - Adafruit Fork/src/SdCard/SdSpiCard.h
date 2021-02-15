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
#ifndef SdSpiCard_h
#define SdSpiCard_h
/**
 * \file
 * \brief SdSpiCard class for V2 SD/SDHC cards
 */
#include <stddef.h>
#include "SysCall.h"
#include "SdInfo.h"
#include "../FatLib/BaseBlockDriver.h"
#include "../SpiDriver/SdSpiDriver.h"
//==============================================================================
/**
 * \class SdSpiCard
 * \brief Raw access to SD and SDHC flash memory cards via SPI protocol.
 */
#if ENABLE_EXTENDED_TRANSFER_CLASS || ENABLE_SDIO_CLASS
class SdSpiCard : public BaseBlockDriver {
#else  // ENABLE_EXTENDED_TRANSFER_CLASS || ENABLE_SDIO_CLASS
class SdSpiCard {
#endif  // ENABLE_EXTENDED_TRANSFER_CLASS || ENABLE_SDIO_CLASS
 public:
  /** Construct an instance of SdSpiCard. */
  SdSpiCard() : m_errorCode(SD_CARD_ERROR_INIT_NOT_CALLED), m_type(0) {}
  /** Initialize the SD card.
   * \param[in] spi SPI driver for card.
   * \param[in] csPin card chip select pin.
   * \param[in] spiSettings SPI speed, mode, and bit order.
   * \return true for success else false.
   */
  bool begin(SdSpiDriver* spi, uint8_t csPin, SPISettings spiSettings);
  /**
   * Determine the size of an SD flash memory card.
   *
   * \return The number of 512 byte sectors in the card
   *         or zero if an error occurs.
   */
  uint32_t cardCapacity();
  /** \return Card size in sectors or zero if an error occurs. */
  uint32_t cardSize() {return cardCapacity();}
  /** Clear debug stats. */
  void dbgClearStats();
  /** Print debug stats. */
  void dbgPrintStats();
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
  /** Determine if card supports single block erase.
   *
   * \return true is returned if single block erase is supported.
   * false is returned if single block erase is not supported.
   */
  bool eraseSingleBlockEnable();
  /**
   *  Set SD error code.
   *  \param[in] code value for error code.
   */
  void error(uint8_t code) {
    m_errorCode = code;
  }
  /**
   * \return code for the last error. See SdInfo.h for a list of error codes.
   */
  int errorCode() const {
    return m_errorCode;
  }
  /** \return error data for last error. */
  int errorData() const {
    return m_status;
  }
  /**
   * Check for busy.  MISO low indicates the card is busy.
   *
   * \return true if busy else false.
   */
  bool isBusy();
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
  bool readCID(cid_t* cid) {
    return readRegister(CMD10, cid);
  }
  /**
   * Read a card's CSD register. The CSD contains Card-Specific Data that
   * provides information regarding access to the card's contents.
   *
   * \param[out] csd pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool readCSD(csd_t* csd) {
    return readRegister(CMD9, csd);
  }
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
   * \param[in] blockNumber Address of first block in sequence.
   *
   * \note This function is used with readData() and readStop() for optimized
   * multiple block reads.  SPI chipSelect must be low for the entire sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStart(uint32_t blockNumber);
  /** Return the 64 byte card status
   * \param[out] status location for 64 status bytes.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStatus(uint8_t* status);
  /** End a read multiple blocks sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool readStop();
  /** \return success if sync successful. Not for user apps. */
  bool syncBlocks() {return true;}
  /** Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  int type() const {
    return m_type;
  }
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
   * \param[in] blockNumber Address of first block in sequence.
   *
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple block writes.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStart(uint32_t blockNumber);

  /** Start a write multiple blocks sequence with pre-erase.
   *
   * \param[in] blockNumber Address of first block in sequence.
   * \param[in] eraseCount The number of blocks to be pre-erased.
   *
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple block writes.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
  /** End a write multiple blocks sequence.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool writeStop();
  /** Set CS low and activate the card. */
  void spiStart();
  /** Set CS high and deactivate the card. */
  void spiStop();

 private:
  // private functions
  uint8_t cardAcmd(uint8_t cmd, uint32_t arg) {
    cardCommand(CMD55, 0);
    return cardCommand(cmd, arg);
  }
  uint8_t cardCommand(uint8_t cmd, uint32_t arg);
  bool isTimedOut(uint16_t startMS, uint16_t timeoutMS);
  bool readData(uint8_t* dst, size_t count);
  bool readRegister(uint8_t cmd, void* buf);

  void type(uint8_t value) {
    m_type = value;
  }

  bool waitNotBusy(uint16_t timeoutMS);
  bool writeData(uint8_t token, const uint8_t* src);

  //---------------------------------------------------------------------------
  // functions defined in SdSpiDriver.h
  void spiActivate() {
    m_spiDriver->activate();
  }
  void spiDeactivate() {
    m_spiDriver->deactivate();
  }
  uint8_t spiReceive() {
    return m_spiDriver->receive();
  }
  uint8_t spiReceive(uint8_t* buf, size_t n) {
    return  m_spiDriver->receive(buf, n);
  }
  void spiSend(uint8_t data) {
     m_spiDriver->send(data);
  }
  void spiSend(const uint8_t* buf, size_t n) {
    m_spiDriver->send(buf, n);
  }
  void spiSelect() {
    m_spiDriver->select();
  }
  void spiUnselect() {
    m_spiDriver->unselect();
  }
  uint8_t m_errorCode;
  SdSpiDriver *m_spiDriver;
  bool    m_spiActive;
  uint8_t m_status;
  uint8_t m_type;
};
//==============================================================================
/**
 * \class SdSpiCardEX
 * \brief Extended SD I/O block driver.
 */
class SdSpiCardEX : public SdSpiCard {
 public:
  /** Initialize the SD card
   *
   * \param[in] spi SPI driver.
   * \param[in] csPin Card chip select pin number.
   * \param[in] spiSettings SPI speed, mode, and bit order.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool begin(SdSpiDriver* spi, uint8_t csPin, SPISettings spiSettings) {
    m_curState = IDLE_STATE;
    return SdSpiCard::begin(spi, csPin, spiSettings);
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
  uint32_t m_curBlock;
  uint8_t m_curState;
};
#endif  // SdSpiCard_h
