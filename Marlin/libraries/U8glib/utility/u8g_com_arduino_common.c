/*
  
  u8g_com_arduino_common.c
  
  shared procedures for the arduino communication procedures

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

#if defined(ARDUINO)

#if ARDUINO < 100 
#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif

void u8g_com_arduino_digital_write(u8g_t *u8g, uint8_t pin_index, uint8_t value)
{
  uint8_t pin;
  pin = u8g->pin_list[pin_index];
  if ( pin != U8G_PIN_NONE )
    digitalWrite(pin, value);
}

/* this procedure does not set the RW pin */
void u8g_com_arduino_assign_pin_output_high(u8g_t *u8g)
{
  uint8_t i;
  /* skip the RW pin, which is the last pin in the list */
  for( i = 0; i < U8G_PIN_LIST_LEN-1; i++ )
  {
    if ( u8g->pin_list[i] != U8G_PIN_NONE )
    {
      pinMode(u8g->pin_list[i], OUTPUT);	
      digitalWrite(u8g->pin_list[i], HIGH);
    }
  }
}


#endif


