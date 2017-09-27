#ifndef HAL_Servo_Teensy_h
#define HAL_Servo_Teensy_h

#include <Servo.h>

// Inherit and expand on the official library
class libServo : public Servo {
	public:
		int8_t attach(const int pin);
		int8_t attach(const int pin, const int min, const int max);
		void move(const int value);
	private:
	   uint16_t min_ticks;
	   uint16_t max_ticks;
	   uint8_t servoIndex;               // index into the channel data for this servo
};

#endif // HAL_Servo_Teensy_h
