#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

extern void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size);
extern void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size);

class LightManager : public Subject<bool>
{
	public:
		typedef Singleton<LightManager> single;

	public:
		LightManager();

		void state(bool state);
		const bool & state() const;
		void notify();

		static void setState();

	private:
		bool ReadFromEEPROM();
		void WriteToEEPROM(bool state);

	private:
		bool m_state;
		static const int EEPROM_POS = 505;
};

#endif //LIGHT_MANAGER_H
