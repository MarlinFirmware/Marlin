/*

  u8g_pb8h1f.c

  8bit height monochrom (1 bit) page buffer
  byte has horizontal orientation, same as u8g_pb8h1, but byte is flipped

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


  total buffer size is limited to 256 bytes because of the calculation inside the set pixel procedure


*/

#include "u8g.h"
#include <string.h>


void u8g_pb8h1f_Init(u8g_pb_t *b, void *buf, u8g_uint_t width) U8G_NOINLINE;
void u8g_pb8h1f_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index) U8G_NOINLINE;
void u8g_pb8h1f_SetPixel(u8g_pb_t *b, const u8g_dev_arg_pixel_t * const arg_pixel) U8G_NOINLINE ;
void u8g_pb8h1f_Set8PixelStd(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel) U8G_NOINLINE;
uint8_t u8g_dev_pb8h1f_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


void u8g_pb8h1f_Init(u8g_pb_t *b, void *buf, u8g_uint_t width)
{
  b->buf = buf;
  b->width = width;
  u8g_pb_Clear(b);
}

/* limitation: total buffer must not exceed 256 bytes, 20 nov 2012: extended to >256 bytes */
void u8g_pb8h1f_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index)
{
  /*register uint8_t mask, tmp;*/
  register uint8_t mask;
  register u8g_uint_t tmp;
  uint8_t *ptr = b->buf;

  y -= b->p.page_y0;
  tmp = b->width >> 3;
  tmp *= (uint8_t)y;
  ptr += tmp;

  mask = 1;
  mask <<= x & 7;
  x >>= 3;
  ptr += x;
  if ( color_index )
  {
    *ptr |= mask;
  }
  else
  {
    mask ^=0xff;
    *ptr &= mask;
  }
}


void u8g_pb8h1f_SetPixel(u8g_pb_t *b, const u8g_dev_arg_pixel_t * const arg_pixel)
{
  if ( arg_pixel->y < b->p.page_y0 )
    return;
  if ( arg_pixel->y > b->p.page_y1 )
    return;
  if ( arg_pixel->x >= b->width )
    return;
  u8g_pb8h1f_set_pixel(b, arg_pixel->x, arg_pixel->y, arg_pixel->color);
}

void u8g_pb8h1f_Set8PixelStd(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
  register uint8_t pixel = arg_pixel->pixel;
  do
  {
    if ( pixel & 128 )
    {
      u8g_pb8h1f_SetPixel(b, arg_pixel);
    }
    switch( arg_pixel->dir )
    {
      case 0: arg_pixel->x++; break;
      case 1: arg_pixel->y++; break;
      case 2: arg_pixel->x--; break;
      case 3: arg_pixel->y--; break;
    }
    pixel <<= 1;
  } while( pixel != 0  );
}

void u8g_pb8h1f_Set8PixelOpt2(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
  register uint8_t pixel = arg_pixel->pixel;
  u8g_uint_t dx = 0;
  u8g_uint_t dy = 0;

  switch( arg_pixel->dir )
  {
    case 0: dx++; break;
    case 1: dy++; break;
    case 2: dx--; break;
    case 3: dy--; break;
  }

  do
  {
    if ( pixel & 128 )
      u8g_pb8h1f_SetPixel(b, arg_pixel);
    arg_pixel->x += dx;
    arg_pixel->y += dy;
    pixel <<= 1;
  } while( pixel != 0  );
}


uint8_t u8g_dev_pb8h1f_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
  switch(msg)
  {
    case U8G_DEV_MSG_SET_8PIXEL:
      if ( u8g_pb_Is8PixelVisible(pb, (u8g_dev_arg_pixel_t *)arg) )
        u8g_pb8h1f_Set8PixelOpt2(pb, (u8g_dev_arg_pixel_t *)arg);
      break;
    case U8G_DEV_MSG_SET_PIXEL:
      u8g_pb8h1f_SetPixel(pb, (u8g_dev_arg_pixel_t *)arg);
      break;
    case U8G_DEV_MSG_INIT:
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_FIRST:
      u8g_pb_Clear(pb);
      u8g_page_First(&(pb->p));
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      if ( u8g_page_Next(&(pb->p)) == 0 )
        return 0;
      u8g_pb_Clear(pb);
      break;
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
      return u8g_pb_IsIntersection(pb, (u8g_dev_arg_bbx_t *)arg);
#endif
    case U8G_DEV_MSG_GET_PAGE_BOX:
      u8g_pb_GetPageBox(pb, (u8g_box_t *)arg);
      break;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = pb->width;
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = pb->p.total_height;
      break;
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
      break;
    case U8G_DEV_MSG_SET_XY_CB:
      break;
    case U8G_DEV_MSG_GET_MODE:
      return U8G_MODE_BW;
  }
  return 1;
}


