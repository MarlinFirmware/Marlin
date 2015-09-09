#include "AutoLevelManager.h"

AutoLevelManager::AutoLevelManager()
	: Subject<bool>()
	, m_state(false)
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;
	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy != 0)
	{
		state(true);
	}
}

void AutoLevelManager::setState()
{
	//Switch current status
	AutoLevelManager::single::instance().state(!AutoLevelManager::single::instance().state());
}

void AutoLevelManager::state(bool state)
{
	WriteToEEPROM(state);
	m_state = state;
	notify();
}

const bool & AutoLevelManager::state() const
{
	return m_state;
}

bool AutoLevelManager::ReadFromEEPROM()
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

void AutoLevelManager::WriteToEEPROM(bool state)
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;
	if(state)
	{
		dummy = 1;
	}
	_EEPROM_writeData(i, (uint8_t*)&dummy, sizeof(dummy));
}

void AutoLevelManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
