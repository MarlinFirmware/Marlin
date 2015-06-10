#include "ultralcd_st7920_u8glib_rrd.h"

uint8_t  u8g_dev_st7920_128x64_rrd_buf[LCD_PIXEL_WIDTH*(PAGE_HEIGHT/8)] U8G_NOCOMMON;
u8g_pb_t  u8g_dev_st7920_128x64_rrd_pb = {{PAGE_HEIGHT,LCD_PIXEL_HEIGHT,0,0,0},LCD_PIXEL_WIDTH,u8g_dev_st7920_128x64_rrd_buf};
u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi = {u8g_dev_rrd_st7920_128x64_fn,&u8g_dev_st7920_128x64_rrd_pb,&u8g_com_null_fn};

void ST7920_SWSPI_SND_8BIT(uint8_t val)
{
  uint8_t i;
  for( i=0; i<8; i++ )
  {
    WRITE(ST7920_CLK_PIN,0);
    #if F_CPU == 20000000
      __asm__("nop\n\t"); 
    #endif
    WRITE(ST7920_DAT_PIN,val&0x80); 
    val<<=1;
    WRITE(ST7920_CLK_PIN,1);
    #if F_CPU == 20000000
      __asm__("nop\n\t""nop\n\t"); 
    #endif
  }
}


uint8_t u8g_dev_rrd_st7920_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  uint8_t i,y;
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      {
        OUT_WRITE(ST7920_CS_PIN,LOW);
        OUT_WRITE(ST7920_DAT_PIN,LOW);
        OUT_WRITE(ST7920_CLK_PIN,HIGH);

        ST7920_CS();
        u8g_Delay(120);                 //initial delay for boot up
        ST7920_SET_CMD();
        ST7920_WRITE_BYTE(0x08);       //display off, cursor+blink off
        ST7920_WRITE_BYTE(0x01);       //clear CGRAM ram
        u8g_Delay(15);                 //delay for CGRAM clear
        ST7920_WRITE_BYTE(0x3E);       //extended mode + GDRAM active
        for(y=0;y<LCD_PIXEL_HEIGHT/2;y++)        //clear GDRAM
        {
          ST7920_WRITE_BYTE(0x80|y);   //set y
          ST7920_WRITE_BYTE(0x80);     //set x = 0
          ST7920_SET_DAT();
          for(i=0;i<2*LCD_PIXEL_WIDTH/8;i++)     //2x width clears both segments
            ST7920_WRITE_BYTE(0);
          ST7920_SET_CMD();
        }
        ST7920_WRITE_BYTE(0x0C); //display on, cursor+blink off
        ST7920_NCS();
      }
      break;

    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        y = pb->p.page_y0;
        ptr = (uint8_t*)pb->buf;

        ST7920_CS();
        for( i = 0; i < PAGE_HEIGHT; i ++ )
        {
          ST7920_SET_CMD();
          if ( y < 32 )
          {
            ST7920_WRITE_BYTE(0x80 | y);       //y
            ST7920_WRITE_BYTE(0x80);           //x=0
          }
          else
          {
            ST7920_WRITE_BYTE(0x80 | (y-32));  //y
            ST7920_WRITE_BYTE(0x80 | 8);       //x=64
          }

          ST7920_SET_DAT();
          ST7920_WRITE_BYTES(ptr,LCD_PIXEL_WIDTH/8); //ptr is incremented inside of macro
          y++;
        }
        ST7920_NCS();
      }
      break;
  }
#if PAGE_HEIGHT == 8
  return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
#elif PAGE_HEIGHT == 16
  return u8g_dev_pb16h1_base_fn(u8g, dev, msg, arg);
#else
  return u8g_dev_pb32h1_base_fn(u8g, dev, msg, arg);
#endif
}
