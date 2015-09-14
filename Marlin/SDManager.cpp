#include "SDManager.h"

#include "cardreader.h"

SDManager::SDManager()
	: Subject<bool>()
	, m_is_inserted(false)
{ }

void SDManager::updateSDStatus()
{
	if (IS_SD_INSERTED)
	{
		SDManager::single::instance().setInserted(true);
	}
	else
	{
		SDManager::single::instance().setInserted(false);
	}
}

bool const & SDManager::isInserted() const
{
	return m_is_inserted;
}

void SDManager::setInserted(bool state)
{
	if (state != m_is_inserted)
	{
		m_is_inserted = state;
		if (m_is_inserted)
		{
			card.initsd();
		}
		notify();
	}
}

void SDManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_is_inserted);
	}
}
