/*

  dogmsv.c

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


#if defined(DOGXL160_HW_GR) || defined(DOG_DOUBLE_MEMORY)
/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2)
{
  if ( y2 < dog_min_y )
    return;
  if ( y1 > dog_max_y )
    return;
  if ( x >= DOG_WIDTH )
    return;
  if ( y2 > dog_max_y )
    y2 = dog_max_y;
  if ( y1 < dog_min_y )
    y1 = dog_min_y;
  while( y1 <= y2 )
  {
    dog_set_pixel(x, y1);
    y1++;
  }
}
#else
/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2)
{
  unsigned char *ptr;
  if ( y2 < dog_min_y )
    return;
  if ( y1 > dog_max_y )
    return;
  if ( x >= DOG_WIDTH )
    return;
  ptr = dog_page_buffer+x; 
  *ptr |= dog_get_vline_mask(y1, y2);  /* dogmov.c */
}
#endif

