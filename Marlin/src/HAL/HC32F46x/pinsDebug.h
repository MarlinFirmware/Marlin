/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "fastio.h"
#include "../../inc/MarlinConfig.h"

// allow pins with potentially unsafe FuncSel
// #define ALLOW_UNSAFE_FUNCTION_PINS

//
// Translation of routines & variables used by pinsDebug.h
//
#ifndef BOARD_NR_GPIO_PINS
#error "Expected BOARD_NR_GPIO_PINS not found"
#endif

#define NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS
#define NUMBER_PINS_TOTAL BOARD_NR_GPIO_PINS
#define VALID_PIN(pin) (pin >= 0 && pin < BOARD_NR_GPIO_PINS)
#define GET_ARRAY_PIN(p) pin_t(pin_array[p].pin)
#define pwm_status(pin) PWM_PIN(pin)
#define digitalRead_mod(p) extDigitalRead(p)
#define PRINT_PIN(p)                                  \
    do                                                \
    {                                                 \
        sprintf_P(buffer, PSTR("%3hd "), int16_t(p)); \
        SERIAL_ECHO(buffer);                          \
    } while (0)
#define PRINT_PIN_ANALOG(p)                                                   \
    do                                                                        \
    {                                                                         \
        sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin)); \
        SERIAL_ECHO(buffer);                                                  \
    } while (0)
#define PRINT_PORT(p) print_port(p)
#define PRINT_ARRAY_NAME(x)                                                              \
    do                                                                                   \
    {                                                                                    \
        sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); \
        SERIAL_ECHO(buffer);                                                             \
    } while (0)
#define MULTI_NAME_PAD 21 // space needed to be pretty if not first name assigned to a pin

//
// pins that will cause a hang / reset / disconnect in M43 Toggle and Watch utils
//
#ifndef M43_NEVER_TOUCH
// do not touch any pin not defined as gpio or sdio function,
// (explicitly) host serial pins, and
// pins related to the oscillator
#define IS_HOST_USART_PIN(Q) (Q == BOARD_USART2_TX_PIN || Q == BOARD_USART2_RX_PIN)
#define IS_OSC_PIN(Q) (Q == PH0 || Q == PH1 || Q == PH2)
#define IS_PIN_FUNC(Q, FUNC) (PIN_MAP[Q].function == en_port_func_t::Func_##FUNC)

#ifndef ALLOW_UNSAFE_FUNCTION_PINS
#define IS_SAFE_PIN_FUNC(Q) (IS_PIN_FUNC(Q, Gpio) || IS_PIN_FUNC(Q, Sdio))
#else
#define IS_SAFE_PIN_FUNC(Q) (true)
#endif

#define M43_NEVER_TOUCH(Q) ( \
    !IS_SAFE_PIN_FUNC(Q) || IS_HOST_USART_PIN(Q) || IS_OSC_PIN(Q))
#endif

// static int8_t get_pin_mode(pin_t pin) {
//   return VALID_PIN(pin) ? _GET_MODE(pin) : -1;
// }

static pin_t DIGITAL_PIN_TO_ANALOG_PIN(pin_t pin)
{
    if (!VALID_PIN(pin))
        return -1;
    int8_t adc_channel = int8_t(PIN_MAP[pin].adc_channel);
    return pin_t(adc_channel);
}

static bool IS_ANALOG(pin_t pin)
{
    if (!VALID_PIN(pin))
        return false;
    if (PIN_MAP[pin].adc_channel != ADC_PIN_INVALID)
    {
        return _GET_MODE(pin) == INPUT_ANALOG && !M43_NEVER_TOUCH(pin);
    }

    return false;
}

static bool GET_PINMODE(const pin_t pin)
{
    return VALID_PIN(pin) && !IS_INPUT(pin);
}

static bool GET_ARRAY_IS_DIGITAL(const int16_t array_pin)
{
    const pin_t pin = GET_ARRAY_PIN(array_pin);
    return (!IS_ANALOG(pin));
}

static void pwm_details(const pin_t pin)
{
    // TODO stub
}

static void print_port(pin_t pin)
{
    const char port = 'A' + char(pin >> 4); // pin div 16
    const int16_t gbit = PIN_MAP[pin].gpio_bit;
    char buffer[8];
    sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
    if (gbit < 10)
        SERIAL_CHAR(' ');
    SERIAL_ECHO(buffer);
}
