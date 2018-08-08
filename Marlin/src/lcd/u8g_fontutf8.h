/**
 * @file    fontutf8.h
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL/BSD
 */
#ifndef _UXG_FONTUTF8_H
#define _UXG_FONTUTF8_H 1

#include <U8glib.h>
#include "fontutils.h"

// the macro to indicate a UTF-8 string
// You should to save the C/C++ source in UTF-8 encoding!
// Once you change your UTF-8 strings, you need to call the script uxggenpages.sh to create the font data file fontutf8-data.h
#define _UxGT(a) a

typedef struct _uxg_fontinfo_t {
  uint16_t page;
  uint8_t begin;
  uint8_t end;
  uint16_t size;
  const u8g_fntpgm_uint8_t *fntdata;
} uxg_fontinfo_t;

int uxg_SetUtf8Fonts (const uxg_fontinfo_t * fntinfo, int number); // fntinfo is type of PROGMEM

unsigned int uxg_DrawWchar (u8g_t *pu8g, unsigned int x, unsigned int y, wchar_t ch, pixel_len_t max_length);

unsigned int uxg_DrawUtf8Str (u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg, pixel_len_t max_length);
unsigned int uxg_DrawUtf8StrP (u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg, pixel_len_t max_length);

int uxg_GetUtf8StrPixelWidth(u8g_t *pu8g, const char *utf8_msg);
int uxg_GetUtf8StrPixelWidthP(u8g_t *pu8g, const char *utf8_msg);

#define uxg_GetFont(puxg) ((puxg)->font)

#endif // _UXG_FONTUTF8_H
