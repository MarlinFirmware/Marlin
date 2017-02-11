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

//#define U8G_I2C_WITH_NO_ACK

static uint8_t u8g_i2c_err_code;
static uint8_t u8g_i2c_opt;		/* U8G_I2C_OPT_NO_ACK, SAM: U8G_I2C_OPT_DEV_1 */
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

    TWBR = 12;
    TWSR = 0;
	--> 400KHz

    F_CPU/(2*100000)-8  --> calculate TWBR value for 100KHz
*/
  u8g_i2c_opt = options;
  TWSR = 0;
  if ( options & U8G_I2C_OPT_FAST )
  {
    TWBR = F_CPU/(2*400000)-8;
  }
  else
  {  
    TWBR = F_CPU/(2*100000)-8;
  }
  u8g_i2c_clear_error();
}

uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos)
{
  volatile uint16_t cnt = 2000;	/* timout value should be > 280 for 50KHz Bus and 16 Mhz CPU, however the start condition might need longer */
  while( !(TWCR & mask) )
  {
      if ( cnt == 0 )
      {
	if ( u8g_i2c_opt & U8G_I2C_OPT_NO_ACK )
	{
	  return 1;	/* all ok */
	}
	else
	{
	  u8g_i2c_set_error(U8G_I2C_ERR_TIMEOUT, pos);
	  return 0; /* error */
	}
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

  if ( u8g_i2c_opt & U8G_I2C_OPT_NO_ACK )
  {
    /* do not check for ACK */
  }
  else
  {
    status = TW_STATUS;
    /* check status after sla */  
    if ( status != TW_MT_SLA_ACK )
    {
      u8g_i2c_set_error(U8G_I2C_ERR_BUS, 2);
      return 0;
    }
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
    
  if ( u8g_i2c_opt & U8G_I2C_OPT_NO_ACK )
  {
    /* do not check for ACK */
  }
  else
  {
    status = TW_STATUS;  
    if ( status != TW_MT_DATA_ACK )
    {
      u8g_i2c_set_error(U8G_I2C_ERR_BUS, 3);
      return 0;
    }
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

#elif defined(ARDUINO) && defined(__SAM3X8E__)
/* Arduino Due */
#include "Arduino.h"
#include "sam.h"

/*

Controller

TWI0 TWCK0 PA18 A			DUE PCB: SCL1
TWI0 TWD0 PA17 A			DUE PCB: SDA1 
TWI1 TWCK1 PB13 A			DUE PCB: SCL 21
TWI1 TWD1 PB12 A			DUE PCB: SDA 20

Arduino definitions

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)
#define WIRE_INTERFACE       TWI1
#define WIRE_INTERFACE_ID    ID_TWI1
#define WIRE_ISR_HANDLER     TWI1_Handler

#define PIN_WIRE1_SDA        (70u)
#define PIN_WIRE1_SCL        (71u)
#define WIRE1_INTERFACE      TWI0
#define WIRE1_INTERFACE_ID   ID_TWI0
#define WIRE1_ISR_HANDLER    TWI0_Handler


*/

static void i2c_400KHz_delay(void)
{
  /* should be at least 4 */
  /* should be 5 for 100KHz transfer speed */
 
  
  /*
    Arduino Due
    0x NOP: 470KHz
    4x NOP: 450KHz
    8x NOP: 430KHz
    16x NOP: 400KHz
  */
  
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  __NOP();
  __NOP();
  __NOP();
  __NOP();

  __NOP();
  __NOP();
  __NOP();
  __NOP();

  __NOP();
  __NOP();
  __NOP();
  __NOP();
}

static void i2c_100KHz_delay(void)
{
  /* 
    1x u8g_MicroDelay()	ca. 130KHz
    2x u8g_MicroDelay()	ca. 80KHz 
  */
  u8g_MicroDelay();
  u8g_MicroDelay();  
}


uint32_t i2c_started = 0;
uint32_t i2c_scl_pin = 0;
uint32_t i2c_sda_pin = 0;
void (*i2c_delay)(void) = i2c_100KHz_delay;

const PinDescription *i2c_scl_pin_desc;
const PinDescription *i2c_sda_pin_desc;


/* maybe this can be optimized */
static void i2c_init(void)
{
  i2c_sda_pin_desc = &(g_APinDescription[i2c_sda_pin]);
  i2c_scl_pin_desc = &(g_APinDescription[i2c_scl_pin]);
  pinMode(i2c_sda_pin, OUTPUT);
  digitalWrite(i2c_sda_pin, HIGH);
  pinMode(i2c_scl_pin, OUTPUT);
  digitalWrite(i2c_scl_pin, HIGH);
  PIO_Configure( i2c_sda_pin_desc->pPort, PIO_OUTPUT_0, i2c_sda_pin_desc->ulPin, PIO_OPENDRAIN );
  PIO_Configure( i2c_scl_pin_desc->pPort, PIO_OUTPUT_0, i2c_scl_pin_desc->ulPin, PIO_OPENDRAIN );
  PIO_Clear( i2c_sda_pin_desc->pPort, i2c_sda_pin_desc->ulPin) ;
  PIO_Clear( i2c_scl_pin_desc->pPort, i2c_scl_pin_desc->ulPin) ;
  PIO_Configure( i2c_sda_pin_desc->pPort, PIO_INPUT, i2c_sda_pin_desc->ulPin, PIO_DEFAULT ) ;
  PIO_Configure( i2c_scl_pin_desc->pPort, PIO_INPUT, i2c_scl_pin_desc->ulPin, PIO_DEFAULT ) ;
  i2c_delay();
}

/* actually, the scl line is not observed, so this procedure does not return a value */
static void i2c_read_scl_and_delay(void)
{
  uint32_t dwMask = i2c_scl_pin_desc->ulPin;
  //PIO_Configure( i2c_scl_pin_desc->pPort, PIO_INPUT, i2c_scl_pin_desc->ulPin, PIO_DEFAULT ) ;
  //PIO_SetInput( i2c_scl_pin_desc->pPort, i2c_scl_pin_desc->ulPin, PIO_DEFAULT ) ;

  /* set as input */
  i2c_scl_pin_desc->pPort->PIO_ODR = dwMask ;
  i2c_scl_pin_desc->pPort->PIO_PER = dwMask ;

  i2c_delay();
}

static void i2c_clear_scl(void)
{
  uint32_t dwMask = i2c_scl_pin_desc->ulPin;
  
  /* set open collector and drive low */
  //PIO_Configure( i2c_scl_pin_desc->pPort, PIO_OUTPUT_0, i2c_scl_pin_desc->ulPin, PIO_OPENDRAIN );
  //PIO_SetOutput( i2c_scl_pin_desc->pPort, i2c_scl_pin_desc->ulPin, 0, 1, 0);

  /* open drain, zero default output */
  i2c_scl_pin_desc->pPort->PIO_MDER = dwMask;
  i2c_scl_pin_desc->pPort->PIO_CODR = dwMask;
  i2c_scl_pin_desc->pPort->PIO_OER = dwMask;
  i2c_scl_pin_desc->pPort->PIO_PER = dwMask;

  //PIO_Clear( i2c_scl_pin_desc->pPort, i2c_scl_pin_desc->ulPin) ;
}

static uint8_t i2c_read_sda(void)
{
  uint32_t dwMask = i2c_sda_pin_desc->ulPin;
  //PIO_Configure( i2c_sda_pin_desc->pPort, PIO_INPUT, i2c_sda_pin_desc->ulPin, PIO_DEFAULT ) ;
  //PIO_SetInput( i2c_sda_pin_desc->pPort, i2c_sda_pin_desc->ulPin, PIO_DEFAULT ) ;

  /* set as input */
  i2c_sda_pin_desc->pPort->PIO_ODR = dwMask ;
  i2c_sda_pin_desc->pPort->PIO_PER = dwMask ;


  return 1;
}

static void i2c_clear_sda(void)
{
  uint32_t dwMask = i2c_sda_pin_desc->ulPin;
  
  /* set open collector and drive low */
  //PIO_Configure( i2c_sda_pin_desc->pPort, PIO_OUTPUT_0, i2c_sda_pin_desc->ulPin, PIO_OPENDRAIN );
  //PIO_SetOutput( i2c_sda_pin_desc->pPort, i2c_sda_pin_desc->ulPin, 0, 1, 0);
  
  /* open drain, zero default output */
  i2c_sda_pin_desc->pPort->PIO_MDER = dwMask ;
  i2c_sda_pin_desc->pPort->PIO_CODR = dwMask ;
  i2c_sda_pin_desc->pPort->PIO_OER = dwMask ;
  i2c_sda_pin_desc->pPort->PIO_PER = dwMask ;
  
  //PIO_Clear( i2c_sda_pin_desc->pPort, i2c_sda_pin_desc->ulPin) ;
}

static void i2c_start(void)
{
  if ( i2c_started != 0 )
  {
    /* if already started: do restart */
    i2c_read_sda();     /* SDA = 1 */
    i2c_delay();
    i2c_read_scl_and_delay();
  }
  i2c_read_sda();
  /*
  if (i2c_read_sda() == 0)
  {
    // do something because arbitration is lost
  }
  */
  /* send the start condition, both lines go from 1 to 0 */
  i2c_clear_sda();
  i2c_delay();
  i2c_clear_scl();
  i2c_started = 1;
}


static void i2c_stop(void)
{
  /* set SDA to 0 */
  i2c_clear_sda();  
  i2c_delay();
 
  /* now release all lines */
  i2c_read_scl_and_delay();
 
  /* set SDA to 1 */
  i2c_read_sda();
  i2c_delay();
  i2c_started = 0;
}

static void i2c_write_bit(uint8_t val)
{
  if (val)
    i2c_read_sda();
  else
    i2c_clear_sda();
 
  i2c_delay();
  i2c_read_scl_and_delay();
  i2c_clear_scl();
}

static uint8_t i2c_read_bit(void)
{
  uint8_t val;
  /* do not drive SDA */
  i2c_read_sda();
  i2c_delay();
  i2c_read_scl_and_delay();
  val = i2c_read_sda();
  i2c_delay();
  i2c_clear_scl();
  return val;
}

static uint8_t i2c_write_byte(uint8_t b)
{
  i2c_write_bit(b & 128);
  i2c_write_bit(b & 64);
  i2c_write_bit(b & 32);
  i2c_write_bit(b & 16);
  i2c_write_bit(b & 8);
  i2c_write_bit(b & 4);
  i2c_write_bit(b & 2);
  i2c_write_bit(b & 1);
    
  /* read ack from client */
  /* 0: ack was given by client */
  /* 1: nothing happend during ack cycle */  
  return i2c_read_bit();
}



void u8g_i2c_init(uint8_t options)
{
  u8g_i2c_opt = options;
  u8g_i2c_clear_error();

  if ( u8g_i2c_opt & U8G_I2C_OPT_FAST )
  {
    i2c_delay = i2c_400KHz_delay;
  }
  else
  {
    i2c_delay = i2c_100KHz_delay;
  }


  if ( u8g_i2c_opt & U8G_I2C_OPT_DEV_1 )
  {
    i2c_scl_pin = PIN_WIRE1_SCL;
    i2c_sda_pin = PIN_WIRE1_SDA;
    
    //REG_PIOA_PDR = PIO_PB12A_TWD1 | PIO_PB13A_TWCK1;
  }
  else
  {    
    
    i2c_scl_pin = PIN_WIRE_SCL;
    i2c_sda_pin = PIN_WIRE_SDA;
    
    //REG_PIOA_PDR = PIO_PA17A_TWD0 | PIO_PA18A_TWCK0;
  }
  
  i2c_init();

}

/* sla includes also the r/w bit */
uint8_t u8g_i2c_start(uint8_t sla)
{  
  i2c_start();
  i2c_write_byte(sla);
  return 1;
}

uint8_t u8g_i2c_send_byte(uint8_t data)
{
  return i2c_write_byte(data);
}

void u8g_i2c_stop(void)
{
  i2c_stop();
}


#elif defined(U8G_RASPBERRY_PI)

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define I2C_SLA         0x3c

static int fd=-1;
static uint8_t i2cMode = 0;

void u8g_i2c_init(uint8_t options) {
   u8g_i2c_clear_error();
   u8g_i2c_opt = options;

   if (wiringPiSetup() == -1) {
      printf("wiringPi-Error\n");
      exit(1);
   }

   fd = wiringPiI2CSetup(I2C_SLA);
   if (fd < 0) {
      printf ("Unable to open I2C device 0: %s\n", strerror (errno)) ;
      exit (1) ;
   }
   //u8g_SetPIOutput(u8g, U8G_PI_RESET);
   //u8g_SetPIOutput(u8g, U8G_PI_A0);
}
uint8_t u8g_i2c_start(uint8_t sla) {
   u8g_i2c_send_mode(0);

   return 1;
}

void u8g_i2c_stop(void) {
}

uint8_t u8g_i2c_send_mode(uint8_t mode) {
   i2cMode = mode;
} 

uint8_t u8g_i2c_send_byte(uint8_t data) {
   wiringPiI2CWriteReg8(fd, i2cMode, data);

   return 1;
}

uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos)
{
  return 1;
}

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

