#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <stdint.h>

#include "Singleton.h"

class TemperatureControl 
{
	public:
		typedef Singleton<TemperatureControl> single;

	public:
		TemperatureControl();

		void manageControl();

	public:
		float m_control_output;

	private:
		float m_current_temperature;
		uint16_t m_target_temperature;	
		float m_kp;
		float m_ki;
		float m_kb;	
		float m_control_output;

};

#endif //TEMPERATURE_CONTROL_H
