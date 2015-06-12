#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

class LightManager : public Subject<bool>
{
	public:
		typedef Singleton<LightManager> single;

	public:
		LightManager();

		void state(bool state);
		bool state();

		static void setState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //LIGHT_MANAGER_H
