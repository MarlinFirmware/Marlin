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
//==============================================================================
// debug trace macro
#define SD_TRACE(m, b)
// #define SD_TRACE(m, b) Serial.print(m);Serial.println(b);
#define SD_CS_DBG(m)
// #define SD_CS_DBG(m) Serial.println(F(m));

#define DBG_PROFILE_STATS 0
#if DBG_PROFILE_STATS

#define DBG_TAG_LIST\
  DBG_TAG(DBG_CMD0_TIME, "CMD0 time")\
  DBG_TAG(DBG_ACMD41_TIME, "ACMD41 time")\
  DBG_TAG(DBG_CMD_BUSY, "cmd busy")\
  DBG_TAG(DBG_ERASE_BUSY, "erase busy")\
  DBG_TAG(DBG_WAIT_READ, "wait read")\
  DBG_TAG(DBG_WRITE_FLASH, "write flash")\
  DBG_TAG(DBG_WRITE_BUSY, "write busy")\
  DBG_TAG(DBG_WRITE_STOP, "write stop")\
  DBG_TAG(DBG_ACMD41_COUNT, "ACMD41 count")\
  DBG_TAG(DBG_CMD0_COUNT, "CMD0 count")

#define DBG_TIME_DIM DBG_ACMD41_COUNT

enum DbgTag {
  #define DBG_TAG(tag, str) tag,
  DBG_TAG_LIST
  DBG_COUNT_DIM
  #undef DBG_TAG
};

static uint32_t dbgCount[DBG_COUNT_DIM];
static uint32_t dbgBgnTime[DBG_TIME_DIM];
static uint32_t dbgMaxTime[DBG_TIME_DIM];
static uint32_t dbgMinTime[DBG_TIME_DIM];
static uint32_t dbgTotalTime[DBG_TIME_DIM];
//------------------------------------------------------------------------------
static void dbgBeginTime(DbgTag tag) {
  dbgBgnTime[tag] = micros();
}
//------------------------------------------------------------------------------
static void dbgClearStats() {
  for (int i = 0; i < DBG_COUNT_DIM; i++) {
    dbgCount[i] = 0;
    if (i < DBG_TIME_DIM) {
      dbgMaxTime[i] = 0;
      dbgMinTime[i] = 9999999;
      dbgTotalTime[i] = 0;
    }
  }
}
//------------------------------------------------------------------------------
static void dbgEndTime(DbgTag tag) {
  uint32_t m = micros() - dbgBgnTime[tag];
  dbgTotalTime[tag] += m;
  if (m > dbgMaxTime[tag]) {
    dbgMaxTime[tag] = m;
  }
  if (m < dbgMinTime[tag]) {
    dbgMinTime[tag] = m;
  }
  dbgCount[tag]++;
}
//------------------------------------------------------------------------------
static void dbgEventCount(DbgTag tag) {
  dbgCount[tag]++;
}
//------------------------------------------------------------------------------
static void dbgPrintTagText(uint8_t tag) {
  #define DBG_TAG(e, m) case e: Serial.print(F(m)); break;
  switch (tag) {
    DBG_TAG_LIST
  }
  #undef DBG_TAG
}
//------------------------------------------------------------------------------
static void dbgPrintStats() {
  Serial.println();
  Serial.println(F("======================="));
  Serial.println(F("item,event,min,max,avg"));
  Serial.println(F("tag,count,usec,usec,usec"));
  for (int i = 0; i < DBG_COUNT_DIM; i++) {
    if (dbgCount[i]) {
      dbgPrintTagText(i);
      Serial.print(',');
      Serial.print(dbgCount[i]);
      if (i < DBG_TIME_DIM) {
        Serial.print(',');
        Serial.print(dbgMinTime[i]);
        Serial.print(',');
        Serial.print(dbgMaxTime[i]);
        Serial.print(',');
        Serial.print(dbgTotalTime[i]/dbgCount[i]);
      }
      Serial.println();
    }
  }
  Serial.println(F("======================="));
  Serial.println();
}
#undef DBG_TAG_LIST
#define DBG_BEGIN_TIME(tag) dbgBeginTime(tag)
#define DBG_END_TIME(tag) dbgEndTime(tag)
#define DBG_EVENT_COUNT(tag) dbgEventCount(tag)
#else  // DBG_PROFILE_STATS
#define DBG_BEGIN_TIME(tag)
#define DBG_END_TIME(tag)
#define DBG_EVENT_COUNT(tag)
static void dbgClearStats() {}
static void dbgPrintStats() {}
#endif  // DBG_PROFILE_STATS
//==============================================================================
#if USE_SD_CRC
// CRC functions
//------------------------------------------------------------------------------
static uint8_t CRC7(const uint8_t* data, uint8_t n) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < n; i++) {
    uint8_t d = data[i];
    for (uint8_t j = 0; j < 8; j++) {
      crc <<= 1;
      if ((d & 0x80) ^ (crc & 0x80)) {
        crc ^= 0x09;
      }
      d <<= 1;
    }
  }
  return (crc << 1) | 1;
}
//------------------------------------------------------------------------------
#if USE_SD_CRC == 1
// Shift based CRC-CCITT
// uses the x^16,x^12,x^5,x^1 polynomial.
static uint16_t CRC_CCITT(const uint8_t *data, size_t n) {
  uint16_t crc = 0;
  for (size_t i = 0; i < n; i++) {
    crc = (uint8_t)(crc >> 8) | (crc << 8);
    crc ^= data[i];
    crc ^= (uint8_t)(crc & 0xff) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0xff) << 5;
  }
  return crc;
}
#elif USE_SD_CRC > 1  // CRC_CCITT
//------------------------------------------------------------------------------
// Table based CRC-CCITT
// uses the x^16,x^12,x^5,x^1 polynomial.
#ifdef __AVR__
static const uint16_t crctab[] PROGMEM = {
#else  // __AVR__
static const uint16_t crctab[] = {
#endif  // __AVR__
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
#ifdef __AVR__
    crc = pgm_read_word(&crctab[(crc >> 8 ^ data[i]) & 0XFF]) ^ (crc << 8);
#else  // __AVR__
    crc = crctab[(crc >> 8 ^ data[i]) & 0XFF] ^ (crc << 8);
#endif  // __AVR__
  }
  return crc;
}
#endif  // CRC_CCITT
#endif  // USE_SD_CRC
//==============================================================================
// SdSpiCard member functions
//------------------------------------------------------------------------------
bool SdSpiCard::begin(SdSpiDriver* spi, uint8_t csPin, SPISettings settings) {
  m_spiActive = false;
  m_errorCode = SD_CARD_ERROR_NONE;
  m_type = 0;
  m_spiDriver = spi;
  uint16_t t0 = curTimeMS();
  uint32_t arg;

  m_spiDriver->begin(csPin);
  m_spiDriver->setSpiSettings(SD_SCK_HZ(250000));
  spiStart();

  // must supply min of 74 clock cycles with CS high.
  spiUnselect();
  for (uint8_t i = 0; i < 10; i++) {
    spiSend(0XFF);
  }
  spiSelect();

  DBG_BEGIN_TIME(DBG_CMD0_TIME);
  // command to go idle in SPI mode
  for (uint8_t i = 1;; i++) {
    DBG_EVENT_COUNT(DBG_CMD0_COUNT);
    if (cardCommand(CMD0, 0) == R1_IDLE_STATE) {
      break;
    }
    if (i == SD_CMD0_RETRY) {
      error(SD_CARD_ERROR_CMD0);
      goto fail;
    }
    // stop multi-block write
    spiSend(STOP_TRAN_TOKEN);
    // finish block transfer
    for (int i = 0; i < 520; i++) {
      spiReceive();
    }
  }
  DBG_END_TIME(DBG_CMD0_TIME);
#if USE_SD_CRC
  if (cardCommand(CMD59, 1) != R1_IDLE_STATE) {
    error(SD_CARD_ERROR_CMD59);
    goto fail;
  }
#endif  // USE_SD_CRC
  // check SD version
  if (cardCommand(CMD8, 0x1AA) == (R1_ILLEGAL_COMMAND | R1_IDLE_STATE)) {
    type(SD_CARD_TYPE_SD1);
  } else {
    for (uint8_t i = 0; i < 4; i++) {
      m_status = spiReceive();
    }
    if (m_status == 0XAA) {
      type(SD_CARD_TYPE_SD2);
    } else {
      error(SD_CARD_ERROR_CMD8);
      goto fail;
    }
  }
  // initialize card and send host supports SDHC if SD2
  arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;
  DBG_BEGIN_TIME(DBG_ACMD41_TIME);
  while (cardAcmd(ACMD41, arg) != R1_READY_STATE) {
    DBG_EVENT_COUNT(DBG_ACMD41_COUNT);
    // check for timeout
    if (isTimedOut(t0, SD_INIT_TIMEOUT)) {
      error(SD_CARD_ERROR_ACMD41);
      goto fail;
    }
  }
  DBG_END_TIME(DBG_ACMD41_TIME);
  // if SD2 read OCR register to check for SDHC card
  if (type() == SD_CARD_TYPE_SD2) {
    if (cardCommand(CMD58, 0)) {
      error(SD_CARD_ERROR_CMD58);
      goto fail;
    }
    if ((spiReceive() & 0XC0) == 0XC0) {
      type(SD_CARD_TYPE_SDHC);
    }
    // Discard rest of ocr - contains allowed voltage range.
    for (uint8_t i = 0; i < 3; i++) {
      spiReceive();
    }
  }
  spiStop();
  m_spiDriver->setSpiSettings(settings);
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t SdSpiCard::cardCommand(uint8_t cmd, uint32_t arg) {
  // select card
  if (!m_spiActive) {
    spiStart();
  }
  // wait if busy unless CMD0
  if (cmd != CMD0) {
    DBG_BEGIN_TIME(DBG_CMD_BUSY);
    waitNotBusy(SD_CMD_TIMEOUT);
    DBG_END_TIME(DBG_CMD_BUSY);
  }

#if USE_SD_CRC
  // form message
  uint8_t buf[6];
  buf[0] = (uint8_t)0x40U | cmd;
  buf[1] = (uint8_t)(arg >> 24U);
  buf[2] = (uint8_t)(arg >> 16U);
  buf[3] = (uint8_t)(arg >> 8U);
  buf[4] = (uint8_t)arg;

  // add CRC
  buf[5] = CRC7(buf, 5);

  // send message
  spiSend(buf, 6);
#else  // USE_SD_CRC
  // send command
  spiSend(cmd | 0x40);

  // send argument
  uint8_t *pa = reinterpret_cast<uint8_t *>(&arg);
  for (int8_t i = 3; i >= 0; i--) {
    spiSend(pa[i]);
  }
  // send CRC - correct for CMD0 with arg zero or CMD8 with arg 0X1AA
  spiSend(cmd == CMD0 ? 0X95 : 0X87);
#endif  // USE_SD_CRC

  // discard first fill byte to avoid MISO pull-up problem.
  spiReceive();

  // there are 1-8 fill bytes before response.  fill bytes should be 0XFF.
  for (uint8_t i = 0; ((m_status = spiReceive()) & 0X80) && i < 10; i++) {
  }
  return m_status;
}
//------------------------------------------------------------------------------
uint32_t SdSpiCard::cardCapacity() {
  csd_t csd;
  return readCSD(&csd) ? sdCardCapacity(&csd) : 0;
}
//------------------------------------------------------------------------------
void SdSpiCard::dbgClearStats() {::dbgClearStats();}
//------------------------------------------------------------------------------
void SdSpiCard::dbgPrintStats() {::dbgPrintStats();}
//------------------------------------------------------------------------------
bool SdSpiCard::erase(uint32_t firstBlock, uint32_t lastBlock) {
  csd_t csd;
  if (!readCSD(&csd)) {
    goto fail;
  }
  // check for single block erase
  if (!csd.v1.erase_blk_en) {
    // erase size mask
    uint8_t m = (csd.v1.sector_size_high << 1) | csd.v1.sector_size_low;
    if ((firstBlock & m) != 0 || ((lastBlock + 1) & m) != 0) {
      // error card can't erase specified area
      error(SD_CARD_ERROR_ERASE_SINGLE_BLOCK);
      goto fail;
    }
  }
  if (m_type != SD_CARD_TYPE_SDHC) {
    firstBlock <<= 9;
    lastBlock <<= 9;
  }
  if (cardCommand(CMD32, firstBlock)
      || cardCommand(CMD33, lastBlock)
      || cardCommand(CMD38, 0)) {
    error(SD_CARD_ERROR_ERASE);
    goto fail;
  }
  DBG_BEGIN_TIME(DBG_ERASE_BUSY);
  if (!waitNotBusy(SD_ERASE_TIMEOUT)) {
    error(SD_CARD_ERROR_ERASE_TIMEOUT);
    goto fail;
  }
  DBG_END_TIME(DBG_ERASE_BUSY);
  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::eraseSingleBlockEnable() {
  csd_t csd;
  return readCSD(&csd) ? csd.v1.erase_blk_en : false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::isBusy() {
  bool rtn = true;
  bool spiActive = m_spiActive;
  if (!spiActive) {
    spiStart();
  }
  for (uint8_t i = 0; i < 8; i++) {
    if (0XFF == spiReceive()) {
      rtn = false;
      break;
    }
  }
  if (!spiActive) {
    spiStop();
  }
  return rtn;
}
//------------------------------------------------------------------------------
bool SdSpiCard::isTimedOut(uint16_t startMS, uint16_t timeoutMS) {
#if WDT_YIELD_TIME_MICROS
  static uint32_t last;
  if ((micros() - last) > WDT_YIELD_TIME_MICROS) {
    SysCall::yield();
    last = micros();
  }
#endif  // WDT_YIELD_TIME_MICROS
  return (curTimeMS() - startMS) > timeoutMS;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readBlock(uint32_t blockNumber, uint8_t* dst) {
  SD_TRACE("RB", blockNumber);
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) {
    blockNumber <<= 9;
  }
  if (cardCommand(CMD17, blockNumber)) {
    error(SD_CARD_ERROR_CMD17);
    goto fail;
  }
  if (!readData(dst, 512)) {
    goto fail;
  }
  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readBlocks(uint32_t block, uint8_t* dst, size_t count) {
  if (!readStart(block)) {
    return false;
  }
  for (uint16_t b = 0; b < count; b++, dst += 512) {
    if (!readData(dst, 512)) {
      return false;
    }
  }
  return readStop();
}
//------------------------------------------------------------------------------
bool SdSpiCard::readData(uint8_t *dst) {
  return readData(dst, 512);
}
//------------------------------------------------------------------------------
bool SdSpiCard::readData(uint8_t* dst, size_t count) {
#if USE_SD_CRC
  uint16_t crc;
#endif  // USE_SD_CRC
  DBG_BEGIN_TIME(DBG_WAIT_READ);
  // wait for start block token
  uint16_t t0 = curTimeMS();
  while ((m_status = spiReceive()) == 0XFF) {
    if (isTimedOut(t0, SD_READ_TIMEOUT)) {
      error(SD_CARD_ERROR_READ_TIMEOUT);
      goto fail;
    }
  }
  DBG_END_TIME(DBG_WAIT_READ);
  if (m_status != DATA_START_BLOCK) {
    error(SD_CARD_ERROR_READ);
    goto fail;
  }
  // transfer data
  if ((m_status = spiReceive(dst, count))) {
    error(SD_CARD_ERROR_DMA);
    goto fail;
  }

#if USE_SD_CRC
  // get crc
  crc = (spiReceive() << 8) | spiReceive();
  if (crc != CRC_CCITT(dst, count)) {
    error(SD_CARD_ERROR_READ_CRC);
    goto fail;
  }
#else
  // discard crc
  spiReceive();
  spiReceive();
#endif  // USE_SD_CRC
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readOCR(uint32_t* ocr) {
  uint8_t *p = reinterpret_cast<uint8_t*>(ocr);
  if (cardCommand(CMD58, 0)) {
    error(SD_CARD_ERROR_CMD58);
    goto fail;
  }
  for (uint8_t i = 0; i < 4; i++) {
    p[3 - i] = spiReceive();
  }

  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
/** read CID or CSR register */
bool SdSpiCard::readRegister(uint8_t cmd, void* buf) {
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
  if (cardCommand(cmd, 0)) {
    error(SD_CARD_ERROR_READ_REG);
    goto fail;
  }
  if (!readData(dst, 16)) {
    goto fail;
  }
  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readStart(uint32_t blockNumber) {
  SD_TRACE("RS", blockNumber);
  if (type() != SD_CARD_TYPE_SDHC) {
    blockNumber <<= 9;
  }
  if (cardCommand(CMD18, blockNumber)) {
    error(SD_CARD_ERROR_CMD18);
    goto fail;
  }
//  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//-----------------------------------------------------------------------------
bool SdSpiCard::readStatus(uint8_t* status) {
  // retrun is R2 so read extra status byte.
  if (cardAcmd(ACMD13, 0) || spiReceive()) {
    error(SD_CARD_ERROR_ACMD13);
    goto fail;
  }
  if (!readData(status, 64)) {
    goto fail;
  }
  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//-----------------------------------------------------------------------------
void SdSpiCard::spiStart() {
  if (!m_spiActive) {
    spiActivate();
    spiSelect();
    m_spiActive = true;
  }
}
//-----------------------------------------------------------------------------
void SdSpiCard::spiStop() {
  if (m_spiActive) {
    spiUnselect();
    spiSend(0XFF);
    spiDeactivate();
    m_spiActive = false;
  }
}
//------------------------------------------------------------------------------
bool SdSpiCard::readStop() {
  if (cardCommand(CMD12, 0)) {
    error(SD_CARD_ERROR_CMD12);
    goto fail;
  }
  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
// wait for card to go not busy
bool SdSpiCard::waitNotBusy(uint16_t timeoutMS) {
  uint16_t t0 = curTimeMS();
#if WDT_YIELD_TIME_MICROS
  // Call isTimedOut first to insure yield is called.
  while (!isTimedOut(t0, timeoutMS)) {
    if (spiReceive() == 0XFF) {
      return true;
    }
  }
  return false;
#else  // WDT_YIELD_TIME_MICROS
  // Check not busy first since yield is not called in isTimedOut.
  while (spiReceive() != 0XFF) {
    if (isTimedOut(t0, timeoutMS)) {
      return false;
    }
  }
  return true;
#endif  // WDT_YIELD_TIME_MICROS
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeBlock(uint32_t blockNumber, const uint8_t* src) {
  SD_TRACE("WB", blockNumber);
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) {
    blockNumber <<= 9;
  }
  if (cardCommand(CMD24, blockNumber)) {
    error(SD_CARD_ERROR_CMD24);
    goto fail;
  }
  if (!writeData(DATA_START_BLOCK, src)) {
    goto fail;
  }


#if CHECK_FLASH_PROGRAMMING
  // wait for flash programming to complete
  DBG_BEGIN_TIME(DBG_WRITE_FLASH);
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    error(SD_CARD_ERROR_FLASH_PROGRAMMING);
    goto fail;
  }
  DBG_END_TIME(DBG_WRITE_FLASH);
  // response is r2 so get and check two bytes for nonzero
  if (cardCommand(CMD13, 0) || spiReceive()) {
    error(SD_CARD_ERROR_CMD13);
    goto fail;
  }
#endif  // CHECK_PROGRAMMING

  spiStop();
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeBlocks(uint32_t block, const uint8_t* src, size_t count) {
  if (!writeStart(block)) {
    goto fail;
  }
  for (size_t b = 0; b < count; b++, src += 512) {
    if (!writeData(src)) {
      goto fail;
    }
  }
  return writeStop();

 fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeData(const uint8_t* src) {
  // wait for previous write to finish
  DBG_BEGIN_TIME(DBG_WRITE_BUSY);
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    error(SD_CARD_ERROR_WRITE_TIMEOUT);
    goto fail;
  }
  DBG_END_TIME(DBG_WRITE_BUSY);
  if (!writeData(WRITE_MULTIPLE_TOKEN, src)) {
    goto fail;
  }
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
bool SdSpiCard::writeData(uint8_t token, const uint8_t* src) {
#if USE_SD_CRC
  uint16_t crc = CRC_CCITT(src, 512);
#else  // USE_SD_CRC
  uint16_t crc = 0XFFFF;
#endif  // USE_SD_CRC
  spiSend(token);
  spiSend(src, 512);
  spiSend(crc >> 8);
  spiSend(crc & 0XFF);

  m_status = spiReceive();
  if ((m_status & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    error(SD_CARD_ERROR_WRITE);
    goto fail;
  }
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeStart(uint32_t blockNumber) {
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) {
    blockNumber <<= 9;
  }
  if (cardCommand(CMD25, blockNumber)) {
    error(SD_CARD_ERROR_CMD25);
    goto fail;
  }
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
  SD_TRACE("WS", blockNumber);
  // send pre-erase count
  if (cardAcmd(ACMD23, eraseCount)) {
    error(SD_CARD_ERROR_ACMD23);
    goto fail;
  }
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) {
    blockNumber <<= 9;
  }
  if (cardCommand(CMD25, blockNumber)) {
    error(SD_CARD_ERROR_CMD25);
    goto fail;
  }
  return true;

fail:
  spiStop();
  return false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeStop() {
  DBG_BEGIN_TIME(DBG_WRITE_STOP);
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    goto fail;
  }
  DBG_END_TIME(DBG_WRITE_STOP);
  spiSend(STOP_TRAN_TOKEN);
  spiStop();
  return true;

fail:
  error(SD_CARD_ERROR_STOP_TRAN);
  spiStop();
  return false;
}
