/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define HAL_TIMER_RATE      SystemCoreClock  // frequency of timers peripherals

#define STEP_TIMER_NUM      0  // index of timer to use for stepper (also +1 for 32bits counter)
#define PULSE_TIMER_NUM     STEP_TIMER_NUM
#define TONE_TIMER_NUM      2  // index of timer to use for beeper tones (also +1 for 32bits counter)
#define TEMP_TIMER_NUM      4  // index of timer to use for temperature (also +1 for 32bits counter)

#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define STEPPER_TIMER_RATE          HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US  ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per �s
#define STEPPER_TIMER_PRESCALE      (CYCLES_PER_MICROSECOND / STEPPER_TIMER_TICKS_PER_US)

#define PULSE_TIMER_RATE          STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE      STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US  STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED()               HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define TC_PRIORITY(t)        (t == STEP_TIMER_NUM || t == PULSE_TIMER_NUM) ? 2     \
                               : (t == TEMP_TIMER_NUM) ? 6                          \
                               : (t == TONE_TIMER_NUM) ? 5 : 7
#define _TC_HANDLER(t)        void TC##t##_Handler()
#define TC_HANDLER(t)         _TC_HANDLER(t)
#define HAL_STEP_TIMER_ISR()  TC_HANDLER(STEP_TIMER_NUM)
#if STEP_TIMER_NUM != PULSE_TIMER_NUM
  #define HAL_PULSE_TIMER_ISR()  TC_HANDLER(PULSE_TIMER_NUM)
#endif
#define HAL_TEMP_TIMER_ISR()  TC_HANDLER(TEMP_TIMER_NUM)
#define HAL_TONE_TIMER_ISR()  TC_HANDLER(TONE_TIMER_NUM)

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef struct {
  Tc          *pTimer;
  IRQn_Type   IRQ_Id;
  uint8_t     priority;
} tTimerConfig;

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

extern const tTimerConfig TimerConfig[];

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  Tc * const tc = TimerConfig[timer_num].pTimer;
  tc->COUNT32.CC[0].reg = HAL_TIMER_TYPE_MAX - compare;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  Tc * const tc = TimerConfig[timer_num].pTimer;
  return (hal_timer_t)(HAL_TIMER_TYPE_MAX - tc->COUNT32.CC[0].reg);
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  Tc * const tc = TimerConfig[timer_num].pTimer;
  tc->COUNT32.CTRLBSET.reg = TC_CTRLBCLR_CMD_READSYNC;
  SYNC(tc->COUNT32.SYNCBUSY.bit.CTRLB || tc->COUNT32.SYNCBUSY.bit.COUNT);
  return HAL_TIMER_TYPE_MAX - tc->COUNT32.COUNT.reg;
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  Tc * const tc = TimerConfig[timer_num].pTimer;
  // Clear interrupt flag
  tc->COUNT32.INTFLAG.reg = TC_INTFLAG_OVF;
}

#define HAL_timer_isr_epilogue(timer_num)
