/*

  dogmdl.c
  
  draw a line

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


*/

#include "dogm128.h"

void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size)
{
  uint8_t tmp;
  uint8_t x,y;
  uint8_t dx, dy;
  signed char err;
  uint8_t swapxy = 0;
  signed char ystep;
  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;
  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;
  for( x = x1; x <= x2; x++ )
 {
   if ( swapxy == 0 ) 
     dog_DrawPoint(x,y,size); 
   else 
     dog_DrawPoint(y,x,size);
   err -= (uint8_t)dy;
   if ( err < 0 ) 
   {
     y += (uint8_t)ystep;
     err += (uint8_t)dx;
   }
 }  
}



