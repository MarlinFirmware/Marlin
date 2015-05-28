#include "ScreenTransition.h"

namespace screen
{
	ScreenTransition::ScreenTransition(const char * title, const char * text)
		: Screen(title, SELECTOR)
		, m_text(text)
		, m_num_item_added(0)
	{ }

	ScreenTransition::~ScreenTransition()
	{ }

	void ScreenTransition::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_text);
		} while ( painter.nextPage() );
	}

	Screen & ScreenTransition::press(Screen * parent_view)
	{
		return * m_back_screen;
	}

	void ScreenTransition::add(Screen & component)
	{
		if (m_num_item_added % 2)
		{
			m_next_screen = &component;
		}
		else
		{
			m_back_screen = &component;
		}
		m_num_item_added++;
	}
}