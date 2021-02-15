// This example illustrates use of SdFat's
// minimal unbuffered AVR Serial support.
//
// This is useful for debug and saves RAM
// Will not work on Due, Leonardo, or Teensy

#include <SPI.h>
#include "SdFat.h"
#include "FreeStack.h"
#ifdef UDR0  // Must be AVR with serial port zero.
#include "MinimumSerial.h"

MinimumSerial MiniSerial;

void setup() {
  MiniSerial.begin(9600);
  MiniSerial.println(FreeStack());
}
void loop() {
  int c;
  MiniSerial.println(F("Type any Character"));
  while ((c = MiniSerial.read()) < 0) {}
  MiniSerial.print(F("Read: "));
  MiniSerial.println((char)c);
  while (MiniSerial.read() >= 0) {}
}
#else  // UDR0
#error no AVR serial port 0
#endif  // UDR0