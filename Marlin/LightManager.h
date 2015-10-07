#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class LightManager : public Subject<bool>
{
	public:
		typedef Singleton<LightManager> single;

	public:
		LightManager();

		void state(bool state);
		bool state();
		void notify();

		static void setState();

	private:
		bool m_state;
};

#endif //LIGHT_MANAGER_H
