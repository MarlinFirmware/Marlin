#ifndef STEPPERS_H
#define STEPPERS_H

#include "Subject.h"

class Steppers : public Subject<bool>
{
	public:
		Steppers();

		void setState(bool state);

		bool getState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //STEPPERS_H