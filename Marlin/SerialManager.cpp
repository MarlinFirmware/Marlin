#include "SerialManager.h"

#include <Arduino.h>
#include "Configuration.h"

SerialManager::SerialManager()
	: Subject<bool>()
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;
	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy != 0)
	{
		state(true);
	}
}

void SerialManager::setState()
{
	//Switch current status
	SerialManager::single::instance().state(!SerialManager::single::instance().state());
}

void SerialManager::state(bool state)
{
	WriteToEEPROM(state);
	m_state = state;
	notify();
}

const bool & SerialManager::state() const
{
	return m_state;
}

bool SerialManager::ReadFromEEPROM()
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;

	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy == 1)
	{
		return true;
	}

	return false;
}

void SerialManager::WriteToEEPROM(bool state)
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;
	if(state)
	{
		dummy = 1;
	}
	_EEPROM_writeData(i, (uint8_t*)&dummy, sizeof(dummy));
}

void SerialManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
