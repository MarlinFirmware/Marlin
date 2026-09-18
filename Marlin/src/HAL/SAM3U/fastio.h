/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * Fast I/O for Atmel SAM3U (AT91SAM3U4E)
 *
 * Pins are numbered (port * 32 + bit), so every macro below resolves to a
 * constant PIO base pointer and a constant bit mask at compile time, and each
 * READ/WRITE collapses to a single load or store. See include/pinmapping.h.
 */

#include "include/pinmapping.h"
#include "clock.h"

#include "../../inc/MarlinConfigPre.h"

#ifndef MASK
  #define MASK(PIN) _BV(PIN)
#endif

/**
 * The SAM3U's four PWM channels do not cover the 4pi's heater and FET
 * outputs, so this HAL drives them with Marlin's software PWM. No pin is
 * reported as hardware-PWM capable.
 */
#define PWM_PIN(P) false

// ------------------------
// Primitives
// ------------------------

// Read a pin
#define _READ(P) bool(SAM3U_PIO(P)->PIO_PDSR & SAM3U_MASK(P))

// Write to a pin
#define _WRITE(P,V) do{   Pio * const _port = SAM3U_PIO(P);   const uint32_t _mask = SAM3U_MASK(P);   if (V) _port->PIO_SODR = _mask; else _port->PIO_CODR = _mask; }while(0)

// Toggle a pin
#define _TOGGLE(P) _WRITE(P, !_READ(P))

// Set pin as input
#define _SET_INPUT(P) do{   Pio * const _port = SAM3U_PIO(P);   const uint32_t _mask = SAM3U_MASK(P);   pmc_enable_periph_clk(SAM3U_PIO_ID(P));   _port->PIO_IDR  = _mask;   _port->PIO_PUDR = _mask;   _port->PIO_ODR  = _mask;   _port->PIO_PER  = _mask; }while(0)

// Set pin as output. Preset the output latch from the pin's current level
// first, so switching direction does not glitch whatever is attached.
#define _SET_OUTPUT(P) do{   Pio * const _port = SAM3U_PIO(P);   const uint32_t _mask = SAM3U_MASK(P);   pmc_enable_periph_clk(SAM3U_PIO_ID(P));   if (_port->PIO_PDSR & _mask) _port->PIO_SODR = _mask; else _port->PIO_CODR = _mask;   _port->PIO_IDR  = _mask;   _port->PIO_PUDR = _mask;   _port->PIO_MDDR = _mask;   _port->PIO_PER  = _mask;   _port->PIO_OER  = _mask; }while(0)

// Set pin as input with (or without) its pull-up
#define _PULLUP(P,V) do{   Pio * const _port = SAM3U_PIO(P);   const uint32_t _mask = SAM3U_MASK(P);   if (V) _port->PIO_PUER = _mask; else _port->PIO_PUDR = _mask; }while(0)

// ------------------------
// Marlin fastio API
// ------------------------

#define READ(IO)              _READ(IO)
#define WRITE(IO,V)           _WRITE(IO,V)
#define TOGGLE(IO)            _TOGGLE(IO)

#define SET_INPUT(IO)         _SET_INPUT(IO)
#define SET_INPUT_PULLUP(IO)  do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)

// The SAM3U PIO has no pull-downs, so this can only be a plain input.
// inc/SanityCheck.h rejects configurations that actually depend on one.
#define SET_INPUT_PULLDOWN    SET_INPUT

#define SET_OUTPUT(IO)        _SET_OUTPUT(IO)
#define SET_PWM               SET_OUTPUT

#define IS_INPUT(IO)          ((SAM3U_PIO(IO)->PIO_OSR & SAM3U_MASK(IO)) == 0)
#define IS_OUTPUT(IO)         ((SAM3U_PIO(IO)->PIO_OSR & SAM3U_MASK(IO)) != 0)

#define OUT_WRITE(IO,V)       do{ SET_OUTPUT(IO); WRITE(IO,V); }while(0)

#define extDigitalRead(IO)    digitalRead(IO)
#define extDigitalWrite(IO,V) digitalWrite(IO,V)
