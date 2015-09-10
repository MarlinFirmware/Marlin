#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"

class TemperatureManager : public Subject<float>
{
	public:
		typedef Singleton<TemperatureManager> single;

	public:
		TemperatureManager();

		void updateCurrentTemperature(float temp);
		uint16_t const & getCurrentTemperature();
		void setTargetTemperature(uint16_t target);
		uint16_t const & getTargetTemperature() const;
		void notify();

	private:
		float m_current_temperature;
		uint16_t m_round_temperature;
		uint16_t m_target_temperature;
};

#endif //TEMPERATURE_MANAGER_H
