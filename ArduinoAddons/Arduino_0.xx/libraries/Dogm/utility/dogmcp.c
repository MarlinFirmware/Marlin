/*

  dogmcp.c

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


*/

#include "dogm128.h"

#if defined(DOGXL160_HW_GR)

void dog_clr_pixel(uint8_t x, uint8_t y)
{
  uint8_t tmp, mask;
  tmp = y;
  tmp &= (unsigned char)3;
#ifdef DOG_REVERSE
  tmp = 3-tmp;
#endif
  mask = 3;
  while( tmp > 0 )
  {
    mask <<= 1;
    mask <<= 1;
    tmp--;
  }
  mask ^= 0x0ff;
#if defined(DOG_DOUBLE_MEMORY)
  if ( (y & 4) == 0 ) 
  {
    dog_page_buffer[x] &= mask;
  }
  else
  {
    dog_page_buffer[x+DOG_WIDTH] &= mask;
  }
#else
    dog_page_buffer[x] &= mask;
#endif
}

#else

void dog_clr_pixel(uint8_t x, uint8_t y)
{
  uint8_t tmp;
  tmp = y;
  tmp &= (unsigned char)7;
  tmp = dog_bit_to_mask[tmp];
  tmp ^= 0x0ff;
#if defined(DOG_DOUBLE_MEMORY)
  if ( (y & 8) == 0 ) 
  {
    dog_page_buffer[x] &= tmp;
  }
  else
  {
    dog_page_buffer[x+DOG_WIDTH] &= tmp;
  }
#else
  dog_page_buffer[x] &= tmp;
#endif
}

#endif

void dog_ClrPixel(uint8_t x, uint8_t y)
{
  if ( x < DOG_WIDTH )
    if ( y >= dog_min_y && y <=dog_max_y )
    {
      dog_clr_pixel(x, y);
    }
}


