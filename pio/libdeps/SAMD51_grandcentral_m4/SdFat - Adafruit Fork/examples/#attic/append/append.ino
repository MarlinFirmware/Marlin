/*
 * Append Example
 *
 * This program shows how to use open for append.
 * The program will append 100 line each time it opens the file.
 * The program will open and close the file 100 times.
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// file system object
SdFat sd;

// create Serial stream
ArduinoOutStream cout(Serial);

// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void setup() {
  // filename for this example
  char name[] = "append.txt";

  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  // F() stores strings in flash to save RAM
  cout << endl << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  cout << F("Appending to: ") << name;

  for (uint8_t i = 0; i < 100; i++) {
    // open stream for append
    ofstream sdout(name, ios::out | ios::app);
    if (!sdout) {
      error("open failed");
    }

    // append 100 lines to the file
    for (uint8_t j = 0; j < 100; j++) {
      // use int() so byte will print as decimal number
      sdout << "line " << int(j) << " of pass " << int(i);
      sdout << " millis = " << millis() << endl;
    }
    // close the stream
    sdout.close();

    if (!sdout) {
      error("append data failed");
    }

    // output progress indicator
    if (i % 25 == 0) {
      cout << endl;
    }
    cout << '.';
  }
  cout << endl << "Done" << endl;
}
//------------------------------------------------------------------------------
void loop() {}
