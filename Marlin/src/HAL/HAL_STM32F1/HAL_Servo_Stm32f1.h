#ifndef HAL_SERVO_STM32F1_H
#define HAL_SERVO_STM32F1_H


#include <../../libraries/Servo/src/Servo.h>

// Inherit and expand on the official library
class libServo : public Servo {
public:
    int8_t attach(int pin);
    int8_t attach(int pin, int min, int max);
    void move(int value);
private:
    uint16_t min_ticks;
    uint16_t max_ticks;
    uint8_t servoIndex;               // index into the channel data for this servo
};

#endif
