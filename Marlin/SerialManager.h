#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

extern void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size);
extern void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size);

class SerialManager : public Subject<bool>
{
	public:
		typedef Singleton<SerialManager> single;

	public:
		SerialManager();

		void state(bool state);
		const bool & state() const;
		void notify();

		static void setState();

	private:
		bool ReadFromEEPROM();
		void WriteToEEPROM(bool state);

	private:
		bool m_state;
		static const int EEPROM_POS = 509;
};

#endif //SERIAL_MANAGER_H
