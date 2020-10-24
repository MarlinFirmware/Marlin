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

#include "draw_ui.h"
#include "wifi_module.h"
#include "wifi_upload.h"

#include "../../../../MarlinCore.h"

#define WIFI_SET()        WRITE(WIFI_RESET_PIN, HIGH);
#define WIFI_RESET()      WRITE(WIFI_RESET_PIN, LOW);
#define WIFI_IO1_SET()    WRITE(WIFI_IO1_PIN, HIGH);
#define WIFI_IO1_RESET()  WRITE(WIFI_IO1_PIN, LOW);

extern SZ_USART_FIFO  WifiRxFifo;

extern int readUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len);
extern int writeUsartFifo(SZ_USART_FIFO * fifo, int8_t * buf, int32_t len);
extern void esp_port_begin(uint8_t interrupt);
extern int usartFifoAvailable(SZ_USART_FIFO *fifo);
extern void wifi_delay(int n);

#define ARRAY_SIZE(a) sizeof(a) / sizeof((a)[0])

//typedef signed char bool;

// ESP8266 command codes
const uint8_t ESP_FLASH_BEGIN = 0x02;
const uint8_t ESP_FLASH_DATA = 0x03;
const uint8_t ESP_FLASH_END = 0x04;
const uint8_t ESP_MEM_BEGIN = 0x05;
const uint8_t ESP_MEM_END = 0x06;
const uint8_t ESP_MEM_DATA = 0x07;
const uint8_t ESP_SYNC = 0x08;
const uint8_t ESP_WRITE_REG = 0x09;
const uint8_t ESP_READ_REG = 0x0A;

// MAC address storage locations
const uint32_t ESP_OTP_MAC0 = 0x3FF00050;
const uint32_t ESP_OTP_MAC1 = 0x3FF00054;
const uint32_t ESP_OTP_MAC2 = 0x3FF00058;
const uint32_t ESP_OTP_MAC3 = 0x3FF0005C;

const size_t EspFlashBlockSize = 0x0400;      // 1K byte blocks

const uint8_t ESP_IMAGE_MAGIC = 0xE9;
const uint8_t ESP_CHECKSUM_MAGIC = 0xEF;

const uint32_t ESP_ERASE_CHIP_ADDR = 0x40004984;  // &SPIEraseChip
const uint32_t ESP_SEND_PACKET_ADDR = 0x40003C80; // &send_packet
const uint32_t ESP_SPI_READ_ADDR = 0x40004B1C;    // &SPIRead
const uint32_t ESP_UNKNOWN_ADDR = 0x40001121;   // not used
const uint32_t ESP_USER_DATA_RAM_ADDR = 0x3FFE8000; // &user data ram
const uint32_t ESP_IRAM_ADDR = 0x40100000;      // instruction RAM
const uint32_t ESP_FLASH_ADDR = 0x40200000;     // address of start of Flash
//const uint32_t ESP_FLASH_READ_STUB_BEGIN = IRAM_ADDR + 0x18;

UPLOAD_STRUCT esp_upload;

static const unsigned int retriesPerReset = 3;
static const uint32_t connectAttemptInterval = 50;
static const unsigned int percentToReportIncrement = 5; // how often we report % complete
static const uint32_t defaultTimeout = 500;
static const uint32_t eraseTimeout = 15000;
static const uint32_t blockWriteTimeout = 200;
static const uint32_t blockWriteInterval = 15;      // 15ms is long enough, 10ms is mostly too short

// Messages corresponding to result codes, should make sense when followed by " error"
const char *resultMessages[] = {
  "no",
  "timeout",
  "comm write",
  "connect",
  "bad reply",
  "file read",
  "empty file",
  "response header",
  "slip frame",
  "slip state",
  "slip data"
};

// A note on baud rates.
// The ESP8266 supports 921600, 460800, 230400, 115200, 74880 and some lower baud rates.
// 921600b is not reliable because even though it sometimes succeeds in connecting, we get a bad response during uploading after a few blocks.
// Probably our UART ISR cannot receive bytes fast enough, perhaps because of the latency of the system tick ISR.
// 460800b doesn't always manage to connect, but if it does then uploading appears to be reliable.
// 230400b always manages to connect.
static const uint32_t uploadBaudRates[] = { 460800, 230400, 115200, 74880 };

signed char IsReady() {
  return esp_upload.state == upload_idle;
}

void uploadPort_write(const uint8_t *buf, size_t len) {
  #if 0
  int i;

  for (i = 0; i < len; i++) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) { /* nada */ }
    USART_SendData(USART1, *(buf + i));
  }
  #endif
}

char uploadPort_read() {
  uint8_t retChar;
  if (readUsartFifo(&WifiRxFifo, (int8_t *)&retChar, 1) == 1)
    return retChar;
  else
    return 0;
}

int uploadPort_available() {
  return usartFifoAvailable(&WifiRxFifo);
}

void uploadPort_begin() {
  esp_port_begin(1);
}

void uploadPort_close() {
  //WIFI_COM.end();
  //WIFI_COM.begin(115200, true);
  esp_port_begin(0);
}

void flushInput() {
  while (uploadPort_available() != 0) {
    (void)uploadPort_read();
    //IWDG_ReloadCounter();
  }
}

// Extract 1-4 bytes of a value in little-endian order from a buffer beginning at a specified offset
uint32_t getData(unsigned byteCnt, const uint8_t *buf, int ofst) {
  uint32_t val = 0;

  if (buf && byteCnt) {
    unsigned int shiftCnt = 0;
    if (byteCnt > 4)
      byteCnt = 4;
    do{
      val |= (uint32_t)buf[ofst++] << shiftCnt;
      shiftCnt += 8;
    } while (--byteCnt);
  }
  return(val);
}

// Put 1-4 bytes of a value in little-endian order into a buffer beginning at a specified offset.
void putData(uint32_t val, unsigned byteCnt, uint8_t *buf, int ofst) {
  if (buf && byteCnt) {
    if (byteCnt > 4) {
      byteCnt = 4;
    }
    do {
      buf[ofst++] = (uint8_t)(val & 0xFF);
      val >>= 8;
    } while (--byteCnt);
  }
}

// Read a byte optionally performing SLIP decoding.  The return values are:
//
//  2 - an escaped byte was read successfully
//  1 - a non-escaped byte was read successfully
//  0 - no data was available
//   -1 - the value 0xC0 was encountered (shouldn't happen)
//   -2 - a SLIP escape byte was found but the following byte wasn't available
//   -3 - a SLIP escape byte was followed by an invalid byte
int ReadByte(uint8_t *data, signed char slipDecode) {
  if (uploadPort_available() == 0) {
    return(0);
  }

  // at least one byte is available
  *data = uploadPort_read();
  if (!slipDecode) {
    return(1);
  }

  if (*data == 0xC0) {
    // this shouldn't happen
    return(-1);
  }

  // if not the SLIP escape, we're done
  if (*data != 0xDB) {
    return(1);
  }

  // SLIP escape, check availability of subsequent byte
  if (uploadPort_available() == 0) {
    return(-2);
  }

  // process the escaped byte
  *data = uploadPort_read();
  if (*data == 0xDC) {
    *data = 0xC0;
    return(2);
  }

  if (*data == 0xDD) {
    *data = 0xDB;
    return(2);
  }
  // invalid
  return(-3);
}
// When we write a sync packet, there must be no gaps between most of the characters.
// So use this function, which does a block write to the UART buffer in the latest CoreNG.
void _writePacketRaw(const uint8_t *buf, size_t len) {
  uploadPort_write(buf, len);
}

// Write a byte to the serial port optionally SLIP encoding. Return the number of bytes actually written.
void WriteByteRaw(uint8_t b) {
  uploadPort_write((const uint8_t *)&b, 1);
}

// Write a byte to the serial port optionally SLIP encoding. Return the number of bytes actually written.
void WriteByteSlip(uint8_t b) {
  if (b == 0xC0) {
    WriteByteRaw(0xDB);
    WriteByteRaw(0xDC);
  }
  else if (b == 0xDB) {
    WriteByteRaw(0xDB);
    WriteByteRaw(0xDD);
  }
  else {
    uploadPort_write((const uint8_t *)&b, 1);
  }
}

// Wait for a data packet to be returned.  If the body of the packet is
// non-zero length, return an allocated buffer indirectly containing the
// data and return the data length. Note that if the pointer for returning
// the data buffer is nullptr, the response is expected to be two bytes of zero.
//
// If an error occurs, return a negative value.  Otherwise, return the number
// of bytes in the response (or zero if the response was not the standard "two bytes of zero").
EspUploadResult readPacket(uint8_t op, uint32_t *valp, size_t *bodyLen, uint32_t msTimeout) {
  typedef enum {
    begin = 0,
    header,
    body,
    end,
    done
  } PacketState;

  uint8_t resp, opRet;

  const size_t headerLength = 8;

  uint32_t startTime = getWifiTick();
  uint8_t hdr[headerLength];
  uint16_t hdrIdx = 0;

  uint16_t bodyIdx = 0;
  uint8_t respBuf[2];

  // wait for the response
  uint16_t needBytes = 1;

  PacketState state = begin;

  *bodyLen = 0;

  while (state != done) {
    uint8_t c;
    EspUploadResult stat;

    //IWDG_ReloadCounter();

    if (getWifiTickDiff(startTime, getWifiTick()) > msTimeout) {
      return(timeout);
    }

    if (uploadPort_available() < needBytes) {
      // insufficient data available
      // preferably, return to Spin() here
      continue;
    }

    // sufficient bytes have been received for the current state, process them
    switch (state) {
      case begin: // expecting frame start
        c = uploadPort_read();
        if (c != (uint8_t)0xC0) {
          break;
        }
        state = header;
        needBytes = 2;

        break;
      case end:   // expecting frame end
        c = uploadPort_read();
        if (c != (uint8_t)0xC0) {
          return slipFrame;
        }
        state = done;

        break;

      case header:  // reading an 8-byte header
      case body:    // reading the response body
        {
          int rslt;
          // retrieve a byte with SLIP decoding
          rslt = ReadByte(&c, 1);
          if (rslt != 1 && rslt != 2) {
            // some error occurred
            stat = (rslt == 0 || rslt == -2) ? slipData : slipFrame;
            return stat;
          }
          else if (state == header) {
            //store the header byte
            hdr[hdrIdx++] = c;
            if (hdrIdx >= headerLength) {
              // get the body length, prepare a buffer for it
              *bodyLen = (uint16_t)getData(2, hdr, 2);

              // extract the value, if requested
              if (valp != 0) {
                *valp = getData(4, hdr, 4);
              }

              if (*bodyLen != 0) {
                state = body;
              }
              else {
                needBytes = 1;
                state = end;
              }
            }
          }
          else {
            // Store the response body byte, check for completion
            if (bodyIdx < ARRAY_SIZE(respBuf)) {
              respBuf[bodyIdx] = c;
            }
            ++bodyIdx;
            if (bodyIdx >= *bodyLen) {
              needBytes = 1;
              state = end;
            }
          }
        }
        break;

      default:    // this shouldn't happen
        return slipState;
    }
  }

  // Extract elements from the header
  resp = (uint8_t)getData(1, hdr, 0);
  opRet = (uint8_t)getData(1, hdr, 1);
  // Sync packets often provoke a response with a zero opcode instead of ESP_SYNC
  if (resp != 0x01 || opRet != op) {
    //printf("resp %02x %02x\n", resp, opRet); //debug
    return respHeader;
  }

  return success;
}

// Send a block of data performing SLIP encoding of the content.
void _writePacket(const uint8_t *data, size_t len) {
  unsigned char outBuf[2048] = {0};
  unsigned int outIndex = 0;
  while (len != 0) {
    if (*data == 0xC0) {
      outBuf[outIndex++] = 0xDB;
      outBuf[outIndex++] = 0xDC;
    }
    else if (*data == 0xDB) {
      outBuf[outIndex++] = 0xDB;
      outBuf[outIndex++] = 0xDD;
    }
    else {
      outBuf[outIndex++] = *data;

    }
    data++;
    --len;
  }
  uploadPort_write((const uint8_t *)outBuf, outIndex);
}

// Send a packet to the serial port while performing SLIP framing. The packet data comprises a header and an optional data block.
// A SLIP packet begins and ends with 0xC0.  The data encapsulated has the bytes
// 0xC0 and 0xDB replaced by the two-byte sequences {0xDB, 0xDC} and {0xDB, 0xDD} respectively.

void writePacket(const uint8_t *hdr, size_t hdrLen, const uint8_t *data, size_t dataLen) {
  WriteByteRaw(0xC0);           // send the packet start character
  _writePacket(hdr, hdrLen);    // send the header
  _writePacket(data, dataLen);  // send the data block
  WriteByteRaw(0xC0);           // send the packet end character
}

// Send a packet to the serial port while performing SLIP framing. The packet data comprises a header and an optional data block.
// This is like writePacket except that it does a fast block write for both the header and the main data with no SLIP encoding. Used to send sync commands.
void writePacketRaw(const uint8_t *hdr, size_t hdrLen, const uint8_t *data, size_t dataLen) {
  WriteByteRaw(0xC0);             // send the packet start character
  _writePacketRaw(hdr, hdrLen);   // send the header
  _writePacketRaw(data, dataLen); // send the data block in raw mode
  WriteByteRaw(0xC0);             // send the packet end character
}

// Send a command to the attached device together with the supplied data, if any.
// The data is supplied via a list of one or more segments.
void sendCommand(uint8_t op, uint32_t checkVal, const uint8_t *data, size_t dataLen) {
  // populate the header
  uint8_t hdr[8];
  putData(0, 1, hdr, 0);
  putData(op, 1, hdr, 1);
  putData(dataLen, 2, hdr, 2);
  putData(checkVal, 4, hdr, 4);

  // send the packet
  //flushInput();
  if (op == ESP_SYNC)
    writePacketRaw(hdr, sizeof(hdr), data, dataLen);
  else
    writePacket(hdr, sizeof(hdr), data, dataLen);
}

// Send a command to the attached device together with the supplied data, if any, and get the response
EspUploadResult doCommand(uint8_t op, const uint8_t *data, size_t dataLen, uint32_t checkVal, uint32_t *valp, uint32_t msTimeout) {
  size_t bodyLen;
  EspUploadResult stat;

  sendCommand(op, checkVal, data, dataLen);

  stat = readPacket(op, valp, &bodyLen, msTimeout);
  if (stat == success && bodyLen != 2)
    stat = badReply;

  return stat;
}

// Send a synchronising packet to the serial port in an attempt to induce
// the ESP8266 to auto-baud lock on the baud rate.
EspUploadResult Sync(uint16_t timeout) {
  uint8_t buf[36];
  EspUploadResult stat;
  int i ;

  // compose the data for the sync attempt
  memset(buf, 0x55, sizeof(buf));
  buf[0] = 0x07;
  buf[1] = 0x07;
  buf[2] = 0x12;
  buf[3] = 0x20;

  stat = doCommand(ESP_SYNC, buf, sizeof(buf), 0, 0, timeout);

  // If we got a response other than sync, discard it and wait for a sync response. This happens at higher baud rates.
  for (i = 0; i < 10 && stat == respHeader; ++i) {
    size_t bodyLen;
    stat = readPacket(ESP_SYNC, 0, &bodyLen, timeout);
  }

  if (stat == success) {
    // Read and discard additional replies
    for (;;) {
      size_t bodyLen;
      EspUploadResult rc = readPacket(ESP_SYNC, 0, &bodyLen, defaultTimeout);
      if (rc != success || bodyLen != 2) {
        break;
      }
    }
  }
  //DEBUG
  //else debug//printf("stat=%d\n", (int)stat);
  return stat;
}

// Send a command to the device to begin the Flash process.
EspUploadResult flashBegin(uint32_t addr, uint32_t size) {
  // determine the number of blocks represented by the size
  uint32_t blkCnt;
  uint8_t buf[16];
  uint32_t timeout;

  blkCnt = (size + EspFlashBlockSize - 1) / EspFlashBlockSize;

  // ensure that the address is on a block boundary
  addr &= ~(EspFlashBlockSize - 1);

  // begin the Flash process
  putData(size, 4, buf, 0);
  putData(blkCnt, 4, buf, 4);
  putData(EspFlashBlockSize, 4, buf, 8);
  putData(addr, 4, buf, 12);

  timeout = (size != 0) ? eraseTimeout : defaultTimeout;
  return doCommand(ESP_FLASH_BEGIN, buf, sizeof(buf), 0, 0, timeout);
}

// Send a command to the device to terminate the Flash process
EspUploadResult flashFinish(signed char reboot) {
  uint8_t buf[4];
  putData(reboot ? 0 : 1, 4, buf, 0);
  return doCommand(ESP_FLASH_END, buf, sizeof(buf), 0, 0, defaultTimeout);
}

// Compute the checksum of a block of data
uint16_t checksum(const uint8_t *data, uint16_t dataLen, uint16_t cksum) {
  if (data != nullptr) {
    while (dataLen--) {
      cksum ^= (uint16_t)*data++;
    }
  }
  return(cksum);
}

EspUploadResult flashWriteBlock(uint16_t flashParmVal, uint16_t flashParmMask) {
  #if 0
  const uint32_t blkSize = EspFlashBlockSize;
  int i;

  // Allocate a data buffer for the combined header and block data
  const uint16_t hdrOfst = 0;
  const uint16_t dataOfst = 16;
  const uint16_t blkBufSize = dataOfst + blkSize;
  uint32_t blkBuf32[blkBufSize/4];
  uint8_t * const blkBuf = (uint8_t*)(blkBuf32);
  uint32_t cnt;
  uint16_t cksum;
  EspUploadResult stat;

  // Prepare the header for the block
  putData(blkSize, 4, blkBuf, hdrOfst + 0);
  putData(esp_upload.uploadBlockNumber, 4, blkBuf, hdrOfst + 4);
  putData(0, 4, blkBuf, hdrOfst + 8);
  putData(0, 4, blkBuf, hdrOfst + 12);

  // Get the data for the block
  f_read(&esp_upload.uploadFile, blkBuf + dataOfst,  blkSize, &cnt );//->Read(reinterpret_cast<char *>(blkBuf + dataOfst), blkSize);
  if (cnt != blkSize) {
    if (f_tell(&esp_upload.uploadFile) == esp_upload.fileSize) {
      // partial last block, fill the remainder
      memset(blkBuf + dataOfst + cnt, 0xFF, blkSize - cnt);
    }
    else {
      return fileRead;
    }
  }

  // Patch the flash parameters into the first block if it is loaded at address 0
  if (esp_upload.uploadBlockNumber == 0 && esp_upload.uploadAddress == 0 && blkBuf[dataOfst] == ESP_IMAGE_MAGIC && flashParmMask != 0) {
    // update the Flash parameters
    uint32_t flashParm = getData(2, blkBuf + dataOfst + 2, 0) & ~(uint32_t)flashParmMask;
    putData(flashParm | flashParmVal, 2, blkBuf + dataOfst + 2, 0);
  }

  // Calculate the block checksum
  cksum = checksum(blkBuf + dataOfst, blkSize, ESP_CHECKSUM_MAGIC);

  for (i = 0; i < 3; i++) {
    if ((stat = doCommand(ESP_FLASH_DATA, blkBuf, blkBufSize, cksum, 0, blockWriteTimeout)) == success) {
      break;
    }
  }

  //printf("Upload %d\%\n", ftell(&esp_upload.uploadFile) * 100 / esp_upload.fileSize);

  return stat;
  #else
    return success;
  #endif
}

void upload_spin() {
  #if 0
  switch (esp_upload.state) {
  case resetting:

    if (esp_upload.connectAttemptNumber == 9) {
      // Time to give up
      //Network::ResetWiFi();
      esp_upload.uploadResult = connected;
      esp_upload.state = done;
    }
    else {

      // Reset the serial port at the new baud rate. Also reset the ESP8266.
      //  const uint32_t baud = uploadBaudRates[esp_upload.connectAttemptNumber/esp_upload.retriesPerBaudRate];
      if (esp_upload.connectAttemptNumber % esp_upload.retriesPerBaudRate == 0) {
      }
      //uploadPort.begin(baud);
      //uploadPort_close();

      uploadPort_begin();

      wifi_delay(2000);

      flushInput();

      esp_upload.lastAttemptTime = esp_upload.lastResetTime = getWifiTick();
      esp_upload.state = connecting;
    }

    break;

  case connecting:
    if ((getWifiTickDiff(esp_upload.lastAttemptTime, getWifiTick()) >= connectAttemptInterval) && (getWifiTickDiff(esp_upload.lastResetTime, getWifiTick()) >= 500)) {
      // Attempt to establish a connection to the ESP8266.
      EspUploadResult res = Sync(5000);
      esp_upload.lastAttemptTime = getWifiTick();
      if (res == success) {
        // Successful connection
        //MessageF(" success on attempt %d\n", (connectAttemptNumber % retriesPerBaudRate) + 1);
        //printf("connect success\n");
        esp_upload.state = erasing;
      }
      else {
        // This attempt failed
        esp_upload.connectAttemptNumber++;
        if (esp_upload.connectAttemptNumber % retriesPerReset == 0) {
          esp_upload.state = resetting;   // try a reset and a lower baud rate
        }
      }
    }
    break;

  case erasing:
    if (getWifiTickDiff(esp_upload.lastAttemptTime, getWifiTick()) >= blockWriteInterval) {
      uint32_t eraseSize;
      const uint32_t sectorsPerBlock = 16;
      const uint32_t sectorSize = 4096;
      const uint32_t numSectors = (esp_upload.fileSize + sectorSize - 1)/sectorSize;
      const uint32_t startSector = esp_upload.uploadAddress/sectorSize;

      uint32_t headSectors = sectorsPerBlock - (startSector % sectorsPerBlock);
      NOMORE(headSectors, numSectors);

      eraseSize = (numSectors < 2 * headSectors)
                ? (numSectors + 1) / 2 * sectorSize
                : (numSectors - headSectors) * sectorSize;

      //MessageF("Erasing %u bytes...\n", fileSize);
      esp_upload.uploadResult = flashBegin(esp_upload.uploadAddress, eraseSize);
      if (esp_upload.uploadResult == success) {
        //MessageF("Uploading file...\n");
        esp_upload.uploadBlockNumber = 0;
        esp_upload.uploadNextPercentToReport = percentToReportIncrement;
        esp_upload.lastAttemptTime = getWifiTick();
        esp_upload.state = uploading;
      }
      else {
        //MessageF("Erase failed\n");
        esp_upload.state = done;
      }
    }
    break;

  case uploading:
    // The ESP needs several milliseconds to recover from one packet before it will accept another
    if (getWifiTickDiff(esp_upload.lastAttemptTime, getWifiTick()) >= 15) {
      unsigned int percentComplete;
      const uint32_t blkCnt = (esp_upload.fileSize + EspFlashBlockSize - 1) / EspFlashBlockSize;
      if (esp_upload.uploadBlockNumber < blkCnt) {
        esp_upload.uploadResult = flashWriteBlock(0, 0);
        esp_upload.lastAttemptTime = getWifiTick();
        if (esp_upload.uploadResult != success) {
          //MessageF("Flash block upload failed\n");
          esp_upload.state = done;
        }
        percentComplete = (100 * esp_upload.uploadBlockNumber)/blkCnt;
        ++esp_upload.uploadBlockNumber;
        if (percentComplete >= esp_upload.uploadNextPercentToReport) {
          //MessageF("%u%% complete\n", percentComplete);
          esp_upload.uploadNextPercentToReport += percentToReportIncrement;
        }
      }
      else {
        esp_upload.state = done;
      }
    }
    break;

  case done:
    f_close(&esp_upload.uploadFile);
    //uploadPort.end();
    //uploadPort_close();

    //WIFI_COM.begin(115200, true);
    //wifi_init();

    if (esp_upload.uploadResult == success) {
      //printf("upload successfully\n");
    }
    else {
      //printf("upload failed\n");
    }
    esp_upload.state = upload_idle;//idle;
    break;

  default:
    break;
  }
  #endif
}

// Try to upload the given file at the given address
void SendUpdateFile(const char *file, uint32_t address) {
  #if 0
  FRESULT res = f_open(&esp_upload.uploadFile, file,  FA_OPEN_EXISTING | FA_READ);

  if (res !=  FR_OK) return;

  esp_upload.fileSize = f_size(&esp_upload.uploadFile);
  if (esp_upload.fileSize == 0) {
    f_close(&esp_upload.uploadFile);
    return;
  }
  f_lseek(&esp_upload.uploadFile, 0);

  esp_upload.uploadAddress = address;
  esp_upload.connectAttemptNumber = 0;
  esp_upload.state = resetting;
  #endif
}

static const uint32_t FirmwareAddress = 0x00000000, WebFilesAddress = 0x00100000;

void ResetWiFiForUpload(int begin_or_end) {
  #if 0
    uint32_t start, now;

    GPIO_InitTypeDef GPIO_InitStructure;

    #if V1_0_V1_1
      GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStructure.Pin = GPIO_Pin_8;
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    #else
      GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStructure.Pin = GPIO_Pin_13;
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    #endif
    start = getWifiTick();
    now = start;

    if (begin_or_end == 0) {
      #if V1_0_V1_1
        HAL_GPIO_WritePin(GPIOA,GPIO_Pin_8,GPIO_PIN_RESET); //update mode
      #else
        HAL_GPIO_WritePin(GPIOC,GPIO_Pin_13,GPIO_PIN_RESET); //update mode
      #endif
    }
    else {
      #if V1_0_V1_1
        #if V1_0_V1_1
          HAL_GPIO_WritePin(GPIOA,GPIO_Pin_8,GPIO_PIN_SET); //boot mode
          GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
          GPIO_InitStructure.Pin = GPIO_Pin_8;
          GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
          HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        #endif
      #else
        HAL_GPIO_WritePin(GPIOC,GPIO_Pin_13,GPIO_PIN_SET); //boot mode
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStructure.Pin = GPIO_Pin_13;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
      #endif
    }
    WIFI_RESET();
    while (getWifiTickDiff(start, now) < 500) now = getWifiTick();
    WIFI_SET();
  #endif
}

int32_t wifi_upload(int type) {
  esp_upload.retriesPerBaudRate = 9;

  ResetWiFiForUpload(0);

  if (type == 0)
    SendUpdateFile(ESP_FIRMWARE_FILE, FirmwareAddress);
  else if (type == 1)
    SendUpdateFile(ESP_WEB_FIRMWARE_FILE, FirmwareAddress);
  else if (type == 2)
    SendUpdateFile(ESP_WEB_FILE, WebFilesAddress);
  else
    return -1;

  while (esp_upload.state != upload_idle) {
    upload_spin();
    //IWDG_ReloadCounter();
  }

  ResetWiFiForUpload(1);

  return esp_upload.uploadResult == success ? 0 : -1;
}

#endif  // HAS_TFT_LVGL_UI
