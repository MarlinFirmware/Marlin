#include "TemperatureManager.h" 
#include "Configuration.h"

#ifdef DOGLCD
	#include "TemperatureControl.h"
#else
	#include "temperature.h"
#endif

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
{
	setTargetTemperature(0);
#ifdef FAN_BOX_PIN
	pinMode(FAN_BOX_PIN, OUTPUT);
	digitalWrite(FAN_BOX_PIN, HIGH);
#endif //FAN_BOX_PIN

#ifdef FAN_BLOCK_PIN
	pinMode(FAN_BLOCK_PIN, OUTPUT);
	digitalWrite(FAN_BLOCK_PIN, HIGH);
#endif //FAN_BLOCK_PIN
}

void TemperatureManager::updateCurrentTemperature(float temp)
{
	if (m_current_temperature != temp)
	{
		m_current_temperature = temp;
		notify();
	}
}

uint16_t TemperatureManager::getCurrentTemperature()
{
	return m_current_temperature;
}

void TemperatureManager::setTargetTemperature(uint16_t target)
{
	#ifdef DOGLCD
		m_control->setTargetControl(target);
	#else
		target_temperature[0] = target;
	#endif
}

uint16_t TemperatureManager::getTargetTemperature()
{
	#ifdef DOGLCD
		return m_control->getTargetControl();
	#else
		return target_temperature[0];
	#endif
}

void TemperatureManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_current_temperature);
	}
}
