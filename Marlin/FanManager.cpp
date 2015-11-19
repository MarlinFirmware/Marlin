#include "FanManager.h"

#include "StorageManager.h"

FanManager::FanManager()
	: Subject<bool>()
	, m_state(false)
{
	state(eeprom::StorageManager::single::instance().getBoxFan());
}

void FanManager::toogleState()
{
	//Switch current status
	FanManager::single::instance().state(!FanManager::single::instance().state());
}

void FanManager::state(bool state)
{
	eeprom::StorageManager::single::instance().setBoxFan(state);
	m_state = state;
	notify();
}

bool FanManager::state()
{
	return m_state;
}

void FanManager::notify()
{
	if(this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}