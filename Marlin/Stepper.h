#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>

class Stepper
{
	public:
		Stepper() {}
		Stepper(uint8_t pin, bool polarity);
		~Stepper();

		bool isEnabled();

		void enable();
		void disable();

	private:
		bool m_enabled;

		uint8_t m_enable_pin;
		bool m_enable_polarity;
};

#endif //STEPPER_H