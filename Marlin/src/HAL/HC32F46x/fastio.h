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
#include "../cores/io.h"
#include "../board/startup.h"

#define READ(IO) (PORT_GetBitMapp(IO) ? HIGH : LOW)
#define WRITE(IO, V) (((V) > 0) ? PORT_SetBitsMapp(IO) : PORT_ResetBitsMapp(IO))
#define TOGGLE(IO) (PORT_ToggleMapp(IO))

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

#define IS_INPUT(IO) (_GET_MODE(IO) == WiringPinMode::INPUT_FLOATING || _GET_MODE(IO) == WiringPinMode::INPUT_ANALOG || _GET_MODE(IO) == WiringPinMode::INPUT_PULLUP || _GET_MODE(IO) == WiringPinMode::INPUT_PULLDOWN)
#define IS_OUTPUT(IO) (_GET_MODE(IO) == WiringPinMode::OUTPUT || _GET_MODE(IO) == WiringPinMode::OUTPUT_OPEN_DRAIN)

#define PWM_PIN(IO) 0 //(PIN_MAP[IO].timer_device != nullptr)
// TODO: #warning "PWM_PIN(IO) is not implemented"

// digitalRead/Write wrappers
#define extDigitalRead(IO) digitalRead(IO)
#define extDigitalWrite(IO, V) digitalWrite(IO, V)

//
// Pins Definitions
//
#define PA0 0x00
#define PA1 0x01
#define PA2 0x02
#define PA3 0x03
#define PA4 0x04
#define PA5 0x05
#define PA6 0x06
#define PA7 0x07
#define PA8 0x08
#define PA9 0x09
#define PA10 0x0A
#define PA11 0x0B
#define PA12 0x0C
#define PA13 0x0D
#define PA14 0x0E
#define PA15 0x0F

#define PB0 0x10
#define PB1 0x11
#define PB2 0x12
#define PB3 0x13
#define PB4 0x14
#define PB5 0x15
#define PB6 0x16
#define PB7 0x17 // 36 pins (F103T)
#define PB8 0x18
#define PB9 0x19
#define PB10 0x1A
#define PB11 0x1B
#define PB12 0x1C
#define PB13 0x1D
#define PB14 0x1E
#define PB15 0x1F

#define PC0 0x20
#define PC1 0x21
#define PC2 0x22
#define PC3 0x23
#define PC4 0x24
#define PC5 0x25
#define PC6 0x26
#define PC7 0x27
#define PC8 0x28
#define PC9 0x29
#define PC10 0x2A
#define PC11 0x2B
#define PC12 0x2C
#define PC13 0x2D
#define PC14 0x2E
#define PC15 0x2F

#define PD0 0x30
#define PD1 0x31
#define PD2 0x32 // 64 pins (F103R)
#define PD3 0x33
#define PD4 0x34
#define PD5 0x35
#define PD6 0x36
#define PD7 0x37
#define PD8 0x38
#define PD9 0x39
#define PD10 0x3A
#define PD11 0x3B
#define PD12 0x3C
#define PD13 0x3D
#define PD14 0x3E
#define PD15 0x3F

#define PE0 0x40
#define PE1 0x41
#define PE2 0x42
#define PE3 0x43
#define PE4 0x44
#define PE5 0x45
#define PE6 0x46
#define PE7 0x47
#define PE8 0x48
#define PE9 0x49
#define PE10 0x4A
#define PE11 0x4B
#define PE12 0x4C
#define PE13 0x4D
#define PE14 0x4E
#define PE15 0x4F // 100 pins (F103V)

#define PH0 0x50
#define PH1 0x51
#define PH2 0x52
