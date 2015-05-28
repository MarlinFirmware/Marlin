#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Subject.h"

class TemperatureManager : public Subject<float>
{
	public:
		static TemperatureManager & getInstance();

		void updateTemperature(float temp);

	protected:
		TemperatureManager();
		virtual ~TemperatureManager()
		{ };

	private:
		TemperatureManager(TemperatureManager const & orig);
		TemperatureManager & operator=(TemperatureManager & orig);

		void notify();

	private:
		float m_temperature;
};

#endif //TEMPERATURE_MANAGER_H