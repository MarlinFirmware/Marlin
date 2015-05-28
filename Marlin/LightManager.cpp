#include "LightManager.h"

LightManager::LightManager()
	: Subject<bool>()
{ }

void LightManager::setState(bool state, LightManager & instance)
{
	instance.state(state);
	instance.notify();
}

void LightManager::state(bool state)
{
	m_state = state;
}

bool LightManager::state()
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
