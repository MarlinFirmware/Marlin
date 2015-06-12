#include "SteppersManager.h"

SteppersManager::SteppersManager()
	: Subject<bool>()
	, m_state(false)
{ 
	notify();
}

void SteppersManager::setState()
{
	//Always disable steppers
	SteppersManager::single::instance().state(true);
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
