#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Subject.h"

class LightManager : public Subject<bool>
{
	public:
		LightManager();

		void setState(bool state);

		bool getState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //LIGHT_MANAGER_H
