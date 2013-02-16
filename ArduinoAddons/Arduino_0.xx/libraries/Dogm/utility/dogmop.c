/*

  dogmop.c
  
  (o)ther (p)ixel
  
  contains gray level information for DOGXL160

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
uint8_t dog_pixel_value;
#endif

void dog_SetPixelValue(uint8_t value)
{
#if defined(DOGXL160_HW_GR)
  dog_pixel_value = value;
#endif
}




