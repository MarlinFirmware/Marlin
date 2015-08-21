#include "TemperatureManager.h"
#include "temperature.h"

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
	, m_target_temperature(0)
	, m_control_input(0)
	, m_control_output(0)
{
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

float TemperatureManager::manageControl(float kp, float ki, float kb)
{
	float pTerm = 0;
	float iTerm = 0;
	static float bTerm = 0;
	float error = 0;
	static float control_output_temp = 0;
	static float temp_i = 0;

	m_control_input = constrain (getTargetTemperature(), 0, HEATER_0_MAXTEMP);

	if (m_control_input <= HEATER_0_MINTEMP)
	{
		m_control_output = 0;
		iTerm = 0;
	} 
	else
	{
		error = m_control_input - m_current_temperature;

		pTerm = kp * error;
		temp_i += error;
		iTerm = (ki + bTerm) * temp_i;
		control_output_temp = pTerm + iTerm;
		
		if ( control_output_temp >= PID_MAX )
		{
			m_control_output = PID_MAX;		
		}
		else if ( m_control_output < 0 )
		{
			m_control_output = 0;
		}

		bTerm = kb * ( control_output_temp - m_control_output );
	}

	return m_control_output;
}
