#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"

namespace temp
{
	// Temperature parameters
	const static uint8_t default_temp_change_filament = 220;
	const static uint8_t min_temp_cooling = 50;
	const static uint8_t min_temp_operation = 170;
	const static uint8_t max_temp_operation = 250;

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

		public:

		private:
			float m_current_temperature;
			uint16_t m_round_temperature;
			uint16_t m_target_temperature;
	};
}

#endif //TEMPERATURE_MANAGER_H
