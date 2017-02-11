/*
  
  u8g_com_arduino_st7920_custom.c
  
  Additional COM device, initially introduced for 3D Printer community
  Implements a fast SW SPI com subsystem

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

  A special SPI interface for ST7920 controller

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START();
    U8G_ATOMIC_END();


*/

#include "u8g.h"

#if defined(ARDUINO)

#if ARDUINO < 100 
#include <WProgram.h>    
#include "wiring_private.h"
#include "pins_arduino.h"

#else 
#include <Arduino.h> 
#include "wiring_private.h"
#endif

#if defined(__AVR__)

static uint8_t u8g_bitData, u8g_bitNotData;
static uint8_t u8g_bitClock, u8g_bitNotClock;
static volatile uint8_t *u8g_outData;
static volatile uint8_t *u8g_outClock;

static void u8g_com_arduino_init_shift_out(uint8_t dataPin, uint8_t clockPin)
{
  u8g_outData = portOutputRegister(digitalPinToPort(dataPin));
  u8g_outClock = portOutputRegister(digitalPinToPort(clockPin));
  u8g_bitData = digitalPinToBitMask(dataPin);
  u8g_bitClock = digitalPinToBitMask(clockPin);

  u8g_bitNotClock = u8g_bitClock;
  u8g_bitNotClock ^= 0x0ff;

  u8g_bitNotData = u8g_bitData;
  u8g_bitNotData ^= 0x0ff;
}

static void u8g_com_arduino_do_shift_out_msb_first(uint8_t val) U8G_NOINLINE;
static void u8g_com_arduino_do_shift_out_msb_first(uint8_t val)
{
  uint8_t cnt = 8;
  uint8_t bitData = u8g_bitData;
  uint8_t bitNotData = u8g_bitNotData;
  uint8_t bitClock = u8g_bitClock;
  uint8_t bitNotClock = u8g_bitNotClock;
  volatile uint8_t *outData = u8g_outData;
  volatile uint8_t *outClock = u8g_outClock;
  
  
  U8G_ATOMIC_START();
  bitData |= *outData;
  bitNotData &= *outData;
  do
  {
    if ( val & 128 )
      *outData = bitData;
    else
      *outData = bitNotData;

    /*
    *outClock |= bitClock;
    val <<= 1;
    cnt--;
    *outClock &= bitNotClock;
    */

    val <<= 1;
    *outClock &= bitNotClock;
    cnt--;
    // removed micro delays, because AVRs are too slow and the delay is not required
    //u8g_MicroDelay();
    *outClock |= bitClock;
    //u8g_MicroDelay();
  } while( cnt != 0 );
  U8G_ATOMIC_END();
}

#elif defined(__18CXX) || defined(__PIC32MX)

uint16_t dog_bitData, dog_bitNotData;
uint16_t dog_bitClock, dog_bitNotClock;
volatile uint32_t *dog_outData;
volatile uint32_t *dog_outClock;
volatile uint32_t dog_pic32_spi_tmp;

static void u8g_com_arduino_init_shift_out(uint8_t dataPin, uint8_t clockPin)
{
  dog_outData = portOutputRegister(digitalPinToPort(dataPin));
  dog_outClock = portOutputRegister(digitalPinToPort(clockPin));
  dog_bitData = digitalPinToBitMask(dataPin);
  dog_bitClock = digitalPinToBitMask(clockPin);

  dog_bitNotClock = dog_bitClock;
  dog_bitNotClock ^= 0x0ffff;

  dog_bitNotData = dog_bitData;
  dog_bitNotData ^= 0x0ffff;
}

static void u8g_com_arduino_do_shift_out_msb_first(uint8_t val)
{
  uint8_t cnt = 8;
  U8G_ATOMIC_START();
  do
  {
    if ( val & 128 )
	*dog_outData |= dog_bitData;
    else
	*dog_outData &= dog_bitNotData;    
    val <<= 1;
    //u8g_MicroDelay();
    //*dog_outClock |= dog_bitClock;
    *dog_outClock &= dog_bitNotClock;
    cnt--;
    u8g_MicroDelay();
    //*dog_outClock &= dog_bitNotClock;
    *dog_outClock |= dog_bitClock;
    u8g_MicroDelay();
    
  } while( cnt != 0 );
  U8G_ATOMIC_END();
}

#else

/* default interface, Arduino DUE (__arm__) */

uint8_t u8g_data_custom_pin;
uint8_t u8g_clock_custom_pin;

static void u8g_com_arduino_init_shift_out(uint8_t dataPin, uint8_t clockPin)
{
  u8g_data_custom_pin = dataPin;
  u8g_clock_custom_pin = clockPin;
}

static void u8g_com_arduino_do_shift_out_msb_first(uint8_t val)
{
  uint8_t cnt = 8;
  do
  {
    if ( val & 128 )
	digitalWrite(u8g_data_custom_pin, HIGH);
    else
	digitalWrite(u8g_data_custom_pin, LOW);
    val <<= 1;
    //u8g_MicroDelay();
    digitalWrite(u8g_clock_custom_pin, LOW);
    cnt--;
    u8g_MicroDelay();
    digitalWrite(u8g_clock_custom_pin, HIGH);
    u8g_MicroDelay();    
  } while( cnt != 0 );
}

#endif 


static void u8g_com_arduino_st7920_write_byte_seq(uint8_t rs, uint8_t *ptr, uint8_t len)
{
  uint8_t i;

  if ( rs == 0 )
  {
    /* command */
    u8g_com_arduino_do_shift_out_msb_first(0x0f8);
  }
  else if ( rs == 1 )
  {
    /* data */
    u8g_com_arduino_do_shift_out_msb_first(0x0fa);
  }

  while( len > 0 )
  {
    u8g_com_arduino_do_shift_out_msb_first(*ptr & 0x0f0);
    u8g_com_arduino_do_shift_out_msb_first(*ptr << 4);
    ptr++;
    len--;
    u8g_10MicroDelay();
  }
  
  for( i = 0; i < 4; i++ )
    u8g_10MicroDelay();
}

static void u8g_com_arduino_st7920_write_byte(uint8_t rs, uint8_t val)
{
  uint8_t i;

  if ( rs == 0 )
  {
    /* command */
    u8g_com_arduino_do_shift_out_msb_first(0x0f8);
  }
  else if ( rs == 1 )
  {
    /* data */
    u8g_com_arduino_do_shift_out_msb_first(0x0fa);
  }
  
  u8g_com_arduino_do_shift_out_msb_first(val & 0x0f0);
  u8g_com_arduino_do_shift_out_msb_first(val << 4);
  
  for( i = 0; i < 4; i++ )
    u8g_10MicroDelay();
    
}


uint8_t u8g_com_arduino_st7920_custom_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      u8g_com_arduino_assign_pin_output_high(u8g);
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      // u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, LOW);
      u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, HIGH);
      u8g_com_arduino_digital_write(u8g, U8G_PI_MOSI, LOW);
      u8g_com_arduino_init_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK]);
      u8g->pin_list[U8G_PI_A0_STATE] = 0;       /* inital RS state: command mode */
      break;
    
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
	u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;
      
    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable, note: the st7920 has an active high chip select */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      else
      {
        /* enable */
        //u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, HIGH);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_arduino_st7920_write_byte( u8g->pin_list[U8G_PI_A0_STATE], arg_val);
      //u8g->pin_list[U8G_PI_A0_STATE] = 2; 
      //u8g_arduino_sw_spi_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], arg_val);
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
      u8g_com_arduino_st7920_write_byte_seq(u8g->pin_list[U8G_PI_A0_STATE], (uint8_t *)arg_ptr, arg_val);
      break;

      case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_arduino_st7920_write_byte(u8g->pin_list[U8G_PI_A0_STATE], u8g_pgm_read(ptr) );
          //u8g->pin_list[U8G_PI_A0_STATE] = 2; 
          ptr++;
          arg_val--;
        }
      }
      break;
      
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      break;
  }
  return 1;
}

#else /* ARDUINO */

uint8_t u8g_com_arduino_st7920_custom_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

