/*
  
  u8g_com_raspberrypi_hw_spi.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2012, olikraus@gmail.com
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
  
  
  Assumes, that 
    MOSI is at PORTB, Pin 3
  and
    SCK is at PORTB, Pin 5

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START()
    U8G_ATOMIC_END()
 


*/

#include "u8g.h"



#if defined(U8G_RASPBERRY_PI)

#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

uint8_t u8g_com_raspberrypi_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;
    
    case U8G_COM_MSG_INIT:
		// check wiringPi setup
		if (wiringPiSetup() == -1)
		{
			printf("wiringPi-Error\n");
			exit(1);
		}

		if (wiringPiSPISetup (0, 100000) < 0)
		{
			printf ("Unable to open SPI device 0: %s\n", strerror (errno)) ;
			exit (1) ;
		}
		
		u8g_SetPIOutput(u8g, U8G_PI_RESET);
		u8g_SetPIOutput(u8g, U8G_PI_A0);

      break;
    
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
	  u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
		/* Done by the SPI hardware */
      break;
      
    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;
    
    case U8G_COM_MSG_WRITE_BYTE:
		wiringPiSPIDataRW (0, &arg_val, 1) ;
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
		wiringPiSPIDataRW (0, arg_ptr, arg_val);
      break;

	case U8G_COM_MSG_WRITE_SEQ_P:
		wiringPiSPIDataRW (0, arg_ptr, arg_val);		
      break;
  }
  return 1;
}

#else

uint8_t u8g_com_raspberrypi_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif


