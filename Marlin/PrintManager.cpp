#include "PrintManager.h"

#include "Arduino.h"
#include "GuiAction.h"

PrintManager::PrintManager()
	: m_state(STOPPED)
{ }

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

	PrintManager::single::instance().state(PAUSED);
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

void PrintManager::state(PrinterState_t state)
{
	m_state = state;
	notify();
}

PrinterState_t PrintManager::state()
{
	return m_state;
}

void PrintManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
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
