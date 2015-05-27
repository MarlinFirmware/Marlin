#include "Light.h"

Light::Light()
	: Subject<bool>()
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
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}