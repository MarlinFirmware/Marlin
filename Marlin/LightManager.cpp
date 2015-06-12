#include "LightManager.h"

LightManager & LightManager::getInstance()
{
	static LightManager instance;
	return instance;
}

LightManager::LightManager()
	: Subject<bool>()
	, m_state(false)
{ 
	notify();
}

LightManager::~LightManager()
{ }

void LightManager::setState()
{
	//Switch current status
	LightManager::getInstance().state(!LightManager::getInstance().state());
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
