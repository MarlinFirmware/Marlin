#include "LightManager.h"

#include "StorageManager.h"

#include <Arduino.h>
#include "Configuration.h"

LightManager::LightManager()
	: Subject<uint8_t>()
	, m_state(false)
	, m_mode(0)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	pinMode(LIGHT_PIN, OUTPUT);
#endif

	if(mode() == eeprom::LIGHT_OFF)
	{
		state(false);
	}
	else
	{
		state(true);
	}
	notify();
}

void LightManager::setMode()
{
	uint8_t mode = 0;
	uint8_t m_num_modes = 3;

	mode = LightManager::single::instance().mode();
	mode = (mode + 1) % m_num_modes;

	LightManager::single::instance().mode(mode);

	if(mode == eeprom::LIGHT_OFF)
	{
		LightManager::single::instance().state(false);
	}
	else
	{
		LightManager::single::instance().state(true);
	}

	LightManager::single::instance().notify();
}

uint8_t LightManager::getMode()
{
	return LightManager::single::instance().mode();
}

void LightManager::state(bool state)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	digitalWrite(LIGHT_PIN, state);
#endif
	m_state = state;
}

bool LightManager::state()
{
	return m_state;
}

void LightManager::mode(uint8_t mode)
{
	eeprom::StorageManager::single::instance().setLight(mode);
	m_mode = mode;
}

uint8_t LightManager::mode()
{
	m_mode = eeprom::StorageManager::single::instance().getLight();
	return m_mode;
}

void LightManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_mode);
	}
}
