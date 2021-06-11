/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------
#define RTC_TIMER_NUM       8   // This is not a TC but a RTC

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define HAL_TIMER_RATE      F_CPU   // frequency of timers peripherals

#ifndef STEP_TIMER_NUM
  #define STEP_TIMER_NUM        0  // Timer Index for Stepper
#endif
#ifndef PULSE_TIMER_NUM
  #define PULSE_TIMER_NUM       STEP_TIMER_NUM
#endif
#ifndef TEMP_TIMER_NUM
  #define TEMP_TIMER_NUM        RTC_TIMER_NUM // Timer Index for Temperature
#endif

#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define STEPPER_TIMER_RATE          HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US  (STEPPER_TIMER_RATE / 1000000) // stepper timer ticks per µs
#define STEPPER_TIMER_PRESCALE      (CYCLES_PER_MICROSECOND / STEPPER_TIMER_TICKS_PER_US)

#define PULSE_TIMER_RATE          STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE      STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US  STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED()               HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define TC_PRIORITY(t)        t == SERVO_TC ? 1                                     \
                               : (t == STEP_TIMER_NUM || t == PULSE_TIMER_NUM) ? 2  \
                               : (t == TEMP_TIMER_NUM) ? 6                          \
                               : 7

#define _TC_HANDLER(t)          void TC##t##_Handler()
#define TC_HANDLER(t)           _TC_HANDLER(t)
#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR()  TC_HANDLER(STEP_TIMER_NUM)
#endif
#if STEP_TIMER_NUM != PULSE_TIMER_NUM
  #define HAL_PULSE_TIMER_ISR() TC_HANDLER(PULSE_TIMER_NUM)
#endif
#if TEMP_TIMER_NUM == RTC_TIMER_NUM
  #define HAL_TEMP_TIMER_ISR()  void RTC_Handler()
#else
  #define HAL_TEMP_TIMER_ISR()  TC_HANDLER(TEMP_TIMER_NUM)
#endif

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef struct {
  union {
    Tc  *pTc;
    Rtc *pRtc;
  };
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
  // Should never be called with timer RTC_TIMER_NUM
  Tc * const tc = TimerConfig[timer_num].pTc;
  tc->COUNT32.CC[0].reg = compare;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  // Should never be called with timer RTC_TIMER_NUM
  Tc * const tc = TimerConfig[timer_num].pTc;
  return (hal_timer_t)tc->COUNT32.CC[0].reg;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  // Should never be called with timer RTC_TIMER_NUM
  Tc * const tc = TimerConfig[timer_num].pTc;
  tc->COUNT32.CTRLBSET.reg = TC_CTRLBCLR_CMD_READSYNC;
  SYNC(tc->COUNT32.SYNCBUSY.bit.CTRLB || tc->COUNT32.SYNCBUSY.bit.COUNT);
  return tc->COUNT32.COUNT.reg;
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  if (timer_num == RTC_TIMER_NUM) {
    Rtc * const rtc = TimerConfig[timer_num].pRtc;
    // Clear interrupt flag
    rtc->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;
  }
  else {
    Tc * const tc = TimerConfig[timer_num].pTc;
    // Clear interrupt flag
    tc->COUNT32.INTFLAG.reg = TC_INTFLAG_OVF;
  }
}

#define HAL_timer_isr_epilogue(timer_num)
