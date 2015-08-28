#include "PrintManager.h"

#include "Arduino.h"
#include "GuiAction.h"

#include "TemperatureManager.h"
#include "SteppersManager.h"
#include "ViewManager.h"

#define INACTIVITY_TIME_MINUTES 10

PrintManager::PrintManager()
	: m_state(STOPPED)
	, m_known_position(false)
	, m_inactivity_time(0)
	, m_inactivity_flag(true)
{ }

void PrintManager::state(PrinterState_t state)
{
	m_state = state;
	notify();
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
}

void PrintManager::updateInactivity()
{
	if ( (PrintManager::single::instance().getInactivityFlag() == false)
		&& (PrintManager::single::instance().state() == STOPPED) )
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
	TemperatureManager::single::instance().setTargetTemperature(0);
	SteppersManager::disableAllSteppers();

	screen::ViewManager::getInstance().activeView(screen::screen_inactivity);
}

bool PrintManager::knownPosition()
{
	return PrintManager::single::instance().getKnownPosition();
}

void PrintManager::knownPosition(bool state)
{
	PrintManager::single::instance().setKnownPosition(state);
}