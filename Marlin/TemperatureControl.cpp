#include "TemperatureControl.h"
#include "Configuration.h"

TemperatureControl::TemperatureControl()
	: m_current_temperature(0)
	, m_control_output(0)
	, m_target_temperature(0)
{
	m_kp = DEFAULT_Kp; 
	m_ki = DEFAULT_Ki * PID_dT;
	m_kb = DEFAULT_Kb * PID_dT;
}

TemperatureControl::~TemperatureControl()
{ }

void TemperatureControl::setTargetControl(uint16_t const & target)
{
	m_target_temperature = target;
}

const uint16_t & TemperatureControl::getTargetControl() const
{
	return m_target_temperature;
}

void TemperatureControl::manageControl()
{
	float pTerm = 0;
	static float iTerm = 0;
	static float bTerm = 0;
	float error = 0;
	uint16_t control_input = 0;
	float control_output = 0;

	static uint32_t prev_millis = millis();
	static uint16_t max_time = 0;

	control_input = constrain (m_target_temperature, 0, HEATER_0_MAXTEMP);

	if (control_input <= HEATER_0_MINTEMP)
	{
		m_control_output = 0;
		iTerm = 0;
	} 
	else
	{
		error = control_input - m_current_temperature;
		pTerm = m_kp * error;
		iTerm += error * m_ki + bTerm;
		control_output = pTerm + iTerm;
		
		if ( control_output >= PID_MAX )
		{
			m_control_output = PID_MAX;		
		}
		else if ( control_output <= 0 )
		{
			m_control_output = 0;
		}
		else
		{
			m_control_output = control_output;
		}

		bTerm = m_kb * ( m_control_output - control_output );
	}
	SERIAL_ECHO("PID_INPUT: ");
	SERIAL_ECHOLN(control_input);
	SERIAL_ECHO("PID_OUTPUT: ");
	SERIAL_ECHOLN(m_control_output);
	SERIAL_ECHO("Error: ");
	SERIAL_ECHOLN(error);
	SERIAL_ECHO("P_term: ");
	SERIAL_ECHOLN(pTerm);
	SERIAL_ECHO("I_term: ");
	SERIAL_ECHOLN(iTerm);
	SERIAL_ECHO("m_ki*error: ");
	SERIAL_ECHOLN(m_ki*error);
	SERIAL_ECHO("B_term: ");
	SERIAL_ECHOLN(bTerm);
	/*uint32_t time = millis() - prev_millis;
	prev_millis += time;
	if (time > max_time)
	{
		max_time = time;
	}
	SERIAL_ECHOLN("max_time: ");
	SERIAL_ECHOLN(max_time);	
	SERIAL_ECHOLN(" ");	*/
}
