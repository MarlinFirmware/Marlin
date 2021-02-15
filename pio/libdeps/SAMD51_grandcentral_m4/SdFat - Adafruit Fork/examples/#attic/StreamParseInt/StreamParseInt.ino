// Simple demo of the Stream parsInt() member function.
#include <SPI.h>
// The next two lines replace #include <SD.h>.
#include "SdFat.h"
SdFat SD;

// SD card chip select pin - Modify the value of csPin for your SD module.
const uint8_t csPin = SS;

File file;
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while(!Serial) {
    SysCall::yield();
  }
  Serial.println(F("Type any character to start"));
  while (!Serial.available()) {
    SysCall::yield(); 
  }
  // Initialize the SD.
  if (!SD.begin(csPin)) {
    Serial.println(F("begin error"));
    return;
  }
  // Create and open the file.  Use flag to truncate an existing file.
  file = SD.open("stream.txt", O_RDWR|O_CREAT|O_TRUNC);
  if (!file) {
    Serial.println(F("open error"));
    return;
  }
  // Write a test number to the file.
  file.println("12345");

  // Rewind the file and read the number with parseInt().
  file.seek(0);
  int i = file.parseInt();
  Serial.print(F("parseInt: "));
  Serial.println(i);
  file.close();
}

void loop() {}