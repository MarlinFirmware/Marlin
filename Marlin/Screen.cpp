#include "Screen.h"

namespace screen
{
	Screen::Screen(const char * title, ScreenType_t const & type, FuncPtr_t function)
		: m_title(title)
		, m_type(type)
		, m_function(function)
	{ }

	Screen::~Screen()
	{ }

	const char * Screen::title() const
	{
		return m_title;
	}

	Screen::ScreenType_t const & Screen::type() const
	{
		return m_type;
	}

	void Screen::icon(Icon & component)
	{
		if (m_num_icons < max_icons)
		{
			m_icons[m_num_icons] = &component;
			++m_num_icons;
		}
	}

	Icon & Screen::icon()
	{
		return * m_icons[0];
	}
}
