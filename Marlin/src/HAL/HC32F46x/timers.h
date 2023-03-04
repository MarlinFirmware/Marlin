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
#include <stdint.h>
#include "hc32f46x_timer0.h"

//
// Misc.
//
typedef en_tim0_channel_t timer_channel_t;
typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFF

// frequency of the timer peripheral
#define HAL_TIMER_RATE uint32_t(F_CPU)

//
// Timer Channels and Configuration
//
#define STEP_TIMER_NUM Tim0_ChannelB
#define TEMP_TIMER_NUM Tim0_ChannelA
#define PULSE_TIMER_NUM STEP_TIMER_NUM

// channel aliases
#define MF_TIMER_STEP STEP_TIMER_NUM
#define MF_TIMER_TEMP TEMP_TIMER_NUM
#define MF_TIMER_PULSE PULSE_TIMER_NUM

#define TEMP_TIMER_FREQUENCY 1000
#define TEMP_TIMER_PRESCALE 16ul

#define STEPPER_TIMER_PRESCALE 16ul
#define STEPPER_TIMER_RATE (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000)

#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

//
// HAL functions
//
void HAL_timer_start(const timer_channel_t timer_num, const uint32_t frequency);
void HAL_timer_enable_interrupt(const timer_channel_t timer_num);
void HAL_timer_disable_interrupt(const timer_channel_t timer_num);
bool HAL_timer_interrupt_enabled(const timer_channel_t timer_num);
void HAL_timer_set_compare(const timer_channel_t timer_num, const hal_timer_t compare);
hal_timer_t HAL_timer_get_count(const timer_channel_t timer_num);
void HAL_timer_isr_prologue(const timer_channel_t timer_num);
void HAL_timer_isr_epilogue(const timer_channel_t timer_num);

// ISR callbacks
extern void HAL_STEP_TIMER_ISR();
extern void HAL_TEMP_TIMER_ISR();

//
// HAL function aliases
//
#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM);
