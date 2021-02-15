/*
 * This program is a simple binary write/read benchmark
 * for the standard Arduino SD.h library.
 */
#include <SPI.h>
#include <SD.h>

// SD chip select pin
const uint8_t chipSelect = SS;

#define FILE_SIZE_MB 5
#define FILE_SIZE (1000000UL*FILE_SIZE_MB)
#define BUF_SIZE 100

uint8_t buf[BUF_SIZE];

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

  // Discard any input.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);

  // F() stores strings in flash to save RAM
  Serial.println(F("Type any character to start"));
  
  while (!Serial.available()) {
    yield();
  }
  if (!SD.begin(chipSelect)) {
    error("begin");
  }

  // open or create file - truncate existing file.
  file = SD.open("Bench.dat", O_RDWR | O_TRUNC | O_CREAT);
  if (!file) {
    error("open failed");
  }

  // fill buf with known data
  for (uint16_t i = 0; i < (BUF_SIZE-2); i++) {
    buf[i] = 'A' + (i % 26);
  }
  buf[BUF_SIZE-2] = '\r';
  buf[BUF_SIZE-1] = '\n';

  Serial.print(F("File size "));
  Serial.print(FILE_SIZE_MB);
  Serial.println(F("MB"));
  Serial.print(F("Buffer size "));
  Serial.print(BUF_SIZE);
  Serial.println(F(" bytes"));
  Serial.println(F("Starting write test.  Please wait up to a minute"));

  // do write test
  uint32_t n = FILE_SIZE/sizeof(buf);
  maxLatency = 0;
  minLatency = 999999;
  totalLatency = 0;
  uint32_t t = millis();
  for (uint32_t i = 0; i < n; i++) {
    uint32_t m = micros();
    if (file.write(buf, sizeof(buf)) != sizeof(buf)) {
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
  Serial.print(F("Write "));
  Serial.print(s/t);
  Serial.print(F(" KB/sec\n"));
  Serial.print(F("Maximum latency: "));
  Serial.print(maxLatency);
  Serial.print(F(" usec, Minimum Latency: "));
  Serial.print(minLatency);
  Serial.print(F(" usec, Avg Latency: "));
  Serial.print(totalLatency/n);
  Serial.print(F(" usec\n\n"));
  Serial.println(F("Starting read test.  Please wait up to a minute"));
  // do read test
  file.seek(0);
  maxLatency = 0;
  minLatency = 99999;
  totalLatency = 0;
  t = millis();
  for (uint32_t i = 0; i < n; i++) {
    buf[BUF_SIZE-1] = 0;
    uint32_t m = micros();
    if (file.read(buf, sizeof(buf)) != sizeof(buf)) {
      error("read failed");
    }
    m = micros() - m;
    if (maxLatency < m) {
      maxLatency = m;
    }
    if (minLatency > m) {
      minLatency = m;
    }
    totalLatency += m;
    if (buf[BUF_SIZE-1] != '\n') {
      error("data check");
    }
  }
  t = millis() - t;
  Serial.print(F("Read "));
  Serial.print(s/t);
  Serial.print(F(" KB/sec\n"));
  Serial.print(F("Maximum latency: "));
  Serial.print(maxLatency);
  Serial.print(F(" usec, Minimum Latency: "));
  Serial.print(minLatency);
  Serial.print(F(" usec, Avg Latency: "));
  Serial.print(totalLatency/n);
  Serial.print(F(" usec\n\n"));
  Serial.print(F("Done\n\n"));
  file.close();
}