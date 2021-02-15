// Demo of fgets function to read lines from a file.
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD chip select pin
const uint8_t chipSelect = SS;

SdFat sd;
// print stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash memory
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void demoFgets() {
  char line[25];
  int n;
  // open test file
  SdFile rdfile("fgets.txt", O_RDONLY);

  // check for open error
  if (!rdfile.isOpen()) {
    error("demoFgets");
  }

  cout << endl << F(
         "Lines with '>' end with a '\\n' character\n"
         "Lines with '#' do not end with a '\\n' character\n"
         "\n");

  // read lines from the file
  while ((n = rdfile.fgets(line, sizeof(line))) > 0) {
    if (line[n - 1] == '\n') {
      cout << '>' << line;
    } else {
      cout << '#' << line << endl;
    }
  }
}
//------------------------------------------------------------------------------
void makeTestFile() {
  // create or open test file
  SdFile wrfile("fgets.txt", O_WRONLY | O_CREAT | O_TRUNC);

  // check for open error
  if (!wrfile.isOpen()) {
    error("MakeTestFile");
  }

  // write test file
  wrfile.print(F(
                 "Line with CRLF\r\n"
                 "Line with only LF\n"
                 "Long line that will require an extra read\n"
                 "\n"  // empty line
                 "Line at EOF without NL"
               ));
  wrfile.close();
}
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }

  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }
  delay(400);  // catch Due reset problem

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  makeTestFile();

  demoFgets();

  cout << F("\nDone\n");
}
void loop(void) {}
