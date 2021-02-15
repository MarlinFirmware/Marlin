/*

  u8g_polygon.c

  Implementation of a polygon draw algorithm for "convex" polygons.

  Universal 8bit Graphics Library

  Copyright (c) 2013, olikraus@gmail.com
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

  See also:
  http://www.angelfire.com/linux/myp/ConvexPolRas/ConvexPolRas.html
  Computer Graphics, Principles and Practice, Foley, van Dam, Feiner, Hughes (pp 92)
  Michael Abrash's Graphics Programming Black Book, Special Edition (Chapter 38 and 39)

  Optimized for embedded systems
  - static memory usage only
  - consistent data types
  - low flash ROM consumption

*/


#include "u8g.h"




/*===========================================*/
/* procedures, which should not be inlined (save as much flash ROM as possible */

static uint8_t pge_Next(struct pg_edge_struct *pge) PG_NOINLINE;
static uint8_t pg_inc(pg_struct *pg, uint8_t i) PG_NOINLINE;
static uint8_t pg_dec(pg_struct *pg, uint8_t i) PG_NOINLINE;
static void pg_expand_min_y(pg_struct *pg, pg_word_t min_y, uint8_t pge_idx) PG_NOINLINE;
static void pg_line_init(pg_struct * const pg, uint8_t pge_index) PG_NOINLINE;

/*===========================================*/
/* line draw algorithm */

static uint8_t pge_Next(struct pg_edge_struct *pge)
{
  if ( pge->current_y >= pge->max_y )
    return 0;

  pge->current_x += pge->current_x_offset;
  pge->error += pge->error_offset;
  if ( pge->error > 0 )
  {
    pge->current_x += pge->x_direction;
    pge->error -= pge->height;
  }

  pge->current_y++;
  return 1;
}

/* assumes y2 > y1 */
static void pge_Init(struct pg_edge_struct *pge, pg_word_t x1, pg_word_t y1, pg_word_t x2, pg_word_t y2)
{
  pg_word_t dx = x2 - x1;
  pg_word_t width;

  pge->height = y2 - y1;
  pge->max_y = y2;
  pge->current_y = y1;
  pge->current_x = x1;

  if ( dx >= 0 )
  {
    pge->x_direction = 1;
    width = dx;
    pge->error = 0;
  }
  else
  {
    pge->x_direction = -1;
    width = -dx;
    pge->error = 1 - pge->height;
  }

  pge->current_x_offset = dx / pge->height;
  pge->error_offset = width % pge->height;
}

/*===========================================*/
/* convex polygon algorithm */

static uint8_t pg_inc(pg_struct *pg, uint8_t i)
{
    i++;
    if ( i >= pg->cnt )
      i = 0;
    return i;
}

static uint8_t pg_dec(pg_struct *pg, uint8_t i)
{
    i--;
    if ( i >= pg->cnt )
      i = pg->cnt-1;
    return i;
}

static void pg_expand_min_y(pg_struct *pg, pg_word_t min_y, uint8_t pge_idx)
{
  uint8_t i = pg->pge[pge_idx].curr_idx;
  for(;;)
  {
    i = pg->pge[pge_idx].next_idx_fn(pg, i);
    if ( pg->list[i].y != min_y )
      break;
    pg->pge[pge_idx].curr_idx = i;
  }
}

static uint8_t pg_prepare(pg_struct *pg)
{
  pg_word_t max_y;
  pg_word_t min_y;
  uint8_t i;

  /* setup the next index procedures */
  pg->pge[PG_RIGHT].next_idx_fn = pg_inc;
  pg->pge[PG_LEFT].next_idx_fn = pg_dec;

  /* search for highest and lowest point */
  max_y = pg->list[0].y;
  min_y = pg->list[0].y;
  pg->pge[PG_LEFT].curr_idx = 0;
  for( i = 1; i < pg->cnt; i++ )
  {
    if ( max_y < pg->list[i].y )
    {
      max_y = pg->list[i].y;
    }
    if ( min_y > pg->list[i].y )
    {
      pg->pge[PG_LEFT].curr_idx = i;
      min_y = pg->list[i].y;
    }
  }

  /* calculate total number of scan lines */
  pg->total_scan_line_cnt = max_y;
  pg->total_scan_line_cnt -= min_y;

  /* exit if polygon height is zero */
  if ( pg->total_scan_line_cnt == 0 )
    return 0;

  /* if the minimum y side is flat, try to find the lowest and highest x points */
  pg->pge[PG_RIGHT].curr_idx = pg->pge[PG_LEFT].curr_idx;
  pg_expand_min_y(pg, min_y, PG_RIGHT);
  pg_expand_min_y(pg, min_y, PG_LEFT);

  /* check if the min side is really flat (depends on the x values) */
  pg->is_min_y_not_flat = 1;
  if ( pg->list[pg->pge[PG_LEFT].curr_idx].x != pg->list[pg->pge[PG_RIGHT].curr_idx].x )
  {
    pg->is_min_y_not_flat = 0;
  }
  else
  {
    pg->total_scan_line_cnt--;
    if ( pg->total_scan_line_cnt == 0 )
      return 0;
  }

  return 1;
}

static void pg_hline(pg_struct *pg, u8g_t *u8g)
{
  pg_word_t x1, x2, y;
  x1 = pg->pge[PG_LEFT].current_x;
  x2 = pg->pge[PG_RIGHT].current_x;
  y = pg->pge[PG_RIGHT].current_y;

  if ( y < 0 )
    return;
  if ( y >= u8g_GetHeight(u8g) )
    return;
  if ( x1 < x2 )
  {
    if ( x2 < 0 )
      return;
    if ( x1 >= u8g_GetWidth(u8g) )
      return;
    if ( x1 < 0 )
      x1 = 0;
    if ( x2 >= u8g_GetWidth(u8g) )
      x2 = u8g_GetWidth(u8g);
    u8g_DrawHLine(u8g, x1, y, x2 - x1);
  }
  else
  {
    if ( x1 < 0 )
      return;
    if ( x2 >= u8g_GetWidth(u8g) )
      return;
    if ( x2 < 0 )
      x1 = 0;
    if ( x1 >= u8g_GetWidth(u8g) )
      x1 = u8g_GetWidth(u8g);
    u8g_DrawHLine(u8g, x2, y, x1 - x2);
  }
}

static void pg_line_init(pg_struct * pg, uint8_t pge_index)
{
  struct pg_edge_struct  *pge = pg->pge+pge_index;
  uint8_t idx;
  pg_word_t x1;
  pg_word_t y1;
  pg_word_t x2;
  pg_word_t y2;

  idx = pge->curr_idx;
  y1 = pg->list[idx].y;
  x1 = pg->list[idx].x;
  idx = pge->next_idx_fn(pg, idx);
  y2 = pg->list[idx].y;
  x2 = pg->list[idx].x;
  pge->curr_idx = idx;

  pge_Init(pge, x1, y1, x2, y2);
}

static void pg_exec(pg_struct *pg, u8g_t *u8g)
{
  pg_word_t i = pg->total_scan_line_cnt;

  /* first line is skipped if the min y line is not flat */
  pg_line_init(pg, PG_LEFT);
  pg_line_init(pg, PG_RIGHT);

  if ( pg->is_min_y_not_flat != 0 )
  {
    pge_Next(&(pg->pge[PG_LEFT]));
    pge_Next(&(pg->pge[PG_RIGHT]));
  }

  do
  {
    pg_hline(pg, u8g);
    while ( pge_Next(&(pg->pge[PG_LEFT])) == 0 )
    {
      pg_line_init(pg, PG_LEFT);
    }
    while ( pge_Next(&(pg->pge[PG_RIGHT])) == 0 )
    {
      pg_line_init(pg, PG_RIGHT);
    }
    i--;
  } while( i > 0 );
}

/*===========================================*/
/* API procedures */

void pg_ClearPolygonXY(pg_struct *pg)
{
  pg->cnt = 0;
}

void pg_AddPolygonXY(pg_struct *pg, u8g_t *u8g, int16_t x, int16_t y)
{
  if ( pg->cnt < PG_MAX_POINTS )
  {
    pg->list[pg->cnt].x = x;
    pg->list[pg->cnt].y = y;
    pg->cnt++;
  }
}

void pg_DrawPolygon(pg_struct *pg, u8g_t *u8g)
{
  if ( pg_prepare(pg) == 0 )
    return;
  pg_exec(pg, u8g);
}

pg_struct u8g_pg;

void u8g_ClearPolygonXY(void)
{
  pg_ClearPolygonXY(&u8g_pg);
}

void u8g_AddPolygonXY(u8g_t *u8g, int16_t x, int16_t y)
{
  pg_AddPolygonXY(&u8g_pg, u8g, x, y);
}

void u8g_DrawPolygon(u8g_t *u8g)
{
  pg_DrawPolygon(&u8g_pg, u8g);
}

void u8g_DrawTriangle(u8g_t *u8g, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  u8g_ClearPolygonXY();
  u8g_AddPolygonXY(u8g, x0, y0);
  u8g_AddPolygonXY(u8g, x1, y1);
  u8g_AddPolygonXY(u8g, x2, y2);
  u8g_DrawPolygon(u8g);
}

