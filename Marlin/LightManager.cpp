#include "LightManager.h"

#include "StorageManager.h"

#include <Arduino.h>
#include "Configuration.h"

LightManager::LightManager()
	: Subject<bool>()
	, m_state(false)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	pinMode(LIGHT_PIN, OUTPUT);
#endif
	state(eeprom::StorageManager::single::instance().getLight());
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
	eeprom::StorageManager::single::instance().setLight(state);
	m_state = state;
	notify();
}

bool LightManager::state()
{
	return m_state;
}

void LightManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
