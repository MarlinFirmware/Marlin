/*

  dogmsp.c

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 Arduino library.

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

void dog_set_pixel(uint8_t x, uint8_t y)
{
  uint8_t tmp, mask, bits;
  tmp = y;
  tmp &= (unsigned char)3;
#ifdef DOG_REVERSE
  tmp = 3-tmp;
#endif
  bits = dog_pixel_value;
  bits &= 3;
  mask = 3;
  while( tmp > 0 )
  {
    bits <<= 1;
    bits <<= 1;
    mask <<= 1;
    mask <<= 1;
    tmp--;
  }
  mask ^= 0x0ff;
#if defined(DOG_DOUBLE_MEMORY)
  if ( (y & 4) == 0 ) 
  {
    dog_page_buffer[x] &= mask;
    dog_page_buffer[x] |= bits;
  }
  else
  {
    dog_page_buffer[x+DOG_WIDTH] &= mask;
    dog_page_buffer[x+DOG_WIDTH] |= bits;
  }
#else
    dog_page_buffer[x] &= mask;
    dog_page_buffer[x] |= bits;
#endif
}

#else

void dog_set_pixel(uint8_t x, uint8_t y)
{
  uint8_t tmp;
  tmp = y;
  tmp &= (unsigned char)7;
  tmp = dog_bit_to_mask[tmp];
#if defined(DOG_DOUBLE_MEMORY)
  if ( (y & 8) == 0 ) 
  {
    dog_page_buffer[x] |= tmp;
  }
  else
  {
    dog_page_buffer[x+DOG_WIDTH] |= tmp;
  }
#else
  dog_page_buffer[x] |= tmp;
#endif
}

#endif

void dog_SetPixel(uint8_t x, uint8_t y)
{
  if ( x < DOG_WIDTH )
    if ( y >= dog_min_y && y <=dog_max_y )
    {
      dog_set_pixel(x, y);
    }
}


