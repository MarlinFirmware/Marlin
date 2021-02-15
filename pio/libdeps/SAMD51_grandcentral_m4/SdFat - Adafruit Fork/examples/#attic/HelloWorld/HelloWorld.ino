#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

//  create a serial output stream
ArduinoOutStream cout(Serial);

void setup() {
  Serial.begin(9600);

  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  delay(2000);

  cout << "Hello, World!\n";
}

void loop() {}
