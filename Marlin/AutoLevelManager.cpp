#include "AutoLevelManager.h"

static const int CUSTOM_EEPROM_POS = 501;

AutoLevelManager::AutoLevelManager()
	: Subject<bool>()
	, m_state(false)
{
	m_state = ReadFromEEPROM();
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

bool AutoLevelManager::state()
{
	return m_state;
}

bool AutoLevelManager::ReadFromEEPROM()
{
	int i = CUSTOM_EEPROM_POS;
	int dummy = 0;

	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy == 1)
	{
		return true;
	}
	return false;
}

void AutoLevelManager::WriteToEEPROM(bool state)
{
	int i = CUSTOM_EEPROM_POS;
	int dummy = 0;
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
