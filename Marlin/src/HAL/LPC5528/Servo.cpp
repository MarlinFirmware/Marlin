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
#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"
#include "../../MarlinCore.h"

#include <pwm.h>

uint8_t libServo::servoCount = 0;

libServo::libServo() : servoIndex(servoCount < NUM_SERVOS ? servoCount++ : 255) {}

int8_t libServo::attach(const int inPin) {
  if (servoIndex >= NUM_SERVOS) return -1;

  if (inPin > 0) servo_pin = inPin;
  if (servo_pin <= 0) return -1;

  if (!pwm_pin_has_hardware(uint8_t(servo_pin))) return -1;

  // Attach at 0% so the pin idles low until write() sets a real pulse width,
  // then set the 20ms servo frame on whichever timer this pin belongs to.
  if (!is_attached) {
    if (!pwm_attach_pin(uint8_t(servo_pin), 0)) return -1;
    is_attached = true;
  }
  pwm_set_frequency(uint8_t(servo_pin), 1000000UL / SERVO_PERIOD_US);

  if (pulse_us) writeMicroseconds(pulse_us);  // Restore the last position on reattach

  return servoIndex;
}

int8_t libServo::attach(const int inPin, const int inMin, const int inMax) {
  min_us = inMin;
  max_us = inMax;
  return attach(inPin);
}

void libServo::detach() {
  if (!is_attached) return;
  pwm_detach_pin(uint8_t(servo_pin));
  is_attached = false;
}

void libServo::writeMicroseconds(int usec) {
  pulse_us = constrain(usec, min_us, max_us);
  if (is_attached) pwm_write_scaled(uint8_t(servo_pin), pulse_us, SERVO_PERIOD_US);
}

void libServo::write(int inDegrees) {
  degrees = constrain(inDegrees, 0, 180);
  // Scale 0..180 degrees onto the configured pulse width range. Done inline
  // because the Arduino core here provides no map().
  writeMicroseconds(min_us + int((int32_t(degrees) * (max_us - min_us)) / 180));
}

void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (attach(0) >= 0) {
    write(value);
    safe_delay(servo_delay[servoIndex]);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
  }
}

#endif // HAS_SERVOS
#endif // TARGET_LPC5528
