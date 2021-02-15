// Adafruit M0 Express CircuitPython Flash Example
// Author: Tony DiCola
//
// This is an example of reading and writing data from Arduino
// to the M0 Express flash filesystem used by CircuitPython.
// You can create, update, and read files on the CircuitPython
// filesystem in an Arduino sketch and then later load CircuitPython
// to interact with the same files.  This example will print out
// the contents of boot.py and main.py (if found) and add a line
// to a data.txt file on CircuitPython's filesystem.
//
// Note before you use this sketch you must load CircuitPython
// on your M0 Express.  This will create the filesystem and
// initialize it, then you can load this example and read/write
// files on the board.
//
// Usage:
// - Modify the pins and type of fatfs object in the config
//   section below if necessary (usually not necessary).
// - Upload this sketch to your M0 express board.
// - Open the serial monitor at 115200 baud.  You should see the
//   example start to run and messages printed to the monitor.
//   If you don't see anything close the serial monitor, press
//   the board reset buttton, wait a few seconds, then open the
//   serial monitor again.
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>

// On-board external flash (QSPI or SPI) macros should already
// defined in your board variant if supported
// - EXTERNAL_FLASH_USE_QSPI
// - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
#if defined(EXTERNAL_FLASH_USE_QSPI)
  Adafruit_FlashTransport_QSPI flashTransport;

#elif defined(EXTERNAL_FLASH_USE_SPI)
  Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

#else
  #error No QSPI/SPI flash are defined on your board variant.h !
#endif

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;


void setup() {
  // Initialize serial port and wait for it to open before continuing.
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("Adafruit M0 Express CircuitPython Flash Example");

  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    Serial.println("Error, failed to initialize flash chip!");
    while(1);
  }
  Serial.print("Flash chip JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);

  // First call begin to mount the filesystem.  Check that it returns true
  // to make sure the filesystem was mounted.
  if (!fatfs.begin(&flash)) {
    Serial.println("Failed to mount filesystem!");
    Serial.println("Was CircuitPython loaded on the board first to create the filesystem?");
    while(1);
  }
  Serial.println("Mounted filesystem!");

  // Check if a boot.py exists and print it out.
  if (fatfs.exists("boot.py")) {
    File bootPy = fatfs.open("boot.py", FILE_READ);
    Serial.println("Printing boot.py...");
    while (bootPy.available()) {
      char c = bootPy.read();
      Serial.print(c);
    }
    Serial.println();
  }
  else {
    Serial.println("No boot.py found...");
  }

  // Check if a main.py exists and print it out:
  if (fatfs.exists("main.py")) {
    File mainPy = fatfs.open("main.py", FILE_READ);
    Serial.println("Printing main.py...");
    while (mainPy.available()) {
      char c = mainPy.read();
      Serial.print(c);
    }
    Serial.println();
  }
  else {
    Serial.println("No main.py found...");
  }

  // Create or append to a data.txt file and add a new line
  // to the end of it.  CircuitPython code can later open and
  // see this file too!
  File data = fatfs.open("data.txt", FILE_WRITE);
  if (data) {
    // Write a new line to the file:
    data.println("Hello CircuitPython from Arduino!");
    data.close();
    // See the other fatfs examples like fatfs_full_usage and fatfs_datalogging
    // for more examples of interacting with files.
    Serial.println("Wrote a new line to the end of data.txt!");
  }
  else {
    Serial.println("Error, failed to open data file for writing!");
  }

  Serial.println("Finished!");
}

void loop() {
  // Nothing to do in the loop.
  delay(100);
}
