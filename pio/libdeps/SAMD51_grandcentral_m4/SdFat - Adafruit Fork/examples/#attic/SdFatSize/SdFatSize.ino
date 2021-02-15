/*
 * Program to compare size of SdFat with Arduino SD library.
 * See SD_Size.ino for Arduino SD program.
 *
 */
#include <SPI.h>
#include "SdFat.h"

SdFat sd;

SdFile file;
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  
  if (!sd.begin()) {
    Serial.println("begin failed");
    return;
  }
  file.open("SizeTest.txt", O_RDWR | O_CREAT | O_AT_END);

  file.println("Hello");

  file.close();
  Serial.println("Done");
}
//------------------------------------------------------------------------------
void loop() {}
