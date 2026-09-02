/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

/**
 * Fast I/O interfaces for AT32F403
 * The AT32 Arduino core provides digitalRead/digitalWrite/togglePin helpers.
 *
 * AT32F403A GPIO registers (vs STM32):
 *   gpio_x->scr   - Set bits      (vs STM32 GPIOx->BSRR)
 *   gpio_x->clr   - Clear bits    (vs STM32 GPIOx->BRR)
 *   gpio_x->odt   - Output data   (vs STM32 GPIOx->ODR)
 *   gpio_x->idt   - Input data    (vs STM32 GPIOx->IDR)
 *   gpio_x->cfglr - Config pins 0-7  (vs STM32 GPIOx->CRL)
 *   gpio_x->cfghr - Config pins 8-15 (vs STM32 GPIOx->CRH)
 *
 * Reference: CrealityOfficial/K2_Series_Klipper src/at32/gpio.c
 */
// Map Marlin pin-mode names to the core's PinMode_TypeDef
#ifndef INPUT_FLOATING
  #define INPUT_FLOATING INPUT
#endif
#ifndef OUTPUT_PWM
  #define OUTPUT_PWM PWM
#endif

#define READ(IO)            digitalRead(IO)
#define WRITE(IO, V)        digitalWrite(IO, V)
#define TOGGLE(IO)          togglePin(IO)

// The AT32 Arduino core declares pinMode(pin, PinMode_TypeDef) with a strict
// enum, so callers passing a bool / int (as Marlin's M43 and shared code do)
// fail to compile. Wrap it to accept any integral mode and map it to OUTPUT/
// INPUT via the framework's own enum.
inline void pinMode(uint8_t pin, int mode) {
  pinMode(pin, mode ? OUTPUT : INPUT);
}

#define _SET_MODE(IO, M)    pinMode(IO, (PinMode_TypeDef)(M))
#define _SET_OUTPUT(IO)     _SET_MODE(IO, OUTPUT)

#define OUT_WRITE(IO, V)    do { _SET_OUTPUT(IO); WRITE(IO, V); } while (0)

#define SET_INPUT(IO)       _SET_MODE(IO, INPUT_FLOATING)
#define SET_INPUT_PULLUP(IO) _SET_MODE(IO, INPUT_PULLUP)
#define SET_INPUT_PULLDOWN(IO) _SET_MODE(IO, INPUT_PULLDOWN)
#define SET_OUTPUT(IO)      OUT_WRITE(IO, LOW)
#define SET_PWM(IO)         _SET_MODE(IO, OUTPUT_PWM)

// Mode query is not cached by the core; report conservatively.
#define _GET_MODE(IO)       (0)
#define IS_INPUT(IO)        (true)
#define IS_OUTPUT(IO)       (true)

#define PWM_PIN(IO)         IS_PWM_PIN(IO)

#define extDigitalRead(IO)  digitalRead(IO)
#define extDigitalWrite(IO, V) digitalWrite(IO, V)

#define NO_COMPILE_TIME_PWM   // Can't check for PWM at compile time
