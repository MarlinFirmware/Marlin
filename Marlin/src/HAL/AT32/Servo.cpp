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
 *
 */

#ifdef ARDUINO_ARCH_AT32

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"
#include <Servo.h> // AT32 core's Servo class (uses tmr_type directly)

static uint8_t  servoCount = 0;
static MarlinServo* servos[NUM_SERVOS] = {nullptr};

constexpr uint32_t servoDelays[] = SERVO_DELAY;
static_assert(COUNT(servoDelays) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

MarlinServo::MarlinServo() {
  this->channel = servoCount++;
  servos[this->channel] = this;
}

int8_t MarlinServo::attach(const pin_t apin) {
  if (apin >= 0) this->pin = apin;
  if (this->pin < 0) return -1;
  return this->_servo.attach(this->pin) ? 0 : -1;
}

void MarlinServo::detach() { this->_servo.detach(); }

bool MarlinServo::attached() { return this->_servo.attached_state(); }

void MarlinServo::write(servo_angle_t angle) {
  this->angle = angle;
  this->_servo.write(angle);
}

void MarlinServo::move(servo_angle_t angle) {
  if (attach(-1) < 0) return;
  write(angle);
  safe_delay(servoDelays[this->channel]);
  TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
}

servo_angle_t MarlinServo::read() {
  return TERN(OPTIMISTIC_SERVO_READ, this->angle, (servo_angle_t)this->_servo.read());
}

#endif // HAS_SERVOS
#endif // ARDUINO_ARCH_AT32
