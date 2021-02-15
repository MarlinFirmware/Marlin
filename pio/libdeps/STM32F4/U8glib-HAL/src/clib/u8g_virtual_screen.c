/*

  u8g_virtual_screen.c

  Universal 8bit Graphics Library

  Copyright (c) 2012, olikraus@gmail.com
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

struct _u8g_vs_t
{
  u8g_uint_t x;
  u8g_uint_t y;
  u8g_t *u8g;
};
typedef struct _u8g_vs_t u8g_vs_t;

#define U8g_VS_MAX 4
uint8_t u8g_vs_cnt = 0;
u8g_vs_t u8g_vs_list[U8g_VS_MAX];
uint8_t u8g_vs_current;
u8g_uint_t u8g_vs_width;
u8g_uint_t u8g_vs_height;

uint8_t u8g_dev_vs_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    default:
      {
	uint8_t i;
	for( i = 0; i < u8g_vs_cnt; i++ )
	{
	  u8g_call_dev_fn(u8g_vs_list[i].u8g, u8g_vs_list[i].u8g->dev, msg, arg);
	}
      }
      return 1;
    case U8G_DEV_MSG_PAGE_FIRST:
      u8g_vs_current = 0;
      if ( u8g_vs_cnt != 0 )
	return u8g_call_dev_fn(u8g_vs_list[u8g_vs_current].u8g, u8g_vs_list[u8g_vs_current].u8g->dev, msg, arg);
      return 0;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
	uint8_t ret = 0;
	if ( u8g_vs_cnt != 0 )
	  ret = u8g_call_dev_fn(u8g_vs_list[u8g_vs_current].u8g, u8g_vs_list[u8g_vs_current].u8g->dev, msg, arg);
	if ( ret != 0 )
	  return ret;
	u8g_vs_current++;	/* next device */
	if ( u8g_vs_current >= u8g_vs_cnt )  /* reached end? */
	  return 0;
	return u8g_call_dev_fn(u8g_vs_list[u8g_vs_current].u8g, u8g_vs_list[u8g_vs_current].u8g->dev, U8G_DEV_MSG_PAGE_FIRST, arg);
      }
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = u8g_vs_width;
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = u8g_vs_height;
      break;
    case U8G_DEV_MSG_GET_PAGE_BOX:
      if ( u8g_vs_current < u8g_vs_cnt )
      {
	u8g_call_dev_fn(u8g_vs_list[u8g_vs_current].u8g, u8g_vs_list[u8g_vs_current].u8g->dev, msg, arg);
	((u8g_box_t *)arg)->x0 += u8g_vs_list[u8g_vs_current].x;
	((u8g_box_t *)arg)->x1 += u8g_vs_list[u8g_vs_current].x;
	((u8g_box_t *)arg)->y0 += u8g_vs_list[u8g_vs_current].y;
	((u8g_box_t *)arg)->y1 += u8g_vs_list[u8g_vs_current].y;
      }
      else
      {
	((u8g_box_t *)arg)->x0 = 0;
	((u8g_box_t *)arg)->x1 = 0;
	((u8g_box_t *)arg)->y0 = 0;
	((u8g_box_t *)arg)->y1 = 0;
      }
      return 1;
    case U8G_DEV_MSG_SET_PIXEL:
    case U8G_DEV_MSG_SET_8PIXEL:
      if ( u8g_vs_current < u8g_vs_cnt )
      {
        ((u8g_dev_arg_pixel_t *)arg)->x -= u8g_vs_list[u8g_vs_current].x;
        ((u8g_dev_arg_pixel_t *)arg)->y -= u8g_vs_list[u8g_vs_current].y;
	return u8g_call_dev_fn(u8g_vs_list[u8g_vs_current].u8g, u8g_vs_list[u8g_vs_current].u8g->dev, msg, arg);
      }
      break;
  }
  return 1;
}



u8g_dev_t u8g_dev_vs = { u8g_dev_vs_fn, NULL, NULL };

void u8g_SetVirtualScreenDimension(u8g_t *vs_u8g, u8g_uint_t width, u8g_uint_t height)
{
  if ( vs_u8g->dev != &u8g_dev_vs )
    return; 	/* abort if there is no a virtual screen device */
  u8g_vs_width = width;
  u8g_vs_height = height;
}

uint8_t u8g_AddToVirtualScreen(u8g_t *vs_u8g, u8g_uint_t x, u8g_uint_t y, u8g_t *child_u8g)
{
  if ( vs_u8g->dev != &u8g_dev_vs )
    return 0; 	/* abort if there is no a virtual screen device */
  if ( u8g_vs_cnt >= U8g_VS_MAX )
    return 0;  	/* maximum number of  child u8g's reached */
  u8g_vs_list[u8g_vs_cnt].u8g = child_u8g;
  u8g_vs_list[u8g_vs_cnt].x = x;
  u8g_vs_list[u8g_vs_cnt].y = y;
  u8g_vs_cnt++;
  return 1;
}

