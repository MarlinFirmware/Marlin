/*

  u8g_cursor.c

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

void u8g_SetCursorFont(u8g_t *u8g, const u8g_pgm_uint8_t *cursor_font)
{
  u8g->cursor_font = cursor_font;
}

void u8g_SetCursorStyle(u8g_t *u8g, uint8_t encoding)
{
  u8g->cursor_encoding = encoding;
}

void u8g_SetCursorColor(u8g_t *u8g, uint8_t fg, uint8_t bg)
{
  u8g->cursor_bg_color = bg;
  u8g->cursor_fg_color = fg;
}

void u8g_SetCursorPos(u8g_t *u8g, u8g_uint_t cursor_x, u8g_uint_t cursor_y)
{
  u8g->cursor_x = cursor_x;
  u8g->cursor_y = cursor_y;
}

void u8g_EnableCursor(u8g_t *u8g)
{
    u8g->cursor_fn = u8g_DrawCursor;
}

void u8g_DisableCursor(u8g_t *u8g)
{
    u8g->cursor_fn = (u8g_draw_cursor_fn)0;
}

void u8g_DrawCursor(u8g_t *u8g)
{
  const u8g_pgm_uint8_t *font;
  uint8_t color;
  uint8_t encoding = u8g->cursor_encoding;

  /* get current values */
  color = u8g_GetColorIndex(u8g);
  font = u8g->font;

  /* draw cursor */
  u8g->font = u8g->cursor_font;
  encoding++;
  u8g_SetColorIndex(u8g, u8g->cursor_bg_color);
  /* 27. Jan 2013: replaced call to u8g_DrawGlyph with call to u8g_draw_glyph */
  /* required, because y adjustment should not happen to the cursor fonts */
  u8g_draw_glyph(u8g, u8g->cursor_x, u8g->cursor_y, encoding);
  encoding--;
  u8g_SetColorIndex(u8g, u8g->cursor_fg_color);
  /* 27. Jan 2013: replaced call to u8g_DrawGlyph with call to u8g_draw_glyph */
  /* required, because y adjustment should not happen to the cursor fonts */
  /* u8g_DrawGlyph(u8g, u8g->cursor_x, u8g->cursor_y, encoding); */
  u8g_draw_glyph(u8g, u8g->cursor_x, u8g->cursor_y, encoding);

  /* restore previous values */
  u8g->font = font;
  u8g_SetColorIndex(u8g, color);
}

