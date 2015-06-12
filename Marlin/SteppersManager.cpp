#include "SteppersManager.h"

#include <stdint.h>

#include "Configuration.h"

SteppersManager::SteppersManager()
	: Subject<bool>()
	, m_steppers_disabled(false)
{
	m_steppers[X_AXIS] = Stepper(X_ENABLE_PIN, X_ENABLE_ON);
	m_steppers[Y_AXIS] = Stepper(Y_ENABLE_PIN, Y_ENABLE_ON);
	m_steppers[Z_AXIS] = Stepper(Z_ENABLE_PIN, Z_ENABLE_ON);
	m_steppers[EXTRUDER] = Stepper(E0_ENABLE_PIN, E_ENABLE_ON);
}

void SteppersManager::disableAllSteppers()
{
	for (uint8_t i = 0; i < NUM_STEPPERS; i++)
	{
		SteppersManager::single::instance().m_steppers[i].disable();
	}

	SteppersManager::single::instance().m_steppers_disabled = true;
	SteppersManager::single::instance().notify();
}

void SteppersManager::enableStepper(Stepper_t stepper)
{
	m_steppers[stepper].enable();

	m_steppers_disabled = false;
	notify();
}

void SteppersManager::disableStepper(Stepper_t stepper)
{
	m_steppers[stepper].disable();

	m_steppers_disabled = true;
	for (uint8_t i = 0; i < NUM_STEPPERS; i++)
	{
		if (m_steppers[i].isEnabled() == true)
		{
			m_steppers_disabled = false;
		}
	}
	notify();
}

void SteppersManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_steppers_disabled);
	}
}
