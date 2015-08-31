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

		float manageControl(float kp, float ki, float kb);

	private:

};

#endif //TEMPERATURE_CONTROL_H
