/*
  blinkm.cpp - Library for controlling a BlinkM over i2c
  Created by Tim Koster, August 21 2013.
*/
#include "Marlin.h"

#if ENABLED(BLINKM)

#include "blinkm.h"

void SendColors(byte red, byte grn, byte blu) {
  Wire.begin();
  Wire.beginTransmission(0x09);
  Wire.write('o');                    //to disable ongoing script, only needs to be used once
  Wire.write('n');
  Wire.write(red);
  Wire.write(grn);
  Wire.write(blu);
  Wire.endTransmission();
}

#endif //BLINKM

