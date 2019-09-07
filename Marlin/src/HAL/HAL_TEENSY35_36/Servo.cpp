/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#if defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.5/3.6

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

uint8_t servoPin[MAX_SERVOS] = { 0 };

int8_t libServo::attach(const int pin) {
  if (this->servoIndex >= MAX_SERVOS) return -1;
  if (pin > 0) servoPin[this->servoIndex] = pin;
  return super::attach(servoPin[this->servoIndex]);
}

int8_t libServo::attach(const int pin, const int min, const int max) {
  if (pin > 0) servoPin[this->servoIndex] = pin;
  #ifndef SIMPLE_REATTACH
    inputmin = min;
    inputmax = max;
  #endif
  return super::attach(servoPin[this->servoIndex], min, max);
}

#ifndef SIMPLE_REATTACH
  int8_t libServo::reattach() {
    // Not sure about the "native" implementation of the Teensy servo lib,
    // so "emulate" the reattach by attaching with the original arguments.
    return super::attach(servoPin[this->servoIndex], inputmin, inputmax);
  }
#endif

void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (this->reattach() >= 0) {
    this->write(value);
    safe_delay(servo_delay[this->servoIndex]);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      this->detach();
    #endif
  }
}

#endif // HAS_SERVOS

#endif // __MK64FX512__ || __MK66FX1M0__
