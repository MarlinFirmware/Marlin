/*
 * Read the logfile created by the eventlog.ino example.
 * Demo of pathnames and working directories
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// file system object
SdFat sd;

// define a serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
void setup() {
  int c;
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // set current working directory
  if (!sd.chdir("logs/2014/Jan/")) {
    sd.errorHalt("chdir failed. Did you run eventlog.ino?");
  }
  // open file in current working directory
  ifstream file("logfile.txt");

  if (!file.is_open()) {
    sd.errorHalt("open failed");
  }

  // copy the file to Serial
  while ((c = file.get()) >= 0) {
    cout << (char)c;
  }

  cout << "Done" << endl;
}
//------------------------------------------------------------------------------
void loop() {}