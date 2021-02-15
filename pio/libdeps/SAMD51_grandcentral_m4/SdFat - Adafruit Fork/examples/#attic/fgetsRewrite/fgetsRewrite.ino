// Demo of rewriting a line read by fgets
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// SD card chip select pin
const uint8_t chipSelect = SS;

// file system
SdFat sd;

// print stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash memory
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void demoFgets() {
  char line[25];
  int c;
  uint32_t pos;

  // open test file
  SdFile rdfile("fgets.txt", O_RDWR);

  // check for open error
  if (!rdfile.isOpen()) {
    error("demoFgets");
  }

  // list file
  cout << F("-----Before Rewrite\r\n");
  while ((c = rdfile.read()) >= 0) {
    Serial.write(c);
  }

  rdfile.rewind();
  // read lines from the file to get position
  while (1) {
    pos = rdfile.curPosition();
    if (rdfile.fgets(line, sizeof(line)) < 0) {
      error("Line not found");
    }
    // find line that contains "Line C"
    if (strstr(line, "Line C")) {
      break;
    }
  }

  // rewrite line with 'C'
  if (!rdfile.seekSet(pos)) {
    error("seekSet");
  }
  rdfile.println("Line R");
  rdfile.rewind();

  // list file
  cout << F("\r\n-----After Rewrite\r\n");
  while ((c = rdfile.read()) >= 0) {
    Serial.write(c);
  }

  // close so rewrite is not lost
  rdfile.close();
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
                 "Line A\r\n"
                 "Line B\r\n"
                 "Line C\r\n"
                 "Line D\r\n"
                 "Line E\r\n"
               ));
  wrfile.close();
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  makeTestFile();

  demoFgets();

  cout << F("\nDone\n");
}
void loop() {}
