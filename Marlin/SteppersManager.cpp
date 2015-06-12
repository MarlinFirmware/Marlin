#include "SteppersManager.h"

SteppersManager & SteppersManager::getInstance()
{
	static SteppersManager instance;
	return instance;
}

SteppersManager::SteppersManager()
	: Subject<bool>()
	, m_state(false)
{ 
	notify();
}

SteppersManager::~SteppersManager()
{ }

void SteppersManager::setState()
{
	//Always disable steppers
	SteppersManager::getInstance().state(true);
}

void SteppersManager::state(bool state)
{
	m_state = state;
	notify();
}

bool SteppersManager::state()
{
	return m_state;
}

void SteppersManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
