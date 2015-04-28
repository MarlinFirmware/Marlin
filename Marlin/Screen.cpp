#include "Screen.h"

namespace screen
{
	Screen::Screen(const char * label, ScreenType_t const & type)
		: m_label(label)
		, m_type(type)
	{
	}

	Screen::~Screen()
	{
	}

	const char * Screen::label() const
	{
		return m_label;
	}

	ScreenType_t const & Screen::type() const
	{
		return m_type;
	}

	void Screen::icon(Icon & component)
	{
		if (m_num_icons < max_icons)
		{
			max_icons[m_num_icons] = &component;
			++m_num_icons;
		}
	}
}
