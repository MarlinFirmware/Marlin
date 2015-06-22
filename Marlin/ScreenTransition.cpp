#include "ScreenTransition.h"

namespace screen
{
	ScreenTransition::ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr)
		: Screen(title, DIALOG)
		, Functor<void>(fptr)
		, m_message(message)
		, m_box(box)
	{ }

	ScreenTransition::~ScreenTransition()
	{ }

	void ScreenTransition::init()
	{
		draw();
		this->action();
		ViewManager::getInstance().activeView(m_next_screen);
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
	}
}
