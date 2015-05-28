#ifndef STEPPERS_MANAGER_H
#define STEPPERS_MANAGER_H

#include "Subject.h"

class SteppersManager : public Subject<bool>
{
	public:
		SteppersManager();

		void setState(bool state);

		bool getState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //STEPPERS_MANAGER_H