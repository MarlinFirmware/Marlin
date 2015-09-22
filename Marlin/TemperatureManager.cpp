#include <math.h>

#include "TemperatureManager.h"
#include "temperature.h"
#include "Marlin.h"

namespace temp
{
	TemperatureManager::TemperatureManager()
		: Subject<float>()
		, m_current_temperature(0)
		, m_target_temperature(0)
		, m_blower_control(true)
	{
	#ifdef FAN_BLOCK_PIN
		pinMode(FAN_BLOCK_PIN, OUTPUT);
		digitalWrite(FAN_BLOCK_PIN, LOW);
	#endif //FAN_BLOCK_PIN
	}

	void TemperatureManager::updateCurrentTemperature(float temp)
	{
		if (m_current_temperature != temp)
		{
			m_current_temperature = temp;
			notify();
		}
		fanControl();
	}

	uint16_t const & TemperatureManager::getCurrentTemperature()
	{
		m_round_temperature = round(m_current_temperature);
		return m_round_temperature;
	}

	void TemperatureManager::setTargetTemperature(uint16_t target)
	{
		m_target_temperature = target;
		target_temperature[0] = m_target_temperature;
	}

	uint16_t const & TemperatureManager::getTargetTemperature() const
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

	void TemperatureManager::setBlowerControlState(bool state)
	{
		m_blower_control = state;
	}

	void TemperatureManager::fanControl()
	{
		if (m_current_temperature > min_temp_cooling)
		{
		#ifdef FAN_BLOCK_PIN
			digitalWrite(FAN_BLOCK_PIN, HIGH);
		#endif //FAN_BLOCK_PIN
			if (m_target_temperature < min_temp_cooling)
			{
				if (m_blower_control == true)
				{
					fanSpeed = 255;
				}
			}
			else
			{
				if (m_blower_control == true)
				{
					fanSpeed = 0;
				}
			}
		}
		else
		{
		#ifdef FAN_BLOCK_PIN
			digitalWrite(FAN_BLOCK_PIN, LOW);
		#endif //FAN_BLOCK_PIN
			if (m_blower_control == true)
			{
				fanSpeed = 0;	
			}	
		}
	}
}