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

#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_STM32

#include <Arduino.h>
#include <stdint.h>
#include "HAL_softserial_STM32.h"

#ifdef STM32F0xx
  #ifndef HAL_TIMER_RATE
    #define HAL_TIMER_RATE (F_CPU)
  #endif
  
  #ifndef SS_TIMER
    #define SS_TIMER 4
  #endif

#elif defined(STM32F1xx)
  #ifndef HAL_TIMER_RATE
    #define HAL_TIMER_RATE (F_CPU)
  #endif

  #ifndef SS_TIMER
    #define SS_TIMER 3
  #endif

#elif defined(STM32F4xx) || defined(STM32F7xx)

  #ifndef HAL_TIMER_RATE
    #define HAL_TIMER_RATE (F_CPU/2)
  #endif

  #ifndef SS_TIMER
    #define SS_TIMER 9
  #endif

#endif

#define SS_TIMER_RATE 1000000 //1MHz

#define __SS_TIMER_DEV(X) TIM##X
#define _SS_TIMER_DEV(X) __SS_TIMER_DEV(X)
#define SS_TIMER_DEV _SS_TIMER_DEV(SS_TIMER)

#define __SS_TIMER_IRQ(X) TIM##X##_IRQn
#define _SS_TIMER_IRQ(X) __SS_TIMER_IRQ(X)
#define SS_TIMER_IRQ _SS_TIMER_IRQ(SS_TIMER)

stimer_t SSTimerHandle;

void HAL_softSerial_init() {      
  uint32_t prescaler = (HAL_TIMER_RATE / SS_TIMER_RATE) - 1;
  
  SSTimerHandle.timer = SS_TIMER_DEV;
  SSTimerHandle.irqHandle = SoftSerial_Handler;
  TimerHandleInit(&SSTimerHandle, 0, prescaler);
  NVIC_SetPriority(SS_TIMER_IRQ, NVIC_EncodePriority(0, INTERRUPT_PRIORITY, 0));
}

void HAL_softserial_setSpeed(uint32_t speed) {
  NVIC_DisableIRQ(SS_TIMER_IRQ);
  if (speed != 0) {
    uint32_t period  = (SS_TIMER_RATE / (speed*OVERSAMPLE)) - 1;
    
    SS_TIMER_DEV->ARR = period;
    SS_TIMER_DEV->CNT = 0;
    SS_TIMER_DEV->CR1 |= 0x01;
    NVIC_EnableIRQ(SS_TIMER_IRQ);
  }
}

#endif

