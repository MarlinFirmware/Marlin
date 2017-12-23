/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ULCDST7565_H
#define ULCDST7565_H

#include <U8glib.h>

#define ST7565_CLK_PIN  DOGLCD_SCK
#define ST7565_DAT_PIN  DOGLCD_MOSI
#define ST7565_CS_PIN   DOGLCD_CS
#define ST7565_A0_PIN   DOGLCD_A0

#define LCD_PIXEL_WIDTH 128
#define LCD_PIXEL_HEIGHT 64
#define PAGE_HEIGHT 8

//set optimization so ARDUINO optimizes this file
#pragma GCC optimize (3)

// If you want you can define your own set of delays in Configuration.h
//#define ST7565_DELAY_1 DELAY_0_NOP
//#define ST7565_DELAY_2 DELAY_0_NOP
//#define ST7565_DELAY_3 DELAY_0_NOP

/*
#define ST7565_DELAY_1 u8g_10MicroDelay()
#define ST7565_DELAY_2 u8g_10MicroDelay()
#define ST7565_DELAY_3 u8g_10MicroDelay()
*/

#if F_CPU >= 20000000
  #define CPU_ST7565_DELAY_1 DELAY_0_NOP
  #define CPU_ST7565_DELAY_2 DELAY_0_NOP
  #define CPU_ST7565_DELAY_3 DELAY_1_NOP
#elif (MOTHERBOARD == BOARD_3DRAG) || (MOTHERBOARD == BOARD_K8200) || (MOTHERBOARD == BOARD_K8400)
  #define CPU_ST7565_DELAY_1 DELAY_0_NOP
  #define CPU_ST7565_DELAY_2 DELAY_3_NOP
  #define CPU_ST7565_DELAY_3 DELAY_0_NOP
#elif (MOTHERBOARD == BOARD_MINIRAMBO)
  #define CPU_ST7565_DELAY_1 DELAY_0_NOP
  #define CPU_ST7565_DELAY_2 DELAY_4_NOP
  #define CPU_ST7565_DELAY_3 DELAY_0_NOP
#elif (MOTHERBOARD == BOARD_RAMBO)
  #define CPU_ST7565_DELAY_1 DELAY_0_NOP
  #define CPU_ST7565_DELAY_2 DELAY_0_NOP
  #define CPU_ST7565_DELAY_3 DELAY_0_NOP
#elif F_CPU == 16000000
  #define CPU_ST7565_DELAY_1 DELAY_0_NOP
  #define CPU_ST7565_DELAY_2 DELAY_0_NOP
  #define CPU_ST7565_DELAY_3 DELAY_1_NOP
#else
  #error "No valid condition for delays in 'ultralcd_st7565_u8glib_VIKI.h'"
#endif

#ifndef ST7565_DELAY_1
  #define ST7565_DELAY_1 CPU_ST7565_DELAY_1
#endif
#ifndef ST7565_DELAY_2
  #define ST7565_DELAY_2 CPU_ST7565_DELAY_2
#endif
#ifndef ST7565_DELAY_3
  #define ST7565_DELAY_3 CPU_ST7565_DELAY_3
#endif

#if ENABLED(SHARED_SPI)  // Re-ARM requires that the LCD and the SD card share a single SPI

  #define ST7565_WRITE_BYTE(a)                 { spiSend((uint8_t)a); U8G_DELAY(); }
  #define ST7560_WriteSequence(count, pointer) { uint8_t *ptr = pointer; for (uint8_t i = 0; i <  count; i++) {spiSend( *ptr++);} DELAY_10US; }

#else
  #define ST7565_SND_BIT \
    WRITE(ST7565_CLK_PIN, LOW);        ST7565_DELAY_1; \
    WRITE(ST7565_DAT_PIN, val & 0x80); ST7565_DELAY_2; \
    WRITE(ST7565_CLK_PIN, HIGH);       ST7565_DELAY_3; \
    WRITE(ST7565_CLK_PIN, LOW);\
    val <<= 1

  static void ST7565_SWSPI_SND_8BIT(uint8_t val) {
    ST7565_SND_BIT; // 1
    ST7565_SND_BIT; // 2
    ST7565_SND_BIT; // 3
    ST7565_SND_BIT; // 4
    ST7565_SND_BIT; // 5
    ST7565_SND_BIT; // 6
    ST7565_SND_BIT; // 7
    ST7565_SND_BIT; // 8
  }

  #define ST7565_WRITE_BYTE(a)                 { ST7565_SWSPI_SND_8BIT((uint8_t)a); U8G_DELAY(); }
  #define ST7560_WriteSequence(count, pointer) { uint8_t *ptr = pointer; for (uint8_t i = 0; i <  count; i++) {ST7565_SWSPI_SND_8BIT( *ptr++);} DELAY_10US; }
#endif

#if defined(DOGM_SPI_DELAY_US) && DOGM_SPI_DELAY_US > 0
  #define U8G_DELAY() delayMicroseconds(DOGM_SPI_DELAY_US)
#else
  #define U8G_DELAY() u8g_10MicroDelay()
#endif

#define ST7565_CS()                          { WRITE(ST7565_CS_PIN,1); U8G_DELAY(); }
#define ST7565_NCS()                         { WRITE(ST7565_CS_PIN,0); }
#define ST7565_A0()                          { WRITE(ST7565_A0_PIN,1); U8G_DELAY(); }
#define ST7565_NA0()                         { WRITE(ST7565_A0_PIN,0); }


uint8_t u8g_dev_st7565_64128n_2x_VIKI_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT: {
      OUT_WRITE(ST7565_CS_PIN, LOW);
      #if ENABLED(SHARED_SPI)
        u8g_Delay(250);
        spiBegin();
        #ifndef SPI_SPEED
          #define SPI_SPEED SPI_FULL_SPEED  // use same SPI speed as SD card
        #endif
        spiInit(SPI_SPEED);
      #else
        OUT_WRITE(ST7565_DAT_PIN, LOW);
        OUT_WRITE(ST7565_CLK_PIN, LOW);
      #endif
      OUT_WRITE(ST7565_A0_PIN, LOW);

      ST7565_CS();                      /* disable chip */
      ST7565_NA0();                     /* instruction mode */
      ST7565_NCS();                     /* enable chip */

      ST7565_WRITE_BYTE(0x0A2);         /* 0x0A2: LCD bias 1/9 (according to Displaytech 64128N datasheet) */
      ST7565_WRITE_BYTE(0x0A0);         /* Normal ADC Select (according to Displaytech 64128N datasheet) */

      ST7565_WRITE_BYTE(0x0C8);         /* common output mode: set scan direction normal operation/SHL Select; 0x0C0 --> SHL = 0; normal; 0x0C8 --> SHL = 1 */
      ST7565_WRITE_BYTE(0x040);         /* Display start line for Displaytech 64128N */

      ST7565_WRITE_BYTE(0x028 | 0x04);  /* power control: turn on voltage converter */
      //U8G_ESC_DLY(50);                /* delay 50 ms - hangs after a reset if used */

      ST7565_WRITE_BYTE(0x028 | 0x06);  /* power control: turn on voltage regulator */
      //U8G_ESC_DLY(50);                /* delay 50 ms - hangs after a reset if used */

      ST7565_WRITE_BYTE(0x028 | 0x07);  /* power control: turn on voltage follower */
      //U8G_ESC_DLY(50);                /* delay 50 ms - hangs after a reset if used */

      ST7565_WRITE_BYTE(0x010);         /* Set V0 voltage resistor ratio. Setting for controlling brightness of Displaytech 64128N */

      ST7565_WRITE_BYTE(0x0A6);         /* display normal, bit val 0: LCD pixel off. */

      ST7565_WRITE_BYTE(0x081);         /* set contrast */
      ST7565_WRITE_BYTE(0x01E);         /* Contrast value. Setting for controlling brightness of Displaytech 64128N */

      ST7565_WRITE_BYTE(0x0AF);         /* display on */

      U8G_ESC_DLY(100);                 /* delay 100 ms */
      ST7565_WRITE_BYTE(0x0A5);         /* display all points; ST7565 */
      U8G_ESC_DLY(100);                 /* delay 100 ms */
      U8G_ESC_DLY(100);                 /* delay 100 ms */
      ST7565_WRITE_BYTE(0x0A4);         /* normal display */
      ST7565_CS();                      /* disable chip */
    }                                   /* end of sequence */
    break;

    case U8G_DEV_MSG_STOP: break;

    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      ST7565_CS();                      /* disable chip */
      ST7565_NA0();                     /* instruction mode */
      ST7565_NCS();                     /* enable chip */
      ST7565_WRITE_BYTE(0x010);         /* set upper 4 bit of the col adr to 0x10 */
      ST7565_WRITE_BYTE(0x000);         /* set lower 4 bit of the col adr to 0x00. Changed for DisplayTech 64128N */
                                        /* end of sequence */
      ST7565_WRITE_BYTE(0x0B0 | (2*pb->p.page));; /* select current page (ST7565R) */
      ST7565_A0();                      /* data mode */
      ST7560_WriteSequence( (uint8_t) pb->width, (uint8_t *)pb->buf);
      ST7565_CS();                      /* disable chip */
      ST7565_NA0();                     /* instruction mode */
      ST7565_NCS();                     /* enable chip */
      ST7565_WRITE_BYTE(0x010);         /* set upper 4 bit of the col adr to 0x10 */
      ST7565_WRITE_BYTE(0x000);         /* set lower 4 bit of the col adr to 0x00. Changed for DisplayTech 64128N */
                                        /* end of sequence */
      ST7565_WRITE_BYTE(0x0B0 | (2*pb->p.page+1)); /* select current page (ST7565R) */
      ST7565_A0();                      /* data mode */
      ST7560_WriteSequence( (uint8_t) pb->width, (uint8_t *)(pb->buf)+pb->width);
      ST7565_CS();                      /* disable chip */
    }
    break;

    case U8G_DEV_MSG_CONTRAST:
      ST7565_NCS();
      ST7565_NA0();                     /* instruction mode */
      ST7565_WRITE_BYTE(0x081);
      ST7565_WRITE_BYTE((*(uint8_t *)arg) >> 2);
      ST7565_CS();                      /* disable chip */
      return 1;

    case U8G_DEV_MSG_SLEEP_ON:
      ST7565_NA0();                     /* instruction mode */
      ST7565_NCS();                     /* enable chip */
      ST7565_WRITE_BYTE(0x0AC);         /* static indicator off */
      ST7565_WRITE_BYTE(0x000);         /* indicator register set (not sure if this is required) */
      ST7565_WRITE_BYTE(0x0AE);         /* display off */
      ST7565_WRITE_BYTE(0x0A5);         /* all points on */
      ST7565_CS();                      /* disable chip , bugfix 12 nov 2014 */
                                        /* end of sequence */
      return 1;

    case U8G_DEV_MSG_SLEEP_OFF:
      ST7565_NA0();                     /* instruction mode */
      ST7565_NCS();                     /* enable chip */
      ST7565_WRITE_BYTE(0x0A4);         /* all points off */
      ST7565_WRITE_BYTE(0x0AF);         /* display on */
      U8G_ESC_DLY(50);                  /* delay 50 ms */
      ST7565_CS();                      /* disable chip ,  bugfix 12 nov 2014 */
                                        /* end of sequence */
      return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_st7565_64128n_2x_VIKI_buf[LCD_PIXEL_WIDTH*2] U8G_NOCOMMON;
u8g_pb_t u8g_dev_st7565_64128n_2x_VIKI_pb = {{16, LCD_PIXEL_HEIGHT, 0, 0, 0}, LCD_PIXEL_WIDTH, u8g_dev_st7565_64128n_2x_VIKI_buf};
u8g_dev_t u8g_dev_st7565_64128n_2x_VIKI_sw_spi = {u8g_dev_st7565_64128n_2x_VIKI_fn, &u8g_dev_st7565_64128n_2x_VIKI_pb, &u8g_com_null_fn};

class U8GLIB_ST7565_64128n_2x_VIKI : public U8GLIB {
  public:
  U8GLIB_ST7565_64128n_2x_VIKI(uint8_t dummy)
    : U8GLIB(&u8g_dev_st7565_64128n_2x_VIKI_sw_spi)
    {  }
  U8GLIB_ST7565_64128n_2x_VIKI(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
    : U8GLIB(&u8g_dev_st7565_64128n_2x_VIKI_sw_spi)
    {  }
};

#pragma GCC reset_options

#endif // ULCDST7565_H
