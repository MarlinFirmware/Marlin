#ifndef LIGHT_H
#define LIGHT_H

#include "Subject.h"

class Light : public Subject<bool>
{
	public:
		Light();

		void setState(bool state);

		bool getState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //LIGHT_H
