#include "Observer.h"
#include "Subject.h"

Observer::Observer(Subject * model)
	: m_model(model)
{
	if (m_model != 0)
	{
		m_model->attach(this);
	}
}
