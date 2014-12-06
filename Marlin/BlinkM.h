/*
  BlinkM.h
  Library header file for BlinkM library
 */
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Wire.h"

void SendColors(byte red, byte grn, byte blu);
