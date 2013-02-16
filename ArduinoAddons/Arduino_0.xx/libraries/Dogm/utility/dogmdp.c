/*

  dogmdp.c
  
  draw point

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

void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size)
{
  if ( size == 0 ) 
  {
    dog_SetPixel(x,y);
  }
  else if ( size == 1 )
  {
    dog_SetPixel(x,y);
    dog_SetPixel(x+1,y);
    dog_SetPixel(x,y+1);
    if ( x > 0 ) dog_SetPixel(x-1,y);
    if ( y > 0 ) dog_SetPixel(x,y-1);
  }
  else
    dog_SetPixel(x,y);
}



