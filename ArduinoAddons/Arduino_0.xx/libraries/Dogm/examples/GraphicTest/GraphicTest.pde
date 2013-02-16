/*

  GraphicTest
  
  Test for the dogm128 library

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
  
  Analog 0: Read value
  PWM Pin 3: Write analog (Backlight control)
  
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 
  
*/

#include <Dogm.h>

int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module
uint8_t sensorPin = 0;  // analog input
uint8_t backlightPin = 3;

Dogm dogm(a0Pin);

void setup() {
#ifndef DOGXL160_HW_GR
  analogWrite(backlightPin, 0);
#endif
}


void all_page()
{
    dogm.setFont(font_5x8);
    dogm.setRot(0);
  
    // draw a cross at the DOGS x-boundary
    dogm.setVLine(101,0, 3);
    dogm.setVLine(102,1, 4);
    dogm.setHLine(100,103, 2);

    // draw a cross at the DOGM132 y-boundary  
    dogm.setHLine(98,103, 31);
    dogm.setHLine(99,104, 32);
    dogm.setVLine(101,30, 34);
}


void page0()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,10);
    dogm.drawStr("Set Test");
    dogm.setBox(5,20,DOG_WIDTH-5,43);
    dogm.setVLine(80,0, 63);
    dogm.setHLine(0,DOG_WIDTH-1, 2);
    dogm.setPixel(10, 17);
    dogm.setPixel(12, 17);
    dogm.setPixel(14, 17);
}

void page1()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,10);
    dogm.drawStr("Clr Test");
    dogm.setBox(5,20,DOG_WIDTH-5,43);
    dogm.clrBox(20,30,30,40);
    dogm.clrVLine(80,0, 63);
    dogm.clrPixel(10, 27);
    dogm.clrPixel(12, 27);
    dogm.clrPixel(14, 27);
    dogm.clrPixel(10, 17);
    dogm.clrPixel(12, 17);
    dogm.clrPixel(14, 17);
    dogm.clrHLine(16, 23, 27);
}

void page2()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,16);
    dogm.drawStr("Xor Test");
    dogm.xorBox(5,20,90,43);
    dogm.xorVLine(80,0, 63);
    dogm.xorPixel(10, 27);
    dogm.xorPixel(12, 27);
    dogm.xorPixel(14, 27);
    dogm.xorPixel(10, 17);
    dogm.xorPixel(12, 17);
    dogm.xorPixel(14, 17);
    dogm.xorHLine(16, 127, 27);
}

void page3()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(2,24);
    dogm.drawStr("Draw Test");
    dogm.setSize(1);
    dogm.drawLine(0,0,127,20);
    dogm.drawLine(0,1,127,30);
    dogm.setSize(0);
    dogm.drawLine(0,2,127,40);
    dogm.drawLine(0,3,127,50);
    dogm.setSize(0);
    dogm.drawPoint(10,20);
    dogm.drawPoint(16,20);
    dogm.drawArc(53, 40, 10, 192, 64); 
    dogm.setSize(1);
    dogm.drawPoint(13,20);
    dogm.drawArc(53, 40, 14, 192, 64); 
}

void page4()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(64,32);
    dogm.setRot(0);
    dogm.drawStr("-ABCa");
    dogm.drawStr("bc");
    dogm.drawChar('d');
    dogm.setXY(64,32);
    dogm.setRot(1);
    dogm.drawStr("-ABCa");
    dogm.drawStr("bc");
    dogm.drawChar('d');
    dogm.setXY(64,32);
    dogm.setRot(2);
    dogm.drawStr("-ABCa");
    dogm.drawStr("bc");
    dogm.drawChar('d');
    dogm.setXY(64,32);
    dogm.setRot(3);
    dogm.drawStr("-ABCa");
    dogm.drawStr("bc");
    dogm.drawChar('d');
}

void page5()
{
#ifdef DOGXL160_HW_GR
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,10);
    dogm.drawStr("PixVal Test");
    dogm.setPixelValue(3);
    dogm.setBox(5,20,DOG_WIDTH-7,43);
  
    dogm.setHLine(90,DOG_WIDTH-1, 5);
    dogm.setHLine(90,DOG_WIDTH-1, 5 + 5);
    dogm.setHLine(90,DOG_WIDTH-1, 5 + 10);
  
    dogm.setPixelValue(2);
    dogm.setBox(5+2,20+1,DOG_WIDTH-7-2,43-1);
  
    dogm.setHLine(90,DOG_WIDTH-1, 6);
    dogm.setHLine(90,DOG_WIDTH-1, 6 + 5);
    dogm.setHLine(90,DOG_WIDTH-1, 6 + 10);

    dogm.setPixelValue(1);
    dogm.setBox(5+4,20+2,DOG_WIDTH-7-4,43-2);

    dogm.setHLine(90,DOG_WIDTH-1, 7);
    dogm.setHLine(90,DOG_WIDTH-1, 7 + 5);
    dogm.setHLine(90,DOG_WIDTH-1, 7 + 10);

    dogm.setPixelValue(0);
    dogm.setBox(5+6,20+3,DOG_WIDTH-7-6,43-3);
  
    dogm.setHLine(90,DOG_WIDTH-1, 8);
    dogm.setHLine(90,DOG_WIDTH-1, 8 + 5);
    dogm.setHLine(90,DOG_WIDTH-1, 8 + 10);

    dogm.setPixelValue(3);
    dogm.setVLine(80,0, 63);
    dogm.setHLine(0,DOG_WIDTH-1, 2);
    dogm.setPixel(10, 17);
    dogm.setPixel(12, 17);
    dogm.setPixel(14, 17);
#endif
}


uint8_t page = 0;
uint8_t is_invert = 0;

uint16_t sensorValue;

#ifdef DOGXL160_HW_GR
uint8_t backlight = 0;
#endif


void loop() {
  uint8_t i;
  
  dogm.showLibInfo();

  dogm.start();
  do {
    switch(page) {
      case 0: page0(); break;
      case 1: page1(); break;
      case 2: page2(); break;
      case 3: page3(); break;
      case 4: page4(); break;
      case 5: page5(); break;
    }
  } while( dogm.next() );
  for( i = 0; i < 16; i++ )
  {
    dog_Delay(100);
#ifdef DOGXL160_HW_GR
    analogWrite(6, backlight);		/* assume DOGXL160 shield */
    backlight++;
#else
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 2;
    analogWrite(backlightPin, sensorValue);
#endif
  }
  page++;
#ifdef DOGXL160_HW_GR
  if ( page == 6 )
#else
  if ( page == 5 )
#endif
  {
    page = 0;
    is_invert++;
    is_invert &= 1;
    dogm.setInvertPixelMode(is_invert);
  }
}
