#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Subject.h"

class LightManager : public Subject<bool>
{
	public:
		LightManager();

		static void setState(bool state, LightManager & instance);

		void state(bool state);
		bool state();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //LIGHT_MANAGER_H
