/*

  dogmda.c
  
  draw arc

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


/*
  mx, my		center of the arc/circle
  r			radius in pixel
  w0			start angle (counter clock direction, zero at the left)
  w1			end angle
  size		line thickness
  
  Notes:
  1) A circle will be drawn, if w0 == w1
  2) angle: 128 = Pi
*/
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size)
{
  uint8_t l,i,w,x1,y1,x2,y2;
  unsigned short dw;
  if ( w1 > w0 )
    dw = w1-w0;
  else
    dw = 256-w0+w1;
    
  if ( dw == 0 )
    dw = 256;
  
  /* laenge des bogens berechnen l = 2*r*pi*(w1-w0)/256 */
  /* pi wird mit 201/64 = 3.140625 approximiert */ 
  /* l = (2 * r * 201 * (w1-w0)) >> ( 6+8 ) = (r * 201 * (w1-w0)) >> 13 */
  /* das passt nicht in einen unsigned 16 datentyp */
  /* annahme: r < 41 (dann ist der umfang 251 und passt noch in einen u8*/
  /* l = (((r * (w1-w0)) >> 6) * 201)>>7 */
  /* allerdings zeichnen wir dann doch lieber sekanten */
  /* derzeit alle 4 pixel, also muss l abermals um 2 geshiftet werden */
  /* die beiden shifts werden auf die beiden existierenden verteilt, damit sind radien bis 80 zugelassen */
  
  
  /*l = (uint8_t)(((((unsigned short)r * dw) >> 7) * (unsigned short)201)>>8) ;*/
  l = (uint8_t)(((((unsigned short)r * dw) >> 7) * (unsigned short)201)>>7) ;
  
  
  x1 = mx+(((int16_t)r*(int16_t)dog_cos(w0)) >> 6);
  y1 = my+(((int16_t)r*(int16_t)dog_sin(w0)) >> 6);
  for ( i = 1; i <= l; i++ )
  {
    w = ((unsigned short)dw*(unsigned short)i )/(unsigned short)l + w0;
    x2 = mx+(((int16_t)r*(int16_t)dog_cos(w)) >> 6);
    y2 = my+(((int16_t)r*(int16_t)dog_sin(w)) >> 6);
    dog_DrawLine(x1,y1,x2,y2,size);
    x1 = x2;
    y1 = y2;
  }
}



