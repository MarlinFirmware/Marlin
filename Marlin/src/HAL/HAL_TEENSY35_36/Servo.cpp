#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

uint8_t servoPin[MAX_SERVOS] = { 0 };

int8_t libServo::attach(const int pin) {
  if (this->servoIndex >= MAX_SERVOS) return -1;
  if (pin > 0) servoPin[this->servoIndex] = pin;
  return Servo::attach(servoPin[this->servoIndex]);
}

int8_t libServo::attach(const int pin, const int min, const int max) {
  if (pin > 0) servoPin[this->servoIndex] = pin;
  return Servo::attach(servoPin[this->servoIndex], min, max);
}

void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (this->attach(0) >= 0) {
    this->write(value);
    safe_delay(servo_delay[this->servoIndex]);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      this->detach();
    #endif
  }
}

#endif // HAS_SERVOS

#endif // __MK64FX512__ || __MK66FX1M0__
