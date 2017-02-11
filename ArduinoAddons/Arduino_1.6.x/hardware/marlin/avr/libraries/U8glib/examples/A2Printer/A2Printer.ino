/*

  A2Printer.pde
  
  Special example code for the A2 Mciro Printer (https://www.sparkfun.com/products/10438)
  
  
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/


#include "U8glib.h"

// use this serial interface
#define PRINTER_SERIAL Serial
// #define PRINTER_SERIAL Serial1


uint8_t u8g_com_uart(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch(msg) {
    case U8G_COM_MSG_WRITE_BYTE:
      PRINTER_SERIAL.write(arg_val);
      break;
  }
  return 1;
}

// setup u8g object, please remove comment from one of the following constructor calls

// half resolution
//U8GLIB u8g(&u8g_dev_a2_micro_printer_192x120_ds, (u8g_com_fnptr)u8g_com_uart);

// full resolution, requires to uncomment U8G_16BIT in u8g.h
//U8GLIB u8g(&u8g_dev_a2_micro_printer_384x240, (u8g_com_fnptr)u8g_com_uart);

// half resolution, extra log, requires to uncomment U8G_16BIT in u8g.h
//U8GLIB u8g(&u8g_dev_a2_micro_printer_192x360_ds, (u8g_com_fnptr)u8g_com_uart);
U8GLIB u8g(&u8g_dev_a2_micro_printer_192x720_ds, (u8g_com_fnptr)u8g_com_uart);



void drawLogo(uint8_t d) {
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(0+d, 30+d, "U");
  u8g.setFont(u8g_font_gdr30n);
  u8g.drawStr90(23+d,10+d,"8");
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(53+d,30+d,"g");
  
  u8g.drawHLine(2+d, 35+d, 47);
  u8g.drawVLine(45+d, 32+d, 12);
}

void drawURL(void) {
  u8g.setFont(u8g_font_4x6);
  if ( u8g.getHeight() < 59 ) {
    u8g.drawStr(53,9,"code.google.com");
    u8g.drawStr(77,18,"/p/u8glib");
  }
  else {
    u8g.drawStr(1,54,"code.google.com/p/u8glib");
  }
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  
  drawLogo(0);
  drawURL();
  u8g.drawFrame(0,0,u8g.getWidth(), u8g.getHeight());
  
  u8g.setFont(u8g_font_helvR24r);
  u8g.setPrintPos(0, 100);
  u8g.print(u8g.getWidth(), DEC);
  u8g.print("x");
  u8g.print(u8g.getHeight(), DEC);
}

void setup(void) {
  PRINTER_SERIAL.begin(19200);
  
  // flip screen, if required
  // u8g.setRot180();
  
  // assign default color value
  u8g.setColorIndex(1);         // pixel on
}

void loop(void) {
  
  // picture loop: This will print the picture  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  // send manual CR to the printer
  PRINTER_SERIAL.write('\n'); 		
  
  // reprint the picture after 10 seconds
  delay(10000);
}

