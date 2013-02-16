/*

  dogmfont.c

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.


*/

#include "dogm128.h"

/*==============================================================*/
/* font functions */

/*
  FONT
  offset	size
  0		word	capital A index, low byte first
  2		word	small a index, low byte first
  4		byte		bbox width
  5		byte		bbox height
  6		byte		bbox descent
  7-		variable (characters with code 0x20 - 0x7f)
  
  
  CHARACTER (Format 1)
  offset	size
  0		byte			total size of the structure, including this byte
  1		bits 0..4		width
  1		bits 5..7		descent
  2		byte			height
  3-		varable (bitmap, topmost line first)
  
  +====+  ====							====
  |		|								upper space (ascent area)
  |		|						====	====
  |		|
  |		|  bbox					height
  |		|  height
  |=====|		====	====							====baseline===
  |		|		bbox d	descent
  |		|				====	====	====
  |		|								lower space (descent area)
  +====+  ====	====					====		
  
  upper space + lower space = bbox height - height
  lower space = bbox descent - descent
  upper space = bbox height + descent - height - bbox descent 

*/



#define chr_get_len_P(buf) ((unsigned char)(dog_pgm_read((buf)+0)))
#define chr_get_descent_P(buf) (((unsigned char)(dog_pgm_read((buf)+1)))  >>5)
#define chr_get_width_P(buf) ((unsigned char)(dog_pgm_read((buf)+1) & (unsigned char)0x1f))
#define chr_get_height_P(buf) ((unsigned char)(dog_pgm_read((buf)+2)))
#define CHR_DATA 3

uint16_t dog_font_pgm_read_word(DOG_PGM_P buf)
{
  uint16_t result;
  result = dog_pgm_read(buf+1);
  result <<=8;
  result |= dog_pgm_read(buf);
  return result;
}

uint8_t dog_GetFontBBXHeight(DOG_PGM_P buf)
{
  return fnt_get_bbox_height(buf);
}

uint8_t dog_GetFontBBXWidth(DOG_PGM_P buf)
{
  return fnt_get_bbox_width(buf);
}

uint8_t dog_GetFontBBXDescent(DOG_PGM_P buf)
{
  return fnt_get_bbox_descent(buf);
}

static unsigned char chr_get_pixel(DOG_PGM_P buf, unsigned char x, unsigned char y )
{
  unsigned char bytes_per_line, index;
  /* bytes_per_line = ((unsigned char)chr_get_width(buf)+(unsigned char)7) >> 3; */
  bytes_per_line = chr_get_width_P(buf);
  bytes_per_line += (unsigned char)7;
  bytes_per_line >>= (unsigned char)3;
  index = y;
  index *= bytes_per_line;
  index += (unsigned char)(x>>3);
  index += CHR_DATA;
    
  if ( ((unsigned char)dog_pgm_read(buf+index)) & ( (unsigned char)1 << (x&(unsigned char)7)) ) 
    return 1;
  return 0;
}


static unsigned short chr_get_skip_delta(DOG_PGM_P cbuf, unsigned char skip_cnt)
{
  unsigned short pos = 0;
  unsigned char len;
  while( skip_cnt > 0 )
  {
    len = chr_get_len_P(cbuf + pos);
    if ( len == 0 )				
      return pos;			/* should never happen */
    pos += (short)len;
    skip_cnt--;
  }
  return pos;
}

DOG_PGM_P fnt_get_chr(DOG_PGM_P  fbuf, unsigned char code)
{
  DOG_PGM_P adr;
  
  if ( code >= 'a' )
  {
    adr = fbuf + fnt_get_bbox_small_a(fbuf);
    adr = adr + chr_get_skip_delta(adr, code - 'a');
  }
  else if ( code >= 'A' )
  {
    adr = fbuf + fnt_get_bbox_capital_a(fbuf);
    adr = adr + chr_get_skip_delta(adr, code - 'A');
  }
  else
  {
    if ( code < ' ' )
      code = ' ';
    adr = fbuf + FNT_DATA + chr_get_skip_delta(fbuf+FNT_DATA, code - ' ');
  }
  return adr;
}

uint8_t dog_GetCharWidth(DOG_PGM_P font, unsigned char code)
{
  DOG_PGM_P adr;
  /* adr = font + FNT_DATA + chr_get_skip_delta(font+FNT_DATA, code - ' '); */
  adr = fnt_get_chr(font, code);
  return chr_get_width_P(adr);
}

uint8_t dog_char(uint8_t x, uint8_t y, DOG_PGM_P font, uint8_t mode, uint8_t rot, unsigned char code)
{
  DOG_PGM_P cbuf = fnt_get_chr(font, code);
  unsigned char ascent_area, descent_area;
  unsigned char  i, j;
  uint8_t tmp_y, r_y;
  uint8_t tmp_x, r_x;
  uint8_t height,width;

  if ( chr_get_len_P(cbuf) == 0 )
    return 0;
  height = chr_get_height_P(cbuf);
  width= chr_get_width_P(cbuf);
  
  descent_area = fnt_get_bbox_descent(font);
  descent_area  -= chr_get_descent_P(cbuf);
  ascent_area = fnt_get_bbox_height(font);
  ascent_area -= height; 
  ascent_area -= descent_area;

  /*
  printf("code %c, bbox height %d,  aa %d, da %d, char descent %d\n", code, fnt_get_bbox_height(font), ascent_area, descent_area, chr_get_descent(cbuf) );
  */
  
  /*
    ascent_area starts at 		y + chr_get_height(cbuf) - chr_get_descent(cbuf) + ascent_area
    char code starts at		y + chr_get_height(cbuf) - chr_get_descent(cbuf)
    descent area starts at		y - chr_get_descent(cbuf)
    descent area ends at		y - chr_get_descent(cbuf) - descent_area + 1
  */
  
  
  /*tmp_y = y + chr_get_height(cbuf) - chr_get_descent(cbuf) ;*/
  /*tmp_y = y; will be added later*/
  tmp_y = height;
  tmp_y -= chr_get_descent_P(cbuf);

  /* char goes from tmp_y to tmp_y-height-1 and x .. x + width -1*/
  /* y test is done in dog_str */
  /*
  if ( dog_max_y < tmp_y - height + 1 )
    return width;
  if ( dog_min_y > tmp_y )
    return width;
 */

  for( i = height; i > 0; i-- )
  {
    tmp_x = 0;
    for( j = 0; j < width; j++ )
    {
      if ( chr_get_pixel(cbuf, j, i-1 ) > 0 )
      {
	switch( rot )
	{
	  default:
	    r_x = tmp_x;
	    r_y = tmp_y;
	    break;
	  case 1:
	    r_x = -tmp_y;
	    r_y = tmp_x;
	    break;
	  case 2:
	    r_x = -tmp_x;
	    r_y = -tmp_y;
	    break;
	  case 3:
	    r_x = tmp_y;
	    r_y = -tmp_x;
	    break;
	}
	
	r_x += x;
	r_y += y;
	dog_SetPixel(r_x, r_y);
      }
      /*
      else
	dog_ClrPixel(tmp_x, tmp_y);
      */
      tmp_x++;
    }
    tmp_y--;
  }
    
  return width;
}

uint8_t dog_DrawRChar(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, unsigned char code)
{
  signed char y0, y1;
  if ( rot == 0 )
  {
    y0 = y;
    y0 -= fnt_get_bbox_descent(font);  
    y1 = y0;
    y1 += fnt_get_bbox_height(font);  
  
    if ( (signed char)dog_max_y < y0 )
      return dog_GetCharWidth(font, code);
    if ( (signed char)dog_min_y > y1 )
      return dog_GetCharWidth(font, code);
  }
  return dog_char(x,y,font,0,rot,code);
}

uint8_t dog_DrawChar(uint8_t x, uint8_t y, DOG_PGM_P font, unsigned char code)
{
  return dog_DrawRChar(x, y, 0, font, code);
}



