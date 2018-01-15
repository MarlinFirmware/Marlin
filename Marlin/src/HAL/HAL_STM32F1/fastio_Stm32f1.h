/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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
 * Fast I/O interfaces for STM32F1
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */

#ifndef _FASTIO_STM32F1_H
#define _FASTIO_STM32F1_H

#include <libmaple/gpio.h>

#define READ(IO)              (PIN_MAP[IO].gpio_device->regs->IDR & (1U << PIN_MAP[IO].gpio_bit) ? HIGH : LOW)
#define WRITE(IO, v)          (PIN_MAP[IO].gpio_device->regs->BSRR = (1U << PIN_MAP[IO].gpio_bit) << (16 * !(bool)v))
#define TOGGLE(IO)            (PIN_MAP[IO].gpio_device->regs->ODR = PIN_MAP[IO].gpio_device->regs->ODR ^ (1U << PIN_MAP[IO].gpio_bit))
#define WRITE_VAR(IO, v)      WRITE(io, v)

#define _GET_MODE(IO)         (gpio_get_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit))
#define _SET_MODE(IO,M)       do{ gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, M); } while (0)
#define _SET_OUTPUT(IO)       _SET_MODE(IO, GPIO_OUTPUT_PP)

#define SET_INPUT(IO)         _SET_MODE(IO, GPIO_INPUT_FLOATING)
#define SET_INPUT_PULLUP(IO)  _SET_MODE(IO, GPIO_INPUT_PU)
#define SET_OUTPUT(IO)        do{ _SET_OUTPUT(IO); WRITE(IO, LOW); }while(0)

#define GET_INPUT(IO)         (_GET_MODE(IO) == GPIO_INPUT_FLOATING || _GET_MODE(IO) == GPIO_INPUT_ANALOG || _GET_MODE(IO) == GPIO_INPUT_PU || _GET_MODE(IO) == GPIO_INPUT_PD)
#define GET_OUTPUT(IO)        (_GET_MODE(IO) == GPIO_OUTPUT_PP)
#define GET_TIMER(IO)         (PIN_MAP[IO].timer_device != NULL)

#define OUT_WRITE(IO, v)      { _SET_OUTPUT(IO); WRITE(IO, v); }
/**
 * TODO: Write a macro to test if PIN is PWM or not.
 */
#define PWM_PIN(p)            true

#endif // _FASTIO_STM32F1_H
