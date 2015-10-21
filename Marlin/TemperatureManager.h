#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"


namespace temp
{

	class TemperatureControl;


	// Temperature parameters
	const static uint8_t default_temp_change_filament = 200;
	const static uint8_t min_temp_cooling = 50;
	const static uint8_t min_temp_operation = 170;
	const static uint8_t max_temp_operation = 250;

	class TemperatureManager : public Subject<float>
	{
		public:
			typedef Singleton<TemperatureManager> single;

		struct LookUpTableEntry
		{
			LookUpTableEntry()
			: raw(0)
			, temperature(0)
			{ };

			short raw;
			short temperature;
		};

		public:
			TemperatureManager();
			~TemperatureManager();

			void init();

			void updateLUTCache();
			short getRawLUTCache(uint8_t index);
			short getTemperatureLUTCache(uint8_t index);

			void updateCurrentTemperature(float temp);
			void updateCurrentTemperatureRaw(uint16_t temp);

			uint16_t const & getCurrentTemperature();
			void setTargetTemperature(uint16_t target);
			uint16_t const & getTargetTemperature() const;
			void notify();
			void setBlowerControlState(bool state);
			void fanControl();

			void manageTemperatureControl();

		private:

		public:

			TemperatureControl * m_control;	

		private:
			float m_target_temperature;
			float m_current_temperature;
			uint16_t m_current_temperature_raw;

			bool m_blower_control;

			LookUpTableEntry m_cache[4];
			uint16_t m_round_temperature;
	};
}

#endif //TEMPERATURE_MANAGER_H
