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
 * \file
 * \brief Sd2Card class for V2 SD/SDHC cards
 */

/**
 * Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */
#ifndef _SD2CARD_H_
#define _SD2CARD_H_

#include "SdFatConfig.h"
#include "SdInfo.h"

// SPI speed is F_CPU/2^(1 + index), 0 <= index <= 6
uint8_t const SPI_FULL_SPEED = 0,         // Set SCK to max rate of F_CPU/2. See Sd2Card::setSckRate().
              SPI_HALF_SPEED = 1,         // Set SCK rate to F_CPU/4. See Sd2Card::setSckRate().
              SPI_QUARTER_SPEED = 2,      // Set SCK rate to F_CPU/8. See Sd2Card::setSckRate().
              SPI_EIGHTH_SPEED = 3,       // Set SCK rate to F_CPU/16. See Sd2Card::setSckRate().
              SPI_SIXTEENTH_SPEED = 4;    // Set SCK rate to F_CPU/32. See Sd2Card::setSckRate().

uint16_t const SD_INIT_TIMEOUT = 2000,    // init timeout ms
               SD_ERASE_TIMEOUT = 10000,  // erase timeout ms
               SD_READ_TIMEOUT = 300,     // read timeout ms
               SD_WRITE_TIMEOUT = 600;    // write time out ms

// SD card errors
uint8_t const SD_CARD_ERROR_CMD0 = 0X1,                 // timeout error for command CMD0 (initialize card in SPI mode)
              SD_CARD_ERROR_CMD8 = 0X2,                 // CMD8 was not accepted - not a valid SD card
              SD_CARD_ERROR_CMD12 = 0X3,                // card returned an error response for CMD12 (write stop)
              SD_CARD_ERROR_CMD17 = 0X4,                // card returned an error response for CMD17 (read block)
              SD_CARD_ERROR_CMD18 = 0X5,                // card returned an error response for CMD18 (read multiple block)
              SD_CARD_ERROR_CMD24 = 0X6,                // card returned an error response for CMD24 (write block)
              SD_CARD_ERROR_CMD25 = 0X7,                // WRITE_MULTIPLE_BLOCKS command failed
              SD_CARD_ERROR_CMD58 = 0X8,                // card returned an error response for CMD58 (read OCR)
              SD_CARD_ERROR_ACMD23 = 0X9,               // SET_WR_BLK_ERASE_COUNT failed
              SD_CARD_ERROR_ACMD41 = 0XA,               // ACMD41 initialization process timeout
              SD_CARD_ERROR_BAD_CSD = 0XB,              // card returned a bad CSR version field
              SD_CARD_ERROR_ERASE = 0XC,                // erase block group command failed
              SD_CARD_ERROR_ERASE_SINGLE_BLOCK = 0XD,   // card not capable of single block erase
              SD_CARD_ERROR_ERASE_TIMEOUT = 0XE,        // Erase sequence timed out
              SD_CARD_ERROR_READ = 0XF,                 // card returned an error token instead of read data
              SD_CARD_ERROR_READ_REG = 0x10,            // read CID or CSD failed
              SD_CARD_ERROR_READ_TIMEOUT = 0x11,        // timeout while waiting for start of read data
              SD_CARD_ERROR_STOP_TRAN = 0x12,           // card did not accept STOP_TRAN_TOKEN
              SD_CARD_ERROR_WRITE = 0x13,               // card returned an error token as a response to a write operation
              SD_CARD_ERROR_WRITE_BLOCK_ZERO = 0x14,    // REMOVE - not used ... attempt to write protected block zero
              SD_CARD_ERROR_WRITE_MULTIPLE = 0x15,      // card did not go ready for a multiple block write
              SD_CARD_ERROR_WRITE_PROGRAMMING = 0x16,   // card returned an error to a CMD13 status check after a write
              SD_CARD_ERROR_WRITE_TIMEOUT = 0x17,       // timeout occurred during write programming
              SD_CARD_ERROR_SCK_RATE = 0x18,            // incorrect rate selected
              SD_CARD_ERROR_INIT_NOT_CALLED = 0x19,     // init() not called
              SD_CARD_ERROR_CRC = 0x20;                 // crc check error

// card types
uint8_t const SD_CARD_TYPE_SD1  = 1,                    // Standard capacity V1 SD card
              SD_CARD_TYPE_SD2  = 2,                    // Standard capacity V2 SD card
              SD_CARD_TYPE_SDHC = 3;                    // High Capacity SD card

/**
 * define SOFTWARE_SPI to use bit-bang SPI
 */
#if MEGA_SOFT_SPI
  #define SOFTWARE_SPI
#elif USE_SOFTWARE_SPI
  #define SOFTWARE_SPI
#endif

// SPI pin definitions - do not edit here - change in SdFatConfig.h
#if DISABLED(SOFTWARE_SPI)
  // hardware pin defs
  #define SD_CHIP_SELECT_PIN SS_PIN   // The default chip select pin for the SD card is SS.
  // The following three pins must not be redefined for hardware SPI.
  #define SPI_MOSI_PIN MOSI_PIN       // SPI Master Out Slave In pin
  #define SPI_MISO_PIN MISO_PIN       // SPI Master In Slave Out pin
  #define SPI_SCK_PIN SCK_PIN         // SPI Clock pin
#else  // SOFTWARE_SPI
  #define SD_CHIP_SELECT_PIN SOFT_SPI_CS_PIN  // SPI chip select pin
  #define SPI_MOSI_PIN SOFT_SPI_MOSI_PIN      // SPI Master Out Slave In pin
  #define SPI_MISO_PIN SOFT_SPI_MISO_PIN      // SPI Master In Slave Out pin
  #define SPI_SCK_PIN SOFT_SPI_SCK_PIN        // SPI Clock pin
#endif  // SOFTWARE_SPI

/**
 * \class Sd2Card
 * \brief Raw access to SD and SDHC flash memory cards.
 */
class Sd2Card {
  public:

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
  bool readCID(cid_t* cid) { return readRegister(CMD10, cid); }

  /**
   * Read a card's CSD register. The CSD contains Card-Specific Data that
   * provides information regarding access to the card's contents.
   *
   * \param[out] csd pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool readCSD(csd_t* csd) { return readRegister(CMD9, csd); }

  bool readData(uint8_t* dst);
  bool readStart(uint32_t blockNumber);
  bool readStop();
  bool setSckRate(uint8_t sckRateID);
  /**
   * Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  int type() const {return type_;}
  bool writeBlock(uint32_t blockNumber, const uint8_t* src);
  bool writeData(const uint8_t* src);
  bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
  bool writeStop();

  private:
  uint8_t chipSelectPin_,
          errorCode_,
          spiRate_,
          status_,
          type_;

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
  void type(uint8_t value) { type_ = value; }
  bool waitNotBusy(uint16_t timeoutMillis);
  bool writeData(uint8_t token, const uint8_t* src);
};

#endif  // _SD2CARD_H_
