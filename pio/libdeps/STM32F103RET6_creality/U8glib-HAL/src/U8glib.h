/*

  U8glib.h

  C++ Interface

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
#pragma once

#ifdef U8G_HAL_LINKS
  #include "U8glibPrint.h"
  #define U8GLIB_PRINT_CLASS U8glibPrint
#else
  #include <Print.h>
  #define U8GLIB_PRINT_CLASS Print
#endif

#include "clib/u8g.h"

class U8GLIB : public U8GLIB_PRINT_CLASS {
  private:
    u8g_t u8g;
    u8g_uint_t tx, ty;          // current position for the Print base class procedures
    uint8_t is_begin;

    void prepare(void) { tx = 0; ty = 0; is_begin = 0; }
    void cbegin(void) { if ( is_begin == 0 ) { is_begin = 1; u8g_Begin(&u8g); } }
    uint8_t initSPI(u8g_dev_t *dev, uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE);
    uint8_t initHWSPI(u8g_dev_t *dev, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE);
    uint8_t initI2C(u8g_dev_t *dev, uint8_t options);
  protected:
    uint8_t init8BitFixedPort(u8g_dev_t *dev, uint8_t en, uint8_t cs, uint8_t di, uint8_t rw, uint8_t reset);
  private:
    uint8_t init8Bit(u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE);
    uint8_t initRW8Bit(u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset);
  public:

    /* constructor */
    U8GLIB(void)
      { }
    U8GLIB(u8g_dev_t *dev)
      { prepare(); u8g_Init(&u8g, dev); }
    U8GLIB(u8g_dev_t *dev, u8g_com_fnptr com_fn)
      { prepare(); u8g_InitComFn(&u8g, dev, com_fn); }
    U8GLIB(u8g_dev_t *dev, uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset)
      { initSPI(dev, sck, mosi, cs, a0, reset); }
    U8GLIB(u8g_dev_t *dev, uint8_t cs, uint8_t a0, uint8_t reset)
      { initHWSPI(dev, cs, a0, reset); }
    U8GLIB(u8g_dev_t *dev, uint8_t options)
      { initI2C(dev, options); }
    U8GLIB(u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw, uint8_t reset)
      { init8Bit(dev, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset); }
    U8GLIB(u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset)
      { initRW8Bit(dev, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset); }

    uint8_t begin(void) { is_begin = 1; return u8g_Begin(&u8g); }

    void setPrintPos(u8g_uint_t x, u8g_uint_t y) { tx = x; ty = y; }
    u8g_t *getU8g(void) { return &u8g; }
    u8g_uint_t getPrintCol(void) { return tx; }
    u8g_uint_t getPrintRow(void) { return ty; }


    /* implementation of the write interface to the print class */
#if defined(ARDUINO) && ARDUINO < 100
    void write(uint8_t c) { tx += u8g_DrawGlyph(&u8g, tx, ty, c); }
#else
    size_t write(uint8_t c) { tx += u8g_DrawGlyph(&u8g, tx, ty, c); return 1;}
#endif

     /* screen rotation */
    void undoRotation(void) { u8g_UndoRotation(&u8g); }
    void setRot90(void) { u8g_SetRot90(&u8g); }
    void setRot180(void) { u8g_SetRot180(&u8g); }
    void setRot270(void) { u8g_SetRot270(&u8g); }

     /* screen scaling */
    void undoScale(void) { u8g_UndoScale(&u8g); }
    void setScale2x2(void) { u8g_SetScale2x2(&u8g); }

     /* picture loop */
    void firstPage(void) { cbegin(); u8g_FirstPage(&u8g); }
    uint8_t nextPage(void) { return u8g_NextPage(&u8g); }

    /* system commands */
    uint8_t setContrast(uint8_t contrast) { cbegin(); return u8g_SetContrast(&u8g, contrast); }
    void sleepOn(void) { u8g_SleepOn(&u8g); }
    void sleepOff(void) { u8g_SleepOff(&u8g); }

    /* graphic primitives */
    void setColorEntry(uint8_t color_index, uint8_t r, uint8_t g, uint8_t b) { u8g_SetColorEntry(&u8g, color_index, r, g, b); }
    void setHiColor(uint16_t  rgb) { u8g_SetHiColor(&u8g, rgb); }
    void setHiColorByRGB(uint8_t r, uint8_t g, uint8_t b) { u8g_SetHiColorByRGB(&u8g, r, g, b); }
    void setRGB(uint8_t r, uint8_t g, uint8_t b) { u8g_SetRGB(&u8g, r, g, b); }

    void setColorIndex(uint8_t color_index) { u8g_SetColorIndex(&u8g, color_index); }
    uint8_t getColorIndex(void) { return u8g_GetColorIndex(&u8g); }

    void setDefaultForegroundColor(void) { u8g_SetDefaultForegroundColor(&u8g);  }
    void setDefaultBackgroundColor(void) { u8g_SetDefaultBackgroundColor(&u8g);  }
    void setDefaultMidColor(void) { u8g_SetDefaultMidColor(&u8g);  }

    u8g_uint_t getWidth(void) { return u8g_GetWidth(&u8g); }
    u8g_uint_t getHeight(void) { return u8g_GetHeight(&u8g); }
    uint8_t getMode(void) { return u8g_GetMode(&u8g); }

    void drawPixel(u8g_uint_t x, u8g_uint_t y) { return u8g_DrawPixel(&u8g, x, y); }
    void drawHLine(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w) { u8g_DrawHLine(&u8g, x, y, w); }
    void drawVLine(u8g_uint_t x, u8g_uint_t y, u8g_uint_t h) { u8g_DrawVLine(&u8g, x, y, h); }
    void drawLine(u8g_uint_t x1, u8g_uint_t y1, u8g_uint_t x2, u8g_uint_t y2) { u8g_DrawLine(&u8g, x1, y1, x2, y2); }

    void drawFrame(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) { u8g_DrawFrame(&u8g, x, y, w, h); }
    void drawRFrame(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) { u8g_DrawRFrame(&u8g, x, y, w, h,r); }
    void drawBox(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) { u8g_DrawBox(&u8g, x, y, w, h); }
    void drawRBox(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) { u8g_DrawRBox(&u8g, x, y, w, h,r); }

    void drawCircle(u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t opt = U8G_DRAW_ALL) { u8g_DrawCircle(&u8g, x0, y0, rad, opt); }
    void drawDisc(u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t opt = U8G_DRAW_ALL) { u8g_DrawDisc(&u8g, x0, y0, rad, opt); }

    void drawEllipse(u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t opt = U8G_DRAW_ALL) { u8g_DrawEllipse(&u8g, x0, y0, rx, ry, opt); }
    void drawFilledEllipse(u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t opt = U8G_DRAW_ALL) { u8g_DrawFilledEllipse(&u8g, x0, y0, rx, ry, opt); }

    void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
      { u8g_DrawTriangle(&u8g, x0, y0, x1, y1, x2, y2); }



    /* bitmap handling */
    void drawBitmap(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap)
      { u8g_DrawBitmap(&u8g, x, y, cnt, h, bitmap); }
    void drawBitmapP(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap)
      { u8g_DrawBitmapP(&u8g, x, y, cnt, h, bitmap); }

    void drawXBM(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const uint8_t *bitmap)
      { u8g_DrawXBM(&u8g, x, y, w, h, bitmap); }
    void drawXBMP(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap)
      { u8g_DrawXBMP(&u8g, x, y, w, h, bitmap); }


    /* font handling */
    void setFont(const u8g_fntpgm_uint8_t *font) {u8g_SetFont(&u8g, font); }
    int8_t getFontAscent(void) { return u8g_GetFontAscent(&u8g); }
    int8_t getFontDescent(void) { return u8g_GetFontDescent(&u8g); }
    int8_t getFontLineSpacing(void) { return u8g_GetFontLineSpacing(&u8g); }

    u8g_uint_t drawStr(u8g_uint_t x, u8g_uint_t y, const char *s) { return u8g_DrawStr(&u8g, x, y, s); }
    u8g_uint_t drawStr90(u8g_uint_t x, u8g_uint_t y, const char *s) { return u8g_DrawStr90(&u8g, x, y, s); }
    u8g_uint_t drawStr180(u8g_uint_t x, u8g_uint_t y, const char *s) { return u8g_DrawStr180(&u8g, x, y, s); }
    u8g_uint_t drawStr270(u8g_uint_t x, u8g_uint_t y, const char *s) { return u8g_DrawStr270(&u8g, x, y, s); }
    u8g_uint_t drawStrP(u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s) { return u8g_DrawStrP(&u8g, x, y, s); }
    u8g_uint_t drawStr90P(u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s) { return u8g_DrawStr90P(&u8g, x, y, s); }
    u8g_uint_t drawStr180P(u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s) { return u8g_DrawStr180P(&u8g, x, y, s); }
    u8g_uint_t drawStr270P(u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s) { return u8g_DrawStr270P(&u8g, x, y, s); }

    void setFontPosBaseline(void) { u8g_SetFontPosBaseline(&u8g); }
    void setFontPosBottom(void) { u8g_SetFontPosBottom(&u8g); }
    void setFontPosCenter(void) { u8g_SetFontPosCenter(&u8g); }
    void setFontPosTop(void) { u8g_SetFontPosTop(&u8g); }

    void setFontRefHeightText(void) { u8g_SetFontRefHeightText(&u8g); }
    void setFontRefHeightExtendedText(void) { u8g_SetFontRefHeightExtendedText(&u8g); }
    void setFontRefHeightAll(void) { u8g_SetFontRefHeightAll(&u8g); }
    void setFontLineSpacingFactor(uint8_t factor) { u8g_SetFontLineSpacingFactor(&u8g, factor); }


    u8g_uint_t getStrPixelWidth(const char *s) { return u8g_GetStrPixelWidth(&u8g, s); }
    u8g_uint_t getStrPixelWidthP(u8g_pgm_uint8_t *s) { return u8g_GetStrPixelWidthP(&u8g, s); }
    u8g_uint_t getStrWidth(const char *s) { return u8g_GetStrWidth(&u8g, s); }
    u8g_uint_t getStrWidthP(u8g_pgm_uint8_t *s) { return u8g_GetStrWidthP(&u8g, s); }
    int8_t getGlyphDeltaX(uint8_t requested_encoding) { return u8g_GetGlyphDeltaX(&u8g, requested_encoding); }

    void setHardwareBackup(u8g_state_cb backup_cb) { u8g_SetHardwareBackup(&u8g, backup_cb); }

#if defined(ARDUINO) && ARDUINO >= 100 && !defined(U8G_HAL_LINKS)
      //  generates false compiler errors when U8G_HAL_LINKS is enabled
    // support for the F() macro

    u8g_uint_t drawStr(u8g_uint_t x, u8g_uint_t y, const __FlashStringHelper *s) { return u8g_DrawStrP(&u8g, x, y, (u8g_pgm_uint8_t *)s); }
    u8g_uint_t drawStr90(u8g_uint_t x, u8g_uint_t y, const __FlashStringHelper *s) { return u8g_DrawStr90P(&u8g, x, y, (u8g_pgm_uint8_t *)s); }
    u8g_uint_t drawStr180(u8g_uint_t x, u8g_uint_t y, const __FlashStringHelper *s) { return u8g_DrawStr180P(&u8g, x, y, (u8g_pgm_uint8_t *)s); }
    u8g_uint_t drawStr270(u8g_uint_t x, u8g_uint_t y, const __FlashStringHelper *s) { return u8g_DrawStr270P(&u8g, x, y, (u8g_pgm_uint8_t *)s); }

    u8g_uint_t getStrPixelWidth(const __FlashStringHelper *s) { return u8g_GetStrPixelWidthP(&u8g, (u8g_pgm_uint8_t *)s); }
    u8g_uint_t getStrWidth(const __FlashStringHelper *s) { return u8g_GetStrWidthP(&u8g, (u8g_pgm_uint8_t *)s); }
#endif
    /* cursor handling */
    void setCursorFont(const u8g_pgm_uint8_t *cursor_font) { u8g_SetCursorFont(&u8g, cursor_font); }
    void setCursorStyle(uint8_t encoding) { u8g_SetCursorStyle(&u8g, encoding); }
    void setCursorPos(u8g_uint_t cursor_x, u8g_uint_t cursor_y) { u8g_SetCursorPos(&u8g, cursor_x, cursor_y); }
    void setCursorColor(uint8_t fg, uint8_t bg) { u8g_SetCursorColor(&u8g, fg, bg); }
    void enableCursor(void) { u8g_EnableCursor(&u8g); }
    void disableCursor(void) { u8g_DisableCursor(&u8g); }
    void drawCursor(void)  { u8g_DrawCursor(&u8g); }

    /* virtual screen */

    void setVirtualScreenDimension(u8g_uint_t width, u8g_uint_t height) { u8g_SetVirtualScreenDimension(&u8g, width, height); }
    uint8_t addToVirtualScreen(u8g_uint_t x, u8g_uint_t y, U8GLIB &child_u8g) { return u8g_AddToVirtualScreen(&u8g, x, y, &child_u8g.u8g); }

};


class U8GLIB_DOGS102 : public U8GLIB
{
  public:
    U8GLIB_DOGS102(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_dogs102_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGS102(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_dogs102_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGS102_2X : public U8GLIB
{
  public:
    U8GLIB_DOGS102_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_dogs102_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGS102_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_dogs102_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_MINI12864 : public U8GLIB
{
  public:
    U8GLIB_MINI12864(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_mini12864_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_MINI12864(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_mini12864_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_MINI12864_2X : public U8GLIB
{
  public:
    U8GLIB_MINI12864_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_mini12864_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_MINI12864_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1701_mini12864_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGM132 : public U8GLIB
{
  public:
    U8GLIB_DOGM132(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm132_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGM132(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm132_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_NHD_C12832 : public U8GLIB
{
  public:
    U8GLIB_NHD_C12832(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12832_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD_C12832(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12832_hw_spi, cs, a0, reset)
      { }
    U8GLIB_NHD_C12832(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12832_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD_C12832_USART : public U8GLIB
{
  public:
    U8GLIB_NHD_C12832_USART(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12832_hw_usart_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGM128 : public U8GLIB
{
  public:
    U8GLIB_DOGM128(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGM128(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_hw_spi, cs, a0, reset)
      { }
    U8GLIB_DOGM128(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_DOGM128_2X : public U8GLIB
{
  public:
    U8GLIB_DOGM128_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGM128_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_DOGM128_2X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_dogm128_2x_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_LM6059 : public U8GLIB
{
  public:
    U8GLIB_LM6059(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6059_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_LM6059(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6059_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_LM6059_2X : public U8GLIB
{
  public:
    U8GLIB_LM6059_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6059_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_LM6059_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6059_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_LM6063 : public U8GLIB
{
  public:
    U8GLIB_LM6063(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6063_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_LM6063(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6063_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_LM6063_2X : public U8GLIB
{
  public:
    U8GLIB_LM6063_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6063_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_LM6063_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_lm6063_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_64128N : public U8GLIB
{
  public:
    U8GLIB_64128N(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_64128N(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_hw_spi, cs, a0, reset)
      { }
    U8GLIB_64128N(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_64128N_2X : public U8GLIB
{
  public:
    U8GLIB_64128N_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_64128N_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_64128N_2X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_2x_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD_C12864 : public U8GLIB
{
  public:
    U8GLIB_NHD_C12864(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12864_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD_C12864(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12864_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_NHD_C12864_2X : public U8GLIB
{
  public:
    U8GLIB_NHD_C12864_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12864_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD_C12864_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_nhd_c12864_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1601_C128032 : public U8GLIB
{
  public:
    U8GLIB_UC1601_C128032(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1601_c128032_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1601_C128032(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1601_c128032_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1601_C128032_2X : public U8GLIB
{
  public:
    U8GLIB_UC1601_C128032_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1601_c128032_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1601_C128032_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1601_c128032_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1608_240X64 : public U8GLIB
{
  public:
    U8GLIB_UC1608_240X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1608_240X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x64_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1608_240X64_2X : public U8GLIB
{
  public:
    U8GLIB_UC1608_240X64_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x64_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1608_240X64_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x64_2x_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1608_240X128 : public U8GLIB
{
  public:
    U8GLIB_UC1608_240X128(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x128_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1608_240X128(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x128_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_UC1608_240X128_2X : public U8GLIB
{
  public:
    U8GLIB_UC1608_240X128_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x128_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1608_240X128_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1608_240x128_2x_hw_spi, cs, a0, reset)
      { }
};


class U8GLIB_UC1611_DOGM240 : public U8GLIB
{
  public:
    U8GLIB_UC1611_DOGM240(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogm240_i2c, options)
      {}
    U8GLIB_UC1611_DOGM240(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogm240_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1611_DOGM240(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogm240_hw_spi, cs, a0, reset)
      { }
    U8GLIB_UC1611_DOGM240(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogm240_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)  // cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_UC1611_DOGXL240 : public U8GLIB
{
  public:
    U8GLIB_UC1611_DOGXL240(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogxl240_i2c, options)
      {}
    U8GLIB_UC1611_DOGXL240(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogxl240_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_UC1611_DOGXL240(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogxl240_hw_spi, cs, a0, reset)
      { }
    U8GLIB_UC1611_DOGXL240(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1611_dogxl240_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)  // cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_128X64 : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_ST7920_128X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_hw_spi, cs, a0, reset)
      { }
    U8GLIB_ST7920_128X64(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
      { }
    // U8GLIB_ST7920_128X64(uint8_t cs)
    //   : U8GLIB(&u8g_dev_st7920_128x64_sw_spi, cs, U8G_PIN_NONE, U8G_PIN_NONE)
    //   { }
};

class U8GLIB_ST7920_128X64_1X : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_1X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_128X64_1X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_hw_spi, cs, U8G_PIN_NONE, reset)      // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_128X64_1X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)  // cs1 = cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_128X64_CUSTOM_1X : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_CUSTOM_1X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_custom, sck, mosi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_128X64_4X : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_4X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_4x_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)    // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_128X64_4X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_4x_hw_spi, cs, U8G_PIN_NONE, reset)     // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_128X64_4X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_4x_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)   // cs1 = cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_128X64_CUSTOM_4X : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_CUSTOM_4X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_4x_custom, sck, mosi, cs, U8G_PIN_NONE, reset)    // a0 = U8G_PIN_NONE
      { }
};


class U8GLIB_ST7920_192X32 : public U8GLIB  // OBSOLETE, use U8GLIB_ST7920_192X32_1X instead
{
  public:
    U8GLIB_ST7920_192X32(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_ST7920_192X32(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_hw_spi, cs, a0, reset)
      { }
    U8GLIB_ST7920_192X32(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
      { }
};

class U8GLIB_ST7920_192X32_1X : public U8GLIB
{
  public:
    U8GLIB_ST7920_192X32_1X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_192X32_1X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_hw_spi, cs, U8G_PIN_NONE, reset)      // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_192X32_1X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)  // cs1 = cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_192X32_4X : public U8GLIB
{
  public:
    U8GLIB_ST7920_192X32_4X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_4x_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)    // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_192X32_4X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_4x_hw_spi, cs, U8G_PIN_NONE, reset)     // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_192X32_4X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_192x32_4x_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)   // cs1 = cs2 = U8G_PIN_NONE
      { }
};


class U8GLIB_ST7920_202X32 : public U8GLIB
{
  public:
    U8GLIB_ST7920_202X32(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_ST7920_202X32(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_hw_spi, cs, a0, reset)
      { }
    U8GLIB_ST7920_202X32(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
      { }
};

class U8GLIB_ST7920_202X32_1X : public U8GLIB
{
  public:
    U8GLIB_ST7920_202X32_1X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_202X32_1X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_hw_spi, cs, U8G_PIN_NONE, reset)      // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_202X32_1X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)  // cs1 = cs2 = U8G_PIN_NONE
      { }
};

class U8GLIB_ST7920_202X32_4X : public U8GLIB
{
  public:
    U8GLIB_ST7920_202X32_4X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_4x_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)    // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_202X32_4X(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_4x_hw_spi, cs, U8G_PIN_NONE, reset)     // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_202X32_4X(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_202x32_4x_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, U8G_PIN_NONE, U8G_PIN_NONE, di, rw, reset)   // cs1 = cs2 = U8G_PIN_NONE
      { }
};


class U8GLIB_LC7981_160X80 : public U8GLIB
{
  public:
    U8GLIB_LC7981_160X80(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_lc7981_160x80_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_LC7981_240X64 : public U8GLIB
{
  public:
    U8GLIB_LC7981_240X64(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_lc7981_240x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_LC7981_240X128 : public U8GLIB
{
  public:
    U8GLIB_LC7981_240X128(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_lc7981_240x128_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};

// 16 bit mode required: Remove comment from "#define U8G_16BIT 1" in utility/u8g.h
class U8GLIB_LC7981_320X64 : public U8GLIB
{
  public:
    U8GLIB_LC7981_320X64(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_lc7981_320x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
};



class U8GLIB_DOGXL160_BW : public U8GLIB
{
  public:
    U8GLIB_DOGXL160_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGXL160_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_bw_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGXL160_GR : public U8GLIB
{
  public:
    U8GLIB_DOGXL160_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGXL160_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_gr_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGXL160_2X_BW : public U8GLIB
{
  public:
    U8GLIB_DOGXL160_2X_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_2x_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGXL160_2X_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_2x_bw_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_DOGXL160_2X_GR : public U8GLIB
{
  public:
    U8GLIB_DOGXL160_2X_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_2x_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_DOGXL160_2X_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_uc1610_dogxl160_2x_gr_hw_spi, cs, a0, reset)
      { }
};


class U8GLIB_NHD27OLED_BW : public U8GLIB
{
  public:
    U8GLIB_NHD27OLED_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_bw_hw_spi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_BW(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_bw_parallel, d0, d1, d2, d3, d4, d5, d6, d7, U8G_PIN_NONE, cs, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD27OLED_2X_BW : public U8GLIB
{
  public:
    U8GLIB_NHD27OLED_2X_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_2x_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_2X_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_2x_bw_hw_spi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_2X_BW(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_2x_bw_parallel, d0, d1, d2, d3, d4, d5, d6, d7, U8G_PIN_NONE, cs, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD31OLED_BW : public U8GLIB
{
  public:
    U8GLIB_NHD31OLED_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_bw_hw_spi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_BW(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_bw_parallel, d0, d1, d2, d3, d4, d5, d6, d7, U8G_PIN_NONE, cs, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD31OLED_2X_BW : public U8GLIB
{
  public:
    U8GLIB_NHD31OLED_2X_BW(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_2x_bw_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_2X_BW(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_2x_bw_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_NHD31OLED_GR : public U8GLIB
{
  public:
    U8GLIB_NHD31OLED_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_gr_hw_spi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_GR(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_gr_parallel, d0, d1, d2, d3, d4, d5, d6, d7, U8G_PIN_NONE, cs, U8G_PIN_NONE, di, rw, reset)
      { }
};

class U8GLIB_NHD31OLED_2X_GR : public U8GLIB
{
  public:
    U8GLIB_NHD31OLED_2X_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_2x_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD31OLED_2X_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1322_nhd31oled_2x_gr_hw_spi, cs, a0, reset)
      { }
};


class U8GLIB_SSD1306_128X64 : public U8GLIB
{
  public:
    U8GLIB_SSD1306_128X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X64(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_i2c, options)
      { }
};

class U8GLIB_SSD1306_ADAFRUIT_128X64 : public U8GLIB
{
  public:
    U8GLIB_SSD1306_ADAFRUIT_128X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_adafruit_128x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_ADAFRUIT_128X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_adafruit_128x64_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_ADAFRUIT_128X64(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_adafruit_128x64_i2c, options)
      { }
};


class U8GLIB_SSD1306_128X64_2X : public U8GLIB
{
  public:
    U8GLIB_SSD1306_128X64_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X64_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X64_2X(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x64_2x_i2c, options)
      { }
};

class U8GLIB_SH1106_128X64 : public U8GLIB
{
  public:
    U8GLIB_SH1106_128X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SH1106_128X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SH1106_128X64(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_i2c, options)
      { }
};

class U8GLIB_SH1106_128X64_2X : public U8GLIB
{
  public:
    U8GLIB_SH1106_128X64_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SH1106_128X64_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SH1106_128X64_2X(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_sh1106_128x64_2x_i2c, options)
      { }
};

class U8GLIB_SSD1309_128X64 : public U8GLIB
{
  public:
    U8GLIB_SSD1309_128X64(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1309_128x64_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1309_128X64(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1309_128x64_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1309_128X64(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1309_128x64_i2c, options)
      { }
};

class U8GLIB_SSD1306_128X32 : public U8GLIB
{
  public:
    U8GLIB_SSD1306_128X32(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X32(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X32(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_i2c, options)
      { }
};

class U8GLIB_SSD1306_128X32_2X : public U8GLIB
{
  public:
    U8GLIB_SSD1306_128X32_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X32_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_128X32_2X(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_128x32_2x_i2c, options)
      { }
};

class U8GLIB_SSD1306_64X48 : public U8GLIB
{
  public:
    U8GLIB_SSD1306_64X48(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_64X48(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_64X48(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_i2c, options)
      { }
};

class U8GLIB_SSD1306_64X48_2X : public U8GLIB
{
  public:
    U8GLIB_SSD1306_64X48_2X(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_64X48_2X(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_2x_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1306_64X48_2X(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1306_64x48_2x_i2c, options)
      { }
};


class U8GLIB_NHD27OLED_GR : public U8GLIB
{
  public:
    U8GLIB_NHD27OLED_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_gr_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_NHD27OLED_2X_GR : public U8GLIB
{
  public:
    U8GLIB_NHD27OLED_2X_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_2x_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_NHD27OLED_2X_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1325_nhd27oled_2x_gr_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1327_96X96_GR : public U8GLIB
{
  public:
    U8GLIB_SSD1327_96X96_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1327_96X96_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_gr_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1327_96X96_GR(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_gr_i2c, options)
      { }
};

class U8GLIB_SSD1327_96X96_2X_GR : public U8GLIB
{
  public:
    U8GLIB_SSD1327_96X96_2X_GR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_2x_gr_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1327_96X96_2X_GR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_2x_gr_hw_spi, cs, a0, reset)
      { }
    U8GLIB_SSD1327_96X96_2X_GR(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ssd1327_96x96_2x_gr_i2c, options)
      { }
};

/*
class U8GLIB_LD7032_60x32 : public U8GLIB
{
  public:
    U8GLIB_LD7032_60x32(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ld7032_60x32_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_LD7032_60x32(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ld7032_60x32_hw_spi, cs, a0, reset)
      { }
    U8GLIB_LD7032_60x32(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ld7032_60x32_parallel, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
    U8GLIB_LD7032_60x32(uint8_t options = U8G_I2C_OPT_NONE)
      : U8GLIB(&u8g_dev_ld7032_60x32_i2c, options)
      { }

};

*/

class U8GLIB_HT1632_24X16 : public U8GLIB
{
  public:
    U8GLIB_HT1632_24X16(uint8_t wr, uint8_t data, uint8_t cs)
      : U8GLIB(&u8g_dev_ht1632_24x16, wr, data, cs, U8G_PIN_NONE, U8G_PIN_NONE)
      { }
};



class U8GLIB_PCF8812 : public U8GLIB
{
  public:
    U8GLIB_PCF8812(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcf8812_96x65_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_PCF8812(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcf8812_96x65_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_PCD8544 : public U8GLIB
{
  public:
    U8GLIB_PCD8544(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcd8544_84x48_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_PCD8544(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcd8544_84x48_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_TLS8204_84X48 : public U8GLIB
{
  public:
    U8GLIB_TLS8204_84X48(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_tls8204_84x48_sw_spi, sck, mosi, cs, a0, reset)
      { }
};

class U8GLIB_KS0108_128 : public U8GLIB
{
  public:
    U8GLIB_KS0108_128(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ks0108_128x64_fast, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
      { }
};

class U8GLIB_SBN1661_122X32 : public U8GLIB
{
  public:
    U8GLIB_SBN1661_122X32(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_sbn1661_122x32, d0, d1, d2, d3, d4, d5, d6, d7, U8G_PIN_NONE, cs1, cs2, di, rw, reset)
      { }
};

class U8GLIB_T6963_240X128 : public U8GLIB
{
  public:
    U8GLIB_T6963_240X128(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_t6963_240x128_8bit, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset)
      { }
};

class U8GLIB_T6963_128X128 : public U8GLIB
{
  public:
    U8GLIB_T6963_128X128(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_t6963_128x128_8bit, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset)
      { }
};

class U8GLIB_T6963_240X64 : public U8GLIB
{
  public:
    U8GLIB_T6963_240X64(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_t6963_240x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset)
      { }
};

class U8GLIB_T6963_128X64 : public U8GLIB
{
  public:
    U8GLIB_T6963_128X64(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_t6963_128x64_8bit, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset)
      { }
};


class U8GLIB_ST7687_C144MVGD: public U8GLIB
{
  public:
    U8GLIB_ST7687_C144MVGD(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7687_c144mvgd_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_ST7687_C144MVGD(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs, uint8_t a0, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ks0108_128x64_fast, d0, d1, d2, d3, d4, d5, d6, d7, en, cs, U8G_PIN_NONE, a0, rw, reset)
      { }
};

class U8GLIB_ILI9325D_320x240 : public U8GLIB
{
  public:
    /*
    U8GLIB_ILI9325D_320x240(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ili9325d_320x240_8bit, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, U8G_PIN_NONE, di, rw, reset)
      { }
  */
    U8GLIB_ILI9325D_320x240( uint8_t en, uint8_t cs1, uint8_t di, uint8_t rw = U8G_PIN_NONE, uint8_t reset = U8G_PIN_NONE)
      { init8BitFixedPort(&u8g_dev_ili9325d_320x240_8bit, en, cs1, di, rw, reset); }
};



class U8GLIB_SSD1351_128X128_332 : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128_332(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_332_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128_332(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_332_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128_4X_332 : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128_4X_332(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_4x_332_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128_4X_332(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_4x_332_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128GH_332 : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128GH_332(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_332_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128GH_332(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_332_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128GH_4X_332 : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128GH_4X_332(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_4x_332_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128GH_4X_332(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_4x_332_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128_IDX : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128_IDX(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_idx_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128_IDX(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_idx_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128_HICOLOR : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128_HICOLOR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_hicolor_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128_HICOLOR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_hicolor_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128_4X_HICOLOR : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128_4X_HICOLOR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_4x_hicolor_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128_4X_HICOLOR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128_4x_hicolor_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128GH_HICOLOR : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128GH_HICOLOR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_hicolor_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128GH_HICOLOR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_hicolor_hw_spi, cs, a0, reset)
      { }
};

class U8GLIB_SSD1351_128X128GH_4X_HICOLOR : public U8GLIB
{
  public:
    U8GLIB_SSD1351_128X128GH_4X_HICOLOR(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_4x_hicolor_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_SSD1351_128X128GH_4X_HICOLOR(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1351_128x128gh_4x_hicolor_hw_spi, cs, a0, reset)
      { }
};


class U8GLIB_FLIPDISC_2X7 : public U8GLIB
{
  public:
    U8GLIB_FLIPDISC_2X7(void) : U8GLIB(&u8g_dev_flipdisc_2x7)
      {  }
};

class U8GLIB_VS : public U8GLIB
{
  public:
    U8GLIB_VS(void) : U8GLIB(&u8g_dev_vs)
      {  }
};
