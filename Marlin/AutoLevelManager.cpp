#include "AutoLevelManager.h"

AutoLevelManager & AutoLevelManager::getInstance()
{
	static AutoLevelManager instance;
	return instance;
}

AutoLevelManager::AutoLevelManager()
	: Subject<bool>()
	, m_state(false)
{ }

AutoLevelManager::~AutoLevelManager()
{ }

void AutoLevelManager::setState()
{
	//Switch current status
	AutoLevelManager::getInstance().state(!AutoLevelManager::getInstance().state());
	AutoLevelManager::getInstance().notify();
}

void AutoLevelManager::state(bool state)
{
	m_state = state;
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
