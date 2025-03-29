/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// Fast I/O interfaces for GD32F303RE

#include <GPIO.hpp>
#include <PinOps.hpp>
#include <PinOpsMap.hpp>

static inline void fast_write_pin_wrapper(pin_size_t IO, bool V) {
  if (V) gpio::fast_set_pin(getPortFromPin(IO), getPinInPort(IO));
  else gpio::fast_clear_pin(getPortFromPin(IO), getPinInPort(IO));
}

static inline bool fast_read_pin_wrapper(pin_size_t IO) {
  return gpio::fast_read_pin(getPortFromPin(IO), getPinInPort(IO));
}

static inline void fast_toggle_pin_wrapper(pin_size_t IO) {
  gpio::fast_toggle_pin(getPortFromPin(IO), getPinInPort(IO));
}

// ------------------------
// Defines
// ------------------------

#ifndef PWM
  #define PWM OUTPUT
#endif

#define _WRITE(IO, V)           fast_write_pin_wrapper(IO, V)
#define _READ(IO)               fast_read_pin_wrapper(IO)
#define _TOGGLE(IO)             fast_toggle_pin_wrapper(IO)

#define _GET_MODE(IO)
#define _SET_MODE(IO, M)        pinMode((IO), (M))
#define _SET_OUTPUT(IO)         pinMode((IO), OUTPUT)
#define _SET_OUTPUT_OD(IO)      pinMode((IO), OUTPUT_OPEN_DRAIN)

#define WRITE(IO, V)            _WRITE((IO), (V))
#define READ(IO)                _READ(IO)
#define TOGGLE(IO)              _TOGGLE(IO)

#define OUT_WRITE(IO, V)        do { _SET_OUTPUT(IO); WRITE((IO), (V)); } while (0)
#define OUT_WRITE_OD(IO, V)     do { _SET_OUTPUT_OD(IO); WRITE((IO), (V)); } while (0)

#define SET_INPUT(IO)           _SET_MODE((IO), INPUT)
#define SET_INPUT_PULLUP(IO)    _SET_MODE((IO), INPUT_PULLUP)
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE((IO), INPUT_PULLDOWN)
#define SET_OUTPUT(IO)          OUT_WRITE((IO), LOW)
#define SET_OUTPUT_OD(IO)       OUT_WRITE_OD((IO), LOW)
#define SET_PWM(IO)             _SET_MODE((IO), PWM)

#define IS_INPUT(IO)
#define IS_OUTPUT(IO)

#define PWM_PIN(P)              isPinInPinOps(TIMER_PinOps, P)
#define NO_COMPILE_TIME_PWM

// Wrappers for digitalRead and digitalWrite
#define extDigitalRead(IO)      digitalRead(IO)
#define extDigitalWrite(IO, V)  digitalWrite((IO), (V))
