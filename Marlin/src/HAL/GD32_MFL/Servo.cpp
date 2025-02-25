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

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

static uint_fast8_t servoCount = 0;
static libServo* servos[NUM_SERVOS] = {0};
constexpr millis_t servoDelay[] = SERVO_DELAY;
static_assert(COUNT(servoDelay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

// Initialize to the default timer priority. This will be overridden by a call from timers.cpp.
// This allows all timer interrupt priorities to be managed from a single location in the HAL.
static uint32_t servo_interrupt_priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 12, 0);

// This must be called after the MFL Servo class has initialized the timer.
// To be safe this is currently called after every call to attach().
static void fixServoTimerInterruptPriority() {
  NVIC_SetPriority(getTimerUpIRQ(TIMER_SERVO), servo_interrupt_priority);
}

// Default constructor for libServo class.
// Initializes the servo delay, pause state, and pause value.
// Registers the servo instance in the servos array.
libServo::libServo() : delay(servoDelay[servoCount]),
  was_attached_before_pause(false),
  value_before_pause(0) {
  servos[servoCount++] = this;
}

// Attaches a servo to a specified pin.
int8_t libServo::attach(const int pin) {
  if (servoCount >= MAX_SERVOS) return -1;
  if (pin > 0) servo_pin = pin;
  auto result = mflServo.attach(servo_pin);
  fixServoTimerInterruptPriority();
  return result;
}

// Attaches a servo to a specified pin with minimum and maximum pulse widths.
int8_t libServo::attach(const int pin, const int min, const int max) {
  if (servoCount >= MAX_SERVOS) return -1;
  if (pin > 0) servo_pin = pin;
  auto result = mflServo.attach(servo_pin, min, max);
  fixServoTimerInterruptPriority();
  return result;
}

// Moves the servo to a specified position.
void libServo::move(const int value) {
  if (attach(0) >= 0) {
    mflServo.write(value);
    safe_delay(delay);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
  }
}

// Pause the servo by detaching it and storing its current state.
void libServo::pause() {
  was_attached_before_pause = mflServo.attached();
  if (was_attached_before_pause) {
    value_before_pause = mflServo.read();
    mflServo.detach();
  }
}

// Resume a previously paused servo.
// If the servo was attached before the pause, this function re-attaches
// the servo and moves it to the position it was in before the pause.
void libServo::resume() {
  if (was_attached_before_pause) {
    attach();
    move(value_before_pause);
  }
}

// Pause all servos by stopping their timers.
void libServo::pause_all_servos() {
  for (auto& servo : servos)
    if (servo) servo->pause();
}

// Resume all paused servos by starting their timers.
void libServo::resume_all_servos() {
  for (auto& servo : servos)
    if (servo) servo->resume();
}

// Set the interrupt priority for the servo.
// @param preemptPriority The preempt priority level.
// @param subPriority The sub priority level.
void libServo::setInterruptPriority(uint32_t preemptPriority, uint32_t subPriority) {
  servo_interrupt_priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), preemptPriority, subPriority);
}

#endif // HAS_SERVOS
#endif // ARDUINO_ARCH_MFL
