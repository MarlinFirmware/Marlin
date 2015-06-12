#ifndef STEPPERS_MANAGER_H
#define STEPPERS_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

class SteppersManager : public Subject<bool>
{
	public:
		typedef Singleton<SteppersManager> single;

	public:
		SteppersManager();

		void state(bool state);
		bool state();

      static void setState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //STEPPERS_MANAGER_H
