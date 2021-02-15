/*

  u8g_circle.c

  Utility to draw empty and filled circles.

  Universal 8bit Graphics Library

  Copyright (c) 2011, bjthom@gmail.com
  u8g_DrawCircle & u8g_DrawDisc by olikraus@gmail.com

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

  Addition to the U8G Library 02/25/12


*/

#include "u8g.h"

#ifdef OLD_CODE

void circ_upperRight(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0) {
  u8g_DrawPixel(u8g, x0 + x, y0 - y);
  u8g_DrawPixel(u8g, x0 + y, y0 - x);
}

void circ_upperLeft(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0) {
  u8g_DrawPixel(u8g, x0 - x, y0 - y);
  u8g_DrawPixel(u8g, x0 - y, y0 - x);
}

void circ_lowerRight(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0) {
  u8g_DrawPixel(u8g, x0 + x, y0 + y);
  u8g_DrawPixel(u8g, x0 + y, y0 + x);
}

void circ_lowerLeft(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0) {
  u8g_DrawPixel(u8g, x0 - x, y0 + y);
  u8g_DrawPixel(u8g, x0 - y, y0 + x);
}

void circ_all(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0) {
  circ_upperRight(u8g, x, y, x0, y0);
  circ_upperLeft(u8g, x, y, x0, y0);
  circ_lowerRight(u8g, x, y, x0, y0);
  circ_lowerLeft(u8g, x, y, x0, y0);
}

void u8g_DrawEmpCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
  if ( u8g_IsBBXIntersection(u8g, x0-rad-1, y0-rad-1, 2*rad+1, 2*rad+1) == 0)
    return;

  int f = 1 - rad;
  int ddF_x = 1;
  int ddF_y = -2*rad;
  uint8_t x = 0;
  uint8_t y = rad;

  void ( *circ_util )(u8g_t *, u8g_uint_t, u8g_uint_t, u8g_uint_t, u8g_uint_t);

  switch (option)
  {
    case U8G_CIRC_UPPER_RIGHT:
      u8g_DrawPixel(u8g, x0, y0 - rad);
      u8g_DrawPixel(u8g, x0 + rad, y0);
      circ_util = circ_upperRight;
      break;
    case U8G_CIRC_UPPER_LEFT:
      u8g_DrawPixel(u8g, x0, y0 - rad);
      u8g_DrawPixel(u8g, x0 - rad, y0);
      circ_util = circ_upperLeft;
      break;
    case U8G_CIRC_LOWER_RIGHT:
      u8g_DrawPixel(u8g, x0, y0 + rad);
      u8g_DrawPixel(u8g, x0 + rad, y0);
      circ_util = circ_lowerRight;
      break;
    case U8G_CIRC_LOWER_LEFT:
      u8g_DrawPixel(u8g, x0, y0 + rad);
      u8g_DrawPixel(u8g, x0 - rad, y0);
      circ_util = circ_lowerLeft;
      break;
          default:
    case U8G_CIRC_ALL:
      u8g_DrawPixel(u8g, x0, y0 + rad);
      u8g_DrawPixel(u8g, x0, y0 - rad);
      u8g_DrawPixel(u8g, x0 + rad, y0);
      u8g_DrawPixel(u8g, x0 - rad, y0);
      circ_util = circ_all;
      break;
  }

  while( x < y )
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    circ_util(u8g, x, y, x0, y0);
  }
}


void u8g_DrawFillCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
  if ( u8g_IsBBXIntersection(u8g, x0-rad-1, y0-rad-1, 2*rad+1, 2*rad+1) == 0)
    return;

  int f = 1 - rad;
  int ddF_x = 1;
  int ddF_y = -2*rad;
  uint8_t x = 0;
  uint8_t y = rad;

  // Draw vertical diameter at the horiz. center
  // u8g_DrawVLine(u8g, x0, y0 - rad, 2*rad+1);

  if (option == U8G_CIRC_UPPER_LEFT || option == U8G_CIRC_UPPER_RIGHT) {
  u8g_DrawVLine(u8g, x0, y0 - rad, rad+1);
  }
  else if (option == U8G_CIRC_LOWER_LEFT || option == U8G_CIRC_LOWER_RIGHT) {
  u8g_DrawVLine(u8g, x0, y0, rad+1);
  }
  else {
  u8g_DrawVLine(u8g, x0, y0 - rad, 2*rad+1);
  }

  while( x < y )
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

  //Draw vertical lines from one point to another

  switch (option)
  {
    case U8G_CIRC_UPPER_RIGHT:
      u8g_DrawVLine(u8g, x0+x, y0-y, y+1);
      u8g_DrawVLine(u8g, x0+y, y0-x, x+1);
      break;
    case U8G_CIRC_UPPER_LEFT:
      u8g_DrawVLine(u8g, x0-x, y0-y, y+1);
      u8g_DrawVLine(u8g, x0-y, y0-x, x+1);
      break;
    case U8G_CIRC_LOWER_RIGHT:
      u8g_DrawVLine(u8g, x0+x, y0, y+1);
      u8g_DrawVLine(u8g, x0+y, y0, x+1);
      break;
    case U8G_CIRC_LOWER_LEFT:
      u8g_DrawVLine(u8g, x0-x, y0, y+1);
      u8g_DrawVLine(u8g, x0-y, y0, x+1);
      break;
    case U8G_CIRC_ALL:
      u8g_DrawVLine(u8g, x0+x, y0-y, 2*y+1);
      u8g_DrawVLine(u8g, x0-x, y0-y, 2*y+1);
      u8g_DrawVLine(u8g, x0+y, y0-x, 2*x+1);
      u8g_DrawVLine(u8g, x0-y, y0-x, 2*x+1);
      break;
  }
  }
}

#endif

/*=========================================================================*/

static void u8g_draw_circle_section(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0, uint8_t option) U8G_NOINLINE;

static void u8g_draw_circle_section(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0, uint8_t option)
{
    /* upper right */
    if ( option & U8G_DRAW_UPPER_RIGHT )
    {
      u8g_DrawPixel(u8g, x0 + x, y0 - y);
      u8g_DrawPixel(u8g, x0 + y, y0 - x);
    }

    /* upper left */
    if ( option & U8G_DRAW_UPPER_LEFT )
    {
      u8g_DrawPixel(u8g, x0 - x, y0 - y);
      u8g_DrawPixel(u8g, x0 - y, y0 - x);
    }

    /* lower right */
    if ( option & U8G_DRAW_LOWER_RIGHT )
    {
      u8g_DrawPixel(u8g, x0 + x, y0 + y);
      u8g_DrawPixel(u8g, x0 + y, y0 + x);
    }

    /* lower left */
    if ( option & U8G_DRAW_LOWER_LEFT )
    {
      u8g_DrawPixel(u8g, x0 - x, y0 + y);
      u8g_DrawPixel(u8g, x0 - y, y0 + x);
    }
}

void u8g_draw_circle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
    u8g_int_t f;
    u8g_int_t ddF_x;
    u8g_int_t ddF_y;
    u8g_uint_t x;
    u8g_uint_t y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    u8g_draw_circle_section(u8g, x, y, x0, y0, option);

    while ( x < y )
    {
      if (f >= 0)
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      u8g_draw_circle_section(u8g, x, y, x0, y0, option);
    }
}

void u8g_DrawCircle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
  /* check for bounding box */
  {
    u8g_uint_t radp, radp2;

    radp = rad;
    radp++;
    radp2 = radp;
    radp2 *= 2;

    if ( u8g_IsBBXIntersection(u8g, x0-radp, y0-radp, radp2, radp2) == 0)
      return;
  }

  /* draw circle */
  u8g_draw_circle(u8g, x0, y0, rad, option);
}

static void u8g_draw_disc_section(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0, uint8_t option) U8G_NOINLINE;

static void u8g_draw_disc_section(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t x0, u8g_uint_t y0, uint8_t option)
{
    /* upper right */
    if ( option & U8G_DRAW_UPPER_RIGHT )
    {
      u8g_DrawVLine(u8g, x0+x, y0-y, y+1);
      u8g_DrawVLine(u8g, x0+y, y0-x, x+1);
    }

    /* upper left */
    if ( option & U8G_DRAW_UPPER_LEFT )
    {
      u8g_DrawVLine(u8g, x0-x, y0-y, y+1);
      u8g_DrawVLine(u8g, x0-y, y0-x, x+1);
    }

    /* lower right */
    if ( option & U8G_DRAW_LOWER_RIGHT )
    {
      u8g_DrawVLine(u8g, x0+x, y0, y+1);
      u8g_DrawVLine(u8g, x0+y, y0, x+1);
    }

    /* lower left */
    if ( option & U8G_DRAW_LOWER_LEFT )
    {
      u8g_DrawVLine(u8g, x0-x, y0, y+1);
      u8g_DrawVLine(u8g, x0-y, y0, x+1);
    }
}

void u8g_draw_disc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
  u8g_int_t f;
  u8g_int_t ddF_x;
  u8g_int_t ddF_y;
  u8g_uint_t x;
  u8g_uint_t y;

  f = 1;
  f -= rad;
  ddF_x = 1;
  ddF_y = 0;
  ddF_y -= rad;
  ddF_y *= 2;
  x = 0;
  y = rad;

  u8g_draw_disc_section(u8g, x, y, x0, y0, option);

  while ( x < y )
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    u8g_draw_disc_section(u8g, x, y, x0, y0, option);
  }
}

void u8g_DrawDisc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option)
{
  /* check for bounding box */
  {
    u8g_uint_t radp, radp2;

    radp = rad;
    radp++;
    radp2 = radp;
    radp2 *= 2;

    if ( u8g_IsBBXIntersection(u8g, x0-radp, y0-radp, radp2, radp2) == 0)
      return;
  }

  /* draw disc */
  u8g_draw_disc(u8g, x0, y0, rad, option);
}




