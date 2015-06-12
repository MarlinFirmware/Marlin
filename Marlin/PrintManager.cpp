#include "PrintManager.h"

#include "GuiAction.h"

PrintManager::PrintManager()
	: m_state(STOPPED)
{
	notify();
}

void PrintManager::startPrint()
{
	if (PrintManager::single::instance().state() != STOPPED)
	{
		return;
	}

	PrintManager::single::instance().state(PRINTING);
	action_start_print();
}

void PrintManager::stopPrint()
{
	if (PrintManager::single::instance().state() != PRINTING)
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

void PrintManager::togglePause()
{
	if (PrintManager::single::instance().state() == PRINTING)
	{
		PrintManager::single::instance().state(PAUSED);
		action_pause_print();
	}
	else if (PrintManager::single::instance().state() == PAUSED)
	{
		PrintManager::single::instance().state(PRINTING);
		action_resume_print();
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
