#if defined(__STM32F1__)


#include "HAL_Servo_Stm32f1.h"
#include "../../../src/inc/MarlinConfig.h"
#if HAS_SERVOS


int8_t libServo::attach(int pin) {
    if (this->servoIndex >= MAX_SERVOS) return -1;
    return Servo::attach(pin);
}

int8_t libServo::attach(int pin, int min, int max) {
    return Servo::attach(pin, min, max);
}

void libServo::move(int value) {
    if (this->attach(0) >= 0) {
        this->write(value);
        delay(SERVO_DELAY);
        #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
        this->detach();
        #endif
    }
}
#endif // HAS_SERVOS

#endif
