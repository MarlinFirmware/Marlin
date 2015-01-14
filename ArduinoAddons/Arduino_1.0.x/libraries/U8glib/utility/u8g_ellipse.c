/*

  u8g_ellipse.c

  Utility to draw empty and filled ellipses.
  
  Universal 8bit Graphics Library
  
  Copyright (c) 2011, bjthom@gmail.com
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
  
  Addition to the U8G Library as of 02/29/12
  Adapted from Bresenham's Algorithm and the following websites:
	http://free.pages.at/easyfilter/bresenham.html
	http://homepage.smc.edu/kennedy_john/belipse.pdf
  
*/

#include "u8g.h"


#ifdef WORK_IN_PROGRESS

void u8g_DrawEllipseRect(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t x1, u8g_uint_t y1)
{
	int a = abs(x1 - x0);
	int b = abs(y1 - y0);	//get diameters
	int b1 = b&1;
	long dx = 4*(1-a)*b*b;
	long dy = 4*(b1+1)*a*a;
	long err = dx+dy+b1*a*a;
	long e2;
	
	if (x0 > x1) { x0 = x1; x1 += a; }
	if (y0 > y1) { y0 = y1; } 
	y0 += (b+1)/2;
	y1 = y0-b1;
	a *= 8*a;
	b1 = 8*b*b;
	
	do {
		u8g_DrawPixel(u8g, x1, y0);
		u8g_DrawPixel(u8g, x0, y0);
		u8g_DrawPixel(u8g, x0, y1);
		u8g_DrawPixel(u8g, x1, y1);
		e2 = 2*err;
		if (e2 >= dx) {
			x0++;
			x1--;
			err += dx += b1;
		}
		if (e2 <= dy) {
			y0++;
			y1--;
			err += dy += a;
		}
	} while (x0 <= x1);
	
	while (y0-y1 < b) {
		u8g_DrawPixel(u8g, x0-1, y0);
		u8g_DrawPixel(u8g, x1+1, y0++);
		u8g_DrawPixel(u8g, x0-1, y1);
		u8g_DrawPixel(u8g, x1+1, y1--);
	}
}

void u8g_DrawEllipse(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t xr, u8g_uint_t yr)
{
	u8g_DrawPixel(u8g, x0, y0+yr);
	u8g_DrawPixel(u8g, x0, y0-yr);
	u8g_DrawPixel(u8g, x0+xr, y0);
	u8g_DrawPixel(u8g, x0-xr, y0);
}

#endif

