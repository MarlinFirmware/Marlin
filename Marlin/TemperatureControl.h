#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <stdint.h>

class TemperatureControl 
{
	public:
		TemperatureControl();
		~TemperatureControl();

		void setTargetControl(uint16_t target);
		uint16_t getTargetControl();
		void manageControl();

	public:
		float m_control_output;

	private:
		float m_current_temperature;
		uint16_t m_target_temperature;	
		float m_kp;
		float m_ki;
		float m_kb;	

};

#endif //TEMPERATURE_CONTROL_H
