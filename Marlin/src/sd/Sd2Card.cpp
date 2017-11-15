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

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "Sd2Card.h"

#include "../Marlin.h"

// send command and return error code.  Return zero for OK
uint8_t Sd2Card::cardCommand(uint8_t cmd, uint32_t arg) {
  // select card
  chipSelectLow();

  // wait up to 300 ms if busy
  waitNotBusy(300);

  // send command
  spiSend(cmd | 0x40);

  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) spiSend(arg >> s);

  // send CRC
  uint8_t crc = 0xFF;
  if (cmd == CMD0) crc = 0x95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) crc = 0x87;  // correct crc for CMD8 with arg 0x1AA
  spiSend(crc);

  // skip stuff byte for stop read
  if (cmd == CMD12) spiRec();

  // wait for response
  for (uint8_t i = 0; ((status_ = spiRec()) & 0x80) && i != 0xFF; i++) { /* Intentionally left empty */ }
  return status_;
}

/**
 * Determine the size of an SD flash memory card.
 *
 * \return The number of 512 byte data blocks in the card
 *         or zero if an error occurs.
 */
uint32_t Sd2Card::cardSize() {
  csd_t csd;
  if (!readCSD(&csd)) return 0;
  if (csd.v1.csd_ver == 0) {
    uint8_t read_bl_len = csd.v1.read_bl_len;
    uint16_t c_size = (csd.v1.c_size_high << 10)
                      | (csd.v1.c_size_mid << 2) | csd.v1.c_size_low;
    uint8_t c_size_mult = (csd.v1.c_size_mult_high << 1)
                          | csd.v1.c_size_mult_low;
    return (uint32_t)(c_size + 1) << (c_size_mult + read_bl_len - 7);
  }
  else if (csd.v2.csd_ver == 1) {
    uint32_t c_size = ((uint32_t)csd.v2.c_size_high << 16)
                      | (csd.v2.c_size_mid << 8) | csd.v2.c_size_low;
    return (c_size + 1) << 10;
  }
  else {
    error(SD_CARD_ERROR_BAD_CSD);
    return 0;
  }
}

void Sd2Card::chipSelectHigh() {
  digitalWrite(chipSelectPin_, HIGH);
}

void Sd2Card::chipSelectLow() {
  #if DISABLED(SOFTWARE_SPI)
    spiInit(spiRate_);
  #endif  // SOFTWARE_SPI
  digitalWrite(chipSelectPin_, LOW);
}

/**
 * Erase a range of blocks.
 *
 * \param[in] firstBlock The address of the first block in the range.
 * \param[in] lastBlock The address of the last block in the range.
 *
 * \note This function requests the SD card to do a flash erase for a
 * range of blocks.  The data on the card after an erase operation is
 * either 0 or 1, depends on the card vendor.  The card must support
 * single block erase.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::erase(uint32_t firstBlock, uint32_t lastBlock) {
  csd_t csd;
  if (!readCSD(&csd)) goto FAIL;
  // check for single block erase
  if (!csd.v1.erase_blk_en) {
    // erase size mask
    uint8_t m = (csd.v1.sector_size_high << 1) | csd.v1.sector_size_low;
    if ((firstBlock & m) != 0 || ((lastBlock + 1) & m) != 0) {
      // error card can't erase specified area
      error(SD_CARD_ERROR_ERASE_SINGLE_BLOCK);
      goto FAIL;
    }
  }
  if (type_ != SD_CARD_TYPE_SDHC) {
    firstBlock <<= 9;
    lastBlock <<= 9;
  }
  if (cardCommand(CMD32, firstBlock)
      || cardCommand(CMD33, lastBlock)
      || cardCommand(CMD38, 0)) {
    error(SD_CARD_ERROR_ERASE);
    goto FAIL;
  }
  if (!waitNotBusy(SD_ERASE_TIMEOUT)) {
    error(SD_CARD_ERROR_ERASE_TIMEOUT);
    goto FAIL;
  }
  chipSelectHigh();
  return true;
  FAIL:
  chipSelectHigh();
  return false;
}

/**
 * Determine if card supports single block erase.
 *
 * \return true if single block erase is supported.
 *         false if single block erase is not supported.
 */
bool Sd2Card::eraseSingleBlockEnable() {
  csd_t csd;
  return readCSD(&csd) ? csd.v1.erase_blk_en : false;
}

/**
 * Initialize an SD flash memory card.
 *
 * \param[in] sckRateID SPI clock rate selector. See setSckRate().
 * \param[in] chipSelectPin SD chip select pin number.
 *
 * \return true for success, false for failure.
 * The reason for failure can be determined by calling errorCode() and errorData().
 */
bool Sd2Card::init(uint8_t sckRateID, pin_t chipSelectPin) {
  errorCode_ = type_ = 0;
  chipSelectPin_ = chipSelectPin;
  // 16-bit init start time allows over a minute
  uint16_t t0 = (uint16_t)millis();
  uint32_t arg;

  // If init takes more than 4s it could trigger
  // watchdog leading to a reboot loop.
  #if ENABLED(USE_WATCHDOG)
    watchdog_reset();
  #endif

  // set pin modes
//todo: should use chipSelectPin ?
  spiBegin();

  // set SCK rate for initialization commands
  spiRate_ = SPI_SD_INIT_RATE;
  spiInit(spiRate_);

  // must supply min of 74 clock cycles with CS high.
  for (uint8_t i = 0; i < 10; i++) spiSend(0xFF);

  // command to go idle in SPI mode
  while ((status_ = cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) {
      error(SD_CARD_ERROR_CMD0);
      goto FAIL;
    }
  }
  // check SD version
  if ((cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
    type(SD_CARD_TYPE_SD1);
  }
  else {
    // only need last byte of r7 response
    for (uint8_t i = 0; i < 4; i++) status_ = spiRec();
    if (status_ != 0xAA) {
      error(SD_CARD_ERROR_CMD8);
      goto FAIL;
    }
    type(SD_CARD_TYPE_SD2);
  }
  // initialize card and send host supports SDHC if SD2
  arg = type() == SD_CARD_TYPE_SD2 ? 0x40000000 : 0;

  while ((status_ = cardAcmd(ACMD41, arg)) != R1_READY_STATE) {
    // check for timeout
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) {
      error(SD_CARD_ERROR_ACMD41);
      goto FAIL;
    }
  }
  // if SD2 read OCR register to check for SDHC card
  if (type() == SD_CARD_TYPE_SD2) {
    if (cardCommand(CMD58, 0)) {
      error(SD_CARD_ERROR_CMD58);
      goto FAIL;
    }
    if ((spiRec() & 0xC0) == 0xC0) type(SD_CARD_TYPE_SDHC);
    // discard rest of ocr - contains allowed voltage range
    for (uint8_t i = 0; i < 3; i++) spiRec();
  }
  chipSelectHigh();

  #if DISABLED(SOFTWARE_SPI)
    return setSckRate(sckRateID);
  #else  // SOFTWARE_SPI
    UNUSED(sckRateID);
    return true;
  #endif  // SOFTWARE_SPI

  FAIL:
  chipSelectHigh();
  return false;
}

/**
 * Read a 512 byte block from an SD card.
 *
 * \param[in] blockNumber Logical block to be read.
 * \param[out] dst Pointer to the location that will receive the data.
 * \return true for success, false for failure.
 */
bool Sd2Card::readBlock(uint32_t blockNumber, uint8_t* dst) {
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;

  #if ENABLED(SD_CHECK_AND_RETRY)
    uint8_t retryCnt = 3;
    for(;;) {
      if (cardCommand(CMD17, blockNumber))
        error(SD_CARD_ERROR_CMD17);
      else if (readData(dst, 512))
        return true;

      if (!--retryCnt) break;

      chipSelectHigh();
      cardCommand(CMD12, 0); // Try sending a stop command, ignore the result.
      errorCode_ = 0;
    }
  #else
    if (cardCommand(CMD17, blockNumber))
      error(SD_CARD_ERROR_CMD17);
    else
      return readData(dst, 512);
  #endif

  chipSelectHigh();
  return false;
}

/**
 * Read one data block in a multiple block read sequence
 *
 * \param[in] dst Pointer to the location for the data to be read.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::readData(uint8_t* dst) {
  chipSelectLow();
  return readData(dst, 512);
}

#if ENABLED(SD_CHECK_AND_RETRY)
  static const uint16_t crctab[] PROGMEM = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
  };
  static uint16_t CRC_CCITT(const uint8_t* data, size_t n) {
    uint16_t crc = 0;
    for (size_t i = 0; i < n; i++) {
      crc = pgm_read_word(&crctab[(crc >> 8 ^ data[i]) & 0xFF]) ^ (crc << 8);
    }
    return crc;
  }
#endif // SD_CHECK_AND_RETRY

bool Sd2Card::readData(uint8_t* dst, uint16_t count) {
  // wait for start block token
  uint16_t t0 = millis();
  while ((status_ = spiRec()) == 0XFF) {
    if (((uint16_t)millis() - t0) > SD_READ_TIMEOUT) {
      error(SD_CARD_ERROR_READ_TIMEOUT);
      goto FAIL;
    }
  }
  if (status_ != DATA_START_BLOCK) {
    error(SD_CARD_ERROR_READ);
    goto FAIL;
  }
  // transfer data
  spiRead(dst, count);

#if ENABLED(SD_CHECK_AND_RETRY)
  {
    uint16_t calcCrc = CRC_CCITT(dst, count);
    uint16_t recvCrc = spiRec() << 8;
    recvCrc |= spiRec();
    if (calcCrc != recvCrc) {
      error(SD_CARD_ERROR_CRC);
      goto FAIL;
    }
  }
#else
  // discard CRC
  spiRec();
  spiRec();
#endif
  chipSelectHigh();
  // Send an additional dummy byte, required by Toshiba Flash Air SD Card
  spiSend(0XFF);
  return true;
  FAIL:
  chipSelectHigh();
  // Send an additional dummy byte, required by Toshiba Flash Air SD Card
  spiSend(0XFF);
  return false;
}

/** read CID or CSR register */
bool Sd2Card::readRegister(uint8_t cmd, void* buf) {
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
  if (cardCommand(cmd, 0)) {
    error(SD_CARD_ERROR_READ_REG);
    chipSelectHigh();
    return false;
  }
  return readData(dst, 16);
}

/**
 * Start a read multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 *
 * \note This function is used with readData() and readStop() for optimized
 * multiple block reads.  SPI chipSelect must be low for the entire sequence.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::readStart(uint32_t blockNumber) {
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (cardCommand(CMD18, blockNumber)) {
    error(SD_CARD_ERROR_CMD18);
    chipSelectHigh();
    return false;
  }
  chipSelectHigh();
  return true;
}

/**
 * End a read multiple blocks sequence.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::readStop() {
  chipSelectLow();
  if (cardCommand(CMD12, 0)) {
    error(SD_CARD_ERROR_CMD12);
    chipSelectHigh();
    return false;
  }
  chipSelectHigh();
  return true;
}

/**
 * Set the SPI clock rate.
 *
 * \param[in] sckRateID A value in the range [0, 6].
 *
 * The SPI clock will be set to F_CPU/pow(2, 1 + sckRateID). The maximum
 * SPI rate is F_CPU/2 for \a sckRateID = 0 and the minimum rate is F_CPU/128
 * for \a scsRateID = 6.
 *
 * \return The value one, true, is returned for success and the value zero,
 * false, is returned for an invalid value of \a sckRateID.
 */
bool Sd2Card::setSckRate(uint8_t sckRateID) {
  if (sckRateID > 6) {
    error(SD_CARD_ERROR_SCK_RATE);
    return false;
  }
  spiRate_ = sckRateID;
  return true;
}

// wait for card to go not busy
bool Sd2Card::waitNotBusy(uint16_t timeoutMillis) {
  uint16_t t0 = millis();
  while (spiRec() != 0XFF)
    if (((uint16_t)millis() - t0) >= timeoutMillis) return false;

  return true;
}

/**
 * Writes a 512 byte block to an SD card.
 *
 * \param[in] blockNumber Logical block to be written.
 * \param[in] src Pointer to the location of the data to be written.
 * \return true for success, false for failure.
 */
bool Sd2Card::writeBlock(uint32_t blockNumber, const uint8_t* src) {
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (cardCommand(CMD24, blockNumber)) {
    error(SD_CARD_ERROR_CMD24);
    goto FAIL;
  }
  if (!writeData(DATA_START_BLOCK, src)) goto FAIL;

  // wait for flash programming to complete
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    error(SD_CARD_ERROR_WRITE_TIMEOUT);
    goto FAIL;
  }
  // response is r2 so get and check two bytes for nonzero
  if (cardCommand(CMD13, 0) || spiRec()) {
    error(SD_CARD_ERROR_WRITE_PROGRAMMING);
    goto FAIL;
  }
  chipSelectHigh();
  return true;
  FAIL:
  chipSelectHigh();
  return false;
}

/**
 * Write one data block in a multiple block write sequence
 * \param[in] src Pointer to the location of the data to be written.
 * \return true for success, false for failure.
 */
bool Sd2Card::writeData(const uint8_t* src) {
  chipSelectLow();
  // wait for previous write to finish
  if (!waitNotBusy(SD_WRITE_TIMEOUT) || !writeData(WRITE_MULTIPLE_TOKEN, src)) {
    error(SD_CARD_ERROR_WRITE_MULTIPLE);
    chipSelectHigh();
    return false;
  }
  chipSelectHigh();
  return true;
}

// send one block of data for write block or write multiple blocks
bool Sd2Card::writeData(uint8_t token, const uint8_t* src) {
  spiSendBlock(token, src);

  spiSend(0xFF);  // dummy crc
  spiSend(0xFF);  // dummy crc

  status_ = spiRec();
  if ((status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    error(SD_CARD_ERROR_WRITE);
    chipSelectHigh();
    return false;
  }
  return true;
}

/**
 * Start a write multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 * \param[in] eraseCount The number of blocks to be pre-erased.
 *
 * \note This function is used with writeData() and writeStop()
 * for optimized multiple block writes.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
  // send pre-erase count
  if (cardAcmd(ACMD23, eraseCount)) {
    error(SD_CARD_ERROR_ACMD23);
    goto FAIL;
  }
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (cardCommand(CMD25, blockNumber)) {
    error(SD_CARD_ERROR_CMD25);
    goto FAIL;
  }
  chipSelectHigh();
  return true;
  FAIL:
  chipSelectHigh();
  return false;
}

/**
 * End a write multiple blocks sequence.
 *
 * \return true for success, false for failure.
 */
bool Sd2Card::writeStop() {
  chipSelectLow();
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) goto FAIL;
  spiSend(STOP_TRAN_TOKEN);
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) goto FAIL;
  chipSelectHigh();
  return true;
  FAIL:
  error(SD_CARD_ERROR_STOP_TRAN);
  chipSelectHigh();
  return false;
}

#endif // SDSUPPORT
