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
#include <Timer0.h>

//
// Timer Types
//
typedef Timer0Channel timer_channel_t;
typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFF

//
// Timer Configurations
//
#define HAL_TIMER_RATE uint32_t(SYSTEM_CLOCK_FREQUENCIES.pclk1)

// temperature timer (Timer0 Unit1 Channel A)
#define TEMP_TIMER_NUM Timer0Channel::A
#define TEMP_TIMER_PRIORITY DDL_IRQ_PRIORITY_02
#define TEMP_TIMER_RATE 1000 // 1kHz
#define TEMP_TIMER_PRESCALE 16
#define TEMP_TIMER_FREQUENCY TEMP_TIMER_RATE // alias for Marlin

// stepper timer (Timer0 Unit1 Channel B)
#define STEP_TIMER_NUM Timer0Channel::B
#define STEP_TIMER_PRIORITY DDL_IRQ_PRIORITY_01
#define STEPPER_TIMER_RATE 2000000 // 2MHz
#define STEPPER_TIMER_PRESCALE 16
#define STEPPER_TIMER_TICKS_PER_US (STEPPER_TIMER_RATE / 1000000)

// pulse timer (== stepper timer)
#define PULSE_TIMER_NUM STEP_TIMER_NUM
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

//
// channel aliases
//
#define MF_TIMER_TEMP TEMP_TIMER_NUM
#define MF_TIMER_STEP STEP_TIMER_NUM
#define MF_TIMER_PULSE PULSE_TIMER_NUM

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

//
// HAL function aliases
//
#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM);

//
// HAL ISR callbacks
//
void Step_Handler();
void Temp_Handler();

#ifndef HAL_STEP_TIMER_ISR
#define HAL_STEP_TIMER_ISR() void Step_Handler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
#define HAL_TEMP_TIMER_ISR() void Temp_Handler()
#endif

// do not need prologue/epilogue 
#define HAL_timer_isr_prologue(timer_num)
#define HAL_timer_isr_epilogue(timer_num)
