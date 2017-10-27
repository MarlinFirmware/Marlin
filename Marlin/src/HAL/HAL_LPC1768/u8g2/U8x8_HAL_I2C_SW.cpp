/*

  u8x8_byte.c 
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
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


#if defined (__arm__)
#include <clib/u8x8.h>



void delayMicroseconds(uint32_t us);
void digitalWrite(uint8_t pin, uint8_t pin_status);

void u8x8_HAL_SetPin_SW_I2C(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t speed, uint8_t address, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_CS1, address);
  u8x8_SetPin(u8x8, U8X8_PIN_CS2, speed);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}


#define LCD_I2C_RATE u8x8->pins[U8X8_PIN_CS2]
#define LCD_I2C_ADDRESS u8x8->pins[U8X8_PIN_CS1]

/*
  software i2c,
  ignores ACK response (which is anyway not provided by some displays)
  also does not allow reading from the device
*/
static void i2c_delay() U8X8_NOINLINE;
static void i2c_delay()
{
  delayMicroseconds(1);  // 100KHz bit rate
}

static void i2c_init(u8x8_t *u8x8)
{
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_CLOCK], 1);
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_DATA], 1);
  
  i2c_delay();
}

/* actually, the scl line is not observed, so this procedure does not return a value */

static void i2c_read_scl_and_delay(u8x8_t *u8x8)
{
  /* set as input (line will be high) */
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_CLOCK], 1);

  i2c_delay();
}

static void i2c_clear_scl(u8x8_t *u8x8)
{
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_CLOCK], 0);
}

static void i2c_read_sda(u8x8_t *u8x8)
{
  /* set as input (line will be high) */
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_DATA], 1);
}

static void i2c_clear_sda(u8x8_t *u8x8)
{
  /* set open collector and drive low */
  digitalWrite(u8x8->pins[U8X8_PIN_I2C_DATA], 0);
}

static void i2c_start(u8x8_t *u8x8)
{
  if ( u8x8->i2c_started != 0 )
  {
    /* if already started: do restart */
    i2c_read_sda(u8x8);     /* SDA = 1 */
    i2c_delay();
    i2c_read_scl_and_delay(u8x8);
  }
  i2c_read_sda(u8x8);
  /* send the start condition, both lines go from 1 to 0 */
  i2c_clear_sda(u8x8);
  i2c_delay();
  i2c_clear_scl(u8x8);
  u8x8->i2c_started = 1;
}


static void i2c_stop(u8x8_t *u8x8)
{
  /* set SDA to 0 */
  i2c_clear_sda(u8x8);  
  i2c_delay();
 
  /* now release all lines */
  i2c_read_scl_and_delay(u8x8);
 
  /* set SDA to 1 */
  i2c_read_sda(u8x8);
  i2c_delay();
  u8x8->i2c_started = 0;
}

  
static void i2c_write_byte(u8x8_t *u8x8, uint8_t b)    // 100KHz LPC1768
{
  uint8_t val = b;
  
  for (uint8_t i = 0; i < 9; i++) {  // 9th bit for ack/nak
    digitalWrite(u8x8->pins[U8X8_PIN_I2C_DATA], val & 0x80);
//    i2c_delay();
    digitalWrite(u8x8->pins[U8X8_PIN_I2C_CLOCK], 1);
//    i2c_delay();
    digitalWrite(u8x8->pins[U8X8_PIN_I2C_CLOCK], 0);
    val = val << 1;
  }
    
  // read ack from client 
  // 0: ack was given by client 
  // 1: nothing happend during ack cycle  
}

uint8_t u8x8_HAL_sw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      
      while( arg_int > 0 )
      {
        i2c_write_byte(u8x8, *data);
        data++;
        arg_int--;
      }
      
      break;
      
    case U8X8_MSG_BYTE_INIT:
      i2c_init(u8x8);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      i2c_start(u8x8);
      i2c_write_byte(u8x8, LCD_I2C_ADDRESS << 1);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      i2c_stop(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}

#endif  // arm
