/*
 * Calculate the sum and average of a list of floating point numbers
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// object for the SD file system
SdFat sd;

// define a serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
void writeTestFile() {
  // open the output file
  ofstream sdout("AvgTest.txt");

  // write a series of float numbers
  for (int16_t i = -1001; i < 2000; i += 13) {
    sdout << 0.1 * i << endl;
  }
  if (!sdout) {
    sd.errorHalt("sdout failed");
  }

  sdout.close();
}
//------------------------------------------------------------------------------
void calcAverage() {
  uint16_t n = 0;  // count of input numbers
  double num;      // current input number
  double sum = 0;  // sum of input numbers

  // open the input file
  ifstream sdin("AvgTest.txt");

  // check for an open failure
  if (!sdin) {
    sd.errorHalt("sdin failed");
  }

  // read and sum the numbers
  while (sdin >> num) {
    n++;
    sum += num;
  }

  // print the results
  cout << "sum of " << n << " numbers = " << sum << endl;
  cout << "average = " << sum/n << endl;
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  // F() stores strings in flash to save RAM
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // write the test file
  writeTestFile();

  // read the test file and calculate the average
  calcAverage();
}
//------------------------------------------------------------------------------
void loop() {}
