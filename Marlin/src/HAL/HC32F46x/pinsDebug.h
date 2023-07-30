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
#include <drivers/timera/timera_pwm.h>

//
// Translation of routines & variables used by pinsDebug.h
//
#ifndef BOARD_NR_GPIO_PINS
#error "Expected BOARD_NR_GPIO_PINS not found"
#endif

#define NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS
#define NUMBER_PINS_TOTAL BOARD_NR_GPIO_PINS
#define VALID_PIN(pin) IS_GPIO_PIN(pin)
#define GET_ARRAY_PIN(p) pin_t(pin_array[p].pin)
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
// do not touch any of the following pins:
// - host serial pins, and
// - pins that could be connected to oscillators (see datasheet, Table 2.1):
//   - XTAL = PH0, PH1
//   - XTAL32 = PC14, PC15
#define IS_HOST_USART_PIN(Q) (Q == BOARD_USART2_TX_PIN || Q == BOARD_USART2_RX_PIN)
#define IS_OSC_PIN(Q) (Q == PH0 || Q == PH1 || Q == PC14 || Q == PC15)

#define M43_NEVER_TOUCH(Q) ( \
    IS_HOST_USART_PIN(Q) || IS_OSC_PIN(Q))
#endif

// static int8_t get_pin_mode(pin_t pin) {
//   return VALID_PIN(pin) ? _GET_MODE(pin) : -1;
// }

static pin_t DIGITAL_PIN_TO_ANALOG_PIN(pin_t pin)
{
    if (!VALID_PIN(pin))
        return -1;
    int8_t adc_channel = int8_t(PIN_MAP[pin].adc_info.channel);
    return pin_t(adc_channel);
}

static bool IS_ANALOG(pin_t pin)
{
    if (!VALID_PIN(pin))
        return false;

    if (PIN_MAP[pin].adc_info.channel != ADC_PIN_INVALID)
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

/**
 * @brief print pin PWM status
 * @return true if pin is currently a PWM pin, false otherwise
 */
static bool pwm_status(const pin_t pin)
{
    // get timer assignment for pin
    timera_config_t *unit;
    en_timera_channel_t channel;
    en_port_func_t port_function;
    if (!timera_get_assignment(pin, unit, channel, port_function) || unit == NULL)
    {
        // no pwm pin or no unit assigned
        return false;
    }

    // a pin that is PWM output is:
    // - assigned to a timerA unit (tested above)
    // - unit is initialized
    // - channel is active
    // - GPIO function is set to assigned port_function (cannot test this)
    // TODO: check GPIO function is set to assigned port_function
    return timera_is_unit_initialized(unit) && timera_is_channel_active(unit, channel);
}

static void pwm_details(const pin_t pin)
{
    // get timer assignment for pin
    timera_config_t *unit;
    en_timera_channel_t channel;
    en_port_func_t port_function;
    if (!timera_get_assignment(pin, unit, channel, port_function) || unit == NULL)
    {
        // no pwm pin or no unit assigned
        return;
    }

    // print timer assignment of pin, eg. "TimerA1Ch2 Func4"
    SERIAL_ECHOPGM("TimerA");
    SERIAL_ECHO(TIMERA_REG_TO_X(unit->peripheral.register_base));
    SERIAL_ECHOPGM("Ch");
    SERIAL_ECHO(TIMERA_CHANNEL_TO_X(channel));
    SERIAL_ECHOPGM(" Func");
    SERIAL_ECHO(int(port_function));

    SERIAL_ECHO_SP(3); // 3 spaces

    // print timer unit state, eg. "1/16 PERAR=1234" OR "N/A"
    if (timera_is_unit_initialized(unit))
    {
        // unit initialized, print
        // - timer clock divider
        // - timer period value (PERAR)
        const uint8_t clock_divider = timera_clk_div_to_n(unit->state.base_init->enClkDiv);
        const uint16_t period = TIMERA_GetPeriodValue(unit->peripheral.register_base);
        SERIAL_ECHOPGM("1/");
        SERIAL_ECHO(clock_divider);
        SERIAL_ECHOPGM(" PERAR=");
        SERIAL_ECHO(period);
    }
    else
    {
        // unit not initialized
        SERIAL_ECHOPGM("N/A");
        return;
    }

    SERIAL_ECHO_SP(3); // 3 spaces

    // print timer channel state, e.g. "CMPAR=1234" OR "N/A"
    if (timera_is_channel_active(unit, channel))
    {
        // channel active, print
        // - channel compare value
        const uint16_t compare = TIMERA_GetCompareValue(unit->peripheral.register_base, channel);
        SERIAL_ECHOPGM("CMPAR=");
        SERIAL_ECHO(compare);
    }
    else
    {
        // channel inactive
        SERIAL_ECHOPGM("N/A");
    }
}

static void print_port(pin_t pin)
{
    const char port = 'A' + char(pin >> 4); // pin div 16
    const int16_t gbit = PIN_MAP[pin].bit_pos;
    char buffer[8];
    sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
    if (gbit < 10)
        SERIAL_CHAR(' ');
    SERIAL_ECHO(buffer);
}
