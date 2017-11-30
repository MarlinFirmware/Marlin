/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifndef _HAL_TIMERS_STM32F1_H
#define _HAL_TIMERS_STM32F1_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

/**
 * TODO: Check and confirm what timer we will use for each Temps and stepper driving.
 * We should probable drive temps with PWM.
 */
#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFF

#define STEP_TIMER_NUM 5  // index of timer to use for stepper
#define STEP_TIMER_CHAN 1 // Channel of the timer to use for compare and interrupts
#define TEMP_TIMER_NUM 2  // index of timer to use for temperature
#define TEMP_TIMER_CHAN 1 // Channel of the timer to use for compare and interrupts


#define HAL_TIMER_RATE         (F_CPU)  // frequency of timers peripherals
#define STEPPER_TIMER_PRESCALE 36             // prescaler for setting stepper timer, 2Mhz
#define HAL_STEPPER_TIMER_RATE (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per us

#define TEMP_TIMER_PRESCALE     1000 // prescaler for setting Temp timer, 72Khz
#define TEMP_TIMER_FREQUENCY    1000 // temperature interrupt frequency

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt (STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt (STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt (TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt (TEMP_TIMER_NUM)

#define HAL_ENABLE_ISRs() do { if (thermalManager.in_temp_isr)DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)
// TODO change this


#define HAL_TEMP_TIMER_ISR extern "C" void tempTC_Handler(void)
#define HAL_STEP_TIMER_ISR extern "C" void stepTC_Handler(void)

extern "C" void tempTC_Handler(void);
extern "C" void stepTC_Handler(void);

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

static HardwareTimer StepperTimer(STEP_TIMER_NUM);
static HardwareTimer TempTimer(TEMP_TIMER_NUM);

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start (uint8_t timer_num, uint32_t frequency);
void HAL_timer_enable_interrupt(uint8_t timer_num);
void HAL_timer_disable_interrupt(uint8_t timer_num);

/**
 * NOTE: By default libmaple sets ARPE = 1, which means the Auto reload register is preloaded (will only update with an update event)
 * Thus we have to pause the timer, update the value, refresh, resume the timer.
 * That seems like a big waste of time and may be better to change the timer config to ARPE = 0, so ARR can be updated any time.
 * We are using a Channel in each timer in Capture/Compare mode. We could also instead use the Time Update Event Interrupt, but need to disable ARPE
 * so we can change the ARR value on the fly (without calling refresh), and not get an interrupt right there because we caused an UEV.
 * This mode pretty much makes 2 timers unusable for PWM since they have their counts updated all the time on ISRs.
 * The way Marlin manages timer interrupts doesn't make for an efficient usage in STM32F1
 * Todo: Look at that possibility later.
 */

static FORCE_INLINE void HAL_timer_set_count (uint8_t timer_num, uint32_t count) {
  switch (timer_num) {
  case STEP_TIMER_NUM:
    StepperTimer.pause();
    StepperTimer.setCompare (STEP_TIMER_CHAN, count);
    StepperTimer.refresh ();
    StepperTimer.resume ();
    break;
  case TEMP_TIMER_NUM:
    TempTimer.pause();
    TempTimer.setCompare (TEMP_TIMER_CHAN, count);
    TempTimer.refresh ();
    TempTimer.resume ();
    break;
  default:
    break;
  }
}

static FORCE_INLINE hal_timer_t HAL_timer_get_count (uint8_t timer_num) {
  hal_timer_t temp;
  switch (timer_num) {
  case STEP_TIMER_NUM:
    temp = StepperTimer.getCompare(STEP_TIMER_CHAN);
    break;
  case TEMP_TIMER_NUM:
    temp = TempTimer.getCompare(TEMP_TIMER_CHAN);
    break;
  default:
    temp = 0;
    break;
  }
  return temp;
}

static FORCE_INLINE hal_timer_t HAL_timer_get_current_count(uint8_t timer_num) {
  hal_timer_t temp;
  switch (timer_num) {
  case STEP_TIMER_NUM:
    temp = StepperTimer.getCount();
    break;
  case TEMP_TIMER_NUM:
    temp = TempTimer.getCount();
    break;
  default:
    temp = 0;
    break;
  }
  return temp;
}


//void HAL_timer_isr_prologue (uint8_t timer_num);

static FORCE_INLINE void HAL_timer_isr_prologue(uint8_t timer_num) {
  switch (timer_num) {
  case STEP_TIMER_NUM:
    StepperTimer.pause();
    StepperTimer.setCount(0);
    StepperTimer.refresh();
    StepperTimer.resume();
    break;
  case TEMP_TIMER_NUM:
    TempTimer.pause();
    TempTimer.setCount(0);
    TempTimer.refresh();
    TempTimer.resume();
    break;
  default:
    break;
  }
}

#endif // _HAL_TIMERS_STM32F1_H
