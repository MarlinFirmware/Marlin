/*

  u8g_font.c
  
  U8G Font High Level Interface

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

/* font api */

/* pointer to the start adress of the glyph, points to progmem area */
typedef void * u8g_glyph_t;

/* size of the font data structure, there is no struct or class... */
#define U8G_FONT_DATA_STRUCT_SIZE 17

/*
  ... instead the fields of the font data structure are accessed directly by offset 
  font information 
  offset
  0             font format
  1             FONTBOUNDINGBOX width           unsigned
  2             FONTBOUNDINGBOX height          unsigned
  3             FONTBOUNDINGBOX x-offset         signed
  4             FONTBOUNDINGBOX y-offset        signed
  5             capital A height                                unsigned
  6             start 'A'
  8             start 'a'
  10            encoding start
  11            encoding end
  12            descent 'g'                     negative: below baseline
  13            font max ascent
  14            font min decent             negative: below baseline 
  15            font xascent
  16            font xdecent             negative: below baseline 
  
*/

/* use case: What is the width and the height of the minimal box into which string s fints? */
void u8g_font_GetStrSize(const void *font, const char *s, u8g_uint_t *width, u8g_uint_t *height);
void u8g_font_GetStrSizeP(const void *font, const char *s, u8g_uint_t *width, u8g_uint_t *height);

/* use case: lower left edge of a minimal box is known, what is the correct x, y position for the string draw procedure */
void u8g_font_AdjustXYToDraw(const void *font, const char *s, u8g_uint_t *x, u8g_uint_t *y);
void u8g_font_AdjustXYToDrawP(const void *font, const char *s, u8g_uint_t *x, u8g_uint_t *y);

/* use case: Baseline origin known, return minimal box */
void u8g_font_GetStrMinBox(u8g_t *u8g, const void *font, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height);

/* procedures */

/*========================================================================*/
/* low level byte and word access */

/* removed NOINLINE, because it leads to smaller code, might also be faster */
//static uint8_t u8g_font_get_byte(const u8g_fntpgm_uint8_t *font, uint8_t offset) U8G_NOINLINE;
static uint8_t u8g_font_get_byte(const u8g_fntpgm_uint8_t *font, uint8_t offset)
{
  font += offset;
  return u8g_pgm_read( (u8g_pgm_uint8_t *)font );  
}

static uint16_t u8g_font_get_word(const u8g_fntpgm_uint8_t *font, uint8_t offset) U8G_NOINLINE; 
static uint16_t u8g_font_get_word(const u8g_fntpgm_uint8_t *font, uint8_t offset)
{
    uint16_t pos;
    font += offset;
    pos = u8g_pgm_read( (u8g_pgm_uint8_t *)font );
    font++;
    pos <<= 8;
    pos += u8g_pgm_read( (u8g_pgm_uint8_t *)font);
    return pos;
}

/*========================================================================*/
/* direct access on the font */

static uint8_t u8g_font_GetFormat(const u8g_fntpgm_uint8_t *font) U8G_NOINLINE;
static uint8_t u8g_font_GetFormat(const u8g_fntpgm_uint8_t *font)
{
  return u8g_font_get_byte(font, 0);
}

static uint8_t u8g_font_GetFontGlyphStructureSize(const u8g_fntpgm_uint8_t *font) U8G_NOINLINE;
static uint8_t u8g_font_GetFontGlyphStructureSize(const u8g_fntpgm_uint8_t *font)
{
  switch(u8g_font_GetFormat(font))
  {
    case 0: return 6;
    case 1: return 3;
    case 2: return 6;
  }
  return 3;
}

static uint8_t u8g_font_GetBBXWidth(const void *font)
{
  return u8g_font_get_byte(font, 1);
}

static uint8_t u8g_font_GetBBXHeight(const void *font)
{
  return u8g_font_get_byte(font, 2);
}

static int8_t u8g_font_GetBBXOffX(const void *font)
{
  return u8g_font_get_byte(font, 3);
}

static int8_t u8g_font_GetBBXOffY(const void *font)
{
  return u8g_font_get_byte(font, 4);
}

uint8_t u8g_font_GetCapitalAHeight(const void *font)
{
  return u8g_font_get_byte(font, 5);
}

uint16_t u8g_font_GetEncoding65Pos(const void *font) U8G_NOINLINE;
uint16_t u8g_font_GetEncoding65Pos(const void *font)
{
    return u8g_font_get_word(font, 6);
}

uint16_t u8g_font_GetEncoding97Pos(const void *font) U8G_NOINLINE;
uint16_t u8g_font_GetEncoding97Pos(const void *font)
{
    return u8g_font_get_word(font, 8);
}

uint8_t u8g_font_GetFontStartEncoding(const void *font)
{
  return u8g_font_get_byte(font, 10);
}

uint8_t u8g_font_GetFontEndEncoding(const void *font)
{
  return u8g_font_get_byte(font, 11);
}

int8_t u8g_font_GetLowerGDescent(const void *font)
{
  return u8g_font_get_byte(font, 12);
}

int8_t u8g_font_GetFontAscent(const void *font)
{
  return u8g_font_get_byte(font, 13);
}

int8_t u8g_font_GetFontDescent(const void *font)
{
  return u8g_font_get_byte(font, 14);
}

int8_t u8g_font_GetFontXAscent(const void *font)
{
  return u8g_font_get_byte(font, 15);
}

int8_t u8g_font_GetFontXDescent(const void *font)
{
  return u8g_font_get_byte(font, 16);
}


/* return the data start for a font and the glyph pointer */
static uint8_t *u8g_font_GetGlyphDataStart(const void *font, u8g_glyph_t g)
{
  return ((u8g_fntpgm_uint8_t *)g) + u8g_font_GetFontGlyphStructureSize(font);
}

/* calculate the overall length of the font, only used to create the picture for the google wiki */
size_t u8g_font_GetSize(const void *font)
{
  uint8_t *p = (uint8_t *)(font);
  uint8_t font_format = u8g_font_GetFormat(font);
  uint8_t data_structure_size = u8g_font_GetFontGlyphStructureSize(font);
  uint8_t start, end;
  uint8_t i;
  uint8_t mask = 255;
  
  start = u8g_font_GetFontStartEncoding(font);
  end = u8g_font_GetFontEndEncoding(font);

  if ( font_format == 1 )
    mask = 15;

  p += U8G_FONT_DATA_STRUCT_SIZE;       /* skip font general information */  

  i = start;  
  for(;;)
  {
    if ( u8g_pgm_read((u8g_pgm_uint8_t *)(p)) == 255 )
    {
      p += 1;
    }
    else
    {
      p += u8g_pgm_read( ((u8g_pgm_uint8_t *)(p)) + 2 ) & mask;
      p += data_structure_size;
    }
    if ( i == end )
      break;
    i++;
  }
    
  return p - (uint8_t *)font;
}

/*========================================================================*/
/* u8g interface, font access */

uint8_t u8g_GetFontBBXWidth(u8g_t *u8g)
{
  return u8g_font_GetBBXWidth(u8g->font);
}

uint8_t u8g_GetFontBBXHeight(u8g_t *u8g)
{
  return u8g_font_GetBBXHeight(u8g->font);
}

int8_t u8g_GetFontBBXOffX(u8g_t *u8g) U8G_NOINLINE;
int8_t u8g_GetFontBBXOffX(u8g_t *u8g)
{
  return u8g_font_GetBBXOffX(u8g->font);
}

int8_t u8g_GetFontBBXOffY(u8g_t *u8g) U8G_NOINLINE;
int8_t u8g_GetFontBBXOffY(u8g_t *u8g)
{
  return u8g_font_GetBBXOffY(u8g->font);
}

uint8_t u8g_GetFontCapitalAHeight(u8g_t *u8g) U8G_NOINLINE; 
uint8_t u8g_GetFontCapitalAHeight(u8g_t *u8g)
{
  return u8g_font_GetCapitalAHeight(u8g->font);
}

/*========================================================================*/
/* glyph handling */

static void u8g_CopyGlyphDataToCache(u8g_t *u8g, u8g_glyph_t g)
{
  uint8_t tmp;
  switch( u8g_font_GetFormat(u8g->font) )
  {
    case 0:
    case 2:
  /*
    format 0
    glyph information 
    offset
    0             BBX width                                       unsigned
    1             BBX height                                      unsigned
    2             data size                                          unsigned    (BBX width + 7)/8 * BBX height
    3             DWIDTH                                          signed
    4             BBX xoffset                                    signed
    5             BBX yoffset                                    signed
  byte 0 == 255 indicates empty glyph
  */
      u8g->glyph_width =  u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 0 );
      u8g->glyph_height =  u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 1 );
      u8g->glyph_dx =  u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 3 );
      u8g->glyph_x =  u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 4 );
      u8g->glyph_y =  u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 5 );
      break;
    case 1:
    default:
      /*
format 1
  0             BBX xoffset                                    signed   --> upper 4 Bit
  0             BBX yoffset                                    signed --> lower 4 Bit
  1             BBX width                                       unsigned --> upper 4 Bit
  1             BBX height                                      unsigned --> lower 4 Bit
  2             data size                                           unsigned -(BBX width + 7)/8 * BBX height  --> lower 4 Bit
  2             DWIDTH                                          signed --> upper  4 Bit
  byte 0 == 255 indicates empty glyph
      */
    
      tmp = u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 0 );
      u8g->glyph_y =  tmp & 15;
      u8g->glyph_y-=2;
      tmp >>= 4;
      u8g->glyph_x =  tmp;
    
      tmp = u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 1 );
      u8g->glyph_height =  tmp & 15;
      tmp >>= 4;
      u8g->glyph_width =  tmp;
      
      tmp = u8g_pgm_read( ((u8g_pgm_uint8_t *)g) + 2 );
      tmp >>= 4;
      u8g->glyph_dx = tmp;
    
      
      break;
  }
}

//void u8g_FillEmptyGlyphCache(u8g_t *u8g) U8G_NOINLINE;
static void u8g_FillEmptyGlyphCache(u8g_t *u8g)
{
  u8g->glyph_dx = 0;
  u8g->glyph_width = 0;
  u8g->glyph_height = 0;
  u8g->glyph_x = 0;
  u8g->glyph_y = 0;
}

/*
  Find (with some speed optimization) and return a pointer to the glyph data structure
  Also uncompress (format 1) and copy the content of the data structure to the u8g structure
*/
u8g_glyph_t u8g_GetGlyph(u8g_t *u8g, uint8_t requested_encoding)
{
  uint8_t *p = (uint8_t *)(u8g->font);
  uint8_t font_format = u8g_font_GetFormat(u8g->font);
  uint8_t data_structure_size = u8g_font_GetFontGlyphStructureSize(u8g->font);
  uint8_t start, end;
  uint16_t pos;
  uint8_t i;
  uint8_t mask = 255;

  if ( font_format == 1 )
    mask = 15;
  
  start = u8g_font_GetFontStartEncoding(u8g->font);
  end = u8g_font_GetFontEndEncoding(u8g->font);

  pos = u8g_font_GetEncoding97Pos(u8g->font);
  if ( requested_encoding >= 97 && pos > 0 )
  {
    p+= pos;
    start = 97;
  }
  else 
  {
    pos = u8g_font_GetEncoding65Pos(u8g->font);
    if ( requested_encoding >= 65 && pos > 0 )
    {
      p+= pos;
      start = 65;
    }
    else
      p += U8G_FONT_DATA_STRUCT_SIZE;       /* skip font general information */  
  }
  
  if ( requested_encoding > end )
  {
    u8g_FillEmptyGlyphCache(u8g);
    return NULL;                      /* not found */
  }
  
  i = start;
  if ( i <= end )
  {
    for(;;)
    {
      if ( u8g_pgm_read((u8g_pgm_uint8_t *)(p)) == 255 )
      {
        p += 1;
      }
      else
      {
        if ( i == requested_encoding )
        {
          u8g_CopyGlyphDataToCache(u8g, p);
          return p;
        }
        p += u8g_pgm_read( ((u8g_pgm_uint8_t *)(p)) + 2 ) & mask;
        p += data_structure_size;
      }
      if ( i == end )
        break;
      i++;
    }
  }
  
  u8g_FillEmptyGlyphCache(u8g);
    
  return NULL;
}

uint8_t u8g_IsGlyph(u8g_t *u8g, uint8_t requested_encoding)
{
  if ( u8g_GetGlyph(u8g, requested_encoding) != NULL )
    return 1;
  return 0;
}

int8_t u8g_GetGlyphDeltaX(u8g_t *u8g, uint8_t requested_encoding)
{
  if ( u8g_GetGlyph(u8g, requested_encoding) == NULL )
    return 0;  /* should never happen, so return something */
  return u8g->glyph_dx;
}


/*========================================================================*/
/* glyph drawing procedures */

#ifdef OBSOLETE
/*
  Draw a glyph
  x,y: left baseline position of the glyph
*/
int8_t u8g_DrawGlyphDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding)
{
  u8g_glyph_t g;
  uint8_t w, h, i, j;
  const u8g_pgm_uint8_t *data;
  uint8_t bytes_per_line;
  u8g_uint_t ix, iy;

  g = u8g_GetGlyph(u8g, encoding);
  if ( g == NULL  )
    return 0;
  w = u8g->glyph_width;
  h = u8g->glyph_height;

  bytes_per_line = w;
  bytes_per_line += 7;
  bytes_per_line /= 8;
  
  data = u8g_font_GetGlyphDataStart(u8g->font, g);

  switch(dir)
  {
    case 0:
      x += u8g->glyph_x;
      y -= u8g->glyph_y;
      y--;
      //u8g_DrawFrame(u8g, x, y-h+1, w, h);
      if ( u8g_IsBBXIntersection(u8g, x, y-h+1, w, h) == 0 )
        return u8g->glyph_dx;
      
      iy = y;
      iy -= h;
      iy++;
      
      for( j = 0; j < h; j++ )
      {
        ix = x;
        for( i = 0; i < bytes_per_line; i++ )
        {
          u8g_Draw8Pixel(u8g, ix, iy, dir, u8g_pgm_read(data));
          data++;
          ix+=8;
        }
        iy++;
      }
      break;
    case 1:
      x += u8g->glyph_y;
      x++;
      y += u8g->glyph_x;
      //printf("enc %d, dir %d, x %d, y %d, w %d, h %d\n", encoding, dir, x, y, w, h);
      //u8g_DrawFrame(u8g, x, y, h, w);
      if ( u8g_IsBBXIntersection(u8g, x, y, h, w) == 0 )
        return u8g->glyph_dx;
      
      ix = x;
      ix += h;
      ix--;
      for( j = 0; j < h; j++ )
      {
        iy = y;
        for( i = 0; i < bytes_per_line; i++ )
        {
          u8g_Draw8Pixel(u8g, ix, iy, dir, u8g_pgm_read(data));
          data++;
          iy+=8;
        }
        ix--;
      }
      break;
    case 2:
      x -= u8g->glyph_x;
      y += u8g->glyph_y;
      y++;
      if ( u8g_IsBBXIntersection(u8g, x-w-1, y, w, h) == 0 )
        return u8g->glyph_dx;
      
      iy = y;
      iy += h;
      iy--;
      for( j = 0; j < h; j++ )
      {
        ix = x;
        for( i = 0; i < bytes_per_line; i++ )
        {
          u8g_Draw8Pixel(u8g, ix, iy, dir, u8g_pgm_read(data));
          data++;
          ix-=8;
        }
        iy--;
      }
      break;
    case 3:
      x -= u8g->glyph_y;
      x--;
      y -= u8g->glyph_x;
      
      if ( u8g_IsBBXIntersection(u8g, x-h-1, y-w-1, h, w) == 0 )
        return u8g->glyph_dx;
      
      ix = x;
      ix -= h;
      ix++;
      
      for( j = 0; j < h; j++ )
      {
        iy = y;
        for( i = 0; i < bytes_per_line; i++ )
        {
          u8g_Draw8Pixel(u8g, ix, iy, dir, u8g_pgm_read(data));
          data++;
          iy-=8;
        }
        ix++;
      }
      break;    
  }
  return u8g->glyph_dx;
}
#endif

int8_t u8g_draw_glyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  const u8g_pgm_uint8_t *data;
  uint8_t w, h;
  uint8_t i, j;
  u8g_uint_t ix, iy;

  {
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  }
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x += u8g->glyph_x;
  y -= u8g->glyph_y;
  y--;
  
  if ( u8g_IsBBXIntersection(u8g, x, y-h+1, w, h) == 0 )
    return u8g->glyph_dx;

  /* now, w is reused as bytes per line */
  w += 7;
  w /= 8;
  
  iy = y;
  iy -= h;
  iy++;

  for( j = 0; j < h; j++ )
  {
    ix = x;
    for( i = 0; i < w; i++ )
    {
      u8g_Draw8Pixel(u8g, ix, iy, 0, u8g_pgm_read(data));
      data++;
      ix+=8;
    }
    iy++;
  }
  return u8g->glyph_dx;
}

int8_t u8g_DrawGlyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  y += u8g->font_calc_vref(u8g);
  return u8g_draw_glyph(u8g, x, y, encoding);
}

int8_t u8g_draw_glyph90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  const u8g_pgm_uint8_t *data;
  uint8_t w, h;
  uint8_t i, j;
  u8g_uint_t ix, iy;

  {
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  }
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x += u8g->glyph_y;
  x++;
  y += u8g->glyph_x;
  
  if ( u8g_IsBBXIntersection(u8g, x, y, h, w) == 0 )
    return u8g->glyph_dx;

  /* now, w is reused as bytes per line */
  w += 7;
  w /= 8;
  
  ix = x;
  ix += h;
  ix--;
  for( j = 0; j < h; j++ )
  {
    iy = y;
    for( i = 0; i < w; i++ )
    {
      u8g_Draw8Pixel(u8g, ix, iy, 1, u8g_pgm_read(data));
      data++;
      iy+=8;
    }
    ix--;
  }
  return u8g->glyph_dx;
}

int8_t u8g_DrawGlyph90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  x -= u8g->font_calc_vref(u8g);
  return u8g_draw_glyph90(u8g, x, y, encoding);
}


int8_t u8g_draw_glyph180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  const u8g_pgm_uint8_t *data;
  uint8_t w, h;
  uint8_t i, j;
  u8g_uint_t ix, iy;

  {
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  }
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x -= u8g->glyph_x;
  y += u8g->glyph_y;
  y++;
  
  if ( u8g_IsBBXIntersection(u8g, x-(w-1), y, w, h) == 0 )
    return u8g->glyph_dx;

  /* now, w is reused as bytes per line */
  w += 7;
  w /= 8;
  
  iy = y;
  iy += h;
  iy--;
  for( j = 0; j < h; j++ )
  {
    ix = x;
    for( i = 0; i < w; i++ )
    {
      u8g_Draw8Pixel(u8g, ix, iy, 2, u8g_pgm_read(data));
      data++;
      ix-=8;
    }
    iy--;
  }
  return u8g->glyph_dx;
}

int8_t u8g_DrawGlyph180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  y -= u8g->font_calc_vref(u8g);
  return u8g_draw_glyph180(u8g, x, y, encoding);
}


int8_t u8g_draw_glyph270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  const u8g_pgm_uint8_t *data;
  uint8_t w, h;
  uint8_t i, j;
  u8g_uint_t ix, iy;

  {
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  }
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x -= u8g->glyph_y;
  x--;
  y -= u8g->glyph_x;
  
  if ( u8g_IsBBXIntersection(u8g, x-(h-1), y-(w-1), h, w) == 0 )
    return u8g->glyph_dx;
  

  /* now, w is reused as bytes per line */
  w += 7;
  w /= 8;  
      
  ix = x;
  ix -= h;
  ix++;
  
  for( j = 0; j < h; j++ )
  {
    iy = y;
    for( i = 0; i < w; i++ )
    {
      u8g_Draw8Pixel(u8g, ix, iy, 3, u8g_pgm_read(data));
      data++;
      iy-=8;
    }
    ix++;
  }
  return u8g->glyph_dx;
}

int8_t u8g_DrawGlyph270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  x += u8g->font_calc_vref(u8g);
  return u8g_draw_glyph270(u8g, x, y, encoding);
}



#ifdef OBSOLETE
/*
  Draw a glyph
  x,y: lower left corner of the font bounding box
*/
int8_t u8g_DrawGlyphFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding)
{
  /* TODO: apply "dir" */
  x -= u8g_GetFontBBXOffX(u8g);
  y += u8g_GetFontBBXOffY(u8g);
  return u8g_DrawGlyphDir(u8g, x, y, dir, encoding);
}
#endif

/*========================================================================*/
/* string drawing procedures */


u8g_uint_t u8g_DrawStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s)
{
  u8g_uint_t t = 0;
  int8_t d;
  
  //u8g_uint_t u8g_GetStrWidth(u8g, s);
  //u8g_font_GetFontAscent(u8g->font)-u8g_font_GetFontDescent(u8g->font);
  
  y += u8g->font_calc_vref(u8g);
  
  while( *s != '\0' )
  {
    d = u8g_draw_glyph(u8g, x, y, *s);
    x += d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s)
{
  u8g_uint_t t = 0;
  int8_t d;
    
  x -= u8g->font_calc_vref(u8g);

  while( *s != '\0' )
  {
    d = u8g_draw_glyph90(u8g, x, y, *s);
    y += d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s)
{
  u8g_uint_t t = 0;
  int8_t d;

  y -= u8g->font_calc_vref(u8g);
  
  while( *s != '\0' )
  {
    d = u8g_draw_glyph180(u8g, x, y, *s);
    x -= d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s)
{
  u8g_uint_t t = 0;
  int8_t d;

  x += u8g->font_calc_vref(u8g);

  while( *s != '\0' )
  {
    d = u8g_draw_glyph270(u8g, x, y, *s);
    y -= d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStrDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s)
{
  switch(dir)
  {
    case 0:
      return u8g_DrawStr(u8g, x, y, s);
    case 1:
      return u8g_DrawStr90(u8g, x, y, s);
    case 2:
      return u8g_DrawStr180(u8g, x, y, s);
    case 3:
      return u8g_DrawStr270(u8g, x, y, s);
  }
  return 0;
}

u8g_uint_t u8g_DrawStrP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s)
{
  u8g_uint_t t = 0;
  int8_t d;
  uint8_t c;
  
  y += u8g->font_calc_vref(u8g);
  
  for(;;)
  {
    c = u8g_pgm_read(s);
    if ( c == '\0' )
      break;
    d = u8g_draw_glyph(u8g, x, y, c);
    x += d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr90P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s)
{
  u8g_uint_t t = 0;
  int8_t d;
  uint8_t c;
  
  x -= u8g->font_calc_vref(u8g);

  for(;;)
  {
    c = u8g_pgm_read(s);
    if ( c == '\0' )
      break;
    d = u8g_DrawGlyph90(u8g, x, y, c);
    y += d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr180P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s)
{
  u8g_uint_t t = 0;
  int8_t d;
  uint8_t c;

  y -= u8g->font_calc_vref(u8g);
  
  for(;;)
  {
    c = u8g_pgm_read(s);
    if ( c == '\0' )
      break;
    d = u8g_DrawGlyph180(u8g, x, y, c);
    x -= d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStr270P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s)
{
  u8g_uint_t t = 0;
  int8_t d;
  uint8_t c;

  x += u8g->font_calc_vref(u8g);

  for(;;)
  {
    c = u8g_pgm_read(s);
    if ( c == '\0' )
      break;
    d = u8g_DrawGlyph270(u8g, x, y, c);
    y -= d;
    t += d;
    s++;
  }
  return t;
}

u8g_uint_t u8g_DrawStrFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s)
{
  x -= u8g_GetFontBBXOffX(u8g);
  y += u8g_GetFontBBXOffY(u8g);
  return u8g_DrawStrDir(u8g, x, y, dir, s);
}

/* still used by picgen.c, dir argument is ignored */
int8_t u8g_DrawGlyphFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding)
{
  x -= u8g_GetFontBBXOffX(u8g);
  y += u8g_GetFontBBXOffY(u8g);
  u8g_draw_glyph(u8g, x, y, encoding);
  return 0;
}


/*========================================================================*/
/* set ascent/descent for reference point calculation */

void u8g_UpdateRefHeight(u8g_t *u8g)
{
  uint16_t ls;
  if ( u8g->font == NULL )
    return;
  if ( u8g->font_height_mode == U8G_FONT_HEIGHT_MODE_TEXT )
  {
    u8g->font_ref_ascent = u8g_font_GetCapitalAHeight(u8g->font);
    u8g->font_ref_descent = u8g_font_GetLowerGDescent(u8g->font);
  }
  else if ( u8g->font_height_mode == U8G_FONT_HEIGHT_MODE_XTEXT )
  {
    u8g->font_ref_ascent = u8g_font_GetFontXAscent(u8g->font);
    u8g->font_ref_descent = u8g_font_GetFontXDescent(u8g->font);
  }
  else
  {
    u8g->font_ref_ascent = u8g_font_GetFontAscent(u8g->font);
    u8g->font_ref_descent = u8g_font_GetFontDescent(u8g->font);
  }
  
  ls = u8g->font_ref_ascent - u8g->font_ref_descent;
  if ( u8g->font_line_spacing_factor != 64 )
  {
    ls &= 255;
    ls *= u8g->font_line_spacing_factor;
    ls >>= 6;
  }
  u8g->line_spacing = ls;
}

void u8g_SetFontRefHeightText(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_TEXT;
  u8g_UpdateRefHeight(u8g);
}

void u8g_SetFontRefHeightExtendedText(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_XTEXT;
  u8g_UpdateRefHeight(u8g);
}


void u8g_SetFontRefHeightAll(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_ALL;
  u8g_UpdateRefHeight(u8g);
}

/* factor = 64: linespaceing == ascent and descent */
void u8g_SetFontLineSpacingFactor(u8g_t *u8g, uint8_t  factor)
{
  u8g->font_line_spacing_factor = factor;
  u8g_UpdateRefHeight(u8g);
}



/*========================================================================*/
/* callback procedures to correct the y position */

u8g_uint_t u8g_font_calc_vref_font(u8g_t *u8g)
{
  return 0;
}

void u8g_SetFontPosBaseline(u8g_t *u8g)
{
  u8g->font_calc_vref = u8g_font_calc_vref_font;
}


u8g_uint_t u8g_font_calc_vref_bottom(u8g_t *u8g)
{
  /* y += (u8g_uint_t)(u8g_int_t)(u8g->font_ref_descent); */
  return (u8g_uint_t)(u8g_int_t)(u8g->font_ref_descent);
}

void u8g_SetFontPosBottom(u8g_t *u8g)
{
  u8g->font_calc_vref = u8g_font_calc_vref_bottom;
}

u8g_uint_t u8g_font_calc_vref_top(u8g_t *u8g)
{
  u8g_uint_t tmp;
  /* reference pos is one pixel above the upper edge of the reference glyph */

  /*
  y += (u8g_uint_t)(u8g_int_t)(u8g->font_ref_ascent);
  y++;
  */
  tmp = (u8g_uint_t)(u8g_int_t)(u8g->font_ref_ascent);
  tmp++;
  return tmp;
}

void u8g_SetFontPosTop(u8g_t *u8g)
{
  u8g->font_calc_vref = u8g_font_calc_vref_top;
}

u8g_uint_t u8g_font_calc_vref_center(u8g_t *u8g)
{
  int8_t tmp;
  tmp = u8g->font_ref_ascent;
  tmp -= u8g->font_ref_descent;
  tmp /= 2;
  tmp += u8g->font_ref_descent;  
  /* y += (u8g_uint_t)(u8g_int_t)(tmp); */
  return tmp;
}

void u8g_SetFontPosCenter(u8g_t *u8g)
{
  u8g->font_calc_vref = u8g_font_calc_vref_center;
}

/*========================================================================*/
/* string pixel width calculation */

char u8g_font_get_char(const void *s)
{
  return *(const char *)(s);
}

char u8g_font_get_charP(const void *s)
{
  return u8g_pgm_read(s);
}

typedef char (*u8g_font_get_char_fn)(const void *s);


u8g_uint_t u8g_font_calc_str_pixel_width(u8g_t *u8g, const char *s, u8g_font_get_char_fn get_char )
{
  u8g_uint_t  w;
  uint8_t enc;
  
  /* reset the total minimal width to zero, this will be expanded during calculation */
  w = 0;
    
  enc = get_char(s);
  
  /* check for empty string, width is already 0 */
  if ( enc == '\0' )
  {
    return w;
  }
  
  /* get the glyph information of the first char. This must be valid, because we already checked for the empty string */
  /* if *s is not inside the font, then the cached parameters of the glyph are all zero */
  u8g_GetGlyph(u8g, enc);

  /* strlen(s) == 1:       width = width(s[0]) */
  /* strlen(s) == 2:       width = - offx(s[0]) + deltax(s[0]) + offx(s[1]) + width(s[1]) */
  /* strlen(s) == 3:       width = - offx(s[0]) + deltax(s[0]) + deltax(s[1]) + offx(s[2]) + width(s[2]) */
  
  /* assume that the string has size 2 or more, than start with negative offset-x */
  /* for string with size 1, this will be nullified after the loop */
  w = -u8g->glyph_x;  
  for(;;)
  {
    
    /* check and stop if the end of the string is reached */
    s++;
    if ( get_char(s) == '\0' )
      break;
    
    /* if there are still more characters, add the delta to the next glyph */
    w += u8g->glyph_dx;
    
    /* store the encoding in a local variable, used also after the for(;;) loop */
    enc = get_char(s);
    
    /* load the next glyph information */
    u8g_GetGlyph(u8g, enc);
  }
  
  /* finally calculate the width of the last char */
  /* here is another exception, if the last char is a black, use the dx value instead */
  if ( enc != ' ' )
  {
    /* if g was not updated in the for loop (strlen() == 1), then the initial offset x gets removed */
    w += u8g->glyph_width;
    w += u8g->glyph_x;
  }
  else
  {
    w += u8g->glyph_dx;
  }
  
  
  return w;
}

u8g_uint_t u8g_GetStrPixelWidth(u8g_t *u8g, const char *s)
{
  return u8g_font_calc_str_pixel_width(u8g, s, u8g_font_get_char);
}

u8g_uint_t u8g_GetStrPixelWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s)
{
  return u8g_font_calc_str_pixel_width(u8g, (const char *)s, u8g_font_get_charP);
}

int8_t u8g_GetStrX(u8g_t *u8g, const char *s)
{
  u8g_GetGlyph(u8g, *s);
  return u8g->glyph_x;  
}

int8_t u8g_GetStrXP(u8g_t *u8g, const u8g_pgm_uint8_t *s)
{
  u8g_GetGlyph(u8g, u8g_pgm_read(s));
  return u8g->glyph_x;  
}

/*========================================================================*/
/* string width calculation */

u8g_uint_t u8g_GetStrWidth(u8g_t *u8g, const char *s)
{
  u8g_uint_t  w;
  uint8_t encoding;
  
  /* reset the total width to zero, this will be expanded during calculation */
  w = 0;
  
  for(;;)
  {
    encoding = *s;
    if ( encoding == 0 )
      break;

    /* load glyph information */
    u8g_GetGlyph(u8g, encoding);
    w += u8g->glyph_dx;    
    
    /* goto next char */
    s++;
  }
  
  return w;  
}


u8g_uint_t u8g_GetStrWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s)
{
  u8g_uint_t  w;
  uint8_t encoding;
  
  /* reset the total width to zero, this will be expanded during calculation */
  w = 0;
  
  for(;;)
  {
    encoding = u8g_pgm_read(s);
    if ( encoding == 0 )
      break;

    /* load glyph information */
    u8g_GetGlyph(u8g, encoding);
    w += u8g->glyph_dx;    
    
    /* goto next char */
    s++;
  }
  
  return w;  
}


/*========================================================================*/
/* calculation of font/glyph/string characteristics */


/*
  Description:
    Calculate parameter for the minimal bounding box on a given string
  Output
    buf->y_min          extend of the lower left edge if the string below (y_min<0) or above (y_min>0) baseline (descent)
    buf->y_max          extend of the upper left edge if the string below (y_min<0) or above (y_min>0) baseline (ascent)
    buf->w                 the width of the string
*/
struct u8g_str_size_struct
{
  int8_t y_min;         /* descent */
  int8_t y_max;         /* ascent */
  int8_t x, y;             /* the reference point of the font (negated!) */
  u8g_uint_t w;         /* width of the overall string */
};
typedef struct u8g_str_size_struct u8g_str_size_t;

static void u8g_font_calc_str_min_box(u8g_t *u8g, const char *s, u8g_str_size_t *buf)
{
  /* u8g_glyph_t g; */
  int8_t tmp;
  
  /* reset the total minimal width to zero, this will be expanded during calculation */
  buf->w = 0;
    
  /* check for empty string, width is already 0, but also reset y_min and y_max to 0 */
  if ( *s == '\0' )
  {
    buf->y_min = 0;
    buf->y_max = 0;
    buf->x = 0;
    buf->y = 0;
    return;
  }
  
  /* reset y_min to the largest possible value. Later we search for the smallest value */
  /* y_min contains the position [pixel] of the lower left edge of the glyph above (y_min>0) or below (y_min<0) baseline  */
  buf->y_min = 127;
  /* reset y_max to the smallest possible value. Later we search for the highest value */
  /* y_max contains the position [pixel] of the upper left edge of the glyph above (y_max>0) or below (y_max<0) baseline  */
  buf->y_max = -128;

  /* get the glyph information of the first char. This must be valid, because we already checked for the empty string */
  u8g_GetGlyph(u8g, *s);

  /* strlen(s) == 1:       width = width(s[0]) */
  /* strlen(s) == 2:       width = - offx(s[0]) + deltax(s[0]) + offx(s[1]) + width(s[1]) */
  /* strlen(s) == 3:       width = - offx(s[0]) + deltax(s[0]) + deltax(s[1]) + offx(s[2]) + width(s[2]) */
  
  /* assume that the string has size 2 or more, than start with negative offset-x */
  /* for string with size 1, this will be nullified after the loop */
  // buf->w = - u8g_font_GetGlyphBBXOffX(u8g->font, g);
  buf->w = - u8g->glyph_x;
  
  /* Also copy the position of the first glyph. This is the reference point of the string (negated) */
  buf->x = u8g->glyph_x;
  buf->y = u8g->glyph_y;
  
  for(;;)
  {
    
    /* calculated y position of the upper left corner (y_max) and lower left corner (y_min) of the string */
    /* relative to the base line */
    
    tmp = u8g->glyph_y;
    if ( buf->y_min > tmp )
      buf->y_min = tmp;
    
    tmp +=u8g->glyph_height;
    if ( buf->y_max < tmp )
      buf->y_max = tmp;
    
    /* check and stop if the end of the string is reached */
    s++;
    if ( *s == '\0' )
      break;
    
    /* if there are still more characters, add the delta to the next glyph */
    buf->w += u8g->glyph_dx;
    
    /* load the next glyph information */
    u8g_GetGlyph(u8g, *s);
  }
  
  /* finally calculate the width of the last char */
  /* if g was not updated in the for loop (strlen() == 1), then the initial offset x gets removed */
  buf->w += u8g->glyph_width;
  // buf->w += u8g_font_GetGlyphBBXOffX(u8g->font, g);
  
  buf->w += u8g->glyph_x;
}

/* calculate minimal box */
void u8g_font_box_min(u8g_t *u8g, const char *s, u8g_str_size_t *buf)
{
  u8g_font_calc_str_min_box(u8g, s, buf);
}

/* calculate gA box, but do not calculate the overall width */
void u8g_font_box_left_gA(u8g_t *u8g, const char *s, u8g_str_size_t *buf)
{
  
}

/* calculate gA box, including overall width */
void u8g_font_box_all_gA(u8g_t *u8g, const char *s, u8g_str_size_t *buf)
{
  
}


static void u8g_font_get_str_box_fill_args(u8g_t *u8g, const char *s, u8g_str_size_t *buf, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height)
{
  /*
  u8g_glyph_t g;
  g =
  */  
  u8g_GetGlyph(u8g, *s);
  *x += u8g->glyph_x;
  *width = buf->w;
  *y -= buf->y_max;
  /* +1 because y_max is a height, this compensates the next step */
  //*y += 1;                      
  /* because the reference point is one below the string, this compensates the previous step */
  //*y -= 1;
  *height = buf->y_max;
  *height -= buf->y_min;
}


void u8g_GetStrMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height)
{
  u8g_str_size_t buf;
  
  if ( *s == '\0' )
  {
    *width= 0;
    *height = 0;
    return;
  }
  
  u8g_font_calc_str_min_box(u8g, s, &buf);
  u8g_font_get_str_box_fill_args(u8g, s, &buf, x, y, width, height);
}


void u8g_GetStrAMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height)
{
  u8g_str_size_t buf;
  uint8_t cap_a;
  
  if ( *s == '\0' )
  {
    *width= 0;
    *height = 0;
    return;
  }
  
  cap_a = u8g_font_GetCapitalAHeight(u8g->font);
  u8g_font_calc_str_min_box(u8g, s, &buf);
  if ( buf.y_max < cap_a )
    buf.y_max = cap_a;
  u8g_font_get_str_box_fill_args(u8g, s, &buf, x, y, width, height);
}

void u8g_SetFont(u8g_t *u8g, const u8g_fntpgm_uint8_t  *font)
{
  if ( u8g->font != font )
  {
    u8g->font = font;
    u8g_UpdateRefHeight(u8g);
    u8g_SetFontPosBaseline(u8g);
  }
}

/*========================================================================*/
/* anti aliasing fonts */

int8_t u8g_draw_aa_glyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  const u8g_pgm_uint8_t *data;
  uint8_t w, h;
  uint8_t i, j;
  u8g_uint_t ix, iy;

  {
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  }
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x += u8g->glyph_x;
  y -= u8g->glyph_y;
  y--;
  
  if ( u8g_IsBBXIntersection(u8g, x, y-h+1, w, h) == 0 )
    return u8g->glyph_dx;

  /* now, w is reused as bytes per line */
  w += 3;
  w /= 4;
  
  iy = y;
  iy -= h;
  iy++;

  for( j = 0; j < h; j++ )
  {
    ix = x;
    for( i = 0; i < w; i++ )
    {
      u8g_Draw4TPixel(u8g, ix, iy, 0, u8g_pgm_read(data));
      data++;
      ix+=4;
    }
    iy++;
  }
  return u8g->glyph_dx;
}

int8_t u8g_DrawAAGlyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding)
{
  y += u8g->font_calc_vref(u8g);
  return u8g_draw_aa_glyph(u8g, x, y, encoding);
}

u8g_uint_t u8g_DrawAAStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s)
{
  u8g_uint_t t = 0;
  int8_t d;

  if ( u8g_font_GetFormat(u8g->font)  != 2 )
    return 0;
  //u8g_uint_t u8g_GetStrWidth(u8g, s);
  //u8g_font_GetFontAscent(u8g->font)-u8g_font_GetFontDescent(u8g->font);
  
  y += u8g->font_calc_vref(u8g);
  
  while( *s != '\0' )
  {
    d = u8g_draw_aa_glyph(u8g, x, y, *s);
    x += d;
    t += d;
    s++;
  }
  return t;
}
