#include "SteppersManager.h"

SteppersManager::SteppersManager()
	: Subject<bool>()
	, m_state(false)
{ }

void SteppersManager::setState(bool state)
{
	m_state = state;
	notify();
}

bool SteppersManager::getState()
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