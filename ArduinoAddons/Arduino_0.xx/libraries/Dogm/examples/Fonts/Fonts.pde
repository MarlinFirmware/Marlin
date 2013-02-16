/*

  Fonts
  
  Show some fonts of the dog128 library.
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
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
int spi_pin_cs = 29;
int sensorPin = 0;  // analog input

extern const dog_pgm_uint8_t font_6x10_marlin[];

Dogm dogm(a0Pin, spi_pin_cs);

void setup() {
}

int font_select = 0;


void loop() {
  const char *f;
  DOG_PGM_P p;
  unsigned char h;
  
  switch(font_select) {
      default: font_select = 0; /* no break */
      //case 0: h = 10; p = font_4x6; f = "4x6"; break;
      case 0: h = 10; p = font_6x10_marlin; f = "marlin"; break;
      case 1: h = 10; p = font_5x7; f = "5x7"; break;
      case 2: h = 10; p = font_5x8; f = "5x8"; break;
      case 3: h = 10; p = font_6x9; f = "6x9"; break;
      case 4: h = 10; p = font_6x10; f = "6x10"; break;
      case 5: h = 10; p = font_6x12; f = "6x12"; break;
      case 6: h = 11; p = font_6x13; f = "6x13"; break;
      case 7: h = 11; p = font_7x13; f = "7x13"; break;
      case 8: h = 12; p = font_7x14; f = "7x14"; break;
      case 9: h = 12; p = font_8x13; f = "8x13"; break;
  }
  //font_select++;
  
  dogm.start();
  do {
    dogm.setFont(p);
    dogm.setXY(0,h*0+3);
    dogm.print(f);
    dogm.setBox(0, h*0+3, dogm.getStrWidth(f), h*0+3 );
    dogm.print(": _^|'~0123456789.,;");
    dogm.setXY(0,h*2+3);
    dogm.print("abcdefgjijklmnopqrstuvwxyz");
    dogm.setXY(0,h*1+3);
    dogm.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    dogm.setXY(0,h*3+3);
    dogm.print("+-*/#-!$%&(){}[]");
  } while( dogm.next() );
  dog_Delay(1000);
}
