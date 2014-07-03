/*

  u8g_dev_flipdisc.c
  
  1-Bit (BW) Driver for flip disc matrix
  2x 7 pixel height

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

#define WIDTH 28
#define HEIGHT 14
#define PAGE_HEIGHT 14

/*
  Write data to the flip disc matrix.
  This procedure must be implemented by the user.
  Arguments:
    id:	Id for the matrix. Currently always 0.
    page: 	A page has a height of 14 pixel. For a matrix with HEIGHT == 14 this will be always 0
    width: 	The width of the flip disc matrix. Always equal to WIDTH
    row1: 	first data line (7 pixel per byte)
    row2: 	first data line (7 pixel per byte)
*/
void writeFlipDiscMatrix(uint8_t id, uint8_t page, uint8_t width, uint8_t *row1, uint8_t *row2);



void (*u8g_write_flip_disc_matrix)(uint8_t id, uint8_t page, uint8_t width, uint8_t *row1, uint8_t *row2);

void u8g_SetFlipDiscCallback(u8g_t *u8g, void (*cb)(uint8_t id, uint8_t page, uint8_t width, uint8_t *row1, uint8_t *row2))
{
  u8g_write_flip_disc_matrix = cb;
}

uint8_t u8g_dev_flipdisc_2x7_bw_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        
	/* current page: pb->p.page */
	/* ptr to the buffer: pb->buf */
	
	(*u8g_write_flip_disc_matrix)(0, pb->p.page, WIDTH, pb->buf, pb->buf+WIDTH);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      return 1;
  }
  return u8g_dev_pb14v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_flipdisc_2x7_bw_buf[WIDTH*2] U8G_NOCOMMON ; 
u8g_pb_t u8g_dev_flipdisc_2x7_bw_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_flipdisc_2x7_bw_buf}; 
u8g_dev_t u8g_dev_flipdisc_2x7 = { u8g_dev_flipdisc_2x7_bw_fn, &u8g_dev_flipdisc_2x7_bw_pb, u8g_com_null_fn };
