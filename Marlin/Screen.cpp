#include "Screen.h"

namespace screen
{
	Screen::Screen(ScreenType_t const & type, const char * title)
		: m_type(type)
		, m_title(title)
	{
	}

	Screen::~Screen()
	{
	}

	const char * Screen::title() const
	{
		return m_title;
	}

	ScreenType_t const & Screen::type() const
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
}
