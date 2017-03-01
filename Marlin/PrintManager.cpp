///////////////////////////////////////////////////////////////////////////////
/// \file PrintManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the printer.
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

#include "PrintManager.h"

#include "Arduino.h"
#include "GuiAction.h"

#include "LightManager.h"
#include "StorageManager.h"
#include "TemperatureManager.h"
#include "SteppersManager.h"
#include "ViewManager.h"
#include "cardreader.h"
#include "GuiManager.h"
#include "StatsManager.h"

#define INACTIVITY_TIME_MINUTES 10

PrintManager::PrintManager()
	: m_state(STOPPED)
	, m_known_position(false)
	, m_inactivity_time(0)
	, m_inactivity_flag(true)
	, m_bed_missing_flag(false)
{
#ifdef FAN_BOX_PIN
	pinMode(FAN_BOX_PIN, OUTPUT);
	digitalWrite(FAN_BOX_PIN, LOW);
#endif //FAN_BOX_PIN
}

void PrintManager::state(PrinterState_t state)
{
	if( PrintManager::single::instance().state() != SERIAL_CONTROL )
	{
		m_state = state;
		notify();
	}
}

PrinterState_t PrintManager::state()
{
	return m_state;
}

uint32_t PrintManager::getInactivityTime()
{
	return m_inactivity_time;
}

bool PrintManager::getInactivityFlag()
{
	return m_inactivity_flag;
}

void PrintManager::resetInactivityTime()
{
	m_inactivity_time = millis() + 60000 * INACTIVITY_TIME_MINUTES;
}
void PrintManager::setInactivityFlag(bool state)
{
	m_inactivity_flag = state;
}

bool PrintManager::getKnownPosition()
{
	return m_known_position;
}

void PrintManager::setKnownPosition(bool state)
{
	m_known_position = state;
}

void PrintManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}

void PrintManager::startPrint()
{
	if (PrintManager::single::instance().state() != STOPPED)
	{
		return;
	}

	PrintManager::single::instance().state(PRINTING);
	action_start_print();
#ifdef BED_DETECTION
	if(PrintManager::single::instance().getBedMissingFlag() == true)
	{
		return;
	}
#endif // BED_DETECTION
	startTime();
}

void PrintManager::stopPrint()
{
	if ( (PrintManager::single::instance().state() != PRINTING) &&
	     (PrintManager::single::instance().state() != PAUSED) )
	{
		return;
	}

	PrintManager::single::instance().state(STOPPED);
	action_stop_print();
}

void PrintManager::resumePrint()
{
	if (PrintManager::single::instance().state() != PAUSED)
	{
		return;
	}

	PrintManager::single::instance().state(PRINTING);
	action_resume_print();
}

void PrintManager::pausePrint()
{
	if (PrintManager::single::instance().state() != PRINTING)
	{
		return;
	}

	PrintManager::single::instance().state(PAUSING);
	action_pause_print();
}

void PrintManager::endPrint()
{
	if ( (PrintManager::single::instance().state() != PRINTING) &&
	     (PrintManager::single::instance().state() != PAUSED) )
	{
		return;
	}

	PrintManager::single::instance().state(STOPPED);
	ui::ViewManager::getInstance().activeView(ui::screen_print_action_complete);
}

Time_t PrintManager::completePrint()
{
	PrintManager::single::instance().state(COMPLETE);
	return PrintManager::single::instance().m_printing_time;
}

void PrintManager::togglePause()
{
	if (PrintManager::single::instance().state() == PRINTING)
	{
		pausePrint();
	}
	else if (PrintManager::single::instance().state() == PAUSED)
	{
		resumePrint();
	}
	return;
}

Time_t PrintManager::printingTime()
{
	return PrintManager::single::instance().m_printing_time;
}

void PrintManager::startTime()
{
	PrintManager::single::instance().m_printing_time.hours = 0;
	PrintManager::single::instance().m_printing_time.minutes = 0;
	PrintManager::single::instance().m_printing_time.seconds = 0;
	PrintManager::single::instance().m_printing_time.milliseconds = 0;

	PrintManager::single::instance().m_printing_time_raw = millis();
}

void PrintManager::updateTime()
{
	if (PrintManager::single::instance().state() == PRINTING)
	{
		uint32_t local_ellapsed_time = (millis() - PrintManager::single::instance().m_printing_time_raw);
		PrintManager::single::instance().m_printing_time_raw = millis();

		PrintManager::single::instance().m_printing_time.milliseconds += local_ellapsed_time % 1000;
		local_ellapsed_time /= 1000;
		PrintManager::single::instance().m_printing_time.seconds += local_ellapsed_time % 60;
		local_ellapsed_time /= 60;
		PrintManager::single::instance().m_printing_time.minutes += local_ellapsed_time % 60;
		local_ellapsed_time /= 60;
		PrintManager::single::instance().m_printing_time.hours += local_ellapsed_time;

		if (PrintManager::single::instance().m_printing_time.milliseconds >= 1000)
		{
			PrintManager::single::instance().m_printing_time.milliseconds -= 1000;
			PrintManager::single::instance().m_printing_time.seconds++;
		}
		if (PrintManager::single::instance().m_printing_time.seconds >= 60)
		{
			PrintManager::single::instance().m_printing_time.seconds -= 60;
			PrintManager::single::instance().m_printing_time.minutes++;
		}
		if (PrintManager::single::instance().m_printing_time.minutes >= 60)
		{
			PrintManager::single::instance().m_printing_time.minutes -= 60;
			PrintManager::single::instance().m_printing_time.hours++;
		}
	}
	else
	{
		PrintManager::single::instance().m_printing_time_raw = millis();
	}
}

void PrintManager::resetInactivity()
{
	PrintManager::single::instance().resetInactivityTime();
	PrintManager::single::instance().setInactivityFlag(false);

	if(LightManager::single::instance().getMode() == eeprom::LIGHT_AUTO)
	{
		LightManager::single::instance().state(true);
	}
}

void PrintManager::updateInactivity()
{
	if ( PrintManager::single::instance().getInactivityFlag() == false )
	{
		if (millis() > PrintManager::single::instance().getInactivityTime())
		{
			PrintManager::single::instance().setInactivityFlag(true);
			PrintManager::single::instance().inactivityTriggered();
		}
	}
}

void PrintManager::inactivityTriggered()
{
	switch(m_state)
	{
		case COMPLETE:
			temp::TemperatureManager::single::instance().setTargetTemperature(0);
			temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
			SteppersManager::disableAllSteppers();
			if(LightManager::single::instance().getMode() == eeprom::LIGHT_AUTO)
			{
				LightManager::single::instance().state(false);
			}
			break;
		case STOPPED:
			level_plate_step = 0;
			temp::TemperatureManager::single::instance().setTargetTemperature(0);
			temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
			SteppersManager::disableAllSteppers();
			ui::ViewManager::getInstance().activeView(ui::screen_inactivity);
			if(LightManager::single::instance().getMode() == eeprom::LIGHT_AUTO)
			{
				LightManager::single::instance().state(false);
			}
			break;
		case SERIAL_CONTROL:
			if(!card.sdprinting)
			{			
				temp::TemperatureManager::single::instance().setTargetTemperature(0);
				temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
				SteppersManager::disableAllSteppers();
			}
			if(LightManager::single::instance().getMode() == eeprom::LIGHT_AUTO)
			{
				LightManager::single::instance().state(false);
			}
			break;
		case LEVELING:
		case READY:
		case HOMING:
		case PAUSING:
		case PAUSED:
		case PRINTING:
			if(LightManager::single::instance().getMode() == eeprom::LIGHT_AUTO)
			{
				LightManager::single::instance().state(false);
			}
			break;
		default:
			break;
	}
}

bool PrintManager::knownPosition()
{
	return PrintManager::single::instance().getKnownPosition();
}

void PrintManager::knownPosition(bool state)
{
	PrintManager::single::instance().setKnownPosition(state);
}

bool PrintManager::getBedMissingFlag()
{
	return m_bed_missing_flag;
}

void PrintManager::setBedMissingFlag(bool flag)
{
#ifdef BED_DETECTION
	m_bed_missing_flag = flag;
	if(m_bed_missing_flag == true)// && PrintManager::single::instance().state() != SERIAL)
	{
		if(PrintManager::single::instance().state() != SERIAL_CONTROL)
		{
			ui::ViewManager::getInstance().activeView(ui::screen_base_error);
		} 
		else
		{
			SERIAL_ERROR_START;
			SERIAL_ERRORLN(" Bed not detected");
			lcd_emergency_stop();
		}
	}
#endif // BED_DETECTION
}

bool PrintManager::checkPTFEMaintenance()
{
	if(StatsManager::single::instance().getPTFEMaintenanceFlag() == true)
	{
		StatsManager::single::instance().resetPTFEMaintenanceFlag();
		return true;
	}
	
	return false;
}
