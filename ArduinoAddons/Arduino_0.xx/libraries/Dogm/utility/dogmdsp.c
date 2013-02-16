
/*

  dogmdsp.c
  
  draw string

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

uint8_t dog_GetStrWidthP(DOG_PGM_P font, DOG_PSTR_P s)
{
  uint8_t w = 0;
  int8_t c;
  for(;;)
  {
    c = dog_pgm_read(s);
    if ( c == '\0' )
      break;
    w	+= dog_GetCharWidth(font, c);
    s++;
  }
  return w;
}

static uint8_t dog_str_p(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, uint8_t mode, DOG_PSTR_P s)
{
  uint8_t d = 0;
  uint8_t d_sum = 0;
  int8_t c;
  signed char y0, y1;
  
  if ( rot == 0 )
  {
    y0 = y;
    y0 -= fnt_get_bbox_descent(font);  
    y1 = y0;
    y1 += fnt_get_bbox_height(font);
  
    if ( (signed char)dog_max_y < y0 )
      return dog_GetStrWidthP(font, s);
    if ( (signed char)dog_min_y > y1 )
      return dog_GetStrWidthP(font, s);
  }
  
  for(;;)
  {
    c = dog_pgm_read(s);
    if ( c == '\0' )
      break;
    d = dog_char(x,y,font, mode, rot, c);
    switch(rot)
    {
      default:
	x += d;
	break;
      case 1:
	y += d;
	break;
      case 2:
	x -= d;
	break;
      case 3:
	y -= d;
	break;
    }
    d_sum += d;
    s++;
  }
  return d_sum;
}

uint8_t dog_DrawStrP(uint8_t x, uint8_t y, DOG_PGM_P font, DOG_PSTR_P s)
{
  return dog_str_p(x,y,0, font,0,s);
}

uint8_t dog_DrawRStrP(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, DOG_PSTR_P s)
{
  return dog_str_p(x,y,rot, font,0,s);
}
