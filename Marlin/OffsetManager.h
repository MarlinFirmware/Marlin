#ifndef OFFSET_MANAGER_H
#define OFFSET_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

extern void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size);
extern void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size);

class OffsetManager : public Subject<float>
{
	public:
		typedef Singleton<OffsetManager> single;

	public:
		OffsetManager();

		void offset(float value);
		float offset();

		static void setOffset(uint16_t value);

	private:
		float ReadFromEEPROM();
		void WriteToEEPROM(float value);

		void notify();

	private:
		float m_offset;
};

#endif //OFFSET_MANAGER_H
