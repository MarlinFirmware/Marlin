/*

  dogmma.c
  
  math for arc... sinus/cosinus functions

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

const dog_pgm_uint8_t dog_sin_tab[65] = {
  0,2,3,5,6,8,9,11,12,14,16,17,19,20,22,23,24,26,27,29,
  30,32,33,34,36,37,38,39,41,42,43,44,45,46,47,48,49,50,51,52,
  53,54,55,56,56,57,58,59,59,60,60,61,61,62,62,62,63,63,63,64,
  64,64,64,64,64};
  
  /* w: 0..255     64 = PI/2, 128 = PI, 192 = 3*PI/2 */
  /* returns: -64 .. 64 */
signed char dog_sin(uint8_t w)
{
  uint8_t case_w = (w>>6)&3;
  uint8_t idx_w = w & 63;
  signed char result = 0;
  switch(case_w)
  {
    case 0:
      result = dog_pgm_read(dog_sin_tab+idx_w);
      //result = dog_sin_tab[idx_w];
      break;
    case 1:
      result = dog_pgm_read(dog_sin_tab+64-idx_w);
      //result = dog_sin_tab[64-idx_w];
      break;
    case 2:
      result = -(signed char)dog_pgm_read(dog_sin_tab+idx_w);
      //result = -dog_sin_tab[idx_w];
      break;
    case 3:
      result = -(signed char)dog_pgm_read(dog_sin_tab+64-idx_w);
      //result = -dog_sin_tab[64-idx_w];
      break;
  }
  return result;
}

signed char dog_cos(uint8_t w)
{
  return dog_sin( (w+64U) );
}



