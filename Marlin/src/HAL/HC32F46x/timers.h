/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL for stm32duino.com based on Libmaple and compatible (HC32F46x based on STM32F1)
 */

#include <stdint.h>
#include "../board/startup.h"
#include "../../core/boards.h"

// ------------------------
// Defines
// ------------------------

/**
 * TODO: Check and confirm what timer we will use for each Temps and stepper driving.
 * We should probable drive temps with PWM.
 */
#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFF

#define HAL_TIMER_RATE uint32_t(F_CPU) // frequency of timers peripherals

/**
 * Note: Timers may be used by platforms and libraries
 *
 * FAN PWMs:
 *   With FAN_SOFT_PWM disabled the Temperature class uses
 *   FANx_PIN timers to generate FAN PWM signals.
 *
 * Speaker:
 *   When SPEAKER is enabled, one timer is allocated by maple/tone.cpp.
 *   - If BEEPER_PIN has a timer channel (and USE_PIN_TIMER is
 *     defined in tone.cpp) it uses the pin's own timer.
 *   - Otherwise it uses Timer 8 on boards with STM32_HIGH_DENSITY
 *     or Timer 4 on other boards.
 */
#define STEP_TIMER_NUM 2 // TIMER0 CHB
#define TEMP_TIMER_NUM 1 // TIMER0 CHA
#define PULSE_TIMER_NUM STEP_TIMER_NUM

// timer aliases
#define MF_TIMER_STEP STEP_TIMER_NUM
#define MF_TIMER_TEMP TEMP_TIMER_NUM
#define MF_TIMER_PULSE PULSE_TIMER_NUM

#define TEMP_TIMER_FREQUENCY 1000 // temperature interrupt frequency
#define TEMP_TIMER_PRESCALE 16ul  // prescaler for setting Temp timer, 72Khz

#define STEPPER_TIMER_PRESCALE 16ul                                  // prescaler for setting stepper timer
#define STEPPER_TIMER_RATE (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE) // frequency of stepper timer  84/16Mhz = 5250000M
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000)  // stepper timer ticks per µs

#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT() timer_enable_irq(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() timer_disable_irq(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() timer_enable_irq(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() timer_disable_irq(TEMP_TIMER_NUM)

#define HAL_timer_get_count(timer_num) timer_get_count(timer_num)

// ------------------------
// Public Variables
// ------------------------

// static HardwareTimer StepperTimer(STEP_TIMER_NUM);
// static HardwareTimer TempTimer(TEMP_TIMER_NUM);

// ------------------------
// Public functions
// ------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);
void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

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

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare);

#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)
#define TIMER_OC_NO_PRELOAD 0 // Need to disable preload also on compare registers.
