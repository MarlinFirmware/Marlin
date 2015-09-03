#include <stdint.h>

#include "SteppersManager.h"
#include "PrintManager.h"
#include "Configuration.h"

SteppersManager::SteppersManager()
	: Subject<bool>()
	, m_steppers_disabled(true)
{
	m_steppers[AXIS_X] = Stepper(X_ENABLE_PIN, X_ENABLE_ON);
	m_steppers[AXIS_Y] = Stepper(Y_ENABLE_PIN, Y_ENABLE_ON);
	m_steppers[AXIS_Z] = Stepper(Z_ENABLE_PIN, Z_ENABLE_ON);
	m_steppers[EXTRUDER] = Stepper(E0_ENABLE_PIN, E_ENABLE_ON);
}

void SteppersManager::disableAllSteppers()
{
	if (!SteppersManager::single::instance().m_steppers_disabled)
	{
		for (uint8_t i = 0; i < NUM_STEPPERS; i++)
		{
			SteppersManager::single::instance().m_steppers[i].disable();
		}
		PrintManager::knownPosition(false);
		SteppersManager::single::instance().m_steppers_disabled = true;
		SteppersManager::single::instance().notify();
	}
}

void SteppersManager::enableStepper(Stepper_t stepper)
{
	if ( !m_steppers[stepper].isEnabled() )
	{
		m_steppers[stepper].enable();
		m_steppers_disabled = false;
		notify();
	}
}

void SteppersManager::disableStepper(Stepper_t stepper)
{
	if ( m_steppers[stepper].isEnabled() )
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
		PrintManager::knownPosition(false);
		notify();
	}
}

void SteppersManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_steppers_disabled);
	}
}
