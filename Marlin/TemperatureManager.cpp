#include "TemperatureManager.h"
#include "temperature.h"

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
	, m_target_temperature(0)
{ }

void TemperatureManager::updateCurrentTemperature(float temp)
{
	m_current_temperature = temp;
	notify();
}

void TemperatureManager::setTargetTemperature(uint16_t target)
{
	m_target_temperature = target;
	target_temperature[0] = m_target_temperature;
}

uint16_t TemperatureManager::getTargetTemperature()
{
	return m_target_temperature;
}

void TemperatureManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_current_temperature);
	}
}
