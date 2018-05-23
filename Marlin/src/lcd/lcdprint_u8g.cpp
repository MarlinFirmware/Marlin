/**
 * @file    lcdprint_u8g.c
 * @brief   LCD print api for u8glib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(DOGLCD)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#include <U8glib.h>
#pragma GCC diagnostic pop
extern U8GLIB *pu8g;
#define _lcd_write(a) pu8g->print(a)
#define _lcd_setcursor(col, row) pu8g->setPrintPos((col), (row));

#include "ultralcd.h"
#include "../Marlin.h"

#include "fontutils.h"
#include "u8g_fontutf8.h"
#include "lcdprint.h"

int lcd_glyph_height(void) {
  return u8g_GetFontBBXHeight(pu8g->getU8g());
  //return u8g_GetFontBBXOffY(pu8g->getU8g());
}

void lcd_moveto(int col, int row) {
  TRACE("Move to: (%d,%d)", col, row);
  _lcd_setcursor(col, row);
}

int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length) {
  if (c < 256) {
    TRACE("draw char: regular %d", (int)c);
    _lcd_write((char)c);
    return u8g_GetFontBBXWidth(pu8g->getU8g());
  }
  unsigned int x = pu8g->getPrintCol(),
               y = pu8g->getPrintRow(),
               ret = uxg_DrawWchar(pu8g->getU8g(), x, y, c, max_length);
  TRACE("uxg_DrawWchar(x=%d,y=%d,maxlen=%d", x, y, max_length);
  TRACE("u8g->setPrintPos(x=%d + ret=%d,y=%d", x, ret, y);
  pu8g->setPrintPos(x + ret, y);

  return ret;
}

int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length) {
  unsigned int x = pu8g->getPrintCol(),
               y = pu8g->getPrintRow(),
               ret = uxg_DrawUtf8Str(pu8g->getU8g(), x, y, utf8_str, max_length);
  TRACE("uxg_DrawUtf8Str(x=%d,y=%d,maxlen=%d", x, y, max_length);
  TRACE("u8g->setPrintPos(x=%d + ret=%d,y=%d", x, ret, y);
  pu8g->setPrintPos(x + ret, y);
  return ret;
}

int lcd_put_u8str_max_rom(const char * utf8_str_P, pixel_len_t max_length) {
  unsigned int x = pu8g->getPrintCol(),
               y = pu8g->getPrintRow(),
               ret = uxg_DrawUtf8StrP(pu8g->getU8g(), x, y, utf8_str_P, max_length);
  TRACE("uxg_DrawUtf8StrP(x=%d,y=%d,maxlen=%d", x, y, max_length);
  TRACE("u8g->setPrintPos(x=%d + ret=%d,y=%d", x, ret, y);
  pu8g->setPrintPos(x + ret, y);
  return ret;
}

#else // !DOGLCD

  #define _lcd_write(a) TRACE("Write LCD: %c (%d)", (a), (int)(a));
  #define _lcd_setcursor(col, row) TRACE("Set cursor LCD: (%d,%d)", (col), (row));

#endif // !DOGLCD
