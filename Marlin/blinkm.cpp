/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * blinkm.cpp - Library for controlling a BlinkM over i2c
 * Created by Tim Koster, August 21 2013.
 */

#include "Marlin.h"

#if ENABLED(BLINKM)

#include "blinkm.h"

void SendColors(byte red, byte grn, byte blu) {
  I2c.begin();
  uint8_t ary[5];
  ary[0] = 'o';                    //to disable ongoing script, only needs to be used once
  ary[1] = 'n';
  ary[2] = red;
  ary[3] = grn;
  ary[4] = blu;
  uint8_t ary4[4];
  for (int i=1;i<4;i++){ary4[i-1] = ary[i]};
  I2c.write(0x09,ary[0],ary4);
}

#endif // BLINKM

