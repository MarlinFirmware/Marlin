/*
  
  u8g_com_i2c.c

  generic i2c interface

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

#include "u8g.h"

static uint8_t u8g_i2c_err_code;

/*
  position values
    1: start condition
    2: sla transfer
*/
static uint8_t u8g_i2c_err_pos;


void u8g_i2c_clear_error(void)
{
  u8g_i2c_err_code = U8G_I2C_ERR_NONE;
  u8g_i2c_err_pos = 0;
}

uint8_t  u8g_i2c_get_error(void)
{
  return u8g_i2c_err_code;
}

uint8_t u8g_i2c_get_err_pos(void)
{
  return u8g_i2c_err_pos;
}

static void u8g_i2c_set_error(uint8_t code, uint8_t pos)
{
  if ( u8g_i2c_err_code > 0 )
    return;
  u8g_i2c_err_code |= code;
  u8g_i2c_err_pos = pos;
}



#if defined(__AVR__)
#define U8G_ATMEGA_HW_TWI

/* remove the definition for attiny */
#if __AVR_ARCH__ == 2
#undef U8G_ATMEGA_HW_TWI
#endif
#if __AVR_ARCH__ == 25
#undef U8G_ATMEGA_HW_TWI
#endif
#endif

#if defined(U8G_ATMEGA_HW_TWI)

#include <avr/io.h>
#include <util/twi.h>



void u8g_i2c_init(uint8_t options)
{
  /*
  TWBR: bit rate register
  TWSR: status register (contains preselector bits)

  prescalar
    0		1
    1		4
    2		16
    3		64

  f = F_CPU/(16+2*TWBR*prescalar)
  
  F_CPU = 16MHz
    TWBR = 152;
    TWSR = 0;
	--> 50KHz

    TWBR = 72;
    TWSR = 0;
	--> 100KHz

    F_CPU/(2*100000)-8  --> calculate TWBR value for 100KHz
*/
  TWSR = 0;
  TWBR = F_CPU/(2*100000)-8;
  u8g_i2c_clear_error();
}

uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos)
{
  volatile uint16_t cnt = 2000;	/* timout value should be > 280 for 50KHz Bus and 16 Mhz CPU, however the start condition might need longer */
  while( !(TWCR & mask) )
  {
      if ( cnt == 0 )
      {
	u8g_i2c_set_error(U8G_I2C_ERR_TIMEOUT, pos);
	return 0; /* error */
      }
      cnt--;
    }
  return 1;	/* all ok */
}

/* sla includes all 8 bits (with r/w bit), assums master transmit */
uint8_t u8g_i2c_start(uint8_t sla)
{
  register uint8_t status;
  
  /* send start */
  TWCR = _BV(TWINT) |  _BV(TWSTA)  |  _BV(TWEN);
   
  /* wait */
  if ( u8g_i2c_wait(_BV(TWINT), 1) == 0 )
    return 0;
  
  status = TW_STATUS;
 
  /* check status after start */  
  if ( status != TW_START && status != TW_REP_START )
  {
    u8g_i2c_set_error(U8G_I2C_ERR_BUS, 1);
    return 0;
  }

  /* set slave address */  
  TWDR = sla;
  
  /* enable sla transfer */
  TWCR = _BV(TWINT)  |  _BV(TWEN);

  /* wait */
  if ( u8g_i2c_wait(_BV(TWINT), 2) == 0 )
    return 0;
  status = TW_STATUS;

  /* check status after sla */  
  if ( status != TW_MT_SLA_ACK )
  {
    u8g_i2c_set_error(U8G_I2C_ERR_BUS, 2);
    return 0;
  }

   return 1;
}

uint8_t u8g_i2c_send_byte(uint8_t data)
{
  register uint8_t status;
  TWDR = data;
  TWCR = _BV(TWINT)  |  _BV(TWEN);
  if ( u8g_i2c_wait(_BV(TWINT), 3) == 0 )
    return 0;
  status = TW_STATUS;
  
  if ( status != TW_MT_DATA_ACK )
  {
    u8g_i2c_set_error(U8G_I2C_ERR_BUS, 3);
    return 0;
  }
  
  return 1;  
}

void u8g_i2c_stop(void)
{
  /* write stop */
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

  /* no error is checked for the stop condition */  
  u8g_i2c_wait(_BV(TWSTO), 4);
  
}

/*
void twi_send(uint8_t adr, uint8_t data1, uint8_t data2)
{
  u8g_i2c_start(adr<<1);
  u8g_i2c_send_byte(data1);
  u8g_i2c_send_byte(data2);
  u8g_i2c_stop();
}
*/

#else

/* empty interface */

void u8g_i2c_init(uint8_t options)
{
  u8g_i2c_clear_error();
}

uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos)
{
  return 1;
}

uint8_t u8g_i2c_start(uint8_t sla)
{
  return 1;
}
uint8_t u8g_i2c_send_byte(uint8_t data)
{
  return 1;
}

void u8g_i2c_stop(void)
{
}


#endif

