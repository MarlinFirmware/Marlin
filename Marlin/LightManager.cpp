#include "LightManager.h"
#include "Marlin.h"

LightManager & LightManager::getInstance()
{
	static LightManager instance;
	return instance;
}

LightManager::LightManager()
	: Subject<bool>()
	, m_state(false)
{ }

LightManager::~LightManager()
{ }

void LightManager::setState()
{
SERIAL_ECHOLN("LightManager::setState()");
	//Switch current status
	LightManager::getInstance().state(!LightManager::getInstance().state());
	LightManager::getInstance().notify();
}

void LightManager::state(bool state)
{
SERIAL_ECHO("state(");
if (state)
	SERIAL_ECHOLN("true)");
else
	SERIAL_ECHOLN("false)");

	m_state = state;
}

bool LightManager::state()
{
	return m_state;
}

void LightManager::notify()
{
SERIAL_ECHOLN("LightManager::notify()");
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
