/*

  dogm128.c
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  graphic functions for the dogm128 graphics module 
  with ST7565R controller from electronic assembly
  
  optimized c-code for avr-gcc


  This file is part of the dogm128 Arduino library.

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


  ST7565R SPI interface
  
    ST7565R reads data with rising edge of the clock signal (SCL)
      --> CPHA = 0 and CPOL = 0
    ST7565R reads  MSB first 
      --> DORD = 0
  
  __AVR__
  /usr/lib/avr/include/avr/pgmspace.h

  Note: 
  The Microchip PIC Port currently is valid for the MPLAB C18 only.
    

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dogm128.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#elif defined(__18CXX)
#include <delays.h>
#else
#endif

/* moved to dogmspi.c
#ifdef ADA_ST7565P_HW
#include <wiring.h>	
#endif
*/

unsigned char dog_page_buffer[DOG_PAGE_SIZE];
uint8_t dog_curr_page = 0;	/* 0...DOG_PAGE_CNT-1 */
uint8_t dog_min_y = 0;
uint8_t dog_max_y = DOG_PAGE_HEIGHT-1;


uint8_t dog_spi_pin_a0 = PIN_A0_DEFAULT;
uint8_t dog_spi_pin_cs = PIN_SS;	/* arduino chip select pin, defaults to the hardware pin */

#ifdef ADA_ST7565P_HW
uint8_t dog_spi_pin_rst = PIN_RST;
#define ST7565P_STARTBYTES 1
#endif

#ifdef ES13BB0_HW
uint8_t dog_page_map[8] = {4, 5, 6, 7, 0, 1, 2, 3};
#endif


uint8_t dog_spi_result = 0;		/* last value returned from SPI system (after executing the picture loop) */

/*==============================================================*/
/* 
  SPI subsystem  for Arduino
  Pins 15 to 19 (ATMEGA8) or 10 to 13 (Arduino) are used.
*/
/*==============================================================*/



/*
  Delay by the provided number of milliseconds.
  Thus, a 16 bit value will allow a delay of 0..65 seconds
  Makes use of the _delay_loop_2
  
  _delay_loop_2 will do a delay of n * 4 prozessor cycles.
  with f = F_CPU cycles per second,
  n = f / (1000 * 4 )
  with f = 16000000 the result is 4000
  with f = 1000000 the result is 250
  
  the millisec loop, gcc requires the following overhead:
  - movev 1
  - subwi 2x2
  - bne i 2
  ==> 7 cycles
*/
#if defined(__AVR__)
void dog_Delay(uint16_t val)
{
  /* old version did a call to the arduino lib: delay(val); */
  while( val != 0 )
  {
    _delay_loop_2( (F_CPU / 4000 ) -7);
    val--;
  }
}
#elif defined(__18CXX)

#define GetSystemClock()		(64000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)

void dog_Delay(uint16_t val)
{
	unsigned int _iTemp = (val);
	while(_iTemp--)		
		Delay1KTCYx((GetInstructionClock()+999999)/1000000);
}
#elif defined(__PIC32MX)
/* assume chipkig and use chipkit procedure */
void dog_Delay(uint16_t val)
{
	delay(val);
}

#else
void dog_Delay(uint16_t val)
{
	/* do not know how to delay... */
}
#endif


static void dog_init_display(void)
{
  
  dog_Delay(10);
  dog_spi_disable_client();	/* force reset of the spi subsystem of the ST7565R */
  dog_Delay(10);
  dog_spi_enable_client();
  dog_Delay(10);
  dog_cmd_mode();

#ifdef ADA_ST7565P_HW  
  dog_spi_out(0xA3);            // LCD bias select to 1/7 (1/9) does NOT work!

  dog_spi_out(0xA1);            // ACD set to reverse
  dog_spi_out(0xC0);            // Common output mode to normal
  dog_spi_out(0xA6);            // display normal (bit=0 -> pixel off)
  dog_spi_out(0x40 | 0x20);     // Set display line start to 32 !! (pages messed up)

  dog_spi_out(0x28 | 0x4);      // turn on voltage converter (VC=1, VR=0, VF=0)
  dog_Delay(50);                // wait for 50% rising
  dog_spi_out(0x28 | 0x6);      // turn on voltage regulator (VC=1, VR=1, VF=0)
  dog_Delay(50);                // wait >=50ms
  dog_spi_out(0x28 | 0x7);      // turn on voltage follower (VC=1, VR=1, VF=1)
  dog_Delay(10);                // wait

  dog_spi_out(0x20 | 0x6);      // set lcd operating voltage

  dog_spi_out(0x81);            // set contrast
  dog_spi_out(0x18);            // contrast value
  //dog_spi_out(0xAC);            // indicator on
  dog_spi_out(0xAF);            // display on
#endif

  /* mostly taken from the EA dogm description */

#if defined(DOGM128_HW) || defined(ES13BB0_HW)
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x0f8);		/* set booster ratio to */
  dog_spi_out(0x000);		/* 4x */
  dog_spi_out(0x027);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x018);		/* contrast value, EA default: 0x016 */
  dog_spi_out(0x0ac);		/* indicator */
  dog_spi_out(0x000);		/* disable */
  dog_spi_out(0x0af);		/* display on */
#endif

#ifdef DOGM132_HW
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x0f8);		/* set booster ratio to */
  dog_spi_out(0x000);		/* 4x */
  dog_spi_out(0x023);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x01f);		/* contrast value, EA default: 0x01f */
  dog_spi_out(0x0ac);		/* indicator */
  dog_spi_out(0x000);		/* disable */
  dog_spi_out(0x0af);		/* display on */
#endif

#ifdef DOGS102_HW
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x027);		/* regulator, booster and follower */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x00e);		/* contrast value, EA default: 0x010, previous value for S102: 0x0e */
  dog_spi_out(0x0fa);		/* Set Temp compensation */ 
  dog_spi_out(0x090);		/* 0.11 deg/c WP Off WC Off*/
  dog_spi_out(0x0a4);		/* normal display  */
  dog_spi_out(0x0af);		/* display on */
#endif

#if defined(DOGXL160_HW_BW) || defined(DOGXL160_HW_GR)
  dog_spi_out(0x0f1);		/* set display height-1 */
  dog_spi_out(0x067);		/*  */
  dog_spi_out(0x0c0);		/* SEG & COM normal */
  dog_spi_out(0x040);		/* set display start line */
  dog_spi_out(0x050);		/* */
  dog_spi_out(0x02b);		/* set panelloading */
  dog_spi_out(0x0eb);		/* set bias 1/2 */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x05f);		/* */
  dog_spi_out(0x089);		/* set auto increment */
  dog_spi_out(0x0a6);		/* normal pixel mode, issue 103 */
  dog_spi_out(0x0d3);		/* 0xd3=40% RMS separation for gray levels */
  dog_spi_out(0x0af);		/* display on */
#endif

  dog_spi_out(0x0a5);		/* display all points, ST7565, UC1610 */
  dog_Delay(300);
  dog_spi_out(0x0a4);		/* normal display  */
  dog_Delay(300);
  dog_spi_disable_client();
  //Serial.println("exiting dog_init_display");
}

/* pin values of 255 are not assigned */
void dog_InitA0CS(uint8_t pin_a0, uint8_t pin_cs)
{
  dog_Delay(60);			/* initial delay */
  if ( pin_a0 != 255 )
    dog_spi_pin_a0 = pin_a0;
  if ( pin_cs != 255 )
    dog_spi_pin_cs = pin_cs;

#if defined(DOGXL160_HW_GR)
  dog_pixel_value = 3;			/* assign max gray value for DOGXL160 */
#endif

  dog_spi_init();
  dog_init_display();
}

void dog_Init(unsigned short pin_a0)
{
  dog_InitA0CS(pin_a0, 255);
}

#if defined(DOGXL160_HW_BW)
uint8_t dog_4to8[16] = { 0x00, 0x03, 0x0c, 0x0f, 0x30, 0x33, 0x3c, 0x3f, 0xc0, 0xc3, 0xcc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff };
#endif

static void dog_transfer_sub_page(uint8_t page, uint8_t  offset)
{
  uint8_t idx;
  uint8_t *buffer = dog_page_buffer;
  
  buffer += offset;

  /* enable and reset com interface of the ST7565R */
  dog_spi_enable_client();
  
#ifdef DOG_REVERSE
  
#if defined(DOGXL160_HW_BW)
#if defined(DOG_DOUBLE_MEMORY)
  page--;
#endif
  dog_cmd_mode();
  dog_spi_out(0x060 | (page*2) );		/* select current page  (UC1610)*/
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  dog_data_mode();
  idx = 0;
  while( idx != DOG_PAGE_WIDTH )
  {
    dog_spi_out(dog_4to8[buffer[idx] & 15]); 
    idx++;
  }  
  dog_cmd_mode();
  dog_spi_out(0x060 | ((page*2)+1) );		/* select current page  (UC1610)*/
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  dog_data_mode();
  idx = 0;
  while( idx != DOG_PAGE_WIDTH )
  {
    dog_spi_out(dog_4to8[(buffer[idx] >> 4)&15]); 
    idx++;
  }  
#else
  /* set write position */
  dog_cmd_mode();
#ifdef DOGXL160_HW_GR
  dog_spi_out(0x060 | (page) );		/* select current page  (UC1610)*/
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
#else
  dog_spi_out(0x0b0 | page );		/* select current page (ST7565R) */
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
#endif
  /* send a complete page */
  dog_data_mode();
  idx = 0;
  while( idx != DOG_PAGE_WIDTH )
  {
    dog_spi_out(buffer[idx] );
    idx++;
  }
#endif
  
#else /* DOG_REVERSE */
  
#if defined(DOGXL160_HW_BW)
  dog_cmd_mode();
  dog_spi_out(0x060 | (page*2) );		/* select current page  (UC1610)*/
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  dog_data_mode();
  idx = DOG_PAGE_WIDTH;
  while( idx != 0 )
  {
    idx--;
    dog_spi_out(dog_4to8[buffer[idx] & 15]); 
  }  
  dog_cmd_mode();
  dog_spi_out(0x060 | ((page*2)+1) );		/* select current page  (UC1610)*/
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  dog_data_mode();
  idx = DOG_PAGE_WIDTH;
  while( idx != 0 )
  {
    idx--;
    dog_spi_out(dog_4to8[(buffer[idx] >> 4)&15]); 
  }  
#else
  /* set write position */
  dog_cmd_mode();
#ifdef DOGXL160_HW_GR
  dog_spi_out(0x060 | (page) );                 /* select current page  (UC1610)*/
  dog_spi_out(0x010 );                          /* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
#else
  dog_spi_out(0x0b0 | page );           	/* select current page (ST7565R/P) */
#ifndef ADA_ST7565P_HW
  dog_spi_out(0x010 );                          /* set upper 4 bit of the col adr to 0 */
  dog_spi_result = dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
#else
  dog_spi_out( 0x10 );                          /* set upper 4 bits of the col adr to 0 */
  dog_spi_out( 0x00 | 0x03 );                   /* set lower 4 bits of the col adr to 3 ! */ 
#endif
#endif
  
  /* send a complete page */
  dog_data_mode();
  idx = DOG_PAGE_WIDTH;
  while( idx != 0 )
  {
    idx--;
    dog_spi_out(buffer[idx] ); 
  }
#endif
  
#endif

  /* disable com interface of the ST7565R */

  dog_spi_disable_client();
}

static void dog_transfer_page(void)
{
#if defined(DOG_DOUBLE_MEMORY)
#if defined(DOG_REVERSE)
  dog_transfer_sub_page(dog_curr_page*2, DOG_WIDTH);
  dog_transfer_sub_page(dog_curr_page*2+1, 0);
#else
  dog_transfer_sub_page(dog_curr_page*2, 0);
  dog_transfer_sub_page(dog_curr_page*2+1, DOG_WIDTH);
#endif
#else
#ifdef ES13BB0_HW
  dog_transfer_sub_page(dog_page_map[dog_curr_page], 0);
#else 
  dog_transfer_sub_page(dog_curr_page, 0);
#endif
#endif  
}




/*==============================================================*/
/* page buffer functions */
/*==============================================================*/

static void dog_ClearPage(void)
{
  uint16_t i;
  for( i = 0; i < DOG_PAGE_SIZE; i++ )
  {
    dog_page_buffer[i] = 0;
  }
}

void dog_StartPage(void)
{
#ifdef DOG_REVERSE
  dog_curr_page = DOG_PAGE_CNT - 1;
#else
  dog_curr_page = 0;
#endif
  dog_min_y = 0;
  dog_max_y = DOG_PAGE_HEIGHT-1;
  dog_ClearPage();
}

uint8_t dog_NextPage(void)
{
  dog_transfer_page();
  dog_ClearPage();
  
#ifdef DOG_REVERSE
  if ( dog_curr_page == 0 )
      return 0;
  dog_curr_page--;
#else
  dog_curr_page++;
  if ( dog_curr_page >= DOG_PAGE_CNT )
      return 0;
#endif  
 
/*  
  dog_min_y = DOG_PAGE_HEIGHT;
  dog_min_y *= dog_curr_page;
  dog_max_y = dog_min_y;
  dog_max_y += DOG_PAGE_HEIGHT-1;
*/
  
  dog_min_y += DOG_PAGE_HEIGHT;
  dog_max_y += DOG_PAGE_HEIGHT;
  return 1;
}

