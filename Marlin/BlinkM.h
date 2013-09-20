/*
  BlinkM.h
  Library header file for BlinkM library
 */
#if (ARDUINO >= 100)
  # include "Arduino.h"
#else
  # include "WProgram.h"
#endif

#ifndef BLINKM
#define BLINKM

#include "Wire.h"

void SendColors(byte red, byte grn, byte blu);

#endif
