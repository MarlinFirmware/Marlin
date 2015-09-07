#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <stdint.h>

class TemperatureControl 
{
	public:
		TemperatureControl();
		~TemperatureControl();

		void setTargetControl(uint16_t const & target);
		const uint16_t & getTargetControl() const;
		void manageControl();

	public:
		float m_control_output;

	private:
		float m_current_temperature;
		uint16_t m_target_temp;	
		float m_kp;
		float m_ki;
		float m_kb;	

};

#endif //TEMPERATURE_CONTROL_H
