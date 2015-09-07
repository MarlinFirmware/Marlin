#include "TemperatureManager.h" 
#include "Configuration.h"
#include "temperature.h"
#ifdef DOGLCD
	#include "TemperatureControl.h"
#endif

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
	, m_control()
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
	m_control = new TemperatureControl();
}

TemperatureManager::~TemperatureManager()
{
	delete m_control;
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

const uint16_t TemperatureManager::getTargetTemperature()
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

void TemperatureManager::manageTemperatureControl()
{
	updateTemperaturesFromRawValues();
	#ifdef DOGLCD
		m_control->manageControl();
	#else
		manage_heater();
	#endif
}
