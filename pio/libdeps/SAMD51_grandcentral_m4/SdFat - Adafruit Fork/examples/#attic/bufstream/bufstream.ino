/*
 * Use of ibufsteam to parse a line and obufstream to format a line
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// create a serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
void setup() {
  char buf[20];   // buffer for formatted line
  int i, j, k;    // values from parsed line

  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  delay(2000);

  // initialize input string
  ibufstream bin("123 456 789");

  // parse the string "123 456 789"
  bin >> i >> j >> k;

  // initialize output buffer
  obufstream bout(buf, sizeof(buf));

  // format the output string
  bout << k << ',' << j << ',' << i << endl;

  // write the string to serial
  cout << buf;
}

void loop() {}
