#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class LightManager : public Subject<uint8_t>
{
	public:
		typedef Singleton<LightManager> single;

	public:
		LightManager();

		void state(bool state);
		bool state();

		static void setMode();
		uint8_t getMode();

		void notify();

	private:
		void mode(uint8_t mode);
		uint8_t mode();

	private:
		bool m_state;
		uint8_t m_mode;
};

#endif //LIGHT_MANAGER_H
