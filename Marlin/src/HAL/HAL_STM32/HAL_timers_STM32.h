/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2017 Victor Perez
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
#pragma once

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>
#include "../../inc/MarlinConfig.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

#define hal_timer_t uint32_t
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF // Timers can be 16 or 32 bit

#ifdef STM32F0xx

  #define HAL_TIMER_RATE (F_CPU) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 16
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 17
  #endif

#elif defined STM32F1xx

  #define HAL_TIMER_RATE (F_CPU) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 4
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 2
  #endif

#elif defined STM32F4xx

  #define HAL_TIMER_RATE (F_CPU/2) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 5
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 7
  #endif

#elif defined STM32F7xx

  #define HAL_TIMER_RATE (F_CPU/2) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 5
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 7
  #endif

#endif

#ifndef STEP_TIMER_IRQ_PRIO
  #define STEP_TIMER_IRQ_PRIO 1
#endif

#ifndef TEMP_TIMER_IRQ_PRIO
  #define TEMP_TIMER_IRQ_PRIO 2
#endif

#define STEP_TIMER_NUM 0  // index of timer to use for stepper
#define TEMP_TIMER_NUM 1  // index of timer to use for temperature
#define PULSE_TIMER_NUM STEP_TIMER_NUM

#define TEMP_TIMER_RATE 72000 // 72 Khz
#define TEMP_TIMER_PRESCALE ((HAL_TIMER_RATE)/(TEMP_TIMER_RATE))
#define TEMP_TIMER_FREQUENCY 1000

#define STEPPER_TIMER_RATE 2000000 // 2 Mhz
#define STEPPER_TIMER_PRESCALE ((HAL_TIMER_RATE)/(STEPPER_TIMER_RATE))
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs

#define PULSE_TIMER_RATE STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define __TIMER_DEV(X) TIM##X
#define _TIMER_DEV(X) __TIMER_DEV(X)
#define STEP_TIMER_DEV _TIMER_DEV(STEP_TIMER)
#define TEMP_TIMER_DEV _TIMER_DEV(TEMP_TIMER)

#define __TIMER_CALLBACK(X) TIM##X##_IRQHandler
#define _TIMER_CALLBACK(X) __TIMER_CALLBACK(X)

#define STEP_TIMER_CALLBACK _TIMER_CALLBACK(STEP_TIMER)
#define TEMP_TIMER_CALLBACK _TIMER_CALLBACK(TEMP_TIMER)

#define __TIMER_IRQ_NAME(X) TIM##X##_IRQn
#define _TIMER_IRQ_NAME(X) __TIMER_IRQ_NAME(X)

#define STEP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(STEP_TIMER)
#define TEMP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(TEMP_TIMER)

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

extern void Step_Handler(stimer_t *htim);
extern void Temp_Handler(stimer_t *htim);
#define HAL_STEP_TIMER_ISR void Step_Handler(stimer_t *htim)
#define HAL_TEMP_TIMER_ISR void Temp_Handler(stimer_t *htim)

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef stimer_t stm32f4_timer_t;

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

extern stm32f4_timer_t TimerHandle[];

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);
void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

FORCE_INLINE static uint32_t HAL_timer_get_count(const uint8_t timer_num) {
  return __HAL_TIM_GET_COUNTER(&TimerHandle[timer_num].handle);
}

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const uint32_t compare) {
  __HAL_TIM_SET_AUTORELOAD(&TimerHandle[timer_num].handle, compare);
  if (HAL_timer_get_count(timer_num) >= compare)
    TimerHandle[timer_num].handle.Instance->EGR |= TIM_EGR_UG; // Generate an immediate update interrupt
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  return __HAL_TIM_GET_AUTORELOAD(&TimerHandle[timer_num].handle);
}

#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)
