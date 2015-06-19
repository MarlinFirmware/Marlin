/*
  
  u8g_com_arduino_t6963.c

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


  PIN_D0 8
  PIN_D1 9
  PIN_D2 10
  PIN_D3 11
  PIN_D4 4
  PIN_D5 5
  PIN_D6 6
  PIN_D7 7

  PIN_CS 14
  PIN_A0 15
  PIN_RESET 16
  PIN_WR 17
  PIN_RD 18
  
  u8g_InitRW8Bit(u8g, dev, d0, d1, d2, d3, d4, d5, d6, d7, cs, a0, wr, rd, reset)
  u8g_InitRW8Bit(u8g, dev,  8,  9, 10, 11,  4,  5,  6,  7, 14, 15, 17, 18, 16)

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START();
    U8G_ATOMIC_END();
 

*/

#include "u8g.h"

#if  defined(ARDUINO)

#if ARDUINO < 100 
//#include <WProgram.h> 
#include <wiring_private.h> 
#include <pins_arduino.h> 
#else 
#include <Arduino.h> 
#endif


#if defined(__PIC32MX)
/* CHIPKIT PIC32 */
static volatile uint32_t *u8g_output_data_port[8];
static volatile uint32_t *u8g_input_data_port[8];
static volatile uint32_t *u8g_mode_port[8];
static uint32_t u8g_data_mask[8];
#else
static volatile uint8_t *u8g_output_data_port[8];
static volatile uint8_t *u8g_input_data_port[8];
static volatile uint8_t *u8g_mode_port[8];
static uint8_t u8g_data_mask[8];
#endif



static void u8g_com_arduino_t6963_init(u8g_t *u8g)
{
  u8g_output_data_port[0] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D0]));
  u8g_input_data_port[0] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D0]));
  u8g_mode_port[0] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D0]));
  u8g_data_mask[0] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D0]);
  
  u8g_output_data_port[1] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D1]));
  u8g_input_data_port[1] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D1]));
  u8g_mode_port[1] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D1]));
  u8g_data_mask[1] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D1]);
  
  u8g_output_data_port[2] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D2]));
  u8g_input_data_port[2] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D2]));
  u8g_mode_port[2] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D2]));
  u8g_data_mask[2] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D2]);
  
  u8g_output_data_port[3] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D3]));
  u8g_input_data_port[3] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D3]));
  u8g_mode_port[3] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D3]));
  u8g_data_mask[3] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D3]);
  
  u8g_output_data_port[4] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D4]));
  u8g_input_data_port[4] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D4]));
  u8g_mode_port[4] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D4]));
  u8g_data_mask[4] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D4]);
  
  u8g_output_data_port[5] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D5]));
  u8g_input_data_port[5] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D5]));
  u8g_mode_port[5] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D5]));
  u8g_data_mask[5] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D5]);
  
  u8g_output_data_port[6] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D6]));
  u8g_input_data_port[6] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D6]));
  u8g_mode_port[6] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D6]));
  u8g_data_mask[6] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D6]);
  
  u8g_output_data_port[7] =  portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D7]));
  u8g_input_data_port[7] =  portInputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D7]));
  u8g_mode_port[7] =  portModeRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D7]));
  u8g_data_mask[7] =  digitalPinToBitMask(u8g->pin_list[U8G_PI_D7]);  
}


static void u8g_com_arduino_t6963_write_data_pin(uint8_t pin, uint8_t val)
{
  /* no ATOMIC protection required here, this is done by calling procedure */
  if ( val != 0 )
    *u8g_output_data_port[pin] |= u8g_data_mask[pin];
  else
    *u8g_output_data_port[pin] &= ~u8g_data_mask[pin];
}

static void u8g_com_arduino_t6963_set_port_output(void)
{
  uint8_t i;
  U8G_ATOMIC_START();
  for( i = 0; i < 8; i++ )
  {
#if defined(__PIC32MX)
/* CHIPKIT PIC32 */
      *u8g_mode_port[i] |= u8g_data_mask[i]; 
#elif defined(__AVR__)
      *u8g_mode_port[i] |= u8g_data_mask[i]; 
#else
      /* TODO: use generic Arduino API */
      *u8g_mode_port[i] |= u8g_data_mask[i]; 
#endif

  }
  U8G_ATOMIC_END();
}

static void u8g_com_arduino_t6963_set_port_input(void)
{
  uint8_t i;
  U8G_ATOMIC_START();
  for( i = 0; i < 8; i++ )
  {
#if defined(__PIC32MX)
/* CHIPKIT PIC32 */
      *u8g_mode_port[i] &= ~u8g_data_mask[i]; 
#elif defined(__AVR__)
/* avr */
      *u8g_mode_port[i] &= ~u8g_data_mask[i]; 
      *u8g_output_data_port[i] &= ~u8g_data_mask[i]; 	// no pullup
#else
      /* TODO: use generic Arduino API */
      *u8g_mode_port[i] &= ~u8g_data_mask[i]; 
      *u8g_output_data_port[i] &= ~u8g_data_mask[i]; 	// no pullup
#endif
  }
  U8G_ATOMIC_END();
}


static void u8g_com_arduino_t6963_write(u8g_t *u8g, uint8_t val)
{
  U8G_ATOMIC_START();

  u8g_com_arduino_t6963_write_data_pin( 0, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 1, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 2, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 3, val&1 );
  val >>= 1;

  u8g_com_arduino_t6963_write_data_pin( 4, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 5, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 6, val&1 );
  val >>= 1;
  u8g_com_arduino_t6963_write_data_pin( 7, val&1 );
  val >>= 1;
  U8G_ATOMIC_END();
  
  u8g_com_arduino_digital_write(u8g, U8G_PI_WR, 0);
  u8g_MicroDelay(); /* 80ns, reference: t6963 datasheet */
  u8g_com_arduino_digital_write(u8g, U8G_PI_WR, 1);
  u8g_MicroDelay(); /* 10ns, reference: t6963 datasheet */
}

static uint8_t u8g_com_arduino_t6963_read(u8g_t *u8g)
{
  uint8_t val = 0;
  
  u8g_com_arduino_digital_write(u8g, U8G_PI_RD, 0);
  u8g_MicroDelay(); /* 150ns, reference: t6963 datasheet */
  
  U8G_ATOMIC_START();
  /* only read bits 0, 1 and 3 */
  if ( (*u8g_input_data_port[3] & u8g_data_mask[3]) != 0 )
    val++;
  val <<= 1;
  val <<= 1;
  if ( (*u8g_input_data_port[1] & u8g_data_mask[1]) != 0 )
    val++;
  val <<= 1;
  if ( (*u8g_input_data_port[0] & u8g_data_mask[0]) != 0 )
    val++;
  U8G_ATOMIC_END();

  u8g_com_arduino_digital_write(u8g, U8G_PI_RD, 1);
  u8g_MicroDelay(); /* 10ns, reference: t6963 datasheet */
  
  return val;
}

#define U8G_STATUS_TIMEOUT 50

static uint8_t u8g_com_arduino_t6963_until_01_ok(u8g_t *u8g)
{
  long x;
  
  u8g_com_arduino_t6963_set_port_input();
  x = millis();
  x += U8G_STATUS_TIMEOUT;

  for(;;)
  {    
    if ( (u8g_com_arduino_t6963_read(u8g) & 3) == 3 )
      break;
    if ( x < millis() )
      return 0;
  }
  u8g_com_arduino_t6963_set_port_output();
  return 1;
}

static uint8_t u8g_com_arduino_t6963_until_3_ok(u8g_t *u8g)
{
  long x;
  
  u8g_com_arduino_t6963_set_port_input();
  x = millis();
  x += U8G_STATUS_TIMEOUT;

  for(;;)
  {    
    if ( (u8g_com_arduino_t6963_read(u8g) & 8) == 8 )
      break;
    if ( x < millis() )
      return 0;
  }
  u8g_com_arduino_t6963_set_port_output();
  return 1;
}

static uint8_t u8g_com_arduino_t6963_write_cmd(u8g_t *u8g, uint8_t val)
{
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 1);
  if ( u8g_com_arduino_t6963_until_01_ok(u8g) == 0 )
    return 0;
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 1);
  u8g_com_arduino_t6963_write(u8g, val);
  return 1;  
}

static uint8_t u8g_com_arduino_t6963_write_data(u8g_t *u8g, uint8_t val)
{
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 1);
  if ( u8g_com_arduino_t6963_until_01_ok(u8g) == 0 )
    return 0;
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 0);
  u8g_com_arduino_t6963_write(u8g, val);
  return 1;  
}

static uint8_t u8g_com_arduino_t6963_write_auto_data(u8g_t *u8g, uint8_t val)
{
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 1);
  if ( u8g_com_arduino_t6963_until_3_ok(u8g) == 0 )
    return 0;
  u8g_com_arduino_digital_write(u8g, U8G_PI_A0, 0);
  u8g_com_arduino_t6963_write(u8g, val);
  return 1;  
}


uint8_t u8g_com_arduino_t6963_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      u8g->pin_list[U8G_PI_A0_STATE] = 0;
      u8g_com_arduino_t6963_init(u8g);
      /* setup the RW (equal to WR) pin as output and force it to high */
      if ( u8g->pin_list[U8G_PI_WR] != U8G_PIN_NONE )
      {
        pinMode(u8g->pin_list[U8G_PI_WR], OUTPUT);
        u8g_com_arduino_digital_write(u8g, U8G_PI_WR, HIGH);
      }
      /* set all pins (except WR pin) */
      u8g_com_arduino_assign_pin_output_high(u8g);
      break;
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable, active low chip select */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      else
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      break;
    case U8G_COM_MSG_WRITE_BYTE:
      if ( u8g->pin_list[U8G_PI_A0_STATE] == 0 )
      {
	u8g_com_arduino_t6963_write_data(u8g, arg_val);
      }
      else
      {
	u8g_com_arduino_t6963_write_cmd(u8g, arg_val);
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
	u8g_com_arduino_t6963_write_cmd(u8g, 0x0b0);	/* auto write */
        while( arg_val > 0 )
        {
          if ( u8g_com_arduino_t6963_write_auto_data(u8g, *ptr++) == 0 )
	    break;
          arg_val--;
        }
	u8g_com_arduino_t6963_write_cmd(u8g, 0x0b2);	/* auto reset */
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
	u8g_com_arduino_t6963_write_cmd(u8g, 0x0b0);	/* auto write */
        while( arg_val > 0 )
        {
          if ( u8g_com_arduino_t6963_write_auto_data(u8g, u8g_pgm_read(ptr)) == 0 )
	    break;
          ptr++;
          arg_val--;
        }
	u8g_com_arduino_t6963_write_cmd(u8g, 0x0b2);	/* auto reset */
      }
      break;
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 1) or data mode (arg_val = 0) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      //u8g_com_arduino_digital_write(u8g, U8G_PI_DI, arg_val);
      break;
    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;
      
  }
  return 1;
}

#else


uint8_t u8g_com_arduino_t6963_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}


#endif /* ARDUINO */

