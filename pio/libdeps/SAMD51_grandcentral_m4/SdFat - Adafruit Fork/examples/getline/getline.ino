/*
 * Example of getline from section 27.7.1.3 of the C++ standard
 * Demonstrates the behavior of getline for various exceptions.
 * See http://www.cplusplus.com/reference/iostream/istream/getline/
 *
 * Note: This example is meant to demonstrate subtleties the standard and
 * may not the best way to read a file.
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// file system object
SdFat sd;

// create a serial stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
void makeTestFile() {
  ofstream sdout("getline.txt");
  // use flash for text to save RAM
  sdout << F(
          "short line\n"
          "\n"
          "17 character line\n"
          "too long for buffer\n"
          "line with no nl");

  sdout.close();
}
//------------------------------------------------------------------------------
void testGetline() {
  const int line_buffer_size = 18;
  char buffer[line_buffer_size];
  ifstream sdin("getline.txt");
  int line_number = 0;

  while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
    int count = sdin.gcount();
    if (sdin.fail()) {
      cout << "Partial long line";
      sdin.clear(sdin.rdstate() & ~ios_base::failbit);
    } else if (sdin.eof()) {
      cout << "Partial final line";  // sdin.fail() is false
    } else {
      count--;  // Don’t include newline in count
      cout << "Line " << ++line_number;
    }
    cout << " (" << count << " chars): " << buffer << endl;
  }
}
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }

  // F stores strings in flash to save RAM
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // make the test file
  makeTestFile();

  // run the example
  testGetline();
  cout << "\nDone!\n";
}
//------------------------------------------------------------------------------
void loop(void) {}
