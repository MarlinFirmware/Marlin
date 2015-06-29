/*

  u8g_rect.c
  
  U8G high level interface for horizontal and vertical things

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include "u8g.h"

void u8g_draw_hline(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w)
{
  uint8_t pixel = 0x0ff;
  while( w >= 8 )
  {
    u8g_Draw8Pixel(u8g, x, y, 0, pixel);
    w-=8;
    x+=8;
  }
  if ( w != 0 )
  {
    w ^=7;
    w++;
    pixel <<= w&7;
    u8g_Draw8Pixel(u8g, x, y, 0, pixel);
  }
}

void u8g_draw_vline(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t h)
{
  uint8_t pixel = 0x0ff;
  while( h >= 8 )
  {
    u8g_Draw8Pixel(u8g, x, y, 1, pixel);
    h-=8;
    y+=8;
  }
  if ( h != 0 )
  {
    h ^=7;
    h++;
    pixel <<= h&7;
    u8g_Draw8Pixel(u8g, x, y, 1, pixel);
  }
}

void u8g_DrawHLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w)
{
  if ( u8g_IsBBXIntersection(u8g, x, y, w, 1) == 0 )
    return;
  u8g_draw_hline(u8g, x, y, w);
}

void u8g_DrawVLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w)
{
  if ( u8g_IsBBXIntersection(u8g, x, y, 1, w) == 0 )
    return;
  u8g_draw_vline(u8g, x, y, w);
}

/* restrictions: w > 0 && h > 0 */
void u8g_DrawFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
  u8g_uint_t xtmp = x;
  
  if ( u8g_IsBBXIntersection(u8g, x, y, w, h) == 0 )
    return;

  
  u8g_draw_hline(u8g, x, y, w);
  u8g_draw_vline(u8g, x, y, h);
  x+=w;
  x--;
  u8g_draw_vline(u8g, x, y, h);
  y+=h;
  y--;
  u8g_draw_hline(u8g, xtmp, y, w);
}

void u8g_draw_box(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
  do
  { 
    u8g_draw_hline(u8g, x, y, w);
    y++;    
    h--;
  } while( h != 0 );
}

/* restrictions: h > 0 */
void u8g_DrawBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
  if ( u8g_IsBBXIntersection(u8g, x, y, w, h) == 0 )
    return;
  u8g_draw_box(u8g, x, y, w, h);
}


void u8g_DrawRFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r)
{
  u8g_uint_t xl, yu;

  if ( u8g_IsBBXIntersection(u8g, x, y, w, h) == 0 )
    return;

  xl = x;
  xl += r;
  yu = y;
  yu += r;
 
  {
    u8g_uint_t yl, xr;
      
    xr = x;
    xr += w;
    xr -= r;
    xr -= 1;
    
    yl = y;
    yl += h;
    yl -= r; 
    yl -= 1;

    u8g_draw_circle(u8g, xl, yu, r, U8G_DRAW_UPPER_LEFT);
    u8g_draw_circle(u8g, xr, yu, r, U8G_DRAW_UPPER_RIGHT);
    u8g_draw_circle(u8g, xl, yl, r, U8G_DRAW_LOWER_LEFT);
    u8g_draw_circle(u8g, xr, yl, r, U8G_DRAW_LOWER_RIGHT);
  }

  {
    u8g_uint_t ww, hh;

    ww = w;
    ww -= r;
    ww -= r;
    ww -= 2;
    hh = h;
    hh -= r;
    hh -= r;
    hh -= 2;
    
    xl++;
    yu++;
    h--;
    w--;
    u8g_draw_hline(u8g, xl, y, ww);
    u8g_draw_hline(u8g, xl, y+h, ww);
    u8g_draw_vline(u8g, x,         yu, hh);
    u8g_draw_vline(u8g, x+w, yu, hh);
  }
}

void u8g_DrawRBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r)
{
  u8g_uint_t xl, yu;
    u8g_uint_t yl, xr;

  if ( u8g_IsBBXIntersection(u8g, x, y, w, h) == 0 )
    return;

  xl = x;
  xl += r;
  yu = y;
  yu += r;
 
  xr = x;
  xr += w;
  xr -= r;
  xr -= 1;
  
  yl = y;
  yl += h;
  yl -= r; 
  yl -= 1;

  u8g_draw_disc(u8g, xl, yu, r, U8G_DRAW_UPPER_LEFT);
  u8g_draw_disc(u8g, xr, yu, r, U8G_DRAW_UPPER_RIGHT);
  u8g_draw_disc(u8g, xl, yl, r, U8G_DRAW_LOWER_LEFT);
  u8g_draw_disc(u8g, xr, yl, r, U8G_DRAW_LOWER_RIGHT);

  {
    u8g_uint_t ww, hh;

    ww = w;
    ww -= r;
    ww -= r;
    ww -= 2;
    hh = h;
    hh -= r;
    hh -= r;
    hh -= 2;
    
    xl++;
    yu++;
    h--;
    u8g_draw_box(u8g, xl, y, ww, r+1);
    u8g_draw_box(u8g, xl, yl, ww, r+1);
    //u8g_draw_hline(u8g, xl, y+h, ww);
    u8g_draw_box(u8g, x, yu, w, hh);
    //u8g_draw_vline(u8g, x+w, yu, hh);
  }
}
