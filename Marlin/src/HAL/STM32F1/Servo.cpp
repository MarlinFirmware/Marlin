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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

uint8_t ServoCount = 0;

#include "Servo.h"

//#include "Servo.h"

#include <boards.h>
#include <io.h>
#include <pwm.h>
#include <wirish_math.h>

/**
 * 20 millisecond period config. For a 1-based prescaler,
 *
 *    (prescaler * overflow / CYC_MSEC) msec = 1 timer cycle = 20 msec
 * => prescaler * overflow = 20 * CYC_MSEC
 *
 * This uses the smallest prescaler that allows an overflow < 2^16.
 */
#define MAX_OVERFLOW    UINT16_MAX //((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        20
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((uint16_t)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) uint16_t(map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  uint32_t(map((c),  0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a)    uint16_t(map((a),  minAngle, maxAngle, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW))
#define US_TO_ANGLE(us)    int16_t(map((us), SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, minAngle, maxAngle))

void libServo::servoWrite(uint8_t inPin, uint16_t duty_cycle) {
  #ifdef SERVO0_TIMER_NUM
    if (servoIndex == 0) {
      pwmSetDuty(duty_cycle);
      return;
    }
  #endif

  timer_dev *tdev = PIN_MAP[inPin].timer_device;
  uint8_t tchan = PIN_MAP[inPin].timer_channel;
  if (tdev) timer_set_compare(tdev, tchan, duty_cycle);
}

libServo::libServo() {
  servoIndex = ServoCount < MAX_SERVOS ? ServoCount++ : INVALID_SERVO;
  timer_set_interrupt_priority(SERVO0_TIMER_NUM, SERVO0_TIMER_IRQ_PRIO);
}

bool libServo::attach(const int32_t inPin, const int32_t inMinAngle, const int32_t inMaxAngle) {
  if (servoIndex >= MAX_SERVOS) return false;
  if (inPin >= BOARD_NR_GPIO_PINS) return false;

  minAngle = inMinAngle;
  maxAngle = inMaxAngle;
  angle = -1;

  #ifdef SERVO0_TIMER_NUM
    if (servoIndex == 0 && setupSoftPWM(inPin)) {
      pin = inPin; // set attached()
      return true;
    }
  #endif

  if (!PWM_PIN(inPin)) return false;

  timer_dev *tdev = PIN_MAP[inPin].timer_device;
  //uint8_t tchan = PIN_MAP[inPin].timer_channel;

  SET_PWM(inPin);
  servoWrite(inPin, 0);

  timer_pause(tdev);
  timer_set_prescaler(tdev, SERVO_PRESCALER - 1); // prescaler is 1-based
  timer_set_reload(tdev, SERVO_OVERFLOW);
  timer_generate_update(tdev);
  timer_resume(tdev);

  pin = inPin; // set attached()
  return true;
}

bool libServo::detach() {
  if (!attached()) return false;
  angle = -1;
  servoWrite(pin, 0);
  return true;
}

int32_t libServo::read() const {
  if (attached()) {
    #ifdef SERVO0_TIMER_NUM
      if (servoIndex == 0) return angle;
    #endif
    timer_dev *tdev = PIN_MAP[pin].timer_device;
    uint8_t tchan = PIN_MAP[pin].timer_channel;
    return US_TO_ANGLE(COMPARE_TO_US(timer_get_compare(tdev, tchan)));
  }
  return 0;
}

void libServo::move(const int32_t value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

  if (attached()) {
    angle = constrain(value, minAngle, maxAngle);
    servoWrite(pin, US_TO_COMPARE(ANGLE_TO_US(angle)));
    safe_delay(servo_delay[servoIndex]);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
  }
}

#ifdef SERVO0_TIMER_NUM
  extern "C" void Servo_IRQHandler() {
    static timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
    uint16_t SR = timer_get_status(tdev);
    if (SR & TIMER_SR_CC1IF) { // channel 1 off
      #ifdef SERVO0_PWM_OD
        OUT_WRITE_OD(SERVO0_PIN, 1); // off
      #else
        OUT_WRITE(SERVO0_PIN, 0);
      #endif
      timer_reset_status_bit(tdev, TIMER_SR_CC1IF_BIT);
    }
    if (SR & TIMER_SR_CC2IF) { // channel 2 resume
      #ifdef SERVO0_PWM_OD
        OUT_WRITE_OD(SERVO0_PIN, 0); // on
      #else
        OUT_WRITE(SERVO0_PIN, 1);
      #endif
      timer_reset_status_bit(tdev, TIMER_SR_CC2IF_BIT);
    }
  }

  bool libServo::setupSoftPWM(const int32_t inPin) {
    timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
    if (!tdev) return false;
    #ifdef SERVO0_PWM_OD
      OUT_WRITE_OD(inPin, 1);
    #else
      OUT_WRITE(inPin, 0);
    #endif

    timer_pause(tdev);
    timer_set_mode(tdev, 1, TIMER_OUTPUT_COMPARE); // counter with isr
    timer_oc_set_mode(tdev, 1, TIMER_OC_MODE_FROZEN, 0); // no pin output change
    timer_oc_set_mode(tdev, 2, TIMER_OC_MODE_FROZEN, 0); // no pin output change
    timer_set_prescaler(tdev, SERVO_PRESCALER - 1); // prescaler is 1-based
    timer_set_reload(tdev, SERVO_OVERFLOW);
    timer_set_compare(tdev, 1, SERVO_OVERFLOW);
    timer_set_compare(tdev, 2, SERVO_OVERFLOW);
    timer_attach_interrupt(tdev, 1, Servo_IRQHandler);
    timer_attach_interrupt(tdev, 2, Servo_IRQHandler);
    timer_generate_update(tdev);
    timer_resume(tdev);

    return true;
  }

  void libServo::pwmSetDuty(const uint16_t duty_cycle) {
    timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
    timer_set_compare(tdev, 1, duty_cycle);
    timer_generate_update(tdev);
    if (duty_cycle) {
      timer_enable_irq(tdev, 1);
      timer_enable_irq(tdev, 2);
    }
    else {
      timer_disable_irq(tdev, 1);
      timer_disable_irq(tdev, 2);
      #ifdef SERVO0_PWM_OD
        OUT_WRITE_OD(pin, 1); // off
      #else
        OUT_WRITE(pin, 0);
      #endif
    }
  }

  void libServo::pauseSoftPWM() { // detach
    timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
    timer_pause(tdev);
    pwmSetDuty(0);
  }

#else

  bool libServo::setupSoftPWM(const int32_t inPin) { return false; }
  void libServo::pwmSetDuty(const uint16_t duty_cycle) {}
  void libServo::pauseSoftPWM() {}

#endif

#endif // HAS_SERVOS

#endif // __STM32F1__
