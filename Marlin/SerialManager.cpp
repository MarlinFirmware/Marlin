#include "SerialManager.h"

#include "StorageManager.h"

#include <Arduino.h>
#include "Configuration.h"

SerialManager::SerialManager()
	: Subject<bool>()
	, m_state(false)
{
	state(eeprom::StorageManager::single::instance().getSerialScreen());
}

void SerialManager::setState()
{
	//Switch current status
	SerialManager::single::instance().state(!SerialManager::single::instance().state());
}

void SerialManager::state(bool state)
{
	eeprom::StorageManager::single::instance().setSerialScreen(state);
	m_state = state;
	notify();
}

bool SerialManager::state()
{
	return m_state;
}

void SerialManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
