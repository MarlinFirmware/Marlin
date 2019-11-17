/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

uint8_t servoPin[MAX_SERVOS] = { 0 };

int8_t libServo::attach(const int pin) {
  if (servoIndex >= MAX_SERVOS) return -1;
  if (pin > 0) servoPin[servoIndex] = pin;
  return super::attach(servoPin[servoIndex]);
}

int8_t libServo::attach(const int pin, const int min, const int max) {
  if (pin > 0) servoPin[servoIndex] = pin;
  return super::attach(servoPin[servoIndex], min, max);
}

void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (attach(0) >= 0) {
    write(value);
    safe_delay(servo_delay[servoIndex]);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      detach();
    #endif
  }
}
#endif // HAS_SERVOS

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
