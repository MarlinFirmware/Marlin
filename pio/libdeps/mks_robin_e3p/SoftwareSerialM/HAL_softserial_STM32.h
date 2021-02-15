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

#pragma once

#define OVERSAMPLE 3
#define INTERRUPT_PRIORITY 0

#define gpio_set(IO,V)  digitalWrite(IO, V)
#define gpio_get(IO)    digitalRead(IO)

#define cli() __disable_irq()
#define sei() __enable_irq()

#define HAL_softserial_timer_isr_prologue()
#define HAL_softserial_timer_isr_epilogue()

#define HAL_SOFTSERIAL_TIMER_ISR() extern "C" void SoftSerial_Handler(stimer_t *htim)

extern "C" void SoftSerial_Handler(stimer_t *htim);

