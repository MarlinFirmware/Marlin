#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"

class TemperatureControl;

class TemperatureManager : public Subject<float>
{
	public:
		typedef Singleton<TemperatureManager> single;

	public:
		TemperatureManager();
		~TemperatureManager();

		void init();
		void updateCurrentTemperature(float temp);
		uint16_t getCurrentTemperature();
		void setTargetTemperature(uint16_t target);
		const uint16_t getTargetTemperature();
		void notify();
		void manageTemperatureControl();

	private:
		float m_current_temperature;
		TemperatureControl * m_control;
};

#endif //TEMPERATURE_MANAGER_H
