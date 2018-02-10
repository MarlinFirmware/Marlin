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

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "HAL_Servo_Stm32f1.h"

int8_t libServo::attach(const int pin) {
  if (this->servoIndex >= MAX_SERVOS) return -1;
  return Servo::attach(pin);
}

int8_t libServo::attach(const int pin, const int min, const int max) {
  return Servo::attach(pin, min, max);
}

void libServo::move(const int value) {
  if (this->attach(0) >= 0) {
    this->write(value);
    delay(SERVO_DELAY);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      this->detach();
    #endif
  }
}
#endif // HAS_SERVOS

#endif // __STM32F1__
