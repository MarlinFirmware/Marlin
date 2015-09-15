#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class SerialManager : public Subject<bool>
{
	public:
		typedef Singleton<SerialManager> single;

	public:
		SerialManager();

		void state(bool state);
		bool state();
		void notify();

		static void setState();

	private:
		bool m_state;
};

#endif //SERIAL_MANAGER_H
