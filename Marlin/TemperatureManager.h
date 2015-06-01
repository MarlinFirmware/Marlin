#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Subject.h"

class TemperatureManager : public Subject<float>
{
	public:
		static TemperatureManager & getInstance();

		void updateCurrentTemperature(float temp);

		void setTargetTemperature(uint16_t target);
		uint16_t getTargetTemperature();

	protected:
		TemperatureManager();
		virtual ~TemperatureManager()
		{ };

	private:
		TemperatureManager(TemperatureManager const & orig);
		TemperatureManager & operator=(TemperatureManager & orig);

		void notify();

	private:
		float m_current_temperature;
		uint16_t m_target_temperature;
};

#endif //TEMPERATURE_MANAGER_H