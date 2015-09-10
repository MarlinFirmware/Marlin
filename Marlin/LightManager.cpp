#include "LightManager.h"

#include <Arduino.h>
#include "Configuration.h"

LightManager::LightManager()
	: Subject<bool>()
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	pinMode(LIGHT_PIN, OUTPUT);
#endif
	state(ReadFromEEPROM());
}

void LightManager::setState()
{
	//Switch current status
	LightManager::single::instance().state(!LightManager::single::instance().state());
}

void LightManager::state(bool state)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	digitalWrite(LIGHT_PIN, state);
#endif
	WriteToEEPROM(state);
	m_state = state;
	notify();
}

const bool & LightManager::state() const
{
	return m_state;
}

bool LightManager::ReadFromEEPROM()
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

void LightManager::WriteToEEPROM(bool state)
{
	int i = EEPROM_POS;
	uint8_t dummy = 0;
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
