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

/**
 * LCD delay routines - used by all the drivers.
 *
 * These are based on the LPC1768 routines.
 *
 * Couldn't just call exact copies because the overhead resulted in the
 * one microsecond delay being about 4uS.
 */



#include <lpc17xx_pinsel.h>
#include "../../macros.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern volatile uint32_t _millis;

void U8g_delay(int msec) {
   volatile int32_t end = _millis + msec;
   SysTick->VAL = SysTick->LOAD; // reset systick counter so next systick is in exactly 1ms
                                 // this could extend the time between systicks by upto 1ms
   while (_millis < end) __WFE();
}


void u8g_MicroDelay(void)   //software SPI runs at ~ 100kHz - 100mS to do a complete update (10% load)
{
  #define SystemCoreClock 120000000
  static const int nop_factor = (SystemCoreClock / 11000000);
  static volatile int loops = 0;

  //previous ops already burned most of 1us, burn the rest
  loops = nop_factor / 4; //measured at 1us
  while (loops > 0) --loops;
}


void u8g_10MicroDelay(void)
{
  uint8_t us = 10;
  #define SystemCoreClock 120000000
  static const int nop_factor = (SystemCoreClock / 11000000);
  static volatile int loops = 0;

  //previous ops already burned most of 1us, burn the rest
  loops = nop_factor / 4; //measured at 1us
  while (loops > 0) --loops;

  us--;

  // poll systick, more accurate through interrupts if delay > 5 uS
  int32_t start = SysTick->VAL;
  int32_t load = SysTick->LOAD;
  int32_t end = start - (load / 1000) * us;

  if (end >> 31)
    while (!(SysTick->VAL > start && SysTick->VAL < (load + end))) __NOP();
  else
    while (SysTick->VAL > end) __NOP();

}

#ifdef __cplusplus
  }
#endif