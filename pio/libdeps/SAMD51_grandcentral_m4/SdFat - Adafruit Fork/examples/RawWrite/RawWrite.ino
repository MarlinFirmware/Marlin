/*
 * This program illustrates raw write functions in SdFat that
 * can be used for high speed data logging.
 *
 * This program simulates logging from a source that produces
 * data at a constant rate of RATE_KB_PER_SEC.
 *
 * Note: Apps should create a very large file then truncates it
 * to the length that is used for a logging. It only takes
 * a few seconds to erase a 500 MB file since the card only
 * marks the blocks as erased; no data transfer is required.
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include "FreeStack.h"

// SD chip select pin
const uint8_t chipSelect = SS;

const uint32_t RATE_KB_PER_SEC = 100;

const uint32_t TEST_TIME_SEC = 100;

// Time between printing progress dots
const uint32_t DOT_TIME_MS = 5000UL;

// number of blocks in the contiguous file
const uint32_t BLOCK_COUNT = (1000*RATE_KB_PER_SEC*TEST_TIME_SEC + 511)/512;

// file system
SdFat sd;

// test file
SdFile file;

// file extent
uint32_t bgnBlock, endBlock;

// Serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
}
//------------------------------------------------------------------------------
void loop(void) {
  // Read any extra Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);
  // F stores strings in flash to save RAM
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }

  cout << F("FreeStack: ") << FreeStack() << endl;

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // delete possible existing file
  sd.remove("RawWrite.txt");

  // create a contiguous file
  if (!file.createContiguous("RawWrite.txt", 512UL*BLOCK_COUNT)) {
    error("createContiguous failed");
  }
  // get the location of the file's blocks
  if (!file.contiguousRange(&bgnBlock, &endBlock)) {
    error("contiguousRange failed");
  }
  //*********************NOTE**************************************
  // NO SdFile calls are allowed while cache is used for raw writes
  //***************************************************************

  // clear the cache and use it as a 512 byte buffer
  uint8_t* pCache = (uint8_t*)sd.vol()->cacheClear();

  // fill cache with eight lines of 64 bytes each
  memset(pCache, ' ', 512);
  for (uint16_t i = 0; i < 512; i += 64) {
    // put line number at end of line then CR/LF
    pCache[i + 61] = '0' + (i/64);
    pCache[i + 62] = '\r';
    pCache[i + 63] = '\n';
  }

  cout << F("Start raw write of ") << file.fileSize()/1000UL << F(" KB\n");
  cout << F("Target rate: ") << RATE_KB_PER_SEC << F(" KB/sec\n");
  cout << F("Target time: ") << TEST_TIME_SEC << F(" seconds\n");
  
  // tell card to setup for multiple block write with pre-erase
  if (!sd.card()->writeStart(bgnBlock, BLOCK_COUNT)) {
    error("writeStart failed");
  }
  // init stats

  delay(1000);
  uint32_t dotCount = 0;
  uint32_t maxQueuePrint = 0;
  uint32_t maxWriteTime = 0;
  uint32_t minWriteTime = 9999999;
  uint32_t totalWriteTime = 0;
  uint32_t maxQueueSize = 0;
  uint32_t nWrite = 0;
  uint32_t b = 0;

  // write data
    uint32_t startTime = millis();
  while (nWrite < BLOCK_COUNT) {
    uint32_t nProduced = RATE_KB_PER_SEC*(millis() - startTime)/512UL;
    uint32_t queueSize = nProduced - nWrite;
    if (queueSize == 0) continue;
    if (queueSize > maxQueueSize) {
      maxQueueSize = queueSize;
    }
    if ((millis() - startTime - dotCount*DOT_TIME_MS) > DOT_TIME_MS) {
      if (maxQueueSize != maxQueuePrint) {
        cout << F("\nQ: ") << maxQueueSize << endl;
        maxQueuePrint = maxQueueSize;
      } else {
        cout << ".";
        if (++dotCount%10 == 0) {
          cout << endl;
        }
      }
    }
    // put block number at start of first line in block
    uint32_t n = b++;
    for (int8_t d = 5; d >= 0; d--) {
      pCache[d] = n || d == 5 ? n % 10 + '0' : ' ';
      n /= 10;
    }
    // write a 512 byte block
    uint32_t tw = micros();
    if (!sd.card()->writeData(pCache)) {
      error("writeData failed");
    }
    tw = micros() - tw;
    totalWriteTime += tw;
    // check for max write time
    if (tw > maxWriteTime) {
      maxWriteTime = tw;
    }
    if (tw < minWriteTime) {
      minWriteTime = tw;
    }
    nWrite++;
  }
  uint32_t endTime = millis();
  uint32_t avgWriteTime = totalWriteTime/BLOCK_COUNT;
  // end multiple block write mode
  if (!sd.card()->writeStop()) {
    error("writeStop failed");
  }

  cout << F("\nDone\n");
  cout << F("maxQueueSize: ") << maxQueueSize << endl;
  cout << F("Elapsed time: ") << setprecision(3)<< 1.e-3*(endTime - startTime);
  cout << F(" seconds\n");
  cout << F("Min block write time: ") << minWriteTime << F(" micros\n");
  cout << F("Max block write time: ") << maxWriteTime << F(" micros\n");
  cout << F("Avg block write time: ") << avgWriteTime << F(" micros\n");  
  // close file for next pass of loop
  file.close();
  Serial.println();
}