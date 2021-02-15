/*

  u8g_dev_gprof.c

  Device for performance measurement with gprof.
  Does not write any data, but uses a buffer.

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


#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

uint8_t u8g_dev_gprof_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

uint8_t u8g_pb_dev_gprof_buf[WIDTH];
u8g_pb_t u8g_pb_dev_gprof = { {PAGE_HEIGHT, HEIGHT, 0, 0, 0},  WIDTH, u8g_pb_dev_gprof_buf };

u8g_dev_t u8g_dev_gprof = { u8g_dev_gprof_fn, &u8g_pb_dev_gprof, NULL };

uint8_t u8g_dev_gprof_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_FIRST:
      u8g_pb_Clear(pb);
      u8g_page_First(&(pb->p));
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      /*
      {
        uint8_t i, j;
        uint8_t page_height;
        page_height = pb->p.page_y1;
        page_height -= pb->p.page_y0;
        page_height++;
        for( j = 0; j < page_height; j++ )
        {
          printf("%02d ", j);
          for( i = 0; i < WIDTH; i++ )
          {
            if ( (u8g_pb_dev_stdout_buf[i] & (1<<j)) != 0 )
              printf("#");
            else
              printf(".");
          }
          printf("\n");
        }
      }
      */
      if ( u8g_page_Next(&(pb->p)) == 0 )
      {
        //printf("\n");
        return 0;
      }
      u8g_pb_Clear(pb);
      break;
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
       {
        u8g_dev_arg_bbx_t *bbx = (u8g_dev_arg_bbx_t *)arg;
        u8g_uint_t x2, y2;

        y2 = bbx->y;
        y2 += bbx->h;
        y2--;

        if ( u8g_pb_IsYIntersection(pb, bbx->y, y2) == 0 )
          return 0;

        /* maybe this one can be skiped... probability is very high to have an intersection, so it would be ok to always return 1 */
        x2 = bbx->x;
        x2 += bbx->w;
        x2--;

        if ( u8g_pb_IsXIntersection(pb, bbx->x, x2) == 0 )
          return 0;
      }
      return 1;
#endif
    case U8G_DEV_MSG_GET_PAGE_BOX:
      u8g_pb_GetPageBox(pb, (u8g_box_t *)arg);
      break;
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
      break;
    case U8G_DEV_MSG_SET_XY_CB:
      break;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}
