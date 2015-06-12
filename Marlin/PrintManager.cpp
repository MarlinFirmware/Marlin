#include "PrintManager.h"

#include "GuiAction.h"

PrintManager & PrintManager::getInstance()
{
	static PrintManager instance;
	return instance;
}

void PrintManager::startPrint()
{
	if (PrintManager::getInstance().state() != STOPPED)
	{
		return;
	}

	PrintManager::getInstance().state(PRINTING);
	action_start_print();
}

void PrintManager::stopPrint()
{
	if (PrintManager::getInstance().state() != PRINTING)
	{
		return;
	}

	PrintManager::getInstance().state(STOPPED);
	action_stop_print();
}

void PrintManager::resumePrint()
{
	if (PrintManager::getInstance().state() != PAUSED)
	{
		return;
	}

	PrintManager::getInstance().state(PRINTING);
	action_resume_print();
}

void PrintManager::pausePrint()
{
	if (PrintManager::getInstance().state() != PRINTING)
	{
		return;
	}

	PrintManager::getInstance().state(PAUSED);
	action_pause_print();
}

void PrintManager::togglePause()
{
	if (PrintManager::getInstance().state() == PRINTING)
	{
		PrintManager::getInstance().state(PAUSED);
		action_pause_print();
	}
	else if (PrintManager::getInstance().state() == PAUSED)
	{
		PrintManager::getInstance().state(PRINTING);
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

PrintManager::PrintManager()
	: m_state(STOPPED)
{ 
	notify();
}

PrintManager::~PrintManager()
{ }

void PrintManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}
