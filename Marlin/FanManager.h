#ifndef FAN_MANAGER_H
#define FAN_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class FanManager : public Subject<bool>
{
	public:
		typedef Singleton<FanManager> single;

	public:
		FanManager();

		void state(bool state);
		bool state();
		void notify();

		static void toogleState();

	private:
		bool m_state;	
};

#endif //FAN_MANAGER_H