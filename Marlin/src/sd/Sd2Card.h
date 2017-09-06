/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#ifndef SD2CARD_H
#define SD2CARD_H

/**
 * \file
 * \brief Sd2Card class for V2 SD/SDHC cards
 */
#include "SdFatConfig.h"
#include "SdInfo.h"

#include <stdint.h>

//------------------------------------------------------------------------------
/** init timeout ms */
uint16_t const SD_INIT_TIMEOUT = 2000;
/** erase timeout ms */
uint16_t const SD_ERASE_TIMEOUT = 10000;
/** read timeout ms */
uint16_t const SD_READ_TIMEOUT = 300;
/** write time out ms */
uint16_t const SD_WRITE_TIMEOUT = 600;
//------------------------------------------------------------------------------
// SD card errors
/** timeout error for command CMD0 (initialize card in SPI mode) */
uint8_t const SD_CARD_ERROR_CMD0 = 0X1;
/** CMD8 was not accepted - not a valid SD card*/
uint8_t const SD_CARD_ERROR_CMD8 = 0X2;
/** card returned an error response for CMD12 (write stop) */
uint8_t const SD_CARD_ERROR_CMD12 = 0X3;
/** card returned an error response for CMD17 (read block) */
uint8_t const SD_CARD_ERROR_CMD17 = 0X4;
/** card returned an error response for CMD18 (read multiple block) */
uint8_t const SD_CARD_ERROR_CMD18 = 0X5;
/** card returned an error response for CMD24 (write block) */
uint8_t const SD_CARD_ERROR_CMD24 = 0X6;
/**  WRITE_MULTIPLE_BLOCKS command failed */
uint8_t const SD_CARD_ERROR_CMD25 = 0X7;
/** card returned an error response for CMD58 (read OCR) */
uint8_t const SD_CARD_ERROR_CMD58 = 0X8;
/** SET_WR_BLK_ERASE_COUNT failed */
uint8_t const SD_CARD_ERROR_ACMD23 = 0X9;
/** ACMD41 initialization process timeout */
uint8_t const SD_CARD_ERROR_ACMD41 = 0XA;
/** card returned a bad CSR version field */
uint8_t const SD_CARD_ERROR_BAD_CSD = 0XB;
/** erase block group command failed */
uint8_t const SD_CARD_ERROR_ERASE = 0XC;
/** card not capable of single block erase */
uint8_t const SD_CARD_ERROR_ERASE_SINGLE_BLOCK = 0XD;
/** Erase sequence timed out */
uint8_t const SD_CARD_ERROR_ERASE_TIMEOUT = 0XE;
/** card returned an error token instead of read data */
uint8_t const SD_CARD_ERROR_READ = 0XF;
/** read CID or CSD failed */
uint8_t const SD_CARD_ERROR_READ_REG = 0x10;
/** timeout while waiting for start of read data */
uint8_t const SD_CARD_ERROR_READ_TIMEOUT = 0x11;
/** card did not accept STOP_TRAN_TOKEN */
uint8_t const SD_CARD_ERROR_STOP_TRAN = 0x12;
/** card returned an error token as a response to a write operation */
uint8_t const SD_CARD_ERROR_WRITE = 0x13;
/** attempt to write protected block zero */
uint8_t const SD_CARD_ERROR_WRITE_BLOCK_ZERO = 0x14;  // REMOVE - not used
/** card did not go ready for a multiple block write */
uint8_t const SD_CARD_ERROR_WRITE_MULTIPLE = 0x15;
/** card returned an error to a CMD13 status check after a write */
uint8_t const SD_CARD_ERROR_WRITE_PROGRAMMING = 0x16;
/** timeout occurred during write programming */
uint8_t const SD_CARD_ERROR_WRITE_TIMEOUT = 0x17;
/** incorrect rate selected */
uint8_t const SD_CARD_ERROR_SCK_RATE = 0x18;
/** init() not called */
uint8_t const SD_CARD_ERROR_INIT_NOT_CALLED = 0x19;
/** crc check error */
uint8_t const SD_CARD_ERROR_CRC = 0x20;
//------------------------------------------------------------------------------
// card types
/** Standard capacity V1 SD card */
uint8_t const SD_CARD_TYPE_SD1  = 1;
/** Standard capacity V2 SD card */
uint8_t const SD_CARD_TYPE_SD2  = 2;
/** High Capacity SD card */
uint8_t const SD_CARD_TYPE_SDHC = 3;
/**
 * define SOFTWARE_SPI to use bit-bang SPI
 */
//------------------------------------------------------------------------------
#if MEGA_SOFT_SPI
  #define SOFTWARE_SPI
#elif USE_SOFTWARE_SPI
  #define SOFTWARE_SPI
#endif  // MEGA_SOFT_SPI
//------------------------------------------------------------------------------
// SPI pin definitions - do not edit here - change in SdFatConfig.h
//
#define SD_CHIP_SELECT_PIN SS_PIN

#if 0
#if DISABLED(SOFTWARE_SPI)
  // hardware pin defs
  /** The default chip select pin for the SD card is SS. */
  #define SD_CHIP_SELECT_PIN SS_PIN
  // The following three pins must not be redefined for hardware SPI.
  /** SPI Master Out Slave In pin */
  #define SPI_MOSI_PIN MOSI_PIN
  /** SPI Master In Slave Out pin */
  #define SPI_MISO_PIN MISO_PIN
  /** SPI Clock pin */
  #define SPI_SCK_PIN SCK_PIN

#else  // SOFTWARE_SPI

  /** SPI chip select pin */
  #define SD_CHIP_SELECT_PIN SOFT_SPI_CS_PIN
  /** SPI Master Out Slave In pin */
  #define SPI_MOSI_PIN SOFT_SPI_MOSI_PIN
  /** SPI Master In Slave Out pin */
  #define SPI_MISO_PIN SOFT_SPI_MISO_PIN
  /** SPI Clock pin */
  #define SPI_SCK_PIN SOFT_SPI_SCK_PIN
#endif  // SOFTWARE_SPI

#endif

//------------------------------------------------------------------------------
/**
 * \class Sd2Card
 * \brief Raw access to SD and SDHC flash memory cards.
 */
class Sd2Card {
 public:
  /** Construct an instance of Sd2Card. */
  Sd2Card() : errorCode_(SD_CARD_ERROR_INIT_NOT_CALLED), type_(0) {}
  uint32_t cardSize();
  bool erase(uint32_t firstBlock, uint32_t lastBlock);
  bool eraseSingleBlockEnable();
  /**
   *  Set SD error code.
   *  \param[in] code value for error code.
   */
  void error(uint8_t code) {errorCode_ = code;}
  /**
   * \return error code for last error. See Sd2Card.h for a list of error codes.
   */
  int errorCode() const {return errorCode_;}
  /** \return error data for last error. */
  int errorData() const {return status_;}
  /**
   * Initialize an SD flash memory card with default clock rate and chip
   * select pin.  See sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin).
   *
   * \return true for success or false for failure.
   */
  bool init(uint8_t sckRateID = SPI_FULL_SPEED,
            uint8_t chipSelectPin = SD_CHIP_SELECT_PIN);
  bool readBlock(uint32_t block, uint8_t* dst);
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
  bool readData(uint8_t* dst);
  bool readStart(uint32_t blockNumber);
  bool readStop();
  bool setSckRate(uint8_t sckRateID);
  /** Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  int type() const {return type_;}
  bool writeBlock(uint32_t blockNumber, const uint8_t* src);
  bool writeData(const uint8_t* src);
  bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
  bool writeStop();
 private:
  //----------------------------------------------------------------------------
  uint8_t chipSelectPin_;
  uint8_t errorCode_;
  uint8_t spiRate_;
  uint8_t status_;
  uint8_t type_;
  // private functions
  uint8_t cardAcmd(uint8_t cmd, uint32_t arg) {
    cardCommand(CMD55, 0);
    return cardCommand(cmd, arg);
  }
  uint8_t cardCommand(uint8_t cmd, uint32_t arg);

  bool readData(uint8_t* dst, uint16_t count);
  bool readRegister(uint8_t cmd, void* buf);
  void chipSelectHigh();
  void chipSelectLow();
  void type(uint8_t value) {type_ = value;}
  bool waitNotBusy(uint16_t timeoutMillis);
  bool writeData(uint8_t token, const uint8_t* src);
};

#endif // SD2CARD_H
