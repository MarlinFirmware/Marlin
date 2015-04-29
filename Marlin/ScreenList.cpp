#include "ScreenList.h"

namespace screen
{
	ScreenList::ScreenList(const char * label)
		: Screen(label, LIST)
		, m_index(0)
		, m_num_list(0)
	{
	}

	ScreenList::~ScreenList()
	{
	}

	void ScreenList::left()
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

	void ScreenList::right()
	{
		if ( m_index == (m_num_list -1) )
		{
			m_index = 0;
        }
      else
		{
			++m_index;
		}
	}

	void ScreenList::draw()
	{
		SERIAL_ECHO("DRAW:");
		SERIAL_ECHO(m_label);
		SERIAL_ECHO("  (item ");
		SERIAL_ECHO(m_index);
		SERIAL_ECHOLN(") ");
	}

	Screen & ScreenList::press()
	{
		return * m_next_screen;
	}

   void ScreenList::add(Screen & component)
   {
      m_next_screen = &component;
   }
}
