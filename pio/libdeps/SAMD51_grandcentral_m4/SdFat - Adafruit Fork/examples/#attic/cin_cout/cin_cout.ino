/*
 * Demo of ArduinoInStream and ArduinoOutStream
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// create serial output stream
ArduinoOutStream cout(Serial);

// input line buffer
char cinBuf[40];

// create serial input stream
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));
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
  int32_t n = 0;

  cout << "\nenter an integer\n";

  cin.readline();

  if (cin >> n) {
    cout << "The number is: " << n;
  } else {
    // will fail if no digits or not in range [-2147483648, 2147483647]
    cout << "Invalid input: " << cinBuf;
  }
  cout << endl;
}
