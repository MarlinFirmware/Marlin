/**
 * FYSETC
 *
 * Copyright (c) 2019 SoftwareSerialM [https://github.com/FYSETC/SoftwareSerialM]
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
 * HAL for Adafruit Grand Central and compatible (SAMD51)
 */

#pragma once

#include <Arduino.h>

#ifndef SS_TIMER
  #define SS_TIMER 4
#endif

#define OVERSAMPLE 3
#define INTERRUPT_PRIORITY 0

#define gpio_set(IO,V)  do {                                                                  \
                          if (V) digitalPinToPort(IO)->OUTSET.reg = digitalPinToBitMask(IO);  \
                          else digitalPinToPort(IO)->OUTCLR.reg = digitalPinToBitMask(IO);    \
                        }while(0)
#define gpio_get(IO)  ((digitalPinToPort(IO)->IN.reg & digitalPinToBitMask(IO)) ? HIGH : LOW)

#define __SS_TIMERIRQ(t)    TC##t##_IRQn
#define _SS_TIMERIRQ(t)     __SS_TIMERIRQ(t)
#define SS_TIMERIRQ         _SS_TIMERIRQ(SS_TIMER)

#define __SS_TC_DEV(t)      TC##t
#define _SS_TC_DEV(t)       __SS_TC_DEV(t)
#define SS_TC_DEV           _SS_TC_DEV(SS_TIMER)

#define _SS_TC_HANDLER(t)   void TC##t##_Handler()
#define SS_TC_HANDLER(t)    _SS_TC_HANDLER(t)

#define Disable_Irq(i)  do {                  \
                          NVIC_DisableIRQ(i); \
                          __DSB();            \
                          __ISB();            \
                        }while(0)

#define cli()   Disable_Irq(SS_TIMERIRQ)        // Disable timer interrupt
#define sei()   NVIC_EnableIRQ(SS_TIMERIRQ)     // Enable timer interrupt

#define HAL_softserial_timer_isr_prologue() do{ SS_TC_DEV->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF; } while(0)
#define HAL_softserial_timer_isr_epilogue()

#define HAL_SOFTSERIAL_TIMER_ISR() SS_TC_HANDLER(SS_TIMER)
