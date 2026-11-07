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
 * Pins Debugging for AT32
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#include "../../inc/MarlinConfig.h"
#include "fastio.h"

// AT32 pin debugging uses the Arduino PIN_MAP infrastructure
#define NUM_DIGITAL_PINS  PIN_MAX

#ifndef BOARD_USART1_TX_PIN
  #define BOARD_USART1_TX_PIN PA9
#endif
#ifndef BOARD_USART1_RX_PIN
  #define BOARD_USART1_RX_PIN PA10
#endif

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = PIN_MAX;
  static constexpr uint8_t name_pad = 21;   // Space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return IS_PIN(pin); }

  //
  // Pins that will cause a hang / reset / disconnect in M43 Toggle and Watch utils
  //
  static bool never_touch(const uint16_t index) {
    const pin_t pin = pin_t(index);
    return pin == BOARD_USART1_TX_PIN || pin == BOARD_USART1_RX_PIN     // Host USART
        || pin == PD0 || pin == PD1 || pin == PC14 || pin == PC15;      // Oscillators (HEXT, LEXT)
  }

  // The core's analogRead() takes a pin, not an ADC channel, so the "analog index"
  // passed to it by the shared code is the pin itself. print_analog shows the channel.
  static int16_t analog_index(const pin_t pin) { return IS_ADC_PIN(pin) ? int16_t(pin) : -1; }

  static bool is_analog(const pin_t pin) { return IS_ADC_PIN(pin) && IS_ANALOG(pin) && !never_touch(pin); }

  static bool is_output(const pin_t pin) { return is_valid(pin) && IS_OUTPUT(pin); }

  static bool is_digital_at(const uint16_t index) { return !is_analog(getPinByIndex(index)); }

  // Timer number for a PIN_MAP timer, or 0 if unknown
  static uint8_t timer_number(const tmr_type * const tmr) {
    static tmr_type * const timers[] = { TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8, TMR9, TMR10, TMR11, TMR12, TMR13, TMR14 };
    for (uint8_t i = 0; i < COUNT(timers); ++i) if (tmr == timers[i]) return i + 1;
    return 0;
  }

  // Compare value of the pin's timer channel (1-4)
  static uint32_t channel_value(const tmr_type * const tmr, const uint8_t ch) {
    switch (ch) {
      case 1: return tmr->c1dt;
      case 2: return tmr->c2dt;
      case 3: return tmr->c3dt;
      default: return tmr->c4dt;
    }
  }

  // A pin is PWM output if it's routed to its timer (mux output),
  // the timer is running, and the timer channel output is enabled.
  static bool pwm_active(const pin_t pin) {
    if (!IS_PWM_PIN(pin) || !IS_MUX(pin)) return false;
    const tmr_type * const tmr = PIN_MAP[pin].TIMx;
    const uint8_t ch = PIN_MAP[pin].TimerChannel;
    return tmr->ctrl1_bit.tmren && TEST(tmr->cctrl, (ch - 1) * 4);
  }

  // Print the pin's timer and channel, e.g. "TMR3 CH2", plus the duty if PWM is active
  static void print_pwm(const pin_t pin) {
    if (!IS_PWM_PIN(pin)) return;
    const tmr_type * const tmr = PIN_MAP[pin].TIMx;
    const uint8_t ch = PIN_MAP[pin].TimerChannel;
    SERIAL_ECHO(F("TMR"), timer_number(tmr), F(" CH"), ch);
    if (pwm_active(pin)) SERIAL_ECHO(F(" PWM "), channel_value(tmr, ch), C('/'), tmr->pr + 1);
  }

  static void print_port(const pin_t pin) {
    const char port = 'A' + char(pin >> 4);
    const uint8_t gbit = pin & 0x0F;
    char buffer[8];
    sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
    if (gbit < 10) SERIAL_CHAR(' ');
    SERIAL_ECHO(buffer);
  }

  static void print_number(const pin_t pin) {
    char buffer[16];
    sprintf_P(buffer, PSTR("%3hd "), int16_t(pin));
    SERIAL_ECHO(buffer);
  }

  static void print_analog(const pin_t pin) {
    char buffer[16];
    sprintf_P(buffer, PSTR(" (A%2d)  "), int(PIN_MAP[pin].ADC_Channel));
    SERIAL_ECHO(buffer);
  }
};
