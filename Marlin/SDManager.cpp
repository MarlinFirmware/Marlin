#include "SDManager.h"

#include "cardreader.h"

SDManager::SDManager()
	: Subject<SDState_t>()
	, m_state(SD_IS_NOT_INSERTED)
{ }

void SDManager::updateSDStatus()
{
	SDState_t temp;

	if (IS_SD_INSERTED)
	{
		temp = SD_IS_INSERTED;
	}
	else
	{
		temp = SD_IS_NOT_INSERTED;
	}

	if (temp != SDManager::single::instance().state())
	{
		SDManager::single::instance().state(temp);
	}
}

void SDManager::state(SDState_t state)
{
	m_state = state;
	notify();
}

SDState_t SDManager::state()
{
	return m_state;
}

void SDManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
