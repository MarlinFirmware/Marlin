#include "Light.h"
#include "Observer.h"

Light::Light ()
	: Subject()
	, m_state(false)
{ }

void Light::setState(bool state)
{
	m_state = state;
	notify();
}

bool Light::getState()
{ 
	return m_state;
}

void Light::notify()
{
	if (m_observer != 0)
	{
		m_observer->update(m_state);
	}
}
