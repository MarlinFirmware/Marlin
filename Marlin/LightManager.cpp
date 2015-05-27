#include "LightManager.h"

LightManager::LightManager()
	: Subject<bool>()
	, m_state(false)
{ }

void LightManager::setState(bool state)
{
	m_state = state;
	notify();
}

bool LightManager::getState()
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