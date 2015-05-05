#ifndef ULCDST7920_H
#define ULCDST7920_H

#include "Marlin.h"

#ifdef U8GLIB_ST7920

//set optimization so ARDUINO optimizes this file
#pragma GCC optimize (3)

#define ST7920_CLK_PIN  LCD_PINS_D4
#define ST7920_DAT_PIN  LCD_PINS_ENABLE
#define ST7920_CS_PIN   LCD_PINS_RS

//#define PAGE_HEIGHT 8   //128 byte framebuffer
//#define PAGE_HEIGHT 16  //256 byte framebuffer
#define PAGE_HEIGHT 32  //512 byte framebuffer

#define WIDTH 128
#define HEIGHT 64

#include <U8glib.h>

extern void ST7920_SWSPI_SND_8BIT(uint8_t val);

#define ST7920_CS()              {WRITE(ST7920_CS_PIN,1);u8g_10MicroDelay();}
#define ST7920_NCS()             {WRITE(ST7920_CS_PIN,0);}
#define ST7920_SET_CMD()         {ST7920_SWSPI_SND_8BIT(0xf8);u8g_10MicroDelay();}
#define ST7920_SET_DAT()         {ST7920_SWSPI_SND_8BIT(0xfa);u8g_10MicroDelay();}
#define ST7920_WRITE_BYTE(a)     {ST7920_SWSPI_SND_8BIT((a)&0xf0);ST7920_SWSPI_SND_8BIT((a)<<4);u8g_10MicroDelay();}
#define ST7920_WRITE_BYTES(p,l)  {uint8_t i;for(i=0;i<l;i++){ST7920_SWSPI_SND_8BIT(*p&0xf0);ST7920_SWSPI_SND_8BIT(*p<<4);p++;}u8g_10MicroDelay();}

uint8_t u8g_dev_rrd_st7920_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

class U8GLIB_ST7920_128X64_RRD : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_RRD(uint8_t dummy);
};


#endif //U8GLIB_ST7920
#endif //ULCDST7920_H
