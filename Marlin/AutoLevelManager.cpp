#include "AutoLevelManager.h"

#include "StorageManager.h"

AutoLevelManager::AutoLevelManager()
	: Subject<bool>()
	, m_state(false)
{
	state(eeprom::StorageManager::single::instance().getAutoLevel());
}

void AutoLevelManager::setState()
{
	//Switch current status
	AutoLevelManager::single::instance().state(!AutoLevelManager::single::instance().state());
}

void AutoLevelManager::state(bool state)
{
	eeprom::StorageManager::single::instance().setAutoLevel(state);
	m_state = state;
	notify();
}

bool AutoLevelManager::state()
{
	return m_state;
}

void AutoLevelManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
