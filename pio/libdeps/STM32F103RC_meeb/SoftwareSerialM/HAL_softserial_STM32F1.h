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
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#pragma once

#include <HardwareTimer.h>

#define OVERSAMPLE 3
#define INTERRUPT_PRIORITY 0

#define gpio_set(IO,V) (PIN_MAP[IO].gpio_device->regs->BSRR = (1U << PIN_MAP[IO].gpio_bit) << ((V) ? 0 : 16))
#define gpio_get(IO) (PIN_MAP[IO].gpio_device->regs->IDR & (1U << PIN_MAP[IO].gpio_bit) ? HIGH : LOW)

#define cli() noInterrupts()  // Disable interrupts  
#define sei() interrupts()    // Enable interrupts

#define HAL_softserial_timer_isr_prologue()
#define HAL_softserial_timer_isr_epilogue()

#define HAL_SOFTSERIAL_TIMER_ISR() extern "C" void SoftSerial_Handler()

extern "C" void SoftSerial_Handler(void);
