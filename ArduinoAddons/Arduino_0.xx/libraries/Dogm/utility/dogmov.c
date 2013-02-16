/*

  dogmov.c
  
  o = other
  v = vline

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

/* support function for the vline graphics primitives */

#ifdef DOG_REVERSE
static const dog_pgm_uint8_t dog_mask7[8] = { 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01 }; 
static const dog_pgm_uint8_t dog_mask0[8] = { 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };
#else
static const dog_pgm_uint8_t dog_mask0[8] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
static const dog_pgm_uint8_t dog_mask7[8] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 };
#endif

uint8_t dog_get_vline_mask(uint8_t y1, uint8_t y2)
{
  uint8_t tmp;
  if ( y1 < dog_min_y )
  {
    if ( y2 >= dog_max_y )
    {
      /* all bits */
      tmp  = 0xff;
    }
    else
    {
      tmp = y2;
      tmp &= (unsigned char)7;
      /* bits, starting at bit-position 0 */
      tmp = dog_pgm_read(dog_mask0+tmp);
    }
  }
  else
  {
    if ( y2 >= dog_max_y )
    {
      /* bits, ending at bit-position 7 */
      tmp = y1;
      tmp &= (unsigned char)7;
      tmp = dog_pgm_read(dog_mask7+tmp);
    }
    else
    {
      /* bits, somewhere in the middle */
      uint8_t t2;
      tmp = y1;
      tmp &= (unsigned char)7;
      tmp = dog_pgm_read(dog_mask7+tmp);
      t2 = y2;
      t2 &= (unsigned char)7;
      t2= dog_pgm_read(dog_mask0+t2);
      tmp &= t2;
    }
  }
  return tmp;
}


