#include "LightManager.h"

#include <Arduino.h>
#include "Configuration.h"

LightManager::LightManager()
	: Subject<bool>()
{ 
	pinMode(LIGHT_PIN, OUTPUT);
	state(ReadFromEEPROM());
}

void LightManager::setState()
{
	//Switch current status
	LightManager::single::instance().state(!LightManager::single::instance().state());
}

void LightManager::state(bool state)
{
	digitalWrite(LIGHT_PIN, state);
	WriteToEEPROM(state);
	m_state = state;
	notify();
}

bool LightManager::state()
{
	return m_state;
}

bool LightManager::ReadFromEEPROM()
{
	int i = EEPROM_POS;
	int dummy = 0;

	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy == 1)
	{
		return true;
	}

	return false;
}

void LightManager::WriteToEEPROM(bool state)
{
	int i = EEPROM_POS;
	int dummy = 0;
	if(state)
	{
		dummy = 1;
	}
	_EEPROM_writeData(i, (uint8_t*)&dummy, sizeof(dummy));
}

void LightManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
