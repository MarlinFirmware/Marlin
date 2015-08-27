#include "TemperatureManager.h"
#include "temperature.h"

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
	, m_target_temperature(0)
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
	static float iTerm = 0;
	static float bTerm = 0;
	float error = 0;
	uint16_t control_input = 0;
	float control_output = 0;
	float control_output_temp = 0;

	static uint32_t prev_millis = millis();
	static uint16_t max_time = 0;


	control_input = constrain (getTargetTemperature(), 0, HEATER_0_MAXTEMP);

	if (control_input <= HEATER_0_MINTEMP)
	{
		control_output = 0;
		iTerm = 0;
	} 
	else
	{
		error = control_input - m_current_temperature;
		pTerm = kp * error;
		iTerm += error * ki + bTerm;
		control_output_temp = pTerm + iTerm;
		
		if ( control_output_temp >= PID_MAX )
		{
			control_output = PID_MAX;		
		}
		else if ( control_output_temp <= 0 )
		{
			control_output = 0;
		}
		else
		{
			control_output = control_output_temp;
		}

		bTerm = kb * ( control_output - control_output_temp );
	}
	/*SERIAL_ECHO("PID_INPUT: ");
	SERIAL_ECHOLN(control_input);
	SERIAL_ECHO("PID_OUTPUT: ");
	SERIAL_ECHOLN(control_output);
	SERIAL_ECHO("Error: ");
	SERIAL_ECHOLN(error);
	SERIAL_ECHO("P_term: ");
	SERIAL_ECHOLN(pTerm);
	SERIAL_ECHO("I_term: ");
	SERIAL_ECHOLN(iTerm);
	SERIAL_ECHO("Ki*error: ");
	SERIAL_ECHOLN(Ki*error);
	SERIAL_ECHO("B_term: ");
	SERIAL_ECHOLN(bTerm);*/
	/*uint32_t time = millis() - prev_millis;
	prev_millis += time;
	if (time > max_time)
	{
		max_time = time;
	}
	SERIAL_ECHOLN("max_time: ");
	SERIAL_ECHOLN(max_time);	
	SERIAL_ECHOLN(" ");	*/

	return control_output;
}
