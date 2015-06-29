#include "ScreenTransition.h"

namespace screen
{
	ScreenTransition::ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr, Subject<PrinterState_t> * model)
		: Screen(title, DIALOG)
		, Functor<void>(fptr)
		, Observer<PrinterState_t>(model)
		, m_message(message)
		, m_box(box)
		, m_printing_status(PRINTING)
	{ }

	ScreenTransition::~ScreenTransition()
	{ }

	void ScreenTransition::init(uint16_t index)
	{
		draw();
		this->action();
		if (this->m_model == 0)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}	
	}

	void ScreenTransition::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.multiText(m_message);
			painter.box(m_box);
		} while ( painter.nextPage() );
		if (this->m_model != 0 && m_printing_status == PAUSED)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}
	void ScreenTransition::update(PrinterState_t value)
	{
		m_printing_status = value;
	}
}
