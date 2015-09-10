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
		uint16_t getCurrentTemperature();
		void setTargetTemperature(uint16_t target);
		const uint16_t getTargetTemperature();
		void notify();
		void manageTemperatureControl();

	public:
		TemperatureControl * m_control;

	private:
		float m_current_temperature;
		LookUpTableEntry m_cache[4];
};

#endif //TEMPERATURE_MANAGER_H
