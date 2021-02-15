/*
 * Print a table with various formatting options
 * Format dates
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

// create Serial stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// print a table to demonstrate format manipulators
void example(void) {
  const int max = 10;
  const int width = 4;

  for (int row = 1; row <= max; row++) {
    for (int col = 1; col <= max; col++) {
      cout << setw(width) << row * col << (col == max ? '\n' : ' ');
    }
  }
  cout << endl;
}
//------------------------------------------------------------------------------
// print a date as mm/dd/yyyy with zero fill in mm and dd
// shows how to set and restore the fill character
void showDate(int m, int d, int y) {
  // convert two digit year
  if (y < 100) {
    y += 2000;
  }

  // set new fill to '0' save old fill character
  char old = cout.fill('0');

  // print date
  cout << setw(2) << m << '/' << setw(2) << d << '/' << y << endl;

  // restore old fill character
  cout.fill(old);
}
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);

  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  delay(2000);

  cout << endl << "default formatting" << endl;
  example();

  cout << showpos << "showpos" << endl;
  example();

  cout << hex << left << showbase << "hex left showbase" << endl;
  example();

  cout << internal << setfill('0') << uppercase;
  cout << "uppercase hex internal showbase fill('0')" <<endl;
  example();

  // restore default format flags and fill character
  cout.flags(ios::dec | ios::right | ios::skipws);
  cout.fill(' ');

  cout << "showDate example" <<endl;
  showDate(7, 4, 11);
  showDate(12, 25, 11);
}
void loop(void) {}
