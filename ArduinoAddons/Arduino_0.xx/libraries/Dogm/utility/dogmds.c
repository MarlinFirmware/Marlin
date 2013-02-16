
/*

  dogmds.c
  
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

uint8_t dog_GetStrWidth(DOG_PGM_P font, const char *s)
{
  uint8_t w = 0;
  while( *s != '\0' )
  {
    w	+= dog_GetCharWidth(font, (unsigned char)*s);
    s++;
  }
  return w;
}

static uint8_t dog_str(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, uint8_t mode, const char *s)
{
  uint8_t d = 0;
  uint8_t d_sum = 0;
  signed char y0, y1;
  
  if ( rot == 0 )
  {
    y0 = y;
    y0 -= fnt_get_bbox_descent(font);  
    y1 = y0;
    y1 += fnt_get_bbox_height(font);
  
    if ( (signed char)dog_max_y < y0 )
      return dog_GetStrWidth(font, s);
    if ( (signed char)dog_min_y > y1 )
      return dog_GetStrWidth(font, s);
  }
  
  while( *s != '\0' )
  {
    d = dog_char(x,y,font, mode, rot, (unsigned char)*s);
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

uint8_t dog_DrawStr(uint8_t x, uint8_t y, DOG_PGM_P font, const char *s)
{
  return dog_str(x,y,0, font,0,s);
}

uint8_t dog_DrawRStr(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, const char *s)
{
  return dog_str(x,y,rot, font,0,s);
}
