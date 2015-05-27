#include "Steppers.h"

Steppers::Steppers()
	: Subject<bool>()
	, m_state(false)
{ }

void Steppers::setState(bool state)
{
	m_state = state;
	notify();
}

bool Steppers::getState()
{
	return m_state;
}

void Steppers::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}