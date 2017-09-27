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
This code contributed by Triffid_Hunter and modified by Kliment
why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
*/

/**
* Description: Fast IO functions for Arduino Due and compatible (SAM3X8E)
*
* For ARDUINO_ARCH_SAM
*/

#ifndef	_FASTIO_STM32F1_H
#define	_FASTIO_STM32F1_H

#include <libmaple/gpio.h>

/**
utility functions
*/

/**
magic I/O routines
now you can simply SET_OUTPUT(STEP); WRITE(STEP, 1); WRITE(STEP, 0);
*/

/// Read a pin
#define READ(IO) ((bool)(gpio_read_bit(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit) ? HIGH : LOW))
/// Write to a pin
#define WRITE_VAR(IO, v) do { gpio_write_bit(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, v); } while (0)
#define WRITE(IO, v)    do { gpio_write_bit(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, v); } while (0)
/// toggle a pin
#define TOGGLE(IO)      do {gpio_toggle_bit(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit); } while (0)

/// set pin as input
#define SET_INPUT(IO)   do {gpio_set_mode(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit, GPIO_INPUT_FLOATING); } while (0)
/// set pin as output
#define _SET_OUTPUT(IO)  do {gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, GPIO_OUTPUT_PP); } while (0)    
/// set pin as input with pullup mode
#define SET_INPUT_PULLUP(IO)  do {gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, GPIO_INPUT_PU); } while (0)

/* Todo Check if these are ever used or needed
*/    
/// check if pin is an input
#define _GET_INPUT(IO)
/// check if pin is an output
#define _GET_OUTPUT(IO)
/// check if pin is an timer
#define _GET_TIMER(IO)

//  why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html

/// set pin as output wrapper
#define SET_OUTPUT(IO)  do{ _SET_OUTPUT(IO); WRITE(IO, LOW); }while(0)

/// check if pin is an input wrapper
#define GET_INPUT(IO)  _GET_INPUT(IO) // todo: Never used?

/// check if pin is an output wrapper
#define GET_OUTPUT(IO)  (gpio_get_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit) == GPIO_OUTPUT_PP ? TRUE : FALSE)

/// check if pin is an timer 
#define GET_TIMER(IO) (PIN_MAP[IO].timer_device != NULL ? TRUE : FALSE )

// Shorthand
#define OUT_WRITE(IO, v) { _SET_OUTPUT(IO); WRITE(IO, v); }

/**
ports and functions
Are these needed?
*/
/*
// UART
#define RXD        DIO0
#define TXD        DIO1

// TWI (I2C)
#define SCL        DIO21
#define SDA        DIO20
*/

/**
pins
*/



#endif	/* _FASTIO_STM32F1_H */
