#include "TemperatureManager.h"
#include "temperature.h"

TemperatureManager & TemperatureManager::getInstance()
{
	static TemperatureManager m_instance;
	return m_instance;
}

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_temperature(0)
{ }

void TemperatureManager::updateTemperature(float temp)
{
	m_temperature = temp;
	notify();
}

void TemperatureManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_temperature);
	}
}