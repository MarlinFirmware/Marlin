/*

  Walk.pde
  
  Walk Cycle Study for the dogm128 library.
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 Arduino library.

  The dogm128 Arduino library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 Arduino library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 
    
*/

#include <Dogm.h>

int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module
Dogm dogm(a0Pin);

void setup() {
}

void loop() {  
  
  dogm.showLibInfo();

  dogm.start();
  do {
    walk_Draw();
  } while( dogm.next() );
  
  // do some delay so that the LCD can display the character
  dog_Delay(120);
  
  // character movement
  walk_Step();
}
