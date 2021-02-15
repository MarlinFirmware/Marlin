/*
 * This program tests the dateTimeCallback() function
 * and the timestamp() function.
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

SdFat sd;

SdFile file;

// Default SD chip select is SS pin
const uint8_t chipSelect = SS;

// create Serial stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
/*
 * date/time values for debug
 * normally supplied by a real-time clock or GPS
 */
// date 1-Oct-14
uint16_t year = 2014;
uint8_t month = 10;
uint8_t day = 1;

// time 20:30:40
uint8_t hour = 20;
uint8_t minute = 30;
uint8_t second = 40;
//------------------------------------------------------------------------------
/*
 * User provided date time callback function.
 * See SdFile::dateTimeCallback() for usage.
 */
void dateTime(uint16_t* date, uint16_t* time) {
  // User gets date and time from GPS or real-time
  // clock in real callback function

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year, month, day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour, minute, second);
}
//------------------------------------------------------------------------------
/*
 * Function to print all timestamps.
 */
void printTimestamps(SdFile& f) {
  dir_t d;
  if (!f.dirEntry(&d)) {
    error("f.dirEntry failed");
  }

  cout << F("Creation: ");
  f.printFatDate(d.creationDate);
  cout << ' ';
  f.printFatTime(d.creationTime);
  cout << endl;

  cout << F("Modify: ");
  f.printFatDate(d.lastWriteDate);
  cout <<' ';
  f.printFatTime(d.lastWriteTime);
  cout << endl;

  cout << F("Access: ");
  f.printFatDate(d.lastAccessDate);
  cout << endl;
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
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // remove files if they exist
  sd.remove("callback.txt");
  sd.remove("default.txt");
  sd.remove("stamp.txt");

  // create a new file with default timestamps
  if (!file.open("default.txt", O_WRONLY | O_CREAT)) {
    error("open default.txt failed");
  }
  cout << F("\nOpen with default times\n");
  printTimestamps(file);

  // close file
  file.close();
  /*
   * Test the date time callback function.
   *
   * dateTimeCallback() sets the function
   * that is called when a file is created
   * or when a file's directory entry is
   * modified by sync().
   *
   * The callback can be disabled by the call
   * SdFile::dateTimeCallbackCancel()
   */
  // set date time callback function
  SdFile::dateTimeCallback(dateTime);

  // create a new file with callback timestamps
  if (!file.open("callback.txt", O_WRONLY | O_CREAT)) {
    error("open callback.txt failed");
  }
  cout << ("\nOpen with callback times\n");
  printTimestamps(file);

  // change call back date
  day += 1;

  // must add two to see change since FAT second field is 5-bits
  second += 2;

  // modify file by writing a byte
  file.write('t');

  // force dir update
  file.sync();

  cout << F("\nTimes after write\n");
  printTimestamps(file);

  // close file
  file.close();
  /*
   * Test timestamp() function
   *
   * Cancel callback so sync will not
   * change access/modify timestamp
   */
  SdFile::dateTimeCallbackCancel();

  // create a new file with default timestamps
  if (!file.open("stamp.txt", O_WRONLY | O_CREAT)) {
    error("open stamp.txt failed");
  }
  // set creation date time
  if (!file.timestamp(T_CREATE, 2014, 11, 10, 1, 2, 3)) {
    error("set create time failed");
  }
  // set write/modification date time
  if (!file.timestamp(T_WRITE, 2014, 11, 11, 4, 5, 6)) {
    error("set write time failed");
  }
  // set access date
  if (!file.timestamp(T_ACCESS, 2014, 11, 12, 7, 8, 9)) {
    error("set access time failed");
  }
  cout << F("\nTimes after timestamp() calls\n");
  printTimestamps(file);

  file.close();
  cout << F("\nDone\n");
}

void loop() {}
