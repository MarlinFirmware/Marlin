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
#include "../platforms.h"

#ifdef __PLAT_NATIVE_SIM__

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

//#define DEBUG_SERVOS
#define DEBUG_OUT ENABLED(DEBUG_SERVOS)
#include "../../../core/debug_out.h"

uint8_t ServoCount = 0;                                // the total number of attached servos

Servo::Servo() {
  // Constructor stub
  DEBUG_ECHOLNPGM("Debug Servo: constructor");
  this->servoIndex = ServoCount++;                    // assign a servo index to this instance
}

uint8_t Servo::attach(int pin) {
  // Attach stub
  DEBUG_ECHOLNPGM("Debug Servo: attach to pin ", pin, " servo index ", this->servoIndex);
  return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max) {
  // Attach with min and max stub
  DEBUG_ECHOLNPGM("Debug Servo: attach to pin ", pin, " with min ", min, " and max ", max);
  if (pin > 0) servo_pin = pin;
  return this->servoIndex;
}

void Servo::detach() {
  // Detach stub
  DEBUG_ECHOLNPGM("Debug Servo: detach");
}

// If value is < 200 it is treated as an angle, otherwise as pulse width in microseconds
void Servo::write(int value) {
  if (value < MIN_PULSE_WIDTH) { // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    value = map(constrain(value, 0, 180), 0, 180, SERVO_MIN_US(min), SERVO_MAX_US(max));
  }
  writeMicroseconds(value);
  DEBUG_ECHOLNPGM("Debug Servo: write ", value);
}

void Servo::writeMicroseconds(int value) {
  // Simulate the servo movement
  this->value = value;
  hal.set_pwm_duty(pin_t(this->servo_pin), (float(value) / 20000) * UINT16_MAX, UINT16_MAX);
  DEBUG_ECHOLNPGM("Debug Servo: write microseconds ", value);
}

int Servo::read() {
  // Read stub
  DEBUG_ECHOLNPGM("Debug Servo: read ", this->value);
  return this->value;
}

int Servo::readMicroseconds() {
  // Read microseconds stub
  DEBUG_ECHOLNPGM("Debug Servo: read microseconds");
  return 0;
}

bool Servo::attached() {
  // Attached stub
  DEBUG_ECHOLNPGM("Debug Servo: attached");
  return false;
}

int Servo::move(const unsigned char cmd) {
  // Move stub
  DEBUG_ECHOLNPGM("Debug Servo: move ", cmd);
  write(cmd);
  return 0;
}

#endif // HAS_SERVOS
#endif // __PLAT_NATIVE_SIM__
