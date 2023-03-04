/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Fast I/O interfaces for HC32F46x
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */
#include "io.h"

#define READ(IO) (PORT_GetBitGPIO(IO) ? HIGH : LOW)
#define WRITE(IO, V) (((V) > 0) ? PORT_SetBitsGPIO(IO) : PORT_ResetBitsGPIO(IO))
#define TOGGLE(IO) (PORT_ToggleGPIO(IO))

#define _GET_MODE(IO) gpio_get_mode(IO)
#define _SET_MODE(IO, M) gpio_set_mode(IO, M)
#define _SET_OUTPUT(IO) _SET_MODE(IO, OUTPUT)

#define OUT_WRITE(IO, V) \
  do                     \
  {                      \
    _SET_OUTPUT(IO);     \
    WRITE(IO, V);        \
  } while (0)

#define SET_INPUT(IO) _SET_MODE(IO, INPUT_FLOATING)
#define SET_INPUT_PULLUP(IO) _SET_MODE(IO, INPUT_PULLUP)
#define SET_INPUT_PULLDOWN(IO) _SET_MODE(IO, INPUT_PULLDOWN)
#define SET_OUTPUT(IO) OUT_WRITE(IO, LOW)

#define IS_INPUT(IO) (_GET_MODE(IO) == INPUT || _GET_MODE(IO) == INPUT_FLOATING || _GET_MODE(IO) == INPUT_ANALOG || _GET_MODE(IO) == INPUT_PULLUP ||  _GET_MODE(IO) == INPUT_PULLDOWN)
#define IS_OUTPUT(IO) (_GET_MODE(IO) == OUTPUT || _GET_MODE(IO) == OUTPUT_OPEN_DRAIN)

// TODO: #warning "PWM_PIN(IO) is not implemented"
#define PWM_PIN(IO) (false)

#define extDigitalRead(IO) digitalRead(IO)
#define extDigitalWrite(IO, V) digitalWrite(IO, V)
