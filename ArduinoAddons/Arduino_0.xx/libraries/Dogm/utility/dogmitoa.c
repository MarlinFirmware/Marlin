/*

  dogmitoa.c

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

char dog_itoa_buf[12];

char *dog_itoa(unsigned long v)
{
  volatile unsigned char i = 11;
  dog_itoa_buf[11] = '\0';
  while( i > 0)
  {
      i--;
      dog_itoa_buf[i] = (v % 10)+'0';
      v /= 10;
      if ( v == 0 )
	break;
  }
  return dog_itoa_buf+i;
}




