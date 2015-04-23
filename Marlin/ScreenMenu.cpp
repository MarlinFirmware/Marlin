#include "ScreenMenu.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * label)
		: Screen(label, MENU)
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
			m_index = 0;
      }
      else
		{
			++m_index;
		}
	}

	void ScreenMenu::draw()
	{
	}

	Screen & ScreenMenu::press(void (*action)(void))
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
