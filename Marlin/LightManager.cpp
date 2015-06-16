#include "LightManager.h"

#include <Arduino.h>
#include "Configuration.h"

static const int LIGHT_EEPROM_POS = 504;

LightManager::LightManager()
	: Subject<bool>()
{ 
	m_state = ReadFromEEPROM();
	pinMode(LIGHT_PIN, OUTPUT);
	digitalWrite(LIGHT_PIN, m_state);
}

void LightManager::setState()
{
	//Switch current status
	LightManager::single::instance().state(!LightManager::single::instance().state());
}

void LightManager::state(bool state)
{
	m_state = state;
	digitalWrite(LIGHT_PIN, m_state);
	WriteToEEPROM(state);
	notify();
}

bool LightManager::state()
{
	return m_state;
}

bool LightManager::ReadFromEEPROM()
{
	int i = LIGHT_EEPROM_POS;
	int dummy = 0;
	m_state = false;
	_EEPROM_readData(i, (uint8_t*)&dummy, sizeof(dummy));
	if(dummy == 1)
	{
		m_state = true;
	}

	return m_state;
}

void LightManager::WriteToEEPROM(bool state)
{
	int i = LIGHT_EEPROM_POS;
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
