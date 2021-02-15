/*
 * Example use of chdir(), ls(), mkdir(), and  rmdir().
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
// SD card chip select pin.
const uint8_t chipSelect = SS;
//------------------------------------------------------------------------------

// File system object.
SdFat sd;

// Directory file.
SdFile root;

// Use for file creation in folders.
SdFile file;

// Create a Serial output stream.
ArduinoOutStream cout(Serial);

// Buffer for Serial input.
char cinBuf[40];

// Create a serial input stream.
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));
//==============================================================================
// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Wait for USB Serial
  while (!Serial) {
    SysCall::yield();
  }
  delay(1000);

  cout << F("Type any character to start\n");
  // Wait for input line and discard.
  cin.readline();
  cout << endl;

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
  if (sd.exists("Folder1")
    || sd.exists("Folder1/file1.txt")
    || sd.exists("Folder1/File2.txt")) {
    error("Please remove existing Folder1, file1.txt, and File2.txt");
  }

  int rootFileCount = 0;
  if (!root.open("/")) {
    error("open root failed");
  }
  while (file.openNext(&root, O_RDONLY)) {
    if (!file.isHidden()) {
      rootFileCount++;
    }
    file.close();
    if (rootFileCount > 10) {
      error("Too many files in root. Please use an empty SD.");
    }
  }
  if (rootFileCount) {
    cout << F("\nPlease use an empty SD for best results.\n\n");
    delay(1000);
  }
  // Create a new folder.
  if (!sd.mkdir("Folder1")) {
    error("Create Folder1 failed");
  }
  cout << F("Created Folder1\n");

  // Create a file in Folder1 using a path.
  if (!file.open("Folder1/file1.txt", O_WRONLY | O_CREAT)) {
    error("create Folder1/file1.txt failed");
  }
  file.close();
  cout << F("Created Folder1/file1.txt\n");

  // Change volume working directory to Folder1.
  if (!sd.chdir("Folder1")) {
    error("chdir failed for Folder1.\n");
  }
  cout << F("chdir to Folder1\n");

  // Create File2.txt in current directory.
  if (!file.open("File2.txt", O_WRONLY | O_CREAT)) {
    error("create File2.txt failed");
  }
  file.close();
  cout << F("Created File2.txt in current directory\n");

  cout << F("\nList of files on the SD.\n");
  sd.ls("/", LS_R);

  // Remove files from current directory.
  if (!sd.remove("file1.txt") || !sd.remove("File2.txt")) {
    error("remove failed");
  }
  cout << F("\nfile1.txt and File2.txt removed.\n");

  // Change current directory to root.
  if (!sd.chdir()) {
    error("chdir to root failed.\n");
  }

  cout << F("\nList of files on the SD.\n");
  sd.ls(LS_R);

  // Remove Folder1.
  if (!sd.rmdir("Folder1")) {
    error("rmdir for Folder1 failed\n");
  }

  cout << F("\nFolder1 removed.\n");
  cout << F("\nList of files on the SD.\n");
  sd.ls(LS_R);
  cout << F("Done!\n");
}
//------------------------------------------------------------------------------
// Nothing happens in loop.
void loop() {}