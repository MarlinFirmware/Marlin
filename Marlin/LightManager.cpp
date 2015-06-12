#include "LightManager.h"

LightManager::LightManager()
	: Subject<bool>()
	, m_state(false)
{ }

void LightManager::setState()
{
	//Switch current status
	LightManager::single::instance().state(!LightManager::single::instance().state());
}

void LightManager::state(bool state)
{
	m_state = state;
	notify();
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
