/*
 * This program is a simple Print benchmark.
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include "FreeStack.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// number of lines to print
const uint16_t N_PRINT = 20000;

// file system
SdFat sd;

// test file
SdFile file;

// Serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
}
//------------------------------------------------------------------------------
void loop() {
  uint32_t maxLatency;
  uint32_t minLatency;
  uint32_t totalLatency;

  // Read any existing Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);
  // F stores strings in flash to save RAM
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }
  delay(400);  // catch Due reset problem

  cout << F("FreeStack: ") << FreeStack() << endl;

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  cout << F("Type is FAT") << int(sd.vol()->fatType()) << endl;

  cout << F("Starting print test.  Please wait.\n\n");

  // do write test
  for (int test = 0; test < 6; test++) {
    char fileName[13] = "bench0.txt";
    fileName[5] = '0' + test;
    // open or create file - truncate existing file.
    if (!file.open(fileName, O_RDWR | O_CREAT | O_TRUNC)) {
      error("open failed");
    }
    maxLatency = 0;
    minLatency = 999999;
    totalLatency = 0;
    switch(test) {
    case 0:
      cout << F("Test of println(uint16_t)\n");
      break;

    case 1:
      cout << F("Test of printField(uint16_t, char)\n");
      break;

    case 2:
      cout << F("Test of println(uint32_t)\n");
      break;

    case 3:
      cout << F("Test of printField(uint32_t, char)\n");
      break;
    case 4:
      cout << F("Test of println(float)\n");
      break;

    case 5:
      cout << F("Test of printField(float, char)\n");
      break;
    }

    uint32_t t = millis();
    for (uint16_t i = 0; i < N_PRINT; i++) {
      uint32_t m = micros();

      switch(test) {
      case 0:
        file.println(i);
        break;

      case 1:
        file.printField(i, '\n');
        break;

      case 2:
        file.println(12345678UL + i);
        break;

      case 3:
        file.printField((uint32_t) (12345678UL + i), '\n');
        break;

      case 4:
        file.println((float)0.01*i);
        break;

      case 5:
        file.printField((float)0.01*i, '\n');
        break;
      }
      if (file.getWriteError()) {
        error("write failed");
      }
      m = micros() - m;
      if (maxLatency < m) {
        maxLatency = m;
      }
      if (minLatency > m) {
        minLatency = m;
      }
      totalLatency += m;
    }
    file.close();
    t = millis() - t;
    double s = file.fileSize();
    cout << F("Time ") << 0.001*t << F(" sec\n");
    cout << F("File size ") << 0.001*s << F(" KB\n");
    cout << F("Write ") << s/t << F(" KB/sec\n");
    cout << F("Maximum latency: ") << maxLatency;
    cout << F(" usec, Minimum Latency: ") << minLatency;
    cout << F(" usec, Avg Latency: ");
    cout << totalLatency/N_PRINT << F(" usec\n\n");
  }
  cout << F("Done!\n\n");
}
