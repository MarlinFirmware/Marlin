///////////////////////////////////////////////////////////////////////////////
/// \file SteppersManager.cpp
///
/// \author Ruy Garcia
///
/// \brief Management class for the stepper drivers.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "SteppersManager.h"
#include "PrintManager.h"
#include "Configuration.h"
#include "ViewManager.h"

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
	ui::ViewManager::getInstance().setLastFocus(7);
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
