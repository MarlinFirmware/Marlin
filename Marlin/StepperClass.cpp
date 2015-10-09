#include "StepperClass.h"

#include <Arduino.h>

Stepper::Stepper(uint8_t pin, bool polarity)
	: m_enabled(false)
	, m_enable_pin(pin)
	, m_enable_polarity(polarity)
{ 
	disable();
}

Stepper::~Stepper()
{ }

bool Stepper::isEnabled()
{
	return m_enabled;
}

void Stepper::enable()
{
	digitalWrite(m_enable_pin, m_enable_polarity);
	m_enabled = true;
}

void Stepper::disable()
{
	digitalWrite(m_enable_pin, !m_enable_polarity);
	m_enabled = false;
}
