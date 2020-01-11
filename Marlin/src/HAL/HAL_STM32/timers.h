/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>
#include "../../inc/MarlinConfig.h"

// ------------------------
// Defines
// ------------------------

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

#elif defined(STM32F1xx)

  #define HAL_TIMER_RATE (F_CPU) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 4
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 2
  #endif

#elif defined(STM32F4xx) || defined(STM32F7xx)

  #define HAL_TIMER_RATE (F_CPU/2) // frequency of timer peripherals

  #ifndef STEP_TIMER
    #define STEP_TIMER 6
  #endif

  #ifndef TEMP_TIMER
    #define TEMP_TIMER 14 // TIM7 is consumed by Software Serial if used.
  #endif

#endif

#ifndef SWSERIAL_TIMER_IRQ_PRIO
  #define SWSERIAL_TIMER_IRQ_PRIO 1
#endif

#ifndef STEP_TIMER_IRQ_PRIO
  #define STEP_TIMER_IRQ_PRIO 2
#endif

#ifndef TEMP_TIMER_IRQ_PRIO
  #define TEMP_TIMER_IRQ_PRIO 14 //14 = after hardware ISRs
#endif

#define STEP_TIMER_NUM 0  // index of timer to use for stepper
#define TEMP_TIMER_NUM 1  // index of timer to use for temperature
#define PULSE_TIMER_NUM STEP_TIMER_NUM

#define TEMP_TIMER_FREQUENCY 1000 //Temperature::isr() is expected to be called at around 1kHz

//TODO: get rid of manual rate/prescale/ticks/cycles taken for procedures in stepper.cpp
#define STEPPER_TIMER_RATE 2000000 // 2 Mhz
#define STEPPER_TIMER_PRESCALE ((HAL_TIMER_RATE)/(STEPPER_TIMER_RATE))
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs

#define PULSE_TIMER_RATE STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define __TIMER_IRQ_NAME(X) TIM##X##_IRQn
#define _TIMER_IRQ_NAME(X) __TIMER_IRQ_NAME(X)

#define STEP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(STEP_TIMER)
#define TEMP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(TEMP_TIMER)

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

extern void Step_Handler(HardwareTimer *htim);
extern void Temp_Handler(HardwareTimer *htim);
#define HAL_STEP_TIMER_ISR() void Step_Handler(HardwareTimer *htim)
#define HAL_TEMP_TIMER_ISR() void Temp_Handler(HardwareTimer *htim)

// ------------------------
// Public Variables
// ------------------------

extern HardwareTimer *timer_instance[];

// ------------------------
// Public functions
// ------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);
void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

//TIM_TypeDef* HAL_timer_device(const uint8_t timer_num); no need to be public for now. not public = not used externally

// FORCE_INLINE because these are used in performance-critical situations
FORCE_INLINE bool HAL_timer_initialized(const uint8_t timer_num) {
  return timer_instance[timer_num] != NULL;
}
FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  return HAL_timer_initialized(timer_num) ? timer_instance[timer_num]->getCount() : 0;
}

// NOTE: Method name may be misleading.
// STM32 has an Auto-Reload Register (ARR) as opposed to a "compare" register
FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t overflow) {
  if (HAL_timer_initialized(timer_num)) {
    timer_instance[timer_num]->setOverflow(overflow + 1, TICK_FORMAT); // Value decremented by setOverflow()
    // wiki: "force all registers (Autoreload, prescaler, compare) to be taken into account"
    // So, if the new overflow value is less than the count it will trigger a rollover interrupt.
    if (overflow < timer_instance[timer_num]->getCount())  // Added 'if' here because reports say it won't boot without it
      timer_instance[timer_num]->refresh();
  }
}

#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)
