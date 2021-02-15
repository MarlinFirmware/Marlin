/*
 * This program is a simple Print benchmark.
 */
#include <SPI.h>
#include <SD.h>

// SD chip select pin
const uint8_t chipSelect = SS;

// number of lines to print
const uint16_t N_PRINT = 20000;


// test file
File file;

//------------------------------------------------------------------------------
void error(const char* s) {
  Serial.println(s);
  while (1) {
    yield();
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    yield();
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

  // F() stores strings in flash to save RAM
  Serial.println(F("Type any character to start"));
  while (!Serial.available()) {
    yield();
  }

  // initialize the SD card
  if (!SD.begin(chipSelect)) {
    error("begin");
  }
  
  Serial.println(F("Starting print test.  Please wait.\n"));

  // do write test
  for (int test = 0; test < 2; test++) {
    file = SD.open("bench.txt", FILE_WRITE);

    if (!file) {
      error("open failed");
    }
    switch(test) {
    case 0:
      Serial.println(F("Test of println(uint16_t)"));
      break;

    case 1:
      Serial.println(F("Test of println(double)"));
      break;
    }
    maxLatency = 0;
    minLatency = 999999;
    totalLatency = 0;
    uint32_t t = millis();
    for (uint16_t i = 0; i < N_PRINT; i++) {
      uint32_t m = micros();

      switch(test) {
      case 0:
        file.println(i);
        break;

      case 1:
        file.println((double)0.01*i);
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
    file.flush();
    t = millis() - t;
    double s = file.size();
    Serial.print(F("Time "));
    Serial.print(0.001*t);
    Serial.println(F(" sec"));
    Serial.print(F("File size "));
    Serial.print(0.001*s);
    Serial.print(F(" KB\n"));
    Serial.print(F("Write "));
    Serial.print(s/t);
    Serial.print(F(" KB/sec\n"));
    Serial.print(F("Maximum latency: "));
    Serial.print(maxLatency);
    Serial.print(F(" usec, Minimum Latency: "));
    Serial.print(minLatency);
    Serial.print(F(" usec, Avg Latency: "));
    Serial.print(totalLatency/N_PRINT);
    Serial.println(F(" usec\n"));
    SD.remove("bench.txt");
  }
  file.close();
  Serial.println(F("Done!\n"));
}