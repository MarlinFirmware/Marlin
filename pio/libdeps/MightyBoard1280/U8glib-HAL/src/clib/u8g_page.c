/*

  u8g_page.c

  page helper functions, only called by the dev handler.

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

/*
  setup page count structure
  conditions: page_height <= total_height
*/
void u8g_page_Init(u8g_page_t *p, u8g_uint_t page_height, u8g_uint_t total_height )
{
  p->page_height = page_height;
  p->total_height = total_height;
  p->page = 0;
  u8g_page_First(p);
}

void u8g_page_First(u8g_page_t *p)
{
  p->page_y0 = 0;
  p->page_y1 = p->page_height;
  p->page_y1--;
  p->page = 0;
}

uint8_t u8g_page_Next(u8g_page_t * p)
{
  register u8g_uint_t y1;
  p->page_y0 += p->page_height;
  if ( p->page_y0 >= p->total_height )
    return 0;
  p->page++;
  y1 = p->page_y1;
  y1 += p->page_height;
  if ( y1 >= p->total_height )
  {
    y1 = p->total_height;
    y1--;
  }
  p->page_y1 = y1;

  return 1;
}



