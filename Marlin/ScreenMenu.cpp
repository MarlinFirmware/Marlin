#include "ScreenMenu.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * title)
		: Screen(MENU, title)
		, m_index(0)
		, m_num_items(0)
	{
	}

	ScreenMenu::~ScreenMenu()
	{
	}

	void ScreenMenu::left()
	{
		if (m_index == 0)
		{
			m_index = 0;
		}
		else
		{
			--m_index;
		}
	}

	void ScreenMenu::right()
	{
		if ( m_index == (m_num_items -1) )
		{
			m_index = m_num_items -1;
		}
		else
		{
			++m_index;
		}
	}

	void ScreenMenu::draw()
	{
		SERIAL_ECHO(m_title);
		SERIAL_ECHO(">>>");
		SERIAL_ECHO(" [");
		SERIAL_ECHO(m_items[m_index]->title());
		SERIAL_ECHOLN("] ");
	}

	Screen & ScreenMenu::press()
	{
		return * m_items[m_index];
	}

	void ScreenMenu::add(Screen & component)
	{
		if (m_num_items < max_items)
		{
			m_items[m_num_items] = &component;
			++m_num_items;
		}
	}
}
